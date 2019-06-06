#include "StdAfx.h"
#include "GemRemovalFrame.h"
#include "ItemProtoData.h"
#include "Item.h"
#include "ItemMgr.h"
#include "ItemButton.h"
#include "Container.h"
#include "CombatSysUtil.h"
#include "IconStatic.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "IconStaticHL.h"
#include "ComposeStatic.h"
#include "RoleMgr.h"

GemRemovalFrame::GemRemovalFrame(void) : 
m_Trunk(this),
m_pWnd( NULL ),
m_pWndSure( NULL ),
m_pIconEquip( NULL ),
m_pBtnClose( NULL ),
m_pBtnOk( NULL ),
m_pBtnCancel( NULL ),
m_pBtnSureClose( NULL ),
m_pBtnSureOk( NULL ),
m_pBtnSureCancel( NULL ),
m_CurState( EGRS_Operate ),
m_n64Src( GT_INVALID ),
m_dwNPCId( GT_INVALID ),
m_nSilverCost( 0 ),
m_n64GoldCost( 0 ),
m_dwElapseTime( 0 )
{
	memset( m_pIconGems, 0, sizeof(m_pIconGems) );
	memset( m_pIconSymbols, 0, sizeof(m_pIconSymbols) );
	memset( m_Gems, 0, sizeof(m_Gems) );
	memset( m_Symbols, GT_INVALID, sizeof(m_Symbols) );
}

GemRemovalFrame::~GemRemovalFrame(void)
{

}

BOOL GemRemovalFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init( pMgr, pFather, dwParam ) )
		return FALSE;

	//--ע��������Ϣ
	m_pCmdMgr->Register("NS_GetGemRemovalInfo", (NETMSGPROC)m_Trunk.sfp2( &GemRemovalFrame::OnNS_GetRemovalInfo ), _T("NS_GetGemRemovalInfo") );
	m_pCmdMgr->Register("NS_GemRemoval", (NETMSGPROC)m_Trunk.sfp2( &GemRemovalFrame::OnNS_GetRemoval ), _T("NS_GemRemoval") );
	
	return TRUE;
}


BOOL GemRemovalFrame::Destroy()
{
	GameFrame::Destroy();
	if( P_VALID( m_pWnd ) )
		m_pGUI->AddToDestroyList( m_pWnd );

	if( P_VALID(m_pWndSure) )
		m_pGUI->AddToDestroyList( m_pWndSure );

	//--����������Ϣ
	m_pCmdMgr->UnRegister("NS_GetGemRemovalInfo", (NETMSGPROC)m_Trunk.sfp2( &GemRemovalFrame::OnNS_GetRemovalInfo ) );
	m_pCmdMgr->UnRegister("NS_GemRemoval", (NETMSGPROC)m_Trunk.sfp2( &GemRemovalFrame::OnNS_GetRemoval ) );

	return TRUE;
}

VOID GemRemovalFrame::Update()
{
	GameFrame::Update();

	m_dwElapseTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_dwElapseTime > 1000 )//ÿ��1�������Ŀ��NPC�ľ���
	{
		m_dwElapseTime = 0;
		if ( RoleMgr::Inst()->IsOutValidDist( m_dwNPCId ) || !GT_VALID(m_dwNPCId) )
		{
			m_pMgr->AddToDestroyList(this);
			return;
		}
	}
}

VOID GemRemovalFrame::OnEvent( tagGameEvent* pEvent )
{

}

