#pragma once
#include "CombatEvent.h"

class NPC;
class Role;
/** \class ActionSkillAttack_NPC
	\brief NPC���ܹ�����Ϊ
*/
class ActionSkillAttack_NPC
{
public:
	ActionSkillAttack_NPC(void);
	virtual ~ActionSkillAttack_NPC(void);

	void Active(NPC* pRole,Role* pTarget,DWORD skillID,DWORD dwSerial);
	void Update(NPC* pRole,Role* pTarget);
	void Stop(NPC* pRole);
	bool IsClosed(){ return m_bClosed;}
	const TCHAR* GetTrackName(){ return m_trackName.c_str();}

private:
	void StopEffect(bool bAtkEffect);
	void OnMsgCode(NPC* pRole,const DWORD dwMsgCode);
	void PlayAtkSfx(DWORD dwRoleID);
	void StopAtkSfx();
private:
	DWORD					m_skillID;
	DWORD					m_targetID;
	DWORD					m_dwSerial;
	bool					m_bSingle;//�Ƿ��幥��
	int						m_dmgTimes;
	bool					m_bClosed;
	tstring					m_trackName;
	TObjRef<GameFrameMgr>   m_pGameFrameMgr;

	tstring                 m_strAtkSfx;				// ������Ч���
	vector<tstring>			m_vecAtkSfxTagNode;			// ������Ч���ص���������
	tstring					m_strLchSfx;				// ������Ч���
	tstring					m_strLchSfxTagNode;			// ������Ч���ص���������
	vector<DWORD>           m_vecAttackEffectID;		// ��ǰ���ŵĹ�����Ч��ID
	tstring					m_strAtkSound;				// ������Ч���
};
