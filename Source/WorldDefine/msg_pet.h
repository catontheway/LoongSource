//-----------------------------------------------------------------------------
//! filename    msg_pet.h
//! author      hyu/xlguo
//! date        2009-3-24
//! last 
//! brief       ������Ϣ
//! sa          pet_define.h
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push,1)

#include "pet_define.h"
#include "msg_common_errorcode.h"

//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum 
{
	E_Pets_UnkownErr		=-1,
	E_Pets_Success			= 0,

	E_Pets_StateLimit               =   1,
	E_Pets_Soul_NotExist            =   2,
	E_Pets_Soul_AlreadyExist        =   3,
	E_Pets_Soul_PtrNotValid         =   4,
	E_Pets_Soul_NumExceed           =   5,
	E_Pets_Soul_PetEggErr           =   6,
	E_Pets_Soul_NameIllegal         =   7,
	E_Pets_Soul_MasterNotFound      =   8,
	E_Pets_Soul_RoleExpNotEnough    =   9,
	E_Pets_Soul_RoleMoneyNotEnough  =   10,
	E_Pets_Soul_GradeNotFit         =   11,

	E_Pets_Pet_CalledNumExceed      =   12,
	E_Pets_Pet_AlreadyCalled        =   13,
	E_Pets_Pet_NotCalled            =   14,
	E_Pets_Pet_NotExist             =   15,
	E_Pets_AlreadyMaxAptitude       =   16,
	E_Pets_QualityNotFit            =   17,
	E_Pets_EnhanceFailed            =   18,
	E_Pets_ItemNotValid             =   19,
	E_Pets_PetSkill_Master_NotExist =   21,
	E_Pets_PetSkill_InvalidSkillID  =   22,
	E_Pets_PetSkill_Skill_NotExist  =   23,
	E_Pets_PetSkill_AlreadyStarted  =   24,
	E_Pets_PetSkill_Learn_PetLvlLimit           =   25,
	E_Pets_PetSkill_Learn_HasHighLvlSkill       =   26,
	E_Pets_PetSkill_Learn_Type3NotFit           =   27,
	E_Pets_PetSkill_Learn_StageNotFit           =   28,
	E_Pets_PetSkill_Learn_LvlNotEnough          =   29,
	E_Pets_PetSkill_Learn_PotentialNotEnough    =   30,
	E_Pets_PetSkill_Learn_LvlNotRight           =   31,
	E_Pets_PetSkill_Learn_ItemInvalid           =   32,
	E_Pets_PetSkill_Learn_MaxLeanNum            =   33,
	E_Pets_PetSkill_Learn_StateLimit            =   34,
	E_Pets_PetSkill_Use_ItemNotExist            =   35,
	E_Pets_PetSkill_Use_CoolingDown             =   36,
	E_Pets_PetSkill_Use_WuXingEnergyNotEnough   =   37,
	E_Pets_PetSkill_Use_MaxTalentCount          =   38,
	E_Pets_PetSkill_Use_WorkCounting            =   39,
	E_Pets_PetSkill_Use_StateLimit              =   40,
	E_Pets_PetSkill_Use_NotActiveSkill          =   41,
	E_Pets_PetSkill_Use_SkillProtoInvalid       =   42,
	E_Pets_PetSkill_Use_SpiritNotEnough         =   43,
	E_Pets_PetSkill_Learn_NoPreSkill			=	44,
	E_Pets_Soul_CreateDBErr						=   45,		// ��������ʱ ���ݿ����
	E_Pets_Soul_CreateInitErr					=   46,		// ��������ʱ �����ݿ��ȡʱ��ʼ������
	E_Pets_PetEquip_Soul_NotExist               =   50,
	E_Pets_PetEquip_Master_NotExist             =   51,
	E_Pets_PetEquip_BagIndexNotFree             =   52,
	E_Pets_PetEquip_BagFull                     =   53,
	E_Pets_PetEquip_PetEquipNotInBag            =   54,
	E_Pets_PetEquip_UnknownEquipProto           =   55,
	E_Pets_PetEquip_IsNotPetEquip               =   56,
	E_Pets_PetEquip_PetLevelNotEnough           =   57,
	E_Pets_PetEquip_EquipIsNotOnPet             =   58,
	E_Pets_PetEquip_EquipAlreadyInPos           =   59,
	E_Pets_PetEquip_InvalidDstPos               =   60,

    E_PetPourExpNotEnough                       =   70,
	E_PetRoleLvlNotEnough						=	71,
	E_Pets_Mount_DstRoleNotFound				=	72,
	E_Pets_Mount_MountSoulNotFound				=	73,
	E_Pets_Mount_SrcRoleNotFound				=	74,
	E_Pets_Mount_DstRoleRefuse					=	75,
	E_Pets_Lock_AlreadyLocked					=	76,
	E_Pets_Lock_NotLocked						=	77,
	E_Pets_Lock_Locked							=	78,
	E_Pets_Delete_HasEquip						=	79,			// ����Я��װ�����޷�ɾ��
    E_Pets_Carrylevel_NotEnough                 =   80,
	E_Pets_Food_CoolingDown		                =   81,			// ����ʳƷ������ȴ
	E_Pets_Soul_IsWorking						=	82,			// ������

	E_Pets_PetSkill_NoMaster					=	91,			// ������
	E_Pets_PetSkill_NoBody						=	92,			// ��ʵ��
	E_Pets_PetSkill_NoMap						=	93,			// �޵�ͼ
	

	E_Pets_PetSkill_Gather_NoResource			=	94,			// ����Դ��
	E_Pets_PetSkill_Gather_NoGatherSkill		=	95,			// ����������
	E_Pets_PetSkill_Gather_NoBagSpace			=	96,			// �����ռ䲻��
	E_Pets_PetSkill_Gather_ResIsTaged			=	97,			// ��Դ�ѱ�ռ��

	E_Pets_PetSkill_Sale_NoBagItem				=	100,		// �����޴���Ʒ
	E_Pets_PetSkill_Sale_BagItemCoolingDown		=	101,		// ������Ʒ��ȴ��
	E_Pets_PetSkill_Sale_BagItemCantSale		=	102,		// ������Ʒ�޷�����
	E_Pets_PetSkill_Sale_BagLock				=	103,		// ���ұ���

	E_Pets_PetSkill_Experance_NoProductSkill	=	110,		// û����������
	
	E_Pets_PetSkill_Store_NoBagItem				=	120,		// ָ����Ʒ������
	E_Pets_PetSkill_Store_NoBagItemCoolingDown	=	121,		// ָ����Ʒ��ȴ��
	E_Pets_PetSkill_Store_BagLock				=	122,		// ��������

	E_Pets_PetSkill_Wuxing_BagFull				=	130,		// ���������޷������Ʒ
	E_Pets_PetSkill_Wuxing_ItemNotValid			=	131,		// ��Ʒ��Ч

	E_Pets_PetSkill_MasterDead					=	140,			// ��������
	E_Pets_PetSkill_RoleLvlLimit				=	141,			// ���˵ȼ�����
	E_Pets_PetSkill_MedicineCooldown			=	142,			// ҩƷ��ȴ��
	E_Pets_Soul_IngegrateErr					=	150,		// ���ϵ�roleʱ����

    E_Pets_Skill_MountingLimit                  =   300,        // ����״̬���޷�ʹ�ø����޼���
    E_Pets_Skill_UnMountingLimit                =   301,        // �Ǽ���״̬���޷�ʹ�ø����޼���
    E_Pets_Skill_PreparingLimit                 =   302,        // Ԥ������״̬���޷�ʹ�ø����޼���
    E_Pets_Skill_UnPreparingLimit               =   303,        // ��Ԥ������״̬���޷�ʹ�ø����޼���
    E_Pets_Skill_WorkingLimit                   =   304,        // ����״̬���޷�ʹ�ø����޼���
    E_Pets_Skill_UnWorkingLimit                 =   305,        // �ǹ���״̬���޷�ʹ�ø����޼���
    E_Pets_Skill_CalledLimit                    =   306,        // �ٻ�״̬���޷�ʹ�ø����޼���
    E_Pets_Skill_UnCalledLimit                  =   307,        // ���ٻ�״̬���޷�ʹ�ø����޼���

    E_PetErrorEnd,
};



