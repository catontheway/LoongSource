#include "StdAfx.h"
#include ".\WaterReflectEffect.h"
#include "..\SceneGraph\SceneGraph.h"
#include "..\Terrain\SGRiverPatch.h"
#include "..\Kernel.h"
#include "..\Device\IRenderTexture.h"
#include <list>
using namespace std;

namespace Cool3D
{
	//--class tagReflectMap----------------------------------------------------------
	struct tagReflectMap						//����ͼ��Ϣ
	{
		float				surfaceHeight;		//ˮ��߶�
		int					indexRT;			//RT����
		vector<SceneNode*>	sceneNodes;			//��غ���sceneNode
		float				distToCam;			//�뾵ͷ����

		void CalcDistToCam(CameraBase* pCamera)
		{
			distToCam=FLOAT_MAX;
			for(int i=0; i<(int)sceneNodes.size(); i++)
			{
				AABBox box;
				sceneNodes[i]->GetBox(box);

				float dist=box.DistToPoint(pCamera->GetPos());
				if(dist<distToCam)				
					distToCam=dist;
			}			
		}

		void CloseReflectEffect(bool bClose)
		{
			for(int i=0; i<(int)sceneNodes.size(); i++)
			{
				SceneNode* pNode=sceneNodes[i];
				if(pNode->IS_STATIC_CLASS(SGRiverPatch))
					((SGRiverPatch*)pNode)->CloseReflectEffect(bClose);
			}			
		}
	};


	//--class Member-----------------------------------------------------------------
	class WaterReflectEffect::Member
	{
	public:
		static const int NUM_RT=8;					//��������ʱ������RT����
		
		struct tagRenderTex							//RenderTex��Ϣ
		{
			IRenderTexture*		pRT;				//RTָ��
			int					size;				//�ߴ�
			bool				bUsed;				//�Ƿ�ռ��
		};

	public:
		tagRenderTex			m_renderTex[NUM_RT];//��������ʱ�ʹ����õ�RT
		list<tagReflectMap>		m_reflectMaps;		//����ͼ�б�
		SceneGraph*				m_pSG;				//SceneGraph
		EEffectLevel			m_effectLevel;		//Ч������

	public:
		Member()
		{
			m_effectLevel=EL_High;
		}

		void Init()
		{
			for(int i=0;i<NUM_RT;i++)
			{
				int size=0;
				if(i<2)
					size=512;
				else if(i<5)
					size=256;
				else
					size=128;

				m_renderTex[i].pRT=Device()->NewRenderTexture();
				m_renderTex[i].pRT->Create(size,size,EPF_R5G6B5);
				m_renderTex[i].size=size;
				m_renderTex[i].bUsed=false;
			}
		}

		void Destroy()
		{
			for(int i=0;i<NUM_RT;i++)
			{
				delete m_renderTex[i].pRT;
			}
		}

		void BeginAttachNode()
		{
			m_reflectMaps.clear();
			for(int i=0;i<NUM_RT;i++)
			{
				m_renderTex[i].bUsed=false;
			}

		}

		class SortByDist
		{
		public:
			bool operator()(const tagReflectMap& p1,const tagReflectMap& p2)
			{
				return p1.distToCam<p2.distToCam;
			}
		};

		void EndAttachNode()
		{
			//--���㷴��ͼ��Ӧ��SceneNode�뾵ͷ����
			list<tagReflectMap>::iterator iter;
			for(iter=m_reflectMaps.begin();
				iter!=m_reflectMaps.end();iter++)
			{
				iter->CalcDistToCam(m_pSG->GetCamera());
			}

			//--�ɽ���Զ����
			SortByDist sortObj;
			m_reflectMaps.sort(sortObj);

			//--�ɽ���Զ����RT
			for(iter=m_reflectMaps.begin();
				iter!=m_reflectMaps.end();)
			{
				int index=AssignRT(iter->distToCam);
				if(index!=-1)
				{
					iter->indexRT=index;
					iter->CloseReflectEffect(false);
					iter++;
				}
				else
				{
					iter->CloseReflectEffect(true);
					iter=m_reflectMaps.erase(iter);
				}
			}
		}

		list<tagReflectMap>::iterator FindReflectMap(float surfaceHeight)
		{
			list<tagReflectMap>::iterator iter;
			for(iter=m_reflectMaps.begin();
				iter!=m_reflectMaps.end(); iter++)
			{
				if(fabs(iter->surfaceHeight-surfaceHeight)<1.0f)
					break;
			}
			return iter;
		}

		int AssignRT(float distToCam)
		{
			//--
			int bestSize=0;
			if(m_effectLevel==EL_Low)
			{
				if(distToCam<10000.0f)
					bestSize=256;
				else if(distToCam<15000.0f)
					bestSize=128;
				else
					return -1;
			}
			else
			{
				if(distToCam<5000.0f)
					bestSize=512;
				else if(distToCam<10000.0f)
					bestSize=256;
				else if(distToCam<15000.0f)
					bestSize=128;
				else
					return -1;
			}

			//--
			for(int i=0;i<NUM_RT;i++)
			{
				if(m_renderTex[i].size<=bestSize
					&& m_renderTex[i].bUsed==false)
				{
					m_renderTex[i].bUsed=true;
					return i;
				}
			}
			return -1;
		}

