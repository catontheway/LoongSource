#pragma once

class NPC;
class Role;
struct tagTargetSelectEvent;
/** \class ActionSkillSpell_NPC
	\brief NPC����������Ϊ
*/
class ActionSkillSpell_NPC
{
public:
	ActionSkillSpell_NPC(void);
	virtual ~ActionSkillSpell_NPC(void);

	void Active(NPC* pRole,Role* pTarget,DWORD skillID,DWORD spellTime);
	void Update(NPC* pRole,Role* pTarget);
	bool IsClosed(){ return m_bClosed;}
	void Stop(NPC* pRole);
	void OnTargetSelectEvent(tagTargetSelectEvent* pEvent,Role* pRole);
private:
	void StopEffect();
private:
	DWORD					m_skillID;
	DWORD					m_spellTime;
	DWORD					m_startSpellTime;
	bool					m_bClosed;

	vector<DWORD>			m_vecPreSfxID;          // ������ЧID
};
