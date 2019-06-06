//-------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: loong_guild.cpp
// author: Sxg
// actor:
// data: 2009-04-14
// last:
// brief: ������ش���
//-------------------------------------------------------------------
#include "StdAfx.h"

#include "Loong.h"
#include "LoongDB.h"
#include "../ServerDefine/msg_guild.h"

#include <new>

#define MGuildMemMsgSz(nNum)	((nNum - 1) * sizeof(tagGuildMemLoad) + sizeof(tagNDBS_LoadAllGuildMember))
/****************************** ����������� *******************************/
//-------------------------------------------------------------------
// �������а�������
//-------------------------------------------------------------------
DWORD CLoong::LoadAllGuild(DWORD pDBMsg, DWORD dwDummy)
{
	//MGET_MSG(p, pDBMsg, NDBC_LoadAllGuild);

	// Ԥ�ȷ���һ���㹻��Ŀռ�
	CHAR *pBuffLoad = new CHAR[MAX_GUILD_BUFF_SIZE];
	ZeroMemory(pBuffLoad, sizeof(CHAR) * MAX_GUILD_BUFF_SIZE);
	
	INT32 nGuildNum = 0;
	if(m_pDB->LoadAllGuild(pBuffLoad, nGuildNum))
	{
		MTRANS_POINTER(pGuildLoad, pBuffLoad, tagGuildLoad);
		tagNDBS_LoadAllGuild send;
		for(INT32 i=0; i<nGuildNum; ++i)
		{
			memcpy(&send.sGuildLoad, pGuildLoad + i, sizeof(tagGuildLoad));

			SendMsg( &send, send.dwSize);
		}
	}

	SAFE_DEL_ARRAY(pBuffLoad);

	return E_Success;
}

//-------------------------------------------------------------------
// ������ɳ�ʼ��
//-------------------------------------------------------------------
DWORD CLoong::CheckGuildInitOK(DWORD pDBMsg, DWORD dwDummy)
{
	tagNDBS_GuildInitOK send;

	SendMsg( &send, send.dwSize);

	return E_Success;
}

//-------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------
DWORD CLoong::CreateGuild(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_CreateGuild);

	if(!m_pDB->InsertGuild(&p->sCreateGuildInfo))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------
// ��ɢ����
//-------------------------------------------------------------------
DWORD CLoong::DismissGuild(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_DismissGuild);

	if(!m_pDB->DeleteGuild(p->dwGuildID))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------
// �޸İ�����ּ
//-------------------------------------------------------------------
DWORD CLoong::ChangeGuildTenet(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_ChangeGuildTenet);

	if(!m_pDB->UpdateGuildTenet(p->dwGuildID, p->szNewTenet))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------
// �������а�������
//-------------------------------------------------------------------
DWORD CLoong::SaveAllGuild(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_SaveAllGuild);

	if(!m_pDB->UpdateAllGuild(p->pGuildBase, p->nGuildNum))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------
// ����ָ����������
//-------------------------------------------------------------------
DWORD CLoong::SaveGuildAtt(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_SaveGuildAtt);

	if(!m_pDB->UpdateGuildAtt(&p->pGuildBase))
	{
		return GT_INVALID;
	}

	return E_Success;
}

/*************************** ���ɳ�Ա������� ***************************/

//-------------------------------------------------------------------
// �������а��ɳ�Ա
//-------------------------------------------------------------------
DWORD CLoong::LoadAllGuildMember(DWORD pDBMsg, DWORD dwDummy)
{
	//MGET_MSG(p, pDBMsg, NDBC_LoadAllGuildMember);

	// Ԥ�ȷ���һ���㹻��Ŀռ�
	CHAR *pBuffLoad = new CHAR[MAX_GUILD_BUFF_SIZE];
	ZeroMemory(pBuffLoad, sizeof(CHAR) * MAX_GUILD_BUFF_SIZE);

	INT32 nGuildMemNum = 0;
	if(m_pDB->LoadAllGuildMember(pBuffLoad, nGuildMemNum))
	{
		CHAR *pBuffSend = new CHAR[MGuildMemMsgSz(MAX_GUILDMEM_NUM_PER_MSG)];

		tagNDBS_LoadAllGuildMember *pSend = (tagNDBS_LoadAllGuildMember*)(pBuffSend);
		ZeroMemory(pBuffSend, MGuildMemMsgSz(MAX_GUILDMEM_NUM_PER_MSG));
		MMSGINIT(pSend, tagNDBS_LoadAllGuildMember);
				
		MTRANS_POINTER(pCur, pBuffLoad, tagGuildMemLoad);
		INT32 nSendLeft = nGuildMemNum;
		INT32 nSendNum	= 0;
		while(nSendLeft > 0)
		{
			pCur = &pCur[nSendNum];
			
			if(nSendLeft > MAX_GUILDMEM_NUM_PER_MSG)
			{
				nSendNum = MAX_GUILDMEM_NUM_PER_MSG;
			}
			else
			{
				nSendNum = nSendLeft;
			}
			
			memcpy(pSend->sGuildMemberLoad, pCur, nSendNum * sizeof(tagGuildMemLoad));
			pSend->nAllGuildNum	= nSendNum;
			pSend->dwSize		= MGuildMemMsgSz(nSendNum);

			SendMsg( pSend, pSend->dwSize);

			nSendLeft -= nSendNum;
		}

		SAFE_DEL_ARRAY(pBuffSend);
	}

	SAFE_DEL_ARRAY(pBuffLoad);

	// ���ɳ�Ա��ʼ������
	tagNDBS_GuildMemberInitOK send;
	SendMsg(&send, send.dwSize);

	return E_Success;
}

