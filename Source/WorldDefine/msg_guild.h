//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_guild.h
// author: 
// actor:
// data: 2009-04-03
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ -- �������
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "guild_define.h"

#pragma pack(push, 1)
//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum
{
	
};

//----------------------------------------------------------------------------
// ��������
//----------------------------------------------------------------------------
CMD_START(NC_GuildCreate)
	DWORD		dwNPCID;
	TCHAR		szGuildName[1];
CMD_END

CMD_START(NS_GuildCreateBroad)	// ȫ��
	DWORD		dwRoleID;		// ������id
	DWORD		dwRoleNameID;	// ����������id
	DWORD		dwGuildID;
	TCHAR		szName[1];		// ��ɫ��+������
CMD_END

//----------------------------------------------------------------------------
// ��ɢ����
//----------------------------------------------------------------------------
CMD_START(NC_GuildDismiss)
	DWORD		dwNPCID;
CMD_END

CMD_START(NS_GuildDismissBroad)	// ȫ��
	DWORD		dwRoleID;
	TCHAR		szGuildName[1];
CMD_END

//----------------------------------------------------------------------------
// �������
//----------------------------------------------------------------------------
CMD_START(NC_GuildJoinReq)		// �������Ŀ��������
	DWORD		dwDstRoleID;
CMD_END

CMD_START(NS_GuildJoinReq)		// �������������͸�Ŀ�����
	DWORD		dwSrcRoleID;
	DWORD		dwGuildID;
CMD_END

CMD_START(NC_GuildJoinReqRes)	// Ŀ����ҷ���������
	DWORD		dwErrorCode;
	DWORD		dwDstRoleID;
	DWORD		dwDstGuildID;
CMD_END

CMD_START(NS_GuildJoinReqRes)	// ������ת����Ŀ����ҷ���
	DWORD		dwErrorCode;
	DWORD		dwInviterID;
	DWORD		dwInviteeID;
CMD_END

CMD_START(NS_GuildJoinBroad)	// ������
	DWORD		dwRoleID;
	DWORD		dwRoleNameID;
	TCHAR		szRoleName[1];
CMD_END

//------------------------------------------------------------------------------
// ����뿪����
//------------------------------------------------------------------------------
CMD_START(NC_GuildLeave)
CMD_END

CMD_START(NS_GuildLeaveBroad)
	DWORD		dwSrcRoleID;
CMD_END

//------------------------------------------------------------------------------
// ������ҳ���
//------------------------------------------------------------------------------
CMD_START(NC_GuildKick)
	DWORD		dwRoleID;
CMD_END

CMD_START(NS_GuildKickBroad)
	DWORD		dwDstRoleID;
CMD_END

//------------------------------------------------------------------------------
// �ƽ�����
//------------------------------------------------------------------------------
CMD_START(NC_GuildTurnover)
	DWORD		dwRoleID;
CMD_END

CMD_START(NS_GuildTurnoverBroad)
	DWORD		dwSrcRoleID;
	DWORD		dwDstRoleID;
CMD_END

//------------------------------------------------------------------------------
// ��ȥְ��
//------------------------------------------------------------------------------
CMD_START(NC_GuildDemiss)
CMD_END

CMD_START(NS_GuildDemissBroad)
	DWORD		dwRoleID;
	INT8		n8OldGuildPos;
CMD_END

//------------------------------------------------------------------------------
// �������ְλ
//------------------------------------------------------------------------------
CMD_START(NC_GuildAppoint)
	DWORD			dwRoleID;
	EGuildMemberPos	ePos;
CMD_END

CMD_START(NS_GuildAppointBroad)
	DWORD			dwSrcRoleID;
	DWORD			dwDstRoleID;
	EGuildMemberPos	ePos;
CMD_END

//------------------------------------------------------------------------------
// �޸���ּ
//------------------------------------------------------------------------------
CMD_START(NC_GuildTenetChange)
	TCHAR			szTenet[1];
CMD_END

