#pragma once
#include "rendersys.h"

namespace Cool3D
{
	/**	\class RenderSysPS_20 
		\brief ʹ��PixelShader2.0,VertexShader2.0����Ⱦϵͳ
		\remarks ʹ��Shader,���ǲ�������ֻ��ʹ��shader,���ֹ���ʹ��FixedFun pipeline�����,�����ʹ��֮.
	*/
	class RenderSysPS_20 : public RenderSys
	{
	public:
		RenderSysPS_20(void);
		virtual ~RenderSysPS_20(void);

		DECL_RTTI(RenderSysPS_20);
	};
}//namespace Cool3D