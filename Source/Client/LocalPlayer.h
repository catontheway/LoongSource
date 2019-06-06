#pragma once
#include "player.h"

class FSM_LP;
struct tagNS_GetRoleInitState_Att;

/** \class LocalPlayer
	\brief ������ҽ�ɫ
*/
class LocalPlayer : public Player
{
public:
	LocalPlayer(void);
	virtual ~LocalPlayer(void);

	//--Role
	virtual void InitFSM();
	virtual void Update();
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pGameEvent);
	virtual bool IsDead();

	//--Player
	virtual bool IsReadyState();
	virtual bool IsStandState();
	virtual bool IsStyleActionState();
	virtual bool IsIdleState();
	virtual EActionState GetActionState();
	virtual void ReplayLowerAni();

	//--��ɫ�������
	/** \��ȡ��д�����Ͷ��
	*/
	INT GetAttPointAdd(ERoleAttribute eType){return m_rolepointadd[MTransERAAttA2Index(eType)];}
	VOID SetAttPointAdd(ERoleAttribute eType, INT nValue);
	/** \��ȡ���ԭʼһ������
	*/
	INT GetOriginAtt(ERoleAttribute eType);
	/** \������������Գ�ʼ��
	*/
	VOID InitAttribute(tagNS_GetRoleInitState_Att* pCmd);
	/** \��ȡ��д����ҵ�ǰ����
	*/
	INT GetRoleLevelExp(){return m_rolecurlevelexp;}
	VOID SetRoleLevelExp(INT nValue, INT nInc = 0);

	//--
	/** \���ؽ�ɫ�Ƿ���ս��״̬
	*/
	bool IsCombatState();
	/** \���ؽ�ɫ�Ƿ���ʹ�ü���״̬
	*/
	bool IsSkillState();
	/** \���ؽ�ɫ�Ƿ�����Ӿ״̬
	*/
	bool IsSwimState();
	/** \���ؽ�ɫ�Ƿ���ˮ������״̬
	*/
	bool IsOnWaterState();
	/** \���ý�ɫ����״̬
	*/
	void SetRoleStateEx(DWORD dwValue);
	/** \���ý�ɫ����״̬
	*/
	void SetRoleStateEx(ERoleStateEx eType, bool bFlag);
	/** \��ȡ��ɫ����״̬
	*/
	bool GetRoleStateEx(ERoleStateEx eType){return (m_roleStateEx&eType) != 0;}
	/** \�л���ͼ���ͺ��������ꡢ�����
	*/
	void OnGotoMap(const Vector3& pos,const Vector3 faceTo);

	/** \��ҵĸ�������
	*/
	const tagDWPlayerPersonalSet& GetPersonalSet() const { return m_personalSet; }
	void SetPersonalSet( const tagDWPlayerPersonalSet& pps ) { m_personalSet = pps; }

	/** \����,����һ�����ж���
	*/
	void DontPlayEaseSkeleton();

	/** \����,ǿ��ת����idle״̬, Ϊ����ϵͳ���
	*/
	void ChangeUpperToIdle();

	/** \��ǰ״̬�Ƿ�Ϊidle
	*/
	bool IsUpperIdleState();

	/** 
		\ �õ�����ʹ�õ���Ʒ�� typeid�������ǰ����ʹ����Ʒ״̬����0
	*/
	DWORD GetUsingItemTypeID();

protected:
	FSM_LP*					m_pFSM;
	INT						m_rolepointadd[X_ERA_ATTA_NUM];
	INT						m_rolecurlevelexp;
	DWORD					m_roleStateEx;

	tagDWPlayerPersonalSet  m_personalSet;

	DECL_RTTI(LocalPlayer);
};

