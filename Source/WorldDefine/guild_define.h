#pragma once
#include "msg_common_errorcode.h"
#include "RoleDefine.h"
#include "ItemDefine.h"
#include <bitset>

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// ��
//-----------------------------------------------------------------------------

// ��ռ���������=�����ɵȼ�+3��/4
#define MGuildMaxHoldCityNum(byGuildLevel)		((byGuildLevel + 3) / 4)

// ����Ա��
#define MGuildMaxMember(byGuildLevel)			(50 + 25 * (byGuildLevel - 1))

// �ʽ�
#define MGuildMaxFund(byGuildLevel)				(0)//??

// �ʲ�
#define MGuildMaxMaterial(byGuildLevel)			(0)//??

// ÿ��ά������(ά������=1000��(���ɵȼ�^3)����2-���ɰ�����/10000��)
#define MGuildDailyCost(byGuildLevel, nGuildPeace)	(1000.0f * byGuildLevel * byGuildLevel * byGuildLevel * (2.0f - nGuildPeace / 10000.0f))

// ÿ�հ���������
#define MGuildPeaceCost(byGuildLevel)			(byGuildLevel * 10)

// ��ǰٺ»��׼
#define MGuildMemSalary()						(0)//??

// �� -- ����
#define MGuildPosTangZhongNum(byGuildLevel)		(4 + 2 * (byGuildLevel - 1))

// ���� -- ��Ӣ����
#define MGuildPosJingZhongNum(byGuildLevel)		(5 + 2 * (byGuildLevel - 1))

// ���� -- ��ͨ���� = ���������� - ��ǰ���ɳ�Ա����

// ���ɲֿ⵱ǰ�ռ�
#define MGuildWareCurSpace(byGuildLevel)		((byGuildLevel + 1) / 2 * 35)

// ÿ�հ��񷢲�������
#define MGuildAffairTimes(byGuildLevel)			((byGuildLevel + 2) / 3)

// ���ɵȼ���������
#define MGuildSkillLevel(byAcademyLevel)		(20 + (2 * byAcademyLevel))

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------

// �ͻ�����ʾ��Ա��Ϣҳһҳ��ʾ�ĳ�Ա����
const INT32 MAX_GUILDMEM_DISPLAY_PERPAGE	= 26;

// ������֯���Գ���
const INT32 MAX_GUILD_LEVEL			=	10;										// �������ȼ�	
const INT32 MAX_GUILD_HOLDCITY		=	MGuildMaxHoldCityNum(MAX_GUILD_LEVEL);	// �������Ͻ������
const INT32 MAX_GUILD_WARE_SPACE	=	MGuildWareCurSpace(MAX_GUILD_LEVEL);	// ���ɲֿ��������
const INT32 MAX_GUILD_NAME			=	20;										// ������������ֽ���
const INT32 MAX_GUILD_TENET			=	400;									// ������ּ����ֽ���

const INT32 MAX_GUILD_FUND				=	99999999;		// �����ʽ�
const INT32 MAX_GUILD_MATERIAL			=	999999;			// �����ʲ�
const INT16 MAX_GUILD_PEACE				=	9999;			// ���ɰ�����
const INT32 MAX_GUILD_REP				=	999999999;		// ��������
const INT32 MAX_GUILD_GROUP_PURCHASE	=	100000;			// �����Ź�ָ��


// ���ɸ������Գ���
const INT32 MAX_GUILDMEM_TOTALCONTRIB	=	999999999;	// �ۼư��ɹ���
const INT32 MAX_GUILDMEM_CURCONTRIB		=	999999999;	// ��ǰ���ɹ���
const INT32 MAX_GUILDMEM_EXP			=	999999999;	// ��������
const INT32 MAX_GUILDMEM_EXPLOIT		=	999999999;	// ���ɹ�ѫ


// ����ְλ��������
const INT32 MAX_GUILDPOS_BANGZHU		=	1;	// ����
const INT32 MAX_GUILDPOS_FUBANGZHU		=	3;	// ������
const INT32 MAX_GUILDPOS_HONGHUFA		=	1;	// ���»���
const INT32 MAX_GUILDPOS_ZIHUFA			=	1;	// ��������
const INT32 MAX_GUILDPOS_TANGZHU		=	1;	// ����
const INT32 MAX_GUILDPOS_XIANGZHU		=	1;	// ����


