#pragma once
#include "imaterial.h"

namespace F3D
{
	/**	\class ColorMtl 
		\brief ֻʹ��һ����ɫֵ,��ʹ���κ���ͼ�Ĳ���
	*/
	class ColorMtl : public IMaterial
	{
	public:
		ColorMtl(void);
		virtual ~ColorMtl(void);

		DECL_RTTI_DYNC(ColorMtl);
	};
}//namespace F3D