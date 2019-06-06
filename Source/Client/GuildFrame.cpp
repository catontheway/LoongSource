#include "StdAfx.h"
#include "GuildFrame.h"
#include "GuildMgr.h"
#include "GuildMembers.h"
#include "..\WorldDefine\msg_guild.h"
#include "..\WorldDefine\currency_define.h"
#include "PlayerNameTab.h"
#include "GuildEvent.h"
#include "FilterData.h"
#include "CombatSysUtil.h"
#include "GuildMemberPosFrame.h"
#include "RoleMgr.h"
#include "ServerTime.h"
#include <new>
#include "LocalPlayer.h"
#include "IconStatic.h"
#include "SkillMgr.h"
#include "GuildDefine.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "CurrencyMgr.h"
#include "ProfessionMgr.h"
#include "SkillProtoData.h"
#include "Item.h"
#include "ItemMgr.h"



GuildFrame::GuildFrame(void):m_Trunk(this)
{
	m_eSortType		 = GuildMembers::ELCT_Level;
	m_pWnd			 = NULL;
	m_pWndMembers	 = NULL;
	m_pBtnClose		 = NULL;
	m_pLstMembers	 = NULL;
	m_pStcName		 = NULL;	
	m_pStcLevel		 = NULL;
	m_pStcSymbol	 = NULL;
	m_pStcFounder	 = NULL;
	m_pStcLeader	 = NULL;
	m_pStcMemberNum	 = NULL;
	m_pStcReputation = NULL;

	m_pStcFund		 = NULL;		
	m_pStcMaterial	 = NULL;	
	m_pStcPeace		 = NULL;	
	m_pStcDailyCost	 = NULL;
	m_pEdtTenet		 = NULL;	
	m_pPbnMembers	 = NULL;	

	m_pBtnLookupInfo	= NULL;
	m_pBtnLeaveGuild	= NULL;
	m_pBtnTurnLeader	= NULL;
	m_pBtnAppointMember = NULL;
	m_pBtnModifyTenet	= NULL;	
	m_pBtnKickMember	= NULL;	
	m_pBtnDemissPostion = NULL;
	m_pPbnOffline		= NULL;	

	m_pGuild			= NULL;
	m_dwDstRoleID		= GT_INVALID;

	m_pBtnListIndex.clear();
	m_pWndGuild = NULL;
	m_pBtnCurSkillIcon = NULL;
	m_pStcCurSkillName = NULL;
	m_pStcCurSkillLevel = NULL;
	m_pStcFundCost = NULL;
	m_pStcMaterialCost = NULL;
	m_pBtnBookIcon = NULL;
	m_pProgressIncrement = NULL;
	m_pLearnBook = NULL;
	m_pBtnBack = NULL;
	m_pBtnNext = NULL;
	m_pStcLearnGoldCost = NULL;
	m_pStcLearnSilverCost = NULL;
	m_pStcLearnContributionNeed = NULL;
	m_pStcLearnGuildFundCost = NULL;
	m_pStcLearnGuildMaterialCost = NULL;
	m_pBtnLearnSkill = NULL;
	m_pBtnCancelResearch = NULL;
	m_pBtnStartResearch = NULL;
	m_pSkillPatch = NULL;

	for (int i = 0; i < GUILD_SKILL_ITEM_NUM_PERPAGE; ++i)
	{
		m_pSkillItemBack[i] = NULL;
		m_pSkillItem[i] = NULL;
		m_pSkillName[i] = NULL;
		m_pSkillLevel[i] = NULL;
	}

	m_nCurPageNum = 0;
	m_dwCurChooseSkill = GT_INVALID;
	m_eGuildFrameState = EGFS_MemberList;
	m_bShow = false;
	m_pSkillBook = NULL;
	

	m_dwTempRoleID = GT_INVALID;
	m_nTempPOS = 0;

}

GuildFrame::~GuildFrame(void)
{
}

BOOL GuildFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);
	
	// ע����Ϸ�¼�������
	m_pMgr->RegisterEventHandle(_T("GuildEvent_NameGet"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetGuildName));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_TenetGet"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetGuildTenet));
	m_pMgr->RegisterEventHandle(_T("tagRoleGetNameEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetRoleName));
	m_pMgr->RegisterEventHandle(_T("GetNameByNameID"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetNameByNameID));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_ReflashReq"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReq));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_PosAppoint"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildPosAppoint));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_ListReflash"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildListReflash));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_InfoReflash"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReflash));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_PosChange"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReflash));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_RemoveMember"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReflash));
	m_pMgr->RegisterEventHandle(_T("MsgBox_ModifyTenet"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxInputTenet));
	m_pMgr->RegisterEventHandle(_T("MsgBox_LeaveCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxLeaveCheck));
	m_pMgr->RegisterEventHandle(_T("MsgBox_TurnoverCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxTurnoverCheck));
	m_pMgr->RegisterEventHandle(_T("MsgBox_KickCheck"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxKickCheck));
	m_pMgr->RegisterEventHandle(_T("MsgBox_DemissCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxDemissCheck));
	m_pMgr->RegisterEventHandle(_T("Refresh_GuildSkills"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventRefreshGuildSkills));
	m_pMgr->RegisterEventHandle(_T("ShowGuildFrame"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventShowGuildFrame));
	m_pMgr->RegisterEventHandle(_T("HideGuildFrame"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventHideGuildFrame));
	m_pMgr->RegisterEventHandle(_T("Refresh_Guild"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventRefreshGuild));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_UseGuildSkillBook"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventUsGuildSkillBook));
	m_pMgr->RegisterEventHandle(_T("Refresh_Book"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventRefreshBook));
	m_pMgr->RegisterEventHandle(_T("OnEventCommerceWarningMsgBox"), (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventCommerceWarningMsgBox));

	SetSkillDefaultUI();
	m_pPbnMembers->SetState(EGUIBS_PushDown, TRUE);
	GuildMgr::Inst()->SendGuildAllMembersGet();
	GuildMgr::Inst()->SendGetGuildSkillInfo();
	m_pSkillPatch->SetInvisible(TRUE);
	return bRet;
}

BOOL GuildFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	// ע����Ϸ�¼�������
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_NameGet"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetGuildName));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_TenetGet"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetGuildTenet));
	m_pMgr->UnRegisterEventHandler(_T("tagRoleGetNameEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetRoleName));
	m_pMgr->UnRegisterEventHandler(_T("GetNameByNameID"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetNameByNameID));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_ReflashReq"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReq));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_PosAppoint"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildPosAppoint));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_ListReflash"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildListReflash));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_InfoReflash"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReflash));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_PosChange"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReflash));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_RemoveMember"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReflash));
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_ModifyTenet"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxInputTenet));
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_LeaveCheck"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxLeaveCheck));
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_TurnoverCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxTurnoverCheck));
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_KickCheck"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxKickCheck));
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_DemissCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxDemissCheck));
	m_pMgr->UnRegisterEventHandler(_T("Refresh_GuildSkills"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventRefreshGuildSkills));
	m_pMgr->UnRegisterEventHandler(_T("ShowGuildFrame"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventShowGuildFrame));
	m_pMgr->UnRegisterEventHandler(_T("HideGuildFrame"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventHideGuildFrame));
	m_pMgr->UnRegisterEventHandler(_T("Refresh_Guild"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventRefreshGuild));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_UseGuildSkillBook"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventUsGuildSkillBook));
	m_pMgr->UnRegisterEventHandler(_T("Refresh_Book"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventRefreshBook));
	m_pMgr->UnRegisterEventHandler(_T("OnEventCommerceWarningMsgBox"), (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventCommerceWarningMsgBox));
	if(P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
	}

	DestroyMsgBox();
	
	return bRet;
}

