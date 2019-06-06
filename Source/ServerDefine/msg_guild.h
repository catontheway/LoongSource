//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_guild.h
// author: 
// actor:
// data: 2009-04-13
// last:
// brief: �������
//-----------------------------------------------------------------------------
#pragma once

#include "guild_define.h"

#pragma pack(push, 1)
//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
CONST INT32 MAX_LOAD_GUILDMEMBER_NUM_ONCE			= 1000;		// һ������෵�ذ��ɳ�Ա����

//-----------------------------------------------------------------------------
// ���������Ӻ󣬶�ȡ�������
//-----------------------------------------------------------------------------
CMD_START(NDBC_LoadAllGuild)
CMD_END

CMD_START(NDBS_LoadAllGuild)	// ÿ�η���һ��������Ϣ
	tagGuildLoad	sGuildLoad;
CMD_END

CMD_START(NDBC_LoadAllGuildMember)
CMD_END

CMD_START(NDBS_LoadAllGuildMember)
	INT				nAllGuildNum;
	tagGuildMemLoad	sGuildMemberLoad[1];
CMD_END

CMD_START(NDBS_GuildMemberInitOK)
CMD_END

CMD_START(NDBC_GuildInitOK)
CMD_END

CMD_START(NDBS_GuildInitOK)
CMD_END

//-----------------------------------------------------------------------------
// �ر���Ϸ������ʱ������İ�������
//-----------------------------------------------------------------------------
CMD_START(NDBC_SaveAllGuild)
	INT				nGuildNum;
	tagGuildBase	pGuildBase[1];
CMD_END

//-----------------------------------------------------------------------------
// ���Ա��ʱ����İ�������
//-----------------------------------------------------------------------------
CMD_START(NDBC_SaveGuildAtt)
	tagGuildBase	pGuildBase;
CMD_END

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
CMD_START(NDBC_CreateGuild)		// ��������
	tagCreateGuildInfo	sCreateGuildInfo;
CMD_END

CMD_START(NDBS_CreateGuild)		// �������� -- ���ݿⴴ��ʧ�ܲŻ᷵��//??��δ����
	DWORD		dwErrorCode;	
	DWORD		dwGuildID;
CMD_END

CMD_START(NDBC_DismissGuild)
	DWORD		dwGuildID;
CMD_END

CMD_START(NDBC_ChangeGuildTenet)	// �޸İ�����ּ
	DWORD		dwGuildID;
	TCHAR		szNewTenet[1];
CMD_END


//-----------------------------------------------------------------------------
// ���ɳ�Ա
//-----------------------------------------------------------------------------
CMD_START(NDBC_JoinGuild)			// �������
	tagCreateGuildMemInfo	sGuildMemInfo;
CMD_END

CMD_START(NDBC_LeaveGuild)			// �뿪���ɻ���
	DWORD		dwRoleID;
CMD_END

CMD_START(NDBC_ChangeGuildPos)		// ����ְλ�ı�
	DWORD		dwRoleID;
	INT8		n8NewPos;
CMD_END

CMD_START(NDBC_ChangeContrib)		// ���ɹ��׸ı�
	DWORD		dwRoleID;
	INT32		nContribution;		// �仯�����
	INT32		nTotalContribution;
CMD_END

CMD_START(NDBC_ChangeExploit)
	DWORD		dwRoleID;
	INT32		nExploit;
CMD_END

//CMD_START(NDBC_ChangeGuildExploit)	// ���ɹ�ѫ�ı�
//	DWORD		dwRoleID;
//	INT32		nNewExploit;
//CMD_END
//
//CMD_START(NDBC_ChangeGuildSalary)	// ���ɹ��ʸı�
//	DWORD		dwRoleID;
//	INT32		nNewSalary;
//CMD_END

//-----------------------------------------------------------------------------
// ���ɲֿ�
//-----------------------------------------------------------------------------
CMD_START(NDBC_LoadGuildWareItems)
	DWORD			dwGuildID;
CMD_END

