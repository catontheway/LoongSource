//-----------------------------------------------------------------------------
//!\file net_cmd_mgr.h
//!\author Lyp
//!
//!\date 2005-03-08
//! last 2005-03-08
//!
//!\brief �������
//-----------------------------------------------------------------------------
#pragma once

namespace vEngine {

typedef DWORD (WINAPI* NETMSGPROC)(tagNetCmd*, DWORD);
//-----------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------
class VENGINE_API NetCmdMgr
{
public:
	BOOL Init();
	VOID Destroy();

	BOOL Register(LPCSTR szCmd, NETMSGPROC fp, LPCTSTR szDesc);
	BOOL UnRegister(LPCSTR szCmd, NETMSGPROC fp);
	BOOL HandleCmd(tagNetCmd* pMsg, DWORD dwMsgSize, DWORD dwParam);
	
	std::string	GetHadleCmdString(tagNetCmd* pMsg);


protected:
	struct tagCmd
	{
		std::string			strCmd;		// ������
		tstring				strDesc;	// ����
		TList<NETMSGPROC>	listFP;		// ����ָ���б�
		DWORD				dwTimes;	// �յ�������Ĵ���
	};

	TSFPTrunk<NetCmdMgr>	m_Trunk;
	TObjRef<Util>			m_pUtil;
	TObjRef<Log>			m_pLog;
	TObjRef<Console>		m_pConsole;

	TMap<DWORD, tagCmd*>	m_mapProc;

	DWORD List();
	DWORD Find(LPCTSTR szCommand); // ����ID����ע�����������
};


}