BOOL GuildFrame::ReloadUI()
{
	BOOL bRet = GameFrame::ReloadUI();
	
	// ����
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\party.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	
	for (int i = 1; i <= 5; ++i)
	{
		char buffer[512];
		_snprintf(buffer, 512, "party_win\\party_back\\message_back\\basicmessage_back2\\static%d", i);
		m_pGUI->ChangeXml(&ele, buffer, "ClassName", "IconStatic");
	}

	for (int i = 0; i < 12; ++i)
	{
		std::string strItemPath = "party_win\\party_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillback%d\\skill%d";
		char buffer[512];
		_snprintf(buffer, 512, strItemPath.c_str(), i, i, i);
		m_pGUI->ChangeXml(&ele, buffer, "ClassName", "IconStatic");
	}

	m_pGUI->ChangeXml(&ele, "party_win\\party_back\\guildskill_back\\doingskillback_pic\\skillback\\skill", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "party_win\\party_back\\guildskill_back\\doingskillback_pic1\\skillback\\skill", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "party_win\\party_back\\message_back\\basicmessage_back\\partylogo_back\\party_logo", "ClassName", "IconStatic");
	
	
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	//m_pWnd = m_pGUI->CreateFromFile("VFS_System", strPath.c_str());

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)m_Trunk.sfp1(&GuildFrame::EventHandler));
	
	m_pBtnClose  = (GUIButton*)m_pWnd->GetChild(_T("party_back\\closebutt"));
	m_pStcName	 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back\\party_name"));
	m_pStcLevel	 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back\\party_grade"));
	m_pStcSymbol = (IconStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back\\partylogo_back\\party_logo"));
	
	m_pStcFounder	 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back2\\name"));
	m_pStcLeader	 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back2\\leader"));
	m_pStcMemberNum	 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back2\\num1"));
	m_pStcReputation = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back2\\num2"));

	m_pStcFund		 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back3\\num1"));		
	m_pStcMaterial	 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back3\\num2"));	
	m_pStcPeace		 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back3\\num3"));	
	m_pStcDailyCost	 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back3\\num4"));

	m_pEdtTenet		 = (GUIEditBoxEx*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back4\\purpose"));
	
	m_pPbnMembers = (GUIPushButton*)m_pWnd->GetChild(_T("party_back\\fra\\choose1"));
	m_pPbnSkills = (GUIPushButton*)m_pWnd->GetChild(_T("party_back\\fra\\guildskill"));
	m_pPbnInfomation = (GUIPushButton*)m_pWnd->GetChild(_T("party_back\\fra\\guildinformation"));
	m_pWndMembers = m_pWnd->GetChild(_T("party_back\\partymember_back"));
	m_pLstMembers = (GUIListBox*)m_pWndMembers->GetChild(_T("memberlist_back\\member_list"));
	for(INT i=0; i<GuildMembers::ELCT_End; ++i)
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("memberlist_back\\butt%d"), i+1);
		GUIButton* pBtn = (GUIButton*)m_pWndMembers->GetChild(szTmp);
		m_pBtnListIndex.push_back(pBtn);
	}
	m_pBtnLookupInfo	= (GUIButton*)m_pWndMembers->GetChild(_T("partybutt1"));
	m_pBtnLeaveGuild	= (GUIButton*)m_pWndMembers->GetChild(_T("partybutt2"));
	m_pBtnTurnLeader	= (GUIButton*)m_pWndMembers->GetChild(_T("partybutt3"));
	m_pBtnAppointMember = (GUIButton*)m_pWndMembers->GetChild(_T("partybutt4"));
	m_pBtnModifyTenet	= (GUIButton*)m_pWndMembers->GetChild(_T("partybutt5"));	
	m_pBtnKickMember	= (GUIButton*)m_pWndMembers->GetChild(_T("partybutt50"));	
	m_pBtnDemissPostion = (GUIButton*)m_pWndMembers->GetChild(_T("partybutt500"));
	m_pPbnOffline		= (GUIPushButton*)m_pWndMembers->GetChild(_T("word1\\optbutt"));

	m_pLstMembers->SetColNum(7, 84);
	m_pLstMembers->SetColWidth(0, 139);
	m_pLstMembers->SetColWidth(2, 58);
	m_pLstMembers->SetColWidth(3, 58);
	m_pLstMembers->SetColWidth(6, 112);

	m_pGuildMemberPatch = (GUIPatch*)m_pWnd->GetChild(_T("party_back\\partymember_back"));

	
	m_pWndGuild = m_pWnd;
	m_pSkillPatch = (GUIPatch*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back"));
	m_pBtnCurSkillIcon = (IconStatic*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic\\skillback\\skill"));
	m_pStcCurSkillName = (GUIStatic*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic\\skillname"));
	m_pStcCurSkillLevel = (GUIStatic*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic\\skillgrade"));
	m_pStcFundCost = (GUIStatic*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic\\num1"));
	m_pStcMaterialCost = (GUIStatic*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic\\num2"));
	m_pProResearchProgress = (GUIProgress*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic\\progressbarback\\progressbar"));
	m_pBtnBookIcon = (IconStatic*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic1\\skillback\\skill"));
	m_pProgressIncrement = (GUIStatic*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic1\\num5"));
	m_pLearnBook = (GUIButton*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic1\\researchbutt"));
	m_pBtnBack = (GUIButton*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic2\\backbutt"));
	m_pBtnNext = (GUIButton*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic2\\nextbutt"));
	m_pStcLearnGoldCost = (GUIStatic*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\needingback_pic\\num1"));
	m_pStcLearnSilverCost = (GUIStatic*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\needingback_pic\\skillgrade"));
	m_pStcLearnContributionNeed = (GUIStatic*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\needingback_pic\\num4"));
	m_pStcLearnGuildFundCost = (GUIStatic*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\needingback_pic\\num3"));
	m_pStcLearnGuildMaterialCost = (GUIStatic*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\needingback_pic\\num5"));
	m_pBtnLearnSkill = (GUIButton*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\gskillbutt7"));
	m_pBtnCancelResearch = (GUIButton*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\gskillbutt6"));
	m_pBtnStartResearch = (GUIButton*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\gskillbutt5"));
	
	for (int i = 0; i < GUILD_SKILL_ITEM_NUM_PERPAGE; ++i)
	{
		tstring strItemBackPath = _T("party_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillback%d");
		TCHAR buffer[512];
		_sntprintf(buffer, 512, strItemBackPath.c_str(), i, i, i);
		m_pSkillItemBack[i] = (IconStatic*)m_pWndGuild->GetChild(buffer);

		tstring strItemPath = _T("party_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillback%d\\skill%d");
		_sntprintf(buffer, 512, strItemPath.c_str(), i, i, i);
		m_pSkillItem[i] = (IconStatic*)m_pWndGuild->GetChild(buffer);

		tstring strItemNamePaht = _T("party_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillname%d");
		_sntprintf(buffer, 512, strItemNamePaht.c_str(), i, i);
		m_pSkillName[i] = (GUIStatic*)m_pWndGuild->GetChild(buffer);

		tstring strItemLvPaht = _T("party_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillgrade%d");
		_sntprintf(buffer, 512, strItemLvPaht.c_str(), i, i);
		m_pSkillLevel[i] = (GUIStatic*)m_pWndGuild->GetChild(buffer);
	}
	
	for (int i = 1; i <= 5; ++i)
	{
		TCHAR buffer[512];
		_sntprintf(buffer, 512, _T("party_back\\message_back\\basicmessage_back2\\static%d"), i);
		m_pIstState[i - 1] = (IconStatic*)m_pWndGuild->GetChild(buffer);
	}
	
	
	m_pSkillPatch->SetInvisible(FALSE);
	m_pLearnBook->SetEnable(false);
	m_pPbnInfomation->SetEnable(false);

	// ��ȡ������Ϣ
	m_pGuild = GuildMgr::Inst()->GetGuildInfo();
	if(P_VALID(m_pGuild))
	{	
		GuildMgr::Inst()->SendGuildAllMembersGet();
		// ˢ�½���
		ReflashGuild();
		// ˢ�°�ť
		ReflashButton();
		// ��ȡ��Ա��Ϣ
		
		// ˢ���б�
		//ReflashList();
	}
	
	m_pStcSymbol->SetPic(_T("data\\ui\\guild\\guild_icon.bmp"));
	m_bShow = true;
	return bRet;
}

VOID GuildFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Drag:
		{
			for (int i = 0; i < GUILD_SKILL_ITEM_NUM_PERPAGE; ++i)
			{
				if (pWnd == m_pSkillItem[i])
				{
					OnSkillButtonClick(i);
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			for (int i = 0; i < GUILD_SKILL_ITEM_NUM_PERPAGE; ++i)
			{
				if (pWnd == m_pSkillItem[i])
				{
					int nChooseItem = m_nCurPageNum * (GUILD_SKILL_ITEM_NUM_PERPAGE - 1) + i;
					const std::vector<DWORD>& vecSkill = GuildMgr::Inst()->GetSkill();
					if ((size_t)nChooseItem < vecSkill.size())
					{
						DWORD dwSkillID = vecSkill[nChooseItem];
						ShowTip(m_pSkillItem[i], ToolTipCreator::Inst()->GetGuildSkillTips(dwSkillID).c_str());
					}
				}
			}
			if (pWnd == m_pBtnCurSkillIcon)
			{
				ShowTip(m_pBtnCurSkillIcon, ToolTipCreator::Inst()->GetGuildSkillTips(GuildMgr::Inst()->GetCurrentResearchSkillID()).c_str());
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnBack)
			{
				OnClickPreOrNextButton(true);
			}
			else if (pWnd == m_pBtnNext)
			{
				OnClickPreOrNextButton(false);

			}
			else if (pWnd == m_pBtnBookIcon)
			{
				m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_Book"), this));
			}
			else if (pWnd == m_pLearnBook)
			{
				//�ж�״̬�Ƿ�����
				if (GuildMgr::Inst()->IsInSpecState(EGDSS_Shortage) == true ||
					GuildMgr::Inst()->IsInSpecState(EGDSS_Distress) == true ||
					GuildMgr::Inst()->IsInSpecState(EGDSS_Chaos) == true)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_CantAdvSkillState")]);
					return;
				}
				
				// �ж��Ƿ���Ȩ��
				EGuildMemberPos ePos = GuildMgr::Inst()->GetLpInfo()->eGuildPos;
				const tagGuildPower *pPower = GuildMgr::Inst()->GetPowerList(ePos);
				if (!pPower->bAdvSkill)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_CantAdvSkill")]);
					return;
				}
				GuildMgr::Inst()->SendGuildSkillUpgrade(m_pSkillBook->GetItemId());
			}
			else if (pWnd == m_pBtnStartResearch)
			{
				const tagGuildBase *pGuildBase = GuildMgr::Inst()->GetGuildInfo();
				if (!P_VALID(m_dwCurChooseSkill))
					return;
				if (MTransSkillLevel(m_dwCurChooseSkill) > MAX_GUILD_SKILL_LEVEL)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_FullLevel")]);
					return;
				}
				const tagGuildSkillInfo *pGuildSkillInfo = GuildMgr::Inst()->FindSkillInfo(m_dwCurChooseSkill);
				if (pGuildBase->nFund < pGuildSkillInfo->nResearchFund || 
					pGuildBase->nMaterial < pGuildSkillInfo->nResearchMaterial)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_ResearchNotEnoughMoney")]);
					return;
				}
				if (GuildMgr::Inst()->IsInSpecState(EGDSS_Shortage) == true || 
					GuildMgr::Inst()->IsInSpecState(EGDSS_Distress) == true || 
					GuildMgr::Inst()->IsInSpecState(EGDSS_Warfare) == true)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_ResearchState")]);
					return;
				}



				GuildMgr::Inst()->SendResearchSkill(MTransSkillID(m_dwCurChooseSkill));
			}
			else if (pWnd == m_pBtnCancelResearch)
			{
				if (!P_VALID(GuildMgr::Inst()->GetCurrentResearchSkillID()))
					return;
				GuildMgr::Inst()->SendResearchSkill(GT_INVALID);

			}
			else if (pWnd == m_pBtnLearnSkill)
			{
				if (!P_VALID(m_dwCurChooseSkill))
					return;
				// ��Ǯ
				INT64 nSilver = CurrencyMgr::Inst()->GetBagSilver();
				
				const tagGuildMember *pMember = GuildMgr::Inst()->GetLpInfo();
				const tagGuildBase *pBase = GuildMgr::Inst()->GetGuildInfo();
				
				
				INT RoleLv = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
				const tagSkillData* pSkillData = SkillMgr::Inst()->GetSkillDataByTypeID(m_dwCurChooseSkill);
				if (P_VALID(pSkillData))
				{
					if (pSkillData->nLevel >= MTransSkillLevel(m_dwCurChooseSkill))
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_GuildSkillLearnAlready")]);
						return;
					}
				}

				DWORD dwCurPlayerSkillNextLevel;
				const tagSkillData* pData =  SkillMgr::Inst()->GetSkillData(MTransSkillID(m_dwCurChooseSkill));
				if (P_VALID(pData))
					dwCurPlayerSkillNextLevel = pData->nLevel + 1;
				else
					dwCurPlayerSkillNextLevel = 1;
				DWORD dwNextSkillID = MTransSkillTypeID(MTransSkillID(m_dwCurChooseSkill), dwCurPlayerSkillNextLevel);
				if (MTransSkillLevel(dwNextSkillID) > MAX_GUILD_SKILL_LEVEL)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_GuildSkillLearnAlreadyFull")]);
					return;
				}
				const tagGuildSkillInfo *pSkillInfo = GuildMgr::Inst()->FindSkillInfo(dwNextSkillID);
				const tagSkillProtoClient *pSkillClient = SkillProtoData::Inst()->FindSkillProto(dwNextSkillID);
				INT nNeedRoleLv = pSkillClient->nNeedRoleLevel;

				if (nSilver < pSkillInfo->nLearnSilver || 
					pMember->nContribution < pSkillInfo->nLearnContribution ||
					pBase->nFund < pSkillInfo->nLearnFund ||
					pBase->nMaterial < pSkillInfo->nLearnMaterial ||
					RoleLv < nNeedRoleLv ||
					(MTransSkillLevel(m_dwCurChooseSkill) - 1 == 0)
					)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_GuildSkillCannotLearn")]);
					return;
				}
				GuildMgr::Inst()->SendLearnGuildSkill(MTransSkillID(m_dwCurChooseSkill));
			}
			else if(pWnd == m_pBtnClose)// �رմ���
			{
				m_pFrameMgr->SendEvent(&tagGameEvent(_T("Close_Faction"), this));
				m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_Book"), NULL));
				return;
			}
			else if(pWnd == m_pBtnLookupInfo)// �鿴��Ϣ
			{
				OnClickedLookupInfo();
				return;
			}
			else if(pWnd == m_pBtnLeaveGuild)// ��������
			{
				OnClickedLeaveGuild();
				return;
			}
			else if(pWnd == m_pBtnTurnLeader)// �ƽ�����
			{				
				OnClickedTurnoverLeader();
				return;
			}
			else if(pWnd == m_pBtnAppointMember)// ����ְλ
			{
				OnClickedAppointMember();
				return;
			}
			else if(pWnd == m_pBtnModifyTenet)// �޸���ּ
			{	
				OnClickedModifyTenet();
				return;
			}
			else if(pWnd == m_pBtnKickMember)// �����Ա
			{

				OnClickedKickMember();
				return;
			}
			else if(pWnd == m_pBtnDemissPostion)// ��ȥְ��
			{
				OnClickedDemissPostion();
				return;
			}
			else
			{
				for(INT i=0; i<GuildMembers::ELCT_End; ++i)// ����
				{
					if(pWnd == m_pBtnListIndex[i])
					{
						//��������
						GuildMgr::Inst()->SortMembers(i);
						m_eSortType = (GuildMembers::EListColType)(i);
						//ˢ���б�
						ReflashList();
						break;
					}
				}
			}
			
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if(pWnd == m_pPbnOffline)
			{
				ReflashList();
				return;
			}
			else
			{
				if (pEvent->dwParam1 != EGUIBS_PushDown)//���ǰ��²�����
					return;

				if(pWnd == m_pPbnMembers)
				{
					m_eGuildFrameState = EGFS_MemberList;
					m_pSkillPatch->SetInvisible(TRUE);
					m_pGuildMemberPatch->SetInvisible(FALSE);
					//GuildMgr::Inst()->SendGuildAllMembersGet();
					return;
				}
				else if (pWnd == m_pPbnSkills)
				{
					m_eGuildFrameState = EGFS_GuildSkill;
					m_pSkillPatch->SetInvisible(FALSE);
					m_pGuildMemberPatch->SetInvisible(TRUE);
					GuildMgr::Inst()->SendGetGuildSkillInfo();
					return;
				}
			}
			 
		}
		break;
	}
}


VOID GuildFrame::ReflashGuild()
{
	m_pGuild = GuildMgr::Inst()->GetGuildInfo();
	// ����
	m_pStcName->SetText(GuildMgr::Inst()->GetGuildName(m_pGuild->dwID).c_str());
	// �ȼ�
	TCHAR szTmp[X_SHORT_NAME];
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("Role_Level")], m_pGuild->byLevel);
	m_pStcLevel->SetText(szTmp);
	// ���ɱ�־

	// ��ʼ��
	m_pStcFounder->SetText(PlayerNameTab::Inst()->FindNameByNameID(m_pGuild->dwFounderNameID));
	// ����
	m_pStcLeader->SetText(PlayerNameTab::Inst()->FindNameByID(m_pGuild->dwLeaderRoleID));
	// ��Ա����
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_pGuild->n16MemberNum);
	m_pStcMemberNum->SetText(szTmp);
	// ��������
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_pGuild->nReputation);
	m_pStcReputation->SetText(szTmp);
	// ����״̬

	// �����ʽ�
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_pGuild->nFund);
	m_pStcFund->SetText(szTmp);
	// �����ʲ�
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_pGuild->nMaterial);
	m_pStcMaterial->SetText(szTmp);
	// ���ɰ���
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_pGuild->n16Peace);
	m_pStcPeace->SetText(szTmp);
	// ÿ��ά������
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_pGuild->nDailyCost);
	m_pStcDailyCost->SetText(szTmp);
	
	// ������ּ
	m_pEdtTenet->Clear();
	m_pEdtTenet->AddText(GuildMgr::Inst()->GetGuildTenet().c_str());
	
	for (int i = 0; i < 5; ++i)
	{	
		m_pIstState[i]->SetInvisible(TRUE);
	}
	
	int nCount = 0;
	if (GuildMgr::Inst()->IsInSpecState(EGDSS_Shortage))
	{
		m_pIstState[nCount]->SetPic(_T("data\\ui\\guild\\L_guild-icon-duanque.bmp"));
		m_pIstState[nCount]->SetInvisible(FALSE);
		m_pIstState[nCount]->SetTips(g_StrTable[_T("GuildState_01")]);
		nCount++;
	}
	if (GuildMgr::Inst()->IsInSpecState(EGDSS_Distress))
	{
		m_pIstState[nCount]->SetPic(_T("data\\ui\\guild\\L_guild-icon-poor.bmp"));
		m_pIstState[nCount]->SetInvisible(FALSE);
		m_pIstState[nCount]->SetTips(g_StrTable[_T("GuildState_02")]);
		nCount++;
	}
	if (GuildMgr::Inst()->IsInSpecState(EGDSS_Chaos))
	{
		m_pIstState[nCount]->SetPic(_T("data\\ui\\guild\\L_guild-icon-luan.bmp"));
		m_pIstState[nCount]->SetInvisible(FALSE);
		m_pIstState[nCount]->SetTips(g_StrTable[_T("GuildState_03")]);
		nCount++;
	}
	if (GuildMgr::Inst()->IsInSpecState(EGDSS_Warfare))
	{
		m_pIstState[nCount]->SetPic(_T("data\\ui\\guild\\L_guild-icon-war.bmp"));
		m_pIstState[nCount]->SetInvisible(FALSE);
		m_pIstState[nCount]->SetTips(g_StrTable[_T("GuildState_04")]);
		nCount++;
	}
	if (GuildMgr::Inst()->IsInSpecState(EGDSS_Primacy))
	{
		m_pIstState[nCount]->SetPic(_T("data\\ui\\guild\\L_guild-icon-king.bmp"));
		m_pIstState[nCount]->SetInvisible(FALSE);
		m_pIstState[nCount]->SetTips(g_StrTable[_T("GuildState_05")]);
		nCount++;
	}
	if (GuildMgr::Inst()->IsInSpecState(EGDSS_Refulgence))
	{
		m_pIstState[nCount]->SetPic(_T("data\\ui\\guild\\L_guild-icon-huihuang.bmp"));
		m_pIstState[nCount]->SetInvisible(FALSE);
		m_pIstState[nCount]->SetTips(g_StrTable[_T("GuildState_06")]);
		nCount++;
	}
	//m_pWnd->SetRefresh(true);
}

VOID GuildFrame::ReflashList()
{
	m_pLstMembers->Clear();
	ReflashListImpl();
}

VOID GuildFrame::ReflashListImpl()
{
	m_pGuild = GuildMgr::Inst()->GetGuildInfo();
	bool bShowOffline = (m_pPbnOffline->GetState() == EGUIBS_PushDown);
		
	m_pLstMembers->Clear();
	m_mapTemp.clear();
	m_vecIndex.clear();

	INT nRow = 0;
	const GuildMembers* pMembers = GuildMgr::Inst()->GetMembers();
	if(P_VALID(pMembers))
	{
		const GuildMembers::MembersArray *members;
		pMembers->GetMembersList(&members);
		for(INT i=0; i<(INT)members->size(); ++i)
		{
			const tagGuildMemberClient* pInfo = members->at(i);

			if(!(bShowOffline || pInfo->sInfo.bOnline))
				continue;
			
			//���������ɫ�û�
			DWORD dwColor = 0xFFFFFFFF;
			if(!pInfo->sInfo.bOnline)
				dwColor = 0xFF808080;

			//����
			LPCTSTR szName = PlayerNameTab::Inst()->FindNameByID(pInfo->sInfo.dwRoleID);
			if(_tcslen(szName) > 0)
			{
				m_pLstMembers->SetText(nRow, 0, szName, dwColor);
			}
			else
			{
				m_mapTemp.insert(make_pair(pInfo->sInfo.dwRoleID, tagMemberNameReq(nRow, dwColor)));
			}

			//ְλ
			m_pLstMembers->SetText(nRow, 1, GuildMgr::Inst()->GetMemberPos((INT)pInfo->sInfo.n8GuildPos).c_str(), dwColor);
			//����
			TCHAR szTmp[X_SHORT_NAME];
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("Role_Level")], pInfo->sInfo.byLevel);
			m_pLstMembers->SetText(nRow, 2, szTmp, dwColor);
			//�Ա�
			m_pLstMembers->SetText(nRow, 3, GuildMgr::Inst()->GetMemberSex(pInfo->sInfo.bySex).c_str(), dwColor);
			//ְҵ
			m_pLstMembers->SetText(nRow, 4, ProfessionMgr::Inst().GetClassName(EClassType(pInfo->sInfo.byClass)), dwColor);
			//���ɹ���
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), pInfo->sInfo.nCurContrib);
			m_pLstMembers->SetText(nRow, 5, szTmp, dwColor);

			//����ʱ��
			m_pLstMembers->SetText(nRow, 6, GetOfflineTime(pInfo->sInfo.bOnline, pInfo->sInfo.dwTimeLastLogout).c_str(), dwColor);
			
			m_vecIndex.push_back(i);
			++nRow;
		}

		// ��Ա����
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_pGuild->n16MemberNum);
		m_pStcMemberNum->SetText(szTmp);
	}
	m_pWnd->SetRefresh(TRUE);
}