BOOL GemRemovalFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\equip_embed.xml");
	m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() );
	
	//�滻Ԫ������
	for( int i=1; i<= MAX_EQUIPHOLE_NUM; i++ )
	{
		char temp[256] = {0};
		sprintf_s( temp, "embedpic\\embed_pic\\pic\\listback1\\imback%d\\im", i );
		m_pGUI->ChangeXml( &element, temp, "ClassName", "IconStaticHL" );

		memset( temp, 0, sizeof(temp) );
		sprintf_s( temp, "embedpic\\embed_pic\\pic\\listback2\\imback%d\\im", i );
		m_pGUI->ChangeXml( &element, temp, "ClassName", "IconStatic" );
	}
	
	//��Ҫ��ժ����ʯ��װ��
	m_pGUI->ChangeXml( &element, "embedpic\\embed_pic\\pic\\itemback\\item", "ClassName", "ComposeStatic" );

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1( &GemRemovalFrame::EventHandler ) );

	m_pBtnClose	= (GUIButton*)m_pWnd->GetChild( _T("embed_pic\\closebutt") );
	m_pBtnOk = (GUIButton*)m_pWnd->GetChild( _T("embed_pic\\startbutt") );
	m_pBtnCancel = (GUIButton*)m_pWnd->GetChild( _T("embed_pic\\breakbutt") );
	m_pIconEquip = (ComposeStatic*)m_pWnd->GetChild( _T("embed_pic\\pic\\itemback\\item") );
	m_pStcGold = (GUIStatic*)m_pWnd->GetChild( _T("embed_pic\\pic\\num1") );
	m_pStcSilver = (GUIStatic*)m_pWnd->GetChild( _T("embed_pic\\pic\\num2") );

	for( int i=0; i<MAX_EQUIPHOLE_NUM; i++ )
	{
		TCHAR temp[256] = {0};
		_stprintf( temp, _T("embed_pic\\pic\\listback1\\imback%d\\im"), i+1 );
		m_pIconGems[i] = (IconStaticHL*)m_pWnd->GetChild( temp );
		m_pIconGems[i]->SetRefresh( TRUE );

		memset( temp, 0, sizeof(temp) );
		_stprintf( temp, _T("embed_pic\\pic\\listback2\\imback%d\\im"), i+1 );
		m_pIconSymbols[i] = (IconStatic*)m_pWnd->GetChild( temp );
	}

	//---------------ȷ�Ͻ���-------------------------------

	strPath = g_strLocalPath + _T("\\ui\\equip_embed_sure.xml");
	m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() );
	
	m_pWndSure = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	m_pGUI->RegisterEventHandler( m_pWndSure->GetFullName().c_str(), m_Trunk.sfp1( &GemRemovalFrame::EventHandler ) );

	m_pBtnSureClose = (GUIButton*)m_pWndSure->GetChild( _T("embed_sure_pic\\closebutt") );
	m_pBtnSureOk = (GUIButton*)m_pWndSure->GetChild( _T("embed_sure_pic\\surebutt") );
	m_pBtnSureCancel = (GUIButton*)m_pWndSure->GetChild( _T("embed_sure_pic\\canclebutt") );
	m_plbGems = (GUIListBox*)m_pWndSure->GetChild( _T("embed_sure_pic\\listback1\\itemlist") );
	m_plbSymbols = (GUIListBox*)m_pWndSure->GetChild( _T("embed_sure_pic\\listback2\\itemlist") );
	m_pStcSureGold = (GUIStatic*)m_pWndSure->GetChild( _T("embed_sure_pic\\num1") );
	m_pStcSureSilver = (GUIStatic*)m_pWndSure->GetChild( _T("embed_sure_pic\\num2") );

	m_pWnd->SetInvisible( TRUE );
	m_pWndSure->SetInvisible( TRUE );

	return TRUE;
}