CMD_START(NDBS_LoadGuildWareItems)
	DWORD			dwGuildID;
	INT				nItemNum;
	BYTE			byData[1];
CMD_END

CMD_START(NDBC_GuildWarePri)		// ���ɲֿ�Ȩ�ޱ��
	DWORD			dwRoleID;
	BOOL			bEnable;
CMD_END

CMD_START(NDBC_SaveGuildWare)
	INT				nItemNum;
	BYTE			byData[1];
CMD_END

//-----------------------------------------------------------------------------
// ������ʩ
//-----------------------------------------------------------------------------
CMD_START(NDBC_LoadFacilitiesInfo)
	DWORD				dwGuildID;
CMD_END

CMD_START(NDBS_LoadFacilitiesInfo)
	DWORD				dwGuildID;
	INT32				nInfoNum;
	tagFacilitiesLoad	sFacilitiesInfo[1];
CMD_END

CMD_START(NDBC_CreateFacilities)
	tagFacilitiesLoad	sFacilitiesInfo;
CMD_END

CMD_START(NDBC_FacilitiesUpgrade)
	tagFacilitiesLoad	sFacilitiesInfo;
CMD_END

CMD_START(NDBC_RemoveAllFacilities)
	DWORD				dwGuildID;
CMD_END

//-----------------------------------------------------------------------------
// ���ɼ���
//-----------------------------------------------------------------------------
CMD_START(NDBC_LoadGuildSkillInfo)
	DWORD				dwGuildID;
CMD_END

CMD_START(NDBS_LoadGuildSkillInfo)
	DWORD				dwGuildID;
	INT					nInfoNum;
	tagGuildSkillInfo	sGuildSkillInfo[1];
CMD_END

CMD_START(NDBC_CreateGuildSkill)
	DWORD				dwGuildID;
	tagGuildSkillInfo	sSkillInfo;
CMD_END

CMD_START(NDBC_SaveGuildSkill)
	DWORD				dwGuildID;
	tagGuildSkillInfo	sSkillInfo;
CMD_END

CMD_START(NDBC_ChangeResearchSkill)
	DWORD				dwGuildID;
	DWORD				dwNewSkillID;
CMD_END

CMD_START(NDBC_RemoveGuildSkill)
	DWORD				dwGuildID;
CMD_END

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
CMD_START(NDBC_LoadCommodity)
	DWORD					dwGuildID;
CMD_END

CMD_START(NDBS_LoadCommodity)
	DWORD					dwGuildID;
	INT						nCommodityNum;
	tagGuildCommerceInfo	sCommerceInfo[1];
CMD_END

CMD_START(NDBC_LoadCommerceRank)
	DWORD					dwGuildID;
CMD_END

CMD_START(NDBS_LoadCommerceRank)
	DWORD					dwGuildID;
	INT						nRankNum;
	tagCommerceRank			sRankInfo[1];
CMD_END

CMD_START(NDBC_CreateCommodity)		// ��ʼ�̻�����Ϊ��
	DWORD				dwGuildID;
	DWORD				dwRoleID;
	INT					nLevel;
	INT32				nTael;
CMD_END

CMD_START(NDBC_ChangeTael)
	DWORD				dwRoleID;
	INT32				nTael;
CMD_END

CMD_START(NDBC_SaveCommodity)
	DWORD				dwRoleID;
	tagCommerceGoodInfo	sGoodInfo[MAX_COMMODITY_CAPACITY];
CMD_END

CMD_START(NDBC_RemoveCommodity)
	DWORD				dwRoleID;
CMD_END

CMD_START(NDBC_SetCommendation)
	DWORD				dwGuildID;
	BOOL				bCommend;
CMD_END

CMD_START(NDBC_UpdateCommerceRank)
	DWORD				dwGuildID;
	tagCommerceRank		sRankInfo;
CMD_END

CMD_START(NDBC_RemoveCommerceRank)
	DWORD				dwGuildID;
	DWORD				dwRoleID;
CMD_END

//-----------------------------------------------------------------------------
#pragma pack(pop)