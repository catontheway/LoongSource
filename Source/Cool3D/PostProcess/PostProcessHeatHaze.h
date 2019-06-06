#pragma once
#include "postprocess.h"

namespace Cool3D
{
	class ResPixelShader;
	class DynamicMesh;
	class IRenderTexture;
	class ResTexture;
	class IDevice;
	class StateBlock;
	class MBitmap;

	/** \struct tagHeatHaze
	    \brief  �������ȴ�����������ݹ��ⲿ����ʱ���
	*/
	struct tagHeatHaze
	{
		DynamicMesh* pMesh;			// ����ͼ��
		MBitmap*	 pHeatMtl;		// �Ȳ��ʣ������Ŷ��󻺳�
		Matrix4      matWorld;		// �������
		Matrix4      matBumpMap;	// �Ŷ�����ľ���������������
		float        fOffsetFator;	// �Ŷ�ϵ��

		tagHeatHaze() : pMesh( NULL ), pHeatMtl( NULL ), 
			            matWorld( MathConst::IdentityMat ), matBumpMap( MathConst::IdentityMat ),
						fOffsetFator( 0.0f ) {}
	};

	/** \class   PostProcessHeatHaze
		\brief	 �ȱ������Ч��
		\remarks ʹ��һ�������R,Gͨ���Ŷ��󻺳壬������������Ч��
	*/
	class Cool3D_API PostProcessHeatHaze :	public PostProcess
	{
	public:
		PostProcessHeatHaze(void);
		virtual ~PostProcessHeatHaze(void);

		// PostProcess
		virtual bool Init();
		virtual bool Start( IRenderTexture* pSrcTex, IRenderTexture* pDstTex, RenderSys* pRenderSys, bool bRenderToBackBuffer, SceneGraph* pSG );

		/** \����һ���ȴ�����Ч
		*/
		void AddHeatHaze( const tagHeatHaze& hh );
		/** \Ӳ���Ƿ�֧��
		*/
		static bool IsHWSupport();
	private:
		void SetDeviceStates( IDevice* pDev, StateBlock* pBlock );
		void ResumeDeviceStates( IDevice* pDev, StateBlock* pBlock );
	private:
		IPixelShader* m_pPS;
		list<tagHeatHaze> m_listHeatHaze;

		DECL_RTTI(PostProcessHeatHaze);
	};

}//namespace Cool3D
