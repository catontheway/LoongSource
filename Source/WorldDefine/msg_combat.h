#pragma once
#include "msg_common_errorcode.h"
#include "buff_define.h"
#include "move_define.h"

#pragma pack(push, 1)
//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum
{
	E_UseSkill_SkillNotExist		=	1,		// ���ܲ�����
	E_UseSkill_TargetInvalid		=	2,		// Ŀ��Ƿ�
	E_UseSkill_PassiveSkill			=	3,		// �������ܲ���ʹ��
	E_UseSkill_SkillTargetInvalid	=	4,		// ���ܱ����������ս��Ŀ��
	E_UseSkill_CoolDowning			=	5,		// ����������ȴ
	E_UseSkill_Operating			=	6,		// �������ڷ���
	E_UseSkill_UseLimit				=	7,		// ��������������
	E_UseSkill_SexLimit				=	8,		// �Ա�����
	E_UseSkill_PosLimitFront		=	9,		// λ�ò����㣨��������ǰ��
	E_UseSkill_PosLimitBack			=	10,		// λ�ò����㣨���������
	E_UseSkill_DistLimit			=	11,		// ���벻����
	E_UseSkill_RayLimit				=	12,		// ���߼�ⲻ����
	E_UseSkill_CostLimit			=	13,		// ���Ĳ���
	E_UseSkill_TargetLimit			=	14,		// Ŀ������
	E_UseSkill_StallLimit			=	15,		// ��̯״̬�²���ʹ�ü���
	E_UseSkill_WeaponLimit			=	16,		// ��������
	E_UseSkill_SelfStateLimit		=	17,		// ����״̬����
	E_UseSkill_TargetStateLimit		=	18,		// Ŀ��״̬����
	E_UseSkill_SelfBuffLimit		=	19,		// ����Buff����
	E_UseSkill_TargetBuffLimit		=	20,		// Ŀ��Buff����
	E_UseSkill_Mount_NoPreparingPet	=	21,		// û��Ԥ������
	E_UseSkill_VocationLimit		=	22,		// ְҵ����
	E_UseSkill_MapLimit				=	23,		// �޷��ڸõ�ͼ��ʹ��

	E_UseItem_ItemNotExist			=	30,		// ��Ʒ������
	E_UseItem_TargetInvalid			=	31,		// Ŀ��Ƿ�
	E_UseItem_ItemCanNotUse			=	32,		// ��Ʒ����ʹ��
	E_UseItem_CoolDowning			=	33,		// ��Ʒ������ȴ
	E_UseItem_SexLimit				=	34,		// �Ա�����
	E_UseItem_LevelLimit			=   35,		// �ȼ�����
	E_UseItem_DistLimit				=	36,		// ���벻����
	E_UseItem_RayLimit				=	37,		// ���߼�ⲻ����
	E_UseItem_UseLimit				=	38,		// ����������,����ʹ��
	E_UseItem_Operating				=	39,		// ��Ʒ���ڱ�ʹ��
	E_UseItem_NotEnoughSpace		=   40,		// �����ռ䲻��
	E_UseItem_ReAcceptQuest			=	41,		// ���Ѿ���ȡ��������
	E_UseItem_ActivityLimit			=	42,		// �õ�����ֻ���ڹ̶����ʹ��
	E_UseItem_PosLimit				=	43,		// �����ڸ�λ��ʹ�ø���Ʒ
	E_UseItem_SelfStateLimit		=	44,		// �������״̬����
	E_UseItem_AcceptSimQuest		=	45,		// �������Ѿ���ͬ������
	E_UseItem_VocationLimit			=	46,		// ְҵ������Ʒʹ��
	E_UseItem_MapLimit				=	47,		// �޷��ڸõ�ͼ��ʹ��
	E_UseItem_SpecFunError			=	48,		// ��Ʒ������ʹ�����Ͳ�ƥ��

	E_Revive_Unknown				=	50,		// δ֪��������
	E_Revive_NotDead				=	51,		// ��������״̬
	E_Revive_CanNotTransmit			=	52,		// �سǸ���ʱ����ʧ��
	E_Revive_ItemLimit				=	53,		// ԭ�ظ���ʱȱ����Ӧ����
	E_Revive_CanNotAccept			=	54,		// ���ܸ���ʱʧ��
	E_Revive_MapNotFound			=	55,		// û���ҵ������ͼ

    E_SkillMsg_NoMsg                =   999,    // ����ʾ
};