BOOL GemRemovalFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd*	pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnClose )
			{
				m_pFrameMgr->AddToDestroyList( this );
			}
			else if( pWnd == m_pBtnOk )
			{
				ShowSureWnd();
			}
			else if( pWnd == m_pBtnCancel )
			{
				m_pFrameMgr->AddToDestroyList( this );
			}
			else if( pWnd == m_pBtnSureClose )
			{
				m_pWndSure->SetInvisible( TRUE );
			}
			else if( pWnd == m_pBtnSureCancel )
			{
				m_pWndSure->SetInvisible( TRUE );
			}
			else if( pWnd == m_pBtnSureOk )
			{
				if( SubmitCheck() )
				{
					m_CurState = EGRS_ReceiveMsg;
					tagNC_GemRemoval gemRmEvent;
					memset( gemRmEvent.n64GemRomovalSign, GT_INVALID, sizeof(gemRmEvent.n64GemRomovalSign) );
					
					gemRmEvent.n64EquipID = m_n64Src;
					gemRmEvent.dwNPCID = m_dwNPCId;
					gemRmEvent.n16GemNum = GetWantGemNum( gemRmEvent );

					m_pSession->Send( &gemRmEvent );
					m_pWndSure->SetInvisible( TRUE );
				}
			}
			else
			{
				for( int i=0; i < MAX_EQUIPHOLE_NUM; i++ )
				{
					if( !P_VALID(m_pIconGems[i]) )								//��ʯ��λ�Ŀؼ�������
						continue;

					if( pWnd == m_pIconGems[i] )								//��������Ǳ�ʯ��λ
					{
						if( m_Gems[i] == 0 )									//����λû�б�ʯ
							continue;

						if( m_pIconGems[i]->IsHighLight() )
						{
							m_pIconGems[i]->SetHighLightPic( NULL );			//����Ѿ�����ѡ��״̬�������ʾȡ��ѡ��
							if( P_VALID(m_pIconSymbols[i]) )
								m_pIconSymbols[i]->SetInvisible( TRUE );
							
							m_n64GoldCost -= 10;
						}
						else	//ѡ��
						{
							m_pIconGems[i]->SetHighLightPic( _T("data\\ui\\Bag\\quality_yellow.dds") );
							m_pIconGems[i]->SetRefresh( TRUE );
							if( P_VALID(m_pIconSymbols[i]) && GT_VALID(m_Symbols[i]) )
								m_pIconSymbols[i]->SetInvisible( FALSE );
							
							m_n64GoldCost += 10;
						}
						
						if( P_VALID(m_pWndSure) )
							ShowSureWnd();

						DisplayGoldCost( m_n64GoldCost, m_nSilverCost );
					}
				}
			}
		}
		break;
	case EOnEventSendDropGUI_Item:
		{
			if( pWnd == (GUIWnd*)m_pIconEquip )
			{
				//���ڵȴ���Ϣ����,Ϊ�˷�ֹ��ʾ����,���ܸ���ժ��Ŀ��
				if( m_CurState != EGRS_Operate )
					break;
				
				//ֻ�б����е���Ʒ���ܽ��б�ʯ�������
				if( EDT_Item == pEvent->dwParam1 )
				{
					//ֻ��װ�����ܽ��б�ʯ�������
					Item* pItem = ItemMgr::Inst()->GetPocket()->GetValue( (INT16)LOWORD(pEvent->dwParam2) );
					if( !P_VALID(pItem) || !MIsEquipment( pItem->GetItemTypeID()) )
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("GemRemoval_NoEquipment")] );
						break;
					}
					
					//�ѵ�����Ʒ�϶�����װ��
					if( HIWORD(pEvent->dwParam2) > 1)		
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("GemRemoval_TooMuch")]);
						break;
					}

					ClearAll();
					m_n64Src = pItem->GetItemId();

					TCHAR szPath[X_LONG_NAME] = {0};
					_sntprintf(szPath, sizeof(szPath), _T("data\\ui\\Icon\\%s.tga"), pItem->GetDisplay()->szMaleIcon );
					m_pIconEquip->SetPic( szPath );
					
					m_CurState = EGRS_ReceiveMsg;			//�ȴ���Ϣ����

					tagNC_GetGemRemovalInfo ncEvent;
					ncEvent.dwNPCID = m_dwNPCId;
					ncEvent.n64EquipID = m_n64Src;
					m_pSession->Send( &ncEvent );
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if( pWnd == m_pIconEquip )
			{
				Item* pItem = ItemMgr::Inst()->GetPocketItemByID( m_n64Src );
				if( P_VALID(pItem) )
				{
					tstring strTips = ToolTipCreator::Inst()->GetItemTips( pItem );
					ShowTip(pWnd, strTips.c_str() );
				}
			}
			else
			{
				for( int i=0; i< MAX_EQUIPHOLE_NUM; i++ )
				{
					if( pWnd == m_pIconGems[i] )
					{
						if( m_Gems[i] != 0 )
						{
							tstring strTips = ToolTipCreator::Inst()->GetItemTips(m_Gems[i]);
							if( !strTips.empty() )
								ShowTip( pWnd, strTips.c_str() );
						}
					}
					else if( pWnd == m_pIconSymbols[i] )
					{
						if( !GT_VALID(m_Symbols[i]) )
							break;

						Item* pItem = ItemMgr::Inst()->GetPocketItemByID( m_Symbols[i] );
						if( !P_VALID(pItem) )
							break;

						tstring strTips = ToolTipCreator::Inst()->GetItemTips( pItem );
						if( !strTips.empty() )
							ShowTip( pWnd, strTips.c_str() );
					}
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip( pWnd, NULL );
		}
		break;
	}
	
	return FALSE;
}

