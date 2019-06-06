#pragma once
#include "globalstate_lp.h"

class CombatSys;
/** \class GlobalStateCombat_LP
	\brief �������ս��״̬
*/
class GlobalStateCombat_LP :
	public GlobalState_LP
{
	enum EStep
	{
		EStep_HandUpWeapon,   //������
		EStep_OpenEquip,      //չ��װ�������Σ�
		EStep_Fighting,		  //ս��״̬
		EStep_CloseEquip,     //�ر�װ�������Σ�
		EStep_HandDownWeapon, //������
	};
public:
	GlobalStateCombat_LP(void);
	virtual ~GlobalStateCombat_LP(void);

	//--GlobalState_LP
	virtual void Active(tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole);		
	virtual void Update(NavMap* pNav,LocalPlayer* pRole);							
	virtual void OnGameEvent(tagGameEvent* pEvent);	
	virtual void OnNetCmd(tagNetCmd* pNetCmd);

private:
	bool					m_bStartTimer;
	DWORD					m_dwStartTime;
	EStep					m_step;
	tstring					m_szEnterTrackName;
	tstring					m_szLeaveTrackName;

	CombatSys*				m_pCombatSys;
};
