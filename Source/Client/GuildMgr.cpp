#include "StdAfx.h"
#include "GuildDefine.h"
#include "GuildMgr.h"
#include "GuildMembers.h"
#include "FilterData.h"
#include "..\WorldDefine\msg_guild.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "PlayerNameTab.h"
#include "GuildEvent.h"
#include "QuestMgr.h"
#include "GuildFrame.h"
#include "GuildAffairFrame.h"
#include "GuildPickUpSilverFrame.h"
#include "GuildContributeSilver.h"
#include "GuildBusinessFrame.h"
#include "GuildBusinessRankFrame.h"
#include "CombatSysUtil.h"
#include "CurrencyMgr.h"
#include "..\WorldDefine\currency_define.h"
#include "..\WorldDefine\msg_talent.h"
#include "SkillProtoData.h"
#include <new>
#include "RoleFloatTipsFrame.h"
#include "GuildStorageFrame.h"
#include "GuildStorePermitFrame.h"
#include "ItemMgr.h"
#include "GuildBusinessStateWnd.h"
#include "ChatFrame.h"
#include "MallMgr.h"


GuildMgr::GuildMgr(void):m_Trunk(this),
    m_bifNeedInitWarehouse(true),
    m_dwWarehouseLastUpTime(0)
{
	m_pLpInfo		 = NULL;
	m_pGuild		 = NULL;
	m_pMembers		 = NULL;
	m_dwInviterID	 = GT_INVALID;
	m_dwInviterGuild = GT_INVALID;
	m_dwLastSyncTime = 0;
	m_dwLastUpdateTime = 0;
	m_dwCurResearchSkillID = GT_INVALID;
	m_nCurResearchProgress = 0;
	m_nOwnCommerceSilver = 0;
	m_dwTempCompleteCommercePlayerID = GT_INVALID;
	m_nTempFund = 0;
	m_dwCurCOCID = GT_INVALID;
	m_dwCurTalkCOCNPCID = GT_INVALID;
	m_byCurHoldCity = 0;
	m_nRunForBusinessLevel = 0;
	m_bSendGetAllMember = false;
	m_bGetTenet = false;
}

GuildMgr::~GuildMgr(void)
{
	
}

GuildMgr g_guildmgr;
GuildMgr* GuildMgr::Inst()
{
	return &g_guildmgr;
}



VOID GuildMgr::Init()
{
	m_pCmdMgr	= TObjRef<NetCmdMgr>();
	m_pSession	= TObjRef<NetSession>();
	m_pFrameMgr	= TObjRef<GameFrameMgr>();
	m_bIsInitFinish = FALSE;
	m_dwLastSyncTime = timeGetTime();
    m_bifNeedInitWarehouse  = true;
    m_dwWarehouseLastUpTime = 0;
	
	// ע����Ϸ�¼�������
	m_pFrameMgr->RegisterEventHandle(_T("tagRoleGetNameEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGetRoleName));
	m_pFrameMgr->RegisterEventHandle(_T("GuildEvent_Create"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildCreate));
	m_pFrameMgr->RegisterEventHandle(_T("GuildEvent_Dismiss"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildDismiss));
	m_pFrameMgr->RegisterEventHandle(_T("GuildEvent_Invite"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildInvite));
	m_pFrameMgr->RegisterEventHandle(_T("Open_Faction"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildOpen));
	m_pFrameMgr->RegisterEventHandle(_T("Close_Faction"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildClose));
	m_pFrameMgr->RegisterEventHandle(_T("Open_GuildAffair"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildAffairOpen));
	m_pFrameMgr->RegisterEventHandle(_T("Close_GuildAffair"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildAffairClose));
	m_pFrameMgr->RegisterEventHandle(_T("Open_PickUpSilver"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventPickUpSilverOpen));
	m_pFrameMgr->RegisterEventHandle(_T("Close_PickUpSilver"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventPickUpSilverClose));
	m_pFrameMgr->RegisterEventHandle(_T("Open_ContributeSilver"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventContributeSilverOpen));
	m_pFrameMgr->RegisterEventHandle(_T("Close_ContributeSilver"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventContributeSilverClose));
	m_pFrameMgr->RegisterEventHandle(_T("Open_COC"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventCOCOpen));
	m_pFrameMgr->RegisterEventHandle(_T("Close_COC"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventCOCClose));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_CreateGuild"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventMsgBoxInputName));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_DismissCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventMsgBoxDismissCheck));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_JoinReq"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventMsgBoxJoinReq));
	m_pFrameMgr->RegisterEventHandle(_T("GuildEvent_Upgrade"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildUpgrade));
	m_pFrameMgr->RegisterEventHandle(_T("QuitCommerceWarning"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventMsgBoxQuitCommerceWarning));
	m_pFrameMgr->RegisterEventHandle(_T("QuitCommerce"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventQuitCommerce));
    m_pFrameMgr->RegisterEventHandle(_T("OpenGuildStorage"),        (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventOpenGuildStorage));
    m_pFrameMgr->RegisterEventHandle(_T("OpenGuildStoragePermission"),(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventOpenGuildStoragePermission));
	m_pFrameMgr->RegisterEventHandle(_T("Open_CommerceRank"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildCommerceRankOpen));
	m_pFrameMgr->RegisterEventHandle(_T("Close_CommerceRank"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildCommerceRankClose));
	m_pFrameMgr->RegisterEventHandle(_T("Guild_GetTenet"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildGetTenet));

	// ע��������Ϣ������
	m_pCmdMgr->Register("NS_GuildCreateBroad",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildCreateBroad),			_T("NS_GuildCreateBroad"));
	m_pCmdMgr->Register("NS_GuildDismissBroad",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildDismissBroad),		_T("NS_GuildDismissBroad"));
	m_pCmdMgr->Register("NS_GuildJoinReq",				(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildJoinReq),				_T("NS_GuildJoinReq"));
	m_pCmdMgr->Register("NS_GuildJoinReqRes",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildJoinReqRes),			_T("NS_GuildJoinReqRes"));
	m_pCmdMgr->Register("NS_GuildJoinBroad",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildJoinBroad),			_T("NS_GuildJoinBroad"));
	m_pCmdMgr->Register("NS_GuildLeaveBroad",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildLeaveBroad),			_T("NS_GuildLeaveBroad"));
	m_pCmdMgr->Register("NS_GuildKickBroad",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildKickBroad),			_T("NS_GuildKickBroad"));
	m_pCmdMgr->Register("NS_GuildTurnoverBroad",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildTurnoverBroad),		_T("NS_GuildTurnoverBroad"));
	m_pCmdMgr->Register("NS_GuildDemissBroad",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildDemissBroad),			_T("NS_GuildDemissBroad"));
	m_pCmdMgr->Register("NS_GuildAppointBroad",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildAppointBroad),		_T("NS_GuildAppointBroad"));
	m_pCmdMgr->Register("NS_GuildTenetChangeBroad",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildTenetChangeBroad),	_T("NS_GuildTenetChangeBroad"));
	m_pCmdMgr->Register("NS_GuildProcFailed",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildProcFailed),			_T("NS_GuildProcFailed"));
	m_pCmdMgr->Register("NS_GuildGetAllMembers",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildGetAllMembers),		_T("NS_GuildGetAllMembers"));
	m_pCmdMgr->Register("NS_GuildGetMemberEx",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildGetMemberEx),			_T("NS_GuildGetMemberEx"));
	m_pCmdMgr->Register("NS_GuildRefMember",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildRefMember),			_T("NS_GuildRefMember"));
	m_pCmdMgr->Register("NS_GetGuildName",				(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildNameGet),				_T("NS_GetGuildName"));
	m_pCmdMgr->Register("NS_GetGuildTenet",				(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildTenetGet),			_T("NS_GetGuildTenet"));
	m_pCmdMgr->Register("NS_GetRoleInitState_Guild",	(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildAttInit),				_T("NS_GetRoleInitState_Guild"));
	m_pCmdMgr->Register("NS_SpreadGuildAffair",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildSpreadAffair),		_T("NS_SpreadGuildAffair"));
	m_pCmdMgr->Register("NS_SyncGuildInfo",				(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildSyncInfo),			_T("NS_SyncGuildInfo"));
	m_pCmdMgr->Register("NS_GetGuildSkillInfo",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGetGuildSkillInfo),		_T("NS_GetGuildSkillInfo"));
	m_pCmdMgr->Register("NS_SetResearchSkill",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetSetResearchSkill),			_T("NS_SetResearchSkill"));
	m_pCmdMgr->Register("NS_LearnGuildSkill",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetLearnGuildSkill),			_T("NS_LearnGuildSkill"));
	m_pCmdMgr->Register("NS_GuildContribution",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildContribution),		_T("NS_GuildContribution"));
	m_pCmdMgr->Register("NS_GuildStateSet",				(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildStateSet),			_T("NS_GuildStateSet"));
	m_pCmdMgr->Register("NS_GuildStateUnset",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildStateUnSet),			_T("NS_GuildStateUnset"));
	m_pCmdMgr->Register("NS_GuildSkillUpgrade",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildSkillUpgrade),		_T("NS_GuildSkillUpgrade"));
	m_pCmdMgr->Register("NS_GuildSkillLevelUp",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildSkillLevelUp),		_T("NS_GuildSkillLevelUp"));
	m_pCmdMgr->Register("NS_AcceptCommerce",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetAcceptCommerce),			_T("NS_AcceptCommerce"));
	m_pCmdMgr->Register("NS_GetTaelInfo",				(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGetTaelInfo),				_T("NS_GetTaelInfo"));
	m_pCmdMgr->Register("NS_GuildUpgrade",				(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildUpgrade),				_T("NS_GuildUpgrade"));
	m_pCmdMgr->Register("NS_CompleteCommerce",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetCompleteCommerce),			_T("NS_CompleteCommerce"));
	m_pCmdMgr->Register("NS_AbandonCommerce",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetAbandonCommerce),			_T("NS_AbandonCommerce"));
	m_pCmdMgr->Register("NS_GetCommodityInfo",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGetCommodityInfo),			_T("NS_GetCommodityInfo"));
	m_pCmdMgr->Register("NS_GetCofCInfo",				(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGetCofCInfo),				_T("NS_GetCofCInfo"));
	m_pCmdMgr->Register("NS_GetCommerceRank",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGetCommerceRank),			_T("NS_GetCommerceRank"));
	m_pCmdMgr->Register("NS_SwitchCommendation",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetSwitchCommendation),		_T("NS_SwitchCommendation"));
    m_pCmdMgr->Register("NS_GetGuildWare",		        (NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNS_GetGuildWare),		        _T("NS_GetGuildWare"));
    m_pCmdMgr->Register("NS_GuildWarePri",		        (NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNS_GuildWarePri),		        _T("NS_GuildWarePri"));
	m_pCmdMgr->Register("NS_GuildExploit",				(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildExploit),				_T("NS_GuildExploit"));
	m_pCmdMgr->Register("NS_GetSomeGuildName",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGetSomeGuildName),			_T("NS_GetSomeGuildName"));
}

