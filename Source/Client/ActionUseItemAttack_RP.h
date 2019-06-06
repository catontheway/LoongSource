#pragma once

class Player;
class Role;
/** \class ActionUseItemAttack_RP
	\brief Զ�����ʹ����Ʒ��Ϊ
*/
class ActionUseItemAttack_RP
{
public:
	ActionUseItemAttack_RP(void);
	virtual ~ActionUseItemAttack_RP(void);

	void Active(Player* pRole,Role* pTarget,DWORD itemID,DWORD dwSerial);
	void Update(Player* pRole,Role* pTarget);
	bool IsClosed(){ return m_bClosed;}
	const TCHAR* GetTrackName(){ return m_trackName.c_str();}
	void Stop(Player* pRole);

private:
	DWORD					m_itemID;
	DWORD					m_dwSerial;
	DWORD					m_targetID;
	bool					m_bClosed;
	bool					m_bSingle;//�Ƿ�Ե���ʹ��
	tstring					m_trackName;
	bool					m_bMoveable;//�Ƿ�����ƶ�ʹ��
	bool					m_bAniBlend;//�Ƿ�������ֿ�
};
