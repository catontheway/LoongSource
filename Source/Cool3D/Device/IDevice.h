#pragma once
#include "..\Cool3D.h"

#include "..\Math\math.h"
#include "..\Math\GeomDefine.h"
#include "Light.h"
#include "..\Math\Color.h"
#include "FogDefine.h"
#include <list>
namespace Cool3D
{
	class IVertBuffer;
	class IIndexBuffer;
	class IDeviceTex;
	class IRenderTexture;
	class IRenderCubeTexture;
	class IVertShader;
	class IPixelShader;
	class IFont;
	class IDepthFont;
	class IStreamDefine;
	class IRenderToEnvMap;
	class IDeviceVizQuery;
	class LockableObject;
	struct RenderStatus;

	enum EDrawType
	{
		EDraw_PointList,
		EDraw_LineList,
		EDraw_LineStrip,
		EDraw_TriangleList,
		EDraw_TriangleStrip,
		EDraw_TriangleFan,
	};
	enum EMatrixStackType
	{
		EMS_Prj=0,
		EMS_View,
		EMS_World,
		EMS_Tex0,
		EMS_Tex1,
		EMS_Num
	};

	enum ETexMatrixIndex
	{
		ETM_None=0,
		ETM_Tex0=1,
		ETM_Tex1=2,
		ETM_All=ETM_Tex0|ETM_Tex1,
	};

	_inline ETexMatrixIndex TexStage2MatrixIndex(int stage)
	{
		switch(stage)
		{
		case 0:	return ETM_Tex0;
		case 1:	return ETM_Tex1;
		}
		return ETM_None;
	}

	enum ECmpFunc 
	{
		ECMP_Never,
		ECMP_Less,
		ECMP_Equal,
		ECMP_LessEqual,
		ECMP_Greater,
		ECMP_NotEqual,
		ECMP_GreaterEqual,
		ECMP_Always,
	};

	enum ECullMode
	{
		ECull_None,
		ECull_CW,
		ECull_CCW,
	};


	enum EFillMode
	{
		EFill_Point,
		EFill_Wireframe,
		EFill_Solid,
	};

	enum EShadeMode
	{
		EShade_Flat,
		EShade_Gouraud,
	};

	enum EBlend 
	{
		EBlend_Zero,
		EBlend_One,
		EBlend_SrcColor,
		EBlend_InvSrcColor,
		EBlend_SrcAlpha,
		EBlend_InvSrcAlpha,
		EBlend_DestAlpha,
		EBlend_InvDestAlpha,
		EBlend_DestColor,
		EBlend_InvDestColor,
	};

	enum EStencilOP
	{
		ESOP_Keep,
		ESOP_Zero,
		ESOP_Replace,
		ESOP_IncrSat,
		ESOP_DecrSat,
		ESOP_Invert,
		ESOP_Incr,
		ESOP_Decr,
	};

	enum ETextureAddress
	{
		ETAddress_Invalid=0,
		ETAddress_Wrap,
		ETAddress_Mirror,
		ETAddress_Clamp,
		ETAddress_Border,
	};
	enum ETextrueFilter
	{
		ETFilter_Invalid=0,
		ETFilter_NONE,		// ��
		ETFilter_POINT,		// ����� 
		ETFilter_LINEAR,	// ���Թ���
		ETFilter_ANISOTROPIC,// �������Թ���
		ETFilter_GAUSSIAN,	// ��˹ģ��
	};



#define COLORWRITEENABLE_RED     (1L<<0)
#define COLORWRITEENABLE_GREEN   (1L<<1)
#define COLORWRITEENABLE_BLUE    (1L<<2)
#define COLORWRITEENABLE_ALPHA   (1L<<3)

	enum ERStateType
	{
		ERS_DepthFunc,
		ERS_DepthTestEnable,
		ERS_DepthWriteEnable,
		ERS_CullMode,
		ERS_FillMode,
		ERS_ShadeMode,
		ERS_AlphaTestEnable,
		ERS_AlphaTestFunc,
		ERS_AlphaRef,
		ERS_AlphaBlendEnable,
		ERS_SrcBlend,
		ERS_DstBlend,
		ERS_StencilEnable,
		ERS_StencilFunc,
		ERS_StencilOpFail,
		ERS_StencilOpZFail,
		ERS_StencilOpPass,
		ERS_AntialiasedLine,
		ERS_FLightEnable,//�̶��ܵ��ĵƹ��Ƿ���
		ERS_FSpecularEnable,//�̶���Ⱦ�ܵ��ĵƹ��Ƿ������淴��߹�
		ERS_FFogEnable,//�̶��ܵ�����
		ERS_ColorWriteEnable,
		ERS_FFogColor,
		ERS_UseVertexDiffuse,//���ռ���ʱ�Ƿ�Ӷ����л�ȡDiffuse
		ERS_TextureFactor,//����������׶ε���ɫֵ
		ERS_DepthBias,
		ERS_DepthBiasScale,
		ERS_NormalizeNormals,