VOID GuildFrame::ReflashButton()
{
	
	// ��ȡ��ǰ���Ȩ�ޱ�
	const tagGuildPower* pPower = GuildMgr::Inst()->GetPowerList((INT)GuildMgr::Inst()->GetLpInfo()->eGuildPos);
	if (!P_VALID(pPower))
		return;

	m_pBtnLeaveGuild->SetEnable(pPower->bLeaveGuild);		// ������� 
	m_pBtnTurnLeader->SetEnable(pPower->bTurnoverLeader);	// �ƽ�����
	m_pBtnModifyTenet->SetEnable(pPower->bModifyTenet);		// �޸���ּ
	m_pBtnDemissPostion->SetEnable(pPower->bDemissPostion);	// ��ȥְ��
	m_pBtnStartResearch->SetEnable(pPower->bSetSkill);
	m_pBtnCancelResearch->SetEnable(pPower->bSetSkill);
	
	m_pBtnAppointMember->SetEnable(false);
	const tagGuildAppoint *pAppoint = GuildMgr::Inst()->GetAppointList((INT)GuildMgr::Inst()->GetLpInfo()->eGuildPos);
	for (size_t i = 0; i < pAppoint->BitSetGuildAppoint.count(); ++i)
	{
		if (pAppoint->BitSetGuildAppoint[i] != 0)
		{
			m_pBtnAppointMember->SetEnable(true);
			break;
		}
	}


	
	//���ݲ߻�����������Ȼ����������ɣ����ǿ��Ե��������ɵİ�ť��
	if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildPos() == static_cast<int>(EGMP_BangZhu))
	{
		m_pBtnLeaveGuild->SetEnable(true);
	}
	
	//����״̬д�ں��档��


}