//------------------------------------------------------------------------------

//! \struct  tagNC_GetPetAttr
//! \brief   ��ó�������
CMD_START(NC_GetPetAttr)
    DWORD           dwRoleID;       //!< ��ɫID
    DWORD           dwPetID;        //!< ����ID(GT_INVALIDʱΪȡ��������г���)
CMD_END

//! \struct  tagNS_GetPetAttr
//! \brief   ��ó�������
CMD_START(NS_GetPetAttr)
    DWORD           dwRoleID;       //!< ���ID
    tagPetInitAttr	petAttr;        //!< ����ṹ
CMD_END

//! \struct  tagNC_DeletePet
//! \brief   ɾ������
CMD_START(NC_DeletePet)
    DWORD           dwPetID;        //!< ����id
CMD_END

CMD_START(NS_DeletePet)
    DWORD           dwErrCode;      //!< ������
    DWORD           dwPetID;        //!< ����id
CMD_END

//------------------------------------------------------------------------------

//! \struct  tagNC_GetPetDispInfo
//! \brief   ���ĳһ�������ֵ��ʾ��Ϣ(��Ҫ���ػ�����)
CMD_START(NC_GetPetDispInfo)
    DWORD           dwRoleID;                   //!< ��ɫID
    DWORD           dwPetID;                    //!< ��ɫӵ�еĳ���ID
