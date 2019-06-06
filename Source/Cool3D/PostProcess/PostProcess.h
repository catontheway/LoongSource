#pragma once
#include "..\Cool3D.h"
#include "..\Util\RTTIObj.h"
#include "..\Device\IRenderTexture.h"

namespace Cool3D
{
	class RenderSys;
	class SceneGraph;

	/** \class PostProcess
		\brief ���ڴ������,�������������RenderSys������������
		\remarks ͨ��ֱ���޸ĺ󻺳������д���
		\par RenderSys��������Ⱦ��RenderTexture,�ٽ���PostProcess
	*/
	class Cool3D_API PostProcess : public RTTIObj
	{
	public:
		PostProcess(void);
		virtual ~PostProcess(void);

		//!��ʼ��,��������ʱ����һ��
		virtual bool Init() =0;
		//!��ʼ����,ÿ֡����
		virtual bool Start( IRenderTexture* pSrcTex, IRenderTexture* pDstTex, RenderSys* pRenderSys, bool bRenderToBackBuffer, SceneGraph* pSG ) =0;

	private:
		DECL_RTTI(PostProcess);
	};

}//namespace Cool3D