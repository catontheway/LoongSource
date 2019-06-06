//-----------------------------------------------------------------------------
//!\file GameInputMap.cpp
//!\author Lyp
//!
//!\date 2008-10-09
//! last 2008-10-09
//!
//!\brief game input map
//!
//!	Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "GameInputMap.h"

#define KEY_CFG_FILE _T("config\\KeyCfg.xml") 

//-----------------------------------------------------------------------------
// load gui
//-----------------------------------------------------------------------------
BOOL GameInputMap::LoadGUI(LPCTSTR szWindowName, tagPoint& ptPos)
{
	// ��������
	tstring strPath = g_strLocalPath + _T("\\ui\\custombutton.xml");
	m_pWnd = (GUIWindow*)m_pGUI->CreateFromFile("VFS_System", strPath.c_str());
	// ע���¼�������
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&GameInputMap::GUIEventHandler));

	m_pWndList = (GUIListBox*)m_pWnd->GetChild(_T("background\\background\\list"));
	m_pWndOK = (GUIButton*)m_pWnd->GetChild(_T("background\\ok"));
	m_pWndCancle = (GUIButton*)m_pWnd->GetChild(_T("background\\cancel"));
	m_pWndClose = (GUIButton*)m_pWnd->GetChild(_T("background\\close"));
	m_pWndDefault = (GUIButton*)m_pWnd->GetChild(_T("background\\default"));

	m_pWndPage1 = (GUIPushButton*)m_pWnd->GetChild(_T("background\\frame\\page1"));
	m_pWndPage2 = (GUIPushButton*)m_pWnd->GetChild(_T("background\\frame\\page2"));
	m_pWndPage3 = (GUIPushButton*)m_pWnd->GetChild(_T("background\\frame\\page3"));

	m_pText = (GUIStatic*)m_pWnd->GetChild(_T("background\\text"));

	m_pWndPage1->SetState(EGUIBS_PushDown);
	m_pWnd->Move(ptPos);

	this->ShowKeys();
	m_bEnable = TRUE;
	return TRUE;
}


//-----------------------------------------------------------------------------
// unload gui
//-----------------------------------------------------------------------------
VOID GameInputMap::UnloadGUI()
{
	if( P_VALID(m_pWnd) )
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;

		m_pFrameMgr->SendEvent( &tagGameEvent(_T("CloseGameInput"), NULL) );
	}
}


//-----------------------------------------------------------------------------
// show keys
//-----------------------------------------------------------------------------
VOID GameInputMap::ShowKeys()
{
	m_pWndList->Clear();
	m_pWndList->SetColNum(2,130);

	INT nPage = 0;
	if( m_pWndPage2->GetState() == 1 )
		nPage = 1;
	if( m_pWndPage3->GetState() == 1 )
		nPage = 2;

	// ����ҳ����ʾע�ᰴ��
	m_listKeyDesc.ResetIterator();
	DWORD dwDesc = 0;
	INT nIndex = 0;
	while(m_listKeyDesc.PeekNext(dwDesc))
	{
		tagKeyMapInfo* pInfo = m_mapKeyFunction.Peek(dwDesc);
		if( pInfo->bShow && pInfo->nUserData == nPage )
		{
			m_pWndList->SetText(nIndex, 0, pInfo->strDesc.c_str());

			tstringstream stream;
			if( m_dwCurrentSet == nIndex )	// ��ǰ����������Ŀ
			{
				stream << g_StrTable[_T("CustomHotKeyWaitInput")];
				m_pWndList->SetText(nIndex, 1, stream.str().c_str(), 0xffff0000);
			}
			else if( pInfo->dwKey == 0 )
			{
				stream << g_StrTable[_T("CustomHotKeyUndefine")];
				m_pWndList->SetText(nIndex, 1, stream.str().c_str(), 0xffff0000);
			}
			else
			{
				if( pInfo->dwKey & 0xffff )
					stream << this->GetKeyName((DIK)pInfo->dwKey);
				if( pInfo->dwKey & 0xffff0000 )
					stream << _T("+") << this->GetKeyName((DIK)(pInfo->dwKey >> 16));

				m_pWndList->SetText(nIndex, 1, stream.str().c_str(), 0xffffffff);
			}

			
			nIndex++;
		}
	}
	if( m_nCurrentTextRow!=GT_INVALID )
		m_pWndList->SetCurrentTextRow(m_nCurrentTextRow);
}