const INT MAX_UPGRADE_NEED_ITEM_TYPE	=	4;  // ������ʩ����������Ʒ����
const INT MIN_GUILD_UPGRADE_ROLE_LEVEL	=	20; // ������ʩ������ɫ�ȼ�����

// ���ɼ������ȼ�
const INT MAX_GUILD_SKILL_LEVEL			=	40;

// ������س���
const INT MAX_COMMODITY_CAPACITY		=	10;		// ����������������
const INT MAX_COFC_GOODS_NUM			=	5;		// �̻������Ʒ�������
const INT MAX_COMMODITY_GOOD_NUM		=	255;	// �̻����ѵ�����
const INT MAX_COMMERCE_RANK_INFO		=	10;		// �������а��б���
const INT MAX_COMMEND_PLAYER			=	3;		// �������̼ν�����

//-----------------------------------------------------------------------------
// ö��
//-----------------------------------------------------------------------------

// ������
enum
{
	E_Guild_TangZhu_Full				= 1,	// ָ��ְλ�ϳ�Ա�ѵ�����
	E_Guild_XiangZhu_Full				= 2,	// ָ��ְλ�ϳ�Ա�ѵ�����
	E_Guild_TangMember_Full				= 3,	// ָ��ְλ�ϳ�Ա�ѵ�����
	E_Guild_Member_Full					= 4,	// ���ɳ�Ա�ѵ�����
	E_Guild_Pos_Full					= 5,	// ָ��ְλ�ϳ�Ա�ѵ�����

	E_Guild_Power_NotEnough				= 10,	// Ȩ�޲���
	E_Guild_CanNot_Leave				= 11,	// �����������
	E_Guild_CanNot_Demiss				= 12,	// ���ܹ���ְ

	E_Guild_Join_AlreadyIn				= 20,	// �Ѽ������
	E_Guild_Join_LevelLimit				= 21,	// �ȼ�����
	E_Guild_Join_BeInvited				= 22,	// �Ѵ��ڱ�����״̬
	E_Guild_Join_NotBeInvited			= 23,	// ��δ���ڱ�����״̬
	E_Guild_Join_BeRefused				= 24,	// ���뱻�ܾ�

	E_Guild_Create_LevelLimit			= 25,	// �ȼ�����
	E_Guild_Create_NameExist			= 26,	// ����
	E_Guild_Create_Success				= 27,	// �����ɹ�
	E_Guild_Create_AlreadyIn			= 28,	// ���а���
	
	E_GuildSet_Tenet_Empty				= 30,	// ������ּΪ��

	E_Guild_Appoint_SamePos				= 35,	// ��ְǰ��ְλ��ͬ
	E_Guild_Appoint_SexLimit			= 36,	// �Ա�Ҫ�󲻷�

	E_Guild_NotExist					= 40,	// ָ�����ɲ�����
	E_Guild_MemberHasMore				= 41,	// ����������Ա(��Ա����>1)
	E_Guild_MemberNotIn					= 42,	// û���ҵ�ָ����Ա
	E_Guild_Level_Limit					= 43,	// ���ɵȼ�����
	E_Guild_State_Limit					= 44,	// ����״̬����
	E_Guild_Fund_NotEnough				= 45,	// �����ʽ���
	E_Guild_Material_NotEnough			= 46,	// �����ʲĲ���
	E_Guild_HoldCity_Limit				= 47,	// ����ռ���������

	E_GuildWare_NoChange				= 50,	// ���ɲֿ��ޱ仯,����Ҫ����
	E_GuildWare_Power_NotEnough			= 51,	// ���ɲֿ����Ȩ�޲���

	E_GuildUpgrade_Level_Limit			= 60,	// ��ʩ�ﵽ�ȼ�����
	E_GuildUpgrade_Item_NotEnough		= 61,	// �Ͻɵ���Ʒ������������Ҫ
	E_GuildUpgrade_Role_Level           = 62,   // ��ɫ�ȼ��������޷���������

	E_GuildAffair_Times_Limit			= 70,	// ���񷢲���������

	E_GuildSkill_NotResearch			= 80,	// ��ǰû�м����о�
	E_GuildSkill_Wrong_Item				= 81,	// �Ͻ���Ʒ�Ƿ�
	E_GuildSkill_Level_Limit			= 82,	// ���ܴﵽ�ȼ�����
	E_GuildSkill_NotExist				= 83,	// ���ܲ�����

	E_GuildMember_Contribution_Limit	= 90,	// ��Ա��������
	E_GuildMember_Level_Limit			= 91,	// ��Ա�ȼ�����

	E_GuildCommerce_Status_Error		= 100,	// ����״̬����
	E_GuildCommerce_Tael_NotEnough		= 101,	// ��������
	E_GuildCommerce_Commend_Error		= 102,	// �ν��Ѿ�������ر�

	E_GuildCommodity_NotEnough_Space	= 103,	// ���������ռ䲻��
	E_GuildCommodity_NotEnough_Tael		= 104,	// ��������
	E_GuildCommodity_ItemMaxHold		= 105,	// �̻��ﵽ��ӵ������

	E_CofC_NotExist						= 110,	// �̻�Ƿ�
	E_CofC_ItemNotFind					= 111,	// û���ҵ�ָ���̻�
	E_CofC_ItemCannotSell				= 112,	// ָ���̻����ɳ���
	E_CofC_ItemNotEnough				= 113,	// �̻���������
	E_CofC_HoldCity_Limit				= 114,	// û��ռ��ָ������
};

// ״̬
enum EGuildSpecState
{
	EGDSS_NULL				= 0x0000,
	EGDSS_Shortage			= 0x0001,			// �ʽ��ȱ
	EGDSS_Distress			= 0x0002,			// ƶ��
	EGDSS_Chaos				= 0x0004,			// ����
	EGDSS_Warfare			= 0x0008,			// ��ս
	EGDSS_Primacy			= 0x0010,			// ����
	EGDSS_Refulgence		= 0x0020,			// �Ի�

	EGDSS_End				= 0xffff
};

// ְλ
enum EGuildMemberPos
{
	EGMP_Null			= -1,
	EGMP_Start			= 0,

	EGMP_BangZhong		= 0,	//��ͨ����
	EGMP_JingYing		= 1,	//��Ӣ����
	
	EGMP_BangZhu		= 2,	//����
	EGMP_FuBangZhu		= 3,	//������
	EGMP_HongHuFa		= 4,	//���ѻ���
	EGMP_ZiHuFa			= 5,	//��������

	EGMP_Tang_Start		= 6,
	EGMP_QingLongTang	= 6,	//����������
	EGMP_QingLongXiang	= 7,	//����������
	EGMP_QingLongZhong	= 8,	//�����ð���
	
	EGMP_ZhuQueTang		= 9,	//��ȸ������
	EGMP_ZhuQueXiang	= 10,	//��ȸ������
	EGMP_ZhuQueZhong	= 11,	//��ȸ�ð���

	EGMP_BaiHuTang		= 12,	//�׻�������
	EGMP_BaiHuXiang		= 13,	//�׻�������
	EGMP_BaiHuZhong		= 14,	//�׻��ð���

	EGMP_XuanWuTang		= 15,	//����������
	EGMP_XuanWuXiang	= 16,	//����������
	EGMP_XuanWuZhong	= 17,	//�����ð���
	EGMP_Tang_End		= 17,

	EGMP_End			= EGMP_XuanWuZhong,
};

const INT32 X_GUILD_POS_NUM		= EGMP_End - EGMP_Start + 1;

// ��
enum EGuildTangType
{
	EGTT_Null			= -1,
	EGTT_Start			= 0,

	EGTT_QingLong		= 0,	// ������
	EGTT_ZhuQue			= 1,	// ��ȸ��
	EGTT_BaiHu			= 2,	// �׻���
	EGTT_XuanWu			= 3,	// ������

	EGTT_End			= 3,
};

const INT32 X_GUILD_TANG_NUM	= EGTT_End - EGTT_Start + 1;

// ����ְλ
enum ETangMemberPos
{
	ETMP_Null			= -1,

	ETMP_TangZhu		= 0,	// ����
	ETMP_XiangZhu		= 1,	// ����
	ETMP_BangZhong		= 2,	// ����
};