		ERS_Num
	};

	enum EPixelFormat
	{
		EPF_R5G6B5,
		EPF_R8G8B8,
		EPF_A1R5G5B5,
		EPF_A8R8G8B8,
		EPF_X8R8G8B8,
		
		//--�Կ�texture���ܲ�֧�ֵĸ�ʽ
		EPF_A8,
		EPF_A32R32G32B32F,

		EPF_DXT1,
		EPF_R32F,

		EPF_Num
	};

	enum EDepthFormat
	{
		EDF_D16,
		EDF_D24S8,
	};

	enum ETextureOp
	{
		ETOP_Invalid=0,
		ETOP_Disable,
		ETOP_SelectArg1,
		ETOP_SelectArg2,
		ETOP_Mul,
		ETOP_Mul2x,
		ETOP_Mul4x,
		ETOP_Add,
		ETOP_Sub,
		ETOP_BlendCurrentAlpha,//ʹ�õ�ǰ��alphaֵ(ǰ��stage��alphaֵ)����alpha���
		ETOP_BlendTextureAlpha,
		ETOP_BlendDiffuseAlpha,
		ETOP_BlendFactorAlpha,
		ETOP_BumpEnvMap,
	};

	enum EUVTrans
	{
		EUVT_Invalid=0,
		EUVT_Disable,
		EUVT_Count2,
		EUVT_Count3,
		EUVT_Project3,
		EUVT_Project4
	};

	enum EUVIndexFlag//����׶�uv indexѡ��
	{
		EUVI_Invalid=0,
		EUVI_None,				//ʹ�ö���洢����������
		EUVI_CameraSpaceNormal,		//�����㷨��ת����camera�ռ䣬��Ϊ��������
		EUVI_CameraSpacePos,		//������λ��ת����camera�ռ䣬��Ϊ��������
		EUVI_CameraReflectionVec,	//ͨ������λ�úͷ��߼����������������Ϊ��������
		EUVI_SphereMap				//�Զ���������ӳ��
	};
	
	enum ETextureArg
	{
		ETA_Invalid=0,
		ETA_Current,
		ETA_Diffuse,
		ETA_Texture,
		ETA_Temp,//temp�Ĵ���,ȱʡֵΪ(0,0,0,0)
		ETA_TFactor,
		ETA_Specular,
		ETA_TextureAlphaReplicate,
	};

	enum EMultisampleType//����ݼ���
	{
		EMT_None=0,
		EMT_2x,
		EMT_4x,
		EMT_8x,
	};

	struct tagDeviceCfg
	{
		bool				bWindowed;
		UINT				nWidth,
							nHeight;
		UINT				nFullScreenHZ;
		EPixelFormat		pixelFormat;
		EDepthFormat		depthFormat;
		HWND				hWnd;
		EMultisampleType	multiSampleType;
		bool				bVerticalSync;//��ֱͬ��

		tagDeviceCfg()
		{
			memset(this,0,sizeof(tagDeviceCfg));
			bWindowed=true;
		}
	};

	struct tagDisplayMode
	{
		UINT nWidth;
		UINT nHeight;
		UINT nRefreshRate;
		EPixelFormat eFormat;
	};

	struct tagRenderColor
	{
		Color4f diffuse;
		Color4f ambient;
		Color4f specular;
		Color4f emissive;
		float	power;

		tagRenderColor()
		{
			memset(this,0,sizeof(tagRenderColor));
		}
		bool operator==(const tagRenderColor& other)
		{
			return diffuse==other.diffuse
				&& ambient==other.ambient
				&& specular==other.specular
				&& emissive==other.emissive
				&& power==other.power;
		}
	};

	struct tagViewport
	{
		DWORD x;
		DWORD y;
		DWORD width;
		DWORD height;
		float minZ;
		float maxZ;

		bool operator==(const tagViewport& other)
		{
			return x==other.x
				&& y==other.y
				&& width==other.width
				&& height==other.height
				&& minZ==other.minZ
				&& maxZ==other.maxZ;
		}
	};

	struct tagLockedRect
	{
		int pitch;
		void* pBits;
	};

	/**	\class IDevice
		\brief ����Device����״̬�Ľӿ�
		\todo Renderstate lighting enable
	*/
	class Cool3D_API IDevice
	{
	public:
		IDevice(void);
		virtual ~IDevice(void);

