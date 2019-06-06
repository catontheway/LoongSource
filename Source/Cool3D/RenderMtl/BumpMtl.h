#pragma once
#include "diffusemtl.h"

namespace F3D
{
	/**	\class BumpMtl 
		\brife ʹ��Diffuse��Bump Map�Ĳ���
	*/
	class BumpMtl :	public DiffuseMtl
	{
	public:
		BumpMtl(void);
		virtual ~BumpMtl(void);

		DECL_RTTI_DYNC(BumpMtl);
	};
}//namespace F3D