VOID GemRemovalFrame::ShowSureWnd()
{
	if( !GT_VALID(m_n64Src) )	//���û�з�ֹ�κ�Ҫժ����ʯ��װ��������
		return;
	
	m_plbGems->SetColNum( 1, 300 );
	m_plbSymbols->SetColNum(1, 300 );

	INT nGemNum = 0;
	for( int i=0; i<MAX_EQUIPHOLE_NUM; i++ )
	{
		//���ϴ���ʾ�ļ�¼���
		if( P_VALID(m_plbGems) )
			m_plbGems->SetText( i, 0, _T("") );
		
		if( P_VALID(m_plbGems) )
			m_plbSymbols->SetText( i, 0, _T("") );

		//û�п���ʾ�Ķ���
		if( m_Gems[i] == 0 || !GT_VALID(m_Symbols[i]) || !P_VALID(m_pIconGems[i]) || !m_pIconGems[i]->IsHighLight() )
			continue;
		
		nGemNum++;

		//���ñ�ʯ���ֺ����ĵĲ����������
		const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto( m_Gems[i] );
		if( P_VALID(pProto) && P_VALID(m_plbGems) )
			m_plbGems->SetText( i, 0, pProto->pDisplayInfo->szName );

		Item* pItem = ItemMgr::Inst()->GetPocketItemByID( m_Symbols[i] );
		if( P_VALID(pItem) && P_VALID(m_plbSymbols) )
			m_plbSymbols->SetText( i, 0, pItem->GetItemName().c_str() );
	}
	
	//�����ԭ���������ӵ�����������ʾ�µ�����
	m_pStcSureSilver->SetText( _T("") );
	if( GT_VALID(m_nSilverCost) )
	{
		TCHAR szTmp[100] = {0};
		_itot( m_nSilverCost, szTmp, 10 );
		m_pStcSureSilver->SetText( szTmp );
	}
	
	//�����ԭ�����Ľ��ӵ�����������ʾ�µ�����
	m_pStcSureGold->SetText( _T("") );
	if( GT_VALID(m_n64GoldCost) )
	{
		TCHAR szTmp[100] = {0};
		_i64tot( m_n64GoldCost, szTmp, 10 );
		m_pStcSureGold->SetText( szTmp );
	}

	if( nGemNum > 0 )
	{
		m_pWndSure->SetInvisible( FALSE );
		m_pWndSure->SetRefresh( TRUE );
		m_pWndSure->FlipToTop();
	}
	else
	{
		m_pWndSure->SetInvisible( TRUE );
	}
}

DWORD GemRemovalFrame::OnNS_GetRemoval( tagNS_GemRemoval* pMsg, LPVOID pParam )
{
	if( !P_VALID(pMsg) )
	{
		m_pFrameMgr->AddToDestroyList( this );
		return 0;
	}

	if( pMsg->dwErrorCode == E_Compose_Consolidate_Success )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("GemRemoval_Success")] );
		ClearAll();
		return 0;
	}

	switch( pMsg->dwErrorCode )
	{
	case E_Compose_NotEnough_Money:
		CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("GemRemoval_LackOfMoney")] );
		break;

	case E_Compose_Bag_Full:
		CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("GemRemoval_BagFull")] );
		break;

	case E_Gem_NPCCanNotRemoveGem:
		CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("GemRemoval_IsNotGemRmNPC")] );
		break;

	default:
		CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("GemRemoval_Failed")] );
		break; 
	}

	return 0;
}


