#pragma once
#include "..\WorldDefine\skill_define.h"

class Player;
class Role;
class LocalPlayer;

struct tagSkillData;

/** \class CombatActionChecker
	\brief ս����Ϊ���
*/
class CombatActionChecker
{
public:
	enum EErrCode
	{
		EEC_CannotAttack=1,
		EEC_CannotAttackSafeguard=2,
		EEC_PVP=3,
		EEC_Passive=4,
		EEC_InvalidTarget=5,
		EEC_NoTarget=6,
		EEC_TargetNotExist=7,
		EEC_TargetIsDead=8,
		EEC_TargetBlocked=9,
		EEC_SelfStateLimit=10,
		EEC_HPNotEnough=11,
		EEC_MPNotEnough=12,
		EEC_RageNotEnough=13,//ŭ������
		EEC_WeaponLimit=14,
		EEC_SwimLimit=15,//��Ӿ״̬�޷����иò���
		EEC_NoSkill=16,
		EEC_PrisonTransLimit=17,//�������в���ʹ�ô��������
		EEC_TooFar=18,//����̫Զ
		EEC_SkillBuffLimit=19,//����ʩ����������
        EEC_CanNotUseItem=20,//����ʹ����Ʒ
	};
public:
	CombatActionChecker(void);
	~CombatActionChecker(void);

	void Init();
	void Destroy();

	bool IfCanMove(bool bShowErrMsg);
	bool IfCanTalkNPC(bool bShowErrMsg);
	bool IfCanPickGroundItem(bool bShowErrMsg);
	bool IfCanUseSkill(DWORD skillID,DWORD targetID,bool bShowErrMsg);
	bool IfCanUseItem(DWORD itemID,DWORD targetID,bool bShowErrMsg);
	bool IfCanAttack(DWORD targetID);
	bool IfCanPlayStyleAction(bool bMutual, DWORD dwFriendVal, DWORD targetID,bool bShowErrMsg);

	/** Ŀ�����߼��
		\return ���ͨ������true
	*/
	bool RayCollideTarget(Role* pTarget);

	/** ���Ŀ���Ƿ���Ч
	*/
	bool IfTargetIsValid(DWORD targetID,DWORD dwTargetLimit,BOOL bHostile,BOOL bFriendly,BOOL bIndependent);

	static CombatActionChecker* Inst();
private:
	/** �Ƿ���PVP����
	*/
	bool IsInPVPArea(Player* pTarget);
	/** �Ƿ��ڰ�ȫ����
	*/
	bool IsInSafeArea(Player * pTarget);
	/** �Ƿ���ս������
	*/
	bool IsInArena(Player * pTarget);
	/** ����Ŀ������
	*/
	DWORD GetTargetType(Role* pTarget);
	/** �ж�Ŀ���Ƿ��ǵз�
	*/
	bool IsEnemy(Role* pTarget,DWORD targetType);
	/** �ж�Ŀ���Ƿ����ѷ�
	*/
	bool IsFriend(Role* pTarget,DWORD targetType);
	/** �ж�Ŀ���Ƿ���������
	*/
	bool IsIndependent(Role* pTarget,DWORD targetType);
	/** �����ɫ״̬��־λ
	*/
	DWORD GetRoleStateFlag(Role* pRole);

	/** �õ�ʹ��һ�μ��ܣ�Ӧ�û��Ѷ��ٶ���
	*/
	INT GetCostByType( LocalPlayer* pSelf, const tagSkillData* pSkillData, ESkillCostType eSCT );
};