		void AttachNode(SceneNode* pSceneNode,float surfaceHeight)
		{
			if(surfaceHeight>=m_pSG->GetCamera()->GetPos().y)//���Կ�������ˮ��
				return;

			list<tagReflectMap>::iterator iter=FindReflectMap(surfaceHeight);
			if(iter!=m_reflectMaps.end())
			{
				iter->sceneNodes.push_back(pSceneNode);
			}
			else
			{
				tagReflectMap reflectMap;
				reflectMap.surfaceHeight = surfaceHeight;
				reflectMap.sceneNodes.push_back(pSceneNode);
				m_reflectMaps.push_back(reflectMap);
			}
		}

		void Render()
		{
			if(m_reflectMaps.empty())
				return;
			
			list<tagReflectMap>::iterator iter;
			for(iter=m_reflectMaps.begin();
				iter!=m_reflectMaps.end(); iter++)
			{
				Render(*iter);
			}
		}

		void Render(const tagReflectMap& reflectMap)
		{
			CameraBase* pCam=m_pSG->GetCamera();
			IDevice* pDev=Device();

			//--��ˮ���¶Գ�λ�÷���һ�������
			CameraBase newCamera;
			newCamera.SetPerspective((float)pDev->GetClientWidth(),(float)pDev->GetClientHeight(),
				pCam->GetFOV(),pCam->GetZNear(),pCam->GetZFar());

			Vector3 from=pCam->GetPos();
			Vector3 lookAt=pCam->GetLookat();
			from.y=reflectMap.surfaceHeight-(from.y-reflectMap.surfaceHeight);
			lookAt.y=reflectMap.surfaceHeight-(lookAt.y-reflectMap.surfaceHeight);
			newCamera.SetLookAt(from,lookAt);

			//--��Ⱦ����ͼ
			bool bRenderTrnOnly=m_effectLevel==EL_Low;
			m_pSG->RenderReflectMap(&newCamera,m_renderTex[reflectMap.indexRT].pRT,reflectMap.surfaceHeight,bRenderTrnOnly);
			//m_renderTex[reflectMap.indexRT].pRT->WriteToFile(_T("d:\\test.tga"));

			//--������ͼͶӰ��ˮ��
			for(int i=0;i<(int)reflectMap.sceneNodes.size();i++)
			{
				SceneNode* pSceneNode=reflectMap.sceneNodes[i];
				if(pSceneNode->IS_STATIC_CLASS(SGRiverPatch))
				{
					((SGRiverPatch*)pSceneNode)->ProjectReflectMap(&newCamera,m_renderTex[reflectMap.indexRT].pRT,m_pSG);
				}
			}
		}
	};



	//--class WaterReflectEffect-----------------------------------------------------
	WaterReflectEffect::WaterReflectEffect(void)
	{
		m_p=new Member;
		m_bEnable=false;
	}

	WaterReflectEffect::~WaterReflectEffect(void)
	{
		delete m_p;
	}

	void WaterReflectEffect::Init()
	{
		if(!IsHWSurpport())
			return;

		m_p->Init();
	}

	void WaterReflectEffect::Destroy()
	{
		if(!IsHWSurpport())
			return;

		m_p->Destroy();
	}

	WaterReflectEffect* WaterReflectEffect::Inst()
	{
		static WaterReflectEffect Inst;
		return &Inst;
	}

	void WaterReflectEffect::Begin(SceneGraph* pSG)
	{
		if(!m_bEnable)
			return;

		m_p->m_pSG=pSG;
		m_p->BeginAttachNode();
	}

	void WaterReflectEffect::AttachNode(SceneNode* pSceneNode)
	{
		if(!m_bEnable)
			return;

		if(!pSceneNode->IS_STATIC_CLASS(SGRiverPatch))
			return;

		SGRiverPatch* pRiverPatch=(SGRiverPatch*)pSceneNode;
		if(!pRiverPatch->IsEnableReflectEffect())//��������Ч��
			return;

		m_p->AttachNode(pSceneNode,pRiverPatch->GetSurfaceHeight());
	}

	void WaterReflectEffect::End()
	{
		if(!m_bEnable)
			return;

		m_p->EndAttachNode();
		m_p->Render();
	}

	bool WaterReflectEffect::IsHWSurpport()
	{
		IDevice* pDev=Device();
		return pDev->CapsMaxTextureStages()>=4
				&& pDev->FCapsMaxSimultaneousTextures()>=3
				&& pDev->CapsBumpEnvMap()
				&& pDev->CapsTexProjected();
	}

	void WaterReflectEffect::Enable(bool bEnable)
	{
		if(bEnable && !IsHWSurpport())//Ӳ����֧��ʱ�޷�����Ч��
			return;

		m_bEnable=bEnable;
	}

	void WaterReflectEffect::SetEffectLevel(EEffectLevel level)
	{
		m_p->m_effectLevel=level;
	}

	WaterReflectEffect::EEffectLevel WaterReflectEffect::GetEffectLevel()
	{
		return m_p->m_effectLevel;
	}

}//end namespace Cool3D