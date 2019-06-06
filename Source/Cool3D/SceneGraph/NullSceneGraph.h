#pragma once
#include "..\Cool3D.h"
#include "SceneGraph.h"
#include "..\Math\Ray.h"

class IFS;
namespace Cool3D
{
	class Heightmap;

	enum ESceneNodeViewLevel	//��Ұ�ȼ�
	{
		ESNVL_Auto	= 0,		//�Զ�����
		ESNVL_Near,				//������
		ESNVL_Mid,				//�о���
		ESNVL_Far				//Զ����
	};
	/**	\class NullSceneGraph 
		\brief ��ʵ���κγ�����֯�㷨��SceneGraph,��Ҫ��������༭������Ҫ����Ⱦ�ӿڵ�ϵͳ
		\remarks ���нڵ㱣����һ���򵥵�List��,��ÿ�����������׶����
	*/
	class Cool3D_API NullSceneGraph : public SceneGraph
	{
	public:
		NullSceneGraph(void);
		virtual ~NullSceneGraph(void);

		//--SceneGraph
		virtual void LoadFromFile(IFS *pFS,const TCHAR* szMapName);
		virtual void AttachDynamicNode(SceneNode *pNode);
		virtual void DetachDynamicNode(const SceneNode *pNode);
		virtual void Render(float deltaTime);
		virtual void BuildProjectorMesh(const tagLight *pLight,const AABBox& box,DynamicMesh *pMesh);
		virtual void BuildDecalMesh( const AABBox& box,DynamicMesh *pMesh, const Vector3* pNormal );
		virtual TResult RayCollide(const Ray& ray);
		virtual void RenderReflectMap(CameraBase* pCamera,IRenderTexture* pRT,float surfaceHeight,bool bRenderTrnOnly);

		//--
		void LoadTerrainPatch(const TCHAR* szTerrain,const TCHAR* szTerrainPatch,const TCHAR* szTerrainPatchRenderGroups,const TCHAR* szLightMap);
		void LoadRiver(const TCHAR *szRiver,const TCHAR *szPatch);
		void LoadGrassPatch(const TCHAR* szFileName);
		void CreateSimpleTerrainPatch(Heightmap* pHMap,RECT& rc);
		void AttachStaticNode(SceneNode *pNode,ESceneNodeViewLevel viewLevel=ESNVL_Auto);
		void DetachStaticNode(const SceneNode *pNode);
		void BuildShadowMapForTerrain(const TCHAR* szBasePath,int numPatchW, int numPatchH);

		bool SaveAsThreeQuadTreeData(const TCHAR* szSaveBasePath,const TCHAR* szMapName);
		bool SaveAsQuadTreeData(const TCHAR* szFileName);
		set<SceneNode*> GetSceneNodes() const	{return m_sceneList;}

		//--
		vector<SceneNode*> GetPointLights() const { return m_pointlights; }
		void OpenStaticPL();
		void OpenDynamicPL();
		void CloseStaticPL();
		void CloseDynamicPL();

		void Clear();
	private:
		typedef set<SceneNode*>::iterator		SNSetIter;
		typedef vector<SceneNode*>::iterator	SNVecIter;
		typedef map<SceneNode*,ESceneNodeViewLevel> SNViewLevelMap;
		typedef map<SceneNode*,ESceneNodeViewLevel>::iterator SNViewLevelIter;
		set<SceneNode*>		m_dynaList;
		set<SceneNode*>		m_sceneList;
		vector<SceneNode*>	m_pointlights;
		SNViewLevelMap		m_viewLevelmap;		//���scenenode����Ұ�ȼ�

		bool				m_bShowStaticPL;	//�Ƿ���ʾ��̬��Դ
		bool				m_bShowDynamicPL;	//�Ƿ���ʾ��̬��Դ

		DECL_RTTI_DYNC(NullSceneGraph);
	};
}//namespace Cool3D