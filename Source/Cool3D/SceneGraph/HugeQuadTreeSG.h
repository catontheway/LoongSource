#pragma once
#include <set>
using namespace std;
#include "..\Cool3D.h"
#include "..\Math\GeomDefine.h"
#include "scenegraph.h"
#include "QuadTreeData.h"

class IFS;
namespace Cool3D
{
	class SGQuadTreeNode;
	class SGSimpleTerrain;
	class IRenderTexture;
	class IRenderCubeTexture;
	class IRenderToEnvMap;
	class IDeviceVizQuery;
	class Frustum;
	/**	\class HugeQuadTreeSG 
		\brief ���͵��Ĳ�����������,�ṩ�����������֧��
		\remarks 
		\par �ⲿֻ��Ҫ���SG������Node,�����Զ�����Camera��λ��������Node�ļ������ݵ�������
		\par View Zone��һ����Camera��From��Ϊ���ĵ�AABBox,�����Box��Node��������ڴ�,����Ҫ������ڴ�
	*/
	class Cool3D_API HugeQuadTreeSG :	public SceneGraph
	{
	public:
		enum EViewZoneType
		{
			EVZT_LitNode=0,
			EVZT_MidNode,
			EVZT_BigNode,
			EVZT_DynamicNode,
			EVZT_SimpleTerrain,
			EVZT_Num,
		};

		enum EQuadTreeType
		{
			EQTT_SmallNode=0,
			EQTT_MidNode,
			EQTT_BigNode,
			EQTT_Num
		};
	public:
		HugeQuadTreeSG(void);
		virtual ~HugeQuadTreeSG(void);

		//--SceneGraph
		virtual void LoadFromFile(IFS *pFS,const TCHAR* szMapName);
		virtual void AttachDynamicNode(SceneNode *pNode);
		virtual void DetachDynamicNode(const SceneNode *pNode);
		virtual void Render(float deltaTime);
		virtual void BuildProjectorMesh(const tagLight *pLight,const AABBox& box,DynamicMesh *pMesh);
		virtual void BuildDecalMesh( const AABBox& box,DynamicMesh *pMesh, const Vector3* pNormal );
		virtual void BindCamera(CameraBase *pCamera,const bool bSetDefaultFogDist=true);
		virtual void RenderReflectMap(CameraBase* pCamera,IRenderTexture* pRT,float surfaceHeight,bool bRenderTrnOnly);
		virtual void SyncInit();
		virtual void SetSceneFog(const tagDistFog& fog);

		//-- ���������֧�ֺ���
		/**	����View Zone�Ĵ�С
		*/
		void SetViewZoneSize(EViewZoneType type,const Vector3& size);
		const Vector3& GetViewZoneSize(EViewZoneType type);
		/**	���ݵ�ǰ��ViewZone�Ĵ�С��Camera��λ�ü����ViewZone
		*/
		void CalCurrentViewZone(EViewZoneType type,AABBox& out);
		/**	ʹ��ViewZoneʧЧ,���´�Renderʱ����ViewZone
			\remarks һ����Camera�ƶ�ʱ����
		*/
		void InvalidViewZone();

		AABBox GetBox() const;
  
		//�����ӵ��Ƿ�������������Լ��ڵ��ӵ�������͸��(�������Ϊ��ǰ�ӵ�λ�ú���һ�ε��ӵ�λ���Լ���λ�ã�����Ϊ���ӵ���ײ������İ�Χ��)
		bool TestContainBetweenViewPointAndSceneObject(Vector3 vFrom,Vector3 vOldFrom,Vector3 vLookAt,Cool3D::AABBox& box);

		TResult RayCollide(const Ray& ray);

		/** ��ȡ���о�̬Node����
			\remarks ����δ���ص��ڴ��е�
		*/
		DWORD GetNumStaticNodes();
		/** ��ȡ�ڴ��еľ�̬Node
		*/
		void GetStaticNodes(vector<SceneNode*>& sceneNodes);

		bool RenderForRayCollide(Vector3& origin,Vector3& dir,float dist,IRenderTexture* pRenderTex,IDeviceVizQuery* pVizQuery);
		void RenderHemisphere(Vector3& eye,Vector3& dir,float dist,IRenderTexture* pRenderTex,IRenderToEnvMap* pRenderToEnvMap);
		void RenderShadowMap(CameraBase& camera,IRenderTexture* pRenderTex);
		void RenderCubeShadowMap(CameraBase& camera,IRenderCubeTexture* pRenderTex);

	private:
		Vector3			m_viewZoneSize[EVZT_Num];
		QuadTreeData	m_data[EQTT_Num];
		SGQuadTreeNode	*m_pRoot[EQTT_Num];
		bool			m_bLoaded;

		set<SceneNode*>	m_dynaList;	//set��ɾ����list��,���ҿ��Է�ֹ�ظ�attach

		SGSimpleTerrain* m_pSimpleTerrain;

		DECL_RTTI_DYNC(HugeQuadTreeSG);
	};
}//namespace Cool3D