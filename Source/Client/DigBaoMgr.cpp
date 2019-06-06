#include "stdafx.h"
#include "DigBaoMgr.h"
#include "CombatSysUtil.h"
#include "EffectMgr.h"
#include "..\WorldDefine\msg_digbao.h"
#include "RoleMgr.h"
#include "CombatSysUtil.h"
#include "PlayerNameTab.h"
#include "ChatFrame.h"
#include "MapLogicData.h"
#include "ItemProtoData.h"
#include "CreatureData.h"
#include "..\WorldDefine\specfunc_define.h"
#include "ItemMgr.h"
#include "Item.h"

DigBaoMgr::DigBaoMgr( void ):m_Trunk(this)
{
	
}

DigBaoMgr::~DigBaoMgr( void )
{

}

DigBaoMgr g_DigBaoMgr;
DigBaoMgr* DigBaoMgr::Inst()
{
	return &g_DigBaoMgr;
}

void DigBaoMgr::Init()
{
	m_pUtil =  TObjRef<Util>();
	m_pCmdMgr = TObjRef<NetCmdMgr>();
	m_pSession = TObjRef<NetSession>();
	m_pFrameMgr = TObjRef<GameFrameMgr>();

	//--ע����Ϸ�¼�
	m_pFrameMgr->RegisterEventHandle(_T("tagDigBaoEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&DigBaoMgr::OnDigBaoEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagDungeonEnterEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&DigBaoMgr::OnDungeonEnterEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagGodsOfPracticeEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&DigBaoMgr::OnGodsOfPracticeEvent));
	m_pFrameMgr->RegisterEventHandle(_T("ItemEvent_UnlockChest"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&DigBaoMgr::OnTheRoleOfItemEvent));

	//--ע��������Ϣ
	m_pCmdMgr->Register("NS_DiaBao",			(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_DiaBao),				_T("NS_DiaBao"));
	m_pCmdMgr->Register("NS_NormalTreasureMap",	(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_NormalTreasureMap),	_T("NS_NormalTreasureMap"));
	m_pCmdMgr->Register("NS_OldTreasureMap",	(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_OldTreasureMap),		_T("NS_OldTreasureMap"));
	m_pCmdMgr->Register("NS_RareTreasureMap",	(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_RareTreasureMap),		_T("NS_RareTreasureMap"));

	m_pCmdMgr->Register("NS_ChatBillike",		(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_ChatBillike),			_T("NS_ChatBillike"));
	m_pCmdMgr->Register("NS_ChatBadOldSeal",	(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_ChatBadOldSeal),		_T("NS_ChatBadOldSeal"));
	m_pCmdMgr->Register("NS_Plutus",			(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_Plutus),				_T("NS_Plutus"));
	m_pCmdMgr->Register("NS_AwakenGods",		(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_AwakenGods),			_T("NS_AwakenGods"));
	m_pCmdMgr->Register("NS_GodsOfPractice",	(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_GodsOfPractice),		_T("NS_GodsOfPractice"));
	m_pCmdMgr->Register("NS_MonsterAttackCity",	(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_MonsterAttackCity),	_T("NS_MonsterAttackCity"));
	m_pCmdMgr->Register("NS_RoleGetChestItem",	(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_RoleGetChestItem),	_T("NS_RoleGetChestItem"));
}

void DigBaoMgr::Destroy()
{
	//--ע����Ϸ�¼�
	m_pFrameMgr->UnRegisterEventHandler(_T("tagDigBaoEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&DigBaoMgr::OnDigBaoEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagDungeonEnterEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&DigBaoMgr::OnDungeonEnterEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagGodsOfPracticeEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&DigBaoMgr::OnGodsOfPracticeEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_UnlockChest"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&DigBaoMgr::OnTheRoleOfItemEvent));

	//--ע��������Ϣ
	m_pCmdMgr->UnRegister("NS_DiaBao",				(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_DiaBao));
	m_pCmdMgr->UnRegister("NS_NormalTreasureMap",	(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_NormalTreasureMap));
	m_pCmdMgr->UnRegister("NS_OldTreasureMap",		(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_OldTreasureMap));
	m_pCmdMgr->UnRegister("NS_RareTreasureMap",		(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_RareTreasureMap));

	m_pCmdMgr->UnRegister("NS_ChatBillike",			(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_ChatBillike));
	m_pCmdMgr->UnRegister("NS_ChatBadOldSeal",		(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_ChatBadOldSeal));
	m_pCmdMgr->UnRegister("NS_Plutus",				(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_Plutus));
	m_pCmdMgr->UnRegister("NS_AwakenGods",			(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_AwakenGods));
	m_pCmdMgr->UnRegister("NS_GodsOfPractice",		(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_GodsOfPractice));
	m_pCmdMgr->UnRegister("NS_MonsterAttackCity",	(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_MonsterAttackCity));
	m_pCmdMgr->UnRegister("NS_RoleGetChestItem",	(NETMSGPROC)m_Trunk.sfp2(&DigBaoMgr::OnNS_RoleGetChestItem));

	//--
	if( P_VALID(GetObj("DungeonEnterMsgBox")) )
		KillObj("DungeonEnterMsgBox");
	if( P_VALID(GetObj("GodsOfPracticeMsgBox")) )
		KillObj("GodsOfPracticeMsgBox");
}

void DigBaoMgr::Update()
{
	if( m_vecDigBaoCast.size()==0 )
		return;
	vector<tagDigBaoCast>::iterator iter = m_vecDigBaoCast.begin();
	if( iter!=m_vecDigBaoCast.end() )
	{
		OnDigBaoCast(*iter);
		iter = m_vecDigBaoCast.erase(iter);
	}
}

DWORD DigBaoMgr::OnDigBaoEvent( tagDigBaoEvent* pGameEvent )
{
	tagNC_DiaBao msg;
	msg.dwPlayerID = RoleMgr::Inst()->GetLocalPlayerID();
	msg.dwItemID = pGameEvent->dwTargetID;
	msg.dwItemTypdID = pGameEvent->dwTargetTypeID;
	m_pSession->Send(&msg);
	return 0;
}

DWORD DigBaoMgr::OnNS_DiaBao( tagNS_DiaBao* pMsg, DWORD dwParam )
{
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
		{
			EffectMgr::Inst()->PlayEffect( pMsg->v3Pos, 0.0f, _T("txqt08"), NULL, EffectMgr::EEST_XYZ );
		}
		break;
	default:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(_T("�������쳣."));
		break;
	}
	return 0;
}
// ��ͨ
DWORD DigBaoMgr::OnNS_NormalTreasureMap( tagNS_NormalTreasureMap* pMsg, DWORD dwParam )
{
	switch(pMsg->eType)
	{
	case E_Success:
		break;
	case ENTM_Nothing:				//һ������
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao0")]);
		break;
	case ENTM_GainMoney:			//��Ǯ����
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao1")]);
		break;
	case ENTM_ConfrontTrap:			//��������
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao2")]);
		break;
	case ENTM_Billiken:				//�����·� �㲥
		
		break;
	case ENTM_GainOlMap:			//��ù��ϲر�ͼ
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao1")]);
		break;
	case ENTM_GainKey:				//���Կ��
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao1")]);
		break;
	case ENTM_GainOldChest:			//��þ�����
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao1")]);
		break;
	case ENTM_DriftBottle:			//Ư��ƿ
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao1")]);
		break;
	case ENTM_GainLoongSquamaChest://������۱���
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao1")]);
		break;
	case ENTM_BadOldSeal:			//�ڻ��Ϲŷ�ӡ �㲥
		
		break;
	case ENTM_Plutus:				//�����·� �㲥
		
		break;
	default:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(_T("�������쳣."));
		break;
	}
	return 0;
}

// �Ϲ�
DWORD DigBaoMgr::OnNS_OldTreasureMap( tagNS_OldTreasureMap* pMsg, DWORD dwParam )
{
	switch(pMsg->eType)
	{
	case E_Success:
		break;
	case EOTM_Billiken:					//�����·� �㲥
	
		break;
	case EOTM_GainLoongSquamaChest:		//������۱���
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao1")]);
		break;
	case EOTM_GainChest:				//�������
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao1")]);
		break;
	case EOTM_GainLoongChest:			//�����������
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao1")]);
		break;
	case EOTM_GainFourTreasures:		//����ķ��ı�
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao1")]);
		break;
	case EOTM_GainKey:					//���Կ��
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao3")]);
		break;
	case EOTM_GainRareTreaMap:			//��þ����ر�ͼ
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao1")]);
		break;
	case EOTM_Plutus:					//�ڵ�����ү �㲥
		
		break;
	case EOTM_DungeonEnter:				//�����ܾ���� ��������FB
		{
			if( P_VALID(GetObj("DungeonEnterMsgBox")) )
				KillObj("DungeonEnterMsgBox");
			CreateObj("DungeonEnterMsgBox", "MsgBox");
			
			TObjRef<MsgBox>("DungeonEnterMsgBox")->Init(_T(""), g_StrTable[_T("DigBao4")], 
				_T("tagDungeonEnterEvent"), (MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE);
		}
		break;
	case EOTM_BadLoongPulse:			//�������	�㲥
		
		break;
	case EOTM_BadOldSeal:				//�ڻ��Ϲŷ�ӡ �㲥
		
		break;
	case EOTM_AwakenGods:				//�����Ϲ�֮�� �㲥
		
		break;
	default:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(_T("�������쳣."));
		break;
	}
	return 0;
}

// ����
DWORD DigBaoMgr::OnNS_RareTreasureMap( tagNS_RareTreasureMap* pMsg, DWORD dwParam )
{
	switch(pMsg->eType)
	{
	case E_Success:
		break;
	case ERTM_GainMoney:				//��Ǯ����
		
		break;
	case ERTM_GainRareStuff:			//���ϡ�в�������
		
		break;
	case ERTM_GainDelicacyChest:		//��þ��µ�����
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao1")]);
		break;
	case ERTM_GainLoongChest:			//�����������
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao1")]);
		break;
	case ERTM_GainLoongSquamaChest:		//������۱���
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DigBao1")]);
		break;
	case ERTM_GodsOfPractice:			//��֮���� ��ʾ����
		{
			if( P_VALID(GetObj("GodsOfPracticeMsgBox")) )
				KillObj("GodsOfPracticeMsgBox");
			CreateObj("GodsOfPracticeMsgBox", "MsgBox");

			TObjRef<MsgBox>("GodsOfPracticeMsgBox")->Init(_T(""), g_StrTable[_T("DigBao5")], 
				_T("tagGodsOfPracticeEvent"), (MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE);
		}
		break;
	case ERTM_MonsterAttackCity:		//���Ź��﹥�ǻ �㲥
		
		break;
	case ERTM_DungeonEnter:				//�����ܾ���� ��������FB
		{
			if( P_VALID(GetObj("DungeonEnterMsgBox")) )
				KillObj("DungeonEnterMsgBox");
			CreateObj("DungeonEnterMsgBox", "MsgBox");

			TObjRef<MsgBox>("DungeonEnterMsgBox")->Init(_T(""), g_StrTable[_T("DigBao4")], 
				_T("tagDungeonEnterEvent"), (MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE);
		}		
		break;
	case ERTM_AwakenGods:				//�����Ϲ�֮��
		
		break;
	default:
		break;
	}
	return 0;
}

// ��������FB
DWORD DigBaoMgr::OnDungeonEnterEvent( tagMsgBoxEvent* pGameEvent )
{
	if( pGameEvent->eResult == MBF_OK )
	{
		tagNC_EnterAdventureFB msg;
		m_pSession->Send(&msg);
	}
	return 0;
}

// ����
DWORD DigBaoMgr::OnGodsOfPracticeEvent( tagMsgBoxEvent* pGameEvent )
{
	if( pGameEvent->eResult == MBF_OK )
	{
		tagNC_Experience msg;
		m_pSession->Send(&msg);
	}
	return 0;
}

// ������
DWORD DigBaoMgr::OnTheRoleOfItemEvent(tagOnTheRoleOfItemEvent* pGameEvent)
{
	// ����Կ�ײ���
	if( pGameEvent->eSpecFuncSrc!=EISF_ChestKey )
		return 0; 

	//���Ǳ��������Ҳ����
	if( pGameEvent->eTypeSrc!=EICT_Bag
		|| pGameEvent->eTypeDst!=EICT_Bag)
		return 0;

	Item* pChest = ItemMgr::Inst()->GetPocketItem(pGameEvent->n16IndexDst);
	if( P_VALID(pChest) && pChest->GetItemSpecFunc()!=EISF_Chest )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("DigBao13")]);
		return 0;
	}
	Item* pKey = ItemMgr::Inst()->GetPocketItem(pGameEvent->n16IndexSrc);
	if( !P_VALID(pKey) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("DigBao14")]);
		return 0;
	}
	const tagItemProto* pItemCehst = ItemProtoData::Inst()->FindItemProto(pGameEvent->dwTypeIDDst);
	if( P_VALID(pItemCehst) && pItemCehst->nSpecFuncVal2!=pGameEvent->dwTypeIDSrc )
	{ 
		TCHAR szBuff[X_LONG_NAME] = {0};
		_sntprintf( szBuff, X_LONG_NAME, g_StrTable[_T("DigBao21")], pKey->GetItemName(), pChest->GetItemName() );
		CombatSysUtil::Inst()->ShowScreenCenterMsg(szBuff);
		return 0;
	}
	else if( !P_VALID(pItemCehst) )
	{
		return 0;
	}

	tagNC_TreasureChest msg;
	msg.n64ChestID	= pChest->GetItemId();
	msg.n64KeyID	= pKey->GetItemId();
	m_pSession->Send(&msg);

	return 0;
}