VOID GuildMgr::Destroy()
{
	SAFE_DEL(m_pGuild);
	SAFE_DEL(m_pMembers);
	SAFE_DEL(m_pLpInfo);
	
	// ע����Ϸ�¼�������
	m_pFrameMgr->UnRegisterEventHandler(_T("tagRoleGetNameEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGetRoleName));
	m_pFrameMgr->UnRegisterEventHandler(_T("GuildEvent_Create"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildCreate));
	m_pFrameMgr->UnRegisterEventHandler(_T("GuildEvent_Dismiss"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildDismiss));
	m_pFrameMgr->UnRegisterEventHandler(_T("GuildEvent_Invite"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildInvite));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_Faction"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildOpen));
	m_pFrameMgr->UnRegisterEventHandler(_T("Close_Faction"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildClose));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_GuildAffair"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildAffairOpen));
	m_pFrameMgr->UnRegisterEventHandler(_T("Close_GuildAffair"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildAffairClose));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_PickUpSilver"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventPickUpSilverOpen));
	m_pFrameMgr->UnRegisterEventHandler(_T("Close_PickUpSilver"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventPickUpSilverClose));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_ContributeSilver"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventContributeSilverOpen));
	m_pFrameMgr->UnRegisterEventHandler(_T("Close_ContributeSilver"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventContributeSilverClose));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_COC"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventCOCOpen));
	m_pFrameMgr->UnRegisterEventHandler(_T("Close_COC"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventCOCClose));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_CreateGuild"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventMsgBoxInputName));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_DismissCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventMsgBoxDismissCheck));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_JoinReq"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventMsgBoxJoinReq));
	m_pFrameMgr->UnRegisterEventHandler(_T("GuildEvent_Upgrade"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildUpgrade));
	m_pFrameMgr->UnRegisterEventHandler(_T("QuitCommerceWarning"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventMsgBoxQuitCommerceWarning));
    m_pFrameMgr->UnRegisterEventHandler(_T("QuitCommerce"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventQuitCommerce));
    m_pFrameMgr->UnRegisterEventHandler(_T("OpenGuildStorage"),         (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventOpenGuildStorage));
    m_pFrameMgr->UnRegisterEventHandler(_T("OpenGuildStoragePermission"),(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventOpenGuildStoragePermission));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_CommerceRank"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildCommerceRankOpen));
	m_pFrameMgr->UnRegisterEventHandler(_T("Close_CommerceRank"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildCommerceRankClose));
	m_pFrameMgr->UnRegisterEventHandler(_T("Guild_GetTenet"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildMgr::OnEventGuildGetTenet));

	// ע��������Ϣ������
	m_pCmdMgr->UnRegister("NS_GuildCreateBroad",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildCreateBroad));
	m_pCmdMgr->UnRegister("NS_GuildDismissBroad",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildDismissBroad));
	m_pCmdMgr->UnRegister("NS_GuildJoinReq",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildJoinReq));
	m_pCmdMgr->UnRegister("NS_GuildJoinReqRes",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildJoinReqRes));
	m_pCmdMgr->UnRegister("NS_GuildJoinBroad",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildJoinBroad));
	m_pCmdMgr->UnRegister("NS_GuildLeaveBroad",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildLeaveBroad));
	m_pCmdMgr->UnRegister("NS_GuildKickBroad",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildKickBroad));
	m_pCmdMgr->UnRegister("NS_GuildTurnoverBroad",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildTurnoverBroad));
	m_pCmdMgr->UnRegister("NS_GuildDemissBroad",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildDemissBroad));
	m_pCmdMgr->UnRegister("NS_GuildAppointBroad",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildAppointBroad));
	m_pCmdMgr->UnRegister("NS_GuildTenetChangeBroad",	(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildTenetChangeBroad));
	m_pCmdMgr->UnRegister("NS_GuildProcFailed",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildProcFailed));
	m_pCmdMgr->UnRegister("NS_GuildGetAllMembers",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildGetAllMembers));
	m_pCmdMgr->UnRegister("NS_GuildGetMemberEx",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildGetMemberEx));
	m_pCmdMgr->UnRegister("NS_GuildRefMember",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildRefMember));
	m_pCmdMgr->UnRegister("NS_GetGuildName",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildNameGet));
	m_pCmdMgr->UnRegister("NS_GetGuildTenet",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildTenetGet));
	m_pCmdMgr->UnRegister("NS_GetRoleInitState_Guild",	(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildAttInit));
	m_pCmdMgr->UnRegister("NS_SpreadGuildAffair",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildSpreadAffair));
	m_pCmdMgr->UnRegister("NS_SyncGuildInfo",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildSyncInfo));
	m_pCmdMgr->UnRegister("NS_GetGuildSkillInfo",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGetGuildSkillInfo));
	m_pCmdMgr->UnRegister("NS_SetResearchSkill",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetSetResearchSkill));
	m_pCmdMgr->UnRegister("NS_LearnGuildSkill",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetLearnGuildSkill));
	m_pCmdMgr->UnRegister("NS_GuildContribution",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildContribution));
	m_pCmdMgr->UnRegister("NS_GuildStateSet",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildStateSet));
	m_pCmdMgr->UnRegister("NS_GuildStateUnset",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildStateUnSet));
	m_pCmdMgr->UnRegister("NS_GuildSkillUpgrade",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildSkillUpgrade));
	m_pCmdMgr->UnRegister("NS_GuildSkillLevelUp",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildSkillLevelUp));
	m_pCmdMgr->UnRegister("NS_AcceptCommerce",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetAcceptCommerce));
	m_pCmdMgr->UnRegister("NS_GetTaelInfo",				(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGetTaelInfo));
	m_pCmdMgr->UnRegister("NS_CompleteCommerce",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetCompleteCommerce));
	m_pCmdMgr->UnRegister("NS_AbandonCommerce",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetAbandonCommerce));
	m_pCmdMgr->UnRegister("NS_GuildUpgrade",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildUpgrade));
	m_pCmdMgr->UnRegister("NS_GetCommodityInfo",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGetCommodityInfo));
	m_pCmdMgr->UnRegister("NS_GetCofCInfo",				(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGetCofCInfo));
    m_pCmdMgr->UnRegister("NS_GetCommerceRank",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGetCommerceRank));
	m_pCmdMgr->UnRegister("NS_SwitchCommendation",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetSwitchCommendation));
    m_pCmdMgr->UnRegister("NS_GetGuildWare",            (NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNS_GetGuildWare));
    m_pCmdMgr->UnRegister("NS_GuildWarePri",            (NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNS_GuildWarePri));
	m_pCmdMgr->UnRegister("NS_GuildExploit",			(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGuildExploit));
	m_pCmdMgr->UnRegister("NS_GetSomeGuildName",		(NETMSGPROC)m_Trunk.sfp2(&GuildMgr::OnNetGetSomeGuildName));

	if(P_VALID(GetObj("QuitCommerceWarning")))
		KillObj("QuitCommerceWarning");
	if(P_VALID(GetObj("JoinReq")))
		KillObj("JoinReq");
	if(P_VALID(GetObj("CreateGuild")))
		KillObj("CreateGuild");
	if(P_VALID(GetObj("DismissCheck")))
		KillObj("DismissCheck");

	ClearAffairs();
	ClearSkills();
	ClearCommerces();
	ClearGoodInfo();
	ClearCOCInfo();
	
	m_bSendGetAllMember = false;
	m_bGetTenet = false;

	m_syncFrameNames.clear();

}	