//-----------------------------------------------------------------------------
// ö�ٺϷ��Լ��໥��ת������
//-----------------------------------------------------------------------------
static BOOL IsInTang(EGuildMemberPos eGuildPos)
{
	return (eGuildPos >= EGMP_Tang_Start && eGuildPos <= EGMP_Tang_End);
}

static BOOL IsGuildPosValid(EGuildMemberPos ePos)
{
	return (ePos >= EGMP_Start && ePos <= EGMP_End);
}

static VOID GetTangType(EGuildMemberPos eGuildPos, EGuildTangType &eTang, ETangMemberPos &eTangPos)
{
	if(!IsInTang(eGuildPos))
	{
		eTang		= EGTT_Null;
		eTangPos	= ETMP_Null;
		return;
	}

	switch(eGuildPos)
	{
	case EGMP_QingLongTang:
		eTang		= EGTT_QingLong;
		eTangPos	= ETMP_TangZhu;
		break;
	case EGMP_QingLongXiang:
		eTang		= EGTT_QingLong;
		eTangPos	= ETMP_XiangZhu;
		break;
	case EGMP_QingLongZhong:
		eTang		= EGTT_QingLong;
		eTangPos	= ETMP_BangZhong;
		break;

	case EGMP_ZhuQueTang:
		eTang		= EGTT_ZhuQue;
		eTangPos	= ETMP_TangZhu;			
		break;
	case EGMP_ZhuQueXiang:
		eTang		= EGTT_ZhuQue;
		eTangPos	= ETMP_XiangZhu;
		break;
	case EGMP_ZhuQueZhong:
		eTang		= EGTT_ZhuQue;
		eTangPos	= ETMP_BangZhong;
		break;

	case EGMP_BaiHuTang:
		eTang		= EGTT_BaiHu;
		eTangPos	= ETMP_TangZhu;
		break;
	case EGMP_BaiHuXiang:
		eTang		= EGTT_BaiHu;
		eTangPos	= ETMP_XiangZhu;
		break;
	case EGMP_BaiHuZhong:
		eTang		= EGTT_BaiHu;
		eTangPos	= ETMP_BangZhong;
		break;

	case EGMP_XuanWuTang:
		eTang		= EGTT_XuanWu;
		eTangPos	= ETMP_TangZhu;
		break;
	case EGMP_XuanWuXiang:
		eTang		= EGTT_XuanWu;
		eTangPos	= ETMP_XiangZhu;
		break;
	case EGMP_XuanWuZhong:
		eTang		= EGTT_XuanWu;
		eTangPos	= ETMP_BangZhong;
		break;
	}
}

//enum EGuildMemberAtt
//{
//	EGMA_Start				= 0,
//
//	EGMA_TotalContrib		= 0,	//�ۼư��ɹ���
//	EGMA_Contribution		= 1,	//��ǰ���ɹ���
//	EGMA_Exploit			= 2,	//��ǰ���ɹ�ѫ
//	EGMA_Salary				= 3,	//��ǰٺ»��׼
//
//	EGMA_End				= 3,
//};
//
//const INT32 X_GUILD_MEMBER_ATT_NUM		= EGMA_End - EGMA_Start + 1;

//-----------------------------------------------------------------------------
// �ṹ
//-----------------------------------------------------------------------------
struct tagGuildBase
{
	DWORD			dwID;							//����ID -- ��ֵ�ð�������CRC32ֵ
	DWORD			dwFounderNameID;				//���ɴ�ʼ������ID
	DWORD			dwLeaderRoleID;					//���ɰ�����ɫID
	DWORD			dwSpecState;					//���ɵ�ǰ����״̬
	BYTE			byLevel;						//���ɵ�ǰ�ȼ�
	BYTE			byHoldCity[MAX_GUILD_HOLDCITY];	//���ɵ�ǰϽ�����б�ţ���������ɵȼ��ı䣬��Ҫ���¿����ڴ��������

	INT32			nFund;							//���ɵ�ǰ�ʽ�
	INT32			nMaterial;						//���ɵ�ǰ�ʲ�
	INT32			nReputation;					//���ɵ�ǰ����
	INT32			nGroupPurchase;					//�����Ź�ָ��
	INT16			n16Peace;						//���ɵ�ǰ������
	INT16			n16MemberNum;					//���ɵ�ǰ��Ա����	
	INT16			n16Rank;						//���ɵ�ǰ����
	BYTE			byMinJoinLevel;					//��ͼ���ȼ�
	BYTE			byAffairRemainTimes;			//��������ʣ�෢������
	BOOL			bCommendation;					//�������̼ν�״̬
	INT32			nDailyCost;						//����ÿ��ά������

	tagGuildBase()	{ ZeroMemory(this, sizeof(*this)); };
};

// ���ɳ�Ա
struct tagGuildMember
{
	DWORD			dwRoleID;						//���ɳ�Ա��ɫID
	EGuildMemberPos	eGuildPos;						//��ɫ����ְλ

	INT32			nTotalContribution;				//�ۼư��ɹ���
	INT32			nContribution;					//��ǰ���ɹ���
	INT32			nExploit;						//��ǰ���ɹ�ѫ
	INT32			nSalary;						//��ǰٺ»��׼

	BOOL			bUseGuildWare;					//ʹ�ð��ɲֿ�Ȩ��

	tagGuildMember() {}
	
	tagGuildMember(DWORD dwRoleID, EGuildMemberPos ePos,
					INT32 nTotalContrib = 0, INT32 nContrib = 0, 
					INT32 nExploit = 0, INT32 nSalary = 0, BOOL bUseGuildWare = FALSE)
	{
		ASSERT(::IsGuildPosValid(ePos));

		this->dwRoleID				= dwRoleID;
		this->eGuildPos				= ePos;

		this->nTotalContribution	= nTotalContrib;
		this->nContribution			= nContrib;
		this->nExploit				= nExploit;
		this->nSalary				= nSalary;

		this->bUseGuildWare			= bUseGuildWare;
	}

	tagGuildMember(const tagGuildMember& sGuildMember)
	{
		ASSERT(::IsGuildPosValid(sGuildMember.eGuildPos));

		this->dwRoleID				= sGuildMember.dwRoleID;
		this->eGuildPos				= sGuildMember.eGuildPos;

		this->nTotalContribution	= sGuildMember.nTotalContribution;
		this->nContribution			= sGuildMember.nContribution;
		this->nExploit				= sGuildMember.nExploit;
		this->nSalary				= sGuildMember.nSalary;

		this->bUseGuildWare			= sGuildMember.bUseGuildWare;
	}
};

// ��ɫһ����Ϣ
struct tagRoleCommon
{
	INT16			n16RoleLevel;					//��ǰ��ҵȼ�
	BYTE			byRoleSex;						//��ǰ����Ա�
	bool			bOnline;						//��ǰ�Ƿ�����
	EClassType		eRoleClass;						//��ǰ���ְҵ
	DWORD			dwOfflineTime;					//��ǰ����ʱ��
	INT32			nKnowledge;						//��ǰ��Ұ�������
};


// ���ɳ�Ա��Ϣ
struct tagGuildMemInfo
{
	DWORD		dwRoleID;			// ��Ա����
	INT8		n8GuildPos;			// ��Աְλ
	BYTE		byLevel;			// ��Ա�ȼ�
	BYTE		bySex;				// ��Ա�Ա�
	BYTE		byClass;			// ��Աְҵ
	INT32		nCurContrib;		// ��Ա��ǰ���ɹ���
	DWORD		dwTimeLastLogout;	// ��Ա�������ʱ��
	bool		bOnline;			// �Ƿ�����
};

// ���ɳ�Ա��Ϣ��չ
struct tagGuildMemInfoEx
{
	// ���Ի�ͷ��ͬ���������е�ͷ�� -- �����������Ϣ
	INT32		nTotalContrib;		// �ۼƹ��ף���ɫ�ܰ��ɹ���
	INT32		nExploit;			// ���ɹ�ѫ
	INT32		nKnowledge;			// ��������: �����߽�ɫΪ-1
};


struct tagGuildMemberClient
{
	BOOL				bGetInfo;		// �Ƿ��Ի�ȡ��չ��Ϣ
	tagGuildMemInfoEx	sInfoEx;		// ��չ��Ϣ
	tagGuildMemInfo		sInfo;			// ��ͨ��Ϣ

