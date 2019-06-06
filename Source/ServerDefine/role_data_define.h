#pragma once

#include "../WorldDefine/base_define.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/time.h"
#include "../WorldDefine/talent_define.h"
#include "../WorldDefine/currency_define.h"
#include "../ServerDefine/clandata_define.h"
#include "../ServerDefine/strength_define.h"
#pragma pack(push, 1)

struct tagDWORDTime;

//-------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------
const INT MIN_ROLE_SAVE2DB_INTERVAL			= 60 * TICK_PER_SECOND;
const INT ROLE_TIME_STAT_INTERVAL			= 30 * 60 * TICK_PER_SECOND;

// Jason 2009-12-7 �����͸���󼶱�
// �����޶�,�����Ա�ȥ�����
static INT ROLE_MAX_LESSLEVEL_OF_LOONG		= 20 ;

//-------------------------------------------------------------------------------------
// ���ﴴ���ṹ
//-------------------------------------------------------------------------------------
struct tagCreateRoleInfo
{
	tagAvatarAtt	Avatar;						// size = 16
	TCHAR			szRoleName[X_SHORT_NAME];

	tagDWORDTime	CreateTime;					// ����ʱ��
	DWORD			dwMapID;
	FLOAT			fX;
	FLOAT			fY;
	FLOAT			fZ;
	FLOAT			fFaceX;
	FLOAT			fFaceY;
	FLOAT			fFaceZ;
	BYTE			byLevel;
	BYTE			byReserved[3];

};

//---------------------------------------------------------------------------------------
// ʵʱ�������ݿ�ö��
//---------------------------------------------------------------------------------------
enum ERealTimeSaveAtt
{
	ERTSA_Null			= -1,
	ERTSA_Exp			= 0,
	ERTSA_Level			= 1,
};

//---------------------------------------------------------------------------------------
// ����������ݽṹ
//---------------------------------------------------------------------------------------
struct tagRoleInfo
{
	DWORD			dwRoleID;					// ID
	DWORD			dwRoleNameCrc;				// ����CRC
	DWORD			dwAccountID;				// �˻�ID
	TCHAR			szRoleName[X_SHORT_NAME];	// ����
	DWORD			dwMapID;					// ���ڵ�ͼ
	tagDWORDTime	dwTimeLastLogout;			// �������ʱ��
	BYTE			byLevel;					// �ȼ�
	BYTE			bySex;						// �Ա�
	bool			bOnline;					// ����Ƿ�����
	BYTE			byClass;					// ְҵ
	DWORD			dwLoverID;					// ����
};

//---------------------------------------------------------------------------------------
// ��ȫ��
//---------------------------------------------------------------------------------------
struct tagSafeCode
{
	DWORD	dwSafeCodeCrc;		// ��ȫ��Crc32��GT_INVALID��ʾû�����ù�
	DWORD	dwTimeReset;		// ����ʱ�䣬GT_INVALID��ʾû������
};


// Jason �����͸������ļ����ݽṹ
struct tagConfigLessingOfLoong
{
	INT nID;			// ����0-7��Ҳ��8��
	INT nExpireTime;	// ����ʱ��
	INT nExperience;	// ����ֵ
	DWORD	dwRewardItemTypeID;	// ������Ʒtypeid
	INT	nQlty;
	INT	nItemCount;		// ������Ʒ����
	tagConfigLessingOfLoong()
		:nID(0),nExpireTime(0),nExperience(0),dwRewardItemTypeID(0),nItemCount(0),nQlty(0)
	{
	}
	tagConfigLessingOfLoong(const tagConfigLessingOfLoong & t)
		:nID(t.nID),nExpireTime(t.nExpireTime),nExperience(t.nExperience),dwRewardItemTypeID(t.dwRewardItemTypeID),nItemCount(t.nItemCount),
		nQlty(t.nQlty)
	{
	}
	tagConfigLessingOfLoong & operator = (const tagConfigLessingOfLoong & t)
	{
		memcpy(this,&t,sizeof(t));
	}
};