VOID GuildFrame::SendGuildAppoint( DWORD dwDstRoleID, INT nPos )
{
	tagNC_GuildAppoint cmd;
	cmd.dwRoleID = dwDstRoleID;
	cmd.ePos = (EGuildMemberPos)nPos;
	m_pSession->Send(&cmd);

}

VOID GuildFrame::SendGuildLeave()
{
	tagNC_GuildLeave cmd;
	m_pSession->Send(&cmd);
}

VOID GuildFrame::SendGuildKick( DWORD dwDstRoleID )
{
	tagNC_GuildKick cmd;
	cmd.dwRoleID = dwDstRoleID;
	m_pSession->Send(&cmd);
}

VOID GuildFrame::SendGuildTurnover( DWORD dwDstRoleID )
{
	tagNC_GuildTurnover cmd;
	cmd.dwRoleID = dwDstRoleID;
	m_pSession->Send(&cmd);
}

VOID GuildFrame::SendGuildDemiss()
{
	tagNC_GuildDemiss cmd;
	m_pSession->Send(&cmd);
}

VOID GuildFrame::SendGuildTenetChange( tstring strTenetInfo )
{
	//�ж���ּ�Ƿ�Ƿ�
	DWORD dwErrorCode = FilterData::Inst()->WordsFilteror(strTenetInfo, 201);
	if(E_Success != dwErrorCode)
	{
		//������ʾ��Ϣ
		GuildMgr::Inst()->ShowGuildErrorMsg(dwErrorCode);
		return;
	}

	INT nLen = strTenetInfo.size();
	INT nMax = (nLen + 1) * sizeof(TCHAR) + sizeof(tagNC_GuildTenetChange) - 1;
	BYTE* pMsg = new BYTE[nMax];
	tagNC_GuildTenetChange* pCmd = (tagNC_GuildTenetChange*)pMsg;
	pCmd->dwID = pCmd->Crc32("NC_GuildTenetChange");
	memcpy(pCmd->szTenet, strTenetInfo.c_str(), (nLen + 1) * sizeof(TCHAR));
	pCmd->szTenet[nLen] = _T('\0');
	pCmd->dwSize = nMax;
	m_pSession->Send(pCmd);
	SAFE_DEL_ARRAY(pMsg);
}

