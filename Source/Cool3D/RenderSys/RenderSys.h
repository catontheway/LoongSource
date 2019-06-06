#pragma once
#include "..\Cool3D.h"
#include "..\Math\Math.h"
#include "..\Math\GeomDefine.h"
#include "..\Util\RTTIObj.h"
#include "..\Public\vertexdefine.h"
#include "..\Device\IDevice.h"

namespace Cool3D
{
	class CameraBase;
	class SceneGraph;
	class RenderNode;
	class SceneNode;
	class SGLightNode;

	class ResStaticMesh;
	class IVertBuffer;
	class IIndexBuffer;
	class IRenderTexture;

	class AnimationCtrl;
	class IStreamDefine;
	class IMaterial;
	class PostProcess;
	class IRenderCallBack;

	class PostProcessHeatHaze;
	class PostProcessBlur;

	enum EDrawFlags
	{
		EDrawFlag_OccluderAlpha=1,	//��Ϊ�ڵ�ס������(camera from->lookAt֮����߶�)���Զ����ɰ�͸����
	};

	const int RMax_Light=4;//����RenderNode�����м����ƹ�
	struct RenderItem
	{
		SceneNode		*pSGNode;
		RenderNode		*pNode;
		AnimationCtrl	*pAni;
		int				nLod;
		DWORD			drawFlags;//�Ƿ񻭰�͸���ȵ�
		tagLight*		pLights[RMax_Light];
		float			zVal;//for sort
		float			lightModulus[RMax_Light];

		RenderItem()
		{
			memset(this,0,sizeof(RenderItem));
		}
	};

	enum ERenderSysCap
	{
		ERender_ProjectTexture,		//ʹ��Vertex Shader�Զ�����projective texture����
		ERender_ShaderMatrixPal,	//ʹ�þ����ɫ������������
	};
	/** \class RenderSys
		\brief �ṩ��SceneGraph�ĸ߲�ӿڣ�������Ⱦ, ��Ⱦ��ص�Deviceϸ��,��Ⱦ��صļ��㣨����ؼ�֡��ֵ��
		\remarks 
		\par Projection��View�����Deviceȡ��
		\par ��������RenderSys����ҪĿ������Բ�ͬ�Կ��ļ��������Ż�,��Ҫ����֧�ֵ�Shader�Ĳ�ͬ
		\par �ؼ�֡��ֵ����RenderSys��Ҫ�ǿ��ǵ�VertexShader�������ⲿ�ֹ���;
	*/
	class Cool3D_API RenderSys : public RTTIObj
	{
	public:
		RenderSys(void);
		virtual ~RenderSys(void);

		//--
		/**	build mtl render cache if necessary;
			\return num of passes
		*/
		virtual int PrepMtl(IMaterial *pMtl) =0;
		virtual bool GetCapacity(ERenderSysCap cap) =0;
		virtual void EnableFog(bool bEnable) =0;
		/**	��scene graph���������lod��������Ϊnode��lod����
			\remraks scene graph�ǰ���һ��MAX_LOAD���ּ��������,��ĳ��Node��max lod������scenegraph�Ĳ�ͬ
		*/
		static int	 ScaleSGLod(int sgLod,int nodeMaxLod);

		virtual void Begin(SceneGraph *pSG);
		virtual void AttachNode(const SceneNode *pNode);
		virtual void End(SceneGraph *pSG,bool bZScale=false);
		/**	��Ⱦ������scene node, ������begin(),end()�м����.*/
		virtual void RenderSceneNode(CameraBase *pCamera,const SceneNode *pNode,IRenderTexture *pTarget,bool clearTarget,int lod,DWORD targetColor=0xFFFFFFFF,IRenderCallBack* pCallBack=NULL,SceneGraph* pSG=NULL,const tagViewport* pNewViewport=NULL) =0;