//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID GameInputMap::Update()
{
	if( !m_bEnable )
		return;

	if( P_VALID(m_pWnd) && GT_VALID(m_dwCurrentSet) )	// ����״̬
		return UpdateSetup();

	tagKeyMapInfo* pInfo = NULL;
	m_mapKeyFunction.ResetIterator();
	while( m_mapKeyFunction.PeekNext(pInfo) )
	{
		if( !pInfo->bEnable )
			continue;

		BOOL bCall = FALSE, bDown = FALSE;
		DIK k1 = (DIK)(pInfo->dwKey), k2 = (DIK)(pInfo->dwKey>>16);
		if( k2 )	// ˫�����ǵ���
		{
			if( (m_pInput->IsKeyDownOnce(k1) && m_pInput->IsKeyDown(k2))
				|| (m_pInput->IsKeyDown(k1) && m_pInput->IsKeyDownOnce(k2)) )
				bCall = bDown = TRUE;	// ����

			if( (m_pInput->IsKeyUpOnce(k1) && m_pInput->IsKeyDown(k2))
				|| (m_pInput->IsKeyDown(k1) && m_pInput->IsKeyUpOnce(k2)) )
				bCall = TRUE, bDown = FALSE;	// ����
		}
		else	// ����
		{
			if( k1 != DIK_LSHIFT && k1 != DIK_RSHIFT 
				&& k1 != DIK_LMENU && k1 != DIK_RMENU 
				&& k1 != DIK_LCONTROL && k1 != DIK_LCONTROL 
				&& k1 != DIK_LWIN && k1 != DIK_RWIN 
				&& !m_pInput->IsKeyUpOnce(k1) )
			{	// ���ڸ���״̬����Ҫ���⴦������״̬������ʱ�����ֵ������²�������
				if( m_pInput->IsKeyDown(DIK_LSHIFT)
					|| m_pInput->IsKeyDown(DIK_RSHIFT) 
					|| m_pInput->IsKeyDown(DIK_LMENU) 
					|| m_pInput->IsKeyDown(DIK_RMENU) 
					|| m_pInput->IsKeyDown(DIK_LCONTROL) 
					|| m_pInput->IsKeyDown(DIK_LCONTROL) 
					|| m_pInput->IsKeyDown(DIK_LWIN) 
					|| m_pInput->IsKeyDown(DIK_LWIN) )
					continue;
			}

			if( m_pInput->IsKeyDownOnce(k1) )	// ����
				bCall = bDown = TRUE;

			if( m_pInput->IsKeyUpOnce(k1) )	// ����
				bCall = TRUE, bDown = FALSE;
		}

		if( bCall )
		{
			if( !pInfo->bEnableInEdit && m_pGUI->GetActive()->IsEditable() 
				&& bDown == TRUE )	// ����ǰ�����Ϣ����Ҫ����Ƿ񽹵㴦�ڱ༭��
				continue;

			(pInfo->pfn)(pInfo->dwID, bDown);
		}
	}// while
}