DWORD GuildFrame::OnEventGetRoleName( tagRoleGetNameEvent* pGameEvent )
{
	if(pGameEvent->bResult)
	{
		NameReqList::iterator it = m_mapTemp.find(pGameEvent->dwRoleID);
		if(it != m_mapTemp.end())
		{
			const tagMemberNameReq& sMember = it->second;
			m_pLstMembers->SetText(sMember.nIndex, 0, pGameEvent->strRoleName.c_str(), sMember.dwColor);
			//m_pLstMembers->SetRefresh(true);
			m_mapTemp.erase(it);
		}
		
		// ����ǰ�������Ҫ���ð�������
		if(P_VALID(m_pGuild) && m_pGuild->dwLeaderRoleID == pGameEvent->dwRoleID)
		{
			m_pStcLeader->SetText(pGameEvent->strRoleName.c_str());
			//m_pStcLeader->SetRefresh(true);
		}
	}
	return 0;
}


DWORD GuildFrame::OnEventGetNameByNameID( tagGetNameByNameID* pGameEvent )
{
	if(pGameEvent->bResult)
	{
		if(P_VALID(m_pGuild) && pGameEvent->dwNameID == m_pGuild->dwFounderNameID)
		{
			m_pStcFounder->SetText(pGameEvent->strName.c_str());
			//m_pStcFounder->SetRefresh(true);
		}
	}
	return 0;
}

DWORD GuildFrame::OnEventGuildPosAppoint( tagGuildPosAppointEvent* pGameEvent )
{
	// ����������ְλ��û������Ȩ����
	const tagGuildPower *pPower = GuildMgr::Inst()->GetPowerList((INT)pGameEvent->ePos);
	if (!pPower->bCommerce)
	{
		if (GetObj("CommerceWarning_MsgBox"))
			KillObj("CommerceWarning_MsgBox");
		CreateObj("CommerceWarning_MsgBox", "MsgBox");

		TObjRef<MsgBox>("CommerceWarning_MsgBox")->Init(_T(""), g_StrTable[_T("GuildCommerceWarning0")], 
			_T("OnEventCommerceWarningMsgBox"), MsgBoxFlag(MBF_OK | MBF_Cancel), TRUE);
		m_dwTempRoleID = pGameEvent->dwRoleID;
		m_nTempPOS = (INT)pGameEvent->ePos;
		return 0;
	}

	SendGuildAppoint(pGameEvent->dwRoleID, (INT)pGameEvent->ePos);
	return 0;
}