	tagGuildMemberClient()
	{
		ZeroMemory(this, sizeof(tagGuildMemberClient));
	}
	tagGuildMemberClient(const tagGuildMemInfo& sMemInfo)
	{
		memcpy(&sInfo, &sMemInfo, sizeof(tagGuildMemInfo));
		ZeroMemory(&sInfoEx, sizeof(tagGuildMemInfoEx));
		bGetInfo = FALSE;
	}
	VOID Modify(const tagGuildMemInfoEx& sMemInfoEx)
	{
		memcpy(&sInfoEx, &sMemInfoEx, sizeof(tagGuildMemInfoEx));
		bGetInfo = TRUE;
	}
	VOID Modify(const tagGuildMemInfo& sMemInfo, const tagGuildMemInfoEx& sMemInfoEx)
	{
		memcpy(&sInfo, &sMemInfo, sizeof(tagGuildMemInfo));
		memcpy(&sInfoEx, &sMemInfoEx, sizeof(tagGuildMemInfoEx));
		bGetInfo = TRUE;
	}
};

//-----------------------------------------------------------------------------
// �ṹ -- ����Ȩ�ޣ����캯����ʼ��Ϊ0��Ĭ������NullObject(�������˼�guild_mgr.h)
//-----------------------------------------------------------------------------
// ����ְ��
struct tagGuildPower
{
	DWORD	bDismissGuild		: 1,		// ��ɢ����
			bInviteJoin			: 1,		// �����Ա����
			bTurnoverLeader		: 1,		// �ƽ�����
			bLeaveGuild			: 1,		// �뿪����
			bDemissPostion		: 1,		// ��ȥ����ְ��
			bModifyTenet		: 1,		// �޸���ּ
			bModifySgin			: 1,		// �޸ı�־
			bSetWareRights		: 1,		// ���ð��ɲֿ����Ȩ��
			bUpgrade			: 1,		// ������ʩ����
			bCommerce			: 1,		// ��������
			bSetCommend			: 1,		// �������̼ν�״̬
			bAffair				: 1,		// �������񷢲�
			bSetSkill			: 1,		// ���ð�����������
			bAdvSkill			: 1;		// ���ɼ��ܶ�������

	tagGuildPower()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

// ����Ȩ�ޱ�
struct tagGuildKick
{
	std::bitset<X_GUILD_POS_NUM>	BitSetGuildKick;

	tagGuildKick()
	{
		BitSetGuildKick.reset();
	}
};

// ��ְȨ�ޱ�
struct tagGuildAppoint
{
	std::bitset<X_GUILD_POS_NUM>	BitSetGuildAppoint;

	tagGuildAppoint()
	{
		BitSetGuildAppoint.reset();
	}
};

//-----------------------------------------------------------------------------
// �ṹ -- ��������
//-----------------------------------------------------------------------------
struct tagGuildCfg
{
	// ����&������͵ȼ�
	INT32		nCreateRoleMinLevel;
	INT32		nJoinRoleMinLevel;

	// ���������Ǯ
	INT32		nGoldCreateNeeded;

	// ����������
	BYTE		byGuildLevelBegin;
	BYTE		byDummy;
	INT16		n16GuildPeaceBegin;
	INT32		nGuildRepBegin;
	INT32		nGuildFundBegin;
	INT32		nGuildMaterialBegin;
	INT32		nGuildGroupPurchaseBegin;
};


// ���ɲֿ��������
enum EGuildWareUpdate
{
	EGWU_Null			= -1,

	EGWU_Change			= 0,	// ��Ʒ���
	EGWU_Delete			= 1,	// ��Ʒ��ʧ

	EGWU_End
};

//-----------------------------------------------------------------------------
// �ṹ -- ���ɲֿ���µ�λ
//-----------------------------------------------------------------------------
struct tagGuildWareUpdate
{
	EGuildWareUpdate	eType;
	INT16				nIndex;
	BYTE				byData[1];

	tagGuildWareUpdate()
	{
		ZeroMemory(this, sizeof(*this));
		eType = EGWU_Null;
	}