//-------------------------------------------------------------------
// �������
//-------------------------------------------------------------------
DWORD CLoong::JoinGuild(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_JoinGuild);
	
	if(!m_pDB->InsertGuildMember(p->sGuildMemInfo))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------
// �뿪����
//-------------------------------------------------------------------
DWORD CLoong::LeaveGuild(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LeaveGuild);

	if(!m_pDB->DeleteGuildMember(p->dwRoleID))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------
// ����ְλ�仯
//-------------------------------------------------------------------
DWORD CLoong::ChangeGuildPos(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_ChangeGuildPos);

	if(!m_pDB->UpdateGuildMemberPos(p->dwRoleID, p->n8NewPos))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// ���ɲֿ����Ȩ�ޱ��
//------------------------------------------------------------------------------
DWORD CLoong::GuildWarePrivilege(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_GuildWarePri);

	if (!m_pDB->GuildWarePrivilege(p->dwRoleID, p->bEnable))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// ָ�����ɲֿ���Ʒ��ȡ
//------------------------------------------------------------------------------
DWORD CLoong::LoadGuildWareItems(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LoadGuildWareItems);

	// �����㹻����ڴ�
	BYTE* pBuff = new BYTE[TEMP_GUILD_BUFF_SIZE];
	if (!P_VALID(pBuff))
	{
		return GT_INVALID;
	}
	ZeroMemory(pBuff, sizeof(BYTE) * TEMP_GUILD_BUFF_SIZE);

	tagNDBS_LoadGuildWareItems* pSend = (tagNDBS_LoadGuildWareItems*)pBuff;

	//����Ϣͷ
	tagNDBS_LoadGuildWareItems temp;
	pSend->dwID			= temp.dwID;
	pSend->dwSize		= temp.dwSize;
	pSend->dwGuildID	= p->dwGuildID;
	pSend->nItemNum		= GT_INVALID;

	LPVOID pTemp = (LPVOID)(pSend->byData);
	m_pDB->LoadGuildWareItems(pTemp, p->dwGuildID, pSend->nItemNum);

	if (GT_VALID(pSend->nItemNum))
	{
		// ����������Ϣ��С
		pSend->dwSize		= sizeof(tagNDBS_LoadGuildWareItems) + ((BYTE*)pTemp - pSend->byData - 1) * sizeof(BYTE);

		// ������World������
		SendMsg( pSend, pSend->dwSize);
	}

	// �ͷ�
	SAFE_DEL_ARRAY(pBuff);

	return E_Success;
}

//------------------------------------------------------------------------------
// ������µİ��ɲֿ���Ʒ
//------------------------------------------------------------------------------
DWORD CLoong::SaveGuildWareItems(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_SaveGuildWare);

	m_pDB->UpdateItemPosAndUseRel(GT_INVALID, p->byData, p->nItemNum, NULL);

	return E_Success;
}