DWORD GemRemovalFrame::OnNS_GetRemovalInfo( tagNS_GetGemRemovalInfo* pMsg, LPVOID pPrama )
{
	if( pMsg->dwErrCode != E_Success )
	{
		
		m_CurState = EGRS_Operate;
		return 0;
	}
	
	INT16 nGemNum = pMsg->n16GemNum;
	if( nGemNum > MAX_EQUIPHOLE_NUM )
		nGemNum = MAX_EQUIPHOLE_NUM;
	
	//��ʼ����Ҳ����˵������ϴβ����������ۼ�
	m_lstGmRmSymbolInfo.clear();
	memset( m_Gems, 0, sizeof(m_Gems) );
	memset( m_Symbols, GT_INVALID, sizeof(m_Symbols) );
	
	//������Ϣ�������¸�ֵ
	for( INT16 i=0; i< MAX_EQUIPHOLE_NUM; i++ )
	{
		m_Gems[i] = pMsg->dwGemIDs[i];
		
		const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto( m_Gems[i] );
		if( !P_VALID(pItemProto) )
		{
			//���ñ�ʯ��ͼƬΪ��
			IconStatic* pStcIcon = m_pIconGems[i];
			if( !P_VALID(pStcIcon) )
				pStcIcon->SetPic( _T("") );

			continue;
		}

		INT n = pItemProto->byLevel;
		
		//���ñ�ʯ��ͼƬ
		IconStaticHL* pStcIcon = m_pIconGems[i];
		if( P_VALID(pStcIcon) )
		{
			TCHAR szPath[X_LONG_NAME] = {0};
			_sntprintf(szPath, sizeof(szPath), _T("data\\ui\\Icon\\%s.tga"), pItemProto->pDisplayInfo->szMaleIcon );
			pStcIcon->SetPic( szPath );
		}
	}

	//�����뱦ʯ��Ӧ�� ��ʯ������� ��Ϣ
	std::list<Item*> lst;
	ItemMgr::Inst()->GetPocket()->GetSpecFuncMap( EISF_GemRemovalSign, lst );
	CensusGemRmSignal( lst );

	for( INT16 i=0; i< MAX_EQUIPHOLE_NUM; i++ )
	{
		const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto( m_Gems[i] );
		if( !P_VALID(pItemProto) )
			continue;

		std::list<tagGemRmSymbolInfo>::iterator iter = m_lstGmRmSymbolInfo.begin();
		for( ; iter != m_lstGmRmSymbolInfo.end(); ++iter )
		{
			BYTE n = pItemProto->byLevel;
			INT n1 = iter->nLevel;
			INT nQ = iter->nQuantity;
			if( iter->nLevel != pItemProto->byLevel || iter->nQuantity <= 0 )
			{
				continue;
			}
			else
			{
				m_Symbols[i] = iter->n64ItemID;
				iter->nQuantity -= 1;
				m_n64GoldCost += 10;

				if( P_VALID(m_pIconSymbols[i]) )
				{
					Item* pItem = ItemMgr::Inst()->GetPocketItemByID( m_Symbols[i] );
					if( P_VALID( pItem ) )
					{
						TCHAR szPath[X_LONG_NAME] = {0};
						_sntprintf(szPath, sizeof(szPath), _T("data\\ui\\Icon\\%s.tga"), pItem->GetDisplay()->szMaleIcon );
						m_pIconSymbols[i]->SetPic( szPath );
						m_pIconSymbols[i]->SetRefresh( TRUE );
					}
					m_pIconGems[i]->SetHighLightPic( _T("data\\ui\\Bag\\quality_yellow.dds") );
					m_pIconGems[i]->SetRefresh( TRUE );
				}
				break;
			}
		}
	}
	
	DisplayGoldCost( m_n64GoldCost,m_nSilverCost );

	m_CurState = EGRS_Operate;
	return 0;
}

VOID GemRemovalFrame::CensusGemRmSignal( std::list<Item*>& lst )
{
	std::list<Item*>::iterator iter = lst.begin();
	for( ; iter != lst.end(); ++iter )
	{
		Item* pItem = (*iter);
		if( P_VALID(pItem) )
		{
			tagGemRmSymbolInfo info;
			info.n64ItemID = pItem->GetItemId();
			info.nLevel = pItem->GetItemLevel();
			info.nQuantity = pItem->GetItemQuantity();
			m_lstGmRmSymbolInfo.push_back(info);
		}
	}
}