VOID GuildMgr::InitPosPower()
{
	m_vecAppoint.clear();
	m_vecKick.clear();
	m_vecPower.clear();
	

	m_vecAppoint.resize(X_GUILD_POS_NUM);
	m_vecKick.resize(X_GUILD_POS_NUM);
	m_vecPower.resize(X_GUILD_POS_NUM);
	
	const TCHAR* szGuildAppoint = _T("data\\system\\attdata\\guild_appoint.xml");
	const TCHAR* szGuildKick	= _T("data\\system\\attdata\\guild_kick.xml");
	const TCHAR* szGuildPower	= _T("data\\system\\attdata\\guild_power.xml");
	
	list<tstring> appointList;
	list<tstring> kickList;
	list<tstring> powerList;
	list<tstring>::iterator iter;

	//��ʼ��������
	TObjRef<VarContainer> varAppoint = CreateObj( "GuildAppoint", "VarContainer" );
	if(!varAppoint->Load("VFS_System", szGuildAppoint, "id", &appointList))
		IMSG(_T("Load file guild_appoint.xml failed\r\n"));
	
	for(iter=appointList.begin(); iter!=appointList.end(); ++iter)
	{
		tagGuildAppoint appoint;
		EGuildMemberPos ePos =  (EGuildMemberPos)varAppoint->GetInt(_T("id"),	(*iter).c_str(), EGMP_BangZhong);
		appoint.BitSetGuildAppoint.set(EGMP_BangZhong,		(TRUE == varAppoint->GetInt(_T("BangZhong"),		(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_JingYing,		(TRUE == varAppoint->GetInt(_T("JingYing"),		(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_BangZhu,		(TRUE == varAppoint->GetInt(_T("BangZhu"),			(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_FuBangZhu,		(TRUE == varAppoint->GetInt(_T("FuBangZhu"),		(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_HongHuFa,		(TRUE == varAppoint->GetInt(_T("HongHuFa"),		(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_ZiHuFa,			(TRUE == varAppoint->GetInt(_T("ZiHuFa"),			(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_QingLongTang,	(TRUE == varAppoint->GetInt(_T("QingLongTang"),	(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_QingLongXiang,	(TRUE == varAppoint->GetInt(_T("QingLongXiang"),	(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_QingLongZhong,	(TRUE == varAppoint->GetInt(_T("QingLongZhong"),	(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_ZhuQueTang,		(TRUE == varAppoint->GetInt(_T("ZhuQueTang"),		(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_ZhuQueXiang,	(TRUE == varAppoint->GetInt(_T("ZhuQueXiang"),		(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_ZhuQueZhong,	(TRUE == varAppoint->GetInt(_T("ZhuQueZhong"),		(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_BaiHuTang,		(TRUE == varAppoint->GetInt(_T("BaiHuTang"),		(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_BaiHuXiang,		(TRUE == varAppoint->GetInt(_T("BaiHuXiang"),		(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_BaiHuZhong,		(TRUE == varAppoint->GetInt(_T("BaiHuZhong"),		(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_XuanWuTang,		(TRUE == varAppoint->GetInt(_T("XuanWuTang"),		(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_XuanWuXiang,	(TRUE == varAppoint->GetInt(_T("XuanWuXiang"),		(*iter).c_str(), FALSE)));
		appoint.BitSetGuildAppoint.set(EGMP_XuanWuZhong,	(TRUE == varAppoint->GetInt(_T("XuanWuZhong"),		(*iter).c_str(), FALSE)));
		m_vecAppoint[ePos] = appoint;	
	}

	KillObj("GuildAppoint");

	//��ʼ�������
	TObjRef<VarContainer> varKick = CreateObj( "GuildKick", "VarContainer" );
	if(!varKick->Load("VFS_System", szGuildKick, "id", &kickList))
		IMSG(_T("Load file guild_kick.xml failed\r\n"));

	for(iter=kickList.begin(); iter!=kickList.end(); ++iter)
	{
		tagGuildKick kick;
		EGuildMemberPos ePos =  (EGuildMemberPos)varKick->GetInt(_T("id"),	(*iter).c_str(), EGMP_BangZhong);
		kick.BitSetGuildKick.set(EGMP_BangZhong,		(TRUE == varKick->GetInt(_T("BangZhong"),		(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_JingYing,			(TRUE == varKick->GetInt(_T("JingYing"),		(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_BangZhu,			(TRUE == varKick->GetInt(_T("BangZhu"),			(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_FuBangZhu,		(TRUE == varKick->GetInt(_T("FuBangZhu"),		(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_HongHuFa,			(TRUE == varKick->GetInt(_T("HongHuFa"),		(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_ZiHuFa,			(TRUE == varKick->GetInt(_T("ZiHuFa"),			(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_QingLongTang,		(TRUE == varKick->GetInt(_T("QingLongTang"),	(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_QingLongXiang,	(TRUE == varKick->GetInt(_T("QingLongXiang"),	(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_QingLongZhong,	(TRUE == varKick->GetInt(_T("QingLongZhong"),	(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_ZhuQueTang,		(TRUE == varKick->GetInt(_T("ZhuQueTang"),		(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_ZhuQueXiang,		(TRUE == varKick->GetInt(_T("ZhuQueXiang"),		(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_ZhuQueZhong,		(TRUE == varKick->GetInt(_T("ZhuQueZhong"),		(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_BaiHuTang,		(TRUE == varKick->GetInt(_T("BaiHuTang"),		(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_BaiHuXiang,		(TRUE == varKick->GetInt(_T("BaiHuXiang"),		(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_BaiHuZhong,		(TRUE == varKick->GetInt(_T("BaiHuZhong"),		(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_XuanWuTang,		(TRUE == varKick->GetInt(_T("XuanWuTang"),		(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_XuanWuXiang,		(TRUE == varKick->GetInt(_T("XuanWuXiang"),		(*iter).c_str(), FALSE)));
		kick.BitSetGuildKick.set(EGMP_XuanWuZhong,		(TRUE == varKick->GetInt(_T("XuanWuZhong"),		(*iter).c_str(), FALSE)));
		m_vecKick[ePos] = kick;
	}

	KillObj("GuildKick");


	//��ʼ��ְ�ܱ�
	TObjRef<VarContainer> varPower = CreateObj( "GuildPower", "VarContainer" );
	if(!varPower->Load("VFS_System", szGuildPower, "id", &powerList))
		IMSG(_T("Load file guild_power.xml failed\r\n"));

	for(iter=powerList.begin(); iter!=powerList.end(); ++iter)
	{
		tagGuildPower power;
		EGuildMemberPos ePos	= (EGuildMemberPos)varPower->GetInt(_T("id"),		(*iter).c_str(), EGMP_BangZhong);
		power.bDismissGuild		= (TRUE == varPower->GetInt(_T("DismissGuild"),		(*iter).c_str(), FALSE));
		power.bInviteJoin		= (TRUE == varPower->GetInt(_T("InviteJoin"),		(*iter).c_str(), FALSE));
		power.bTurnoverLeader	= (TRUE == varPower->GetInt(_T("TurnoverLeader"),	(*iter).c_str(), FALSE));
		power.bLeaveGuild		= (TRUE == varPower->GetInt(_T("LeaveGuild"),		(*iter).c_str(), FALSE));
		power.bDemissPostion	= (TRUE == varPower->GetInt(_T("DemissPostion"),	(*iter).c_str(), FALSE));
		power.bModifySgin		= (TRUE == varPower->GetInt(_T("ModifySgin"),		(*iter).c_str(), FALSE));
		power.bModifyTenet		= (TRUE == varPower->GetInt(_T("ModifyTenet"),		(*iter).c_str(), FALSE));
		power.bSetWareRights	= (TRUE == varPower->GetInt(_T("SetWareRights"),	(*iter).c_str(), FALSE));
		power.bUpgrade			= (TRUE == varPower->GetInt(_T("Upgrade"),			(*iter).c_str(), FALSE));
		power.bCommerce			= (TRUE == varPower->GetInt(_T("Commerce"),			(*iter).c_str(), FALSE));
		power.bAffair			= (TRUE == varPower->GetInt(_T("Affair"),			(*iter).c_str(), FALSE));
		power.bSetSkill			= (TRUE == varPower->GetInt(_T("SetSkill"),			(*iter).c_str(), FALSE));
		power.bAdvSkill			= (TRUE == varPower->GetInt(_T("AdvSkill"),			(*iter).c_str(), FALSE));
		power.bSetCommend		= (TRUE == varPower->GetInt(_T("SetCommend"),		(*iter).c_str(), FALSE));
		m_vecPower[ePos] = power;	
	}

	KillObj("GuildPower");

}

void GuildMgr::InitGuildAffairs()
{
	m_vecAffairInfo.clear();
	const TCHAR *szGuildAffair = _T("data\\system\\attdata\\guild_affair.xml");
	list<tstring> AffairList;
	
	TObjRef<VarContainer> varAffair = CreateObj("GuildAffair", "VarContainer");
	if(!varAffair->Load("VFS_System", szGuildAffair, "id", &AffairList))
		IMSG(_T("Load file guild_affair.xml failed\r\n"));
	
	list<tstring>::iterator end = AffairList.end();
	for (list<tstring>::iterator iter = AffairList.begin(); iter != end; ++iter)
	{
		tagGuildAffairInfo *pInfo = new	tagGuildAffairInfo();
		pInfo->dwID = varAffair->GetDword(_T("id"), iter->c_str(), 0);
		pInfo->nGuildLv = varAffair->GetInt(_T("guild_level"), iter->c_str(), 0);
		pInfo->nCity = varAffair->GetInt(_T("hold_city"), iter->c_str(), 0);
		pInfo->nFund = varAffair->GetInt(_T("fund"), iter->c_str(), 0);
		pInfo->nMaterial = varAffair->GetInt(_T("material"), iter->c_str(), 0);
		const tagBuffProtoClient *pBuffProtoClinet = SkillProtoData::Inst()->FindBuffProto(pInfo->dwID);
		if (pBuffProtoClinet)
		{
			pInfo->strName = pBuffProtoClinet->szName;
			pInfo->strDesc = pBuffProtoClinet->szDesc;
		}
		m_vecAffairInfo.push_back(pInfo);
	}

	KillObj("GuildAffair");
}

void GuildMgr::InitGuildSkills()
{
	m_mapSkill.clear();
	const TCHAR *szGuildSkillPath = _T("data\\system\\attdata\\guild_skill.xml");
	list<tstring> Skilllist;
	TObjRef<VarContainer> con = CreateObj("GuildSkill", "VarContainer");
	if(!con->Load("VFS_System", szGuildSkillPath, "id", &Skilllist))
		IMSG(_T("Load file guild_skill.xml failed\r\n"));
	list<tstring>::iterator end = Skilllist.end();
	for (list<tstring>::iterator iter = Skilllist.begin(); iter != end; ++iter)
	{
		tagGuildSkillInfo *pInfo = new tagGuildSkillInfo();
		pInfo->dwSkillID = con->GetDword(_T("id"), iter->c_str(), 0);
		pInfo->n16Fulfill = con->GetInt(_T("fullfill"), iter->c_str(), 0);
		pInfo->nResearchFund = con->GetInt(_T("research_fund"), iter->c_str(), 0);
		pInfo->nResearchMaterial = con->GetInt(_T("research_material"), iter->c_str(), 0);
		pInfo->nLearnSilver = con->GetInt(_T("learn_silver"), iter->c_str(), 0);
		pInfo->nLearnContribution = con->GetInt(_T("learn_contribution"), iter->c_str(), 0);
		pInfo->nLearnFund = con->GetInt(_T("learn_fund"), iter->c_str(), 0);
		pInfo->nLearnMaterial = con->GetInt(_T("learn_material"), iter->c_str(), 0);
		const tagSkillProtoClient *pSkillClient = SkillProtoData::Inst()->FindSkillProto(pInfo->dwSkillID);
		if (pSkillClient)
			pInfo->strName = pSkillClient->szName;
		m_mapSkill.insert(make_pair(pInfo->dwSkillID, pInfo));
	}
	KillObj("GuildSkill");
}

void GuildMgr::InitGuildCommerce()
{
	m_vecCommerceInfo.clear();
	const TCHAR *szGuildCommercePath = _T("data\\system\\attdata\\guild_commerce.xml");
	list<tstring> CommerceList;
	TObjRef<VarContainer> con = CreateObj("GuildCommerce", "VarContainer");
	if(!con->Load("VFS_System", szGuildCommercePath, "id", &CommerceList))
		IMSG(_T("Load file guild_commerce.xml failed\r\n"));
	list<tstring>::iterator end = CommerceList.end();
	for (list<tstring>::iterator iter = CommerceList.begin(); iter != end; ++iter)
	{
		tagCommerceInfo *pInfo = new tagCommerceInfo;
		pInfo->nLevelRange = con->GetInt(_T("id"), iter->c_str(), 0);
		pInfo->nDeposit = con->GetInt(_T("deposit"), iter->c_str(), 0);
		pInfo->nBeginningTael = con->GetInt(_T("beginning_tael"), iter->c_str(), 0);
		pInfo->nPurposeTael = con->GetInt(_T("purpose_tael"), iter->c_str(), 0);
		pInfo->nMaxTael = con->GetInt(_T("max_tael"), iter->c_str(), 0);
		m_vecCommerceInfo.push_back(pInfo);
	}
	KillObj("GuildCommerce");
}

void GuildMgr::InitGoodsInfo()
{
	m_mapGoodInfo.clear();
	tstring strPath = g_strLocalPath + _T("\\attdata\\guild_commodity_name.xml");
	list<tstring> GoodList;
	TObjRef<VarContainer> con = CreateObj("CommerceGoods", "VarContainer");
	if(!con->Load("VFS_System", strPath.c_str(), "id", &GoodList))
		IMSG(_T("Load file guild_commodity_name.xml failed\r\n"));
	list<tstring>::iterator end = GoodList.end();
	for (list<tstring>::iterator iter = GoodList.begin(); iter != end; ++iter)
	{
		tagGoodInfo *pInfo = new tagGoodInfo;
		pInfo->dwID = con->GetDword(_T("id"), iter->c_str(), 0);
		pInfo->strName = con->GetString(_T("name"), iter->c_str(), _T(""));
		pInfo->strIconPath = con->GetString(_T("icon"), iter->c_str(), _T(""));
		pInfo->strDisplayType = con->GetString(_T("display_type"), iter->c_str(), _T(""));
		pInfo->strDesc = con->GetString(_T("desc"), iter->c_str(), _T(""));
		m_mapGoodInfo.insert(std::make_pair(pInfo->dwID, pInfo));
	}
	KillObj("CommerceGoods");
}

void GuildMgr::InitCOCInfo()
{
	m_mapCOCInfo.clear();
	tstring strPath = g_strLocalPath + _T("\\attdata\\guild_CofC_name.xml");
	list<tstring> COCList;
	TObjRef<VarContainer> con = CreateObj("COC", "VarContainer");
	if(!con->Load("VFS_System", strPath.c_str(), "id", &COCList))
		IMSG(_T("Load file guild_CofC_name.xml failed\r\n"));
	list<tstring>::iterator end = COCList.end();
	for (list<tstring>::iterator iter = COCList.begin(); iter != end; ++iter)
	{
		tagCOCInfo *pInfo = new tagCOCInfo;
		pInfo->dwID = con->GetDword(_T("id"), iter->c_str(), 0);
		pInfo->strName = con->GetString(_T("name"), iter->c_str(), _T(""));
		pInfo->strDesc = con->GetString(_T("desc"), iter->c_str(), _T(""));
		m_mapCOCInfo.insert(std::make_pair(pInfo->dwID, pInfo));
	}
	KillObj("COC");
}

VOID GuildMgr::SendGuildCreate( LPCTSTR szGuildName, DWORD dwNPCID )
{
	//�ж������Ƿ�Ƿ�
	DWORD dwErrorCode = FilterData::Inst()->IsValidName(szGuildName, MAX_GUILD_NAME);
	if(E_Success != dwErrorCode)
	{
		//������ʾ��Ϣ
		ShowGuildErrorMsg(dwErrorCode);
		return;
	}

	//����������Ϣ
	INT size = (_tcslen(szGuildName) - 1) * sizeof(TCHAR) + sizeof(tagNC_GuildCreate);
	LPBYTE pMsg = new BYTE[size];
	tagNC_GuildCreate* pCmd = (tagNC_GuildCreate*)pMsg;
	pCmd->dwID = pCmd->Crc32("NC_GuildCreate");
	pCmd->dwNPCID = dwNPCID;
	memcpy(pCmd->szGuildName, szGuildName, _tcslen(szGuildName)*sizeof(TCHAR));
	pCmd->dwSize = size;
	m_pSession->Send(pCmd);
	SAFE_DEL_ARRAY(pMsg);
}

VOID GuildMgr::SendGuildDismiss( DWORD dwNPCID )
{
	//�ж��Ƿ���Ȩ�޽������

	tagNC_GuildDismiss cmd;
	cmd.dwNPCID = dwNPCID;
	m_pSession->Send(&cmd);

}

VOID GuildMgr::SendGuildJoinReq( DWORD dwRoleID )
{
	tagNC_GuildJoinReq cmd;
	cmd.dwDstRoleID = dwRoleID;
	m_pSession->Send(&cmd);
}


VOID GuildMgr::SortMembers( INT nCol )
{
	if (P_VALID(m_pMembers))
		m_pMembers->SortMember((GuildMembers::EListColType)nCol);
}

const tagGuildMemberClient* GuildMgr::GetMember( INT nIndex ) const
{
	if(nIndex <0 || nIndex >= (INT)m_pMembers->m_Members.size())
		return NULL;
	
	return m_pMembers->m_Members[nIndex];
}


const tagGuildMemberClient* GuildMgr::GetMemberByID( DWORD dwRoleID ) const
{
	if( P_VALID(m_pMembers) )
	{
		return m_pMembers->FindMember(dwRoleID);
	}
	return NULL;
}


INT GuildMgr::GetMemberIndex( DWORD dwRoleID ) const
{
	return m_pMembers->FindIndex(dwRoleID);
}

tstring GuildMgr::GetMemberSex( BYTE bySex ) const
{
	if(ES_Man == bySex)
	{
		return g_StrTable[_T("RoleInfoMale")];
	}
	else if(ES_Woman == bySex)
	{
		return g_StrTable[_T("RoleInfoFemale")];
	}
	return _T("");
}

tstring GuildMgr::GetMemberPos( INT nPos ) const
{
	if(IsGuildPosValid(EGuildMemberPos(nPos)))
	{
		if(IsInTang((EGuildMemberPos)nPos))
		{
			EGuildTangType eTang;
			ETangMemberPos eTangPos;
			GetTangType((EGuildMemberPos)nPos, eTang, eTangPos);

			tstring strRet;
			TCHAR szTmp[X_SHORT_NAME];
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("GuildTang_Name%d"), eTang);
			strRet = g_StrTable[szTmp];
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("GuildPos_Tang%d"), eTangPos);
			strRet += g_StrTable[szTmp];	
			return strRet;
		}
		else
		{
			TCHAR szTmp[X_SHORT_NAME];
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("GuildPos_Name%d"), nPos);
			return g_StrTable[szTmp];
		}
	}
	return _T("");
}

const GuildMembers* GuildMgr::GetMembers() const
{
	return m_pMembers;
}

const tagGuildBase* GuildMgr::GetGuildInfo() const
{
	return m_pGuild;
}

const tagGuildMember* GuildMgr::GetLpInfo() const
{
	return m_pLpInfo;
}

bool GuildMgr::IsCanJionReq( DWORD dwRoleID , DWORD *pReaseon) const
{
	// ���ж��Լ���û��������ʸ�
	if(!P_VALID(m_pLpInfo))//������Ҳ��ڰ��������Է�
	{
		if (P_VALID(pReaseon))
			*pReaseon = (DWORD)(ERQ_NoSelfRight);
		return false;
	}
	const tagGuildPower& sPower = m_vecPower[m_pLpInfo->eGuildPos];
	if(!sPower.bInviteJoin)//û������Ȩ�޲�������Է�
	{
		if (P_VALID(pReaseon))
			*pReaseon = (DWORD)(ERQ_NoSelfRight);
		return false;
	}

	// ���ж϶Է��Ƿ���Ա�����
	if(!IS_PLAYER(dwRoleID))// ������Ҳ��ܱ�����
	{
		if (P_VALID(pReaseon))
			*pReaseon = (DWORD)(ERQ_OtherReason);
		return false;
	}

	Player* pPlayer = (Player*)RoleMgr::Inst()->FindRole(dwRoleID);
	if(P_VALID(pPlayer) && pPlayer->IsInGuild())//����ڰﲻ�ܱ�����
	{
		// ������������ͱ��������ͬһ������
		if (pPlayer->GetRoleGuildID() == RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID())
		{
			if (P_VALID(pReaseon))
				*pReaseon = (DWORD)(ERQ_AlreadyInSelfGuild);
		}
		else
		{
			if (P_VALID(pReaseon))
				*pReaseon = (DWORD)(ERQ_AlreadyInOtherGuild);
		}
		return false;
	}
	
	return true;
}

const tagGuildPower* GuildMgr::GetPowerList( INT nPos ) const
{
	if(nPos < 0 || nPos >= X_GUILD_POS_NUM)
		return NULL;

	return &m_vecPower[nPos];
}

const tagGuildKick* GuildMgr::GetKickList( INT nPos ) const
{
	if(nPos < 0 || nPos >= X_GUILD_POS_NUM)
		return NULL;

	return &m_vecKick[nPos];
}

const tagGuildAppoint* GuildMgr::GetAppointList( INT nPos ) const
{
	if(nPos < 0 || nPos >= X_GUILD_POS_NUM)
		return NULL;

	return &m_vecAppoint[nPos];
}

void GuildMgr::SendGetSomeGuildName(int nFlag, const std::vector<DWORD> &vecIDList)
{
	tagNC_GetSomeGuildName cmd;
	cmd.nUserData = nFlag;
	cmd.nNum = vecIDList.size();
	int size = sizeof(tagNC_GetSomeGuildName) - sizeof(DWORD) + sizeof(DWORD) * cmd.nNum;
	BYTE *pByAllocData = new BYTE[size];
	tagNC_GetSomeGuildName *pRGSCmd = (tagNC_GetSomeGuildName*)pByAllocData;
	memcpy(pRGSCmd, &cmd, (sizeof(cmd)));
	for (int i = 0; i < cmd.nNum; ++i)
	{
		pRGSCmd->dwAllID[i] = vecIDList[i];
	}
	pRGSCmd->dwSize = size;
	m_pSession->Send(pRGSCmd);
	delete [] pByAllocData;
}
tstring GuildMgr::GetGuildName( DWORD dwGuildID )
{
	std::map<DWORD, tstring>::iterator it = m_mapGuildName.find(dwGuildID);
	if(it != m_mapGuildName.end())
		return it->second;
	else
	{
		//����Ѿ��ڻ��������������
		if(m_setNameReq.find(dwGuildID) != m_setNameReq.end())
			return _T("");
		//���GuildMgr��û�б����ӦID��name������������Ϣ
		tagNC_GetGuildName cmd;
		cmd.dwGuildID = dwGuildID;
		m_pSession->Send(&cmd);
		
		//���浽������
		m_setNameReq.insert(dwGuildID);
	}
	return _T("");
}
bool GuildMgr::IsGuildNameExist(DWORD dwGuildID)
{
	std::map<DWORD, tstring>::iterator iter = m_mapGuildName.find(dwGuildID);
	if (iter != m_mapGuildName.end())
		return true;
	return false;
}

DWORD GuildMgr::OnNetGuildCreateBroad( tagNS_GuildCreateBroad* pNetCmd, DWORD )
{
	// ���������Ҫ��ȡ������Ϣ
	if(pNetCmd->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
	{
		ClearGuildInfo();
		m_bIsInitFinish = TRUE;
		tagNC_GetRoleInitState cmd;
		cmd.eType = ERIT_Guild;
		m_pSession->Send(&cmd);

		// ���ñ�����Ұ���״̬����
		LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
		if(P_VALID(pLp))
		{
			pLp->SetRoleGuildID(pNetCmd->dwGuildID);
			pLp->SetRoleGuildPos(EGMP_BangZhu);
		}
	}

	// pNetCmd->szName = "��ɫ��\0������\0"
	TCHAR szTmp[X_SHORT_NAME];
	// �����������ƫ����
	INT nPos = _tcslen(pNetCmd->szName) + 1;
	// ����������ĳ���
	INT nSize = (pNetCmd->dwSize - sizeof(tagNS_GuildCreateBroad))/sizeof(TCHAR) + 1 - nPos;
	_tcsncpy(szTmp, pNetCmd->szName+nPos, nSize);
	
	// ��ʾ������Ϣ
	CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("GuildBroad_GuildCreate")], pNetCmd->szName, szTmp);
	
	TCHAR szBuffer[256];
	_sntprintf(szBuffer, 256, g_StrTable[_T("GuildBroad_GuildCreate")], pNetCmd->szName, szTmp);

	ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
	if( P_VALID(pFrame) )
	{
		pFrame->PushInfo(szBuffer, ESCC_Affiche);
	}
	
	return 0;
}

DWORD GuildMgr::OnNetGuildDismissBroad( tagNS_GuildDismissBroad* pNetCmd, DWORD )
{
	// ����������ǵ�ǰ��ң�ɾ��������Ϣ
	if(pNetCmd->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
	{
		ClearGuildInfo();
	}
	
	// ��ʾ������Ϣ
	CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("GuildBroad_GuildDismiss")], pNetCmd->szGuildName);
	TCHAR szBuffer[256];
	_sntprintf(szBuffer, 256, g_StrTable[_T("GuildBroad_GuildDismiss")], pNetCmd->szGuildName);
	ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
	if( P_VALID(pFrame) )
	{
		pFrame->PushInfo(szBuffer, ESCC_Affiche);
	}
	return 0;
}

DWORD GuildMgr::OnNetGuildJoinReq( tagNS_GuildJoinReq* pNetCmd, DWORD )
{
	// ��¼�����߼������
	m_dwInviterID	 = pNetCmd->dwSrcRoleID;
	m_dwInviterGuild = pNetCmd->dwGuildID;

	// ��ʾѡ���
	ShowJoinReqMsgBox();
	return 0;
}

DWORD GuildMgr::OnNetGuildJoinReqRes( tagNS_GuildJoinReqRes* pNetCmd, DWORD )
{
	if(E_Success == pNetCmd->dwErrorCode)
	{
		if(pNetCmd->dwInviterID == RoleMgr::Inst()->GetLocalPlayerID())// ������
		{
			
		}
		else if(pNetCmd->dwInviteeID == RoleMgr::Inst()->GetLocalPlayerID())// ��������
		{

		}
	}
	else
	{
		// ��ʾ������Ϣ
		ShowGuildErrorMsg(pNetCmd->dwErrorCode);
	}
	return 0;
}

DWORD GuildMgr::OnNetGuildJoinBroad( tagNS_GuildJoinBroad* pNetCmd, DWORD )
{
	// ��ʾ������Ϣ
	ShowGuildMsg(g_StrTable[_T("GuildBroad_JoinGuild")], pNetCmd->szRoleName);
	
	// ����Ǳ�����ұ�����
	if(pNetCmd->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
	{
		ClearGuildInfo();
		m_bIsInitFinish = TRUE;
		tagNC_GetRoleInitState cmd;
		cmd.eType = ERIT_Guild;
		m_pSession->Send(&cmd);
	}
	else
	{
		// ������Ϣδ��ȡ�򲻴������Ϣ
		if(!P_VALID(m_pGuild))
			return 0;
		// ���ɳ�Ա��Ϣδ��ȡ��ֻ�ı��Ա����
		if(!P_VALID(m_pMembers))
		{
			// ��Ҽ�����ɳ�Ա�����Լ�
			++m_pGuild->n16MemberNum;
			return 0;
		}
		
		// ����������Ϣ��ȡ�����Ϣ
		tagNC_GuildRefMember cmd;
		cmd.dwRoleID = pNetCmd->dwRoleID;
		m_pSession->Send(&cmd);
	}
	return 0;
}

DWORD GuildMgr::OnNetGuildProcFailed( tagNS_GuildProcFailed* pNetCmd, DWORD )
{
	if(E_Success != pNetCmd->dwErrorCode && E_Guild_Create_Success != pNetCmd->dwErrorCode)
	{
		// ��ʾ������Ϣ
		ShowGuildErrorMsg(pNetCmd->dwErrorCode);
	}
	return 0;
}

DWORD GuildMgr::OnNetGuildLeaveBroad( tagNS_GuildLeaveBroad* pNetCmd, DWORD )
{
	if(pNetCmd->dwSrcRoleID == RoleMgr::Inst()->GetLocalPlayerID())
	{
		// ����Ǳ����뿪�������������Ϣ
		ClearGuildInfo();
	}
	else
	{
		// ������Ϣδ��ȡ�򲻴������Ϣ
		if(!P_VALID(m_pGuild))
			return 0;
		// ���ɳ�Ա��Ϣδ��ȡ��ֻ�ı��Ա����
		if(!P_VALID(m_pMembers))
		{
			// ����뿪���ɳ�Ա�����Լ�
			--m_pGuild->n16MemberNum;
			return 0;
		}

		// ���Լ��뿪���ɵ���Ҵӳ�Ա�б���ɾ��
		m_pMembers->RemoveMember(pNetCmd->dwSrcRoleID);
		// ���°��ɳ�Ա������Ϣ
		m_pGuild->n16MemberNum = m_pMembers->m_Members.size();

		// ������Ϸ�¼�
		tagGuildInfoReflashEvent event(_T("GuildEvent_RemoveMember"), NULL);
		event.dwRoleID = pNetCmd->dwSrcRoleID;
		m_pFrameMgr->SendEvent(&event);
	}

	// ��ʾ������Ϣ
	LPCTSTR szTmp = PlayerNameTab::Inst()->FindNameByID(pNetCmd->dwSrcRoleID);
	if(szTmp != NULL && _tcslen(szTmp) > 0)
	{
		ShowGuildMsg(g_StrTable[_T("GuildBroad_LeaveGuild")], szTmp);
	}
	return 0;
}

DWORD GuildMgr::OnNetGuildKickBroad( tagNS_GuildKickBroad* pNetCmd, DWORD )
{
	if(pNetCmd->dwDstRoleID == RoleMgr::Inst()->GetLocalPlayerID())
	{
		// ����Ǳ��˱������������������Ϣ
		ClearGuildInfo();
	}
	else
	{
		// ������Ϣδ��ȡ�򲻴������Ϣ
		if(!P_VALID(m_pGuild))
			return 0;
		// ���ɳ�Ա��Ϣδ��ȡ��ֻ�ı��Ա����
		if(!P_VALID(m_pMembers))
		{
			// ��ұ�������ɳ�Ա�����Լ�
			--m_pGuild->n16MemberNum;
			return 0;
		}

		// �����������Ҵӳ�Ա�б���ɾ��
		m_pMembers->RemoveMember(pNetCmd->dwDstRoleID);
		// ���°��ɳ�Ա������Ϣ
		m_pGuild->n16MemberNum = m_pMembers->m_Members.size();

		// ������Ϸ�¼�
		tagGuildInfoReflashEvent event(_T("GuildEvent_RemoveMember"), NULL);
		event.dwRoleID = pNetCmd->dwDstRoleID;
		m_pFrameMgr->SendEvent(&event);
	}

	// ��ʾ������Ϣ
	LPCTSTR szTmp = PlayerNameTab::Inst()->FindNameByID(pNetCmd->dwDstRoleID);
	if(szTmp != NULL && _tcslen(szTmp) > 0)
	{
		ShowGuildMsg(g_StrTable[_T("GuildBroad_KickMember")], szTmp);
	}
	return 0;
}

DWORD GuildMgr::OnNetGuildTurnoverBroad( tagNS_GuildTurnoverBroad* pNetCmd, DWORD )
{
	// ������ͨ����
	if(P_VALID(m_pMembers))
	{	
		// ����ǰ������ְλ��ִ�ж���ת�Ƶ�Ŀ�����ִ�ж���ְλ��Ϊ��ͨ����
		tagGuildMemberClient* pMember = m_pMembers->FindMember(pNetCmd->dwSrcRoleID);
		if(P_VALID(pMember))
		{
			pMember->sInfo.n8GuildPos = (INT8)EGMP_BangZhong;
		}
		pMember = m_pMembers->FindMember(pNetCmd->dwDstRoleID);
		if(P_VALID(pMember))
		{
			pMember->sInfo.n8GuildPos = (INT8)EGMP_BangZhu;
		}
	}

	// ���������
	if(P_VALID(m_pLpInfo))
	{
		if(m_pLpInfo->dwRoleID == pNetCmd->dwDstRoleID)
			SetLocalPlayerGuildPos((INT8)EGMP_BangZhu);
		else if(m_pLpInfo->dwRoleID == pNetCmd->dwSrcRoleID)
			SetLocalPlayerGuildPos((INT8)EGMP_BangZhong);
	}

	// �޸����ΰ���
	if(P_VALID(m_pGuild))
		m_pGuild->dwLeaderRoleID = pNetCmd->dwDstRoleID;
	
	// ��ʾ������Ϣ
	LPCTSTR szSrc = PlayerNameTab::Inst()->FindNameByID(pNetCmd->dwSrcRoleID);
	LPCTSTR szDst = PlayerNameTab::Inst()->FindNameByID(pNetCmd->dwDstRoleID);
	if(szSrc != NULL && _tcslen(szSrc) > 0 && szDst != NULL && _tcslen(szDst) > 0)
	{
		ShowGuildMsg(g_StrTable[_T("GuildBroad_TurnoverPos")], szSrc, szDst);
	}

	// ������Ϸ�¼�
	tagGuildInfoReflashEvent event(_T("GuildEvent_PosChange"), NULL);
	event.dwRoleID = pNetCmd->dwSrcRoleID;
	m_pFrameMgr->SendEvent(&event);
	event.dwRoleID = pNetCmd->dwDstRoleID;
	m_pFrameMgr->SendEvent(&event);
	return 0;
}

DWORD GuildMgr::OnNetGuildDemissBroad( tagNS_GuildDemissBroad* pNetCmd, DWORD )
{	
	INT8 n8OldPos = EGMP_Null;

	// ������ͨ���ɳ�Ա
	if(P_VALID(m_pMembers))
	{
		// ����ְ�����ְλ��Ϊ��ͨ����
		tagGuildMemberClient* pMember = m_pMembers->FindMember(pNetCmd->dwRoleID);
		if(P_VALID(pMember))
		{
			n8OldPos = pMember->sInfo.n8GuildPos;
			pMember->sInfo.n8GuildPos = (INT8)EGMP_BangZhong;
		}
	}
	// ���������
	if(P_VALID(m_pLpInfo) && m_pLpInfo->dwRoleID == pNetCmd->dwRoleID)
	{
		n8OldPos = (INT8)m_pLpInfo->eGuildPos;
		SetLocalPlayerGuildPos((INT8)EGMP_BangZhong);
	}

	// ��ʾ������Ϣ
	LPCTSTR szName = PlayerNameTab::Inst()->FindNameByID(pNetCmd->dwRoleID);
	if(n8OldPos != EGMP_Null && szName != NULL && _tcslen(szName) > 0)
	{
		ShowGuildMsg(g_StrTable[_T("GuildBroad_DemissPos")], szName, GetMemberPos(n8OldPos).c_str());
	}

	// ������Ϸ�¼�
	tagGuildInfoReflashEvent event(_T("GuildEvent_PosChange"), NULL);
	event.dwRoleID = pNetCmd->dwRoleID;
	m_pFrameMgr->SendEvent(&event);
	return 0;
}

DWORD GuildMgr::OnNetGuildAppointBroad( tagNS_GuildAppointBroad* pNetCmd, DWORD )
{
	// ������ͨ���ɳ�Ա
	if(P_VALID(m_pMembers))
	{
		// �޸����������ְλ
		tagGuildMemberClient* pMember = m_pMembers->FindMember(pNetCmd->dwDstRoleID);
		if(P_VALID(pMember))
		{
			pMember->sInfo.n8GuildPos = (INT8)pNetCmd->ePos;
		}
	}

	// ���������
	if(P_VALID(m_pLpInfo) && m_pLpInfo->dwRoleID == pNetCmd->dwDstRoleID)
		SetLocalPlayerGuildPos((INT8)pNetCmd->ePos);

	// ��ʾ������Ϣ
	LPCTSTR szSrc = PlayerNameTab::Inst()->FindNameByID(pNetCmd->dwSrcRoleID);
	LPCTSTR szDst = PlayerNameTab::Inst()->FindNameByID(pNetCmd->dwDstRoleID);
	if(szSrc != NULL && _tcslen(szSrc) > 0 && szDst != NULL && _tcslen(szDst) > 0)
	{
		ShowGuildMsg(g_StrTable[_T("GuildBroad_AppointPos")], szDst, szSrc, GetMemberPos((INT)pNetCmd->ePos).c_str());
	}

	// ������Ϸ�¼�
	tagGuildInfoReflashEvent event(_T("GuildEvent_PosChange"), NULL);
	event.dwRoleID = pNetCmd->dwDstRoleID;
	m_pFrameMgr->SendEvent(&event);
	return 0;
}

DWORD GuildMgr::OnNetGuildTenetChangeBroad( tagNS_GuildTenetChangeBroad* pNetCmd, DWORD )
{
	// ������ּ����
	if(pNetCmd->szTenet == NULL || _tcslen(pNetCmd->szTenet) <= 0)
		m_strTenet = _T("");
	else
		m_strTenet = pNetCmd->szTenet;

	// ���͵�����Ƶ��
	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildBroad_TenetChange")]);
	ShowGuildMsg(g_StrTable[_T("GuildBroad_TenetGet")], pNetCmd->szTenet);
	
	// ������Ϸ�¼�
	tagGameEvent e(_T("GuildEvent_TenetGet"), NULL);
	m_pFrameMgr->SendEvent(&e);
	return 0;
}

DWORD GuildMgr::OnNetGuildAttInit( tagNS_GetRoleInitState_Guild* pNetCmd, DWORD )
{
	if(!P_VALID(m_pGuild))
		m_pGuild = new tagGuildBase;
	// ��ʼ��������Ϣ	
	memcpy(m_pGuild, &pNetCmd->sGuildBase, sizeof(tagGuildBase));

	// ��ʼ�����ɵ��Ź�ָ��
	MallMgr::Inst()->SetGroupPurExp( m_pGuild->nGroupPurchase );
	
	// ��ʼ������Ȩ�ޱ�
	InitPosPower();

	// ��ʼ�����������
	InitGuildAffairs();

	// ��ʼ�����ɼ��ܱ�
	InitGuildSkills();

	// ��ʼ���������̱�
	InitGuildCommerce();

	// ��ʼ����Ʒ��Ϣ
	InitGoodsInfo();

	// ��ʼ���̻���Ϣ
	InitCOCInfo();
	
	// ��ʼ���������
	if(!P_VALID(m_pLpInfo))
		m_pLpInfo = new tagGuildMember;
	memcpy(m_pLpInfo, &pNetCmd->sGuildMember, sizeof(tagGuildMember));
	// ���ñ�����Ұ���ְ��
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp))
	{
		pLp->SetRoleGuildID(pNetCmd->sGuildBase.dwID);
		pLp->SetRoleGuildPos((INT8)pNetCmd->sGuildMember.eGuildPos);
	}

	// ��ȡ�������ƺͰ�������ʼ������
	GetGuildName(m_pGuild->dwID);
	PlayerNameTab::Inst()->FindNameByID(m_pGuild->dwLeaderRoleID);
	PlayerNameTab::Inst()->FindNameByNameID(m_pGuild->dwFounderNameID);
	//// ��ȡ������ּ
	//tagNC_GetGuildTenet cmd;
	//m_pSession->Send(&cmd);

	if(!m_bIsInitFinish)	
	{
		m_bIsInitFinish = TRUE;
		// �ڳ�ʼ����Ϣ����ͱ�����ҳ�ʼ�������Ϣ
		tagGameEvent evtInit( _T("tagInitLPAttEvent"), NULL );
		m_pFrameMgr->SendEvent(&evtInit);
	}
	
	if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleState(ERS_Commerce))
	{
		GuildBusinessStateWnd *pFrame = (GuildBusinessStateWnd*)m_pFrameMgr->GetFrame(_T("GuildBusinessState"));
		if (!P_VALID(pFrame))
		{
			m_pFrameMgr->CreateFrame(_T("World"), _T("GuildBusinessState"), _T("GuildBusinessStateWnd"), 0);
		}
	}
	return 0;
}

DWORD GuildMgr::OnNetGuildNameGet( tagNS_GetGuildName* pNetCmd, DWORD )
{
	// �����¼
	tstring strTmp;
	if(pNetCmd->szGuildName == NULL || _tcslen(pNetCmd->szGuildName) <= 0)
		strTmp = _T("");
	else
		strTmp = pNetCmd->szGuildName;

	m_mapGuildName.insert(make_pair(pNetCmd->dwGuildID, strTmp));

	// ������Ϸ�¼�
	tagGuildNameGetEvent e(_T("GuildEvent_NameGet"), NULL);
	e.dwGuildID	=	pNetCmd->dwGuildID;
	e.strName	=	strTmp;
	m_pFrameMgr->SendEvent(&e);
	
	m_setNameReq.erase(pNetCmd->dwGuildID);

	//������������������ߵ���ͬ���򵯳�����Ի���
	if(pNetCmd->dwGuildID == m_dwInviterGuild)
	{
		ShowJoinReqMsgBox();
	}
	return 0;
}

DWORD GuildMgr::OnNetGuildTenetGet( tagNS_GetGuildTenet* pNetCmd, DWORD )
{
	// ������ּ����
	if(pNetCmd->szGuildTenet == NULL || _tcslen(pNetCmd->szGuildTenet) <= 0)
		m_strTenet = _T("");
	else
		m_strTenet = pNetCmd->szGuildTenet;

	// ���͵�����Ƶ��
	ShowGuildMsg(g_StrTable[_T("GuildBroad_TenetGet")], pNetCmd->szGuildTenet);

	// ������Ϸ�¼�
	tagGameEvent e(_T("GuildEvent_TenetGet"), NULL);
	m_pFrameMgr->SendEvent(&e);
	return 0;
}

VOID GuildMgr::SendGuildAllMembersGet()
{
	if(!P_VALID(m_pGuild))
		return;
	if (m_bSendGetAllMember)
		return;
	// �����û�г�Ա�б��򴴽�
	if(!P_VALID(m_pMembers))
		m_pMembers = new GuildMembers;
	else // ������У�����ճ�Ա�б�
		m_pMembers->FreeArray();
	// ���ӱ�����ҵ���Ա�б�
	AddLocalPlayer();
	
	GuildFrame *pFrame = (GuildFrame*)m_pFrameMgr->GetFrame(_T("Guild"));


	if(m_pGuild->n16MemberNum > 1)
	{
		tagNC_GuildGetAllMembers cmd;
		m_pSession->Send(&cmd);
		m_bSendGetAllMember = true;
	}
	else
	{
		m_pFrameMgr->SendEvent(&tagGameEvent(_T("GuildEvent_ListReflash"), NULL));
	}

}

VOID GuildMgr::AddLocalPlayer()
{
	if(!P_VALID(m_pLpInfo))
		return;

	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return;
	
	tagGuildMemberClient* pMember = new tagGuildMemberClient;
	// �������Ը�ֵ
	pMember->sInfo.dwRoleID			= m_pLpInfo->dwRoleID;
	pMember->sInfo.n8GuildPos		= (INT8)m_pLpInfo->eGuildPos;
	pMember->sInfo.byLevel			= (BYTE)pLp->GetRoleLevel();
	pMember->sInfo.bySex			= pLp->GetPlayerSex();
	pMember->sInfo.byClass			= (BYTE)pLp->GetPlayerClass();
	pMember->sInfo.nCurContrib		= m_pLpInfo->nContribution;		
	pMember->sInfo.dwTimeLastLogout	= GT_INVALID;
	pMember->sInfo.bOnline			= true;
	// ��չ���Ը�ֵ
	pMember->sInfoEx.nTotalContrib	= m_pLpInfo->nTotalContribution;
	pMember->sInfoEx.nExploit		= m_pLpInfo->nExploit;	
	pMember->sInfoEx.nKnowledge		= pLp->GetAttribute(ERA_Knowledge);
	// �����Ƿ��ȡ��չ��Ϣ��־λ
	pMember->bGetInfo = true;

	m_pMembers->m_Members.push_back(pMember);
}

DWORD GuildMgr::OnNetGuildGetAllMembers( tagNS_GuildGetAllMembers* pNetCmd, DWORD )
{
	m_bSendGetAllMember = false;
	if(pNetCmd->dwErrorCode == E_Success)
	{
		// ��û�����򴴽����ɳ�Ա�б�
		if(!P_VALID(m_pMembers))
			m_pMembers = new GuildMembers;

		for(INT i=0,pos=0; i<pNetCmd->n16Num; ++i)
		{
			// ��������
			tagGuildMemInfo m;
			memcpy(&m, pNetCmd->byData+pos, sizeof(tagGuildMemInfo));
			// ����ƫ����
			pos += sizeof(tagGuildMemInfo);
			// �����Ա�б�
			m_pMembers->AddMember(m);
		}

		// ������Ϸ�¼�
		tagGameEvent event(_T("GuildEvent_ListReflash"), NULL);
		m_pFrameMgr->SendEvent(&event);
	}
	else
	{
		// ��ʾ������Ϣ
		ShowGuildErrorMsg(pNetCmd->dwErrorCode);
	}
	return 0;
}

DWORD GuildMgr::OnNetGuildGetMemberEx( tagNS_GuildGetMemberEx* pNetCmd, DWORD )
{
	// �޸�ĳ����Ա����չ��Ϣ
	m_pMembers->ModifyMember(pNetCmd->dwRoleID, pNetCmd->sGuildMemInfoEx);

	// ���������Ϸ�¼�
	tagGuildInfoReflashEvent event(_T("GuildEvent_InfoUpdate"), NULL);
	event.dwRoleID = pNetCmd->dwRoleID;
	m_pFrameMgr->SendEvent(&event);
	return 0;
}

DWORD GuildMgr::OnNetGuildRefMember( tagNS_GuildRefMember* pNetCmd, DWORD )
{
	// ˢ��ĳ����Ա����ͨ��Ϣ����չ��Ϣ
	m_pMembers->ModifyMember(pNetCmd->sGuildMemInfo, pNetCmd->sGuildMemInfoEx);

	// ������������һ�����ɳ�Ա�����°��ɳ�Ա������Ϣ
	m_pGuild->n16MemberNum = m_pMembers->m_Members.size();
	
	// ���������Ϸ�¼�
	tagGuildInfoReflashEvent event(_T("GuildEvent_InfoReflash"), NULL);
	event.dwRoleID = pNetCmd->sGuildMemInfo.dwRoleID;
	m_pFrameMgr->SendEvent(&event);
	return 0;
}

VOID GuildMgr::ShowJoinReqMsgBox()
{
	// ��ȡ�����ߵ������͹���
	LPCTSTR szName  = PlayerNameTab::Inst()->FindNameByID(m_dwInviterID);
	tstring strGuild = GetGuildName(m_dwInviterGuild);
	// ���ֺͰ���Ϊ���򷵻�
	if(_tcslen(szName) <=0 || strGuild.size() <=0)
		return;

	TCHAR szTmp[X_LONG_NAME];
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildText_JionReq")], szName, strGuild.c_str());

	if(P_VALID(GetObj("JoinReq")))
		KillObj("JoinReq");

	CreateObj("JoinReq", "MsgBox");
	TObjRef<MsgBox>("JoinReq")->Init(
		_T(""), szTmp, _T("MsgBox_JoinReq"), (MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE);
}

DWORD GuildMgr::OnEventMsgBoxJoinReq( tagMsgBoxEvent* pGameEvent )
{
	// ����������
	tagNC_GuildJoinReqRes cmd;
	cmd.dwDstRoleID	 = m_dwInviterID;
	cmd.dwDstGuildID = m_dwInviterGuild;
	if(MBF_OK == pGameEvent->eResult)
		cmd.dwErrorCode = E_Success;
	else 
		cmd.dwErrorCode = E_Guild_Join_BeRefused;
	m_pSession->Send(&cmd);
	// �����������Ϣ
	m_dwInviterID    = GT_INVALID;
	m_dwInviterGuild = GT_INVALID;
	return 0;
}

DWORD GuildMgr::OnEventGuildCreate( tagGameEvent* pGameEvent )
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return 0;
	// ����а����򷵻�
	if(GT_VALID(pLp->GetRoleGuildID()))
	{
		ShowGuildErrorMsg(E_Guild_Create_AlreadyIn);
		return 0;
	}
	// �ȼ���������ʾ
	if(pLp->GetRoleLevel() < CREATE_NEED_LEVEL)
	{
		ShowGuildErrorMsg(E_Guild_Create_LevelLimit);
		return 0;
	}
	// ��Ǯ��������ʾ
	if(CurrencyMgr::Inst()->GetBagSilver() < MGold2Silver(CREATE_NEED_GOLD))
	{
		ShowGuildErrorMsg(E_BagSilver_NotEnough);
		return 0;
	}

	// �����������������
	if(P_VALID(GetObj("CreateGuild")))
		KillObj("CreateGuild");

	CreateObj("CreateGuild", "MsgInputBox");
	TObjRef<MsgInputBox>("CreateGuild")->Init(
		_T(""),g_StrTable[_T("GuildText_InputName")],
		_T("MsgBox_CreateGuild"),GT_INVALID, MBIT_Name);
	return 0;
}

DWORD GuildMgr::OnEventGuildDismiss( tagGameEvent* pGameEvent )
{
	// ������һ�ȡȨ�ޱ�
	if(!P_VALID(m_pLpInfo))
		return 0;

	const tagGuildPower* pPower = GetPowerList((INT)m_pLpInfo->eGuildPos);
	if(!P_VALID(pPower))
		return 0;
	// ���û�н�ɢ���ɵ�Ȩ������ʾ
	if(!pPower->bDismissGuild)
	{
		ShowGuildErrorMsg(E_Guild_Power_NotEnough);
		return 0;
	}
	
	// ������ɳ�Ա��ֻ�Լ����򷵻���ʾ
	if(!P_VALID(m_pGuild) || m_pGuild->n16MemberNum > 1)
	{
		
		ShowGuildErrorMsg(E_Guild_MemberHasMore);
		return 0;
	}
	
	// ����ȷ�϶Ի���
	if(P_VALID(GetObj("DismissCheck")))
		KillObj("DismissCheck");

	CreateObj("DismissCheck", "MsgBox");
	TObjRef<MsgBox>("DismissCheck")->Init(
		_T(""), g_StrTable[_T("GuildText_DismissCheck")],
		_T("MsgBox_DismissCheck"), (MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE);
	return 0;
}

DWORD GuildMgr::OnEventMsgBoxDismissCheck( tagMsgBoxEvent* pGameEvent )
{
	if(MBF_OK == pGameEvent->eResult)
	{
		// ���ͽ�ɢ������Ϣ
		SendGuildDismiss(QuestMgr::Inst()->GetTalkNPCGlobalID());
	}
	return 0;
}


DWORD GuildMgr::OnEventMsgBoxQuitCommerceWarning(tagMsgBoxEvent* pGameEvent)
{
	if (MBF_OK == pGameEvent->eResult)
	{
		SendQuitCommerce();
	}
	return 0;
}

DWORD GuildMgr::OnEventMsgBoxInputName( tagMsgInputBoxEvent* pGameEvent )
{
	if(MBIT_Name == pGameEvent->eInputType && MBF_OK == pGameEvent->eResult)
	{
		// ������Ҵ����¼�
		SendGuildCreate(pGameEvent->strInputName.c_str(), QuestMgr::Inst()->GetTalkNPCGlobalID());
	}
	return 0;
}

DWORD GuildMgr::OnEventGuildOpen( tagGameEvent* pGameEvent )
{
	if (RoleMgr::Inst()->GetLocalPlayer()->IsInGuild() == false)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildError_Code10357")]);
		m_pFrameMgr->SendEvent(&tagGameEvent(_T("Close_Guild_UI"), NULL));
		return 0;
	}
	SendSyncGuildInfo(tstring(_T("Guild")));
	return 0;
}

DWORD GuildMgr::OnEventGuildClose( tagGameEvent* pGameEvent )
{
	// ��ȡ��ǰ���ɽ���
	GuildFrame* pFrame = (GuildFrame*)m_pFrameMgr->GetFrame(_T("Guild"));
	if(P_VALID(pFrame))
	{
		m_pFrameMgr->SendEvent(&tagGameEvent(_T("HideGuildFrame"), NULL));
	}
	return 0;
}

VOID GuildMgr::ShowGuildErrorMsg( DWORD dwErrorCode )
{
	// ȡ��ֵ
	if(dwErrorCode > 0x0fffffff)
		dwErrorCode = 0 - dwErrorCode + 10000;//ȡ��ֵ�������ͻ����10000

	TCHAR szTmp[X_SHORT_NAME];
	_sntprintf(szTmp,sizeof(szTmp)/sizeof(TCHAR), _T("GuildError_Code%d"), dwErrorCode);
	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[szTmp]);
}

VOID GuildMgr::ClearGuildInfo()
{
	// ɾ������
	SAFE_DEL(m_pGuild);
	SAFE_DEL(m_pLpInfo);
	SAFE_DEL(m_pMembers);
	// ��հ�����ּ
	m_strTenet = _T("");
	// ɾ������
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Close_Faction"), NULL));
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Close_GuildAffair"), NULL));
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Close_GuildWare"), NULL));

	GuildFrame *pFrame = (GuildFrame*)m_pFrameMgr->GetFrame(_T("Guild"));
	if (P_VALID(pFrame))
		m_pFrameMgr->AddToDestroyList(pFrame);

	// ���ñ�����Ұ���״̬����
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp))
	{
		pLp->SetRoleGuildID(GT_INVALID);
		pLp->SetRoleGuildPos(EGMP_Null);
	}

    // ɾ���ֿ����
    m_bifNeedInitWarehouse  = true;
    m_dwWarehouseLastUpTime = 0;
}

VOID GuildMgr::SetLocalPlayerGuildPos( INT8 n8Pos )
{
	// ���ñ�����Ұ���ְλ
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp))
	{
		pLp->SetRoleGuildPos(n8Pos);
		m_pLpInfo->eGuildPos = (EGuildMemberPos)n8Pos;
	}
}

DWORD GuildMgr::OnEventGuildInvite( tagInviteJoinGuildEvent* pGameEvent )
{
	if(IsCanJionReq(pGameEvent->dwRoleID))
	{
		SendGuildJoinReq(pGameEvent->dwRoleID);
	}
	return 0;
}

DWORD GuildMgr::OnEventGetRoleName( tagRoleGetNameEvent* pGameEvent )
{
	if(pGameEvent->bResult)
	{
		if(pGameEvent->dwRoleID == m_dwInviterID)
		{
			ShowJoinReqMsgBox();
		}
		if (pGameEvent->dwRoleID == m_dwTempCompleteCommercePlayerID)
		{
			ShowGuildMsg(g_StrTable[_T("GuildCommerceComplete")], pGameEvent->strRoleName.c_str(), m_nTempFund);
			m_dwTempCompleteCommercePlayerID = GT_INVALID;
			m_nTempFund = 0;
		}
		if (pGameEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayer()->GetID() && !m_bGetTenet)
		{
			m_bGetTenet = true;
			// ��ȡ������ּ
			tagGameEvent evt(_T("Guild_GetTenet"), NULL);
			m_pFrameMgr->SendEvent(&evt);
		}
	}
	return 0;
}

VOID GuildMgr::SendSpreadGuildAffair(DWORD dwID)
{
	tagNC_SpreadGuildAffair cmd;
	cmd.dwBuffID = dwID;
	m_pSession->Send(&cmd);
}

VOID GuildMgr::SendGetGuildSkillInfo()
{
	tagNC_GetGuildSkillInfo cmd;
	m_pSession->Send(&cmd);
}

DWORD GuildMgr::OnNetGuildSpreadAffair(tagNS_SpreadGuildAffair *pNetCmd, DWORD)
{
	//���°�����Ϣ
	m_pGuild->byAffairRemainTimes = pNetCmd->byRemainTimes;
	
	const TCHAR *szName = PlayerNameTab::Inst()->FindNameByID(pNetCmd->dwRole);
	tstring strBuffName;
	const tagBuffProtoClient *pBuffClient = SkillProtoData::Inst()->FindBuffProto(pNetCmd->dwBuffID);
	if (P_VALID(pBuffClient))
	{
		strBuffName = pBuffClient->szName;
	}
	ShowGuildMsg(g_StrTable[_T("GuildBroad_Affair")], szName, strBuffName.c_str());
	
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_GuildAffair"), NULL));
	return 0;
}

VOID GuildMgr::SendSyncGuildInfo(const tstring &strFrameName)
{
	DWORD dwTime = timeGetTime() - m_dwLastSyncTime;
	// ����ͬ����Ϣ���͵��ٶ�
	if (dwTime > SYNC_TIME_LIMIT)
	{
		m_dwLastSyncTime = timeGetTime();
		// ����Ѿ���ͬ���б���,������
		std::deque<tstring>::iterator end = m_syncFrameNames.end();
		for (std::deque<tstring>::iterator iter = m_syncFrameNames.begin(); iter != end; ++iter)
		{
				if (*iter == strFrameName)
					return;
		}

		// ����ͬ���б�,�Ƚ��ȳ�
		m_syncFrameNames.push_back(strFrameName);
		tagNC_SyncGuildInfo cmd;
		m_pSession->Send(&cmd);
	}
}

DWORD GuildMgr::OnNetGuildSyncInfo(tagNS_SyncGuildInfo *pNetCmd, DWORD)
{
	if (P_VALID(m_pGuild))
	{
		ZeroMemory(m_pGuild, sizeof(tagGuildBase));
		memcpy((void*)m_pGuild, (const void*)(&pNetCmd->sGuildInfo), sizeof(tagGuildBase));
	}

	// �յ�ͬ����Ϣ������ʾ����
	if (!m_syncFrameNames.empty())
	{
		const tstring &strName = m_syncFrameNames.front();
		if (strName == _T("GuildAffair"))
		{
			GuildAffairFrame *pFrame = (GuildAffairFrame*)m_pFrameMgr->GetFrame(_T("GuildAffair"));
			if (!P_VALID(pFrame))
				m_pFrameMgr->CreateFrame(_T("World"), _T("GuildAffair"), _T("GuildAffairFrame"), 0);
			else
				m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_GuildAffair"), NULL));
				
		}
		else if (strName == _T("Guild"))
		{
			GuildFrame *pFrame = (GuildFrame*)m_pFrameMgr->GetFrame(_T("Guild"));
			if (!P_VALID(pFrame))
				m_pFrameMgr->CreateFrame(_T("World"), _T("Guild"), _T("GuildFrame"), 0);
			else
			{
				bool bShow = pFrame->IsShow();
				// ���û����ʾ����ʾ
				if (!bShow)	
				{
					m_pFrameMgr->SendEvent(&tagGameEvent(_T("ShowGuildFrame"), NULL));
					if (pFrame->GetFrameState() == GuildFrame::EGFS_MemberList)
					{
						SendGuildAllMembersGet();
					}
					else if (pFrame->GetFrameState() == GuildFrame::EGFS_GuildSkill)
					{
						SendGetGuildSkillInfo();
					}
				}
				// �����ʾ��ˢ��
				else
				{
					m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_Guild"), NULL));
					//��ˢ�³�Ա�б�
					//if (pFrame->GetFrameState() == GuildFrame::EGFS_MemberList)
					//{
					//	SendGuildAllMembersGet();
					//}
					if (pFrame->GetFrameState() == GuildFrame::EGFS_GuildSkill)
					{
						SendGetGuildSkillInfo();
					}
				}
			}
		}
		m_syncFrameNames.pop_front();
	}
	return 0;
}

DWORD GuildMgr::OnEventGuildAffairOpen(tagGameEvent *pEvent)
{
	if (RoleMgr::Inst()->GetLocalPlayer()->IsInGuild() == false)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildError_Code10357")]);
		return 0;
	}
	if (m_vecPower[m_pLpInfo->eGuildPos].bAffair == TRUE)
	{
		// ����ͬ����Ϣ
		SendSyncGuildInfo(tstring(_T("GuildAffair")));
	}
	else
	{
		ShowGuildErrorMsg(E_Guild_Power_NotEnough);
	}
	return 0;
}

DWORD GuildMgr::OnEventGuildAffairClose(tagGameEvent *pEvent)
{
	GuildAffairFrame *pFrame = (GuildAffairFrame*)m_pFrameMgr->GetFrame(_T("GuildAffair"));
	if (P_VALID(pFrame))
	{
		m_pFrameMgr->AddToDestroyList(pFrame);
	}
	return 0;
}

DWORD GuildMgr::OnEventPickUpSilverOpen(tagGameEvent *pEvent)
{
	if (RoleMgr::Inst()->GetLocalPlayer()->IsInGuild() == false)
		return 0;
	INT nLv = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
	const tagCommerceInfo *pInfo = GuildMgr::Inst()->FindCommerceInfo(nLv);
	if (!P_VALID(pInfo))
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildCommerceError_Code3")]);
		return 0;
	}
	const tagGuildPower *pPower = GuildMgr::Inst()->GetPowerList((INT)m_pLpInfo->eGuildPos);
	// �ж��Ƿ���Ȩ��
	if (!pPower->bCommerce)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildCommerceError_Code2")]);
		return 0;
	}
	// �ж��Ƿ��Ѿ���������״̬
	if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleState(ERS_Commerce) == true)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildCommerceError_Code1")]);
		return 0;
	}
	// ��ս״̬���ܽ���
	if (GuildMgr::Inst()->IsInSpecState(EGDSS_Warfare))
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildCommerceError_Code12")]);
		return 0;
	}
	GuildPickUpSilverFrame *pFrame = (GuildPickUpSilverFrame*)m_pFrameMgr->GetFrame(_T("GuildPickUpSilver"));
	if (!P_VALID(pFrame))
	{
		m_pFrameMgr->CreateFrame(_T("World"), _T("GuildPickUpSilver"), _T("GuildPickUpSilverFrame"), 0);
	}
	tagRefreshPickUpSilverEvent event(_T("Refresh_PickUpSilver"), NULL);
	event.pCommerceInfo = pInfo;
	m_pFrameMgr->SendEvent(&event);
	return 0;
}

DWORD GuildMgr::OnEventPickUpSilverClose(tagGameEvent *pEvent)
{
	GuildPickUpSilverFrame *pFrame = (GuildPickUpSilverFrame*)m_pFrameMgr->GetFrame(_T("GuildPickUpSilver"));
	if (P_VALID(pFrame))
	{
		m_pFrameMgr->AddToDestroyList(pFrame);
	}
	return 0;
}

DWORD GuildMgr::OnEventContributeSilverOpen(tagGameEvent *pEvent)
{
	if (RoleMgr::Inst()->GetLocalPlayer()->IsInGuild() == false)
		return 0;
	const tagGuildPower *pPower = GuildMgr::Inst()->GetPowerList((INT)m_pLpInfo->eGuildPos);
	if (!pPower->bCommerce)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildCommerceError_Code2")]);
		return 0;
	}
	if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleState(ERS_Commerce) == false)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildCommerceError_Code7")]);
		return 0;
	}
	//����������뱾�����̵���Ϣ
	SendGetTaelInfo();
	return 0;
}

DWORD GuildMgr::OnEventContributeSilverClose(tagGameEvent *pEvent)
{
	GuildContributeSilverFrame *pFrame = (GuildContributeSilverFrame*)m_pFrameMgr->GetFrame(_T("GuildContributeSilver"));
	if (P_VALID(pFrame))
	{
		m_pFrameMgr->AddToDestroyList(pFrame);
	}
	return 0;
}

DWORD GuildMgr::OnEventCOCOpen(tagGameEvent *pEvent)
{
	if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleState(ERS_Commerce) == false)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildCommerceError_Code8")]);
		return 0;
	}
	// ��������
	GuildBusinessFrame *pFrame = (GuildBusinessFrame*)m_pFrameMgr->GetFrame(_T("GuildBusiness"));
	if (!P_VALID(pFrame))
	{
		m_pFrameMgr->CreateFrame(_T("World"), _T("GuildBusiness"), _T("GuildBusinessFrame"), 0);
	}
	// ȡ�õ�ǰ�Ի�NPCID
	m_dwCurTalkCOCNPCID = QuestMgr::Inst()->GetTalkNPCTypeID();

	// ���������̻���Ϣ��Ϣ
	SendGetCofCInfo(m_dwCurTalkCOCNPCID);
	return 0;
}

DWORD GuildMgr::OnEventCOCClose(tagGameEvent *pEvent)
{
	GuildBusinessFrame *pFrame = (GuildBusinessFrame*)m_pFrameMgr->GetFrame(_T("GuildBusiness"));
	if (P_VALID(pFrame))
		m_pFrameMgr->AddToDestroyList(pFrame);

	SendCloseCOC(m_dwCurTalkCOCNPCID);
	return 0;
}

DWORD GuildMgr::OnEventGuildUpgrade(tagGameEvent *pEvent)
{
	LocalPlayer* pLP = RoleMgr::Inst()->GetLocalPlayer();
	if( NULL == pLP )
		return 0;
	
	GameFrame *pUpgradeFrame = m_pFrameMgr->GetFrame(_T("GuildUpgrade"));

	if( !pLP->IsInGuild() )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildError_Code10357")]);
		return 0;
	}
	
	// �ȼ�����
	if( pLP->GetRoleLevel() < MIN_GUILD_UPGRADE_ROLE_LEVEL )
	{
		ShowGuildErrorMsg(E_GuildUpgrade_Role_Level);
		if( P_VALID( pUpgradeFrame ) )
			m_pFrameMgr->AddToDestroyList( pUpgradeFrame );
		return 0;
	}
	
	// ״̬����
	if( IsInSpecState( EGDSS_Shortage ) ||
		IsInSpecState( EGDSS_Distress ) ||
		IsInSpecState( EGDSS_Chaos ) ||
		IsInSpecState( EGDSS_Warfare ) )
	{
		ShowGuildErrorMsg(E_Guild_State_Limit);
		if( P_VALID( pUpgradeFrame ) )
			m_pFrameMgr->AddToDestroyList( pUpgradeFrame );
		return 0;
	}

	// û��Ȩ��
	if( !m_vecPower[m_pLpInfo->eGuildPos].bUpgrade )
	{
		ShowGuildErrorMsg(E_Guild_Power_NotEnough);
		if( P_VALID( pUpgradeFrame ) )
			m_pFrameMgr->AddToDestroyList( pUpgradeFrame );
		return 0;
	}

	if ( !P_VALID(pUpgradeFrame) )
		m_pFrameMgr->CreateFrame(_T("World"), _T("GuildUpgrade"), _T("GuildUpgradeFrame"), 0);
	return 0;
}

DWORD GuildMgr::OnEventQuitCommerce(tagGameEvent *pEvent)
{
	if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleState(ERS_Commerce) == false)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildCommerceError_Code8")]);
		return 0;
	}
	if(P_VALID(GetObj("QuitCommerceWarning")))
		KillObj("QuitCommerceWarning");
	CreateObj("QuitCommerceWarning", "MsgBox");
	TObjRef<MsgBox>("QuitCommerceWarning")->Init(_T(""), g_StrTable[_T("GuildQuitCommerceWarning")], 
		_T("QuitCommerceWarning"));
	return 0;
}

DWORD GuildMgr::OnNetGetGuildSkillInfo(tagNS_GetGuildSkillInfo *pNetCmd, DWORD)
{
	m_vecSkill.clear();
	m_dwCurResearchSkillID = pNetCmd->dwCurSkillID;
	if (P_VALID(pNetCmd->dwCurSkillID))
	{	
		m_nCurResearchProgress = static_cast<INT32>(pNetCmd->n16Progress);
	}
	else
	{
		m_nCurResearchProgress = 0;
	}
	
	for (int i = 0; i < pNetCmd->nSkillNum; ++i)
	{
		m_vecSkill.push_back(pNetCmd->dwSkillInfo[i]);
	}
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_GuildSkills"), NULL));
	return 0;
}

VOID GuildMgr::ClearAffairs()
{
	std::vector<tagGuildAffairInfo*>::iterator end = m_vecAffairInfo.end();
	for (std::vector<tagGuildAffairInfo*>::iterator iter = m_vecAffairInfo.begin(); iter != end; ++iter)
	{
		SAFE_DELETE(*iter);
	}
	m_vecAffairInfo.clear();
}

VOID GuildMgr::ClearSkills()
{
	std::map<DWORD, tagGuildSkillInfo*>::iterator end = m_mapSkill.end();
	for (std::map<DWORD, tagGuildSkillInfo*>::iterator iter = m_mapSkill.begin(); iter != end; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	m_mapSkill.clear();
}


VOID GuildMgr::ClearCommerces()
{
	std::vector<tagCommerceInfo*>::iterator end = m_vecCommerceInfo.end();
	for (std::vector<tagCommerceInfo*>::iterator iter = m_vecCommerceInfo.begin(); iter != end; ++iter)
	{
		SAFE_DELETE(*iter);
	}
	m_vecCommerceInfo.clear();
}

VOID GuildMgr::ClearGoodInfo()
{
	std::map<DWORD, tagGoodInfo*>::iterator end = m_mapGoodInfo.end();
	for (std::map<DWORD,tagGoodInfo*>::iterator iter = m_mapGoodInfo.begin(); iter != end; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	m_mapGoodInfo.clear();
}

VOID GuildMgr::ClearCOCInfo()
{
	std::map<DWORD, tagCOCInfo*>::iterator end = m_mapCOCInfo.end();
	for (std::map<DWORD,tagCOCInfo*>::iterator iter = m_mapCOCInfo.begin(); iter != end; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	m_mapCOCInfo.clear();
}

const tagGuildSkillInfo* GuildMgr::FindSkillInfo(DWORD dwSkillID)
{
	std::map<DWORD, tagGuildSkillInfo*>::iterator end = m_mapSkill.end();
	std::map<DWORD, tagGuildSkillInfo*>::iterator iter = m_mapSkill.find(dwSkillID);
	if (iter != end)
		return iter->second;
	return static_cast<tagGuildSkillInfo*>(0);
}

const tagCommerceInfo * GuildMgr::FindCommerceInfo(INT nPlayerLevel)
{
	std::vector<tagCommerceInfo*>::iterator end = m_vecCommerceInfo.end();
	for (std::vector<tagCommerceInfo*>::iterator iter = m_vecCommerceInfo.begin(); iter != end; ++iter)
	{
		INT32 nLvUpperLimit = (*iter)->nLevelRange % 1000;
		INT32 nLvLowerLimit = (*iter)->nLevelRange / 1000;
		if ((nLvLowerLimit <= nPlayerLevel) && (nPlayerLevel <= nLvUpperLimit))
			return (*iter);
	}
	return static_cast<const tagCommerceInfo*>(0);
}

const tagGoodInfo* GuildMgr::FindGoodInfo(DWORD dwGoodID)
{
	std::map<DWORD, tagGoodInfo*>::iterator iter = m_mapGoodInfo.find(dwGoodID);
	if (iter != m_mapGoodInfo.end())
		return iter->second;
	return static_cast<const tagGoodInfo*>(0);
}

const tagCOCInfo* GuildMgr::FindCOCInfo(DWORD dwCOCID)
{
	std::map<DWORD, tagCOCInfo*>::iterator iter = m_mapCOCInfo.find(dwCOCID);
	if (iter != m_mapCOCInfo.end())
		return iter->second;
	return static_cast<const tagCOCInfo*>(0);
}


VOID GuildMgr::SendResearchSkill(DWORD dwID)
{
	tagNC_SetResearchSkill cmd;
	cmd.dwSkillID = dwID;
	m_pSession->Send(&cmd);
}

DWORD GuildMgr::OnNetSetResearchSkill(tagNS_SetResearchSkill *pNetCmd, DWORD)
{
	m_dwCurResearchSkillID = pNetCmd->dwSkillID;
	m_nCurResearchProgress = static_cast<INT32>(pNetCmd->n16Progress);
	
	tstring strSkillName = _T("");
	if (P_VALID(m_dwCurResearchSkillID))
	{
		std::map<DWORD, tagGuildSkillInfo*>::iterator iter= m_mapSkill.find(m_dwCurResearchSkillID);
		if (iter != m_mapSkill.end())
		{
			strSkillName = iter->second->strName;
		}
	}
	if (strSkillName != _T(""))
		ShowGuildMsg(g_StrTable[_T("GuildBroad_SetCurrentResearchSkill")], strSkillName.c_str());
	else
		ShowGuildMsg(g_StrTable[_T("GuildBroad_SetCurrentResearchSkill")], g_StrTable[_T("GuildText_Empty")]);
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_GuildSkills"), NULL));
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_Guild"), NULL));
	return 0;
}

VOID GuildMgr::SendLearnGuildSkill(DWORD dwID)
{
	tagNC_LearnGuildSkill cmd;
	cmd.dwSkillID = dwID;
	m_pSession->Send(&cmd);
}

DWORD GuildMgr::OnNetLearnGuildSkill(tagNS_LearnGuildSkill *pNetCmd, DWORD)
{
	switch (pNetCmd->dwErrorCode)
	{
	case E_Success:
		{
			// ˢ�½���
			m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_GuildSkills"), NULL));
			m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_Guild"), NULL));
			const tagGuildSkillInfo *pInfo = FindSkillInfo(pNetCmd->dwSkillID);
			if (!pInfo)
				return 0;
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_LearnSkill_Success")], pInfo->strName.c_str());
		} break;
	case E_LearnSkill_Existed:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildSkillError_Code01")]);
		} break;
	case E_LearnSkill_ProtoNotFound:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildSkillError_Code02")]);
		} break;
	case E_LearnSkill_NeedMoreLevel:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildSkillError_Code03")]);
		} break;
	case E_LearnSkill_NoPreSkill:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildSkillError_Code04")]);
		} break;
	case E_LevelUpSkill_NotExist:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildSkillError_Code05")]);
		} break;
	case E_LevelUpSkill_ExceedMaxLevel:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildSkillError_Code06")]);
		} break;
	default: break;
	}
	return 0;
}

void GuildMgr::Update()
{
	
	DWORD dwTime = timeGetTime() - m_dwLastUpdateTime;
	if (dwTime > UPDATE_TIME_LIMIT)
	{
		// ÿ5��ͬ��һ�ν���
		GuildAffairFrame *pGuildAffairFrame = (GuildAffairFrame*)m_pFrameMgr->GetFrame(_T("GuildAffair"));
		// �����������������
		if (P_VALID(pGuildAffairFrame))
		{
			tagNC_SyncGuildInfo cmd;
			m_pSession->Send(&cmd);
			// ����ͬ���б�,�Ƚ��ȳ�
			m_syncFrameNames.push_back(_T("GuildAffair"));
		}
		GuildFrame *pGuildFrame = (GuildFrame*)m_pFrameMgr->GetFrame(_T("Guild"));
		// ������ɽ������
		if (P_VALID(pGuildFrame))
		{
			bool bShow = pGuildFrame->IsShow();
			if (bShow)
			{
				tagNC_SyncGuildInfo cmd;
				m_pSession->Send(&cmd);
				// ����ͬ���б�,�Ƚ��ȳ�
				m_syncFrameNames.push_back(_T("Guild"));
			}
		}
		m_dwLastUpdateTime = timeGetTime();
	}
}

DWORD GuildMgr::OnNetGuildContribution(tagNS_GuildContribution *pNetCmd, DWORD)
{
	if(!P_VALID(m_pLpInfo))
		return 0;
	m_pLpInfo->nContribution = pNetCmd->nContribution;
	m_pLpInfo->nTotalContribution = pNetCmd->nTotalContribution;
	return 0;
}

DWORD GuildMgr::OnNetGuildStateSet(tagNS_GuildStateSet *pNetCmd, DWORD)
{
	m_pGuild->dwSpecState |= static_cast<DWORD>(pNetCmd->eState);
	return 0;
}

DWORD GuildMgr::OnNetGuildStateUnSet(tagNS_GuildStateUnset *pNetCmd, DWORD)
{
	m_pGuild->dwSpecState &= (~(static_cast<DWORD>(pNetCmd->eState)));
	return 0;
}

bool GuildMgr::IsInSpecState(EGuildSpecState eState) const
{
	if (!P_VALID(m_pGuild))
		return false;
	if (m_pGuild->dwSpecState == 0)
	{
		if (static_cast<DWORD>(eState) == 0)
			return true;
		else
			return false;
	}
	else
	{
		if (m_pGuild->dwSpecState & static_cast<DWORD>(eState))
			return true;
		return false;
	}
}

VOID GuildMgr::SendGuildSkillUpgrade(INT64 n64Serial)
{
	tagNC_GuildSkillUpgrade Cmd;
	Cmd.n64ItemSerial = n64Serial;
	m_pSession->Send(&Cmd);
}

DWORD GuildMgr::OnNetGuildSkillUpgrade(tagNS_GuildSkillUpgrade *pNetCmd, DWORD)
{
	m_dwCurResearchSkillID = pNetCmd->dwSkillID;
	m_nCurResearchProgress = static_cast<INT32>(pNetCmd->n16Progress);
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_GuildSkills"), NULL));
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_Book"), NULL));
	return 0;
}

DWORD GuildMgr::OnNetGuildSkillLevelUp(tagNS_GuildSkillLevelUp *pNetCmd, DWORD)
{
	m_dwCurResearchSkillID = pNetCmd->dwSkillID;

	tstring strSkillName = _T("");
	if (P_VALID(m_dwCurResearchSkillID))
	{
		std::map<DWORD, tagGuildSkillInfo*>::iterator iter= m_mapSkill.find(m_dwCurResearchSkillID);
		if (iter != m_mapSkill.end())
		{
			strSkillName = iter->second->strName;
		}
	}
	ShowGuildMsg(g_StrTable[_T("GuildBroad_SkillUpgrate")], strSkillName.c_str());
	return 0;
}

VOID GuildMgr::SendAcceptCommerce()
{
	tagNC_AcceptCommerce cmd;
	m_pSession->Send(&cmd);
}

DWORD GuildMgr::OnNetAcceptCommerce(tagNS_AcceptCommerce *pNetCmd, DWORD)
{
	m_nOwnCommerceSilver = pNetCmd->nBeginningTael;
	GuildBusinessStateWnd *pFrame = (GuildBusinessStateWnd*)m_pFrameMgr->GetFrame(_T("GuildBusinessState"));
	if (!P_VALID(pFrame))
	{
		m_pFrameMgr->CreateFrame(_T("World"), _T("GuildBusinessState"), _T("GuildBusinessStateWnd"), 0);
	}
	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildCommerceAccept")]);
	return 0;
}

VOID GuildMgr::SendGetTaelInfo()
{
	tagNC_GetTaelInfo cmd;
	m_pSession->Send(&cmd);
}

DWORD GuildMgr::OnNetGetTaelInfo(tagNS_GetTaelInfo *pNetCmd, DWORD)
{
	INT nLevel = pNetCmd->nLevel;
	const tagCommerceInfo *pInfo = FindCommerceInfo(nLevel);
	if (P_VALID(pInfo))
	{
		// ��ȡ��Ϣ�󴴽�����
		GuildContributeSilverFrame *pFrame = (GuildContributeSilverFrame*)m_pFrameMgr->GetFrame(_T("GuildContributeSilver"));
		if (!P_VALID(pFrame))
		{
			m_pFrameMgr->CreateFrame(_T("World"), _T("GuildContributeSilver"), _T("GuildContributeSilverFrame"), 0);
		}
		tagRefreshContributeSilverEvent event(_T("Refresh_ContributeSilver"), NULL);
		event.nLevel = nLevel;
		event.pCommerceInfo = pInfo;
		m_pFrameMgr->SendEvent(&event);
	}
	return 0;
}

VOID GuildMgr::SendCompleteCommerce()
{
	tagNC_CompleteCommerce cmd;
	m_pSession->Send(&cmd);
}

DWORD GuildMgr::OnNetCompleteCommerce(tagNS_CompleteCommerce *pNetCmd, DWORD)
{
	m_dwTempCompleteCommercePlayerID = pNetCmd->dwRoleID;
	m_nTempFund = pNetCmd->nFund;
	m_nOwnCommerceSilver = 0;
	m_mapPlayerGoods.clear();
	GuildBusinessStateWnd *pFrame = (GuildBusinessStateWnd*)m_pFrameMgr->GetFrame(_T("GuildBusinessState"));
	if (P_VALID(pFrame))
	{
		m_pFrameMgr->AddToDestroyList(pFrame);
	}

	LPCTSTR szName = PlayerNameTab::Inst()->FindNameByID(m_dwTempCompleteCommercePlayerID);
	if(_tcslen(szName) > 0)
	{
		ShowGuildMsg(g_StrTable[_T("GuildCommerceComplete")], szName, m_nTempFund);
		m_dwTempCompleteCommercePlayerID = GT_INVALID;
		m_nTempFund = 0;
	}
	return 0;
}

VOID GuildMgr::SendQuitCommerce()
{
	tagNC_AbandonCommerce cmd;
	m_pSession->Send(&cmd);
}

DWORD GuildMgr::OnNetAbandonCommerce(tagNS_AbandonCommerce *pNetCmd, DWORD)
{
	GuildBusinessStateWnd *pFrame = (GuildBusinessStateWnd*)m_pFrameMgr->GetFrame(_T("GuildBusinessState"));
	if (P_VALID(pFrame))
	{
		m_pFrameMgr->AddToDestroyList(pFrame);
	}
	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildCommerceQuit")]);
	// ��ձ��ر����
	m_mapPlayerGoods.clear();
	return 0;
}

DWORD GuildMgr::OnNetGuildUpgrade(tagNS_GuildUpgrade *pNetCmd, DWORD)
{
	tstringstream stream;
	stream<<_T("Guild_Facilitie")<<pNetCmd->eType;
	TCHAR szBuff[100] = { 0 };
	_sntprintf( szBuff, 100, g_StrTable[_T("GuildUpgrade_LevelUp")], g_StrTable[stream.str().c_str()], pNetCmd->byNewLevel );

	// ������ʩ��������ʾ��Ϣǰ����ϡ����ɡ�
	if( EFT_Guild != pNetCmd->eType )
	{
		tstring strMsg;
		tstringstream streamGuild;
		streamGuild<<_T("Guild_Facilitie")<<EFT_Guild;
		strMsg = g_StrTable[streamGuild.str().c_str()];
		strMsg += szBuff;
		CombatSysUtil::Inst()->ShowScreenCenterMsg( strMsg.c_str() );
	}
	else
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg( szBuff );
	}
	return 0;
}

VOID GuildMgr::SendGetCofCInfo(DWORD dwNPCID)
{
	// ���ͻ�ȡ�̻���Ϣ������Ϣ
	tagNC_GetCofCInfo cmd;
	cmd.dwNPCID = dwNPCID;
	m_pSession->Send(&cmd);
}

DWORD GuildMgr::OnNetGetCommodityInfo(tagNS_GetCommodityInfo *pNetCmd, DWORD)
{ 
	m_nOwnCommerceSilver = pNetCmd->nCurTael;
	m_nRunForBusinessLevel = pNetCmd->nLevel;
	for (INT n = 0; n < pNetCmd->nGoodNum; ++n)
	{	
		DWORD dwID = pNetCmd->sGoodInfo[n].dwGoodID;
		std::map<DWORD, tagCommerceGoodInfo>::iterator iter = m_mapPlayerGoods.find(dwID);
		if (iter != m_mapPlayerGoods.end())
		{
			iter->second.byGoodNum = pNetCmd->sGoodInfo[n].byGoodNum;
			iter->second.nCost = pNetCmd->sGoodInfo[n].nCost;
			if (iter->second.byGoodNum == 0)
				m_mapPlayerGoods.erase(iter);
		}
		else
		{
			m_mapPlayerGoods.insert(std::make_pair(pNetCmd->sGoodInfo[n].dwGoodID, pNetCmd->sGoodInfo[n]));
		}
	}
	tagGameEvent event(_T("Refresh_Commodity"), NULL);
	m_pFrameMgr->SendEvent(&event);
	return 0;
}

DWORD GuildMgr::OnNetGetCofCInfo(tagNS_GetCofCInfo *pNetCmd, DWORD)
{
	// �����ǰ�̻�ID������������Ϣ���̻�ID�������ʱ������̻��̻���Ϣ
	if (m_dwCurCOCID != pNetCmd->dwCofCID)
	{
		m_mapCOCGoods.clear();
	}
	m_dwCurCOCID = pNetCmd->dwCofCID;
	m_byCurHoldCity = pNetCmd->byHoldCity;
	for (INT n = 0; n < pNetCmd->nGoodNum; ++n)
	{
		DWORD dwID = pNetCmd->sGoodInfo[n].dwGoodID;
		std::map<DWORD, tagCommerceGoodInfo>::iterator iter = m_mapCOCGoods.find(dwID);
		if (iter != m_mapCOCGoods.end())
		{
			iter->second.byGoodNum = pNetCmd->sGoodInfo[n].byGoodNum;
			iter->second.nCost = pNetCmd->sGoodInfo[n].nCost;
		}
		else
		{
			m_mapCOCGoods.insert(std::make_pair(pNetCmd->sGoodInfo[n].dwGoodID, pNetCmd->sGoodInfo[n]));
		}
	}

	tagGameEvent event(_T("Refresh_COC"), NULL);
	m_pFrameMgr->SendEvent(&event);

	// ������ɼ�����Ϊ�ɼ�
	GuildBusinessFrame *pFrame = (GuildBusinessFrame*)m_pFrameMgr->GetFrame(_T("GuildBusiness"));
	if (P_VALID(pFrame))
	{
		if (!pFrame->IsShow())
			pFrame->Show(true);
	}
	return 0;
}

VOID GuildMgr::SendCloseCOC(DWORD dwNPCID)
{
	tagNC_CloseCofC cmd;
	cmd.dwNPCID = dwNPCID;
	m_pSession->Send(&cmd);
}

VOID GuildMgr::SendBuyCOCGoods(DWORD dwNPCID, DWORD dwGoodID, BYTE byBuyNum)
{
	tagNC_BuyCofCGoods cmd;
	cmd.dwNPCID = dwNPCID;
	cmd.dwGoodID = dwGoodID;
	cmd.byBuyNum = byBuyNum;
	m_pSession->Send(&cmd);
}

VOID GuildMgr::SendSellCOCGoods(DWORD dwNPCID, DWORD dwGoodID, BYTE bySellNum)
{
	tagNC_SellCofCGoods cmd;
	cmd.dwNPCID = dwNPCID;
	cmd.dwGoodID = dwGoodID;
	cmd.bySellNum = bySellNum;
	m_pSession->Send(&cmd);
}

DWORD GuildMgr::OnEventOpenGuildStorage( tagGameEvent *pEvent )
{
    GuildStorageFrame *pFrame = (GuildStorageFrame*)m_pFrameMgr->GetFrame(_T("GuildStorage"));
    if (!P_VALID(pFrame))
    {
        pFrame = (GuildStorageFrame*)m_pFrameMgr->CreateFrame(_T("World"), _T("GuildStorage"), _T("GuildStorageFrame"), 0);
    }

    DWORD npcId = QuestMgr::Inst()->GetTalkNPCGlobalID();
    pFrame->SetNPCID(npcId);
    
    // �Ƿ�������
    LocalPlayer* lpl = RoleMgr::Inst()->GetLocalPlayer();
    if (!lpl->IsAttributeInited())
        return 0;

    if (lpl->IsInGuild())
    {
        BeginNC_GetGuildWare(npcId);
    }
    else
    {
        CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildPrivacyErr")]);
    }
	return 0;
}

DWORD GuildMgr::OnEventOpenGuildStoragePermission( tagGameEvent *pEvent )
{
    GuildStorePermitFrame *pFrame = (GuildStorePermitFrame*)m_pFrameMgr->GetFrame(_T("GuildStorePermit"));
    if (!P_VALID(pFrame))
    {
        pFrame = (GuildStorePermitFrame*)m_pFrameMgr->CreateFrame(_T("World"), _T("GuildStorePermit"), _T("GuildStorePermitFrame"), 0);
    }

    pFrame->SetNpcId(QuestMgr::Inst()->GetTalkNPCGlobalID());

    // �Ƿ�������
    LocalPlayer* lpl = RoleMgr::Inst()->GetLocalPlayer();
    if (!lpl->IsAttributeInited() || !m_bIsInitFinish)
        return 0;

    if (lpl->IsInGuild())
    {
        // ֻ�а����򸱰����в���Ȩ��
        EGuildMemberPos pos = (EGuildMemberPos)lpl->GetRoleGuildPos();
        if (m_vecPower[pos].bSetWareRights)
            pFrame->ShowWnd(TRUE);
        else
            CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildPrivacySetErr")]);
    }
    else
    {
        CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildPrivacyErr")]);
    }
	return 0;
}

DWORD GuildMgr::OnEventGuildCommerceRankOpen(tagGameEvent *pEvent)
{
	// ������ڰ���
	if (RoleMgr::Inst()->GetLocalPlayer()->IsInGuild() == false)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildCommerceError_Code11")]);
		return 0;
	}
	GuildBusinessRankFrame *pFrame = (GuildBusinessRankFrame*)m_pFrameMgr->GetFrame(_T("GuildBusinessRank"));
	if (!P_VALID(pFrame))
	{
		m_pFrameMgr->CreateFrame(_T("World"), _T("GuildBusinessRank"), _T("GuildBusinessRankFrame"), 0);
	}
	// ����������Ϣ
	SendGetCommerceRank();
	return 0;
}

DWORD GuildMgr::OnEventGuildCommerceRankClose(tagGameEvent *pEvent)
{
	GuildBusinessRankFrame *pFrame = (GuildBusinessRankFrame*)m_pFrameMgr->GetFrame(_T("GuildBusinessRank"));
	if (P_VALID(pFrame))
		m_pFrameMgr->AddToDestroyList(pFrame);
	return 0;
}

DWORD GuildMgr::OnEventGuildGetTenet( tagGameEvent *pEvent )
{
	//// ��ȡ������ּ
	tagNC_GetGuildTenet cmd;
	m_pSession->Send(&cmd);
	return 0;
}

DWORD GuildMgr::OnNetGetCommerceRank(tagNS_GetCommerceRank *pNetCmd, DWORD)
{
	tagRefreshGuildBusinessRankEvent event(_T("Refresh_GuildBusinessRank"), NULL);
	event.bCommend = pNetCmd->bCommend;

	for (int n = 0; n < pNetCmd->nRankNum; ++n)
	{
		event.vecRankInfo.push_back(pNetCmd->sRankInfo[n]);
	}
	
	m_pFrameMgr->SendEvent(&event);
	return 0;
}

VOID GuildMgr::SendSwitchCommendation(BOOL b)
{
	tagNC_SwitchCommendation cmd;
	cmd.bSwitchON = b;
	m_pSession->Send(&cmd);
}

DWORD GuildMgr::OnNetSwitchCommendation(tagNS_SwitchCommendation *pNetCmd, DWORD)
{
	tagRefreshBusinessRankCommendStateEvent event(_T("Refresh_GuildBusinessRankCommendState"), NULL);
	if (pNetCmd->bSwitchON)
		ShowGuildMsg(g_StrTable[_T("GuildBroadcast_CommendOpen")]);
	else
		ShowGuildMsg(g_StrTable[_T("GuildBroadcast_CommendClose")]);
	event.bCommend = pNetCmd->bSwitchON;
	m_pFrameMgr->SendEvent(&event);
	return 0;
}

VOID GuildMgr::SendGetCommerceRank()
{
	tagNC_GetCommerceRank cmd;
	m_pSession->Send(&cmd);
}

void GuildMgr::BeginNC_GetGuildWare( DWORD dwNPCID )
{
    DWORD lastTime = m_bifNeedInitWarehouse ? 0 : m_dwWarehouseLastUpTime;

    tagNC_GetGuildWare e;
    e.dwLastUpdateTime      = lastTime;
    e.dwNPCID               =   QuestMgr::Inst()->GetTalkNPCGlobalID();

    m_pSession->Send(&e);
}

DWORD GuildMgr::OnNS_GetGuildWare( tagNS_GetGuildWare* pMsg, DWORD )
{
    if (pMsg->dwErrorCode == E_Success)
    {
        if (m_bifNeedInitWarehouse)
        {
            ItemMgr::Inst()->ClearGuildWare();
            m_bifNeedInitWarehouse = false;
        }
        ItemMgr::Inst()->OnGetGuildWare(pMsg);
        m_dwWarehouseLastUpTime = pMsg->dwLastUpdateTime;
    }
    else
    {
        // ������Ϸ�¼�
        tagGameEvent e(_T("InitGuildWareEvent"), NULL);
        m_pFrameMgr->SendEvent(&e);
    }
	return 0;
}

void GuildMgr::BeginNC_GuildWarePri( DWORD npcID, DWORD roleID, BOOL canUse )
{
    tagNC_GuildWarePri e;
    e.dwNPCID   =   npcID;
    e.dwRoleID  =   roleID;
    e.bCanUse   =   canUse;

    m_pSession->Send(&e);
}

DWORD GuildMgr::OnNS_GuildWarePri( tagNS_GuildWarePri* pMsg, DWORD )
{
    if (pMsg->dwRoleID == m_pLpInfo->dwRoleID)
    {
        // ���ڱ������
        m_pLpInfo->bUseGuildWare    =   pMsg->bCanUse;
    }

    // ������Ϸ�¼�
    tagGuildWarePriChange e(_T("tagGuildWarePriChange"), NULL);
    e.dwRoleID      =   pMsg->dwRoleID;
    e.bCanUse       =   pMsg->bCanUse;

    m_pFrameMgr->SendEvent(&e);
	return 0;
}

bool GuildMgr::IsBeOwned(BYTE byCity)
{
	if (!P_VALID(m_pGuild))
		return false;
	
	if (byCity == 0)
		return true;

	for (int n = 0; n < MAX_GUILD_HOLDCITY; ++n)
	{
		if (byCity == m_pGuild->byHoldCity[n])
			return true;
	}
	return false;
}

void GuildMgr::ShowGuildMsg(const TCHAR* szFormat,...)
{
	va_list args;
	va_start(args, szFormat);

	TCHAR szBuffer[512];
	_vstprintf_s(szBuffer, 512, szFormat, args);

	va_end(args);
	
	ChatFrame *pChatFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
	if (P_VALID(pChatFrame))
		pChatFrame->PushInfo(szBuffer,ESCC_Guild);
}

bool GuildMgr::isWareOpretable()
{
    bool bRet = false;

    // ֻ����ʹ��Ȩ��
    LocalPlayer* lpl = RoleMgr::Inst()->GetLocalPlayer();
    if (!lpl->IsAttributeInited() || !m_bIsInitFinish)
        return false;

    EGuildMemberPos pos = (EGuildMemberPos)lpl->GetRoleGuildPos();
    bRet = m_vecPower[pos].bSetWareRights || m_pLpInfo->bUseGuildWare;

    return bRet;
}

DWORD GuildMgr::OnNetGuildExploit( tagNS_GuildExploit *pNetCmd, DWORD )
{
	tagGuildMemberClient* pMember = m_pMembers->FindMember(pNetCmd->dwRoleID);
	if (P_VALID(pMember))
	{
		pMember->sInfoEx.nExploit = pNetCmd->nExploit;
	}
	return 0;
}

DWORD GuildMgr::OnNetGetSomeGuildName(tagNS_GetSomeGuildName *pNetCmd, DWORD)
{
	for (int i = 0; i < pNetCmd->nNum; ++i)
	{
		m_mapGuildName[pNetCmd->IdName[i].dwID] = pNetCmd->IdName[i].szName;
	}

	// ������Ϣ֪ͨ
	tagGetSomeGuildNameEvent event(_T("GetSomeGuildName"), NULL);
	event.nFlag = pNetCmd->nUserData;
	m_pFrameMgr->SendEvent(&event);
	return 0;
}