//----------------------------------------------------------------------------
// ��ɫHP�仯ԭ��
//----------------------------------------------------------------------------
enum ERoleHPChangeCause
{
	ERHPCC_SkillDamage,				//�����˺�����
	ERHPCC_BuffDamage,				//Buff�˺�����
	ERHPCC_Other,					//����ԭ��
};

//----------------------------------------------------------------------------
// ��ɫ����ԭ��
//----------------------------------------------------------------------------
enum ERoleDeadCause
{
	ERDC_Skill,						//�����˺�����
	ERDC_Buff,						//Buff�˺�����
	ERDC_Other,						//����ԭ��
};

//----------------------------------------------------------------------------
// ����Ŀ��ԭ��
//----------------------------------------------------------------------------
enum EHitTargetCause
{
	EHTC_Skill,						//ʹ�ü���
	EHTC_Item,						//ʹ����Ʒ
};

//----------------------------------------------------------------------------
// ������Ч����
//----------------------------------------------------------------------------
enum ESceneEffectType
{
	ESET_ByObjID,					//ʹ��ObjID���в���
	ESET_ByPos,						//��ָ��λ�ò���
};

//----------------------------------------------------------------------------
// ������Ч����
//----------------------------------------------------------------------------
enum ESceneMusicType
{
	ESMT_2D,						//2D����
	ESMT_3D,						//3D��Ч
};

//----------------------------------------------------------------------------
// ����/�뿪ս��
//----------------------------------------------------------------------------
CMD_START(NC_EnterCombat)
CMD_END

CMD_START(NC_LeaveCombat)
CMD_END

//----------------------------------------------------------------------------
// ���ܹ���
//----------------------------------------------------------------------------
CMD_START(NC_Skill)			
	Vector3		vSrcPos;			//�����ߵ�ǰ����
	DWORD		dwTargetRoleID;		//Ŀ��ID
	DWORD		dwSkillID;			//����ID
	DWORD		dwSerial;			//��Ϣ���к�
CMD_END

CMD_START(NS_Skill)			
	DWORD		dwSrcRoleID;		//������ID
	DWORD		dwTarRoleID;		//Ŀ��ID
	DWORD		dwSkillID;			//����ID
	DWORD		dwSerial;			//��Ϣ���к�
	INT			nSpellTime;			//����ʱ�䣬���������ļ���Ϊ0
	DWORD		dwErrorCode;		//�������
CMD_END


//----------------------------------------------------------------------------
// ���ܴ��
//----------------------------------------------------------------------------
CMD_START(NC_SkillInterrupt)		//��ϼ��ܻ�����	
	DWORD		dwSkillID;			//����ID
CMD_END

CMD_START(NS_SkillInterrupt)		//��ϼ��ܻ�����	
	DWORD		dwRoleID;			//������ID
	DWORD		dwSkillID;			//����ID
CMD_END

//----------------------------------------------------------------------------
// ʹ����Ʒ
//----------------------------------------------------------------------------
CMD_START(NC_UseItem)
	Vector3		vSrcPos;			//��Ʒʹ���ߵ�ǰ����
	DWORD		dwTargetRoleID;		//Ŀ��ID
	INT64		n64ItemID;			//��Ʒ64λID
	DWORD		dwSerial;			//��Ϣ���к�
CMD_END

