#include "StdAfx.h"
#include ".\WaterReflectMapRender.h"
#include "..\SceneGraph\SceneGraph.h"
#include "..\Terrain\SGRiverPatch.h"
#include "..\Kernel.h"
#include "..\Device\IRenderTexture.h"
#include "..\Math\Transform.h"
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
		UINT				riverID;
		Matrix4				mat;

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

	};


	//--class Member-----------------------------------------------------------------
	class WaterReflectMapRender::Member
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
					iter++;
				}
				else
				{
					iter=m_reflectMaps.erase(iter);
				}
			}
		}

		list<tagReflectMap>::iterator FindReflectMap(UINT riverID)
		{
			list<tagReflectMap>::iterator iter=m_reflectMaps.begin();
			for(; iter!=m_reflectMaps.end(); iter++)
			{
				if(iter->riverID==riverID)
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
				else if(distToCam<30000.0f)
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
				else if(distToCam<30000.0f)
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

		void AttachNode(SceneNode* pSceneNode)
		{
			ASSERT(pSceneNode->IS_STATIC_CLASS(SGRiverPatch));

			SGRiverPatch *pRiverPatch=(SGRiverPatch*)pSceneNode;
			if(pRiverPatch->GetSurfaceHeight()>=m_pSG->GetCamera()->GetPos().y)//���Կ�������ˮ��
				return;

			list<tagReflectMap>::iterator iter=FindReflectMap(pRiverPatch->GetRiverID());
			if(iter!=m_reflectMaps.end())
			{
				iter->sceneNodes.push_back(pSceneNode);
			}
			else
			{
				tagReflectMap reflectMap;
				reflectMap.surfaceHeight = pRiverPatch->GetSurfaceHeight();
				reflectMap.sceneNodes.push_back(pSceneNode);
				reflectMap.riverID=pRiverPatch->GetRiverID();
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

		void CreateViewPrjMat(CameraBase *pCamera,Matrix4& matViewPrj)
		{
			/* 
			�������ܣ�	    ��������ͼ���ӳ�õ�Ƭһ��ͶӰ��ˮ�棬ˮ���µľ�ͷ���Ƿ�ӳ��

			ʵ�ַ�����		�޸�RWaterMesh����uv����ֵΪ����ֵ��
							1,����world���꾭��ӳ��viewPrj����任���ٳ���w
							2,ת�����������꣺��0.5����0.5


			�������⣺	    ��Ӱ����Ť����
							ԭ����ÿ�����ص�w���ܲ�ͬ������ÿ�����ص�uvֵ�������Եģ�����
							ͨ�������ֵ�õ������뵥�����㡣

			���������		1,������uvֵ����Ϊ��ӳ��view�ռ�����ֵ
							2,����uv�任����Ϊ��ӳ��proj����
							3,����uv�任��־EUVT_Project��ʵ���������Զ���w�Ĺ���
							4,�������ˣ���0.5����0.5���������ģ�
							���ǣ�����Proj�������档
							ʲô����Ҳ�У�����ֵ�϶�����(��u�ļ���Ϊ��)��

							(x*0.5+0.5)/w != x/w*0.5+0.5

							�ðɣ���һ�£�

							(x*0.5+0.5*w)/w

							����dx�й���͸��ͶӰ�����壬���w����view�ռ��zֵ������uv�任����

							Matrix4 mat=*pCamera->GetPrj();
							mat._11*=0.5f;	//ʵ��x*0.5	
							mat._22*=-0.5f;	//ʵ��y*-0.5
							mat._31+=0.5f;  //ʵ��+0.5*w
							mat._32+=0.5f;	//ʵ��+0.5*w
			*/

			Transform trans;
			Matrix4 viewMat=*pCamera->GetView();
			trans.LoadMatrix(&viewMat);
			Matrix4 projMat=*pCamera->GetPrj();
			projMat._11*=0.5f;
			projMat._22*=-0.5f;
			projMat._31+=0.5f;
			projMat._32+=0.5f;
			trans.CombineTransRight(&projMat);
			matViewPrj=*trans.GetMatrix();
		}

		void Render(tagReflectMap& reflectMap)
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
			bool bRenderTrnOnly=(m_effectLevel==EL_Low || m_effectLevel==EL_Mid);
			m_pSG->RenderReflectMap(&newCamera,m_renderTex[reflectMap.indexRT].pRT,reflectMap.surfaceHeight,bRenderTrnOnly);
			//m_renderTex[reflectMap.indexRT].pRT->WriteToFile(_T("d:\\test.tga"));

			CreateViewPrjMat(&newCamera,reflectMap.mat);

			//--������ͼͶӰ��ˮ��
			for(int i=0;i<(int)reflectMap.sceneNodes.size();i++)
			{
				SceneNode* pSceneNode=reflectMap.sceneNodes[i];
				if(pSceneNode->IS_STATIC_CLASS(SGRiverPatch))
				{
					((SGRiverPatch*)pSceneNode)->SetReflectMap(m_renderTex[reflectMap.indexRT].pRT);
				}
			}
		}
	};



	//--class WaterReflectMapRender-----------------------------------------------------
	WaterReflectMapRender::WaterReflectMapRender(void)
	{
		m_p=new Member;
		m_bEnable=false;
	}

	WaterReflectMapRender::~WaterReflectMapRender(void)
	{
		delete m_p;
	}

	void WaterReflectMapRender::Init()
	{
		if(!IsHWSurpport())
			return;

		m_p->Init();
	}

	void WaterReflectMapRender::Destroy()
	{
		if(!IsHWSurpport())
			return;

		m_p->Destroy();
	}

	WaterReflectMapRender* WaterReflectMapRender::Inst()
	{
		static WaterReflectMapRender Inst;
		return &Inst;
	}

	void WaterReflectMapRender::Begin(SceneGraph* pSG)
	{
		if(!m_bEnable)
			return;

		m_p->m_pSG=pSG;
		m_p->BeginAttachNode();
	}

	void WaterReflectMapRender::AttachNode(SceneNode* pSceneNode)
	{
		if(!m_bEnable)
			return;

		if(!pSceneNode->IS_STATIC_CLASS(SGRiverPatch))
			return;

		SGRiverPatch* pPatch=(SGRiverPatch*)pSceneNode;
		if(!pPatch->IsEnableEffect())//��������Ч��
			return;

		m_p->AttachNode(pSceneNode);
	}

	void WaterReflectMapRender::End()
	{
		if(!m_bEnable)
			return;

		m_p->EndAttachNode();
		m_p->Render();
	}

	bool WaterReflectMapRender::IsHWSurpport()
	{
		IDevice* pDev=Device();
		if( pDev!=NULL )
		{
			return pDev->CapsMaxTextureStages()>=3
				&& pDev->FCapsMaxSimultaneousTextures()>=3
				&& pDev->CapsVertexShaderVersion()>=D3DVS_VERSION(2,0)
				&& pDev->CapsPixelShaderVersion()>=D3DPS_VERSION(2,0);
		}
		return false;
	}

	void WaterReflectMapRender::Enable(bool bEnable)
	{
		if(bEnable && !IsHWSurpport())//Ӳ����֧��ʱ�޷�����Ч��
			return;

		m_bEnable=bEnable;
	}

	void WaterReflectMapRender::SetEffectLevel(EEffectLevel level)
	{
		m_p->m_effectLevel=level;
	}

	WaterReflectMapRender::EEffectLevel WaterReflectMapRender::GetEffectLevel()
	{
		return m_p->m_effectLevel;
	}

	bool WaterReflectMapRender::GetRViewPrjMatrix( UINT riverID,Matrix4& mat )
	{
		for(list<tagReflectMap>::iterator iter=m_p->m_reflectMaps.begin();
			iter!=m_p->m_reflectMaps.end();++iter)
		{
			if(riverID==iter->riverID)
			{
				mat=iter->mat;
				return true;
			}
		}

		return false;
	}
}//end namespace Cool3D