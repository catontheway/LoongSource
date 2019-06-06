//-----------------------------------------------------------------------------
//! filename    pet_define.h
//! author      hyu/xlguo
//! date        2009-3-24
//! last        2009-3-25
//! brief       ������ض���
//! sa 
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

//! ����ģ����̬����
enum EPetModelType
{
    EPMT_Follow  =   0,  //!< ����
    EPMT_Mount,         //!< ���
    //EPMT_Fly,	        //!< ����
    EPMT_NUM
};

//! ����״̬
enum EPetState
{
	EPS_Called		= 0x01,
	EPS_Working		= 0x02,
	EPS_Preparing	= 0x04,
	EPS_Mounting	= 0x08,
};

enum EPetStateFlag
{
	EPSF_Mounting		= 0x0001,
	EPSF_UnMounting		= 0x0002,
	EPSF_Preparing		= 0x0004,
	EPSF_UnPreparing	= 0x0008,
	EPSF_Working		= 0x0010,
	EPSF_UnWorking		= 0x0020,
	EPSF_Called			= 0x0040,
	EPSF_UnCalled		= 0x0080,
};


//! ����״̬
union UPetState
{
	struct
	{
		bool bisCalled      : 1;
		bool bisWorking     : 1;
		bool bisPreparing   : 1;
		bool bisRiding      : 1;
	};
	BYTE	byPetState;
};


// �����ʼ������
struct tagPetInitAttr
{
    DWORD           dwID;                       //!< ���������ʱ,����Ϸ�������ɵ�ΨһID
    DWORD           dwTypeID;                   //!< ��������ID,��ӦtagPetProto�е�ID
    UINT8           nQuality;                   //!< Ʒ��
    INT16           nAptitude;                  //!< ����
    INT16           nSpiritMax;                 //!< �������ֵ
    INT16           nSpirit;                    //!< ����
    INT16           nPotential;                 //!< Ǳ��
    INT16           nWuXingEnergy;              //!< ������(���9999)
    UINT8           nLevel;                     //!< ����ȼ�
    INT             nExpCurrent;                //!< ��ǰ����
    INT             nExpLevelUp;                //!< ��������
    INT             nMountSpeed;                //!< ����ٶ�
    INT             nBagGrid;                   //!< ���Ҹ���
    INT8            byPetState;                 //!< ����״̬(EPetState)
    INT8            nMountNum;                  //!< ��ǰ�����Ŀ
    INT8            nMountNumMax;               //!< ��������Ŀ
	bool			bLocked;					//!< �Ƿ�����
    INT8            nPetEquipNum;	            //!< ����װ����
    INT8            nPetSkillNum;	            //!< ���＼����
    BYTE			byData[1];                  //!< ����Ϊ����װ���ṹtagPetEquipMsgInfo,���м��ܽṹtagPetSkillMsgInfo
};

// �軺�屾�صĳ�����Ϣ
struct tagPetDispInfo
{
    TCHAR           szName[X_SHORT_NAME];       //!< ������
    DWORD	        dwTypeID;                   //!< ����ԭ��id
};


// ͨѶ�ó�������ö��
enum ECSPetAtt
{
	ECSPA_Null				= -1,
	ECSPA_Begin				= 0,

	ECSPA_nQuality          = 0,    //!< Ʒ��
	ECSPA_nAptitude         = 1,    //!< ����
    ECSPA_nSpiritMax        = 2,    //!< �������
	ECSPA_nSpirit           = 3,    //!< ����
    ECSPA_nPotential        = 4,    //!< Ǳ��
	ECSPA_nWuXingEnergy     = 5,    //!< ������
	ECSPA_nLevel            = 6,    //!< ����ȼ�
	ECSPA_nExpCurrent       = 7,    //!< ��ǰ����
	ECSPA_nExpLevelUp       = 8,    //!< ��������
	ECSPA_nMountSpeed       = 9,    //!< ����ٶ�
    ECSPA_BagGrid           = 10,    //!< ���Ҹ���
	ECSPA_PetState          = 11,   //!< ����״̬(EPetState)
	ECSPA_nMountNum			= 12,	//!< ��ǰ�������
	ECSPA_nMountNumMax		= 13,	//!< ����������
	ECSPA_bLocked			= 14,	//!< �Ƿ�����

	ECSPA_End,
};

// ����������ü��������Ŀ
CONST INT MAX_PET_STEP_UPGRADE_SKILL_NUM		= 6;


// ��������ԭ��
struct tagPetLvlUpProto
{
	DWORD	dwVLevel;
	INT		nStep;
	INT		nGrade;
	INT		nExpLvlUpNeed;
	INT		nMoneyRatePourExpNeed;
};

// �������׵��߱�
struct tagPetLvlUpItemProto
{
	DWORD	dwTypeID;
	INT		dwSkillIDs[MAX_PET_STEP_UPGRADE_SKILL_NUM];
};

const INT ECSPA_Num = ECSPA_End - ECSPA_Begin;

#define EPATOERPA(n)	(ECSPetAtt)(n)

// ��
const INT MIN_PET_STEP	= 0;								// ������С��
const INT MAX_PET_STEP	= 9;								// ��������
const INT NUM_PET_STEP	= MAX_PET_STEP - MIN_PET_STEP + 1;	// �������
#define IS_PET_STEP(nlvl)	((nlvl) >= MIN_PET_STEP && (nLvl) <= MAX_PET_STEP)

// ��
const INT MIN_PET_GRADE	= 1;								// ������С��
const INT MAX_PET_GRADE	= 9;								// ��������
const INT NUM_PET_GRADE	= MAX_PET_GRADE - MIN_PET_GRADE + 1;// �������
#define IS_PET_GRADE(nlvl)	((nlvl) >= MIN_PET_GRADE && (nLvl) <= MAX_PET_GRADE)

// ����ȼ�
const INT MIN_PET_VLEVEL	= 1;
const INT MAX_PET_VLEVEL	= 90;
const INT NUM_PET_VLEVEL	= MAX_PET_VLEVEL - MIN_PET_VLEVEL + 1;// ��������ȼ�
#define IS_PET_VLEVEL(nlvl)	((nlvl) >= MIN_PET_VLEVEL && (nLvl) <= MAX_PET_VLEVEL)

#define MMID(val, min, max)		((val) = ((val) > (max)) ? (max) : (((val) < (min)) ? (min) : val))
// ����ȼ����׵ȵ�ת��
inline VOID TransVLevel2StepGrade( INT nVLevel, INT &nStep, INT &nGrade )
{
	MMID(nVLevel, MIN_PET_VLEVEL, MAX_PET_VLEVEL);
	nVLevel -= 1;
	nStep	= nVLevel / NUM_PET_GRADE;
	nGrade	= nVLevel % NUM_PET_GRADE + 1;
}
// �׵ȵ�����ȼ���ת��
inline VOID TransStepGrade2VLevel( INT nStep, INT nGrade, INT &nVLevel )
{
	MMID(nStep, MIN_PET_STEP, MAX_PET_STEP);
	MMID(nGrade, MIN_PET_GRADE, MAX_PET_GRADE);
	nGrade	-= 1;
	nVLevel = nStep * NUM_PET_GRADE + nGrade;
	nVLevel += 1;
}
#undef MMID

// ��Զ��������˾���
const FLOAT MAX_MOUNTINVITE_DISTANCE = 10 * 50.0f;	

#pragma pack(pop)