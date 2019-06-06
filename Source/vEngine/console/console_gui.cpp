//-----------------------------------------------------------------------------
//!\file console_gui.cpp
//!\author Lyp
//!
//!\date 2004-06-17
//! last 2008-02-15
//!
//!\brief GUI����̨
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "console_gui.h"

// ʹ����������
#include "console_command.h"
#include "..\gui\gui_static.h"
#include "..\gui\gui_window.h"
#include "..\gui\gui_editbox.h"
#include "..\gui\gui_editboxex.h"
#include "..\gui\gui_listbox.h"
#include "..\gui\gui_patch.h"
#include "..\gui\gui_event_define.h"
#include "..\input\input_map.h"

namespace vEngine {
//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
Console::Console():m_Trunk(this)
{
	m_pCommander = NULL;
	m_pWnd = NULL;
	m_pWndCaption = NULL;
	m_pWndCommand = NULL;
	m_pWndDisplay = NULL;
}
//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
Console::~Console()
{
	Destroy();
}

//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL Console::Init(LPCSTR szVFS, INT nMaxLine, BOOL bVisable, BOOL bShowCommand)
{
	m_pCommander = new ConsoleCommand();
	if( !P_VALID(m_pCommander) )
		return FALSE;

	m_nMaxLine = nMaxLine;
	m_bShowCommand = bShowCommand;

	m_pCommander->SetOutput(m_Trunk.sfp1(&vEngine::Console::Output));
	m_pCommander->Register( _T("cls"),  m_Trunk.sfp1(&vEngine::Console::Cls), _T("clear screen"));

	
	// ��������
	tstring strPath = g_strLocalPath + _T("\\ui\\console.xml");
	m_pWnd = m_pGUI->CreateFromFile(szVFS, strPath.c_str());
	// ע���¼�������
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&vEngine::Console::EventHandler));

	m_pWndCaption = (GUIPatch*)m_pWnd->GetChild(_T("panel"));
	m_pWndDisplay = (GUIEditBoxEx*)m_pWndCaption->GetChild(_T("frame0\\display"));
	m_pWndCommand = (GUIEditBox*)m_pWndCaption->GetChild(_T("frame\\command"));
	m_pWndSubmit = m_pWndCaption->GetChild(_T("submit"));

	m_pWndWatch = (GUIListBox*)m_pWndCaption->GetChild(_T("frame0\\list"));
	m_pWndWatch->SetColNum(2,100);

	m_pWndCommand->SetMaxTextCount(1000);	// �������1000��

	if( !bVisable )
		m_pWnd->SetInvisible(TRUE);

	m_pInputMap->Register(0, _T("console switch"), (INPUTMAPEVENTHANDLE)
		m_Trunk.sfp2(&vEngine::Console::Switch), DIK_F1, TRUE, TRUE, 1);


	m_pGUI->SetActive(m_pWndCommand);
	return TRUE;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
BOOL Console::Destroy()
{
	SAFE_DEL( m_pCommander );
	tagWatchItem* pItem = NULL;
	m_mapWatch.ResetIterator();
	while( m_mapWatch.PeekNext(pItem) )
		SAFE_DEL(pItem);
	m_mapWatch.Clear();
	return TRUE;
}


//-----------------------------------------------------------------------------
// ���ÿ���̨����
//-----------------------------------------------------------------------------
VOID Console::SetCaption( LPCTSTR szCaption )
{
	if( P_VALID(m_pWndCaption) )
		m_pWndCaption->SetText(szCaption);
}


