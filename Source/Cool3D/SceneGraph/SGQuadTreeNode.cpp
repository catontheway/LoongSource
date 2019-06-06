#include "StdAfx.h"
#include ".\sgquadtreenode.h"
#include "..\Math\Frustum.h"
#include "..\Util\StlExt.h"
#include "QuadTreeData.h"
#include "..\RenderSys\RenderSys.h"
#include "..\Terrain\TerrainPatch.h"
#include "..\Terrain\SGRiverPatch.h"
#include "..\Terrain\GrassPatch.h"
#include "EntityNode.h"
#include "..\ResSys\resrenderable.h"
#include "..\ResSys\resscenemesh.h"
#include "SceneGraph.h"
#include "..\RenderSys\WaterReflectMapRender.h"
#include "..\SceneGraph\SGLightNode.h"

namespace Cool3D
{
	IMP_RTTI(SGQuadTreeNode,RTTIObj);

	SGQuadTreeNode::SGQuadTreeNode(void)
	{
		m_id=-1;
		m_lv=0;
		memset(m_pChildren,0,sizeof(m_pChildren));
		for(int i=0;i<4;i++)
			m_childrenID[i]=-1;
	}

	SGQuadTreeNode::~SGQuadTreeNode(void)
	{
		FreePtrVector<SceneNode> freeSceneNodes;
		freeSceneNodes(m_pContent);
		for(int i=0;i<4;i++)
			SAFE_DELETE(m_pChildren[i]);
	}

	bool SGQuadTreeNode::IsInFrustum(const Frustum *pFrumstum) const
	{
		return pFrumstum->BoxInFrustum(m_box);
	}

	void SGQuadTreeNode::BuildProjectorMesh_Trn(const AABBox& box,DynamicMesh *pMesh)
	{
		if(!m_box.Intersects(box))
			return;

		//todo

		//--�ݹ���������ӽڵ�
		for(int i=0;i<4;i++)
		{
			if(m_pChildren[i])
				m_pChildren[i]->BuildProjectorMesh_Trn(box,pMesh);
		}
	}

	void SGQuadTreeNode::BuildDecalMesh( const AABBox& box,DynamicMesh *pMesh, const Vector3* pNormal )
	{
		if( !m_box.Intersects( box ) )
			return;

		if( m_pContent.size() > 0 )
		{
			for( vector<SceneNode*>::iterator iter = m_pContent.begin();
				iter!=m_pContent.end();iter++ )
			{
				SceneNode *pNode = (*iter);
				if( pNode->IS_STATIC_CLASS( TerrainPatch ) )
					( (TerrainPatch*)pNode )->BuildDecalMesh( box, pMesh, pNormal );
				if( pNode->IS_STATIC_CLASS( EntityNode ) )
					( (EntityNode*)pNode )->BuildDecalMesh( box,pMesh, pNormal );
			}
		}

		//--�ݹ���������ӽڵ�
		for( int i=0;i<4;i++ )
		{
			if( m_pChildren[i] )
				m_pChildren[i]->BuildDecalMesh( box, pMesh, pNormal );
		}
	}

	void SGQuadTreeNode::ForceUpdate(SceneGraph *pSG)
	{
		for(vector<SceneNode*>::iterator iter=m_pContent.begin();
			iter!=m_pContent.end();iter++)
		{
			SceneNode *pNode=(*iter);
			pNode->ForceUpdate(0.0f,pSG);
		}
		//--�ݹ���������ӽڵ�
		for(int i=0;i<4;i++)
		{
			if(m_pChildren[i])
				m_pChildren[i]->ForceUpdate(pSG);
		}
	}

	void SGQuadTreeNode::AttachToRenderSys(SceneGraph *pSG,RenderSys *pSys,Frustum *pFrumstum,float deltaTime,bool bParentVis)
	{
		//--�Ĳ�����׶����
		bool bVis;
		if(bParentVis)
			bVis=IsInFrustum(pFrumstum);
		else
			bVis=false;
			
		for(vector<SceneNode*>::iterator iter=m_pContent.begin();
			iter!=m_pContent.end();iter++)
		{
			SceneNode *pNode=(*iter);
			pNode->ForceUpdate(deltaTime,pSG);
			if(bVis)
			{
				if(pNode->IsInFrustum(pFrumstum))//per scene node ��׶����
				{
					if(pNode->IS_STATIC_CLASS(SGLightNode))
					{
						SGLightNode *pLightNode=(SGLightNode*)pNode;
						if(!pLightNode->IsDynamic())
							continue;
					}

					pNode->Update(deltaTime,pSG);
					pSys->AttachNode(pNode);

					WaterReflectMapRender::Inst()->AttachNode(pNode);
				}			
			}
		}

		//--�ݹ���������ӽڵ�
		for(int i=0;i<4;i++)
		{
			if(m_pChildren[i])
				m_pChildren[i]->AttachToRenderSys(pSG,pSys,pFrumstum,deltaTime,bVis);
		}			
	}

