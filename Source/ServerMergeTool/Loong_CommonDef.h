#pragma once
#include "stdafx.h"
#include "../WorldDefine/base_define.h"
#include "../WorldDefine/login.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/vcard_define.h"
#include "../WorldDefine/mall_define.h"


#include "../ServerDefine/base_define.h"
#include "../ServerDefine/login_define.h"
#include "../ServerDefine/role_data_define.h"
#include "../ServerDefine/msg_famehall.h"
#include "../ServerDefine/msg_mall.h"
#include "../ServerDefine/guild_define.h"
#include "../ServerDefine/pet_define.h"
#include "../ServerDefine/msg_vipstall.h"
#include "../ServerDefine/activity_define.h"

#pragma pack(push,1)
//------------------------------------------------------------------------------
// ��
//------------------------------------------------------------------------------
#define CONFIG_INI			"ServerMerge.ini"
#define COMPENSATION_INI	"ServerMerge_C.ini"		// ��Ҳ���
#define LOG_FOLDER			"Log"
#define MIBAO_LEN			240						// �ܱ����̶�
#define DB_PAGING			100000					// ��ҳ������10W
#define THREAD_POOL_NUM		5						// �̳߳��߳���
#define ROLE_MAX_LIMIT		5						// ���˺���������
//------------------------------------------------------------------------------
// ö��
//------------------------------------------------------------------------------
enum ERoleCompensationType
{
	ERC_START				= 0,	//��ʼ
	ERC_CHANGE_ROLE_NAME	= 0,	//�����Ĳ���
	ERC_END					= 1,
};

enum EThreadPoolState
{
	ETPS_NULL				= -1, 
	ETPS_STILL_HAVE_ACTIVE	= 0, //��Ȼ���߳��ڹ���
	ETPS_SUCCESS			= 1, //�ɹ�
	ETPS_FAILURE			= 2, //ʧ��
};

enum EPagingState
{		
	EPS_DB_NULL = -1,
	EPS_DB_SUCCESS = 0,			//�ɹ�
	EPS_DB_ERROR = 1,			//ʧ��
	EPS_DB_RESULT_NULL = 2,		//�������
};
enum EDBType 
{
	EM_DB_NULL = -1,

	EM_DB_SRC_ONE = 0,		//Դһ DB
	EM_DB_SRC_TWO = 1,		//Դ�� DB
	EM_DB_TAR = 2,			//Ŀ�� DB

	EM_DB_TYPE_END = 3,
};
enum EDBMergeType 
{
	EM_MERGE_NULL = -1,

	EM_MERGE_INSIDE_SECTION = 0,		//������
	EM_MERGE_CROSS_SECTION = 1,			//�����

	EM_MERGE_TYPE_END = 2,
};

//item biao ����
enum ETBItemType 
{
	EM_TB_NULL = -1,

	EM_TB_ITEM = 0,				//item
	EM_TB_ITEM_BAIBAO = 1,		//item baibao
	EM_TB_ITEM_DEL = 2,			//item del
	EM_TB_END = 3,
};
enum ETBRoleRemoveType
{
	EM_R_NULL = -1,

	EM_R_ONESELF = 1,		//��ɾ��
	EM_R_MERGE = 2,			//�ϲ�ʱɾ��
	EM_R_END = 3,
};
//------------------------------------------------------------------------------
// �ṹ��
//------------------------------------------------------------------------------
//�û�������Ϣ
struct tagConfigDetail{
	tstring strIp;		//ip
	tstring strUser;	//�û���
	tstring strPsd;		//����
	tstring strDBName;	//���ݿ���
	DWORD	dwPort;		//���ݿ�˿�
	tagConfigDetail()
	{
		memset(this,0,sizeof(tagConfigDetail));
	}
};


//------------------------------------------------------------------------------
// �ṹ�� login���ݿ���
//------------------------------------------------------------------------------
struct tagTWrap_Longin_Account
{
	CHAR	szName[X_SHORT_NAME];			// �û���
	CHAR	szPsd[MAX_MD5_ARRAY];			// ����
	CHAR	szMibao[MIBAO_LEN];				// �ܱ�
	tagProofResult stOthers;

	tagTWrap_Longin_Account()
	{
		memset(szName,0,X_SHORT_NAME);

		memset(szPsd,0,MAX_MD5_ARRAY);

		memset(szMibao,0,MIBAO_LEN);
	}
};
struct tagTWrap_Longin_WorldForbid
{
	DWORD dwAccountID;
	DWORD dwWorldNameCrc;