//------------------------------------------------------------------------------
// ��ȡ���а�����ʩ������Ϣ
//------------------------------------------------------------------------------
DWORD CLoong::LoadGuildFacilities(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LoadFacilitiesInfo);

	// �����㹻����ڴ�
	BYTE* pBuff = new BYTE[TEMP_GUILD_BUFF_SIZE];
	if (!P_VALID(pBuff))
	{
		return GT_INVALID;
	}
	ZeroMemory(pBuff, sizeof(BYTE) * TEMP_GUILD_BUFF_SIZE);

	tagNDBS_LoadFacilitiesInfo* pSend = (tagNDBS_LoadFacilitiesInfo*)pBuff;

	//����Ϣͷ
	tagNDBS_LoadFacilitiesInfo temp;
	pSend->dwID			= temp.dwID;
	pSend->dwSize		= temp.dwSize;
	pSend->dwGuildID	= p->dwGuildID;

	if (!m_pDB->LoadGuildFacilities((LPVOID)pSend->sFacilitiesInfo, pSend->nInfoNum, p->dwGuildID))
	{
		SAFE_DEL_ARRAY(pBuff);
		return GT_INVALID;
	}

	// ���¼�����Ϣ��С
	pSend->dwSize = sizeof(tagNDBS_LoadFacilitiesInfo) + (pSend->nInfoNum - 1) * sizeof(tagFacilitiesLoad);

	// ������World������
	SendMsg( pSend, pSend->dwSize);

	// �ͷ��ڴ�
	SAFE_DEL_ARRAY(pBuff);

	return E_Success;
}

//------------------------------------------------------------------------------
// ���������ʩ��Ϣ
//------------------------------------------------------------------------------
DWORD CLoong::InsertGuildFacilities(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_CreateFacilities);

	if (!m_pDB->InsertGuildFacilities(&p->sFacilitiesInfo))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// ���°�����ʩ��Ϣ
//------------------------------------------------------------------------------
DWORD CLoong::UpdateGuildFacilities(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_FacilitiesUpgrade);

	if (!m_pDB->UpdateGuildFacilities(&p->sFacilitiesInfo))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// ɾ������������ʩ��Ϣ
//------------------------------------------------------------------------------
DWORD CLoong::RemoveAllFacilities(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_RemoveAllFacilities);

	if (!m_pDB->RemoveAllFacilities(p->dwGuildID))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// ���°��ɳ�Ա����
//------------------------------------------------------------------------------
DWORD CLoong::ChangeMemberContribution(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_ChangeContrib);

	if (!m_pDB->ChangeMemberContribution((LPVOID)p))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// ��ȡ���ɼ�����Ϣ
//------------------------------------------------------------------------------
DWORD CLoong::LoadGuildSkillInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LoadGuildSkillInfo);

	// �����㹻��Ŀռ�
	CHAR *pBuffLoad = new CHAR[TEMP_GUILD_BUFF_SIZE];
	ZeroMemory(pBuffLoad, sizeof(CHAR) * TEMP_GUILD_BUFF_SIZE);

	tagNDBS_LoadGuildSkillInfo* pSend = (tagNDBS_LoadGuildSkillInfo*)pBuffLoad;
	
	// ����Ϣͷ
	tagNDBS_LoadGuildSkillInfo temp;
	pSend->dwID			= temp.dwID;
	pSend->dwSize		= temp.dwSize;
	pSend->dwGuildID	= p->dwGuildID;
	
	if (!m_pDB->LoadGuildSkillInfo(pSend->sGuildSkillInfo, p->dwGuildID, pSend->nInfoNum))
	{
		SAFE_DEL_ARRAY(pBuffLoad);
		return GT_INVALID;
	}

	// ���¼�����Ϣ��С
	pSend->dwSize = sizeof(tagNDBS_LoadGuildSkillInfo) + (pSend->nInfoNum - 1) * sizeof(tagGuildSkillInfo);

	// ������World������
	SendMsg( pSend, pSend->dwSize);

	// �ͷ��ڴ�
	SAFE_DEL_ARRAY(pBuffLoad);

	return E_Success;
}

//------------------------------------------------------------------------------
// �������ɵļ�����Ϣ
//------------------------------------------------------------------------------
DWORD CLoong::InsertGuildSkillInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_CreateGuildSkill);

	if (!m_pDB->InsertGuildSkillInfo(p->dwGuildID, &p->sSkillInfo))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// ������ɼ�����Ϣ
//------------------------------------------------------------------------------
DWORD CLoong::SaveGuildSkillInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_SaveGuildSkill);

	if (!m_pDB->UpdateGuildSkillInfo(p->dwGuildID, &p->sSkillInfo))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// ���õ�ǰ�о����ɼ���
//------------------------------------------------------------------------------
DWORD CLoong::ChangeResearchGuildSkill(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_ChangeResearchSkill);

	if (!m_pDB->ChangeResearchGuildSkill(p->dwGuildID, p->dwNewSkillID))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// ɾ��ָ�����ɵļ�����Ϣ
//------------------------------------------------------------------------------
DWORD CLoong::RemoveGuildSkillInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_RemoveGuildSkill);

	if (!m_pDB->RemoveGuildSkillInfo(p->dwGuildID))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// ��ȡָ������������Ϣ
