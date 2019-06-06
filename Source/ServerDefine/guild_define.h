#pragma once

#include "../WorldDefine/guild_define.h"

#pragma pack(push, 1)
//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
const INT32 MAX_GUILD_INVITE_DELAY_TIME	= TICK_PER_SECOND * 60;	// ����������Ӧʱ��
const INT32 MAX_GUILD_BUFF_SIZE			= 100 * 1000 * 1000;	// ���������ʱ����
const INT32 MAX_GUILDMEM_NUM_PER_MSG	= 10 * 1000;			// ÿ����Ϣ�а��ɳ�Ա��������
const INT32 TEMP_GUILD_BUFF_SIZE		= 100 * 1000;			// ������ʱ����

const INT32 MAX_GUILD_NAME_LEN			= 24;
const INT32 MAX_GUILD_TENET_LEN			= 400;
const INT32 MAX_GUILD_SYMBOL_LEN		= 200;

//-----------------------------------------------------------------------------
// �ṹ
//-----------------------------------------------------------------------------
// ��������
struct tagGuild: public tagGuildBase
{
	tstring				strName;						//��������
	tstring				strTenet;						//������ּ
	tstring				strSymbol;						//���ɱ�־

	tagGuild(): tagGuildBase()	{}

	VOID Init(const tstring &strGuildName, DWORD dwGuildID, DWORD dwLeaderID, 
		DWORD dwCreatorNameID, BYTE byGuildLevel, INT16 n16GuildPeace, 
		INT32 nGuildFund, INT32 nGuildMaterial, INT32 nGuildRep, INT32 nGroupPurchase)
	{
		this->strName				= strGuildName;
		this->dwID					= dwGuildID;	
		this->dwLeaderRoleID		= dwLeaderID;
		this->dwFounderNameID		= dwCreatorNameID;
		this->byLevel				= byGuildLevel;
		this->n16Peace				= n16GuildPeace;
		this->nDailyCost			= (INT32)MGuildDailyCost(this->byLevel, this->n16Peace);
		this->nFund					= nGuildFund;
		this->nMaterial				= nGuildMaterial;
		this->nReputation			= nGuildRep;
		this->nGroupPurchase		= nGroupPurchase;
		this->byAffairRemainTimes	= MGuildAffairTimes(this->byLevel);
		this->bCommendation			= FALSE;
	}
};

// ����������Ϣ
struct tagCreateGuildInfo
{
	DWORD		dwGuildID;
	DWORD		dwCreateRoleNameID;

	INT32		nGuildRep;
	INT32		nGuildFund;
	INT32		nGuildMaterial;
	INT32		nGroupPurchase;

	INT16		n16GuildPeace;
	BYTE		byGuildLevel;
	BYTE		byAffairRemainTimes;
	TCHAR		szName[1];
};

// �¼����Ա��Ϣ
struct tagCreateGuildMemInfo
{
	DWORD		dwRoleID;
	DWORD		dwGuildID;
	INT8		n8GuildPos;
};

// ������
struct tagInvite
{
	DWORD		dwInviterID;	// ������id
	INT32		nInviteCountDown;

	tagInvite(DWORD dwInviterID)
	{
		this->dwInviterID		= dwInviterID;
		this->nInviteCountDown	= MAX_GUILD_INVITE_DELAY_TIME;
	}
};

//-----------------------------------------------------------------------------
// ��ʼ���Ӻ󣬶���Ľṹ
//-----------------------------------------------------------------------------
struct tagGuildLoad: public tagGuildBase
{
	TCHAR			szName[MAX_GUILD_NAME_LEN];			// ��������
	TCHAR			szTenet[MAX_GUILD_TENET_LEN];		// ������ּ
	TCHAR			szSymbolURL[MAX_GUILD_SYMBOL_LEN];	// ���ɱ�־
};

struct tagGuildMemLoad
{
	DWORD			dwGuildID;
	tagGuildMember	sGuildMember;
};

