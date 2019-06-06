#pragma once
#include "diffusemtl.h"

namespace F3D
{
	/**	\class TexCoordGenMtl 
		\brief ʹ��Diffuse��ͼ,����ҪΪ�ڶ�����ͼ�Զ�������������Ĳ���
		\remarks ֧��Sphere map, cube map
	*/
	class TexCoordGenMtl : public DiffuseMtl
	{
	public:
		TexCoordGenMtl(void);
		virtual ~TexCoordGenMtl(void);

		DECL_RTTI_DYNC(TexCoordGenMtl);
	};
}//namespace F3D