CMD_END

//! \struct  tagNS_GetPetDispInfo
//! \brief   ͬ��,����
CMD_START(NS_GetPetDispInfo)
    DWORD           dwPetID;                    //!< ����ID
    tagPetDispInfo  DispData;                   //!< ��Ϣ
CMD_END

//! \struct  tagNS_PetDispInfoChange
//! \brief   ����ͬ����ʾ��Ϣ����ʱ�ľŹ���㲥,��������,�ɳ��׶α��ʱ�㲥
CMD_START(NS_PetDispInfoChange)
    DWORD           dwPetID;                    //!< ����ID
    tagPetDispInfo  DispData;                   //!< ��Ϣ
CMD_END

//-----------------------------------------------------------------------------
//  ���ؽ�ɫ�������
//-----------------------------------------------------------------------------

CMD_START(NC_UsePetEgg)
    INT64           n64ItemID;                  //!< ���ﵰid
    TCHAR           szPetName[X_SHORT_NAME];    //!< �����ĳ�����
CMD_END

CMD_START(NS_UsePetEgg)
    INT64           n64ItemID;                  //!< ���ﵰ64λid
    DWORD			dwErrCode;                  //!< ������
CMD_END

//! \struct  tagNS_PetAttrChange
//! \brief   ͬ�����ؽ�ɫ������ֵ���Ա仯,���Ҫ��ʵʱ��ǿ�������ֶ�,ͨ�������������仯֪ͨ
CMD_START(NS_PetAttrChange)
    DWORD           dwPetID;        //!< ����ID
    INT8            u8AttrType;     //!< �����������(ECSPetAtt)
    INT32           u32NewValue;    //!< ������ֵ
CMD_END

//! \struct tagNC_SetPetState
CMD_START(NC_SetPetState)
    DWORD           dwPetID;        //!< ����ID
    UINT8           ePetState;      //!< ״̬����(EPetStateType)
    bool            bVal;           //!< ״ֵ̬
CMD_END

