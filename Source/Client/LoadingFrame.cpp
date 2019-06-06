//-----------------------------------------------------------------------------
//!\file LoadingFrame.cpp
//!\author Lyp
//!
//!\date 2008-02-17
//! last 2008-04-15
//!
//!\brief loading frame
//!
//!	Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "LoadingFrame.h"

#include "MapMgr.h"
#include "SeleRoleMgr.h"
//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
LoadingFrame::LoadingFrame():m_Trunk(this)
{
	m_pWnd = NULL;
	m_pWndText = NULL;
	m_pProgress = NULL;
	m_dwCurrent = 0;
	m_dwQuitTime = GT_INVALID;
	m_eLoadingType = ELT_World;
}


//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
LoadingFrame::~LoadingFrame()
{
	
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL LoadingFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	TCHAR szBackFileName[255];
	_stprintf( szBackFileName, _T("data\\ui\\loading\\L_loading-diban%02d.jpg"), m_pUtil->RandomInRange(1,4) );
	m_strBackPicFileName = szBackFileName;
	if( dwParam )
		m_strBackPicFileName.assign((LPCTSTR)dwParam);

	tstring strPath = g_strLocalPath + _T("\\tip\\loading_tips.xml");
	m_pTips = CreateObj("LoadingTips", "VarContainer");
	if( !m_pTips )
		return FALSE;

	if( FALSE == m_pTips->Load("VFS_System", strPath.c_str()) )
		return FALSE;


	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	// ����F10����
	m_pGUI->GetDesktop()->SetInvisible(false);	// ��ʾ����
	m_pInput->SetAllEnable(FALSE);
	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
BOOL LoadingFrame::Destroy()
{
	GameFrame::Destroy();

	m_pWndText = NULL;
	if( P_VALID(m_pWnd) )
		m_pGUI->AddToDestroyList(m_pWnd);

	KillObj("LoadingTips");

	m_pInput->SetAllEnable(TRUE);
	return TRUE;
}



//-----------------------------------------------------------------------------
//! �¼�����
//! 
//-----------------------------------------------------------------------------
BOOL LoadingFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_KeyDown:
		break;

	}

	return FALSE;
}



//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL LoadingFrame::ReloadUI()
{
	XmlElement element;

	tstring strPath = g_strLocalPath + _T("\\ui\\loading.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());

	m_pGUI->ChangeXml(&element, "panel", "PicFileName", 
		m_pUtil->UnicodeToUnicode8(m_strBackPicFileName.c_str()));

	// ���ű�����������Ϸ����
	GUIWnd* pDesktop = m_pGUI->GetDesktop();
	std::stringstream streamWidth, streamHeight;
	streamWidth << pDesktop->GetSize().x;
	streamHeight << pDesktop->GetSize().y;
	
	m_pGUI->ChangeXml(&element, "panel", "Size_x", streamWidth.str().c_str());
	m_pGUI->ChangeXml(&element, "panel", "Size_y", streamHeight.str().c_str());
/*
	// ���������Լ������ƶ�������λ��
	std::stringstream streamY;
	streamY << (pDesktop->GetSize().y - 50);
	m_pGUI->ChangeXml(&element, "panel\\progress", "LogicPos_y", streamY.str().c_str());
*/
	// ����
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &element);

	// ע���¼�������
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&LoadingFrame::EventHandler));

	m_pWndText = (GUIStatic*)m_pWnd->GetChild(_T("text"));
	m_pProgress = (GUIProgress*)m_pWnd->GetChild(_T("progress"));
	m_pWndText->Align();

	// ����tips����
	INT nMaxTips = m_pTips->GetVarNum() / 3;
	if( nMaxTips > 0 )
	{
		INT nTips = (rand() % nMaxTips) + 1;

		tstringstream stream;
		stream << nTips;
		LPCTSTR szText = m_pTips->GetString(stream.str().c_str());

		if( szText )
			m_pWndText->SetText(szText);
	}

	m_pProgress->SetSpeed(100000);
	m_pProgress->SetMaxValue(100);
	m_pProgress->SetValue(0);

	return TRUE;
}


//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID LoadingFrame::Update()
{
	GameFrame::Update();

	Sleep(50);	// ����ȡ�̸߳���CPU

	UINT uiFinished=0, uiReq=0;
	ResMgr::Inst()->GetLoadCount(uiFinished, uiReq);
	if( uiFinished == uiReq )
	{
		if( m_eLoadingType == ELT_World )
		{
			SceneGraph* pSG = MapMgr::Inst()->GetSceneGraph();
			if( pSG )
				pSG->SyncInit();
		}
		else
		{
			SceneGraph* pSG = SeleRoleMgr::Inst()->GetHugeSG();
			if( P_VALID(pSG) )
				pSG->SyncInit();
		}

		ResMgr::Inst()->GetLoadCount(uiFinished, uiReq);
		if( GT_INVALID == m_dwQuitTime && uiFinished == uiReq )	// ��Ȼ��ȡ������˳�
			m_dwQuitTime = timeGetTime();
	}

	DWORD dwCurrent = uiFinished * 100 / uiReq;
	if( dwCurrent > m_dwCurrent )
		m_dwCurrent = dwCurrent;

	m_pProgress->SetValue((FLOAT)m_dwCurrent);
	m_pProgress->SetRefresh();

	if( GT_VALID(m_dwQuitTime) && timeGetTime() - m_dwQuitTime > 200 )
	{
		m_pFrameMgr->AddToDestroyList(this);

		// ���ʹ򿪵�ͼ���������Ϸ�¼�
		tagGameEvent evt( _T("LoadMapIsOK"), NULL );
		m_pFrameMgr->SendEvent(&evt);
	}
}

BOOL LoadingFrame::EscCancel()
{
	return TRUE;
}

