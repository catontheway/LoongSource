#include "MeshExportBaseDef.h"
#pragma once
#include "Max.h"
#include "..\F3DMaxExp\F3DExpMtl.h"
namespace Cool3D
{
	/**	\class ExpMtl
		\brief ��������
	*/
	class ExpMtl : public F3DExpMtl
	{
	public:
		ExpMtl(Mtl* pMtl,int subID,int myID,Mtl *pBakedMtl);
		virtual ~ExpMtl();
	};
}