//-----------------------------------------------------------------------------
// ����ʱ�ĸ���
//-----------------------------------------------------------------------------
VOID GameInputMap::UpdateSetup()
{
	std::vector<DIK> vec;
	std::vector<DIK>::iterator it;
	BOOL bResult = m_pInput->GetKeyDown(vec);

	LPCTSTR szText = m_pWndList->GetText(m_dwCurrentSet, 0);
	if( !P_VALID(szText) )
		return;

	DWORD dwID = m_pUtil->Crc32(szText);
	tagKeyMapInfo* pInfo = m_mapKeyFunction.Peek(dwID);
	if( !P_VALID(pInfo) )
		return;

	if( !bResult && m_dwTempKey != 0 )	// ����ȫ���ɿ�
	{
		m_dwCurrentSet = GT_INVALID;	// �˳�����״̬
		pInfo->dwKey = m_dwTempKey;
		this->CheckRepeatSet(pInfo);
		this->ShowKeys();
		return;
	}

	if( vec.size() > 0 )
	{
		m_dwTempKey = vec[0];

		//һЩ�����λ�������ÿ�ݼ�
		if( m_dwTempKey==DIK_ESCAPE )
		{
			m_dwTempKey = 0;
			return;
		}

		if( vec.size() > 1 )	// ͬʱ��������
		{
			//�⼸����Ҫ���⴦��
			if( vec[1]==DIK_LSHIFT || vec[1]==DIK_RSHIFT 
				|| vec[1]==DIK_LMENU || vec[1]==DIK_RMENU 
				|| vec[1]==DIK_LCONTROL || vec[1]==DIK_LCONTROL 
				|| vec[1]==DIK_LWIN || vec[1]==DIK_RWIN  )
			{
				m_dwTempKey = vec[1];
				m_dwTempKey |= (vec[0] << 16);
			}
			else
			{
				m_dwTempKey |= (vec[1] << 16);
			}
			pInfo->dwKey = m_dwTempKey;
			m_dwCurrentSet = GT_INVALID;	// �˳�����״̬
			this->CheckRepeatSet(pInfo);
		}
		this->ShowKeys();
	}
}