		/**	��Ⱦ������rendernode��������begin()��end()�м����
		*/
		virtual void RenderRenderNode(CameraBase *pCamera,const SceneNode *pSceneNode,const RenderNode *pRenderNode,IRenderTexture *pTarget,bool clearTarget,int lod,DWORD targetColor=0xFFFFFFFF,IRenderCallBack* pCallBack=NULL,SceneGraph *pSG=NULL) =0;

		virtual IStreamDefine* GetStreamDefine(EVertType vertType) =0;
		/**	���ڲ���Pool�еõ�һ��VertBuffer,�ʺ�����Щʹ�ö�̬VB����Ⱦ��Ӧ��
			\remarks ע��:�ⲿ����ʹ��VertStream������VB,����VB�޷��õ���ȷ��stride!!!!
		*/
		IVertBuffer* AllocVB(UINT sizeInBytes);
		/**	��һ�����ڲ�Pool�����VertBuffer�黹��Pool
		*/
		void FreeVB(IVertBuffer* pVB);
		/** ���ڲ�Pool�õ�һ��IndexBuffer
		*/
		IIndexBuffer* AllocIB(UINT sizeInBytes);
		/** ��һ���ڲ�Pool�����IndexBuffer�黹��Pool
		*/
		void FreeIB(IIndexBuffer* pIB);

		/** ���һ�����ڴ������
			\remarks this���������� 
		*/
		void AddPostProcess(PostProcess* pPostProcess);
		/** ɾ��һ�����ڴ������
		*/
		void DelPostProcess(PostProcess* pPostProcess);
		/** ɾ�����к��ڴ������
		*/
		void ClearAllPostProcess();

		/** ��Ⱦ������ͼ
		*/
		virtual void RenderReflectMap(SceneGraph* pSG,const vector<SceneNode*>& sceneNodes,IRenderTexture* pRT,float surfaceHeight,bool bZScale=false){}

		/** ��ȾShadowMap
		*/
		virtual void RenderShadowMap(SceneGraph* pSG,const vector<SceneNode*>& sceneNodes,IRenderTexture* pRT){}
		virtual void RenderCubeShadowMap(SceneGraph* pSG,const vector<SceneNode*>& sceneNodes,IRenderCubeTexture* pRT) {}

		/** ���ڴ����أ��ر��򲻽������к��ڴ�����Ⱦ������ɾ���������Ĭ��Ϊ�򿪣�
		*/
		void EnablePostProcess( const bool bEnable ) { m_bEnablePostProcess = bEnable; }
		bool IsEnablePostProcess() const { return m_bEnablePostProcess; }
	public:
		static tagRenderColor	WhiteRenderColor;
		void IgnoreSGLights(bool ignore)	{	m_bIgnoreSGLights=ignore;}
		PostProcessHeatHaze* GetPostProcessHeatHaze() { return m_pPostProcessHeatHaze; }
		bool EnablePostProcessBlur( const bool bEnalbe );
	protected:
		friend class Kernel;//ֻ����Kernel�����������
		virtual void InitDevice();//��������Ҫ���ø���Ĵ˺���

		void AssignLights(const SceneNode* pNode,vector<SGLightNode*>& lgts);
		AnimationCtrl *GetAniCtrlFromSGNode(const SceneNode *pNode);
		
		void BuildRenderItems(SceneGraph *pSG);
		virtual void AddRenderItem(RenderItem& newItem) =0;

		bool	m_bIgnoreSGLights;	//����SceneGraph�ĵƹ�

		vector<PostProcess*> m_PostProcessArray;	 // ���ڴ����������	
		PostProcessHeatHaze* m_pPostProcessHeatHaze; // �ȱ������������������е��ȴ�����Ч��ʹ����ͳһ����
		PostProcessBlur*     m_pPostProcessBlur;

		bool				 m_bEnablePostProcess;   // �Ƿ�������ڴ���
	private:
		class Member;
		Member *m_p;

		DECL_RTTI(RenderSys);
	};
}//namespace Cool3D