//---------------------------------------------------------------------------------------
// accountrel�����������
//---------------------------------------------------------------------------------------
struct tagAccountCommon
{
	tagSafeCode	sSafeCode;
	DWORD		dwBagPsdCrc;
	INT			nBaiBaoYuanBao;
	INT64		n64WareSilver;
	INT16		n16WareSize;

	VOID Init()
	{
		sSafeCode.dwSafeCodeCrc = GT_INVALID;
		sSafeCode.dwTimeReset	= GT_INVALID;

		dwBagPsdCrc				= GT_INVALID;
		n64WareSilver			= 0;
		nBaiBaoYuanBao			= 0;
		n16WareSize				= 0;
	}
};

//---------------------------------------------------------------------------------------
// ��Ϸ�����������ݿ��������ȡ��������ݽṹ
//---------------------------------------------------------------------------------------
struct tagRoleDataConst
{
	DWORD			dwRoleNameCrc;				// ����CRC
	TCHAR			szRoleName[X_SHORT_NAME];	// ����
	tagAvatarAtt	Avatar;						// ���
	tagDWORDTime	CreateTime;					// ����ʱ��
};

struct tagRoleDataSave
{
	tagDWORDTime	LoginTime;					// ����ʱ��
	tagDWORDTime	LogoutTime;					// �ǳ�ʱ��
	INT				nOnlineTime;				// ������ʱ��
	INT				nCurOnlineTime;				// ��������ʱ��

	// Jason 2009-12-7 �����͸�
	DWORD	timeLastLessing;			// �ϴ������͸�ʱ�䣬ע�⣬��ֵ��������ʱ�䵥λһ��
	INT				nLessingLevel;				// �͸��������ROLE_MAX_LESSLEVEL_OF_LOONG��

	tagAvatarEquip	AvatarEquip;				// װ�����
	tagDisplaySet	DisplaySet;					// �����ʾ����

	DWORD			dwMapID;					// ���ڵĵ�ͼ��ID
	DWORD			dwRebornMapID;				// �����ͼID
	FLOAT			fCoordinate[3];				// λ�ã�x,y,z��
	FLOAT			fFace[3];					// ����Vectro3��

	EClassType		eClass;						// ְҵ
	EClassTypeEx	eClassEx;					// ��չְҵ

	INT				nLevel;						// ��ҵȼ�
	INT				nCurExp;					// ��ҵ�ǰ�������������

	INT				nHP;						// ����
	INT				nMP;						// ����
	INT				nRage;						// ŭ��
	INT				nEndurance;					// �־���
	INT				nVitality;					// ����
	INT				nInjury;					// ����
	INT				nKnowledge;					// ����
	INT				nMorale;					// ʿ��
	INT				nMorality;					// ����
	INT				nCulture;					// ��ѧ��Ϊ
	INT				nHostility;					// ����
	INT				nCredit;					// ���ö�
	INT				nIdentity;					// ���
	INT				nVIPPoint;					// ��Ա����

	INT				nAttPoint;					// ��ҿ��õ����Ե�
	INT				nTalentPoint;				// ��ҿ��õ����ʵ�

	tagStrength		Strength;					// ʵ��ֵ
	INT				nStrengthNum;				// ʵ������

	INT				nAttPointAdd[X_ERA_ATTA_NUM];// ���Ͷ�ŵ�����һ�������е�ֵ
	tagRoleTalent	talent[X_MAX_TALENT_PER_ROLE];// ��ҵ�����

	BOOL			bSafeGuard;					// �Ƿ�PK����
	tagDWORDTime	CloseSafeGuardTime;			// �ϴιر�PK������ʱ��
	INT				nDeadUnSetSafeGuardCountDown;	//�ر��������Ϊ0ʱ�سǸ��������ϵ�pk��������tick
	DWORD           dwItemTransportMapID;           //��ǰ��¼���ͷ��󶨵�ͼID
	FLOAT			fItemTransportX;				//��ǰ��¼���ͷ��󶨵�x����
	FLOAT			fItemTransportZ;				//��ǰ��¼���ͷ��󶨵�z����
	FLOAT			fItemTransportY;				//��ǰ��¼���ͷ��󶨵�y����

	INT16			n16BagSize;					// ������С
	INT				nBagGold;					// �����н�����
	INT				nBagSilver;					// ������������
	INT				nBagYuanBao;				// ������Ԫ����
	INT				nExVolume;					// ��ɫ������

	//INT16			n16WareSize;				// �˺Ųֿ��С
	//INT				nWareGold;					// �˺Ųֿ��н�����
	//INT				nWareSilver;				// �˺Ųֿ���Ӭ����
	//INT				nWareYuanBao;				// �˺Ųֿ���Ԫ����

	DWORD			dwGuildID;					// ����id
	
	INT				nTotalTax;					// ��˰�ܶ�

	tagRemoteOpenSet sRemoteOpenSet;			// ��Զ����ҹ�����Ϣ����
	UINT16			u16ActiveTitleID;			// ��ǰ�ƺ�

	DWORD			dwTimeGetMallFree;			// �ϴδ��̳���ȡ�����Ʒʱ��

	INT	 			nSkillNum;					// ���ܸ���			-- tagSkillSave
	INT				nBuffNum;					// ״̬����			-- tagBuffSave

	INT				nTitleNum;					// �ƺŸ���			-- tagTitleSave

	INT				nItemNum;					// ��Ʒװ������		-- tagItem,tagEquip
	INT				nFriendNum;					// ���Ѹ���			-- ��: tagFriendSave, ��: ʵʱ,����������Ϣ
	INT				nEnemyNum;					// ��и���			-- ��: DWORD dwEnemyID, ��: ʵʱ,����������Ϣ
	INT				nQuestNum;					// �����б����		-- tagQuestSave
	INT				nQuestDoneNum;				// ����ɵ�������� -- ����tagQuestDoneSave���棺ʵʱ
	INT				nItemCDTimeNum;				// ��Ʒ��ȴʱ��		-- tagCDTime
	INT				nFriendshipNum;				// �Ѻö�			-- tagFriendshipSave
	INT				nBlackNum;					// ������			-- DWORD
	BOOL			bClanDataChg;				// ���������Ƿ�ı�
	INT				nPetsNum;					// ������Ŀ
	tagScriptData<ESD_Role>	Data;				// ��ҽű�����
	INT				nTreasureSum;				// ��ҿ���������

	DWORD			dwLastStallTime;			// ̯λ�������ʱ��
	INT32			nStallDailyExp;				// ̯λ���վ���
	INT32			nStallCurExp;				// ̯λ��ǰ�ȼ�����
	BYTE			byStallLevel;				// ̯λ�ȼ�
	DWORD			dwLoverID;					// ���˵�roleid û������4294967295
	BOOL			bHaveWedding;				// �Ƿ���й�����
	BOOL			bHasLeftMsg;				// ������
	
	BOOL			bSpeakOff;					// ����״̬
	BOOL			bNeedPrisonRevive;			// ��Ҫ����������


	/*********************************************************************************************************
	*�����б�״̬�б��ƺ��б��ƺ������б���Ʒ�б�װ���б������б�����б������б�����������б�
	*��Ʒ��ȴʱ���, �Ѻöȱ���ɫ����(�Զ���)
	*���밴˳�򿽱�
	*********************************************************************************************************/

	BYTE			byData[1];
};

struct tagRoleDataLoad
{
	DWORD				dwSize;					// �����Ϣ��С
	tagRoleDataConst	RoleDataConst;
	tagRoleDataSave		RoleDataSave;
};

