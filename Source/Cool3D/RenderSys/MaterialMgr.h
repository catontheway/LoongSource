#pragma once
#include "..\F3D.h"

	class IFS;
namespace F3D
{
	class Material;
	struct tagMtlDesc;
	/**	\class MaterialMgr
		\brief ���ʹ�����,��Ҫ���ݲ�ͬ��RenerSys���������εĲ���
	*/
	class F3D_API MaterialMgr
	{
	public:
		MaterialMgr(void);
		virtual ~MaterialMgr(void);

		static MaterialMgr *Inst();

		Material *CreateMtl(tagMtlDesc *pDesc);
		void DestroyMtl(Material *pMtl);

	};
}//namespace F3D