CMD_START(NS_UseItem)			
	DWORD		dwSrcRoleID;		//��Ʒʹ����ID
	DWORD		dwTarRoleID;		//Ŀ��ID
	INT64		n64ItemID;			//��Ʒ64λID
	DWORD		dwTypeID;			//��ƷTypeID
	DWORD		dwSerial;			//��Ϣ���к�
	INT			nSpellTime;			//����ʱ�䣬������������ƷΪ0
	DWORD		dwErrorCode;		//�������
	bool		bInmmediate;		//�Ƿ�˲����
CMD_END

//----------------------------------------------------------------------------
// ʹ����Ʒ���
//----------------------------------------------------------------------------
CMD_START(NC_UseItemInterrupt)		//�������	
	INT64		n64ItemID;			//��Ʒ64λID
CMD_END

CMD_START(NS_UseItemInterrupt)		//�������	
	DWORD		dwRoleID;			//������ID
	INT64		n64ItemID;			//��Ʒ64λID
	DWORD		dwTypeID;			//��ƷTypeID
CMD_END


//----------------------------------------------------------------------------
// ʹ�ü��ܻ���Ʒ����Ŀ��
//----------------------------------------------------------------------------
CMD_START(NS_HitTarget)
	DWORD		dwRoleID;			//��ɫID
	DWORD		dwSrcRoleID;		//������ID
	EHitTargetCause	eCause;			//ԭ��
	DWORD		dwMisc;				//����Ǽ��ܣ���Ϊ����ID�������ʹ����Ʒ����Ϊ��ƷTypeID
	DWORD		dwSerial;			//����Ǽ��ܣ���Ϊ������Ϣ���кţ��������Ʒ��Ϊʹ����Ʒ��Ϣ���к�
CMD_END


//----------------------------------------------------------------------------
// HP�ı���ʾ��Ϣ
//----------------------------------------------------------------------------
CMD_START(NS_RoleHPChange)
	DWORD		dwRoleID;			//��ɫID
	ERoleHPChangeCause	eCause;		//HP�仯ԭ��
	bool		bMiss;				//�Ƿ�Miss
	bool		bCrit;				//�Ƿ񱩻�
	bool		bBlocked;			//�Ƿ񱻸�
	INT			nHPChange;			//HP�仯ֵ
	DWORD		dwSrcRoleID;		//������ID
	DWORD		dwMisc;				//����ԭ��������
	DWORD		dwMisc2;			//����ԭ��������
	DWORD		dwSerial;			//����ԭ�������壬����Ǽ��ܣ���Ϊ������Ϣ���кţ�����ԭ����GT_INVALID
CMD_END

//----------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------
enum ERoleReviveType
{
	ERRT_Start			= 0,

	ERRT_ReturnCity		= 0,		// �سǸ���
	ERRT_Locus			= 1,		// ԭ�ظ���
	ERRT_Accept			= 2,		// ���ܸ���
	ERRT_Prison			= 3,		// ���и���

	ERRT_None,						//�޽���
	ERRT_End			= ERRT_None,
	
};

//----------------------------------------------------------------------------
// ��ɫ������Ϣ
//----------------------------------------------------------------------------
CMD_START(NS_RoleDead)
	DWORD		dwRoleID;			//��ɫID
	ERoleDeadCause eCause;			//����ԭ��
	DWORD		dwSrcRoleID;		//������ID
	DWORD		dwMisc;				//����Ǽ��ܣ���Ϊ����ID�������buff����Ϊbuff id
	DWORD		dwMisc2;			//����Ǽ��ܣ���Ϊ�ܵ����к�
	DWORD		dwSerial;			//����Ǽ��ܣ���Ϊ������Ϣ���кţ�����ԭ����GT_INVALID
	ERoleReviveType eSuggestRevive;//���ʽ����ΪERRT_None ��������Լ�ѡ��
CMD_END



CMD_START(NC_RoleRevive)
	ERoleReviveType	eType;
	INT64			n64ItemID;		// ԭ�ظ����������Ʒ64λID