//-----------------------------------------------------------------------------
// ������ʩ��������ṹ
//-----------------------------------------------------------------------------
struct tagItemNeedInfo 
{
	DWORD	dwItemTypeID;
	INT32	nItemNeedNum;

	BOOL IsValid()
	{
		return GT_VALID(dwItemTypeID) && GT_VALID(nItemNeedNum);
	}
};

struct tagGuildUpgradeNeed
{
	DWORD					dwKey;
	INT16					n16FullFill;
	INT16					n16Step;
	INT32					nFund;
	INT32					nMaterial;
	INT32					nBaseExploit;
	TList<tagItemNeedInfo>	listItemInfo;

	tagGuildUpgradeNeed()
	{
		n16FullFill		= 0;
		n16Step			= 0;
		nFund			= 0;
		nMaterial		= 0;
		nBaseExploit	= 0;
		listItemInfo.Clear();
	}
};

//-----------------------------------------------------------------------------
// ������ʩ�����ýṹ
//-----------------------------------------------------------------------------
struct tagFacilitiesLoad
{
	EFacilitiesType	eType;
	DWORD			dwGuildID;
	DWORD			dwItemTypeID[MAX_UPGRADE_NEED_ITEM_TYPE];
	INT32			nItemNeed[MAX_UPGRADE_NEED_ITEM_TYPE];
	INT16			n16Progress;
	BYTE			byLevel;

	tagFacilitiesLoad()
	{
		ZeroMemory(this, sizeof(*this));
	}

	tagFacilitiesLoad& operator= (tagGuildFacilitiesInfo& sInfo)
	{
		eType		= sInfo.eType;
		byLevel		= sInfo.byLevel;
		n16Progress	= sInfo.nProgress;

		for (int n=0; n<MAX_UPGRADE_NEED_ITEM_TYPE; n++)
		{
			dwItemTypeID[n]	= sInfo.dwItemID[n];
			nItemNeed[n]	= sInfo.nNeedNum[n];
		}

		return *this;
	}
};

//-----------------------------------------------------------------------------
// ����������Ϣ
//-----------------------------------------------------------------------------
struct tagGuildAffairInfo 
{
	DWORD		dwBuffID;
	INT32		nFund;
	INT32		nMaterial;
	BYTE		byGuildLevel;
	BYTE		byHoldCity;
};

//-----------------------------------------------------------------------------
// ���ɼ�����Ϣ
//-----------------------------------------------------------------------------
struct tagGuildSkillInfo
{
	DWORD		dwSkillID;
	INT			nLevel;
	BOOL		bResearching;	// ֻ������ʱʹ��
	INT16		n16Progress;
};

//-----------------------------------------------------------------------------
// ����������Ϣ
//-----------------------------------------------------------------------------
struct tagRedoundInfo
{
	INT32	nExp;			// ���齱��
	INT32	nContribution;	// ���ɹ��׽���
	INT32	nExploit;		// ������������(δʵ��)
};

struct tagCommerceInfo		// ��������̬��Ϣ
{
	DWORD			dwID;	// �ȼ���Χ
	tagTaelInfo		sTaelInfo;
	tagRedoundInfo	sRedoundInfo;
};

struct tagGuildCommerceInfo 
{
	DWORD				dwRoleID;
	INT					nLevel;
	INT32				nTael;
	tagCommerceGoodInfo	sGoodInfo[MAX_COMMODITY_CAPACITY];
};

//-----------------------------------------------------------------------------
// �����þ�̬��Ϣ
//-----------------------------------------------------------------------------
struct tagCommodityProto 
{
	DWORD				dwGoodID;
	INT32				nLowPrice;
	INT32				nHighPrice;
	BYTE				byRefreshNum;
	BYTE				byHolderID;
};

struct tagCofCProto 
{
	INT64				n64Key;
	FLOAT				fProfit;
};

struct tagCofCSPProto
{
	DWORD				dwCofCID;
	BYTE				byHolderID;
};

//-----------------------------------------------------------------------------
#pragma pack(pop)