//�����·��㲥 �������㲥����XX�ڱ����˵����������·���
DWORD DigBaoMgr::OnNS_ChatBillike( tagNS_ChatBillike* pMsg, DWORD dwParam )
{
	ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
	if( P_VALID(pFrame) )
	{
		tstring strPlayerName = PlayerNameTab::Inst()->FindNameByID(pMsg->dwRoleID);
		if( strPlayerName.empty() )
		{
			tagDigBaoCast cast;
			cast.eType = EDBCT_ChatBillike;
			cast.dwRoleID = pMsg->dwRoleID;
			m_vecDigBaoCast.push_back(cast);
			return 0;
		}
		TCHAR szBuff[X_LONG_NAME] = {0};
		_stprintf( szBuff, g_StrTable[_T("DigBao6")], strPlayerName.c_str() );
		pFrame->PushScrollText(szBuff);
	}
	return 0;
}

//�ڻ��Ϲŷ�ӡ�㲥 �������㲥����XX�ڻ����Ϲŷ�ӡ���Ϲŵ�������XXX�����˼䣬���·Ӣ����ȥն����ħ����
DWORD DigBaoMgr::OnNS_ChatBadOldSeal( tagNS_ChatBadOldSeal* pMsg, DWORD dwParam )
{
	ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
	if( P_VALID(pFrame) )
	{
		tstring strPlayerName = PlayerNameTab::Inst()->FindNameByID(pMsg->dwRoleID);
		if( strPlayerName.empty() )
		{
			tagDigBaoCast cast;
			cast.eType = EDBCT_ChatBadOldSeal;
			cast.dwRoleID = pMsg->dwRoleID;
			cast.dwMapCrc = pMsg->dwMapCrc;
			m_vecDigBaoCast.push_back(cast);
			return 0;
		}
		tstring strMap = MapLogicData::Inst()->FindMapShowName(pMsg->dwMapCrc);
		TCHAR szBuff[X_LONG_NAME] = {0};
		_stprintf( szBuff, g_StrTable[_T("DigBao7")], strPlayerName.c_str(), strMap.c_str() );
		pFrame->PushScrollText(szBuff);
	}
	return 0;
}