//------------------------------------------------------------------------------
DWORD CLoong::LoadGuildCommerceInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LoadCommodity);

	// �����㹻��Ŀռ�
	CHAR *pBuffLoad = new CHAR[TEMP_GUILD_BUFF_SIZE];
	ZeroMemory(pBuffLoad, sizeof(CHAR) * TEMP_GUILD_BUFF_SIZE);

	tagNDBS_LoadCommodity* pSend = (tagNDBS_LoadCommodity*)pBuffLoad;

	// ����Ϣͷ
	tagNDBS_LoadCommodity temp;
	pSend->dwID			= temp.dwID;
	pSend->dwSize		= temp.dwSize;
	pSend->dwGuildID	= p->dwGuildID;

	m_pDB->LoadGuildCommerceInfo(pSend->sCommerceInfo, p->dwGuildID, pSend->nCommodityNum);

	// ���¼�����Ϣ��С
	pSend->dwSize = sizeof(tagNDBS_LoadCommodity) + (pSend->nCommodityNum - 1) * sizeof(tagGuildCommerceInfo);

	// ������World������
	SendMsg( pSend, pSend->dwSize);

	// �ͷ��ڴ�
	SAFE_DEL_ARRAY(pBuffLoad);

	return E_Success;
}

//------------------------------------------------------------------------------
// ��ȡָ����������������Ϣ
//------------------------------------------------------------------------------
DWORD CLoong::LoadCommerceRankInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LoadCommerceRank);

	// �����㹻��Ŀռ�
	CHAR *pBuffLoad = new CHAR[TEMP_GUILD_BUFF_SIZE];
	ZeroMemory(pBuffLoad, sizeof(CHAR) * TEMP_GUILD_BUFF_SIZE);

	tagNDBS_LoadCommerceRank* pSend = (tagNDBS_LoadCommerceRank*)pBuffLoad;

	// ����Ϣͷ
	tagNDBS_LoadCommerceRank temp;
	pSend->dwID			= temp.dwID;
	pSend->dwSize		= temp.dwSize;
	pSend->dwGuildID	= p->dwGuildID;

	if (!m_pDB->LoadCommerceRankInfo(pSend->sRankInfo, p->dwGuildID, pSend->nRankNum))
	{
		SAFE_DEL_ARRAY(pBuffLoad);
		return GT_INVALID;
	}

	// ���¼�����Ϣ��С
	pSend->dwSize = sizeof(tagNDBS_LoadCommerceRank) + (pSend->nRankNum - 1) * sizeof(tagCommerceRank);

	// ������World������
	SendMsg( pSend, pSend->dwSize);

	// �ͷ��ڴ�
	SAFE_DEL_ARRAY(pBuffLoad);

	return E_Success;
}

//------------------------------------------------------------------------------
// ������������
//------------------------------------------------------------------------------
DWORD CLoong::CreateCommodityInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_CreateCommodity);

	if (!m_pDB->InsertCommodityInfo((LPVOID)p))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// ��������
//------------------------------------------------------------------------------
DWORD CLoong::UpdateCommodityTael(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_ChangeTael);

	if (!m_pDB->UpdateCommodityTael(p->dwRoleID, p->nTael))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// �����̻�
//------------------------------------------------------------------------------
DWORD CLoong::SaveCommodityInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_SaveCommodity);

	if (!m_pDB->SaveCommodityInfo(p->sGoodInfo, p->dwRoleID))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// ����̻���Ϣ
//------------------------------------------------------------------------------
DWORD CLoong::RemoveCommodityInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_RemoveCommodity);

	if (!m_pDB->RemoveCommodityInfo(p->dwRoleID))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// �������̼ν�״̬
//------------------------------------------------------------------------------
DWORD CLoong::SetCommendation(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_SetCommendation);

	if (!m_pDB->SetCommendation(p->dwGuildID, p->bCommend))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// �������߸�����������
//------------------------------------------------------------------------------
DWORD CLoong::UpdateCommerceRankInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_UpdateCommerceRank);

	if (!m_pDB->UpdateCommerceRankInfo(p->dwGuildID, &p->sRankInfo))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------
// ɾ������������Ϣ
//------------------------------------------------------------------------------
DWORD CLoong::RemoveCommerceRankInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_RemoveCommerceRank);

	if (!m_pDB->RemoveCommerceRankInfo(p->dwGuildID, p->dwRoleID))
	{
		return GT_INVALID;
	}

	return E_Success;
}