	void SGQuadTreeNode::AttachToRenderSysForShadowMap(SceneGraph *pSG,RenderSys *pSys,Frustum *pFrumstum,
		float deltaTime,bool bParentVis)
	{
		//--�Ĳ�����׶���ã����Ժ����ڵ㣬�����з�����Ⱦ
		bool bVis;
		if(bParentVis)
			bVis=IsInFrustum(pFrumstum);
		else
			bVis=false;

		for(vector<SceneNode*>::iterator iter=m_pContent.begin();
			iter!=m_pContent.end();iter++)
		{
			SceneNode *pNode=(*iter);

			if(pNode->IS_STATIC_CLASS(SGRiverPatch))
				continue;
			pNode->ForceUpdate(deltaTime,pSG);
			if(bVis)
			{
				if(pNode->IsInFrustum(pFrumstum))//per scene node ��׶����
				{
					pNode->Update(deltaTime,pSG);
					pSys->AttachNode(pNode);
				}			
			}
		}

		//--�ݹ���������ӽڵ�
		for(int i=0;i<4;i++)
		{
			if(m_pChildren[i])
				m_pChildren[i]->AttachToRenderSysForShadowMap(pSG,pSys,pFrumstum,deltaTime,bVis);
		}	
	}

	void SGQuadTreeNode::InvalidViewData(const AABBox& viewZone,QuadTreeData *pDataSrc)
	{
		for(int i=0;i<4;i++)
		{
			if(m_childrenID[i]!=-1)
			{
				if(viewZone.Intersects(m_childrenBox[i]))
				{
					if(m_pChildren[i]==NULL)
						m_pChildren[i]=pDataSrc->LoadNode(m_childrenID[i]);
				}
				else
				{
					if(m_pChildren[i]!=NULL)
					{
						delete m_pChildren[i];
						m_pChildren[i]=NULL;
					}
				}
				//--�ݹ�������п��ܿɼ����ӽڵ�
				if(m_pChildren[i]!=NULL)
					m_pChildren[i]->InvalidViewData(viewZone,pDataSrc);
			}//endof if
		}//endof for
	}

	void SGQuadTreeNode::Serialize(FArchive& ar)
	{
		ar<<m_id;
		ar<<m_lv;
		ar<<m_box.max;
		ar<<m_box.min;

		int i;
		for(i=0;i<4;i++)
			ar<<m_childrenID[i];
		for(i=0;i<4;i++)
		{
			ar<<m_childrenBox[i].max;
			ar<<m_childrenBox[i].min;
		}
		
		SerialObjPtrVector(ar,m_pContent);

		//--������QuadTreeNode���ӽڵ�
	}

	void SGQuadTreeNode::Deserialize(FArchive& ar)
	{
		ar>>m_id;
		ar>>m_lv;
		ar>>m_box.max;
		ar>>m_box.min;

		int i;
		for(i=0;i<4;i++)
			ar>>m_childrenID[i];
		for(i=0;i<4;i++)
		{
			ar>>m_childrenBox[i].max;
			ar>>m_childrenBox[i].min;
		}
		
		DeserialObjPtrVector(ar,m_pContent);
	}

