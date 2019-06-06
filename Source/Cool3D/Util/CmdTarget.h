#include "..\Cool3D.h"
#pragma once

namespace Cool3D
{
	class CmdDispatch;

	/**	\class CmdTarget
		\brief ��������Ķ���Ľӿ�
	*/
	class Cool3D_API CmdTarget  
	{
	public:
		virtual void OnCmd(DWORD cmd, DWORD param1, DWORD param2) =0;
		CmdTarget(CmdDispatch *pOwner);
		virtual ~CmdTarget();
		
	private:
		CmdDispatch *m_pOwner;
	};
}//namespace Cool3D