//-----------------------------------------------------------------------------
//!\file	server.cpp
//!\brief	��Ӫ������
//!
//!\date	2009-04-20
//! last	2009-04-20
//!\author	zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../WorldDefine/base_define.h"

#include "server.h"
#include "section_mgr.h"
#include "client_mgr.h"

//-----------------------------------------------------------------------------
// ȫ�ֱ���
//-----------------------------------------------------------------------------
Server g_Server;

//-----------------------------------------------------------------------------
// ���캯��   
//-----------------------------------------------------------------------------
Server::Server() : m_Trunk(this), m_bTerminateUpdate(FALSE)
{

}

//------------------------------------------------------------------------------
// ��ʼ��
//------------------------------------------------------------------------------
BOOL Server::Init()
{
	m_pLog			=	"Log";
	m_pUtil			=	"Util";
	m_pVar			=	"VarContainer";

	// ����log
	m_pLog->Create();

	// �����ʼ����
	TCHAR tszFilePath[X_LONG_STRING] = {0};
	GetModuleFileName(NULL, tszFilePath, X_LONG_STRING);
	int nCount = _tcslen(tszFilePath);
	while( tszFilePath[nCount-1] != '\\' )
	{
		nCount--;
	}
	tszFilePath[nCount] = '\0';

	_tcscat(tszFilePath, _T(SERVICE_CONFIG_INI));
	if (!m_pUtil->GetIniPath(tszFilePath, tszFilePath))
		return FALSE;
	m_pVar->Load("VirtualFileSys", tszFilePath);

	// ��ʼ������������
	if( !g_ClientMgr.Init() )
		return FALSE;
	if( !g_SectionMgr.Init() )
		return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------
//    
//-----------------------------------------------------------------
void Server::MainLoop()
{
	_tprintf(_T("enter MainLoop........\n"));

	DWORD dwTime = 0;

	_set_se_translator(vEngine::SEH_Translation);

	try
	{
		while ( m_bTerminateUpdate == FALSE )
		{
			dwTime = timeGetTime();

			Lock();

			// ����SectionMgr
			g_SectionMgr.Update();

			// ����ClientMgr
			g_ClientMgr.Update();
			
			// ����ʱ����Ϣ������
			g_TimeLimitMsgMgr.Update();

			UnLock();

			// ����Sleepʱ��,ÿ��Tick����һ��
			dwTime = timeGetTime() - dwTime;
			if( dwTime < TICK_TIME )
				Sleep(TICK_TIME - dwTime);
		}
	}
	catch( vEngine::Exception )
	{
		//����ǵ��ԣ����׳��쳣
		if ( true == m_pUtil->IsDebuggerPresent() )
			throw;		
		else
			exit(1);
	}
}

//-----------------------------------------------------------------
//    
//-----------------------------------------------------------------
void Server::Destroy()
{
	g_SectionMgr.Destroy();
	g_ClientMgr.Destroy();
	m_pLog->Close();
}
