#pragma once

#include "ResRenderable.h"

	class IFS;
namespace Cool3D
{
	/**
	\class UserResMgr
	\brief �û��������Դ��ϵͳ�Ľӿ�
	*/
	class UserResMgr
	{
	public:
		UserResMgr(void);
		virtual ~UserResMgr(void);

		virtual ResRenderable *NewRes(const TCHAR* szName) =0;
		virtual void DelRes(ResRenderable *pObj) =0;
		virtual void SetFS(IFS *pFS) =0;
	};
}//namespace Cool3D