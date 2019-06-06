#pragma once

#include "ItemDefine.h"
#include "pet_define.h"

#pragma pack(push,1)

//-----------------------------------------------------------------------------
// ����idת���ĺ�
//-----------------------------------------------------------------------------
#define MTransPetSkillTypeID(skillId, lv) (DWORD)(skillId * 100 + lv)
#define MTransPetSkillID(skillTypeId) (DWORD)(skillTypeId / 100)
#define MTransPetSkillLevel(skillTypeId) (INT)(skillTypeId % 100)

//-----------------------------------------------------------------------------
// �����������
//-----------------------------------------------------------------------------
const INT MAX_PET_SKILL_NUM		= 12;

const INT MAX_PET_SKILL_DATA	= 1024;

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
enum EPetskillType
{
	EPT_Null			=-1,	
	EPT_Gather			= 0,	//��ǲ�ɼ�
	EPT_PickUp			= 1,	//�Զ�ʰȡ
	EPT_MedicineFeed	= 2,	//�Զ�ιҩ
	EPT_Forage			= 3,	//�Զ���ʳ
	EPT_Experience		= 4,	//����ѵ�� 
	EPT_Deliver			= 5,	//�����ʲ�
	EPT_Sale			= 6,	//����С��
	EPT_StoreHouse		= 7,	//���޲ֹ�
	EPT_Strengthen		= 8,	//ǿ��    
	EPT_Buff			= 9,	//״̬����
	EPT_WuXing			=10,    //�츳
	EPT_MountAdd		=11,    //��˸���
};

//-----------------------------------------------------------------------------
// ��������2
//-----------------------------------------------------------------------------
enum EPetskillType2
{
	EPT2_Null		=   0,
	EPT2_Work       =   1,
	EPT2_Instant    =   2,
	EPT2_Continue   =   3
};

//-----------------------------------------------------------------------------
// ��������3
//-----------------------------------------------------------------------------
enum EPetskillType3
{
    EPT3_All        =   0,  //!< ����
    EPT3_Horse      =   1,  //!< ��
    EPT3_Mule       =   2   //!< ��
};

//-----------------------------------------------------------------------------
// ����ʩ������
//-----------------------------------------------------------------------------
enum EPetskillCastType
{
	EPCT_Passive    =   0,	// ����
    EPCT_Active		=	1,	// ����
	EPCT_Enhance	=	2,	// ����ǿ��
};

//-----------------------------------------------------------------------------
// ����ʩ��״̬
//-----------------------------------------------------------------------------
enum EPetskillCastCondition
{
    EPCC_NULL       =   0,  // ��
    EPCC_Called     =   1,  // �ٻ�
    EPCC_Rest       =   2   // ��Ϣ
};

//-----------------------------------------------------------------------------
// ���＼�ܼ�ʱ����
//-----------------------------------------------------------------------------
enum EPetTimerType
{
	EPTT_CoolDown	=	0,	// ��ȴ����
	EPTT_WorkTime	=	1,	// ����ʱ��
};

//-----------------------------------------------------------------------------
// ���＼�ܴ��Ϳͻ��˵���Ϣ�ṹ
//-----------------------------------------------------------------------------
struct tagPetSkillMsgInfo
{
    DWORD	dwTypeID;							// TypeID(���ȼ�)
    INT     nCurrCoolDown;                      // ��ǰ��ȴʱ��
    INT     nMaxCoolDown;                       // �����ȴʱ��
};

//-----------------------------------------------------------------------------
// �ɼ�����Ʒ������Ŀ
//-----------------------------------------------------------------------------
const INT NUM_ITEMS_PET_GATHER			= 5;
const INT NUM_RARE_ITEMS_PET_GATHER		= 5;

//-----------------------------------------------------------------------------
// ����ɼ���
//-----------------------------------------------------------------------------
struct tagPetGatherProto
{
	DWORD	dwTypeID;
	DWORD	dwItemTypeID[NUM_ITEMS_PET_GATHER];
	INT		nMin[NUM_RARE_ITEMS_PET_GATHER];
	INT		nMax[NUM_RARE_ITEMS_PET_GATHER];

	DWORD	dwRareItemID[NUM_RARE_ITEMS_PET_GATHER];
	INT		nProb[NUM_RARE_ITEMS_PET_GATHER];
};

//-----------------------------------------------------------------------------
// ������Ϣ�ɱ�ṹ
//-----------------------------------------------------------------------------

//! �Զ�ʰȡ
struct tagPetPickUpData
{
    INT64       n64GroundSerial;      //!< tbc:��ͼʰȡ����ƷID ��Ӧ������Ʒid��Ӧ���õ���Ψһ���к�
};

//! �Զ�ιҩ
enum E_Pet_Supply_Type
{
    EPST_Health,        //!< ����
    EPST_Power,         //!< ����
    EPST_Toughness,     //!< �־���
    EPST_Energy,        //!< ����
};
struct tagPetSupplyData
{
    E_Pet_Supply_Type   eSupplyType;    //!< �Զ�ιҩ����
    INT64               n64ItemID;      //!< ҩƷID
};

//! �Զ���ʳ
struct tagPetForageData
{
    INT64       n64ItemID;          //!< ҩƷID
};

struct tagPetWuXingItem
{
	DWORD		dwItemTypeID;
	INT			nNum;
};

//! �Զ��ɼ�(NC)
struct tagPetGatherDataNC
{
    DWORD       dwResourceID;       //!< ��Դ��ID
};

struct tagPetGatherDataNS
{
    enum E_Result
    {
        ER_Continue     = 0,
        ER_Break        = 1,
    };

    E_Result    eRet;
    DWORD       dwResourceID;       //!< ��Դ��ID
    DWORD       dwItemTypeID;       //!< �ɼ������Ʒitem type id
};

//! �ʲ�
struct tagPetMailData
{
    enum MSG_TYPE
    {
        MailType_Use    = 0,    //!< ��һ��ʹ�ü���ʱ��
        MailType_Send   = 1,    //!< ����ʱ�䵽ʱ������Ʒ�б�
    };
    
    MSG_TYPE    eMsgType;
    DWORD       dwRoleID;       //!< Ŀ���ɫID
    BYTE        byData[1];      //!< ��Ʒ�б�(64λID)
};

//! С��
struct tagPetSellData
{
	INT			nNum;			//!< ����
    BYTE        byData[1];      //!< ��Ʒ64λID�б�
};

//! ѵ��
struct tagPetExperenceData
{
	DWORD		dwTypeID;		//!< Ҫѵ������������id
};

//! �ֹ�
struct tagPetStoreData
{
	INT			nNum;			//!< ����
	BYTE        byData[1];      //!< ��Ʒ64λID�б�
};

#pragma pack(pop)