	tagTWrap_Longin_WorldForbid()
	{
		dwAccountID = 0;
		dwWorldNameCrc = 0;
	}
};
//������
struct tagTWrap_Longin_BlackList
{
	CHAR  szIP[X_IP_LEN];				//ip

	tagTWrap_Longin_BlackList()
	{
		memset(this,0,sizeof(tagTWrap_Longin_BlackList));
	}
};
//������
struct tagTWrap_Longin_FatigueTime
{
	DWORD dwAccountNameCrc;
	DWORD dwOnlineTime;
	DWORD dwOfflineTime;

	tagTWrap_Longin_FatigueTime()
	{
		memset(this,0,sizeof(tagTWrap_Longin_FatigueTime));
	}
};
//------------------------------------------------------------------------------
// �ṹ�� loong���ݿ���
//------------------------------------------------------------------------------
struct tagTWrap_Loong_AccountCommon
{
	DWORD dwAccountID;
	CHAR  szAccountName[X_SHORT_NAME];// �û���
	tagAccountCommon stOthers;

	tagTWrap_Loong_AccountCommon()
	{
		dwAccountID = GT_INVALID;
		memset(szAccountName,0,X_SHORT_NAME);
	}
};

//BlackList
struct tagTWrap_Loong_BlackList
{
	DWORD dwRoleID;
	DWORD dwBlackID;

	tagTWrap_Loong_BlackList()
	{
		dwRoleID = GT_INVALID;
		dwBlackID = GT_INVALID;
	}
};
//buff
struct tagTWrap_Loong_Buff
{
	DWORD dwRoleID;
	CHAR *pcBlob;

	tagBuffSave stOthers;

	tagTWrap_Loong_Buff()
	{
		dwRoleID = GT_INVALID;
		pcBlob = NULL;
	}
	~tagTWrap_Loong_Buff()
	{
		if(pcBlob != NULL)
		{
			delete []pcBlob;
		}
	}
};

//����
struct tagTWrap_Loong_ClanData
{
	DWORD dwRoleID;
	tagDBReputeData stOthers;

	tagTWrap_Loong_ClanData()
	{
		dwRoleID = GT_INVALID;
	}
};
//Enemy
struct tagTWrap_Loong_Enemy
{
	DWORD dwRoleID;
	DWORD dwEnemyID;

	tagTWrap_Loong_Enemy()
	{
		dwRoleID = GT_INVALID;
		dwEnemyID = GT_INVALID;
	}
};
//equip
struct tagTWrap_Loong_Equip
{
	INT64 n64SerialNum;
	tagEquip stOthers;

	tagTWrap_Loong_Equip()
	{
		n64SerialNum = GT_INVALID;
	}
};
struct tagTWrap_Loong_EquipDel
{
	INT64 n64SerialNum;
	tagEquip stOthers;

	tagTWrap_Loong_EquipDel()
	{
		n64SerialNum = GT_INVALID;
	}
};
struct tagTWrap_Loong_EquipBaiBao
{
	INT64 n64SerialNum;
	tagEquip stOthers;

	tagTWrap_Loong_EquipBaiBao()
	{
		n64SerialNum = GT_INVALID;
	}
};

//�����䱦�����
typedef tagNDBC_InsertActClanTreasure tagTWrap_Loong_ActClanTreasure;

//����������
typedef tagNDBC_InsertFameHallSnap tagTWrap_Loong_FameHallEnterSnap;

//������������ʱ�����
typedef tagNDBC_UpDateRepRstTime tagTWrap_Loong_RepRstTime;

//����
struct tagTWrap_Loong_Friend
{
	DWORD dwRoleID;
	tagFriendSave stOthers;

	tagTWrap_Loong_Friend()
	{
		dwRoleID = GT_INVALID;
	}
};

//�Ѻö�
struct tagTWrap_Loong_FriendshipSave
{
	DWORD dwRoleID;
	tagFriendshipSave stOthers;

	tagTWrap_Loong_FriendshipSave()
	{
		dwRoleID = GT_INVALID;
	}
};

//�Ź�
struct tagTWrap_Loong_GroupPurchase
{
	CHAR *pcBlob;
	DWORD dwBlobSize;

	tagGPInfo stOthers;

	tagTWrap_Loong_GroupPurchase()
	{
		pcBlob = NULL;
		dwBlobSize = 0;
	}
	~tagTWrap_Loong_GroupPurchase()
	{
		if(pcBlob != NULL)
		{
			delete []pcBlob;
		}
	}
};
//����
struct tagTWrap_Loong_Guild
{
	char szDate[X_DATATIME_LEN + 1];
	tagGuildLoad stOthers;

