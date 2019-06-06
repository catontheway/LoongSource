#pragma once
#include "SceneNode.h"
#include "..\Math\GeomDefine.h"
#include "..\Math\Ray.h"

namespace Cool3D
{
	class QuadTreeData;
	class RenderSys;
	class DynamicMesh;
	class TerrainPatch;
	/**	\class SGQuadTreeNode
		\brief �Ĳ����ռ�ָ�Ľڵ�
	*/
	class SGQuadTreeNode : public RTTIObj,public ISerializable
	{
	public:
		SGQuadTreeNode(void);
		virtual ~SGQuadTreeNode(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
				
		void AttachToRenderSys(SceneGraph *pSG,RenderSys *pSys,Frustum *pFrumstum,float deltaTime,bool bParentVis);
		void AttachToRenderSysForShadowMap(SceneGraph *pSG,RenderSys *pSys,Frustum *pFrumstum,float deltaTime,bool bParentVis);
		void InvalidViewData(const AABBox& viewZone,QuadTreeData *pDataSrc);
		void ForceUpdate(SceneGraph *pSG);

		void BuildProjectorMesh_Trn(const AABBox& box,DynamicMesh *pMesh);
		void BuildDecalMesh( const AABBox& box,DynamicMesh *pMesh, const Vector3* pNormal );

		int GetID() const				{	return m_id;}
		const AABBox& GetBox() const	{	return m_box;}

		//�����ӵ��Ƿ�������������Լ��ڵ��ӵ�������͸����(�������Ϊ��ǰ�ӵ�λ�ú���һ�ε��ӵ�λ���Լ���λ�ã�����Ϊ���ӵ���ײ������İ�Χ��)
		bool TestContainBetweenViewPointAndSceneObject(Vector3 vFrom,Vector3 vOldFrom,Vector3 vLookAt,Cool3D::AABBox& objbox);

		TResult RayCollide(const Ray& ray);

		void GetSceneNodes(const Frustum *pFrumstum,vector<SceneNode*>& sceneNodes,bool bTrnOnly,bool bIgnoreRvr,bool bIgnoreGrass=true);
		void GetSceneNodes(vector<SceneNode*>& sceneNodes);
			
	protected:
		bool IsInFrustum(const Frustum *pFrumstum) const;
		void RayCollideR(const Ray& ray,TResult& lastTR);
	protected:
		int		m_id;	//id in Quad tree scene graph
		int		m_lv;	//level in quad tree,lv 0 is root
		AABBox	m_box;

		//--��������,��Ҫ��Ϊ����û��childʵ�������ʱ����child�Ƿ�ɼ�
		int					m_childrenID[4];
		AABBox				m_childrenBox[4];
		
		//--
		vector<SceneNode*>	m_pContent;//ʵ�ְ�����Ⱦ���ݵĽڵ�
		SGQuadTreeNode*		m_pChildren[4];

		DECL_RTTI_DYNC(SGQuadTreeNode);
	};
}//namespace Cool3D