//! \struct tagNS_SetPetState
CMD_START(NS_SetPetState)
	DWORD           dwPetID;        //!< ����ID
	UINT8           ePetState;      //!< ״̬����(EPetStateType)
	DWORD			dwErrCode;
CMD_END

//! \struct tagNC_PetPourExp
//! \brief  ��ע����
CMD_START(NC_GetPetPourExpMoneyNeed)
	DWORD           dwPetID;        //!< ����ID
CMD_END

//! \struct tagPetNS_PourExp
CMD_START(NS_GetPetPourExpMoneyNeed)
	DWORD           dwPetID;        //!< ����id
	INT64			n64MoneyNeed;
CMD_END


//! \struct tagNC_PetPourExp
//! \brief  ��ע����
CMD_START(NC_PetPourExp)
    DWORD           dwPetID;        //!< ����ID
	INT64			n64ItemID;		//!< 64λ��ƷID
CMD_END

//! \struct tagNS_PetPourExp
CMD_START(NS_PetPourExp)
    DWORD           dwErrCode;      //!< ������
    DWORD           dwPetID;        //!< ����id
CMD_END

// ��һ�οͻ��˷�n64itemid��Ч
// ��һ�η�������ndststep��ndststep��Ч
// �ڶ��οͻ��˷�ndststep��ndststep��Ч n64itemid��Ч
//! \struct tagNC_PetUpStep
//! \brief  ����
CMD_START(NC_PetUpStep)
	INT64			n64ItemID;
	INT				nDstStep;
	DWORD			dwSkillID;
CMD_END

//! \struct tagNC_PetUpStep
CMD_START(NS_PetUpStep)
	DWORD           dwErrCode;      //!< ������
	INT64			n64ItemID;
	INT				nDstStep;
	DWORD			dwSkillID;
CMD_END


//! \struct tagNC_PetEnhance
//! \brief  ��������
CMD_START(NC_PetEnhance)
	INT64			n64ItemID;
CMD_END

//! \struct tagNS_PetEnhance
CMD_START(NS_PetEnhance)
	DWORD           dwErrCode;      //!< ������
	INT64			n64ItemID;
CMD_END

//! \struct tagNC_PetFood
//! \brief  ʹ��ҩƷ
CMD_START(NC_PetFood)
    INT64			n64ItemID;
CMD_END

//! \struct tagNS_PetFood
CMD_START(NS_PetFood)
    DWORD           dwErrCode;      //!< ������
    INT64			n64ItemID;
CMD_END

//! \struct tagNS_Mount
//! \���
CMD_START(NS_Mount)
	DWORD	dwRoleID;
	DWORD	dwPetTypeID;
	DWORD	dwPetID;
CMD_END

//! \struct tagNS_Mount2
//! \�ڶ������
CMD_START(NS_Mount2)
	DWORD	dwPetID;
	DWORD	dwRoleID;
CMD_END

// �������
// ����Ԧ��->������	NC_MountInvite 
// ������->��������	NS_MountOnInvite
// ��������->������	NC_MountOnInvite
// ������->����Ԧ��	NS_MountInvite

//! \struct tagNS_MountInvite
//! \������
CMD_START(NS_MountOnInvite)
	DWORD	dwSrcRoleID;
CMD_END

CMD_START(NC_MountOnInvite)
	DWORD	dwSrcRoleID;
	BOOL	bReceive;
CMD_END

//! \struct tagNS_MountInvite
//! \����
CMD_START(NC_MountInvite)
	DWORD	dwDstRoleID;
CMD_END

CMD_START(NS_MountInvite)
	DWORD	dwDstRoleID;
	DWORD	dwErrCode;
CMD_END

//! \struct tagNS_PetSetLock
//! \����
CMD_START(NC_PetSetLock)
	DWORD	dwPetID;
	INT64	n64ItemID;
	bool	bSet;
CMD_END

CMD_START(NS_PetSetLock)
	DWORD	dwPetID;
	DWORD	dwErrCode;
CMD_END


#pragma pack(pop)