//�����·��㲥 �������㲥����XX�ڱ����˵����������·���
DWORD DigBaoMgr::OnNS_Plutus( tagNS_Plutus* pMsg, DWORD dwParam )
{
	ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
	if( P_VALID(pFrame) )
	{
		tstring strPlayerName = PlayerNameTab::Inst()->FindNameByID(pMsg->dwRoleID);
		if( strPlayerName.empty() )
		{
			tagDigBaoCast cast;
			cast.eType = EDBCT_Plutus;
			cast.dwRoleID = pMsg->dwRoleID;
			m_vecDigBaoCast.push_back(cast);
			return 0; 
		}
		TCHAR szBuff[X_LONG_NAME] = {0};
		_stprintf( szBuff, g_StrTable[_T("DigBao8")], strPlayerName.c_str() );
		pFrame->PushScrollText(szBuff);
	}
	return 0;
}

//�����Ϲ�֮�� �������㲥��XXX�ڱ��������Ϲ����飬���������˼俼�������������
DWORD DigBaoMgr::OnNS_AwakenGods( tagNS_AwakenGods* pMsg, DWORD dwParam )
{
	ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
	if( P_VALID(pFrame) )
	{
		tstring strPlayerName = PlayerNameTab::Inst()->FindNameByID(pMsg->dwRoleID);
		if( strPlayerName.empty() )
		{
			tagDigBaoCast cast;
			cast.eType = EDBCT_AwakenGods;
			cast.dwRoleID = pMsg->dwRoleID;
			m_vecDigBaoCast.push_back(cast);
			return 0;
		}
		TCHAR szBuff[X_LONG_NAME] = {0};
		_stprintf( szBuff, g_StrTable[_T("DigBao9")], strPlayerName.c_str() );
		pFrame->PushScrollText(szBuff);
	}
	return 0;
}

