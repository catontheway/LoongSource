///-----------------------------------------------------------------------------
/// File: service
/// Desc: ������ά����
/// Author: Zhang Rong
/// Date: Apr 2009
///
/// Copyright (c) 1985-2007 CTCRST Entertainment All rights reserved.
///-----------------------------------------------------------------------------
#pragma once

//------------------------------------------------------------------------------
// Զ�˵���ά����
//------------------------------------------------------------------------------
class RtServer
{
public:

	DWORD   m_dwID;
	BYTE	m_byType;
};

//------------------------------------------------------------------------------
// ������ά���������
//------------------------------------------------------------------------------
class CService
{
public:
	CService();
	~CService();

	// ����Ŀ����͹رպ���
	VOID	ServiceStart(DWORD dwArgc, LPTSTR *lpszArgv);
	VOID	ServiceStop();

	// ����ĳ�ʼ�������ٺ���
	BOOL	Init();
	VOID	Destroy();

	// ����ǳ��ص�����
	DWORD	LoginCallBack(LPBYTE pMsg, DWORD dwSize);
	VOID	LogoutCallBack(DWORD dwID);

	// ��Ϣ������
	VOID	HandleCloseWorld(tagNetCmd* pMsg,LPVOID pCli);
	VOID	HandleStartWorld(tagNetCmd* pMsg,LPVOID pCli);
	VOID	HandleServerLogin(tagNetCmd* pMsg,LPVOID pCli);
	VOID	HandleSystemCmd(tagNetCmd* pMsg,LPVOID pCli);

private:
	// �жϽ����Ƿ��Ѿ�����
	DWORD   GetProcessIdFromName(LPCTSTR pName);
	// ȡ�������ļ��ľ���·��
	BOOL	GetWindowsServiceInstallPath(LPCTSTR ServiceName, tstring& szPath);

	TSFPTrunk<CService>				m_Trunk;
	
	// vEngine����ع���
	TObjRef<VarContainer>			m_pVar;
	TObjRef<Log>					m_pLog;
	TObjRef<Util>					m_pUtil;
	TObjRef<NetCmdMgr>				m_pMsgCmdMgr;

	// ��������Ϣ
	INT								m_nPort;
	tstring							m_szPath;
	tstring							m_szLoginName;
	tstring							m_szDBName;
	tstring							m_szWorldName;

	// ��������
	VOLATILE BOOL					m_bServiceStop;
	VOLATILE DWORD					m_dwIDHolder;
	CRITICAL_SECTION				m_Lock;

	// �������
	TObjRef<StreamServer>			m_pStreamServer;
	
	// ��¼�������ӵ��������Զ����ά����
	TMap<DWORD, RtServer*>			m_mapRtServer;
	
};