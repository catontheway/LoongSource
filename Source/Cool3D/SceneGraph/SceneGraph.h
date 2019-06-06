#pragma once
#include "..\Cool3D.h"
#include "..\Math\Camerabase.h"
#include "..\Device\Light.h"
#include "..\Device\FogDefine.h"
#include "..\Util\RTTIObj.h"
#include "..\Math\GeomDefine.h"

class IFS;
namespace Cool3D
{
	class SGEffectNode;
	class SGSkyNode;
	class SceneNode;
	class DynamicMesh;
	class SGSunNode;
	class IRenderTexture;
	class TerrainPatch;
	
	/**	\class SceneGraph
		\brief ���𳡾��ڵ�Ĵ洢���ɼ��Լ��ã���ײ���, ���ڵ��ύ��RenderSys
		\see RenderSys
		\todo �ⲿ�����Ҫʹ�ö��SceneGraph,����Ҫ�Ķ�Kernel��this->Camera����
	*/
	class Cool3D_API SceneGraph : public RTTIObj
	{
	public:
		enum { MAX_LOD=10 };
		SceneGraph(void);
		virtual ~SceneGraph(void);

		static SceneGraph* NewSceneGraph(const TCHAR* szClassName);
		virtual void LoadFromFile(IFS *pFS,const TCHAR* szMapName) =0;

		virtual void AttachDynamicNode(SceneNode *pNode) =0;
		/** If you detach the nod from scene graph,you need DELETE it yourself*/
		virtual void DetachDynamicNode(const SceneNode *pNode) =0;

		/**	ʹ����׶����,�ڵ����õ��ҵ���Ҫ��Ⱦ��node,Ȼ��update����,Ȼ����Ⱦ����
		*/
		virtual void Render(float deltaTime) =0;
		virtual void BuildProjectorMesh(const tagLight *pLight,const AABBox& box,DynamicMesh *pMesh) =0;
		virtual void BuildDecalMesh( const AABBox& box,DynamicMesh *pMesh, const Vector3* pNormal ) =0;

		/*
		virtual void AddSceneEffect() =0;//rain,snow,fog,sky
		*/

		int CalcDetailLevel(const AABBox& box,const Matrix4* pWorldMat=NULL);
		virtual void BindCamera(CameraBase *pCamera,const bool bSetDefaultFogDist=true);
		CameraBase* GetCamera() const			{ return m_pCamera;        }
		
		void SetSunLight(const tagLight& lgt)	{	m_sunLight=lgt;        }
		const tagLight& GetSunLight() const		{	return m_sunLight;	   }
		virtual void SetSceneFog(const tagDistFog& fog)	{	m_sceneFog=fog;}
		const tagDistFog& GetSceneFog() const	{	return m_sceneFog;     }
		void SwitchSceneFog( const tagDistFog& fog );
		void SetSceneFogFadeTime( const float& time ) { m_fFogFadeTime = time; }

		//!	������սڵ�,�������������this����
		void SetSkyNode(SGSkyNode *pNode);
		SGSkyNode* GetSkyNode() const;

		//! ����̫����㣬�������������this����
		void SetSunNode(SGSunNode *pNode);
		SGSunNode* GetSunNode() const;

		/** ��Դ������ɺ�������SceneNodes����ͬ����ʼ������
		*/
		virtual void SyncInit(){};
		bool IsSyncInit()	{ return m_bSyncInit;}
		void BeginSyncInit() { m_bSyncInit=true;}
		void EndSyncInit()	{ m_bSyncInit=false;}

		/** ��Ⱦ������ͼ
		*/
		virtual void RenderReflectMap(CameraBase* pCamera,IRenderTexture* pRT,float surfaceHeight,bool bRenderTrnOnly){};

	private:
		void UpdataSceneFogFade();
	protected:
		tagDistFog		m_sceneFog;
		tagLight		m_sunLight;
		float			m_sunModulus;
		CameraBase		*m_pCamera;
		float			m_lodDistSq[MAX_LOD];
		SGSkyNode		*m_pSkyNode;//��ΪSkyNode��RenderSys�б������⴦��,���Ե�������
		SGSunNode		*m_pSunNode;//SunNode��RendSys�����⴦��
		bool			m_bSyncInit;

		bool            m_bFogFading;           //���Ƿ����ڹ���
		float           m_fFogSwitchTime;       //���л���ʱ��
		float           m_fFogFadeTime;         //����ɵ�ʱ�䳤��
		tagDistFog      m_lastFog;				//�ϴε���
		tagDistFog      m_nextFog;				//�л���Ŀ����

		DECL_RTTI(SceneGraph);
	};
}//namespace Cool3D