//��֮���� ����BOSS��ɱ����ȫ�����棺��XXX���ڱ��������ң���ս###��BOSS���ƣ��ɹ��������XXX��
DWORD DigBaoMgr::OnNS_GodsOfPractice( tagNS_GodsOfPractice* pMsg, DWORD dwParam )
{
	ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
	if( P_VALID(pFrame) )
	{
		tstring strPlayerName = PlayerNameTab::Inst()->FindNameByID(pMsg->dwRoleID);
		if( strPlayerName.empty() )
		{
			tagDigBaoCast cast;
			cast.eType = EDBCT_GodsOfPractice;
			cast.dwRoleID = pMsg->dwRoleID;
			cast.dwNpcTypeID = pMsg->dwNpcTypeID;
			cast.dwItemTypeID = pMsg->dwItemTypeID;
			m_vecDigBaoCast.push_back(cast);
			return 0;
		}
		const tagCreatureProto* pCreature = CreatureData::Inst()->FindNpcAtt(pMsg->dwNpcTypeID);
		if( !P_VALID(pCreature) )
			return 0;
		tstring strNPC = pCreature->szName;

		const tagItemDisplayInfo* pItem = ItemProtoData::Inst()->FindItemDisplay(pMsg->dwItemTypeID);
		if( !P_VALID(pItem) ) 
			return 0;
		tstring strItem = pItem->szName;

		TCHAR szBuff[X_LONG_NAME] = {0};
		_stprintf( szBuff, g_StrTable[_T("DigBao10")], strPlayerName.c_str(), strNPC.c_str(), strItem.c_str() );
		pFrame->PushScrollText(szBuff);
	}
	return 0;
}