	tagTWrap_Loong_Guild()
	{
		memset(szDate,0,X_DATATIME_LEN+1);
	}
};
//���ɳ�Ա
struct tagTWrap_Loong_GuildMem
{
	char szDate[X_DATATIME_LEN + 1];
	tagGuildMemLoad stOthers;

	tagTWrap_Loong_GuildMem()
	{
		memset(szDate,0,X_DATATIME_LEN+1);
	}
};
//item

//item
struct tagTWrap_Loong_Item
{
	tagItem stOthers;
};
struct tagTWrap_Loong_ItemBaiBao
{
	tagItem stOthers;
};
struct tagTWrap_Loong_ItemDel
{
	tagItem stOthers;
};

//item cd time
struct tagTWrap_Loong_ItemCDTime
{
	DWORD dwRoleID;
	std::vector <tagCDTime> vecCDTime;

	tagTWrap_Loong_ItemCDTime()
	{
		dwRoleID = GT_INVALID;
	}

};

//item need log
struct tagTWrap_Loong_ItemNeedLog
{
	DWORD dwTypeID; //'��Ʒ��TypeID(Ψһ)',
	BOOL  bNeedLog;	//'�Ƿ��¼log(0:����¼;1:��¼)',
	SHORT sMinQlty;	//'���¼log����Ʒ�����Ʒ��',

	tagTWrap_Loong_ItemNeedLog()
	{
		dwTypeID = GT_INVALID;
		bNeedLog = TRUE;
		sMinQlty = 0;
	}
};

//log baibao
struct tagTWrap_Loong_LogBaiBao
{
	DWORD dwAccountID;
	TCHAR  *ptcWords;
	tagBaiBaoRecord stOthers;

	tagTWrap_Loong_LogBaiBao()
	{
		dwAccountID = GT_INVALID;
		ptcWords     = NULL;
	}
};


//��ɫɾ��
struct tagTWrap_Loong_RoleDel
{
	DWORD dwAccountID;
	DWORD dwRoleID;
	CHAR  szRoleName[X_SHORT_NAME];				// ��
	CHAR  szIP[X_IP_LEN];						// ip
	CHAR  szDeleteTime[X_DATATIME_LEN + 1];		// del time

	tagTWrap_Loong_RoleDel()
	{
		memset(this,0,sizeof(tagTWrap_Loong_RoleDel));

		dwAccountID = GT_INVALID;
		dwRoleID = GT_INVALID;
	}

};
//��ɫ����
struct tagTWrap_Loong_RoleData
{
	DWORD dwAccountID;
	DWORD dwRoleID;
	BOOL  bRemoveFlag;
	CHAR  szRemoveTime[X_DATATIME_LEN + 1];		// del time
	tagRoleDataLoad	stOthers;

	tagTWrap_Loong_RoleData()
	{
		bRemoveFlag = FALSE;
		dwRoleID = GT_INVALID;
		dwAccountID = GT_INVALID;
		memset(szRemoveTime ,0 ,X_DATATIME_LEN + 1);
	}

};
struct tagTWrap_Loong_RoleDataDel
{
	DWORD dwAccountID;
	DWORD dwRoleID;
	BOOL  bRemoveFlag;
	CHAR  szRemoveTime[X_DATATIME_LEN + 1];		// del time
	tagRoleDataLoad	stOthers;

	tagTWrap_Loong_RoleDataDel()
	{
		bRemoveFlag = FALSE;
		dwRoleID = GT_INVALID;
		dwAccountID = GT_INVALID;
		memset(szRemoveTime ,0 ,X_DATATIME_LEN + 1);
	}

};
//����
struct tagTWrap_Loong_Skill
{

	DWORD dwRoleID;
	tagSkillSave stOthers;

	tagTWrap_Loong_Skill()
	{
		dwRoleID = GT_INVALID;
	}
};

//����
struct tagTWrap_Loong_Task
{

	DWORD dwRoleID;
	tagQuestSave stOthers;

	tagTWrap_Loong_Task()
	{
		dwRoleID = GT_INVALID;
	}
};

//�������
struct tagTWrap_Loong_TaskDone
{
	
	DWORD dwRoleID;
	tagQuestDoneSave stOthers;

	tagTWrap_Loong_TaskDone()
	{
		dwRoleID = GT_INVALID;
	}
};

//�ƺ�
struct tagTWrap_Loong_Title
{

