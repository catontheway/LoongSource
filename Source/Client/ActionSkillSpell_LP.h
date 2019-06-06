#pragma once

class LocalPlayer;
class Role;
/** \class ActionSkillSpell_LP
	\brief ������Ҽ���������Ϊ
*/
class ActionSkillSpell_LP
{
	enum EStep
	{
		EStep_Action1,	//��һ������
		EStep_Action2,	//�ڶ�������
	};
public:
	ActionSkillSpell_LP(void);
	virtual ~ActionSkillSpell_LP(void);

	void Active(LocalPlayer* pRole,Role* pTarget,DWORD skillID,DWORD dwSerial);
	void Update(LocalPlayer* pRole,Role* pTarget);

	bool IsClosed(){ return m_bClosed;}
	void OnRecvSpellTime(DWORD spellTime);
	void Stop(LocalPlayer* pRole,bool bSendStopCmd);

private:
	void PlayEffect(DWORD dwRoleID);
	void StopEffect();
	void FacetoTarget(LocalPlayer* pSelf,Role* pTarget);

private:
	bool					m_bClosed;
	DWORD					m_skillID;
	DWORD					m_dwSerial;
	DWORD					m_targetID;
	DWORD					m_startSpellTime;
	DWORD					m_spellTime;
	tstring					m_szTrackName1;
	tstring					m_szTrackName2;
	bool					m_bRecvSpellTime;
	bool					m_bMoveSpell;			// �Ƿ�����ƶ�����

	EStep					m_step;

	tstring                 m_strPreSfx;			// ������Ч
	vector<tstring>         m_vecPreSfxTagNode;		// ������Ч���ص���������
	vector<DWORD>			m_vecPreSfxID;          // ������ЧID

	DWORD					m_dwChopSoundID;		// �ɼ���ЧID
};