CMD_START(NS_GuildTenetChangeBroad)
	DWORD			dwRoleID;
	TCHAR			szTenet[1];
CMD_END

//------------------------------------------------------------------------------
// ����ʧ�ܷ���
//------------------------------------------------------------------------------
CMD_START(NS_GuildProcFailed)
	DWORD			dwErrorCode;
CMD_END

//----------------------------------------------------------------------------
// ͬ��������Ϣ
//----------------------------------------------------------------------------
CMD_START(NC_SyncGuildInfo)
CMD_END

CMD_START(NS_SyncGuildInfo)
	tagGuildBase	sGuildInfo;
CMD_END

//----------------------------------------------------------------------------
// ��ȡ�������г�Ա
//----------------------------------------------------------------------------
CMD_START(NC_GuildGetAllMembers)
CMD_END

CMD_START(NS_GuildGetAllMembers)
	DWORD		dwErrorCode;
	INT16		n16Num;
	BYTE		byData[1];		// tagGuildMemInfo
CMD_END

//----------------------------------------------------------------------------
// ��ȡ����ָ����Ա��չ��Ϣ
//----------------------------------------------------------------------------
CMD_START(NC_GuildGetMemberEx)
	DWORD		dwRoleID;
CMD_END

CMD_START(NS_GuildGetMemberEx)
	DWORD				dwRoleID;
	tagGuildMemInfoEx	sGuildMemInfoEx;
CMD_END

//----------------------------------------------------------------------------
// ˢ�°���ָ����Ա��չ��Ϣ
//----------------------------------------------------------------------------
CMD_START(NC_GuildRefMember)
	DWORD		dwRoleID;
CMD_END

CMD_START(NS_GuildRefMember)
	tagGuildMemInfo		sGuildMemInfo;
	tagGuildMemInfoEx	sGuildMemInfoEx;
CMD_END

//------------------------------------------------------------------------------
// ���ɳ�Ա���ױ��
//------------------------------------------------------------------------------
CMD_START(NS_GuildContribution)
	DWORD		dwRoleID;
	INT32		nContribution;
	INT32		nTotalContribution;
CMD_END

//------------------------------------------------------------------------------
// ���ɳ�Ա��ѫ���
//------------------------------------------------------------------------------
CMD_START(NS_GuildExploit)
	DWORD		dwRoleID;
	INT32		nExploit;
CMD_END

//------------------------------------------------------------------------------
// ��ȡ��������
//------------------------------------------------------------------------------
CMD_START(NC_GetGuildName)
	DWORD		dwGuildID;
CMD_END

CMD_START(NS_GetGuildName)
	DWORD		dwGuildID;
	TCHAR		szGuildName[1];
CMD_END

CMD_START(NC_GetSomeGuildName)
INT		nUserData;
INT		nNum;		// id������
DWORD	dwAllID[1];	// �䳤����,���dwNum��id
CMD_END

// ���ID�����ֶ�Ӧ�Ľṹ
struct tagGuildIDName
{
	DWORD dwID;
	TCHAR szName[X_SHORT_NAME];
};

CMD_START(NS_GetSomeGuildName)
INT				nUserData;
INT				nNum;				// ��������
tagGuildIDName	IdName[1];
CMD_END

//-----------------------------------------------------------------------------
// ��ȡ������ּ��ֻ�û�ȡ�Լ����ɵģ�
//-----------------------------------------------------------------------------
CMD_START(NC_GetGuildTenet)
CMD_END

CMD_START(NS_GetGuildTenet)
	TCHAR		szGuildTenet[1];
CMD_END

//-----------------------------------------------------------------------------
// ��ȡ���ɲֿ���Ʒ��Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_GetGuildWare)
	DWORD		dwNPCID;
	DWORD		dwLastUpdateTime;
CMD_END

CMD_START(NS_GetGuildWare)
	DWORD		dwErrorCode;
	DWORD		dwLastUpdateTime;
	INT16		nSzGuildWare;
	INT			nItemNum;
	BYTE		byData[1];