//���Ź��﹥�ǻ ϵͳ������ʾ����###������������Ĺ������5���Ӻ��##�ǽ��б���������Э��������
DWORD DigBaoMgr::OnNS_MonsterAttackCity( tagNS_MonsterAttackCity* pMsg, DWORD dwParam )
{
	ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
	if( P_VALID(pFrame) )
	{
		tstring strPlayerName = PlayerNameTab::Inst()->FindNameByID(pMsg->dwRoleID);
		if( strPlayerName.empty() )
		{
			tagDigBaoCast cast;
			cast.eType = EDBCT_MonsterAttackCity;
			cast.dwRoleID = pMsg->dwRoleID;
			cast.dwMapCrc = pMsg->dwMapCrc;
			m_vecDigBaoCast.push_back(cast);
			return 0;
		}
		
		tstring strMap = MapLogicData::Inst()->FindMapShowName(pMsg->dwMapCrc);
		TCHAR szBuff[X_LONG_NAME] = {0};
		_stprintf( szBuff, g_StrTable[_T("DigBao11")], strPlayerName.c_str(), strMap.c_str() );
		pFrame->PushScrollText(szBuff);
	}
	return 0;
}

// �㲥���俪������Ʒ
DWORD DigBaoMgr::OnNS_RoleGetChestItem(tagNS_RoleGetChestItem* pMsg, DWORD dwParam)
{
	ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
	if( P_VALID(pFrame) )
	{
		tstring strPlayerName = PlayerNameTab::Inst()->FindNameByID(pMsg->dwRoleID);
		if( strPlayerName.empty() )
		{
			tagDigBaoCast cast;
			cast.eType = EDBCT_RoleGetChestItem;
			cast.dwRoleID = pMsg->dwRoleID;
			cast.dwMapCrc = pMsg->dwItemTypeID;
			m_vecDigBaoCast.push_back(cast);
			return 0;
		}
		const tagItemDisplayInfo* pItem = ItemProtoData::Inst()->FindItemDisplay(pMsg->dwItemTypeID);
		TCHAR szBuff[X_LONG_NAME] = {0};
		_stprintf( szBuff, g_StrTable[_T("DigBao12")], strPlayerName.c_str(), pItem->szName );
		//pFrame->PushScrollText(szBuff);
		pFrame->PushInfo(szBuff, ESCC_Affiche);

	}
	return 0;
}

