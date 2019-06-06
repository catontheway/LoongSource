//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_mgr.cpp
// author: Sxg
// actor:
// data: 2009-4-7
// last:
// brief: ���ɹ���
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/filter.h"
#include "../WorldDefine/msg_guild.h"
#include "../ServerDefine/msg_guild.h"
#include "../ServerDefine/log_cmdid_define.h"

#include "guild_mgr.h"
#include "role_mgr.h"
#include "role.h"
#include "guild.h"
#include "script_mgr.h"
#include "mall.h"

GuildMgr g_guildMgr;
//-------------------------------------------------------------------------------
// ����&����
//-------------------------------------------------------------------------------
GuildMgr::GuildMgr()
{
	;
}

GuildMgr::~GuildMgr()
{
	Destroy();
}

//-------------------------------------------------------------------------------
// init
//-------------------------------------------------------------------------------
BOOL GuildMgr::Init()
{
	INT32 nTmp = 0;

	g_ScriptMgr.GetGlobal("guild_CreateRoleMinLevel", nTmp);
	m_GuildCfg.nCreateRoleMinLevel	= nTmp;

	g_ScriptMgr.GetGlobal("guild_GoldCreateNeeded", nTmp);
	m_GuildCfg.nGoldCreateNeeded	= nTmp;

	g_ScriptMgr.GetGlobal("guild_GuildLevelBegin", nTmp);
	m_GuildCfg.byGuildLevelBegin	= (BYTE)nTmp;

	g_ScriptMgr.GetGlobal("guild_GuildPeaceBegin", nTmp);
	m_GuildCfg.n16GuildPeaceBegin	= (INT16)nTmp;

	g_ScriptMgr.GetGlobal("guild_GuildRepBegin", nTmp);
	m_GuildCfg.nGuildRepBegin		= nTmp;

	g_ScriptMgr.GetGlobal("guild_GuildFundBegin", nTmp);
	m_GuildCfg.nGuildFundBegin		= nTmp;

	g_ScriptMgr.GetGlobal("guild_GuildMaterialBegin", nTmp);
	m_GuildCfg.nGuildMaterialBegin	= nTmp;

	g_ScriptMgr.GetGlobal("guild_GuildGroupPurchaseBegin", nTmp);
	m_GuildCfg.nGuildGroupPurchaseBegin = nTmp;

	g_ScriptMgr.GetGlobal("guild_JoinRoleMinLevel", nTmp);
	m_GuildCfg.nJoinRoleMinLevel	= nTmp;

	return TRUE;
}

//-------------------------------------------------------------------------------
// update
//-------------------------------------------------------------------------------
VOID GuildMgr::Update()
{
	Guild *pGuild;
	MapGuild::TMapIterator iter = m_mapGuild.Begin();
	while(m_mapGuild.PeekNext(iter, pGuild))
	{
		pGuild->Update();
	}
}

//-------------------------------------------------------------------------------
// destroy
//-------------------------------------------------------------------------------
VOID GuildMgr::Destroy()
{
	Guild *pGuild = NULL;
	MapGuild::TMapIterator iter = m_mapGuild.Begin();
	while(m_mapGuild.PeekNext(iter, pGuild))
	{
		SAFE_DEL(pGuild);
	}

	m_mapGuild.Clear();
}

//-------------------------------------------------------------------------------
// ��LoongDB���Ӻ����а������Գ�ʼ��
//-------------------------------------------------------------------------------
VOID GuildMgr::InitDBGuild(const tagGuildLoad* pGuildLoad)
{
	if(m_mapGuild.IsExist(pGuildLoad->dwID))
	{
		// LoongDB˲��
		return;
	}
	
	Guild *pNewGuild = new Guild(pGuildLoad);
	if(!P_VALID(pNewGuild))
	{
		ASSERT(0);
		IMSG(_T("\n\n\nCaution:\tGuild(load from db: id=%u) create failed!\n\n"), pGuildLoad->dwID);
		return;
	}

	m_mapGuild.Add(pGuildLoad->dwID, pNewGuild);
}

