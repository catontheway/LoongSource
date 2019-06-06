#pragma once
#include "..\WorldDefine\move_define.h"
#include "..\WorldDefine\msg_combat.h"

/** ����ƶ�/��Ӿ�¼�
*/
struct tagMouseMoveEvent : public tagGameEvent
{
	Vector3					start;			//���
	Vector3					end;			//�յ�
	float					validDist;		//��Ч����
	bool					bUserCtrl;		//�Ƿ��û�����
	bool					bNeedSendStopCmd;	//�Ƿ���Ҫ����ֹͣ��Ϣ
	
	tagMouseMoveEvent():tagGameEvent(_T("tagMouseMoveEvent"),NULL),bUserCtrl(true),validDist(0.0f),bNeedSendStopCmd(false)
	{}
	tagMouseMoveEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) 
		: tagGameEvent(szEventName,pSenderFrame),bUserCtrl(true),validDist(0.0f),bNeedSendStopCmd(false)
	{}
};

/** ����ƶ�/��Ӿ�����¼�
*/
struct tagMouseMoveFinishEvent : public tagGameEvent
{
	bool					bSuccessed;			//�Ƿ�ɹ�

	tagMouseMoveFinishEvent():tagGameEvent(_T("tagMouseMoveFinishEvent"),NULL),bSuccessed(true)
	{}
	tagMouseMoveFinishEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame),bSuccessed(true)
	{}
};

/** �����ƶ�/��Ӿ�¼�
*/
struct tagKeyboardMoveEvent : public tagGameEvent
{
	Vector3					start;			//���
	EWalkDir				dir;			//����

	tagKeyboardMoveEvent():tagGameEvent(_T("tagKeyboardMoveEvent"),NULL)
	{}
	tagKeyboardMoveEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ֹͣ�����ƶ�/��Ӿ�¼�
*/
struct tagStopKeyboardMoveEvent : public tagGameEvent
{
	tagStopKeyboardMoveEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ��Ծ�¼�
*/
struct tagJumpEvent : public tagGameEvent
{
	Vector3					start;			//���
	Vector3					dir;			//����

	tagJumpEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{
		dir=MathConst::Zero3;
	}
};

/** �����¼�
*/
struct tagDropEvent : public tagGameEvent
{
	Vector3					start;			//���
	Vector3					dir;			//����

	tagDropEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ��ֱ�����¼�
*/
struct tagVDropEvent : public tagGameEvent
{
	Vector3					start;			//���

	tagVDropEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** �����¼�
*/
struct tagSlideEvent : public tagGameEvent
{
	Vector3					start;			//���

	tagSlideEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};


/** ʹ�ü�����Ϸ�¼�
*/
struct tagUseSkillEvent : public tagGameEvent
{
	DWORD					dwTargetRoleID;
	DWORD					dwSkillID;

	tagUseSkillEvent():tagGameEvent(_T("tagUseSkillEvent"),NULL)
	{}
	tagUseSkillEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ��NPC�Ի���Ϸ�¼�
*/
struct tagTalkNPCEvent : public tagGameEvent
{
	DWORD					dwNPCID;
	DWORD					dwTypeID;

	tagTalkNPCEvent():tagGameEvent(_T("tagTalkNPCEvent"),NULL)
	{}
	tagTalkNPCEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ��ʾ��ɫͷ��HP�仯��Ϣ��Ϸ�¼�
*/
struct tagShowHPChangeEvent : public tagGameEvent
{
	DWORD					dwRoleID;		//��ɫID
	ERoleHPChangeCause		eCause;			//�仯ԭ��
	int						nHPChange;		//HP�仯ֵ
	DWORD					dwSrcRoleID;	//�����߽�ɫID
	bool					bMiss;			//�Ƿ�Miss
	bool					bCrit;			//�Ƿ񱩻�
	bool					bBlocked;		//�Ƿ񱻸�

	tagShowHPChangeEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ����Ŀ����Ϸ�¼�
*/
struct tagHitTargetEvent : public tagGameEvent
{
	DWORD					dwSrcRoleID;	//������ID
	DWORD					dwTarRoleID;	//Ŀ��ID
	bool					bSingle;		//�Ƿ��幥��		
	bool					bLast;			//�Ƿ��Ǳ��ι��������һ���¼�
	EHitTargetCause			eCause;			//����Ŀ��ԭ��
	DWORD					dwMisc;			//����Ǽ��ܣ�Ϊ����ID;�������Ʒ����ƷID
	DWORD					dwMisc2;		//�ܵ����
	DWORD					dwSerial;		//���ܻ�ʹ����Ʒ���к�

	tagHitTargetEvent():tagGameEvent(_T("tagHitTargetEvent"),NULL)
	{}
	tagHitTargetEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ��������Ϸ�¼�
*/
struct tagBeAttackEvent : public tagGameEvent
{
	bool					bMiss;			//�Ƿ�MISS
	bool					bBlock;			//�Ƿ��
	tagBeAttackEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{
		bMiss=false;
		bBlock=false;
	}
};

/** ��ɫ�������״̬֪ͨ��Ϸ�¼�
*/
struct tagEnterStandStateEvent : public tagGameEvent
{
	tagEnterStandStateEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ��ɫ����ս��״̬��Ϸ�¼�
*/
struct tagEnterCombatEvent : public tagGameEvent
{
	tagEnterCombatEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ��ɫ�뿪ս��״̬��Ϸ�¼�
*/
struct tagLeaveCombatEvent : public tagGameEvent
{
	tagLeaveCombatEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** �����ɫ��Ϸ�¼�
*/
struct tagFollowRoleEvent : public tagGameEvent
{
	DWORD					dwTargetRoleID;	//������Ľ�ɫID

	tagFollowRoleEvent():tagGameEvent(_T("tagFollowRoleEvent"),NULL)
	{}
	tagFollowRoleEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ����PK������Ϸ�¼�
*/
struct tagOpenSafeguardEvent : public tagGameEvent
{
	bool					bOpen;			
	tagOpenSafeguardEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ��������״̬��Ϸ�¼�
*/
struct tagOpenPKEvent : public tagGameEvent
{
	bool bPK;
	tagOpenPKEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ��ʾ�����˵���Ϸ�¼�
*/
struct tagShowDeadMenuEvent : public tagGameEvent
{
	tagShowDeadMenuEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};


struct tagPersonAct;
/** ���Ÿ��Զ����¼�
*/
struct tagPlayPersonalityActEvent : public tagGameEvent
{
	DWORD dwTargetID;
	bool  bDestRole;   // �Ƿ������ɫ������������
	const tagPersonAct* pData;
	tagPlayPersonalityActEvent(LPCTSTR szEventName, GameFrame* pSenderFrame,const tagPersonAct* pAct) 
		: tagGameEvent(szEventName,pSenderFrame),dwTargetID(GT_INVALID),pData(pAct),bDestRole(false){}
};

/** �������Զ����˳��¼�
*/
struct tagPersonalityActExitEvent : public tagGameEvent
{
	DWORD dwRoleID;   // ���Ÿ��Զ����Ľ�ɫID
	DWORD dwTargetID; // �������Զ�����Ŀ���ɫID���ǽ�����ΪGT_INVALID��
	DWORD dwActID;    // ���Զ���ID
	tagPersonalityActExitEvent() : tagGameEvent(_T("tagPersonalityActExitEvent"),NULL),dwRoleID(GT_INVALID),dwTargetID(GT_INVALID),dwActID(GT_INVALID){}
	tagPersonalityActExitEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame),dwRoleID(GT_INVALID),dwTargetID(GT_INVALID),dwActID(GT_INVALID){}
};

/** ʹ����Ʒ��Ϸ�¼�
*/
struct tagUseItemEvent : public tagGameEvent
{
	DWORD					dwTargetRoleID;
	INT64					n64ItemID;
	DWORD					dwItemTypeID;
	bool					bSpell;		//�Ƿ�����֣�ս��ϵͳ�ڲ�ʹ��

	tagUseItemEvent():tagGameEvent(_T("tagUseItemEvent"),NULL),bSpell(false)
	{}
	tagUseItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};


/** ʰȡ��Ʒ��Ϸ�¼�
*/
struct tagPickupItemEvent : public tagGameEvent
{
	INT64					n64ItemID;