DWORD GuildFrame::OnEventGuildInfoReflash( tagGuildInfoReflashEvent* pGameEvent )
{
	// ����Ǳ�����ң���Ҫˢ��һ�°�ť
	if(pGameEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
	{
		ReflashButton();
	}
	ReflashGuild();
	ReflashList();
	return 0;
}

DWORD GuildFrame::OnEventGuildInfoReq( tagGuildInfoReflashEvent* pGameEvent )
{
	//���������¼�
	tagNC_GuildRefMember cmd;
	cmd.dwRoleID = pGameEvent->dwRoleID;
	m_pSession->Send(&cmd);
	return 0;
}

DWORD GuildFrame::OnEventMsgBoxInputTenet( tagMsgInputBoxEvent* pGameEvent )
{
	if(MBIT_Text == pGameEvent->eInputType && MBF_OK == pGameEvent->eResult)
	{
		// �����޸��¼�
		SendGuildTenetChange(pGameEvent->strInputName.c_str());
	}
	return 0;
}

DWORD GuildFrame::OnEventGetGuildName( tagGuildNameGetEvent* pGameEvent )
{
	if(P_VALID(m_pGuild) && pGameEvent->dwGuildID == m_pGuild->dwID)
	{
		m_pStcName->SetText(pGameEvent->strName.c_str());
		//m_pStcName->SetRefresh(true);
	}
	return 0;
}

DWORD GuildFrame::OnEventGetGuildTenet( tagGameEvent* pGameEvent )
{
	m_pEdtTenet->Clear();
	m_pEdtTenet->SetText(GuildMgr::Inst()->GetGuildTenet().c_str());
	//m_pEdtTenet->SetRefresh(true);
	return 0;
}

DWORD GuildFrame::OnEventGuildListReflash( tagGameEvent* pGameEvent )
{
	// ˢ���б�
	GuildMgr::Inst()->SortMembers(m_eSortType);
	ReflashList();
	return 0;
}


VOID GuildFrame::OnClickedLookupInfo()
{
	GameFrame* pFrame = m_pMgr->GetFrame(_T("MemberInfo"));
	if(P_VALID(pFrame))// �д��壬���͸����¼�
	{
		const tagGuildMemberClient* pMember = this->GetMember(m_pLstMembers->GetCurSelectedRow());
		if(P_VALID(pMember))
		{
			// �����û��ȡ��չ��Ϣ����Ҫ��ȡһ��
			if(!pMember->bGetInfo)
			{
				// ����������Ϣ��ȡ��Ա��չ��Ϣ
				tagNC_GuildGetMemberEx cmd;
				cmd.dwRoleID = pMember->sInfo.dwRoleID;
				m_pSession->Send(&cmd);
			}

			tagGuildInfoReflashEvent event(_T("GuildEvent_InfoUpdate"), this);
			event.dwRoleID = pMember->sInfo.dwRoleID;
			m_pMgr->SendEvent(&event);
		}
	}
	else// û�������´���
	{
		INT nIndex = m_pLstMembers->GetCurSelectedRow();
		if(nIndex >= 0 && nIndex < (INT)m_vecIndex.size())
			m_pMgr->CreateFrame(m_strName.c_str(), _T("MemberInfo"), _T("GuildMemberInfoFrame"), m_vecIndex[nIndex]);
		const tagGuildMemberClient* pMember = this->GetMember(m_pLstMembers->GetCurSelectedRow());
		if(P_VALID(pMember))
		{
			// �����û��ȡ��չ��Ϣ����Ҫ��ȡһ��
			if(!pMember->bGetInfo)
			{
				// ����������Ϣ��ȡ��Ա��չ��Ϣ
				tagNC_GuildGetMemberEx cmd;
				cmd.dwRoleID = pMember->sInfo.dwRoleID;
				m_pSession->Send(&cmd);
			}

			tagGuildInfoReflashEvent event(_T("GuildEvent_InfoUpdate"), this);
			event.dwRoleID = pMember->sInfo.dwRoleID;
			m_pMgr->SendEvent(&event);
		}
	}
}

VOID GuildFrame::OnClickedLeaveGuild()
{
	// ����ǰ���
	if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildPos() == static_cast<int>(EGMP_BangZhu))
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_BangZhuLeaveCheck")]);
		return;
	}
	else
	{
		// ����ȷ�϶Ի���
		if(P_VALID(GetObj("LeaveGuild")))
			KillObj("LeaveGuild");

		CreateObj("LeaveGuild", "MsgBox");
		TObjRef<MsgBox>("LeaveGuild")->Init(
			_T(""),g_StrTable[_T("GuildText_LeaveCheck")],
			_T("MsgBox_LeaveCheck"));
	}
}

VOID GuildFrame::OnClickedTurnoverLeader()
{
	const tagGuildMemberClient* pInfo = this->GetMember(m_pLstMembers->GetCurSelectedRow());
	if(P_VALID(pInfo))
	{
		// ������Լ�ֱ�ӷ���
		if(pInfo->sInfo.dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
			return;
		
		// �洢Ŀ��ID
		m_dwDstRoleID = pInfo->sInfo.dwRoleID;


		// ����ȷ�϶Ի���
		if(P_VALID(GetObj("TurnoverLeader")))
			KillObj("TurnoverLeader");

		TCHAR szTmp[X_LONG_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildText_TurnoverCheck")], 
			PlayerNameTab::Inst()->FindNameByID(pInfo->sInfo.dwRoleID));

		CreateObj("TurnoverLeader", "MsgBox");
		TObjRef<MsgBox>("TurnoverLeader")->Init(
			_T(""), szTmp, _T("MsgBox_TurnoverCheck"));
	}
}

VOID GuildFrame::OnClickedAppointMember()
{
	const tagGuildMemberClient* pInfo = this->GetMember(m_pLstMembers->GetCurSelectedRow());
	if(P_VALID(pInfo))
	{
		// ������Լ�ֱ�ӷ���
		if(pInfo->sInfo.dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
			return;

		// ���ҽ��棬�������ֱ��ˢ�£�û���򴴽�
		GameFrame* pFrame = m_pMgr->GetFrame(_T("MemberPos"));
		if(P_VALID(pFrame))
		{
			((GuildMemberPosFrame*)pFrame)->ReflashPosButton(m_vecIndex[m_pLstMembers->GetCurSelectedRow()]);
		}
		else
		{
			m_pMgr->CreateFrame(m_strName.c_str(), _T("MemberPos"), _T("GuildMemberPosFrame"), m_vecIndex[m_pLstMembers->GetCurSelectedRow()]);
		}
	}
}

VOID GuildFrame::OnClickedModifyTenet()
{
	// ����������ּ����Ի���
	if(P_VALID(GetObj("ModifyTenet")))
		KillObj("ModifyTenet");

	CreateObj("ModifyTenet", "MsgInputBox");
	TObjRef<MsgInputBox>("ModifyTenet")->Init(
		_T(""),g_StrTable[_T("GuildText_InputTenet")],
		_T("MsgBox_ModifyTenet"),GT_INVALID, MBIT_Text);
}

VOID GuildFrame::OnClickedKickMember()
{
	const tagGuildMemberClient* pInfo = this->GetMember(m_pLstMembers->GetCurSelectedRow());
	if(P_VALID(pInfo))
	{
		// ������Լ�ֱ�ӷ���
		if(pInfo->sInfo.dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
			return;
		// �ж��Ƿ���Կ�������
		const tagGuildMember* pLp = GuildMgr::Inst()->GetLpInfo();
		if(!P_VALID(pLp))
			return;
		// ��ȡ�����
		const tagGuildKick* pKick = GuildMgr::Inst()->GetKickList((INT)pLp->eGuildPos);
		if(!P_VALID(pKick))
			return;
		// �ж��Ƿ�ɿ���
		if(!pKick->BitSetGuildKick[pInfo->sInfo.n8GuildPos])
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildError_Kick")]);
			return;
		}

		// �洢Ŀ��ID
		m_dwDstRoleID = pInfo->sInfo.dwRoleID;

		// ����ȷ�϶Ի���
		if(P_VALID(GetObj("KickMember")))
			KillObj("KickMember");

		TCHAR szTmp[X_LONG_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildText_KickCheck")], 
			PlayerNameTab::Inst()->FindNameByID(pInfo->sInfo.dwRoleID), PlayerNameTab::Inst()->FindNameByID(pInfo->sInfo.dwRoleID));
		
		CreateObj("KickMember", "MsgBox");
		TObjRef<MsgBox>("KickMember")->Init(
			_T(""), szTmp, _T("MsgBox_KickCheck"));
	}
}

VOID GuildFrame::OnClickedDemissPostion()
{
	// ����ȷ�϶Ի���
	if(P_VALID(GetObj("DemissPostion")))
		KillObj("DemissPostion");

	CreateObj("DemissPostion", "MsgBox");
	TObjRef<MsgBox>("DemissPostion")->Init(
		_T(""),g_StrTable[_T("GuildText_DemissCheck")],
		_T("MsgBox_DemissCheck"));
}

BOOL GuildFrame::EscCancel()
{
	if(m_pWnd->IsInvisible())
		return FALSE;
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Close_Faction"), this));
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_Book"), NULL));
	return TRUE;
}

VOID GuildFrame::DestroyMsgBox()
{
	if(P_VALID(GetObj("ModifyTenet")))
		KillObj("ModifyTenet");

	if(P_VALID(GetObj("LeaveGuild")))
		KillObj("LeaveGuild");	

	if(P_VALID(GetObj("TurnoverLeader")))
		KillObj("TurnoverLeader");

	if(P_VALID(GetObj("KickMember")))
		KillObj("KickMember");

	if(P_VALID(GetObj("DemissPostion")))
		KillObj("DemissPostion");

	if (GetObj("CommerceWarning_MsgBox"))
		KillObj("CommerceWarning_MsgBox");
}

DWORD GuildFrame::OnEventMsgBoxLeaveCheck( tagMsgBoxEvent* pGameEvent )
{
	if(pGameEvent->eResult == MBF_OK)
	{
		SendGuildLeave();
	}
	return 0;
}

DWORD GuildFrame::OnEventMsgBoxTurnoverCheck( tagMsgBoxEvent* pGameEvent )
{
	if(pGameEvent->eResult == MBF_OK)
	{
		SendGuildTurnover(m_dwDstRoleID);
		m_dwDstRoleID = GT_INVALID;
	}
	return 0;
}

DWORD GuildFrame::OnEventMsgBoxKickCheck( tagMsgBoxEvent* pGameEvent )
{
	if(pGameEvent->eResult == MBF_OK)
	{
		SendGuildKick(m_dwDstRoleID);
		m_dwDstRoleID = GT_INVALID;
	}
	return 0;
}

DWORD GuildFrame::OnEventMsgBoxDemissCheck( tagMsgBoxEvent* pGameEvent )
{
	if(pGameEvent->eResult == MBF_OK)
	{
		SendGuildDemiss();
	}
	return 0;
}

tstring GuildFrame::GetOfflineTime( bool bIsOnline, DWORD dwOfflineTime /*= GT_INVALID*/ )
{
	if(bIsOnline)// ����
	{
		return g_StrTable[_T("GuildList_Online")];
	}
	
	// ��ȡ��ǰʱ�������ʱ��
	tagDWORDTime srcTime = (tagDWORDTime)dwOfflineTime;
	tagDWORDTime dstTime = ServerTime::Inst()->CalCurrentServerDwordTime();
	// ������˶�����
	DWORD dwSecond = CalcTimeDiff(dstTime, srcTime);
	if(dwSecond > 365*24*60*60)// ����1��
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildList_Offline_Year")], dwSecond/(365*24*60*60));
		return szTmp;
	}
	else if(dwSecond > 30*24*60*60)// ����1����
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildList_Offline_Month")], dwSecond/(30*24*60*60));
		return szTmp;
	}
	else if(dwSecond > 24*60*60)// ����1��
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildList_Offline_Day")], dwSecond/(24*60*60));
		return szTmp;
	}
	else if(dwSecond > 60*60)// ����1Сʱ
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildList_Offline_Hour")], dwSecond/(60*60));
		return szTmp;
	}
	else// С��һСʱ
	{
		return g_StrTable[_T("GuildList_Offline_Min")];
	}
}

