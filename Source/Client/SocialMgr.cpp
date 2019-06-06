/********************************************************************
	created:	2008/12/10
	created:	10:12:2008   11:28
	filename: 	d:\loong_client\Client\SocialMgr.cpp
	file path:	d:\loong_client\Client
	file base:	SocialMgr
	file ext:	cpp
	author:		leili
	
	purpose:	���ѹ�����ʵ��
*********************************************************************/

#include "stdafx.h"
#include "SocialMgr.h"
#include "SocialListFrame.h"
#include "PlayerNameTab.h"
#include "SocialEvent.h"
#include "CombatSysUtil.h"
#include "ItemProtoData.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "ChatEvent.h"
#include "SocialReply.h"
#include "ChatFrame.h"
#include "AudioSys.h"

namespace
{
	const float G_GiftChoiceTime = 30.0f;	// ѡ���Ƿ��������ʱ��30��
	const float G_InviteChoiceTime = 60.0f;	// ѡ���Ƿ��������ʱ��30��

	void CreateDir(const TCHAR* Path)
	{
		TCHAR DirName[256];
		const TCHAR* p = Path;
		TCHAR* q = DirName;	

		while(*p)
		{
			if ((_T('\\') == *p) || (_T('/') == *p))
			{
				if (_T(':') != *(p-1))
				{
					CreateDirectory(DirName, NULL);
				}
			}
			*q++ = *p++;
			*q = _T('\0');
		}
		CreateDirectory(DirName, NULL);
	}
}

SocialMgr::SocialMgr( void )
: m_Trunk(this)
, m_pData(0)
, m_pReply(0)
, m_pListFrame(0)
, m_giftGlobalID(GT_INVALID)
, m_giftTypeID(GT_INVALID)
, m_giftTiming(false)
, m_timeSelStart(0.0f)
, m_sendRoleID(GT_INVALID)
, m_autoReply(false)
, m_showOutline(false)
, m_blackID(GT_INVALID)
, m_mountRoleID(GT_INVALID)
, m_inviteTiming(false)
, m_timeInvStart(0.0f)
, m_isUsingGift(false)
{

}

SocialMgr::~SocialMgr( void )
{

}