//-----------------------------------------------------------------------------
// ����GUI�¼�
//-----------------------------------------------------------------------------
BOOL GameInputMap::GUIEventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pWndOK )
		{
			this->Save();
			this->UnloadGUI();
			return TRUE;
		}
		if( pWnd == m_pWndCancle || pWnd == m_pWndClose )
		{
			this->Recover();
			this->UnloadGUI();
			return TRUE;
		}
		if( pWnd == m_pWndDefault )
		{
			this->RecoverDefault();
			this->ShowKeys();
			return TRUE;
		}
	case EGUIE_PushButtonDown:
		if( pWnd == m_pWndPage1 || pWnd == m_pWndPage2 || pWnd == m_pWndPage3 )
		{
			m_dwCurrentSet = GT_INVALID;
			this->ShowKeys();
			return TRUE;
		}
		break;

	case EGUIE_ItemClick:
		if( pWnd == m_pWndList )
		{
			LPCTSTR szText = m_pWndList->GetText(pEvent->dwParam1, pEvent->dwParam2);
			if( !P_VALID(szText) )
			{
				m_dwCurrentSet = GT_INVALID;
				this->ShowKeys();
				break;
			}

			m_dwCurrentSet = pEvent->dwParam1;	// ��������״̬
			m_nCurrentTextRow = m_pWndList->GetCurrentTextRow();
			m_dwTempKey = 0;
			this->ShowKeys();
		}
		break;
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// ����˴�����
//-----------------------------------------------------------------------------
BOOL GameInputMap::Save()
{
	tagKeyMapInfo* pInfo = NULL;
	m_mapKeyFunction.ResetIterator();
	while( m_mapKeyFunction.PeekNext(pInfo) )
	{
		if( pInfo->pfnChange && pInfo->dwOriginKey != pInfo->dwKey )
		{
			// ֪ͨ��������
			(pInfo->pfnChange)(pInfo->dwID, (DIK)pInfo->dwKey, (DIK)(pInfo->dwKey>>16));
		}
		pInfo->dwOriginKey = pInfo->dwKey;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// ȡ���˴����ã��ָ��ϴ�����
//-----------------------------------------------------------------------------
VOID GameInputMap::Recover()
{
	tagKeyMapInfo* pInfo = NULL;
	m_mapKeyFunction.ResetIterator();
	while( m_mapKeyFunction.PeekNext(pInfo) )
	{
		// ֪ͨ��������
		if( pInfo->pfnChange && pInfo->dwKey != pInfo->dwOriginKey )
			(pInfo->pfnChange)(pInfo->dwID, (DIK)pInfo->dwOriginKey, (DIK)(pInfo->dwOriginKey>>16));

		pInfo->dwKey = pInfo->dwOriginKey;
	}
}


//-----------------------------------------------------------------------------
// �ָ�Ĭ������
//-----------------------------------------------------------------------------
VOID GameInputMap::RecoverDefault()
{
	tagKeyMapInfo* pInfo = NULL;
	m_mapKeyFunction.ResetIterator();
	while( m_mapKeyFunction.PeekNext(pInfo) )
	{
		// ֪ͨ��������
		if( pInfo->pfnChange && pInfo->dwKey != pInfo->dwDefaultKey )
			(pInfo->pfnChange)(pInfo->dwID, (DIK)pInfo->dwDefaultKey, (DIK)(pInfo->dwDefaultKey>>16));

		pInfo->dwKey = pInfo->dwDefaultKey;
	}
}


//-----------------------------------------------------------------------------
// ����Ƿ����ظ����õİ���������У������
//-----------------------------------------------------------------------------
VOID GameInputMap::CheckRepeatSet(tagKeyMapInfo* pInfoFind)
{
	tagKeyMapInfo* pInfo = NULL;
	m_mapKeyFunction.ResetIterator();
	while( m_mapKeyFunction.PeekNext(pInfo) )
	{
		if( !pInfo->bShow || pInfo == pInfoFind )
			continue;

		if( pInfo->dwKey == pInfoFind->dwKey )
		{
			// �����ظ�����������Ϊ��
			pInfo->dwKey = 0;
			this->ShowKeys();
			TCHAR mess[X_LONG_STRING];
			_stprintf(mess, g_StrTable[_T("CustomHotKeyRedefine")], pInfo->strDesc.c_str());

			m_pText->SetText(mess);
			m_pWnd->SetRefresh(TRUE);
		}
	}
}


//-----------------------------------------------------------------------------
// ����Ƿ����ظ����õİ���������У������
//-----------------------------------------------------------------------------
BOOL GameInputMap::LoadFromFile()
{
	TObjRef<VarContainer> pBucket = CreateObj("KeyConfig", "VarContainer");

	std::list<tstring> listName;
	if( pBucket->Load(NULL, KEY_CFG_FILE, "name", &listName) )
	{
		std::list<tstring>::iterator it;
		for(it=listName.begin();it!=listName.end();it++)
		{
			DWORD dwID = m_pUtil->Crc32((*it).c_str());
			tagKeyMapInfo* pInfo = m_mapKeyFunction.Peek(dwID);
			if( P_VALID(pInfo) )
			{
				pInfo->dwKey = pBucket->GetDword((*it).c_str());

				// ֪ͨ��������
				if( pInfo->pfnChange && pInfo->dwKey != pInfo->dwOriginKey )
					(pInfo->pfnChange)(pInfo->dwID, (DIK)pInfo->dwKey, (DIK)(pInfo->dwKey>>16));

				pInfo->dwOriginKey = pInfo->dwKey;
			}
		}
	}

	KillObj("KeyConfig");
	return TRUE;
}


//-----------------------------------------------------------------------------
// ����Ƿ����ظ����õİ���������У������
//-----------------------------------------------------------------------------
BOOL GameInputMap::SaveToFile()
{
	TObjRef<VarContainer> pBucket = CreateObj("KeyConfig", "VarContainer");
	pBucket->SetSaveSupport(TRUE);

	tagKeyMapInfo* pInfo = NULL;
	m_mapKeyFunction.ResetIterator();
	while( m_mapKeyFunction.PeekNext(pInfo) )
	{
		tstring strName = _T("value ");
		strName += pInfo->strDesc.c_str();
		pBucket->Add(pInfo->dwKey, strName.c_str());
	}

	pBucket->Save(KEY_CFG_FILE);
	KillObj("KeyConfig");
	return TRUE;
}