CMD_END

CMD_START(NS_RoleRevive)			// ��Ҫ����Χ��ҷ���
	DWORD			dwErrorCode;
	DWORD			dwRoleID;
CMD_END

CMD_START(NS_RoleReviveNotify)
CMD_END

//----------------------------------------------------------------------
// ���ɾ������һ��Buff
//----------------------------------------------------------------------
CMD_START(NS_AddRoleBuff)
	DWORD			dwRoleID;			// ID
	tagBuffMsgInfo	Buff;				// Buff����
CMD_END

CMD_START(NS_RemoveRoleBuff)
	DWORD			dwRoleID;			// ID
	DWORD			dwBuffTypeID;		// Buff����ID
CMD_END

CMD_START(NS_UpdateRoleBuff)
	DWORD			dwRoleID;			// ID
	tagBuffMsgInfo	Buff;				// Buff����
CMD_END

//----------------------------------------------------------------------------
// �ͻ���ȡ��һ��Buff
//----------------------------------------------------------------------------
CMD_START(NC_CancelBuff)
	DWORD			dwBuffTypeID;
CMD_END

//----------------------------------------------------------------------------
// �ͻ���ֹͣ������Ϊ
//----------------------------------------------------------------------------
CMD_START(NS_StopAction)
	DWORD			dwRoleID;
	Vector3			curPos;
	Vector3			faceTo;
	bool			bSwim;
CMD_END


//----------------------------------------------------------------------------
// �������ս�������ڿͻ��˲�����Ч
//----------------------------------------------------------------------------
CMD_START(NS_MonsterEnterCombat)
	DWORD			dwRoleID;
CMD_END

//----------------------------------------------------------------------------
// �����ƶ������ˣ���棬˲�Ƶȣ�
//----------------------------------------------------------------------------
CMD_START(NS_SpecialMove)
	ESpecialMoveType	eType;          // �ƶ�����
	DWORD				dwRoleID;       // ��ɫID
	Vector3				vDestPos;       // Ŀ���
CMD_END


//----------------------------------------------------------------------------
// ���ﲥ��һ������
//----------------------------------------------------------------------------
CMD_START(NS_MonsterPlayAction)
	DWORD			dwRoleID;				// ��ɫID
	DWORD			dwActionFourCC;			// ����FourCC
CMD_END


//----------------------------------------------------------------------------
// ����˵��
//----------------------------------------------------------------------------
CMD_START(NS_MonsterSay)
	DWORD				dwRoleID;			// ��ɫID
	DWORD				dwSayID;			// ˵������ID����ӦMonsterSay.xml�е�һ����¼
CMD_END


//----------------------------------------------------------------------------
// ����һ��������Ч
//----------------------------------------------------------------------------
CMD_START(NS_PlaySceneEffect)
	ESceneEffectType	eType;				// ������Ч����
	DWORD				dwObjID;			// ��ЧObjID
	Vector3				vPos;				// ��������
	DWORD				dwEffectID;			// ��ЧID��������Ч�ļ�����ʽ��SCN+��ЧID��������Ч�����ָ��Ŀ¼
CMD_END

//----------------------------------------------------------------------------
// ֹͣһ��������Ч(���޵�ͼobj��ʽ)
//----------------------------------------------------------------------------
CMD_START(NS_StopSceneEffect)
    DWORD           dwObjID;                // ��ЧobjID
CMD_END

//----------------------------------------------------------------------------
// ����һ���������ֻ���Ч
//----------------------------------------------------------------------------
CMD_START(NS_PlaySceneMusic)
	ESceneMusicType		eType;				// �������ֻ���Ч����
	Vector3				vPos;				// 3D��Ч����
	DWORD				dwMusicID;			// ���ֻ���ЧID��SCN+��ЧID ��ӦSoundTable.xml�е�һ����¼
CMD_END


#pragma pack(pop)