//-------------------------------------------------------------------------------
// ��LoongDB���Ӻ����а��ɳ�Ա��ʼ��
//-------------------------------------------------------------------------------
VOID GuildMgr::InitDBGuildMember(const tagGuildMemLoad *pGuildMemberLoad, const INT32 nNum)
{
	ASSERT(nNum > 0);

/*	Guild *pChkGuild = GetGuild(pGuildMemberLoad[0].dwGuildID);
	if(P_VALID(pChkGuild))
	{
		// ���LoongDB�Ƿ��ǳ�������
		if(pChkGuild->GetGuildMemberNum() > 0)
		{
			ASSERT(0);
			IMSG(_T("\n\nCaution: \n\tLoongDB connected is not the first time! \n"));
			IMSG(_T("\tIf you want to reload db data, please run clear in console and restart LoongDB Server!\n\n"));
			return;
		}
	}
*/	
	for(INT32 i=0; i<nNum; ++i)
	{
		Guild *pGuild = GetGuild(pGuildMemberLoad[i].dwGuildID);
		if(!P_VALID(pGuild))
		{
			// �ϲ㴦���߼������⣬�����ݿ�(δʹ��������)��������
			ASSERT(0);
			continue;
		}

		pGuild->InitDBGuildMember(pGuildMemberLoad[i].sGuildMember);
	}
}

//-------------------------------------------------------------------------------
// ����Ƿ����а��ɶ���ʼ���ɹ�
//-------------------------------------------------------------------------------
BOOL GuildMgr::IsGuildInitOK()
{
	BOOL bRet	= TRUE;

	Guild* pGuild	= NULL;
	MapGuild::TMapIterator iter = m_mapGuild.Begin();
	while (m_mapGuild.PeekNext(iter, pGuild))
	{
		if (!P_VALID(pGuild))
		{
			continue;
		}

		if (!pGuild->IsGuildInitOK())
		{
			ASSERT(0);
			ILOG->Write(_T("Guild<id: %u> load failed! Please check db! \n"), pGuild->GetGuildAtt().dwID);
			bRet = FALSE;
		}
	}

	return bRet;
}

//-------------------------------------------------------------------------------
// �ر�LoongWorldʱ���������а��ɻ�����Ϣ
//-------------------------------------------------------------------------------
VOID GuildMgr::SendAllGuild2DB()
{
	INT32 nGuildNum = GetGuildNum();
	if(nGuildNum <= 0)
	{
		return;
	}

	INT32 nMsgSz = sizeof(tagNDBC_SaveAllGuild) + (nGuildNum - 1) * sizeof(tagGuildBase);
	MCREATE_MSG(pSend, nMsgSz, NDBC_SaveAllGuild);
	pSend->nGuildNum	= nGuildNum;

	INT32 nIndex	= 0;
	Guild *pGuild	= NULL;
	MapGuild::TMapIterator iter = m_mapGuild.Begin();
	while(m_mapGuild.PeekNext(iter, pGuild))
	{
		pSend->pGuildBase[nIndex++] = (tagGuildBase)pGuild->GetGuildAtt();
	}

	ASSERT(nIndex == nGuildNum);
	g_dbSession.Send(pSend, pSend->dwSize);
	MDEL_MSG(pSend);
}

//-------------------------------------------------------------------------------
// �ͻ������Ӻ󣬳�ʼ�����ڰ�����Ϣ
//-------------------------------------------------------------------------------
VOID GuildMgr::InitGuildInitState(Role *pRole, OUT tagGuildBase &sGuildBase, 
								  OUT tagGuildMember &sGuildMember)
{
	ASSERT(P_VALID(pRole));

	sGuildBase.dwID			= GT_INVALID;
	sGuildMember.dwRoleID	= GT_INVALID;

	Guild *pGuild = GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		if(m_mapGuild.Size() > 0)
		{
			// ִ�е��ô���˵���ó�Ա�ѱ��߳�
			pRole->SetGuildID(GT_INVALID);
			return;
		}
		
		// ִ�е��ô����п������ݿ���û�а��ɣ�Ҳ���ܶ�ȡʧ�� -- �������ݿ������ô���һ������
		ASSERT(0);
		IMSG(_T("\n\n\tGuild load from db maybe have problem! Please check!\n\n"));
		return;
	}

	sGuildBase = (const tagGuildBase)pGuild->GetGuildAtt();
	sGuildBase.n16MemberNum = pGuild->GetGuildMemberNum();

	tagGuildMember *pMember = pGuild->GetMember(pRole->GetID());
	if(!P_VALID(pMember))
	{
		if(pGuild->GetGuildMemberNum() > 0)
		{
			// ִ�е��ô���˵���ó�Ա�ѱ��߳�
			pRole->SetGuildID(GT_INVALID);
		}
		else
		{
			// ˵�����ɳ�Ա��ȡʧ��
			ASSERT(0);
			IMSG(_T("\n\nWarning:\n\tGuild members load failed! Please check!\n\n"));
		}
		
		return;
	}

	sGuildMember = *pMember;
}