	DWORD dwRoleID;
	tagTitleSave stOthers;

	tagTWrap_Loong_Title()
	{
		dwRoleID = GT_INVALID;
	}
};

//����
struct tagTWrap_Loong_VisitingCard
{
	TCHAR tcHeadUrl[LEN_MAX_URL];
	TCHAR tcSignature[LEN_MAX_SIGNATURE];

	tagVCardData stOthers;

	tagTWrap_Loong_VisitingCard()
	{
		memset(tcHeadUrl,0,LEN_MAX_URL);
		memset(tcSignature,0,LEN_MAX_SIGNATURE);
	}
};



//Ԫ��
typedef tagYBAccount tagTWrap_Loong_YBAccount;

//Ԫ�����׶���
typedef tagYuanBaoOrder tagTWrap_Loong_YuanBaoOrder;

//bill_item
struct tagTWrap_Loong_BillItem
{
	CHAR  szTokenID[50];	// 
	DWORD dwAccountID;
	DWORD dwItemTypeID;
	INT nNum;
	tagTWrap_Loong_BillItem()
	{
		memset(this,0,sizeof(tagTWrap_Loong_BillItem));
	}
};
//bill_yuanbao
struct tagTWrap_Loong_BillYuanBao
{
	CHAR  szTokenID[50];	// 
	DWORD dwAccountID;
	INT	nNum;

	tagTWrap_Loong_BillYuanBao()
	{
		memset(this,0,sizeof(tagTWrap_Loong_BillYuanBao));
	}
};
//bill_yuanbao_log
struct tagTWrap_Loong_BillYuanBaoLog
{
	CHAR  szTokenID[50];	// 
	DWORD dwAccountID;
	INT	  nNum;
	CHAR  szTime[X_DATATIME_LEN + 1];//time

	tagTWrap_Loong_BillYuanBaoLog()
	{
		memset(this,0,sizeof(tagTWrap_Loong_BillYuanBaoLog));
	}
};
//pet_data
typedef tagDBPetAtt tagTWrap_Loong_PetData;

//pet_skill
struct tagTWrap_Loong_PetSkill
{
	DWORD dwPetID;
	tagDBPetSkill stOthers;
	tagTWrap_Loong_PetSkill()
	{

	}
};

//vip_stall
typedef tagVipStall2DB tagTWrap_Loong_VipStall;

//guild_commodity
struct tagTWrap_Loong_GuildCommodity
{
	DWORD dwGuildID;
	DWORD dwGoodInfoSize;
	tagGuildCommerceInfo stOthers;

	tagTWrap_Loong_GuildCommodity()
	{
		dwGuildID = 0;
		dwGoodInfoSize = 0;
	}
};
//guild_skill
struct tagTWrap_Loong_GuildSkill
{
	DWORD dwGuildID;
	tagGuildSkillInfo stOthers;

	tagTWrap_Loong_GuildSkill()
	{
		dwGuildID = 0;
	}
};

//��������
struct tagTWrap_Loong_VipNetBar
{	
	DWORD dwAccountID;
	DWORD dwLoginTime;

	tagTWrap_Loong_VipNetBar()
	{
		dwAccountID = 0;
		dwLoginTime = 0;
	}
};

//������ʩ
typedef tagFacilitiesLoad tagTWrap_Loong_GuildFacilities;

//��������
struct tagTWrap_Loong_CommerceRank
{
	DWORD dwGuildID;
	tagCommerceRank stOthers;

	tagTWrap_Loong_CommerceRank()
	{
		dwGuildID = 0;
	}
};

//��ű�
typedef tagActivityData tagTWrap_Loong_ActivityData;


//����
struct tagTWrap_Loong_LeftMsg
{
	DWORD dwMsgID;

	DWORD dwRoleID;

	CHAR *pcBlob;
	DWORD dwBlobSize;

	tagTWrap_Loong_LeftMsg()
	{
		dwMsgID		= GT_INVALID;
		dwRoleID	= GT_INVALID;
		pcBlob		= NULL;
		dwBlobSize	= 0;
	}
	~tagTWrap_Loong_LeftMsg()
	{
		if(pcBlob != NULL)
		{
			delete []pcBlob;
		}
	}

};

//------------------------------------------------------------------------------
// �ṹ�� ����ת�����ձ�Ľṹ
//------------------------------------------------------------------------------

//��Ҳ���
struct tagRoleCompensation
{
	ERoleCompensationType	Type;	// ��������
	DWORD	dwAccountID;			// �˺�id
	DWORD	dwRoleID;				// ���id

