//-----------------------------------------------------------------------------
//!\file net_cmd_mgr.h
//!\author Lyp
//!
//!\date 2005-03-08
//! last 2005-03-08
//!
//!\brief �������
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "net_cmd_mgr.h"

#include "..\console\console_gui.h"
namespace vEngine {
//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL NetCmdMgr::Init()
{
	m_mapProc.Clear();

	// ע�����̨����
	m_pConsole->Register(_T("netcmdlist"), m_Trunk.sfp0(&NetCmdMgr::List), _T("net cmd list"), 0);
	m_pConsole->Register(_T("netcmd"), m_Trunk.sfp1(&NetCmdMgr::Find), _T("net cmd find via id"), 1);
	return TRUE;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID NetCmdMgr::Destroy()
{
	m_mapProc.ResetIterator();
	tagCmd* pCmd = NULL;
	while( m_mapProc.PeekNext(pCmd) )
	{
		// ������������ܵ��Ĵ�����¼��log�ļ�
		m_pLog->Write(_T("%s:%d\r\n"), pCmd->strCmd.c_str(), pCmd->dwTimes);
		delete(pCmd);
	}

	m_mapProc.Clear();
}


//-----------------------------------------------------------------------------
// register
//-----------------------------------------------------------------------------
BOOL NetCmdMgr::Register(LPCSTR szCmd, NETMSGPROC fp, LPCTSTR szDesc)
{
	DWORD dwID = m_pUtil->Crc32(szCmd);
	tagCmd* pCmd = m_mapProc.Peek(dwID);
	if( P_VALID(pCmd) )
	{
		if( pCmd->strCmd != szCmd )
		{
			ASSERT(0);	// ��������ӵ����ͬ��CRC
			return FALSE;
		}

		ASSERT(!pCmd->listFP.IsExist(fp));	// ����ע����
		pCmd->listFP.PushBack(fp);
	}
	else
	{
		pCmd = new tagCmd;
		pCmd->dwTimes = 0;
		pCmd->listFP.PushBack(fp);
		pCmd->strCmd = szCmd;
		pCmd->strDesc = szDesc;
		m_mapProc.Add(dwID, pCmd);
	}
	return TRUE;
}


//-----------------------------------------------------------------------------
// unregister
//-----------------------------------------------------------------------------
BOOL NetCmdMgr::UnRegister(LPCSTR szCmd, NETMSGPROC fp)
{
	DWORD dwID = m_pUtil->Crc32(szCmd);
	tagCmd* pCmd = m_mapProc.Peek(dwID);
	if( !P_VALID(pCmd) )
		return FALSE;

	if( FALSE == pCmd->listFP.Erase(fp) )
		return FALSE;
	
	if( pCmd->listFP.Size() <= 0 )
	{
		SAFE_DEL(pCmd);
		m_mapProc.Erase(dwID);
	}

	return TRUE;
}
//-----------------------------------------------------------------------------
// �õ�ִ�к���������
//-----------------------------------------------------------------------------
std::string NetCmdMgr::GetHadleCmdString(tagNetCmd* pMsg)
{
	tagCmd* pCmd = m_mapProc.Peek(pMsg->dwID);
	if( !P_VALID(pCmd) )
	{
		// �ڱ�����ʾ
		IMSG(_T("Unknow net command recved[<cmdid>%d ]\r\n"), pMsg->dwID);
		return "";
	}
	return pCmd->strCmd;
}
//-----------------------------------------------------------------------------
// ִ����Ϣ������
//-----------------------------------------------------------------------------
BOOL NetCmdMgr::HandleCmd(tagNetCmd* pMsg, DWORD dwMsgSize, DWORD dwParam)
{
	tagCmd* pCmd = m_mapProc.Peek(pMsg->dwID);
	if( !P_VALID(pCmd) )
	{
		// �ڱ�����ʾ
		IMSG(_T("Unknow net command recved[<cmdid>%d <size>%d]\r\n"), pMsg->dwID, dwMsgSize);
		return FALSE;
	}

	if( pMsg->dwSize != dwMsgSize )
	{
		// ֪ͨ�ͻ��ˣ�����ȷǷ�
		IMSG(_T("Invalid net command size[<cmd>%s <size>%d]\r\n"), 
			m_pUtil->Unicode8ToUnicode(pCmd->strCmd.c_str()), pMsg->dwSize);
		return FALSE;
	}

	++pCmd->dwTimes;

	NETMSGPROC fp = NULL;
	pCmd->listFP.ResetIterator();
	while(pCmd->listFP.PeekNext(fp))
		(fp)(pMsg, dwParam);
	return TRUE;
}


//-----------------------------------------------------------------------------
// �г�����ע�����������
//-----------------------------------------------------------------------------
DWORD NetCmdMgr::List()
{
	tagCmd* pCmd = NULL;
	m_mapProc.ResetIterator();
	while( m_mapProc.PeekNext(pCmd) )
	{
		IMSG(_T("%s:%s\r\n"), m_pUtil->Unicode8ToUnicode(pCmd->strCmd.c_str()), pCmd->strDesc.c_str());
	}

	return 0;
}


//-----------------------------------------------------------------------------
// ����ID����ע�����������
//-----------------------------------------------------------------------------
DWORD NetCmdMgr::Find(LPCTSTR szCommand)
{
	DWORD dwID = _tcstoul(szCommand, NULL, 16);

	tagCmd* pCmd = m_mapProc.Peek(dwID);
	if( !P_VALID(pCmd) )
	{
		// �ڱ�����ʾ
		IMSG(_T("Unknow net command[<cmdid>0x%X]\r\n"), dwID);
		return -1;
	}

	IMSG(_T("[%d] %s:%s\r\n"), dwID, m_pUtil->Unicode8ToUnicode(pCmd->strCmd.c_str()), pCmd->strDesc.c_str());
	return 0;
}


}