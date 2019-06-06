//-----------------------------------------------------------------------------
//!\file GameFrameMgr.cpp
//!\author Lyp
//!
//!\date 2008-02-17
//! last 2008-04-15
//!
//!\brief game frame mgr
//!
//!	Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "GameFrameMgr.h"

//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
GameFrameMgr::GameFrameMgr()
{
}


//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
GameFrameMgr::~GameFrameMgr()
{
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL GameFrameMgr::Init()
{
	m_pRoot = new GameFrame;
	m_pRoot->Init(this, NULL, 0);
	m_pRoot->m_strName = _T("Root");

	DWORD dwID = m_pUtil->Crc32(_T("Root"));
	m_mapChild.Add(dwID, m_pRoot);

	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
BOOL GameFrameMgr::Destroy()
{
	m_pRoot->Destroy();
	SAFE_DEL(m_pRoot);
	return TRUE;
}


//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------
VOID GameFrameMgr::Update()
{
	// ����destroy�б�
	if( m_listDestroy.Size() > 0 )
	{
		GameFrame* pFrame;
		m_listDestroy.ResetIterator();
		while( m_listDestroy.PeekNext(pFrame) )
			this->DestroyFrame(pFrame);	// ����ɾ���ؼ�

		m_listDestroy.Clear();
	}

	m_pRoot->Update();
}


//-----------------------------------------------------------------------------
// Render
//-----------------------------------------------------------------------------
VOID GameFrameMgr::Render3D()
{
	m_pRoot->Render3D();
}


//-----------------------------------------------------------------------------
// CreateFrame
//-----------------------------------------------------------------------------
GameFrame* GameFrameMgr::CreateFrame(LPCTSTR szFatherName, LPCTSTR szName, LPCTSTR szClass, 
									 DWORD dwParam)
{
	DWORD dwID = m_pUtil->Crc32(szName);
	DWORD dwFatherID = m_pUtil->Crc32(szFatherName);
	DWORD dwClassID = m_pUtil->Crc32(szClass);

	GameFrame* pFather = m_mapChild.Peek(dwFatherID);
	if( !P_VALID(pFather) )
	{
		ASSERT(0);
		IMSG(_T("Create frame:%s failed\r\n"), szName);
		return NULL;
	}

	if( m_mapChild.IsExist(dwID) )
	{
		ASSERT(0);
		IMSG(_T("Create frame:%s failed\r\n"), szName);
		return NULL;
	}

	GameFrame* pFrame = (GameFrame*)m_Factory.Create(dwClassID);
	pFrame->m_strName = szName;
	pFrame->m_dwClassID = dwClassID;
	m_mapChild.Add(dwID, pFrame);
	if( !pFrame->Init(this, pFather, dwParam) )
	{
		SAFE_DEL(pFrame);
		m_mapChild.Erase(dwID);
		IMSG(_T("Create frame:%s failed\r\n"), szName);
		return NULL;
	}
	
	pFather->AddChild(pFrame);
	return pFrame;
}



//-----------------------------------------------------------------------------
// ����ɾ��ָ��Frame
//-----------------------------------------------------------------------------
VOID GameFrameMgr::DestroyFrame(GameFrame* pFrame)
{
	// ��ȫ�ֱ���ɾ��
	m_mapChild.Erase(m_pUtil->Crc32(pFrame->GetName().c_str()));

	// ������ɾ������
	pFrame->Destroy();

	// ���ڴ���ɾ��
	m_Factory.Destroy(pFrame, pFrame->m_dwClassID);
}



//-----------------------------------------------------------------------------
// ��һ��ɾ��
//-----------------------------------------------------------------------------
BOOL GameFrameMgr::AddToDestroyList(GameFrame* pFrame)
{
	DWORD dwID = m_pUtil->Crc32(pFrame->GetName().c_str());
	if( !GT_VALID(m_mapChild.Peek(dwID)) )
		return FALSE;	// ������ָ���ؼ�

	if( m_listDestroy.IsExist(pFrame) )
		return FALSE;	// ɾ���б����Ѿ��������ظ����

	m_listDestroy.PushBack(pFrame);

	// ��ȫ�ֱ���ɾ��
	m_mapChild.Erase(m_pUtil->Crc32(pFrame->GetName().c_str()));

	return TRUE;
}


//-----------------------------------------------------------------------------
// ע��Frame�¼�������
//-----------------------------------------------------------------------------
BOOL GameFrameMgr::RegisterEventHandle(LPCTSTR szEvent, FRAMEEVENTPROC fp)
{
	DWORD dwID = m_pUtil->Crc32(szEvent);
	tagEventHandle* pHandle = m_mapHandle.Peek(dwID);
	if( !P_VALID(pHandle) )
	{
		pHandle = new tagEventHandle;
		pHandle->strName = szEvent;
		m_mapHandle.Add(dwID, pHandle);
	}

	ASSERT( !pHandle->listFP.IsExist(fp) );
	pHandle->listFP.PushBack(fp);
	return TRUE;
}


//-----------------------------------------------------------------------------
// ע���¼�������
//-----------------------------------------------------------------------------
BOOL GameFrameMgr::UnRegisterEventHandler(LPCTSTR szEvent, FRAMEEVENTPROC fp)
{
	DWORD dwID = m_pUtil->Crc32(szEvent);
	tagEventHandle* pHandle = m_mapHandle.Peek(dwID);
	if( !P_VALID(pHandle) )
		return FALSE;

	pHandle->listFP.Erase(fp);
	if( pHandle->listFP.Size() <= 0 )
	{
		m_mapHandle.Erase(dwID);
		SAFE_DEL(pHandle);
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// �����¼�������
//-----------------------------------------------------------------------------
VOID GameFrameMgr::SendEvent( tagGameEvent* pEvent )
{
	tagEventHandle* pHandle = m_mapHandle.Peek(pEvent->dwNameCrc);
	if( !P_VALID(pHandle) )
		return;

	pHandle->listFP.ResetIterator();
	FRAMEEVENTPROC fp = 0;
	while(pHandle->listFP.PeekNext(fp))
		fp(pEvent);
}