void DigBaoMgr::OnDigBaoCast( tagDigBaoCast cast )
{
	ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
	if( !P_VALID(pFrame) )
		return;
	switch(cast.eType)
	{
	case EDBCT_ChatBillike:
		{
			tstring strPlayerName = PlayerNameTab::Inst()->FindNameByID(cast.dwRoleID);
			if( strPlayerName.empty() )
			{
				m_vecDigBaoCast.push_back( cast );
				return;
			}
			TCHAR szBuff[X_LONG_NAME] = {0};
			_stprintf( szBuff, g_StrTable[_T("DigBao6")], strPlayerName.c_str() );
			pFrame->PushScrollText(szBuff);
		}
		break;
	case EDBCT_ChatBadOldSeal:
		{
			tstring strPlayerName = PlayerNameTab::Inst()->FindNameByID(cast.dwRoleID);
			if( strPlayerName.empty() )
			{
				m_vecDigBaoCast.push_back( cast );
				return;
			}
			tstring strMap = MapLogicData::Inst()->FindMapShowName(cast.dwMapCrc);
			TCHAR szBuff[X_LONG_NAME] = {0};
			_stprintf( szBuff, g_StrTable[_T("DigBao7")], strPlayerName.c_str(), strMap.c_str() );
			pFrame->PushScrollText(szBuff);
		}
		break;
	case EDBCT_Plutus:
		{
			tstring strPlayerName = PlayerNameTab::Inst()->FindNameByID(cast.dwRoleID);
			if( strPlayerName.empty() )
			{
				m_vecDigBaoCast.push_back(cast);
				return;
			}
			TCHAR szBuff[X_LONG_NAME] = {0};
			_stprintf( szBuff, g_StrTable[_T("DigBao8")], strPlayerName.c_str() );
			pFrame->PushScrollText(szBuff);
		}
		break;
	case EDBCT_AwakenGods:
		{
			tstring strPlayerName = PlayerNameTab::Inst()->FindNameByID(cast.dwRoleID);
			if( strPlayerName.empty() )
			{
				m_vecDigBaoCast.push_back(cast);
				return;
			}
			TCHAR szBuff[X_LONG_NAME] = {0};
			_stprintf( szBuff, g_StrTable[_T("DigBao9")], strPlayerName.c_str() );
			pFrame->PushScrollText(szBuff);
		}
		break;
	case EDBCT_GodsOfPractice:
		{
			tstring strPlayerName = PlayerNameTab::Inst()->FindNameByID(cast.dwRoleID);
			if( strPlayerName.empty() )
			{
				m_vecDigBaoCast.push_back(cast);
				return;
			}
			const tagCreatureProto* pCreature = CreatureData::Inst()->FindNpcAtt(cast.dwNpcTypeID);
			if( !P_VALID(pCreature) )
				return;
			tstring strNPC = pCreature->szName;

			const tagItemDisplayInfo* pItem = ItemProtoData::Inst()->FindItemDisplay(cast.dwItemTypeID);
			if( !P_VALID(pItem) ) 
				return;
			tstring strItem = pItem->szName;

			TCHAR szBuff[X_LONG_NAME] = {0};
			_stprintf( szBuff, g_StrTable[_T("DigBao10")], strPlayerName.c_str(), strNPC.c_str(), strItem.c_str() );
			pFrame->PushScrollText(szBuff);
		}
		break;
	case EDBCT_MonsterAttackCity:
		{
			tstring strPlayerName = PlayerNameTab::Inst()->FindNameByID(cast.dwRoleID);
			if( strPlayerName.empty() )
			{
				m_vecDigBaoCast.push_back(cast);
				return;
			}

			tstring strMap = MapLogicData::Inst()->FindMapShowName(cast.dwMapCrc);
			TCHAR szBuff[X_LONG_NAME] = {0};
			_stprintf( szBuff, g_StrTable[_T("DigBao11")], strPlayerName.c_str(), strMap.c_str() );
			pFrame->PushScrollText(szBuff);
		}
		break;
	case EDBCT_RoleGetChestItem:
		{
			tstring strPlayerName = PlayerNameTab::Inst()->FindNameByID(cast.dwRoleID);
			if( strPlayerName.empty() )
			{
				m_vecDigBaoCast.push_back(cast);
				return;
			}
			const tagItemDisplayInfo* pItem = ItemProtoData::Inst()->FindItemDisplay(cast.dwItemTypeID);
			TCHAR szBuff[X_LONG_NAME] = {0};
			_stprintf( szBuff, g_StrTable[_T("DigBao12")], strPlayerName.c_str(), pItem->szName );
			//pFrame->PushScrollText(szBuff);
			pFrame->PushInfo(szBuff, ESCC_Affiche);
		}
		break;
	}
}