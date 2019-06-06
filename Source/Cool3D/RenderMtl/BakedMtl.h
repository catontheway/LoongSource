#pragma once
#include "diffusemtl.h"

namespace F3D
{
	/**	\class BakedMtl 
		\brief ʹ��Diffuse��LightingMap�Ĳ���
	*/
	class BakedMtl : public DiffuseMtl
	{
	public:
		BakedMtl(void);
		virtual ~BakedMtl(void);

		DECL_RTTI_DYNC(BakedMtl);
	};
}//namespace F3D