CMD_END

//-----------------------------------------------------------------------------
// ��ȡ�������ɲֿ�Ȩ���б�
//-----------------------------------------------------------------------------
CMD_START(NC_GetGuildWarePriList)
	DWORD		dwNPCID;
CMD_END

CMD_START(NS_GetGuildWarePriList)
	INT32			nNum;				// �б�������
	tagGuildWarePri	sGuildWarePri[1];	// Ȩ����Ϣ
CMD_END

//-----------------------------------------------------------------------------
// ����������ɲֿ�Ȩ��
//-----------------------------------------------------------------------------
CMD_START(NC_GuildWarePri)
	DWORD		dwNPCID;
	DWORD		dwRoleID;
	BOOL		bCanUse;
CMD_END

CMD_START(NS_GuildWarePri)	//Board??
	DWORD		dwRoleID;
	BOOL		bCanUse;
CMD_END

//-----------------------------------------------------------------------------
// ����״̬�ı�
//-----------------------------------------------------------------------------
CMD_START(NS_GuildStateSet)
	EGuildSpecState	eState;
CMD_END

CMD_START(NS_GuildStateUnset)
	EGuildSpecState	eState;
CMD_END

//-----------------------------------------------------------------------------
// ��ȡ������ʩ������Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_GetGuildUpgradeInfo)	//??�����Ƿ���Ҫ��ʱ����»���
	DWORD					dwNPCID;
CMD_END

CMD_START(NS_GetGuildUpgradeInfo)	// ��ȡ���а�����ʩ��Ϣ
	tagGuildFacilitiesInfo	sFacilitiesInfo[MAX_GUILD_FACILITIES_TYPE];
CMD_END

CMD_START(NS_UpdateFacilitiesInfo)	// ����ĳһ��������ʩ��Ϣ
	tagGuildFacilitiesInfo	sFacilitiesInfo;
CMD_END

//-----------------------------------------------------------------------------
// �Ͻɰ�����ʩ����������Ʒ
//-----------------------------------------------------------------------------
CMD_START(NC_HandInItems)
	DWORD					dwNPCID;
	EFacilitiesType			eType;
CMD_END

//-----------------------------------------------------------------------------
// �㲥��ʩ������Ϣ
//-----------------------------------------------------------------------------
CMD_START(NS_GuildUpgrade)
	EFacilitiesType			eType;
	BYTE					byNewLevel;
CMD_END

//-----------------------------------------------------------------------------
// �������񷢲���Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_SpreadGuildAffair)
	DWORD					dwBuffID;
CMD_END

CMD_START(NS_SpreadGuildAffair)
	DWORD					dwRole;			// ������
	DWORD					dwBuffID;
	BYTE					byRemainTimes;	// ÿ��ʣ�෢������
CMD_END

//-----------------------------------------------------------------------------
// ���ɼ�����Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_GetGuildSkillInfo)
CMD_END

CMD_START(NS_GetGuildSkillInfo)
	DWORD			dwCurSkillID;		// ��ǰ�о�����
	INT16			n16Progress;		// ��ǰ�о�����
	INT				nSkillNum;			// ���ɼ�������
	DWORD			dwSkillInfo[1];		// ���ɼ��ܵȼ�
CMD_END

CMD_START(NC_GuildSkillUpgrade)
	INT64			n64ItemSerial;		// �Ͻɵ伮64λID
CMD_END

CMD_START(NS_GuildSkillUpgrade)			// ��ǰ�о������Ƿ����ͨ��NS_SetResearchSkill����
	DWORD			dwSkillID;			// ��������ID
	INT16			n16Progress;		// ��������
CMD_END

CMD_START(NS_GuildSkillLevelUp)
	DWORD			dwSkillID;
CMD_END

CMD_START(NC_LearnGuildSkill)
	DWORD			dwSkillID;			// ��ѧ��������ID
CMD_END

