#pragma once
#include "ColorMtl.h"

namespace F3D
{
	/**	\class DiffuseMtl 
		\brief ֻʹ��diffuse��ͼ�Ĳ���
	*/
	class DiffuseMtl : public ColorMtl
	{
	public:
		DiffuseMtl(void);
		virtual ~DiffuseMtl(void);

		DECL_RTTI_DYNC(DiffuseMtl);
	};
}//namespace F3D