#pragma once
#include "CombatEvent.h"

class LocalPlayer;
class Role;
/** \class ActionSkillAttack_LP
	\brief ������Ҽ��ܹ�����Ϊ
*/
class ActionSkillAttack_LP
{
public:
	ActionSkillAttack_LP(void);
	virtual ~ActionSkillAttack_LP(void);

	void Active(LocalPlayer* pRole,Role* pTarget,DWORD skillID,DWORD dwSerial,bool bSendNetMsg);
	void Update(LocalPlayer* pRole,Role* pTarget);
	bool IsClosed(){ return m_bClosed;}
	const TCHAR* GetTrackName(){ return m_trackName.c_str();}

	/** ֹͣ
	*/
	void Stop(LocalPlayer* pRole,bool bSendStopCmd);

private:
	void StopEffect(bool bAtkEffect);
	void FaceToTarget(LocalPlayer* pSelf,Role* pTarget);
	void RestoreYawAndLowerAni(LocalPlayer* pSelf);
	void OnMsgCode(LocalPlayer* pRole,const DWORD dwMsgCode);
	void PlayAtkSfx(DWORD dwRoleID);
	void StopAtkSfx();
private:
	tstring					m_trackName;
	DWORD					m_skillID;
	DWORD					m_targetID;
	bool					m_bSingle;//�Ƿ��幥��
	int						m_dmgTimes;//�˺�����
	DWORD					m_dwSerial;//��Ϣ���к�
	DWORD					m_attackEffectID;
	bool					m_bClosed;

	bool					m_bMoveable;
	bool					m_bAniBlend;
	bool                    m_bHostile;

	tstring                 m_strAtkSfx;				// ������Ч���
	vector<tstring>			m_vecAtkSfxTagNode;			// ������Ч���ص���������
	tstring					m_strLchSfx;				// ������Ч���
	tstring					m_strLchSfxTagNode;			// ������Ч���ص���������
	vector<DWORD>           m_vecAttackEffectID;		// ��ǰ���ŵĹ�����Ч��ID
	tstring					m_strAtkSound;				// ������Ч���
	DWORD                   m_dwMoveEffectID[2];		// �ƶ�������Ч��ID��2����
	Vector3                 m_vPlayerPos;               // ����һ����ɫ�����꣬�����жϽ�ɫ�Ƿ���λ��
};
