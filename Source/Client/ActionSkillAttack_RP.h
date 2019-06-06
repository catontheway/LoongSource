#pragma once
#include "CombatEvent.h"

class Player;
class Role;
/** \class ActionSkillAttack_RP
	\brief Զ����Ҽ��ܹ�����Ϊ
*/
class ActionSkillAttack_RP
{
public:
	ActionSkillAttack_RP(void);
	virtual ~ActionSkillAttack_RP(void);

	void Active(Player* pRole,Role* pTarget,DWORD skillID,DWORD dwSerial);
	void Update(Player* pRole,Role* pTarget);
	bool IsClosed(){ return m_bClosed;}
	const TCHAR* GetTrackName(){ return m_trackName.c_str();}

	void Stop(Player* pRole);

private:
	void StopEffect(bool bAtkEffect);
	void FaceToTarget(Player* pSelf,Role* pTarget);
	void RestoreYawAndLowerAni(Player* pSelf);
	void OnMsgCode(Player* pRole,const DWORD dwMsgCode);
	void PlayAtkSfx(DWORD dwRoleID);
	void StopAtkSfx();
private:
	DWORD					m_skillID;
	DWORD					m_dwSerial;
	DWORD					m_targetID;
	bool					m_bClosed;
	tstring					m_trackName;
	bool					m_bSingle;
	int						m_dmgTimes;
	bool					m_bMoveable;
	bool					m_bAniBlend;
	bool                    m_bHostile;

	tstring                 m_strAtkSfx;          // ������Ч���
	vector<tstring>			m_vecAtkSfxTagNode;	  // ������Ч���ص���������
	tstring					m_strLchSfx;          // ������Ч���
	tstring					m_strLchSfxTagNode;   // ������Ч���ص���������
	vector<DWORD>           m_vecAttackEffectID;  // ��ǰ���ŵĹ�����Ч��ID
	tstring					m_strAtkSound;		  // ������Ч���
	DWORD                   m_dwMoveEffectID[2];  // �ƶ�������Ч��ID��2����
	Vector3                 m_vPlayerPos;         // ����һ����ɫ�����꣬�����жϽ�ɫ�Ƿ���λ��
};
