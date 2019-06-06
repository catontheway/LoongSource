#pragma once
#include "postprocess.h"
#include "..\Device\IPixelShader.h"
#include "..\Device\IRenderTexture.h"
#include "..\Device\IVertBuffer.h"
#include "..\ResSys\VertStream.h"
#include "..\ResSys\ResTexture.h"

namespace Cool3D
{
	/** \class PostProcessBlur
		\brief ȫ���ữЧ��
	*/
	class Cool3D_API PostProcessBlur :	public PostProcess
	{
	public:
		PostProcessBlur(void);
		virtual ~PostProcessBlur(void);

		//--PostProcess
		virtual bool Init();
		virtual bool Start( IRenderTexture* pSrcTex, IRenderTexture* pDstTex, RenderSys* pRenderSys, bool bRenderToBackBuffer, SceneGraph* pSG );
		/** \Ӳ���Ƿ�֧��
		*/
		static bool IsHWSupport();
	private:
		//!���Ŷ�����������ͼ����
		void UpdateVertStream( float tarW, float tarH );
		//!����һ��
		void RenderOnce( IRenderTexture* pSrcTex, IRenderTexture* pDstTex, IPixelShader* pPS, bool bRenderToBackBuffer );

		//!ģ��PS
		IPixelShader* m_pBlurPS;	
		IPixelShader* m_pBlurHPS;
		IPixelShader* m_pBlurVPS;

		//!��СPS
		IPixelShader* m_pScaleDownPS;
		//!�Ŵ�PS
		IPixelShader* m_pScaleUpPS;
		//!���PS
		IPixelShader* m_pMulPS;

		//!��ʱtexture
		IRenderTexture* m_pTempTex1;
		IRenderTexture* m_pTempTex2;

		//!��������
		VertStream m_vertStream;
		//!���㻺����
		IVertBuffer* m_pVb;

		//!ģ��λ��
		static float m_BlurPixel[4][2];
		Vector4	m_BlurPixelParam[4];

		DECL_RTTI(PostProcessBlur);
	};

}//namespace Cool3D