INT16 GemRemovalFrame::GetWantGemNum( tagNC_GemRemoval& evt )
{
	INT16 nNum = 0;
	for( int i=0; i<MAX_EQUIPHOLE_NUM; i++ )
	{
		if( m_Gems[i] == 0 )
			continue;

		if( P_VALID(m_pIconGems[i]) && m_pIconGems[i]->IsHighLight() && GT_VALID(m_Symbols[i]) )
		{
			nNum++;
			evt.n64GemRomovalSign[i] = m_Symbols[i];
		}
	}

	return nNum;
}

bool GemRemovalFrame::SubmitCheck()
{
	if( P_VALID( GetObj("GemRemoval_MsgBox") ) )
		KillObj( "GemRemoval_MsgBox" );
	CreateObj( "GemRemoval_MsgBox", "MsgBox" );

	if( !GT_VALID(m_n64Src) )
	{
		TObjRef<MsgBox>("GemRemoval_MsgBox")->Init( _T(""), g_StrTable[_T("GemRemoval_EquipIsLawless")], _T("tagMsgBoxEvent"), MBF_OK, TRUE );
		return false;
	}

	if( !GT_VALID(m_dwNPCId) )
	{
		TObjRef<MsgBox>("GemRemoval_MsgBox")->Init( _T(""), g_StrTable[_T("Common_Error28")], _T("tagMsgBoxEvent"), MBF_OK, TRUE );
		return false;
	}


	//���ѡ���ı�ʯ�Ƿ�Ϸ�
	int nGemNum = 0;
	for( int i=0; i<MAX_EQUIPHOLE_NUM; i++ )
	{
		if( m_Gems[i] != 0 && GT_VALID(m_Symbols[i]) && P_VALID(m_pIconGems[i]) && m_pIconGems[i]->IsHighLight() )
			nGemNum++;
	}

	if( nGemNum <= 0 )
	{
		TObjRef<MsgBox>("GemRemoval_MsgBox")->Init( _T(""), g_StrTable[_T("GemRemoval_LackOfSymbol")], _T("tagMsgBoxEvent"), MBF_OK, TRUE );
		return false;
	}

	return true;
}

void GemRemovalFrame::ClearAll()
{
	memset( m_Gems, 0, sizeof(m_Gems) );
	memset( m_Symbols, GT_INVALID, sizeof(m_Symbols) );
	m_CurState = EGRS_Operate;
	m_n64Src = GT_INVALID;
	m_n64GoldCost = 0;
	m_nSilverCost = 0;
	m_lstGmRmSymbolInfo.clear();

	if( P_VALID(m_pIconEquip) )
	{
		m_pIconEquip->SetPic( _T("") );
	}

	for( int i=0; i< MAX_EQUIPHOLE_NUM; i++ )
	{
		if( P_VALID(m_pIconGems[i]) )
		{
			m_pIconGems[i]->SetPic( _T("") );
			m_pIconGems[i]->SetHighLightPic( NULL );
			m_pIconGems[i]->SetInvisible( FALSE );
		}

		if( P_VALID(m_pIconSymbols[i]) )
		{
			m_pIconSymbols[i]->SetPic( _T("") );
			m_pIconGems[i]->SetInvisible( FALSE );
		}
	}
}

void GemRemovalFrame::DisplayGoldCost( INT nGoldCost, INT nSilver )
{
	TCHAR szCost[X_SHORT_NAME] = {0};
	_stprintf( szCost, _T("%d"),nGoldCost );

	if( P_VALID(m_pStcGold) )
	{
		m_pStcGold->SetText( szCost );
		m_pStcGold->SetRefresh( TRUE );
	}

	if( P_VALID(m_pStcSureGold) )
	{
		m_pStcSureGold->SetText( szCost );
		m_pStcSureGold->SetRefresh( TRUE );
	}

	_stprintf( szCost, _T("%d"), nSilver );
	if( P_VALID(m_pStcSilver) )
	{
		m_pStcSilver->SetText( szCost );
		m_pStcSilver->SetRefresh( TRUE );
	}

	if( P_VALID(m_pStcSureSilver) )
	{
		m_pStcSureSilver->SetText( szCost );
		m_pStcSureSilver->SetRefresh( TRUE );
	}
}