	INT Size()
	{
		INT nSize = sizeof(EGuildWareUpdate) + sizeof(INT16);

		switch (eType)
		{
		case EGWU_Change:
			{
				// �ϲ㱣֤byDataֵ��Ч
				tagItem* pItem = (tagItem*)byData;
				if (MIsEquipment(pItem->dwTypeID))
				{
					nSize += sizeof(tagEquip);
				}
				else
				{
					nSize += sizeof(tagItem);
				}
			}
			break;

		case EGWU_Delete:
			break;

		default:
			nSize = 0;
			break;
		}

		return nSize;
	}
};

//-----------------------------------------------------------------------------
// �ṹ -- ���ɲֿ�Ȩ���б�λ
//-----------------------------------------------------------------------------
struct tagGuildWarePri 
{
	DWORD	dwRoleID;
	BOOL	bCanUse;
};

//-----------------------------------------------------------------------------
// ������ʩ����
//-----------------------------------------------------------------------------
enum EFacilitiesType
{
	EFT_Null		= -1,

	EFT_Guild		= 0,		// ����
	EFT_Fund		= 1,		// ���
	EFT_Material	= 2,		// �ʲƿ�
	EFT_Academy		= 3,		// ��Ժ

	EFT_End
};

const INT MAX_GUILD_FACILITIES_TYPE = EFT_End - EFT_Null - 1;

//-----------------------------------------------------------------------------
// �ṹ -- ������ʩ��Ϣ
//-----------------------------------------------------------------------------
struct tagGuildFacilitiesInfo
{
	EFacilitiesType	eType;
	INT16	nProgress;
	INT16	nFulfill;
	DWORD	dwItemID[MAX_UPGRADE_NEED_ITEM_TYPE];
	INT32	nNeedNum[MAX_UPGRADE_NEED_ITEM_TYPE];
	INT32	nNeedFund;
	INT32	nMaterial;
	BYTE	byLevel;

	tagGuildFacilitiesInfo()
	{
		ZeroMemory(this, sizeof(*this));
	}

	BOOL IsLevelUp()
	{
		return nProgress >= nFulfill;
	}

	VOID ResetItemInfo()
	{
		for (int n=0; n<MAX_UPGRADE_NEED_ITEM_TYPE; n++)
		{
			dwItemID[n]	= GT_INVALID;
			nNeedNum[n] = 0;
		}
	}
};

//-----------------------------------------------------------------------------
// �ṹ -- ���ɼ���������Ϣ
//-----------------------------------------------------------------------------
struct tagGuildSkill 
{
	DWORD		dwSkillID;
	INT16		n16Progress;
	INT16		n16Fulfill;
	INT32		nResearchFund;
	INT32		nResearchMaterial;

	INT32		nLearnSilver;
	INT32		nLearnContribution;
	INT32		nLearnFund;
	INT32		nLearnMaterial;

	INT			nLevel;

	tagGuildSkill()
	{
		ZeroMemory(this, sizeof(*this));
	}
	tagGuildSkill(const tagGuildSkill &rhs)
	{
		memcpy(this, &rhs, sizeof(*this));
	}
};

//-----------------------------------------------------------------------------
// �ṹ -- �������̳�ʼ��Ϣ
//-----------------------------------------------------------------------------
struct tagTaelInfo 
{
	INT32	nDeposit;		// ��֤��
	INT32	nBeginningTael;	// ��ʼ����
	INT32	nPurposeTael;	// ָ������
	INT32	nMaxTael;		// ��������
};

//-----------------------------------------------------------------------------
// �ṹ -- �����������а�
//-----------------------------------------------------------------------------
struct tagCommerceRank 
{
	DWORD		dwRoleID;
	INT			nTimes;
	INT32		nTael;
};

//-----------------------------------------------------------------------------
// �ṹ -- ���������̻�
//-----------------------------------------------------------------------------
struct tagCommerceGoodInfo
{
	DWORD	dwGoodID;
	INT32	nCost;
	BYTE	byGoodNum;

	tagCommerceGoodInfo()
	{
		dwGoodID	= GT_INVALID;
		nCost		= 0;
		byGoodNum	= 0;
	}

	BOOL IsValid()
	{
		if (!GT_VALID(dwGoodID) || (dwGoodID == 0) || (byGoodNum == 0))
		{
			return FALSE;
		}

		return TRUE;
	}
};

//-----------------------------------------------------------------------------
#pragma pack(pop)