//-------------------------------------------------------------------------------
// ɾ����ɫʱ�����������
//-------------------------------------------------------------------------------
DWORD GuildMgr::ClearRoleRemove(DWORD dwRoleID)
{
	Guild *pGuild = NULL;
	tagGuildMember *pMember = NULL;
	MapGuild::TMapIterator iter = m_mapGuild.Begin();
	while(m_mapGuild.PeekNext(iter, pGuild))
	{
		pMember = pGuild->GetMember(dwRoleID);
		if(P_VALID(pMember))
		{
			return pGuild->ClearRoleRemove(dwRoleID);
		}
	}

	return E_Success;
}

//-------------------------------------------------------------------------------
// ������Ϣ�����͵��ͻ���
//-------------------------------------------------------------------------------
VOID GuildMgr::SendGuildProcFailed2Client(PlayerSession *pSession, DWORD dwErrorCode)
{
	ASSERT(P_VALID(pSession));
	ASSERT(dwErrorCode != E_Success);

	tagNS_GuildProcFailed send;
	send.dwErrorCode = dwErrorCode;
	pSession->SendMessage(&send, send.dwSize);
}

//-------------------------------------------------------------------------------
// ����������Ϣ���͵�LoongDB
//-------------------------------------------------------------------------------
VOID GuildMgr::SendCreateGuild2DB(const tagGuild& sGuild)
{
	INT32 nNumTCHAR = sGuild.strName.size();
	INT32 nMsgSz = sizeof(tagNDBC_CreateGuild) + nNumTCHAR * sizeof(TCHAR);
	
	MCREATE_MSG(pSend, nMsgSz, NDBC_CreateGuild);
	
	pSend->sCreateGuildInfo.dwGuildID			= sGuild.dwID;
	pSend->sCreateGuildInfo.dwCreateRoleNameID	= sGuild.dwFounderNameID;

	pSend->sCreateGuildInfo.nGuildRep			= sGuild.nReputation;
	pSend->sCreateGuildInfo.nGuildFund			= sGuild.nFund;
	pSend->sCreateGuildInfo.nGuildMaterial		= sGuild.nMaterial;
	pSend->sCreateGuildInfo.nGroupPurchase		= sGuild.nGroupPurchase;

	pSend->sCreateGuildInfo.n16GuildPeace		= sGuild.n16Peace;
	pSend->sCreateGuildInfo.byGuildLevel		= sGuild.byLevel;
	pSend->sCreateGuildInfo.byAffairRemainTimes	= sGuild.byAffairRemainTimes;

	_tcscpy_s(pSend->sCreateGuildInfo.szName, nNumTCHAR + 1, sGuild.strName.c_str());
	ASSERT(_T('\0') == pSend->sCreateGuildInfo.szName[nNumTCHAR]);

	g_dbSession.Send(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}

//-------------------------------------------------------------------------------
// ��ɢ������Ϣ���͵�LoongDB
//-------------------------------------------------------------------------------
VOID GuildMgr::SendDismissGuild2DB(DWORD dwGuildID)
{
	tagNDBC_DismissGuild send;
	send.dwGuildID = dwGuildID;

	g_dbSession.Send(&send, send.dwSize);
}

//-------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------
DWORD GuildMgr::CreateGuild(Role* pCreator, LPCTSTR strGuildName, INT32 nNumTHCAR)
{
	ASSERT(P_VALID(pCreator));

	// �жϴ������Ƿ��Ѿ�����һ������
	if(pCreator->IsInGuild())
	{
		// �����ط����߼�����
		ASSERT(P_VALID(GetGuild(pCreator->GetGuildID())));

		return E_Guild_Create_AlreadyIn;
	}

	// �ȼ��Ƿ�ﵽ20��
	if(pCreator->GetLevel() < g_guildMgr.GetGuildCfg().nCreateRoleMinLevel)
	{
		return E_Guild_Create_LevelLimit;
	}

	// ��ȡ���� -- ȷ�����ַ���������
	tstring szTmpGuildName(strGuildName, nNumTHCAR);

	// �Ƿ����ѽ���������
	DWORD dwNewGuildID = g_world.LowerCrc32(szTmpGuildName.c_str());
	if(m_mapGuild.IsExist(dwNewGuildID))
	{
		return E_Guild_Create_NameExist;
	}

	// �������Ϸ����ж�
	DWORD dwErrorCode = Filter::CheckName(szTmpGuildName.c_str(), g_attRes.GetVariableLen().nGuildNameMax, 
							g_attRes.GetVariableLen().nGuildNameMin, g_attRes.GetNameFilterWords());
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	// �ֽ��Ƿ��㹻(10��)
	INT64 n64Cost = MGold2Silver(GetGuildCfg().nGoldCreateNeeded);
	if(pCreator->GetCurMgr().GetBagSilver() < n64Cost)
	{
		return E_BagSilver_NotEnough;
	}

	// ��������
	Guild *pNewGuild = CreateGuild(pCreator, szTmpGuildName, dwNewGuildID);
	if(!P_VALID(pNewGuild))
	{
		ASSERT(0);
		return GT_INVALID;
	}

	// �۳���Ǯ
	pCreator->GetCurMgr().DecBagSilver(n64Cost, ELCLD_Guild_Create);

	// ���浽map��
	m_mapGuild.Add(pNewGuild->GetGuildAtt().dwID, pNewGuild);

	// ���浽���ݿ���
	SendCreateGuild2DB(pNewGuild->GetGuildAtt());

	return E_Guild_Create_Success;
}


//-------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------
Guild* GuildMgr::CreateGuild(Role* pCreator, const tstring& strGuildName, DWORD dwNewGuildID)
{
	Guild* pNewGuild = new Guild();
	if(!P_VALID(pNewGuild))
	{
		ASSERT(0);
		return NULL;
	}

	// ��ʼ����������
	pNewGuild->InitCreate(pCreator, strGuildName, dwNewGuildID, g_guildMgr.GetGuildCfg());

	// ȫ���㲥
	tagRoleInfo *pRoleInfo = g_roleMgr.GetRoleInfo(pCreator->GetID());
	if (P_VALID(pRoleInfo))
	{
		INT32 nRoleNameCnt = _tcsclen(pRoleInfo->szRoleName);
		INT32 nMsgSz = sizeof(tagNS_GuildCreateBroad) + (strGuildName.length() + nRoleNameCnt + 1) * sizeof(TCHAR);

		MCREATE_MSG(pSend, nMsgSz, NS_GuildCreateBroad);
		pSend->dwRoleID		= pCreator->GetID();
		pSend->dwRoleNameID	= pCreator->GetNameID();
		pSend->dwGuildID	= pNewGuild->GetGuildAtt().dwID;

		_tcscpy_s(pSend->szName, nRoleNameCnt + 1, pRoleInfo->szRoleName);
		_tcscpy_s((pSend->szName + nRoleNameCnt + 1), strGuildName.length() + 1, strGuildName.c_str());

		g_roleMgr.SendWorldMsg(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
	}

	// ͬ������Χ���
	tagGuildMember* pMember = pNewGuild->GetMember(pCreator->GetID());
	Map*			pMap	= pCreator->GetMap();
	if (P_VALID(pMember) && P_VALID(pMap))
	{
		tagNS_RemoteRoleGuildInfoChange send;
		send.dwGuildID		= pNewGuild->GetGuildAtt().dwID;
		send.dwRoleID		= pCreator->GetID();
		send.n8GuildPos		= pMember->eGuildPos;

		pMap->SendBigVisTileMsg(pCreator, &send, send.dwSize);
	}

	return pNewGuild;
}

//-------------------------------------------------------------------------------
// ��ɢ����
//-------------------------------------------------------------------------------
DWORD GuildMgr::DismissGuild(Role* pLeader, DWORD dwGuildID)
{
	ASSERT(P_VALID(pLeader));
	ASSERT(pLeader->GetGuildID() == dwGuildID);
	
	// �ҵ�����
	Guild *pGuild = GetGuild(dwGuildID);
	if(!P_VALID(pGuild))
	{
		return E_Guild_NotExist;
	}

	// �ж��Ƿ����ɾ��
	DWORD dwErrorCode = pGuild->DismissGuild(pLeader);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	// ����ð��ɵ��Ź���Ϣ
	g_mall.RemoveGuildPurchaseInfo(dwGuildID);

	// ȫ���㲥
	INT32 nGuildNameCnt = pGuild->GetGuildAtt().strName.size();
	INT32 nMsgSz = sizeof(tagNS_GuildDismissBroad) + nGuildNameCnt * sizeof(TCHAR);

	MCREATE_MSG(pSend, nMsgSz, NS_GuildDismissBroad);
	pSend->dwRoleID	= pLeader->GetID();
	_tcscpy_s(pSend->szGuildName, nGuildNameCnt + 1, pGuild->GetGuildAtt().strName.c_str());
	g_roleMgr.SendWorldMsg(pSend, pSend->dwSize);
	MDEL_MSG(pSend);

	// ͬ������Χ���
	Map* pMap	= pLeader->GetMap();
	if (P_VALID(pMap))
	{
		tagNS_RemoteRoleGuildInfoChange send;
		send.dwGuildID		= GT_INVALID;
		send.dwRoleID		= pLeader->GetID();
		send.n8GuildPos		= EGMP_Null;

		pMap->SendBigVisTileMsg(pLeader, &send, send.dwSize);
	}

	// ��ɢ����
	SAFE_DEL(pGuild);
	m_mapGuild.Erase(dwGuildID);

	// ���浽���ݿ���
	SendDismissGuild2DB(dwGuildID);
	
	return E_Success;
}


//-------------------------------------------------------------------------------
// ����ɲֿ���Ӳ����¼�
//-------------------------------------------------------------------------------
VOID GuildMgr::AddGuildWareEvent(DWORD dwSender, EEventType eType, DWORD dwSize, void *pMsg)
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwSender);
	if (!P_VALID(pRole))
	{
		// ��Ҳ�����
		return;
	}

	DWORD dwGuildID = pRole->GetGuildID();
	if (!GT_VALID(dwGuildID))
	{
		// ���û�а���
		return;
	}

	Guild* pGuild = GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		// ���ɲ�����
		return;
	}
	
	pGuild->GetGuildWarehouse().AddEvent(dwSender, eType, dwSize, pMsg);
}

//-------------------------------------------------------------------------------
// ÿ�����ð�������״̬���۳�����
//-------------------------------------------------------------------------------
VOID GuildMgr::DailyGuildReset()
{
	Guild *pGuild = NULL;
	MapGuild::TMapIterator iter = m_mapGuild.Begin();
	while(m_mapGuild.PeekNext(iter, pGuild))
	{
		if (!P_VALID(pGuild))
		{
			continue;
		}
		pGuild->DailyGuildReset();
	}
}

//-------------------------------------------------------------------------------
// ���ɼ�����������
//-------------------------------------------------------------------------------
VOID GuildMgr::UpgradeGuildSkill()
{
	Guild *pGuild = NULL;
	MapGuild::TMapIterator iter = m_mapGuild.Begin();
	while(m_mapGuild.PeekNext(iter, pGuild))
	{
		if (!P_VALID(pGuild))
		{
			continue;
		}
		pGuild->GetGuildSkill().SkillUpgradeOnClock();
	}
}