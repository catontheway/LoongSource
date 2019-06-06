#pragma once

class LocalPlayer;
class Role;
/** \class ActionUseItemSpell_LP
	\brief �������ʹ����Ʒ������Ϊ
*/
class ActionUseItemSpell_LP
{
public:
	ActionUseItemSpell_LP(void);
	virtual ~ActionUseItemSpell_LP(void);

	void Active(LocalPlayer* pRole,Role* pTarget,INT64 itemID,DWORD itemTypeID,DWORD dwSerial);
	void Update(LocalPlayer* pRole,Role* pTarget);

	bool IsClosed(){ return m_bClosed;}
	void OnRecvSpellTime(DWORD spellTime);
	void Stop(LocalPlayer* pRole,bool bSendStopCmd);

private:
	bool					m_bClosed;
	INT64					m_n64ItemID;
	DWORD					m_itemID;
	DWORD					m_dwSerial;
	DWORD					m_targetID;
	DWORD					m_startSpellTime;
	DWORD					m_spellTime;
	tstring					m_szTrackName;
	bool					m_bRecvSpellTime;

	DWORD					m_dwEffectID;			//ʹ����Ʒ������Ч��Ŀǰֻ�е���ʹ��
};