	tagPickupItemEvent():tagGameEvent(_T("tagPickupItemEvent"),NULL)
	{}
	tagPickupItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ������Ľ��������Ϣ��Ϸ�¼�
	\remark �Ӿ��顢����
*/
struct tagProcessRewardMsgEvent : public tagGameEvent
{
	DWORD					dwRoleID;	//��������
	tagProcessRewardMsgEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};


/** ������ĳͷ������Ϣ��Ϸ�¼�
	\remark ����
*/
struct tagProcessCostMsgEvent : public tagGameEvent
{
	DWORD					dwRoleID;	//���ͷ���
	tagProcessCostMsgEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** NPC�������ж���ʱ˵����Ϸ�¼�
*/
struct tagNPCEaseSpeechEvent : public tagGameEvent
{
	DWORD					dwNPCID;
	DWORD					dwTypeID;
	tagNPCEaseSpeechEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) 
		: tagGameEvent(szEventName,pSenderFrame),dwNPCID(GT_INVALID),dwTypeID(GT_INVALID)
	{}
};

/** ׷���¼�
*/
struct tagPursueEvent : public tagGameEvent
{
	enum ETargetType
	{
		ETargetType_Role,
		ETargetType_Pos,
	};

	ETargetType				targetType;
	Vector3					start;				//���
	DWORD					targetRoleID;		//Ŀ���ɫID
	Vector3					targetPos;			//Ŀ��λ��
	float					validDist;			//��Ч����
	bool					bNeedSendStopCmd;	//�Ƿ���Ҫ����ֹͣ��Ϣ

	tagPursueEvent():tagGameEvent(_T("tagPursueEvent"),NULL)
	{}
	tagPursueEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ֹͣ׷���¼�
*/
struct tagStopPursueEvent : public tagGameEvent
{
	tagStopPursueEvent():tagGameEvent(_T("tagStopPursueEvent"),NULL)
	{}
	tagStopPursueEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ׷������¼�
*/
struct tagPursueFinishEvent : public tagGameEvent
{
	bool					bSuccessed;			//�Ƿ�ɹ�

	tagPursueFinishEvent():tagGameEvent(_T("tagPursueFinishEvent"),NULL)
	{}
	tagPursueFinishEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ��ɫ��ʼ�����¼�
*/
struct tagRoleSpellStartEvent : public tagGameEvent
{
	DWORD		dwRoleID;
	bool		bSkill;          // �Ƿ���ʹ�ü�������������Ϊʹ����Ʒ��
	DWORD		dwTypeID;        // ���ܻ���ƷID
	DWORD		dwCurTime;       // ��ǰ�Ѿ�������ʱ��
	DWORD		dwTimeCount;     // ��ʱ��

	tagRoleSpellStartEvent():tagGameEvent(_T("tagRoleSpellStartEvent"),NULL),dwRoleID(GT_INVALID),bSkill(false),dwTypeID(GT_INVALID),dwCurTime(0),dwTimeCount(0)
	{}
	tagRoleSpellStartEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ��ɫ���������¼�
*/
struct tagRoleSpellStopEvent : public tagGameEvent
{
	DWORD		dwRoleID;
	bool		bInterrupt;      // �Ƿ񱻴��

	tagRoleSpellStopEvent():tagGameEvent(_T("tagRoleSpellStopEvent"),NULL),dwRoleID(GT_INVALID),bInterrupt(false)
	{}
	tagRoleSpellStopEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

/** ѡ��Ŀ���¼�(ֻ������ѡ�еĽ�ɫ)
*/
struct tagTargetSelectEvent : public tagGameEvent
{
	tagTargetSelectEvent():tagGameEvent(_T("tagTargetSelectEvent"),NULL)
	{}
	tagTargetSelectEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};


/** ESCȡ����ͨ����������
*/
struct tagESCCancelEvent : public tagGameEvent
{
	BOOL		bCanceled;

	tagESCCancelEvent():tagGameEvent(_T("tagESCCancelEvent"),NULL),bCanceled(FALSE)
	{}
	tagESCCancelEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};