	tagRoleCompensation()
	{
		Type			= ERC_START;
		dwAccountID		= GT_INVALID;
		dwRoleID		= GT_INVALID;
	}

	tagRoleCompensation(ERoleCompensationType _Type, DWORD _AccountID, DWORD _RoleID)
	{
		Type			= _Type;
		dwAccountID		= _AccountID;
		dwRoleID		= _RoleID;
	}
	~tagRoleCompensation()
	{

	}
};

//����
struct tagCompensation
{
	tagCompensation()
	{
	
	}
	virtual ~tagCompensation()
	{

	}
};
//��Ʒ����
struct tagItemCompensation : public tagCompensation
{
	DWORD	dwItemTypeID;

	tagItemCompensation()
	{
		dwItemTypeID	= GT_INVALID;
	}
	~tagItemCompensation()
	{

	}
	tagItemCompensation operator = (const tagItemCompensation& rhs)
	{
		dwItemTypeID = rhs.dwItemTypeID;

		return *this;
	}
	bool operator < (const tagItemCompensation& rhs) const
	{
		return dwItemTypeID < rhs.dwItemTypeID;
	}
};
//////////////////////////////////////////////////////////////////////////
struct tagRoleIDKey
{
	DWORD dwRoleID;

	EDBType emDBType;

	tagRoleIDKey(DWORD dwRoleID,EDBType emDBType)
	{
		this->dwRoleID = dwRoleID;
		this->emDBType = emDBType;
	}
	bool operator<(const tagRoleIDKey& ver) const
	{
		if( dwRoleID<ver.dwRoleID)
			return true;
		else if(dwRoleID>ver.dwRoleID)
			return false;

		if( emDBType<ver.emDBType)
			return true;
		else if(emDBType>ver.emDBType)
			return false;

		return false;
	}

};
struct tagGuildIDKey
{
	DWORD dwGuildID;		// ����id

	EDBType emDBType;

	tagGuildIDKey(DWORD dwGuildID,EDBType emDBType)
	{
		this->dwGuildID = dwGuildID;
		this->emDBType = emDBType;
	}
	bool operator<(const tagGuildIDKey& ver) const
	{
		if( this->dwGuildID<ver.dwGuildID)
			return true;
		else if(this->dwGuildID>ver.dwGuildID)
			return false;

		if( emDBType<ver.emDBType)
			return true;
		else if(emDBType>ver.emDBType)
			return false;

		return false;
	}
};
struct tagAccountIDKey
{
	DWORD dwAccountID;

	EDBType emDBType;

	tagAccountIDKey(DWORD dwAccountID,EDBType emDBType)
	{
		this->dwAccountID = dwAccountID;
		this->emDBType = emDBType;
	}
	bool operator<(const tagAccountIDKey& ver) const
	{
		if(dwAccountID<ver.dwAccountID)
			return true;
		else if(dwAccountID>ver.dwAccountID)
			return false;

		if( emDBType<ver.emDBType)
			return true;
		else if(emDBType>ver.emDBType)
			return false;

		return false;
	}
};

struct tagActClanTreasureKey
{
	BYTE	byClanType;				// ��������
	UINT16	u16TreasureID;			// �䱦��������Ʒid

	tagActClanTreasureKey(BYTE	byClanType,UINT16	u16TreasureID)
	{
		this->byClanType = byClanType;
		this->u16TreasureID = u16TreasureID;
	}
	bool operator<(const tagActClanTreasureKey& ver) const
	{
		if(byClanType<ver.byClanType)
			return true;
		else if(byClanType>ver.byClanType)
			return false;

		if( u16TreasureID<ver.u16TreasureID)
			return true;
		else if(u16TreasureID>ver.u16TreasureID)
			return false;

		return false;
	}
};
/*
struct tagItemNumKey
{
	DWORD dwItemSerialNum;

	EDBType emDBType;

	tagItemNumKey(DWORD dwItemSerialNum,EDBType emDBType)
	{
		this->dwItemSerialNum = dwItemSerialNum;
		this->emDBType = emDBType;
	}
	bool operator<(const tagItemNumKey& ver) const
	{
		if(dwItemSerialNum<ver.dwItemSerialNum)
			return true;
		else if(dwItemSerialNum>ver.dwItemSerialNum)
			return false;

		if( emDBType<ver.emDBType)
			return true;
		else if(emDBType>ver.emDBType)
			return false;

		return false;
	}
};
*/
#pragma pack(pop)