const tagGuildMemberClient* GuildFrame::GetMember( INT nIndex )
{
	if(nIndex <0 || nIndex >= (INT)m_vecIndex.size())
		return NULL;

	return GuildMgr::Inst()->GetMember(m_vecIndex[nIndex]);
}

DWORD GuildFrame::OnEventRefreshGuildSkills(tagGameEvent *pEvent)
{
	//m_pGuildMemberPatch->SetInvisible(TRUE);
	RefreshSkills();
	//m_pSkillPatch->SetInvisible(FALSE);
	return 0;
}

DWORD GuildFrame::OnEventShowGuildFrame(tagGameEvent *pEvent)
{
	
	// ˢ�°���
	ReflashGuild();
	// ˢ�¹��ܰ�ť
	ReflashButton();

	//m_pLstMembers->Clear();

	m_pWnd->SetInvisible(FALSE);

	m_pFrameMgr->SendEvent(&tagGameEvent( _T("Open_Guild_UI"), NULL ));
	m_bShow = true;
	return 0;
}

DWORD GuildFrame::OnEventHideGuildFrame(tagGameEvent *pEvent)
{
	// ���ؼ���ҳ
	m_pSkillPatch->SetInvisible(TRUE);
	// ��ʾ��Ա��
	m_pGuildMemberPatch->SetInvisible(FALSE);
	m_pPbnMembers->SetState(EGUIBS_PushDown, TRUE);
	m_eGuildFrameState = EGFS_MemberList;
	// ��������
	m_pWnd->SetInvisible(TRUE);
	m_bShow = false;
	m_pFrameMgr->SendEvent(&tagGameEvent( _T("Close_Guild_UI"), NULL ));
	
	return 0;
}

void GuildFrame::SetSkillDefaultUI()
{
	for (int i = 0; i < GUILD_SKILL_ITEM_NUM_PERPAGE; ++i)
	{
		m_pSkillItem[i]->SetPic(_T("data\\ui\\main\\l_icon_back.bmp"));
		m_pSkillName[i]->SetText(_T(""));
		m_pSkillLevel[i]->SetText(_T(""));
	}
	m_pBtnCurSkillIcon->SetPic(_T("data\\ui\\main\\l_icon_back.bmp"));
	m_pBtnBookIcon->SetPic(_T("data\\ui\\main\\l_icon_back.bmp"));

	m_nCurPageNum = 0;
	m_pBtnBack->SetEnable(false);
	m_pBtnNext->SetEnable(false);
	const std::vector<DWORD>& vecSkills = GuildMgr::Inst()->GetSkill();
	int nSize = static_cast<int>(vecSkills.size());
	if (nSize > (m_nCurPageNum + 1) * GUILD_SKILL_ITEM_NUM_PERPAGE)
	{
		m_pBtnNext->SetEnable(true);
	}
	m_pProResearchProgress->SetValue(0);
}

void GuildFrame::RefreshSkills()
{
	//SetSkillDefaultUI();
	const std::vector<DWORD>& vecSkills = GuildMgr::Inst()->GetSkill();
	size_t size = vecSkills.size();
	size_t ItemBegin = m_nCurPageNum * GUILD_SKILL_ITEM_NUM_PERPAGE;
	if (ItemBegin < size)
	{
		DWORD dwItem = 0;
		for (size_t i = ItemBegin; i < size; ++i, ++dwItem)
		{
			DWORD dwID = vecSkills[i];
			if (MTransSkillLevel(dwID) <= MAX_GUILD_SKILL_LEVEL)
			{
				tstring strIconPath = SkillMgr::Inst()->GetSkillIconPath(dwID);
				m_pSkillItem[i]->SetPic(strIconPath);
				DWORD dwLevel = MTransSkillLevel(dwID) - 1;
				DWORD dwCurPlayerSkillLevel;
				const tagSkillData* pData =  SkillMgr::Inst()->GetSkillData(MTransSkillID(dwID));
				if (P_VALID(pData))
					dwCurPlayerSkillLevel = pData->nLevel;
				else
					dwCurPlayerSkillLevel = 0;

				tstring strFormat = _T("%d/%d");
				TCHAR buffer[10];
				_sntprintf(buffer, 10, strFormat.c_str(), dwCurPlayerSkillLevel, dwLevel);
				m_pSkillLevel[i]->SetText(buffer);

				const tagGuildSkillInfo *pInfo = GuildMgr::Inst()->FindSkillInfo(dwID);
				if (P_VALID(pInfo))
					m_pSkillName[i]->SetText(pInfo->strName.c_str());
			}
			else
			{
				// ת������һ��
				DWORD dwChangeID = MTransSkillTypeID((MTransSkillID(dwID)),(MTransSkillLevel(dwID) - 1));
				tstring strIconPath = SkillMgr::Inst()->GetSkillIconPath(dwChangeID);
				m_pSkillItem[i]->SetPic(strIconPath);
				DWORD dwLevel = MTransSkillLevel(dwID) - 1;
				DWORD dwCurPlayerSkillLevel;
				const tagSkillData* pData =  SkillMgr::Inst()->GetSkillData(MTransSkillID(dwID));
				if (P_VALID(pData))
					dwCurPlayerSkillLevel = pData->nLevel;
				else
					dwCurPlayerSkillLevel = 0;
				tstring strFormat = _T("%d/%d");
				TCHAR buffer[10];
				_sntprintf(buffer, 10, strFormat.c_str(), dwCurPlayerSkillLevel, dwLevel);
				m_pSkillLevel[i]->SetText(buffer);

				const tagGuildSkillInfo *pInfo = GuildMgr::Inst()->FindSkillInfo(dwChangeID);
				if (P_VALID(pInfo))
					m_pSkillName[i]->SetText(pInfo->strName.c_str());
			}
		}
	}
	// ���õ�ǰ�о�����
	DWORD curResearchSkillID= GuildMgr::Inst()->GetCurrentResearchSkillID();
	if (P_VALID(curResearchSkillID) && MTransSkillLevel(curResearchSkillID) <= MAX_GUILD_SKILL_LEVEL)
	{
		INT32 curResearchSkillProgress = GuildMgr::Inst()->GetCurrentResearchSkillProgress();
		tstring strCurResearchSkillIcon = SkillMgr::Inst()->GetSkillIconPath(curResearchSkillID);
		m_pBtnCurSkillIcon->SetPic(strCurResearchSkillIcon);
		const tagGuildSkillInfo *pCurInfo = GuildMgr::Inst()->FindSkillInfo(curResearchSkillID);
		m_pStcCurSkillName->SetText(pCurInfo->strName.c_str());
		tstring strFormat = _T("%d");
		TCHAR buffer[10];
		_sntprintf(buffer, 10, strFormat.c_str(), MTransSkillLevel(pCurInfo->dwSkillID));
		m_pStcCurSkillLevel->SetText(buffer);
		_sntprintf(buffer, 10, strFormat.c_str(), pCurInfo->nResearchFund);
		m_pStcFundCost->SetText(buffer);
		_sntprintf(buffer, 10, strFormat.c_str(), pCurInfo->nResearchMaterial);
		m_pStcMaterialCost->SetText(buffer);
		m_pProResearchProgress->SetMaxValue((FLOAT)pCurInfo->n16Fulfill);
		m_pProResearchProgress->SetValue((FLOAT)curResearchSkillProgress, true);
		m_pProResearchProgress->SetRefresh();
	}
	// �����ǰ�о����ܲ�����
	else
	{
		m_pBtnCurSkillIcon->SetPic(_T("data\\ui\\main\\l_icon_back.bmp"));
		m_pStcCurSkillName->SetText(_T(""));
		m_pStcCurSkillLevel->SetText(_T(""));
		m_pStcFundCost->SetText(_T(""));
		m_pStcMaterialCost->SetText(_T(""));
		m_pProResearchProgress->SetMaxValue(0);
		m_pProResearchProgress->SetValue(0, true);
	}
	if (!P_VALID(m_dwCurChooseSkill))
		m_pBtnLearnSkill->SetEnable(false);
}