		virtual bool Create(tagDeviceCfg *pCfg) =0;
		virtual void Destroy() =0;
		//--��ͼ����
		/**	ֻʹ��Vertex Buffer����ͼ	*/
		virtual bool Draw(EDrawType type,UINT numPrimitive) =0;
		/** ʹ���ڴ��еĶ�����������ͼ�ĺ���	*/
		virtual bool DrawMem(EDrawType type,UINT numPrimitive,void *pVert,UINT nStride) =0;
		/**	ʹ��Vertex Buffer��Indexed Buffer����ͼ	*/
		virtual bool DrawIndexed(EDrawType type,UINT baseIndex,UINT numVert,UINT numPrimitive,UINT startIndex=0) =0;
		/** ʹ��Vertex buffer�еĶ�������Point sprites,����point sprites�Ĵ�С��һ��
			\todo:�������Ҫ,����ʵ��ÿ��sprite��С��ͬ
			*/
		virtual bool DrawPointSprites(float pointSize,int numPoints) =0;
		virtual void ColorFill(const RECT* pRect,DWORD color) =0;
				
		//--��Ҫ��״̬
		virtual void SetClearParam(bool target,bool depth, bool stencil,DWORD color,float z,DWORD stencilVal) =0;
		virtual void Clear() =0;
		virtual void Clear(const RECT* pRect) =0;
		virtual void Clear(bool target,bool depth, bool stencil) =0;
		virtual void Clear(bool target,bool depth, bool stencil,DWORD color,float z,DWORD stencilVal,const RECT* pRect=NULL) =0;
		virtual bool Begin() =0;
		virtual void End() =0;
		virtual void Present(HWND hDestWnd=NULL) =0;

		virtual void SetDepthFunc(ECmpFunc zFun) =0;
		virtual void EnableDepthTest(bool bEnable) =0;
		virtual void EnableDepthWrite(bool bEnable) =0;

		virtual void SetCullMode(ECullMode cullMode)=0;

		virtual void SetFillMode(EFillMode fillMode) =0;
		virtual void SetShadeMode(EShadeMode shadeMode) =0;

		virtual void EnableAlphaTest(bool bEnable) =0;
		virtual void SetAlphaTestFunc(ECmpFunc func) =0;
		virtual void SetAlphaRef(BYTE ref) =0;

		virtual void EnableAlphaBlend(bool bEnable) =0;
		virtual void SetAlphaBlend(EBlend src,EBlend dst) =0;

		virtual void EnableStenciling(bool bEnable) =0;
		virtual void SetStencilFunc(ECmpFunc func, int refVal, DWORD mask) =0;
		virtual void SetStencilOp(EStencilOP fail,EStencilOP zFail,EStencilOP pass) =0;

		virtual void SetRenderState(ERStateType state, DWORD val, bool bForce=false) =0;//����״̬������һ���ȼۺ���
		virtual DWORD GetRenderState(ERStateType state) =0;

		virtual void SetSimplerAddress(DWORD sampler,ETextureAddress addU,ETextureAddress addV,ETextureAddress addW) =0;
		virtual void SetSimplerFilter(DWORD sampler,ETextrueFilter mag,ETextrueFilter min) =0;
		virtual void SetSimplerMipFilter(DWORD sampler,ETextrueFilter mip) =0;//mip map
		
		//--��Դ��
		virtual IVertBuffer *NewVB() =0;
		virtual IIndexBuffer *NewIB() =0;
		virtual IDeviceTex* NewTexture() =0;
		virtual IRenderTexture* NewRenderTexture() =0;
		virtual IRenderCubeTexture* NewRenderCubeTexture() =0;
		virtual IVertShader* NewVS() =0;
		virtual IPixelShader* NewPS() =0;
		virtual IFont* NewFont() =0;
		virtual IDepthFont *NewDepthFont() =0;
		virtual IStreamDefine* NewStreamDefine() =0;
		virtual IRenderToEnvMap* NewRenderToEnvMap() =0;
		virtual IDeviceVizQuery* NewVizQuery() =0;
		virtual void SetStreamDefine(IStreamDefine *pDef) =0;
		virtual void SetVB(DWORD stream,IVertBuffer *pVB) =0;
		virtual void SetIB(IIndexBuffer *pIB) =0;
		virtual void SetTexture(DWORD sample,IDeviceTex *pTex) =0;
		virtual void SetTexture(DWORD sample,IRenderTexture *pTex) =0;
		virtual void SetTexture(DWORD sample,IRenderCubeTexture *pTex) =0;
		virtual void SetVertShader(IVertShader *pVS) =0;
		virtual void SetPixelShader(IPixelShader *pPS) =0;

		//--����任
		virtual Matrix4* GetMatrix(EMatrixStackType type) =0;
		virtual void MatPush(EMatrixStackType type) =0;
		virtual void MatPop(EMatrixStackType type) =0;
		virtual void MatLoad(EMatrixStackType type, const Matrix4 *pMat) =0;