BOOL SocialMgr::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	m_pData = new SocialData();
	if( !P_VALID(m_pData) )
		return FALSE;

	// �����ͺ����б�������
	/*tagNC_GetRoleInitState msg;
	msg.eType = ERIT_FrindAndEnemy;
	m_pZoneSession->Send(&msg);*/

	// ע��������Ϣ
	m_pCmdMgr->Register("NS_SendFriendBlackList",	(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_SendFriendBlackList),	_T("��÷��͵ĺ��Ѽ�������"));
	m_pCmdMgr->Register("NS_UpdateFriGroup",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_MoveToFriend),			_T("�ƶ��������б�"));
	m_pCmdMgr->Register("NS_MoveToBlackList",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_MoveToBlackList),		_T("�ƶ���������"));
	m_pCmdMgr->Register("NS_RoleMakeFriend",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_AddFriend),				_T("���Ӻ���"));
	m_pCmdMgr->Register("NS_RoleCancelFriend",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_DeleteFriend),			_T("ɾ������"));
	m_pCmdMgr->Register("NS_DeleteBlackList",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_DeleteBlack),			_T("ɾ��������"));
	m_pCmdMgr->Register("NS_SendGiftToFri",			(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_GetGift),				_T("������"));
	m_pCmdMgr->Register("NS_SendGiftBroadcast",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_SendGiftBroadcast),		_T("�㲥����������"));
	m_pCmdMgr->Register("NS_LoginToFriend",			(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_FriendLogIn),			_T("���ѵ���"));
	m_pCmdMgr->Register("NS_LogoutToFriend",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_FriendLogOut),			_T("���ѵǳ�"));
	m_pCmdMgr->Register("NS_UpdateFriState",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_UpdateFriendState),		_T("���º���״̬"));
	m_pCmdMgr->Register("NS_SendGiftToSender",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_SendGiftError),			_T("��������ʧ��"));
	m_pCmdMgr->Register("NS_UpdateFriValue",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_UpdateFriendValue),		_T("���º��Ѷ�"));
	m_pCmdMgr->Register("NS_MakeFriNotice",			(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_FriendNotice),			_T("����ע��"));
	m_pCmdMgr->Register("NS_MountOnInvite",			(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_MountOnInvite),			_T("�������"));

	// ע����Ϸ�¼�
	pMgr->RegisterEventHandle(_T("Open_Sociality"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_OpenList));
	pMgr->RegisterEventHandle(_T("Close_Sociality"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_CloseList));
	pMgr->RegisterEventHandle(_T("SocialEvent_AddFriend"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_AddFriend));
	pMgr->RegisterEventHandle(_T("SocialEvent_AddBlack"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_AddBlack));
	pMgr->RegisterEventHandle(_T("SocialEvent_UseGift"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_UseGift));
	pMgr->RegisterEventHandle(_T("SocialEvent_GiveGiftTo"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_GiveGiftTo));
	pMgr->RegisterEventHandle(_T("SocialEvent_GetGift"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_GetGift));
	pMgr->RegisterEventHandle(_T("tagRoleGetNameEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_GetRoleName));
	pMgr->RegisterEventHandle(_T("SocialEvent_MsgAddBlack"),(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_MsgAddBlack));
	pMgr->RegisterEventHandle(_T("MountEvent_Invite"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_Mount));
	pMgr->RegisterEventHandle(_T("tagInitLPAttEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_LPAttIsOK));
	pMgr->RegisterEventHandle(_T("Event_RoleNotExist"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_GetRoleIDError));

	return GameFrame::Init(pMgr,pFather,dwParam);
}

BOOL SocialMgr::Destroy()
{
	// �����ֳ�
	m_logInDelayNames.clear();
	m_logOutDelayNames.clear();
	m_noticeDelayNames.clear();
	m_giftDelayBCs.clear();
	m_giveGiftDelayIDs.clear();
	m_getGiftDelayNames.clear();
	m_getMountDelayNames.clear();
	m_giftList.clear();
	m_mountList.clear();

	SAFE_DELETE(m_pReply);
	SAFE_DELETE(m_pData);

	LPVOID pObj = GetObj("GiftChoiceBox");
	if( P_VALID(pObj) )
		KillObj("GiftChoiceBox");

	if( P_VALID(m_pListFrame) )
	{
		//m_pMgr->AddToDestroyList(m_pListFrame);
		m_pListFrame = NULL;
	}

	// ע��������Ϣ
	m_pCmdMgr->UnRegister("NS_SendFriendBlackList",	(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_SendFriendBlackList));
	m_pCmdMgr->UnRegister("NS_UpdateFriGroup",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_MoveToFriend));
	m_pCmdMgr->UnRegister("NS_MoveToBlackList",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_MoveToBlackList));
	m_pCmdMgr->UnRegister("NS_RoleMakeFriend",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_AddFriend));
	m_pCmdMgr->UnRegister("NS_RoleCancelFriend",	(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_DeleteFriend));
	m_pCmdMgr->UnRegister("NS_DeleteBlackList",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_DeleteBlack));
	m_pCmdMgr->UnRegister("NS_SendGiftToFri",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_GetGift));
	m_pCmdMgr->UnRegister("NS_SendGiftBroadcast",	(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_SendGiftBroadcast));
	m_pCmdMgr->UnRegister("NS_LoginToFriend",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_FriendLogIn));
	m_pCmdMgr->UnRegister("NS_LogoutToFriend",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_FriendLogOut));
	m_pCmdMgr->UnRegister("NS_UpdateFriState",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_UpdateFriendState));
	m_pCmdMgr->UnRegister("NS_SendGiftToSender",	(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_SendGiftError));
	m_pCmdMgr->UnRegister("NS_UpdateFriValue",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_UpdateFriendValue));
	m_pCmdMgr->UnRegister("NS_MakeFriNotice",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_FriendNotice));
	m_pCmdMgr->UnRegister("NS_MountOnInvite",		(NETMSGPROC)m_Trunk.sfp2(&SocialMgr::NetRecvMessage_MountOnInvite));

	// ע����Ϸ�¼�
	m_pMgr->UnRegisterEventHandler( _T("Open_Sociality"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_OpenList));
	m_pMgr->UnRegisterEventHandler( _T("Close_Sociality"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_CloseList));
	m_pMgr->UnRegisterEventHandler( _T("SocialEvent_AddFriend"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_AddFriend));
	m_pMgr->UnRegisterEventHandler( _T("SocialEvent_AddBlack"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_AddBlack));
	m_pMgr->UnRegisterEventHandler( _T("SocialEvent_UseGift"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_UseGift));
	m_pMgr->UnRegisterEventHandler( _T("SocialEvent_GiveGiftTo"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_GiveGiftTo));
	m_pMgr->UnRegisterEventHandler( _T("SocialEvent_GetGift"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_GetGift));
	m_pMgr->UnRegisterEventHandler( _T("tagRoleGetNameEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_GetRoleName));
	m_pMgr->UnRegisterEventHandler(_T("SocialEvent_MsgAddBlack"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_MsgAddBlack));
	m_pMgr->UnRegisterEventHandler(_T("MountEvent_Invite"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_Mount));
	m_pMgr->UnRegisterEventHandler( _T("tagInitLPAttEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_LPAttIsOK));
	m_pMgr->UnRegisterEventHandler(_T("Event_RoleNotExist"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialMgr::OnEvent_GetRoleIDError));

	return GameFrame::Destroy();
}

VOID SocialMgr::Update()
{
	GameFrame::Update();

	if( m_giftTiming )
	{
		if( Kernel::Inst()->GetAccumTime() - m_timeSelStart > G_GiftChoiceTime )//����30����û��ѡ��
		{
			m_giftTiming = false;
			SendGiftReply( m_giftTypeID, FALSE );

			KillObj("GiftChoiceBox");
		}
	}
	else//�鿴�����б����Ƿ������͵�����
	{
		if( m_giftList.size() > 0 )
		{
			tagNS_SendGiftToFri& msg = *m_giftList.begin();

			LPCTSTR szName = PlayerNameTab::Inst()->FindNameByID(msg.dwSrcRoleID);
			if( _tcslen(szName) > 0 )
				ShowGetGift( &msg );
			else
				m_getGiftDelayNames.push_back( msg );

			m_giftList.erase( m_giftList.begin() );
		}
	}

	if( m_inviteTiming )
	{
		Role *pTarget = NULL;
		map<DWORD,Role*>::const_iterator finder = RoleMgr::Inst()->GetRoleMap().find(m_mountRoleID);
		if( finder != RoleMgr::Inst()->GetRoleMap().end() )
			pTarget = finder->second;

		if( (Kernel::Inst()->GetAccumTime() - m_timeInvStart > G_InviteChoiceTime)//����60����û��ѡ��
			|| !P_VALID(pTarget)
			|| (Vec3DistSq(RoleMgr::Inst()->GetLocalPlayer()->GetPos(), pTarget->GetPos()) > 250000) )//���볬��10��
		{
			m_inviteTiming =false;
			SendMountReply( FALSE );

			KillObj("MountInviteBox");
		}
	}
	else
	{
		if( m_mountList.size() > 0 )
		{
			tagNS_MountOnInvite& msg = *m_mountList.begin();

			LPCTSTR szName = PlayerNameTab::Inst()->FindNameByID(msg.dwSrcRoleID);
			if( _tcslen(szName) > 0 )
				ShowMountInvite( &msg );
			else
				m_getMountDelayNames.push_back( msg );

			m_mountList.erase( m_mountList.begin() );
		}
	}
}

//-----------------------------------------------------------------------------
// ע����Frame
//-----------------------------------------------------------------------------
void SocialMgr::RegisterFrames( GameFrameMgr* pMgr )
{
	pMgr->Register(TWrap<SocialListFrame>(), _T("SocialListFrame"));
}

//-----------------------------------------------------------------------------
// ����������ϢNS_SendFriendBlackList
//-----------------------------------------------------------------------------
DWORD SocialMgr::NetRecvMessage_SendFriendBlackList( tagNS_SendFriendBlackList* pMsg, DWORD pParam )
{
	for( INT i=0; i<pMsg->nNum; i++ )
	{
		// ����������
		PlayerNameTab::Inst()->FindNameByID( pMsg->FriendInfo[i].dwFriendID );

		switch( pMsg->FriendInfo[i].byGroup )
		{
		case 1:
			m_pData->m_friendsArray1.push_back( pMsg->FriendInfo[i] );
			break;
		case 2:
			m_pData->m_friendsArray2.push_back( pMsg->FriendInfo[i] );
			break;
		case 3:
			m_pData->m_friendsArray3.push_back( pMsg->FriendInfo[i] );
			break;
		case 4:
			m_pData->m_friendsArray4.push_back( pMsg->FriendInfo[i] );
			break;
		}
	}

	for( INT i=0; i<MAX_BLACKLIST; i++ )
	{
		if( !P_VALID(pMsg->dwRolesID[i]) )
			break;

		// ����������
		PlayerNameTab::Inst()->FindNameByID( pMsg->dwRolesID[i] );

		m_pData->m_blackArray.push_back( pMsg->dwRolesID[i] );
	}

	tagNC_GetRoleInitState msg;
	msg.eType = ERIT_Money;
	TObjRef<NetSession>()->Send(&msg);

	return 0;
}

//-----------------------------------------------------------------------------
// ����������ϢNS_UpdateFriGroup
//-----------------------------------------------------------------------------
DWORD SocialMgr::NetRecvMessage_MoveToFriend( tagNS_UpdateFriGroup* pMsg, DWORD pParam )
{
	switch( pMsg->dwErrorCode )
	{
	case E_Success:
		{
			const tagFriendInfo* pFriendInfo = m_pData->GetFriendPtrByID( (SocialData::EListType)pMsg->byOldGroup, pMsg->dwDestRoleID );
			if( P_VALID(pFriendInfo) )
			{
				m_pData->AddToFriendList( (SocialData::EListType)pMsg->byNewGroup, *pFriendInfo );

				m_pData->DeleteFromFriendList( (SocialData::EListType)pMsg->byOldGroup, pMsg->dwDestRoleID );
			}

			// ˢ�º����б���ʾ
			if( P_VALID(m_pListFrame) )
				m_pListFrame->RefreshList( (SocialData::EListType)pMsg->byOldGroup );
		}
		break;

	default:
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ����������ϢNS_MoveToBlackList
//-----------------------------------------------------------------------------
DWORD SocialMgr::NetRecvMessage_MoveToBlackList( tagNS_MoveToBlackList* pMsg, DWORD pParam )
{
	switch( pMsg->dwErrorCode )
	{
	case E_Success:
		{
			m_pData->AddToBlackList( pMsg->dwDestRoleID );
			m_pData->DeleteFromFriendList( (SocialData::EListType)pMsg->byOldGroup, pMsg->dwDestRoleID );

			// ˢ�º����б���ʾ
			if( P_VALID(m_pListFrame) )
				m_pListFrame->RefreshList();
		}
		break;
    case E_Friend_CanNotDel_Spouse:
        {
            CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Friend_CanNotDel_Spouse")]);
        }

	default:
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ����������ϢNS_RoleMakeFriend
//-----------------------------------------------------------------------------
DWORD SocialMgr::NetRecvMessage_AddFriend( tagNS_RoleMakeFriend* pMsg, DWORD pParam )
{
	switch( pMsg->dwErrorCode )
	{
	case E_Success:
		{
			tagFriendInfo info;
			info.dwFriendID = pMsg->dwDestRoleID;
			info.bOnline	= pMsg->bOnline==TRUE;
			info.byGroup	= 1;//Ĭ�Ϸŵ���һ��
			info.nLevel		= pMsg->nLevel;
			info.dwFriVal	= 0;

			m_pData->AddToFriendList( SocialData::ELT_Friend1, info );

			// ˢ�º����б���ʾ
			if( P_VALID(m_pListFrame) )
				m_pListFrame->RefreshList( SocialData::ELT_Friend1 );
		}
		break;

	case E_Friend_Target_Not_Exist:
		{
			tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
			chatInfo.strInfo = g_StrTable[_T("Social_RoleIsNotExist")];
			m_pMgr->SendEvent(&chatInfo);
		}
		break;

	default:
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ����������ϢNS_RoleCancelFriend
//-----------------------------------------------------------------------------
DWORD SocialMgr::NetRecvMessage_DeleteFriend( tagNS_RoleCancelFriend* pMsg, DWORD pParam )
{
	switch( pMsg->dwErrorCode )
	{
	case E_Success:
		{
			m_pData->DeleteFromFriendList( (SocialData::EListType)pMsg->byGroup, pMsg->dwDestRoleID );

			// ˢ�º����б���ʾ
			if( P_VALID(m_pListFrame) )
				m_pListFrame->RefreshList( (SocialData::EListType)pMsg->byGroup );
		}
		break;
    case E_Friend_CanNotDel_Spouse:
        {
            CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Social_CanNotDelSpouse")]);
        }
        break;

	default:
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ����������ϢNS_DeleteBlackList
//-----------------------------------------------------------------------------
DWORD SocialMgr::NetRecvMessage_DeleteBlack( tagNS_DeleteBlackList* pMsg, DWORD pParam )
{
	switch( pMsg->dwErrorCode )
	{
	case E_Success:
		{
			m_pData->DeleteFromBlackList( pMsg->dwDestRoleID );

			// ˢ�º����б���ʾ
			if( P_VALID(m_pListFrame) )
				m_pListFrame->RefreshList( SocialData::ELT_BlackList );
		}
		break;

	default:
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ����������ϢNS_LoginToFriend
//-----------------------------------------------------------------------------
DWORD SocialMgr::NetRecvMessage_FriendLogIn( tagNS_LoginToFriend* pMsg, DWORD pParam )
{
	LPCTSTR szName = PlayerNameTab::Inst()->FindNameByID( pMsg->dwRoleID );
	if( _tcslen(szName) > 0 )
		ShowFriendLogInInfo( pMsg->dwRoleID, szName );
	else
		m_logInDelayNames.push_back( pMsg->dwRoleID );

	//����������ʾ
	TObjRef<AudioSys>()->Play2DSound( _T("friend_login") );
	return 0;
}

//-----------------------------------------------------------------------------
// ����������ϢNS_LogoutToFriend
//-----------------------------------------------------------------------------
DWORD SocialMgr::NetRecvMessage_FriendLogOut( tagNS_LogoutToFriend* pMsg, DWORD pParam )
{
	LPCTSTR szName = PlayerNameTab::Inst()->FindNameByID( pMsg->dwRoleID );
	if( _tcslen(szName) > 0 )
		ShowFriendLogOutInfo( pMsg->dwRoleID, szName );
	else
		m_logOutDelayNames.push_back( pMsg->dwRoleID );

	return 0;
}

//-----------------------------------------------------------------------------
// ����������ϢNS_SendGift
//-----------------------------------------------------------------------------
DWORD SocialMgr::NetRecvMessage_GetGift( tagNS_SendGiftToFri* pMsg, DWORD pParam )
{
	m_giftList.push_back( *pMsg );

	return 0;
}

//-----------------------------------------------------------------------------
// ����������ϢNS_SendGiftBroadcast
//-----------------------------------------------------------------------------
DWORD SocialMgr::NetRecvMessage_SendGiftBroadcast( tagNS_SendGiftBroadcast* pMsg, DWORD pParam )
{
	LPCTSTR srcName = PlayerNameTab::Inst()->FindNameByID( pMsg->dwSrcRoleID );
	LPCTSTR destName = PlayerNameTab::Inst()->FindNameByID( pMsg->dwDestRoleID );

	if( _tcslen(srcName) > 0 && _tcslen(destName) > 0 )
		ShowGiftBroadcast( pMsg );
	else
	{
		tagBC bc;
		if( _tcslen(srcName) > 0 )
			bc.srcRoleID = GT_INVALID;
		else
			bc.srcRoleID = pMsg->dwSrcRoleID;

		if( _tcslen(destName) > 0 )
			bc.destRoleID = GT_INVALID;
		else
			bc.destRoleID = pMsg->dwDestRoleID;

		bc.msg = *pMsg;
		m_giftDelayBCs.push_back(bc);
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ����������ϢNS_UpdateFriState
//-----------------------------------------------------------------------------
DWORD SocialMgr::NetRecvMessage_UpdateFriendState( tagNS_UpdateFriState* pMsg, DWORD pParam )
{
	for( INT i=0; i<pMsg->nNum; i++ )
	{
		tagFriendInfo *pInfo = m_pData->GetFriend( pMsg->FriUpdate[i].dwRoleID );
		if( P_VALID(pInfo) )
		{
			pInfo->bOnline	= pMsg->FriUpdate[i].bOnline;
			pInfo->nLevel	= pMsg->FriUpdate[i].nLevel;
		}
	}

	if( P_VALID(m_pListFrame) )
	{
		m_pListFrame->RefreshList();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ����������ϢNS_UpdateFriValue
//-----------------------------------------------------------------------------
DWORD SocialMgr::NetRecvMessage_UpdateFriendValue( tagNS_UpdateFriValue* pMsg, DWORD pParam )
{
	tagFriendInfo *pInfo = m_pData->GetFriend( pMsg->dwRoleID );
	if( P_VALID(pInfo) )
	{
		pInfo->dwFriVal = pMsg->nFriVal;
	}

	if( P_VALID(m_pListFrame) )
	{
		m_pListFrame->RefreshList();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ����������ϢNS_MakeFriNotice
//-----------------------------------------------------------------------------
DWORD SocialMgr::NetRecvMessage_FriendNotice( tagNS_MakeFriNotice* pMsg, DWORD pParam )
{
	LPCTSTR szName = PlayerNameTab::Inst()->FindNameByID( pMsg->dwRoleID );
	if( _tcslen( szName ) > 0 )
		ShowFriendNotice( szName );
	else
		m_noticeDelayNames.push_back( pMsg->dwRoleID );

	return 0;
}

//-----------------------------------------------------------------------------
// ����������ϢNS_SendGiftToSender
//-----------------------------------------------------------------------------
DWORD SocialMgr::NetRecvMessage_SendGiftError( tagNS_SendGiftToSender* pMsg, DWORD pParam )
{
	switch( pMsg->dwErrorCode )
	{
	case E_Gift_Not_Exit:
		{
			tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
			chatInfo.strInfo = g_StrTable[_T("Social_ServerError_GiftNotExist")];
			m_pMgr->SendEvent(&chatInfo);
		}
		break;

	case E_Gift_Not_Both_Friend:
		{
			tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
			chatInfo.strInfo = g_StrTable[_T("Social_ServerError_NotBothFriend")];
			m_pMgr->SendEvent(&chatInfo);
		}
		break;

	case E_Gift_Friend_Not_Online:
		{
			tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
			chatInfo.strInfo = g_StrTable[_T("Social_ServerError_Outline")];
			m_pMgr->SendEvent(&chatInfo);
		}
		break;

	case E_Gift_Not_Gift:
		{
			tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
			chatInfo.strInfo = g_StrTable[_T("Social_ServerError_NotGift")];
			m_pMgr->SendEvent(&chatInfo);
		}
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ���ͽ�roleID�ƶ��������б�group�е���Ϣ
//-----------------------------------------------------------------------------
VOID SocialMgr::SendMoveFriend( DWORD roleID, BYTE group )
{
	tagNC_UpdateFriGroup msg;
	msg.dwDestRoleID	= roleID;
	msg.byGroup			= group;
	m_pZoneSession->Send(&msg);
}

//-----------------------------------------------------------------------------
// �������Ӻ���roleID����Ϣ
//-----------------------------------------------------------------------------
VOID SocialMgr::SendAddFriend( DWORD roleID )
{
	tagNC_RoleMakeFriend msg;
	msg.dwDestRoleID	= roleID;
	m_pZoneSession->Send(&msg);
}

//-----------------------------------------------------------------------------
// �������Ӻ�����roleID����Ϣ
//-----------------------------------------------------------------------------
VOID SocialMgr::SendAddBlack( DWORD roleID )
{
	tagNC_MoveToBlackList msg;
	msg.dwDestRoleID	= roleID;
	m_pZoneSession->Send(&msg);
}

//-----------------------------------------------------------------------------
// ����ɾ������roleID����Ϣ
//-----------------------------------------------------------------------------
VOID SocialMgr::SendDeleteFriend( DWORD roleID )
{
	tagNC_RoleCancelFriend msg;
	msg.dwDestRoleID	= roleID;
	m_pZoneSession->Send(&msg);
}

//-----------------------------------------------------------------------------
// ����ɾ������������Ϣ
//-----------------------------------------------------------------------------
VOID SocialMgr::SendDeleteBlack( DWORD roleID )
{
	tagNC_DeleteBlackList msg;
	msg.dwDestRoleID	= roleID;
	m_pZoneSession->Send(&msg);
}

//-----------------------------------------------------------------------------
// ���ͽ����������Ϣ
//-----------------------------------------------------------------------------
VOID SocialMgr::SendGiftReply( DWORD giftID,BOOL bReceive )
{
	tagNC_SendGiftReply msg;
	msg.dwSrcRoleID		= m_sendRoleID;
	msg.dwTypeID		= giftID;
	msg.bResult			= bReceive;
	m_pZoneSession->Send(&msg);
}

//-----------------------------------------------------------------------------
// ���͸��������Ϣ
//-----------------------------------------------------------------------------
VOID SocialMgr::SendGiveGiftTo( DWORD roleID,INT64 giftID )
{
	tagNC_SendGift msg;
	msg.dwDestRoleID	= roleID;
	msg.n64ItemID		= giftID;
	m_pZoneSession->Send(&msg);
}

//-----------------------------------------------------------------------------
// ���ͽ�������������Ϣ
//-----------------------------------------------------------------------------
VOID SocialMgr::SendMountReply( BOOL bReceive )
{
	tagNC_MountOnInvite msg;
	msg.dwSrcRoleID		= m_mountRoleID;
	msg.bReceive		= bReceive;
	m_pZoneSession->Send(&msg);
}

//-----------------------------------------------------------------------------
// �򿪺����б�
//-----------------------------------------------------------------------------
DWORD SocialMgr::OnEvent_OpenList( tagGameEvent* pEvent )
{
	//���͸��º�����Ϣ����Ϣ
	tagNC_UpdateFriState msg;
	m_pZoneSession->Send(&msg);

	if( !P_VALID(m_pListFrame) )
	{
		DWORD dwParam = 0;
		if( m_showOutline )
			dwParam |= 0x00010000;
		if( m_autoReply )
			dwParam |= 0x00000001;
		m_pListFrame = (SocialListFrame*)m_pMgr->CreateFrame( m_strName.c_str(), _T("SocialList"), _T("SocialListFrame"), dwParam );
	}

	m_pListFrame->Show();

	return 0;
}

//-----------------------------------------------------------------------------
// �رպ����б�
//-----------------------------------------------------------------------------
DWORD SocialMgr::OnEvent_CloseList( tagGameEvent* pEvent )
{
	if( P_VALID(m_pListFrame) )
	{
		m_pListFrame->Hide();
		m_pMgr->AddToDestroyList(m_pListFrame);
		m_pListFrame = NULL;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ���Ӻ���
//-----------------------------------------------------------------------------
DWORD SocialMgr::OnEvent_AddFriend( tagAddFriendEvent* pEvent )
{
	AddFriend( pEvent->roleID );

	return 0;
}

//-----------------------------------------------------------------------------
// ���Ӻ�����
//-----------------------------------------------------------------------------
DWORD SocialMgr::OnEvent_AddBlack( tagAddBlackEvent* pEvent )
{
	AddBlack( pEvent->roleID );

	return 0;
}

//-----------------------------------------------------------------------------
// ʹ������
//-----------------------------------------------------------------------------
DWORD SocialMgr::OnEvent_UseGift( tagUseGiftEvent* pEvent )
{
	m_giftGlobalID = pEvent->giftGlobalID;
	m_giftTypeID = pEvent->giftTypeID;

	if( !m_isUsingGift )
	{
		m_pMsgInputBox->Init( g_StrTable[_T("Social_GiveGiftCaption")], g_StrTable[_T("Social_GiveGiftText")], _T("SocialEvent_GiveGiftTo"), GT_INVALID, MBIT_Name, _T(""), FALSE);
		m_isUsingGift = true;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// �������������
//-----------------------------------------------------------------------------
DWORD SocialMgr::OnEvent_GiveGiftTo( tagMsgInputBoxEvent* pEvent )
{
	m_isUsingGift = false;

	if(MBF_OK == pEvent->eResult)
	{
		DWORD roleID = PlayerNameTab::Inst()->FindIDByName( pEvent->strInputName.c_str() );
		if( P_VALID( roleID ) )
			GiveGiftTo( roleID );
		else
			m_giveGiftDelayIDs.push_back( pEvent->strInputName );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------
DWORD SocialMgr::OnEvent_GetGift( tagMsgBoxEvent* pEvent )
{
	m_giftTiming = false;

	if( MBF_OK == pEvent->eResult )
		SendGiftReply( m_giftTypeID, TRUE );
	else
		SendGiftReply( m_giftTypeID, FALSE );

	return 0;
}

//-----------------------------------------------------------------------------
// �ӳٻ��ĳ��ɫ����
//-----------------------------------------------------------------------------
DWORD SocialMgr::OnEvent_GetRoleName( tagRoleGetNameEvent* pEvent )
{
	DelayName::iterator nameIter;
	DelayBC::iterator bcIter;
	DelayID::iterator idIter;
	GetGiftName::iterator giftIter;
	GetInviteName::iterator invIter;

	// ���ѵ���
	for( nameIter=m_logInDelayNames.begin(); nameIter!=m_logInDelayNames.end(); )
	{
		if( *nameIter == pEvent->dwRoleID )//�ҵ�����ͬID������
		{
			ShowFriendLogInInfo( pEvent->dwRoleID, pEvent->strRoleName.c_str() );

			nameIter = m_logInDelayNames.erase( nameIter );
		}
		else
			++nameIter;
	}

	// ���ѵǳ�
	for( nameIter=m_logOutDelayNames.begin(); nameIter!=m_logOutDelayNames.end(); )
	{
		if( *nameIter == pEvent->dwRoleID )//�ҵ�����ͬID������
		{
			ShowFriendLogOutInfo( pEvent->dwRoleID, pEvent->strRoleName.c_str() );

			nameIter = m_logOutDelayNames.erase( nameIter );
		}
		else
			++nameIter;
	}

	//����㲥
	for( bcIter=m_giftDelayBCs.begin(); bcIter!=m_giftDelayBCs.end(); )
	{
		tagBC& bc = *bcIter;
		if( bc.srcRoleID == pEvent->dwRoleID )//�ҵ���ͬ����
		{
			bc.srcRoleID = GT_INVALID;
		}
		if( bc.destRoleID == pEvent->dwRoleID )//�ҵ���ͬ����
		{
			bc.destRoleID = GT_INVALID;
		}

		if( (bc.srcRoleID == GT_INVALID) && (bc.destRoleID == GT_INVALID) )
		{
			ShowGiftBroadcast( &(bc.msg) );

			bcIter = m_giftDelayBCs.erase( bcIter );
		}
		else
			++bcIter;
	}

	//��������
	for( nameIter=m_noticeDelayNames.begin(); nameIter!=m_noticeDelayNames.end(); )
	{
		if( *nameIter == pEvent->dwRoleID )//�ҵ���ͬID������
		{
			ShowFriendNotice( pEvent->strRoleName.c_str() );

			nameIter = m_noticeDelayNames.erase( nameIter );
		}
		else
			++nameIter;
	}

	//�������
	for( idIter=m_giveGiftDelayIDs.begin(); idIter!=m_giveGiftDelayIDs.end(); )
	{
		if( *idIter == pEvent->strRoleName )//�ҵ���ͬ����
		{
			GiveGiftTo( pEvent->dwRoleID );

			idIter = m_giveGiftDelayIDs.erase( idIter );
		}
		else
			++idIter;
	}

	//��������
	for( giftIter=m_getGiftDelayNames.begin(); giftIter!=m_getGiftDelayNames.end(); )
	{
		tagNS_SendGiftToFri& msg = *giftIter;
		if( msg.dwSrcRoleID == pEvent->dwRoleID )//�ҵ���ͬ����
		{
			ShowGetGift( &msg );

			giftIter = m_getGiftDelayNames.erase( giftIter );
		}
		else
			++giftIter;
	}

	//�������
	for( invIter=m_getMountDelayNames.begin(); invIter!=m_getMountDelayNames.end(); )
	{
		tagNS_MountOnInvite& msg = *invIter;
		if( msg.dwSrcRoleID == pEvent->dwRoleID )//�ҵ���ͬ����
		{
			ShowMountInvite( &msg );

			invIter = m_getMountDelayNames.erase( invIter );
		}
		else
			++invIter;
	}

	return 0;
}

DWORD SocialMgr::OnEvent_MsgAddBlack( tagMsgBoxEvent* pEvent )
{
	if(MBF_OK == pEvent->eResult)
		SendAddBlack( m_blackID );

	return 0;
}

VOID SocialMgr::SetReply( LPCTSTR szReply )
{
	if( P_VALID(m_pReply) )
		m_pReply->SetReply( szReply );
}

LPCTSTR SocialMgr::GetReply( void ) const
{
	if( P_VALID(m_pReply) )
		return m_pReply->GetReply();

	return NULL;
}

//-----------------------------------------------------------------------------
// ���ѵ�¼���һЩ�������
//-----------------------------------------------------------------------------
VOID SocialMgr::ShowFriendLogInInfo( DWORD roleID, LPCTSTR szRoleName )
{
	SocialData::EListType listType = m_pData->FriendLogIn( roleID );

	TCHAR szInfo[128];
	_stprintf( szInfo, g_StrTable[_T("Social_FriendLogIn")], szRoleName );
	tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
	chatInfo.strInfo = szInfo;
	m_pMgr->SendEvent(&chatInfo);

	// ˢ�º����б���ʾ
	if( P_VALID(m_pListFrame) )
		m_pListFrame->RefreshList( listType );
}

//-----------------------------------------------------------------------------
// ���ѵǳ����һЩ�������
//-----------------------------------------------------------------------------
VOID SocialMgr::ShowFriendLogOutInfo( DWORD roleID, LPCTSTR szRoleName )
{
	SocialData::EListType listType = m_pData->FriendLogOut( roleID );

	TCHAR szInfo[128];
	_stprintf( szInfo, g_StrTable[_T("Social_FriendLogOut")], szRoleName );
	tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
	chatInfo.strInfo = szInfo;
	m_pMgr->SendEvent(&chatInfo);

	// ˢ�º����б���ʾ
	if( P_VALID(m_pListFrame) )
		m_pListFrame->RefreshList( listType );
}

VOID SocialMgr::ShowGiftBroadcast( tagNS_SendGiftBroadcast* pMsg )
{
	LPCTSTR srcName = PlayerNameTab::Inst()->FindNameByID( pMsg->dwSrcRoleID );
	LPCTSTR destName = PlayerNameTab::Inst()->FindNameByID( pMsg->dwDestRoleID );

	const tagItemProto *pItem = ItemProtoData::Inst()->FindItemProto( pMsg->dwTypeID );
	if( !P_VALID(pItem) )
		return;

	ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
	if( !P_VALID(pFrame) )
		return;

	TCHAR szContent[255];
	if( pMsg->bResult )
		_stprintf_s(szContent, 255, g_StrTable[_T("Social_ReceiveGift")], srcName, destName, pItem->pDisplayInfo->szName);
	else
		_stprintf_s(szContent, 255, g_StrTable[_T("Social_RefuseGift")], srcName, destName, pItem->pDisplayInfo->szName, destName, srcName);

	//pFrame->PushScrollText(szContent);
	pFrame->PushInfo(szContent, ESCC_Affiche);
}

VOID SocialMgr::ShowFriendNotice( LPCTSTR szRoleName )
{
	TCHAR szInfo[128];
	_stprintf( szInfo, g_StrTable[_T("Social_FriendNotice")], szRoleName );
	tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
	chatInfo.strInfo = szInfo;
	m_pMgr->SendEvent(&chatInfo);
}

VOID SocialMgr::GiveGiftTo( DWORD roleID )
{
	if( P_VALID(roleID) )//����Ľ�ɫ�Ƿ����
	{
		tagFriendInfo* pInfo = m_pData->GetFriend( roleID );
		if( P_VALID(pInfo) )//���Ѵ���
		{
			if( pInfo->bOnline )//��������
			{
				SendGiveGiftTo(roleID, m_giftGlobalID);
			}
			else
			{
				tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
				chatInfo.strInfo = g_StrTable[_T("Social_GiveGift_Outline")];
				m_pMgr->SendEvent(&chatInfo);
			}
		}
		else
		{
			tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
			chatInfo.strInfo = g_StrTable[_T("Social_GiveGift_NameIsNotExist")];
			m_pMgr->SendEvent(&chatInfo);
		}
	}
	else
	{
		tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
		chatInfo.strInfo = g_StrTable[_T("Social_RoleIsNotExist")];
		m_pMgr->SendEvent(&chatInfo);
	}
}

VOID SocialMgr::ShowGetGift( tagNS_SendGiftToFri* pMsg )
{
	const tagItemProto* pItem = ItemProtoData::Inst()->FindItemProto( pMsg->dwTypeID );
	if( !P_VALID(pItem) )
		return;

	LPVOID pObj = GetObj("GiftChoiceBox");
	if( P_VALID(pObj) )
		KillObj("GiftChoiceBox");

	CreateObj("GiftChoiceBox", "MsgBox");

	TCHAR szText[128]={0};
	_stprintf( szText, g_StrTable[_T("Social_GetGiftText")], 
		PlayerNameTab::Inst()->FindNameByID(pMsg->dwSrcRoleID),
		pItem->pDisplayInfo->szName);
	m_sendRoleID = pMsg->dwSrcRoleID;
	m_giftTypeID = pMsg->dwTypeID;

	TObjRef<MsgBox>("GiftChoiceBox")->Init( 
		g_StrTable[_T("Social_GetGiftCaption")],
		szText,
		_T("SocialEvent_GetGift"),
		(MsgBoxFlag)(MBF_OK|MBF_Cancel) );

	// ��ʼ��ʱ
	m_giftTiming = true;
	m_timeSelStart = Kernel::Inst()->GetAccumTime();
}

VOID SocialMgr::AddFriend( DWORD roleID )
{
	if( m_pData->HasFriend( roleID ) )//�Ѿ��Ǻ���
	{
		tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
		chatInfo.strInfo = g_StrTable[_T("Social_FriendIsExist")];
		m_pMgr->SendEvent(&chatInfo);
		return;
	}

	if( m_pData->GetFriendNum() >= MAX_FRIENDNUM )//�����б�����
	{
		tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
		chatInfo.strInfo = g_StrTable[_T("Social_FriendListIsFull")];
		m_pMgr->SendEvent(&chatInfo);
		return;
	}

	if( m_pData->HasBlack( roleID ) )//���������޷������Ϊ����
	{
		tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
		chatInfo.strInfo = g_StrTable[_T("Social_BlackNotBeFriend")];
		m_pMgr->SendEvent(&chatInfo);
		return;
	}

	SendAddFriend( roleID );
}

VOID SocialMgr::AddBlack( DWORD roleID )
{
	if( m_pData->HasBlack( roleID ) )//�Ѿ��ں�������
	{
		tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
		chatInfo.strInfo = g_StrTable[_T("Social_BlackIsExist")];
		m_pMgr->SendEvent(&chatInfo);
		return;
	}

	if( m_pData->GetBlackNum() >= MAX_BLACKLIST )//����������
	{
		tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
		chatInfo.strInfo = g_StrTable[_T("Social_BlackListIsFull")];
		m_pMgr->SendEvent(&chatInfo);
		return;
	}

	if( m_pData->HasFriend( roleID ) )//�Ǻ���
	{
		// ��ʾ�����ѺöȽ������
		m_blackID = roleID;
		TCHAR szText[128]={0};
		_stprintf( szText, g_StrTable[_T("Social_MoveToBlackText")], PlayerNameTab::Inst()->FindNameByID(roleID) );
		m_pMsgBox->Init( 
			g_StrTable[_T("Social_MoveToBlackCaption")],
			szText,
			_T("SocialEvent_MsgAddBlack"),
			(MsgBoxFlag)(MBF_OK|MBF_Cancel),
			TRUE);
		return;
	}

	// todo �����ϵ�Ĳ������Σ����ޡ�ʦͽ������

	SendAddBlack( roleID );
}

DWORD SocialMgr::NetRecvMessage_MountOnInvite( tagNS_MountOnInvite* pMsg, DWORD pParam )
{
	m_mountList.push_back( *pMsg );

	return 0;
}

DWORD SocialMgr::OnEvent_Mount( tagMsgBoxEvent* pEvent )
{
	m_inviteTiming = false;

	if( MBF_OK == pEvent->eResult )
		SendMountReply( TRUE );
	else
		SendMountReply( FALSE );

	return 0;
}

DWORD SocialMgr::OnEvent_LPAttIsOK( tagGameEvent* pEvent )
{
	if( P_VALID(m_pReply) )
		SAFE_DELETE(m_pReply);

	m_pReply = new SocialReply();
	if( P_VALID(m_pReply) )
	{
		LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
		tstring strLocalName = pPlayer->GetRoleName();
		tstring strPath = _T("config\\");
		strPath += strLocalName;
		strPath += _T("\\");

		CreateDir( strPath.c_str() );
		strPath += _T("Reply.cfg");

		m_pReply->Init( strPath.c_str() );
	}

	return 0;
}

VOID SocialMgr::ShowMountInvite( tagNS_MountOnInvite* pMsg )
{
	LPVOID pObj = GetObj("MountInviteBox");
	if( P_VALID(pObj) )
		KillObj("MountInviteBox");

	CreateObj("MountInviteBox", "MsgBox");

	TCHAR szText[128]={0};
	_stprintf( szText, g_StrTable[_T("Mount_Invite")], 
		PlayerNameTab::Inst()->FindNameByID(pMsg->dwSrcRoleID) );
	m_mountRoleID = pMsg->dwSrcRoleID;

	TObjRef<MsgBox>("MountInviteBox")->Init( 
		_T(""),
		szText,
		_T("MountEvent_Invite"),
		(MsgBoxFlag)(MBF_OK|MBF_Cancel) );

	// ��ʼ��ʱ
	m_inviteTiming = true;
	m_timeInvStart = Kernel::Inst()->GetAccumTime();
}

DWORD SocialMgr::OnEvent_GetRoleIDError( tagGameEvent* pEvent )
{
	tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
	chatInfo.strInfo = g_StrTable[_T("Social_RoleIsNotExist")];
	m_pMgr->SendEvent(&chatInfo);

	return 0;
}