struct tagRecordDel // ɾ���������Ϣ(���α����)
{
	INT	 	nSkillNum;		// ���ܸ���(dwSkillID):  ����ԭ������ļ�����ʧ,�������������ѧ,�˴�Ҳ���¼
	INT		nBuffNum;		// ״̬����(dwBuffID):    �����������ʧ��������״̬
//	INT		nTalentNum;		// �����б�(ETalentType): ϴ�������
//	INT		nItemNum;		// ��Ʒ����(n64Serial):   ��ݻٵģ���������ݿ���ɾ���� -- ʵʱ
//	INT		nEquipNum;		// װ������(n64Serial):   ��ݻٵģ���������ݿ���ɾ���� -- ʵʱ
	INT		nQuestNum;		// �������(dwQuestID):   ����ɾ���ĺ���ɵ�

	/*********************************************************************************************************
	*�����б�״̬�б���Ʒ�б�װ���б������б�
	*���밴˳�򿽱�
	*********************************************************************************************************/
	BYTE	byData[1];		// ��¼����
};

//---------------------------------------------------------------------------------------------
// ���＼��
//---------------------------------------------------------------------------------------------
struct tagSkillSave
{
	DWORD	dwID;				// ����ID
	INT		nSelfLevel;			// ����ȼ�
	INT		nLearnLevel;		// Ͷ��ȼ�
	INT		nCoolDown;		// ���ܻ���Ҫ����ȴʱ��
	INT     nProficiency;		// ����������
};

//---------------------------------------------------------------------------------------------
// ����buff�Ļ�����Ϣ
//---------------------------------------------------------------------------------------------
struct tagBuffSave
{
	DWORD		dwBuffID;					// ID
	INT32		nPersistTick;				// ��ǰ����ʱ��

	INT8		n8Level;					// �ȼ�
	INT8		n8CurLapTimes;				// ��ǰ���Ӵ���
	INT8		n8ModifierNum;				// Ӱ�����ļ��ܸ���
	INT8		n8Dummy[1];

	DWORD		dwSrcUnitID;				// ԴUnit��ID
	DWORD		dwSrcSkillID;				// ���ĸ����ܲ�������ID��
	INT64		n64Serial;					// ���ĸ���Ʒ��װ������
	DWORD		dwItemTypeID;				// ��Ʒ��װ��������ID

	BYTE		byData[1];
};

//---------------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------------
struct tagFriendSave
{
	DWORD		dwFriendID;		// ����id
	INT32		nGroupID;		// ����
};

struct tagFriendshipSave
{
	DWORD		dwFriendID;		// ����id
	INT32		nFriVal;		// �Ѻö�
};

//-----------------------------------------------------------------------------
// ���浽���ݿ����������
//-----------------------------------------------------------------------------
struct tagTitleSave
{
	UINT16			u16TitleID;			// �ƺ�ID
	DWORD			dwStateMark;		// ��Ϊ0�����ʾ�ѻ�ȡ�������ʾpara2
};

//---------------------------------------------------------------------------------------
// ��Ҹ�����������Ӧ�Ļ������Լ������ü��õ����ʺ�������һ������ľ������� -- ��xml�ж���
//---------------------------------------------------------------------------------------
struct tagLevelUpEffect
{
	INT		nExpLevelUp;		// ������һ������ľ�������
	INT16	n16RoleAtt[6];		// ��ǡ�������Ԫ������������������
	INT16	n16RoleAttAvail;	// �õ��Ŀ�Ͷ���Ե�
	INT16	n16TalentAvail;		// �õ��Ŀ�Ͷ���ʵ�
	INT16	n16HP;				// ��ǰ����Ѫ��
	INT16	n16MP;				// ��ǰ����������
	INT16	n16Vitality;		// ��ǰ�������
	INT16	n16Dummy;

	tagLevelUpEffect() { ZeroMemory(this, sizeof(*this)); nExpLevelUp = GT_INVALID; }
};


//---------------------------------------------------------------------------------------
// ��ҽű��������Ͷ���
//---------------------------------------------------------------------------------------
enum ERoleScriptData
{
	ERSD_Injury			=	0,				// 0������������״̬
	ERSD_END			=	100,
};
//-------------------------------------------------------------------------------------------
#pragma pack(pop)