	//�����ӵ��Ƿ�������������Լ��ڵ��ӵ�������͸����(�������Ϊ��ǰ�ӵ�λ�ú���һ�ε��ӵ�λ���Լ���λ�ã�����Ϊ���ӵ���ײ������İ�Χ��)
	bool SGQuadTreeNode::TestContainBetweenViewPointAndSceneObject(Vector3 vFrom,Vector3 vOldFrom,Vector3 vLookAt,Cool3D::AABBox& objbox)
	{
		for(vector<SceneNode*>::iterator iter=m_pContent.begin();
			iter!=m_pContent.end();iter++)
		{
			SceneNode *pNode=(*iter);
			if(pNode->IS_STATIC_CLASS(EntityNode))
			{
				ResRenderable* pRenderable=((EntityNode*)pNode)->GetResObj();
				if(pRenderable!=NULL && pRenderable->IS_STATIC_CLASS(ResSceneMesh))
				{
					Cool3D::AABBox box;
					pNode->GetBox(box);

					//�����ڵ��ӵ�������͸��
					/*Matrix4 mat=pNode->GetWorldMat();
					bool bIsOcclusion=((ResSceneMesh*)pRenderable)->TestLineCollision(vFrom,vLookAt,mat);
					if(bIsOcclusion)
					{
						((EntityNode*)pNode)->SetTransparent(true);

						((ResSceneMesh*)pRenderable)->UpdateMeshSort(vFrom);
					}

					else
						((EntityNode*)pNode)->SetTransparent(false);*/

					//�����ӵ���ײ���
					//��������AABB��������㣬����и���ϸ�Ĳ���
					bool bIsContain=box.Contain(vFrom);
					if(bIsContain)
					{
						//���������С�����壬����Ҫ���и���ϸ�Ĳ���
						float fSize=Vec3Dist(box.max,box.min);
						if(fSize<500.0f)
						{
							objbox=box;
							return true;
						}

						//��������������
						Matrix4 mat=pNode->GetWorldMat();

						bIsContain=((ResSceneMesh*)pRenderable)->TestLineCollision(vFrom,vOldFrom,mat);
						if(bIsContain)
						{
							objbox=box;
							return true;
						}
						else
							return false;	
					}
				}
			}
		}
		//--�ݹ���������ӽڵ�
		for(int i=0;i<4;i++)
		{
			if(m_pChildren[i])
			{
				bool bIsContain=m_pChildren[i]->TestContainBetweenViewPointAndSceneObject(vFrom,vOldFrom,vLookAt,objbox);
				if(bIsContain)
				{
					return true;
				}
			}
		}
		return false;
	}

	TResult SGQuadTreeNode::RayCollide(const Ray& ray)
	{
		TResult tr(false,FLOAT_MAX);
		RayCollideR(ray,tr);
		return tr;
	}

	void SGQuadTreeNode::RayCollideR(const Ray& ray,TResult& lastTR)
	{
		//���ж����Ĳ���box���ཻ
		TResult tr=ray.Intersect(m_box);
		if(!tr.first)
			return;
		
		//�ж�ÿ��scene node
		for(vector<SceneNode*>::iterator iter=m_pContent.begin();
			iter!=m_pContent.end();iter++)
		{
			SceneNode *pNode=(*iter);
			if(pNode->IS_STATIC_CLASS(EntityNode))
			{
				EntityNode *pEntity=(EntityNode*)pNode;
				tr=pEntity->RayCollide(ray);
				if(tr.first
					&& tr.second<lastTR.second)
				{
					lastTR=tr;
				}
			}
		}//endof for

		//--�ݹ���������ӽڵ�
		for(int i=0;i<4;i++)
		{
			if(m_pChildren[i])
				m_pChildren[i]->RayCollideR(ray,lastTR);
		}
	}

	void SGQuadTreeNode::GetSceneNodes(const Frustum *pFrumstum,vector<SceneNode*>& sceneNodes,bool bTrnOnly,bool bIgnoreRvr,bool bIgnoreGrass)
	{
		//--�Ĳ�����׶����
		if(!IsInFrustum(pFrumstum))
			return;

		for(vector<SceneNode*>::iterator iter=m_pContent.begin();iter!=m_pContent.end();++iter)
		{
			SceneNode *pNode=(*iter);

			if(bTrnOnly && !pNode->IS_STATIC_CLASS(TerrainPatch))
				continue;

			if(bIgnoreRvr && pNode->IS_STATIC_CLASS(SGRiverPatch))
				continue;

			if(bIgnoreGrass && pNode->IS_STATIC_CLASS(GrassPatch))
				continue;

			if(pNode->IsInFrustum(pFrumstum))//per scene node ��׶����
				sceneNodes.push_back(pNode);
		}		

		//--�ݹ���������ӽڵ�
		for(int i=0;i<4;i++)
		{
			if(m_pChildren[i])
				m_pChildren[i]->GetSceneNodes(pFrumstum,sceneNodes,bTrnOnly,bIgnoreRvr);
		}
	}

	void SGQuadTreeNode::GetSceneNodes(vector<SceneNode*>& sceneNodes)
	{
		for(vector<SceneNode*>::iterator iter=m_pContent.begin();iter!=m_pContent.end();++iter)
		{
			SceneNode *pNode=(*iter);
			sceneNodes.push_back(pNode);
		}		

		//--�ݹ���������ӽڵ�
		for(int i=0;i<4;i++)
		{
			if(m_pChildren[i])
				m_pChildren[i]->GetSceneNodes(sceneNodes);
		}
	}
}//namespace Cool3D