void GuildFrame::OnSkillButtonClick(int nButton)
{
	int nChooseItem = m_nCurPageNum * (GUILD_SKILL_ITEM_NUM_PERPAGE - 1) + nButton;
	const std::vector<DWORD>& vecSkill = GuildMgr::Inst()->GetSkill();
	if ((size_t)nChooseItem < vecSkill.size())
	{
		DWORD dwSkillID = vecSkill[nChooseItem];
		m_dwCurChooseSkill = dwSkillID;
		DWORD dwCurPlayerSkillNextLevel;
		const tagSkillData* pData =  SkillMgr::Inst()->GetSkillData(MTransSkillID(dwSkillID));
		if (P_VALID(pData))
			dwCurPlayerSkillNextLevel = pData->nLevel + 1;
		else
			dwCurPlayerSkillNextLevel = 1;
		DWORD dwNextSkillID = MTransSkillTypeID(MTransSkillID(dwSkillID), dwCurPlayerSkillNextLevel);
		m_pSkillItemBack[nButton]->SetPic(_T("data\\ui\\Common\\L_icon-l.bmp"));
		if (MTransSkillLevel(dwNextSkillID) > MAX_GUILD_SKILL_LEVEL)
			return;
		m_pBtnLearnSkill->SetEnable(true);
		
		const tagGuildSkillInfo *pInfo = GuildMgr::Inst()->FindSkillInfo(dwNextSkillID);
		tstring strFormat = _T("%d");
		TCHAR buffer[20];
		_sntprintf(buffer, 20, strFormat.c_str(), MSilver2DBGold(pInfo->nLearnSilver));
		m_pStcLearnGoldCost->SetText(buffer);
		_sntprintf(buffer, 20, strFormat.c_str(), MSilver2DBSilver(pInfo->nLearnSilver));
		m_pStcLearnSilverCost->SetText(buffer);
		_sntprintf(buffer, 20, strFormat.c_str(), pInfo->nLearnFund);
		m_pStcLearnGuildFundCost->SetText(buffer);
		_sntprintf(buffer, 20, strFormat.c_str(), pInfo->nLearnMaterial);
		m_pStcLearnGuildMaterialCost->SetText(buffer);
		_sntprintf(buffer, 20, strFormat.c_str(), pInfo->nLearnContribution);
		m_pStcLearnContributionNeed->SetText(buffer);
		
		for (int i = 0; i < GUILD_SKILL_ITEM_NUM_PERPAGE; ++i)
		{
			if (i == nButton)
				continue;
			m_pSkillItemBack[i]->SetPic(_T("data\\ui\\common\\l_icon.bmp"));
		}
	}
}

void GuildFrame::OnClickPreOrNextButton(bool bOrder)
{
	// ����Ǻ��˰�ť
	if (bOrder)
	{
		if (m_nCurPageNum > 0)
		{
			m_nCurPageNum--;
			m_pBtnNext->SetEnable(true);
		}
		if (m_nCurPageNum == 0)
			m_pBtnBack->SetEnable(false);
	}
	// �����ǰ����ť
	else
	{
		const std::vector<DWORD>& vecSkill = GuildMgr::Inst()->GetSkill();
		int nSize = static_cast<int>(vecSkill.size());
		if (nSize > (m_nCurPageNum + 1) * GUILD_SKILL_ITEM_NUM_PERPAGE)
		{
			m_nCurPageNum++;
			m_pBtnNext->SetEnable(true);
		}
		if (nSize > (m_nCurPageNum + 1) * GUILD_SKILL_ITEM_NUM_PERPAGE)
			m_pBtnNext->SetEnable(false);
	}
}

DWORD GuildFrame::OnEventRefreshGuild(tagGameEvent *pEvent)
{
	ReflashGuild();
	ReflashButton();
	return 0;
}

DWORD GuildFrame::OnEventUsGuildSkillBook(tagUseGuildSkillBookEvent *pGameEvent)
{
	//�ж��Ƿ��е�ǰ�о�����
	if (!P_VALID(GuildMgr::Inst()->GetCurrentResearchSkillID()))
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_HaveNotCurReserachSkill")]);
		return 0;
	}
	Item *pItem = pGameEvent->pItem;
	tstring strIconPath = ItemMgr::Inst()->GetItemIconPath(pItem->GetItemTypeID());
	m_pBtnBookIcon->SetPic(strIconPath);
	const tagItemProto *pProto = pItem->GetProto();
	if (!P_VALID(pProto))
		return 0;
	m_pSkillBook = pItem;
	INT32 nProgressInc = pProto->nSpecFuncVal1;
	tstring strFormat(_T("%d"));
	TCHAR buffer[12];
	_sntprintf(buffer, 12, strFormat.c_str(), nProgressInc);
	m_pProgressIncrement->SetText(buffer);
	m_pLearnBook->SetEnable(true);
	return 0;
}

DWORD GuildFrame::OnEventRefreshBook(tagGameEvent *pGameEvent)
{
	m_pBtnBookIcon->SetPic(_T("data\\ui\\main\\l_icon_back.bmp"));
	m_pProgressIncrement->SetText(_T(""));
	m_pLearnBook->SetEnable(false);
	m_pSkillBook = NULL;
	return 0;
}

DWORD GuildFrame::OnEventCommerceWarningMsgBox(tagMsgBoxEvent *pEvent)
{
	if (pEvent->eResult == MBF_OK)
	{
		SendGuildAppoint(m_dwTempRoleID, (INT)m_nTempPOS);
	}
	return 0;
}