		//--����
		virtual void SetScissorRect(const RECT *pRect)=0;//NULL to disable
		virtual bool GetClipPlane(Plane *pPlane) =0;
		virtual void SetClipPlane(Plane *pPlane) =0;
		virtual void GetViewport(tagViewport& viewport) =0;
		virtual void SetViewport(const tagViewport* pViewport) =0;
		virtual void SetDepthBias(float bias) =0;
		virtual void SetDepthBiasScale(float biasScale) =0;

		//--�̶���Ⱦ�ܵ����ܺ���
		virtual UINT FGetMaxTextureStage() =0;
		virtual void FFlushMatrix(bool world=true,bool view=true,bool prj=true,bool tex=true,DWORD texIndex=ETM_All) =0;
		virtual void FSetTextureColorOp(DWORD stage,ETextureOp op,ETextureArg arg1,ETextureArg arg2) =0;
		virtual void FSetTextureAlphaOp(DWORD stage,ETextureOp op,ETextureArg arg1,ETextureArg arg2) =0;
		virtual void FSetTextureColorOp(DWORD stage,ETextureOp op) =0;
		virtual void FSetTextureAlphaOp(DWORD stage,ETextureOp op) =0;
		virtual void FSetTexCoordSrc(DWORD stage,UINT uvIndex,DWORD flags=0) =0;
		virtual void FSetTexBumpMat(DWORD stage,const Matrix2& mat) =0;
		virtual void FEnableFog(bool bEnable) =0;
		virtual void FSetFog(DWORD color,float start,float end,float density) =0;
		virtual void FSetFogFormula(EFogFormula fm) =0;
		virtual void FEnableLight(bool bEnable) =0;
		virtual void FEnableSpecular(bool bEnable) =0;
		virtual void FSetUVTrans(DWORD stage,EUVTrans trans) =0;
		/**	���õƹ�,NULL to disable		*/
		virtual void FSetLight(DWORD index,const tagLight *pLight) =0;
		virtual void FSetRenderColor(const tagRenderColor* pColor) =0;
	
		//--
		virtual RenderStatus *GetRenderStatus() =0;
		virtual UINT GetClientWidth() =0;
		virtual UINT GetClientHeight() =0;

		//--
		/**	�õ����豸ͬʱ����ʹ�ü�����ͼ*/
		virtual int FCapsMaxSimultaneousTextures() =0;
		virtual int CapsMaxTextureStages() =0;

		//** �õ����豸֧�ֵ�PixelShader�汾��*/
		virtual DWORD CapsPixelShaderVersion() =0;
		//** �õ����豸֧�ֵ�VertexShader�汾��*/
		virtual DWORD CapsVertexShaderVersion() =0;

		/** �Ƿ�֧��bump mapʵ�ֵ���������ƫ��*/
		virtual bool CapsBumpEnvMap() =0;
		/** �Ƿ�֧��uv�����Զ���w����*/
		virtual bool CapsTexProjected() =0;

		/** �Ƿ�֧��Border�����װģʽ*/
		virtual bool CapsBorderTexWrapMode() =0;
		/** �Ƿ�֧����ɫд�뿪�أ�����clear()������Ч��*/
		virtual bool CapsColorWriteEnable() =0;
		/** �Ƿ�֧���ڵ���ѯ */
		virtual bool CapsVizQuery() =0;
		//--
		virtual bool ScreenShot(const TCHAR* szPathName, BOOL bJPG=FALSE) =0;

		virtual void CreateBltBuffer() =0;
		virtual void ReleaseBltBuffer() =0;
		virtual void BitBltBackbuffer(HDC src,int x,int y,int w,int h) =0;
		
		virtual void CopyFromBackbuffer(IRenderTexture* pTex,const RECT* src,const RECT* dest) =0;


		/**	����Ļ��������ת��Ϊ��һ������
		*/
		float NormalizeScreenX(UINT x);
		float NormalizeScreenY(UINT y);

		/** ö���豸֧�ֵ���ʾģʽ�б�*/
		virtual bool EnumDisplayModes( std::list<tagDisplayMode>& ModeList, const UINT nMinW, const UINT nMinH ) =0;

		/** �豸Reset��
		*/
		virtual LockableObject* GetResetLock() =0;
		/** �����ⲿ�Ƿ���Ҫ�����豸��λ
		*/
		virtual bool GetDeviceResetFlag() =0;
		/** �ⲿ�������Ժ������־
		*/
		virtual void ClearDeviceResetFlag() =0;
		/** ��ȡ�Կ���Ϣ
		*/
		virtual UINT GetAdapterCount() =0;
		virtual bool GetAdapterIdentifier( UINT nAdapter, D3DADAPTER_IDENTIFIER9* pIdentifier ) =0;
		virtual UINT GetAvailableTextureMem() =0;
	};
}//namespace Cool3D