CMD_START(NS_LearnGuildSkill)
	DWORD			dwErrorCode;		// �����뼼�ܴ�����ö���ص�,�������ô���Ϣ
	DWORD			dwSkillID;			// ѧϰ���ܵ�ID
CMD_END

CMD_START(NC_SetResearchSkill)
	DWORD			dwSkillID;			// ���õ�ǰ�о�����,ȡ���о�ΪGT_INVALID
CMD_END

CMD_START(NS_SetResearchSkill)
	DWORD			dwSkillID;			// ���ص�ǰ�о�����ID
	INT16			n16Progress;		// ��������
CMD_END

//-----------------------------------------------------------------------------
// �����������
//-----------------------------------------------------------------------------
CMD_START(NC_GetCommodityInfo)			// ��ȡ����̻���Ϣ
CMD_END

CMD_START(NS_GetCommodityInfo)			// (����Ҳ�ô���Ϣ)
	INT32				nCurTael;		// ��ǰ��������
	INT					nLevel;			// ��ȡ����ʱ�ĵȼ�
	INT					nGoodNum;		// �̻�����
	tagCommerceGoodInfo	sGoodInfo[1];	// �̻���Ϣ
CMD_END

CMD_START(NC_GetTaelInfo)				// ��ȡ���̳�ʼ��Ϣ
CMD_END

CMD_START(NS_GetTaelInfo)
	INT				nLevel;				// ��ȡʱ��ҵȼ�
CMD_END

CMD_START(NC_GetCommerceRank)			// ��ȡ��������
CMD_END

CMD_START(NS_GetCommerceRank)
	BOOL			bCommend;			// �ν�״̬
	INT				nRankNum;			// �ϰ�����
	tagCommerceRank	sRankInfo[1];		// ������Ϣ
CMD_END

CMD_START(NC_AcceptCommerce)			// ��������
CMD_END

CMD_START(NS_AcceptCommerce)
	INT32			nBeginningTael;		// ��ʼ����
CMD_END

CMD_START(NC_CompleteCommerce)			// �Ͻ�����
CMD_END

CMD_START(NS_CompleteCommerce)			// �Ͻɳɹ�(�㲥?)
	DWORD			dwRoleID;			// �Ͻ���
	INT32			nFund;				// �����ʽ�
CMD_END

CMD_START(NC_AbandonCommerce)			// ��������
CMD_END

CMD_START(NS_AbandonCommerce)			// �ɹ�����
CMD_END

CMD_START(NC_SwitchCommendation)		// �������̼ν�
	BOOL			bSwitchON;
CMD_END

CMD_START(NS_SwitchCommendation)		// �㲥�ν�״̬
	DWORD			dwRoleID;			// ������
	BOOL			bSwitchON;			// �ν�״̬
CMD_END

//-----------------------------------------------------------------------------
// �����̻�
//-----------------------------------------------------------------------------
CMD_START(NC_GetCofCInfo)				// ��ȡ�̻��̻���Ϣ
	DWORD				dwNPCID;		// �̻�ID
CMD_END

CMD_START(NS_GetCofCInfo)				// (����Ҳ�ô���Ϣ)
	DWORD				dwCofCID;		// �̻�ID
	BYTE				byHoldCity;		// �ز��̻�(0��ʾһ���̻�)
	INT					nGoodNum;		// �����̻�����
	tagCommerceGoodInfo	sGoodInfo[1];	// �̻���Ϣ
CMD_END

CMD_START(NC_CloseCofC)					// �ر��̻�
	DWORD				dwNPCID;		// �̻�NPCID
CMD_END

CMD_START(NC_BuyCofCGoods)
	DWORD			dwNPCID;			// �̻�NPCID
	DWORD			dwGoodID;			// �̻�ID
	BYTE			byBuyNum;			// ��������
CMD_END

CMD_START(NC_SellCofCGoods)
	DWORD			dwNPCID;
	DWORD			dwGoodID;
	BYTE			bySellNum;
CMD_END

//------------------------------------------------------------------------------
#pragma pack(pop)