//-----------------------------------------------------------------------------
//! ע�����̨����
//-----------------------------------------------------------------------------
VOID Console::Register(LPCTSTR szName, LPVOID fpFunction, LPCTSTR szDesc, INT nParamNum)
{
	if( m_pCommander )
		m_pCommander->Register(szName, fpFunction, szDesc, nParamNum);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DWORD Console::Output(LPCTSTR szText)
{
	this->Print(szText);
	return 0;
}


//-----------------------------------------------------------------------------
//! Print����̨���
//! ����printf��ʽ
//-----------------------------------------------------------------------------
VOID Console::Print(LPCTSTR szText, ... )
{
	if( !m_pWndDisplay )
		return;

	TCHAR szTextBuf[2048];

	ZeroMemory(szTextBuf, sizeof(szTextBuf));
	va_list argptr = NULL;
	va_start(argptr, szText);
	_vstprintf_s(szTextBuf, sizeof(szTextBuf)/sizeof(TCHAR), szText, argptr);
	va_end(argptr);

	if( !m_pCommander )
	{
		::MessageBox(NULL, szTextBuf, _T("info"), MB_OK);
		return;
	}

	// �ڴ˽���������
	m_pWndDisplay->EnterLock();
	// ----------------------------------
	BOOL bShow = m_pWndDisplay->IsShowLastLine();
	m_pWndDisplay->AddText(szTextBuf);
	// ��������������ƾ�ɾ��ǰ����У�ɾ��һ��
	if( m_pWndDisplay->GetLineNum() > m_nMaxLine )
		m_pWndDisplay->DelFrontLine(m_nMaxLine / 2);

	if( bShow )
		m_pWndDisplay->SetCurLine(0xffffff);	// �������
	// ----------------------------------
	m_pWndDisplay->LeaveLock();

}


//-----------------------------------------------------------------------------
//! ����
//! 
//-----------------------------------------------------------------------------
DWORD Console::Cls(DWORD)
{
	m_pWndDisplay->EnterLock();
	m_pWndDisplay->Clear();
	m_pWndDisplay->LeaveLock();
	return 0;
}


//-----------------------------------------------------------------------------
//! �¼�����
//! 
//-----------------------------------------------------------------------------
BOOL Console::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_EditBoxEnter:
		if( pWnd == m_pWndCommand )
		{
			tstring strCommand = m_pWndCommand->GetText();
			if( !strCommand.empty() )
			{
				this->Excute(strCommand.c_str(), m_bShowCommand);
				m_pWndCommand->SetText(_T(""));
			}
		}
		break;

	case EGUIE_Click:
		if( pWnd == m_pWndSubmit )
		{
			tstring strCommand = m_pWndCommand->GetText();
			if( !strCommand.empty() )
			{
				this->Excute(strCommand.c_str(), m_bShowCommand);
				m_pWndCommand->SetText(_T(""));
			}
		}
	}

	return FALSE;
}



//-----------------------------------------------------------------------------
// ִ��ĳָ��
//-----------------------------------------------------------------------------
DWORD Console::Excute(LPCTSTR szCommand, BOOL bShowCommand)
{
	if( TRUE == bShowCommand )
	{
		this->Print(szCommand);
		this->Print(_T("\r\n"));
	}

	return m_pCommander->Excute(szCommand);
}



//-----------------------------------------------------------------------------
// ���ӱ���
//-----------------------------------------------------------------------------
VOID Console::Watch(LPCTSTR szName, DWORD dwValue)
{
	TCHAR szMess[X_LONG_STRING];
	DWORD dwID = m_pUtil->Crc32(szName);
	tagWatchItem* pItem = m_mapWatch.Peek(dwID);
	
	if( P_VALID(pItem) )
	{
		if( dwValue == pItem->dwValue )
			return;

		if( timeGetTime() - pItem->dwLastRefreshTime < 500 ) // 500msˢ��һ��
			return;

		pItem->dwValue = dwValue;
		pItem->dwLastRefreshTime = timeGetTime();
		_itot(dwValue, szMess, 10);
		m_pWndWatch->SetText(pItem->nListIndex, 1, szMess);
	}
	else
	{
		pItem = new tagWatchItem;
		if( !P_VALID(pItem) )
			return;

		pItem->dwValue = dwValue;
		pItem->nListIndex = m_pWndWatch->GetTotalRowNum();
		pItem->dwLastRefreshTime = timeGetTime();
		_itot(dwValue, szMess, 10);
		m_pWndWatch->SetText(pItem->nListIndex, 0, szName);
		m_pWndWatch->SetText(pItem->nListIndex, 1, szMess);
		m_mapWatch.Add(dwID, pItem);
	}
}


//-----------------------------------------------------------------------------
// ����̨����
//-----------------------------------------------------------------------------
DWORD Console::Switch(DWORD dwID, BOOL bDown)
{
	if( bDown )
	{
		if(P_VALID(m_pWnd)) 
			m_pWnd->SetInvisible(!m_pWnd->IsInvisible());
			
		if( !m_pWnd->IsInvisible() )
			m_pGUI->SetActive(m_pWndCommand);
		else
			m_pGUI->SetActive(m_pGUI->GetDesktop());
	}
	return 0;
}



} // namespace vEngine {
