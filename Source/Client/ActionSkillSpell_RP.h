#pragma once

class Player;
class Role;
struct tagTargetSelectEvent;

/** \class ActionSkillSpell_RP
	\brief Զ����Ҽ���������Ϊ
*/
class ActionSkillSpell_RP
{
	enum EStep
	{
		EStep_Action1,	//��һ������
		EStep_Action2,	//�ڶ�������
	};
public:
	ActionSkillSpell_RP(void);
	virtual ~ActionSkillSpell_RP(void);

	void Active(Player* pRole,Role* pTarget,DWORD skillID,DWORD spellTime);
	void Update(Player* pRole,Role* pTarget);
	bool IsClosed(){ return m_bClosed;}
	void Stop(Player* pRole);
private:
	void PlayEffect(DWORD dwRoleID);
	void StopEffect();
	void FacetoTarget(Player* pSelf,Role* pTarget);

private:
	DWORD					m_skillID;
	DWORD					m_spellTime;
	DWORD					m_startSpellTime;
	bool					m_bClosed;
	EStep					m_step;
	tstring					m_szTrackName1;
	tstring					m_szTrackName2;
	bool					m_bMoveSpell;			// �Ƿ�����ƶ�����

	tstring                 m_strPreSfx;			// ������Ч
	vector<tstring>         m_vecPreSfxTagNode;		// ������Ч���ص���������
	vector<DWORD>			m_vecPreSfxID;          // ������ЧID
};
