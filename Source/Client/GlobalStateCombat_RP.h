#pragma once
#include "globalstate_rp.h"

/** \class GlobalStateCombat_RP
	\brief Զ�����ս��״̬
*/
class GlobalStateCombat_RP :
	public GlobalState_RP
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
	GlobalStateCombat_RP(void);
	virtual ~GlobalStateCombat_RP(void);

	//--GlobalState_RP
	virtual void Active(tagNetCmd* pNetCmd,NavMap* pNav,Player* pRole,DWORD dwParam=0);		
	virtual void Update(NavMap* pNav,Player* pRole);							
	virtual void OnGameEvent(tagGameEvent* pEvent);	
	virtual void OnNetCmd(tagNetCmd* pNetCmd);

private:
	EStep m_step;
	TObjRef<Util> m_pUtil;
	tstring	m_szEnterTrackName;
	tstring	m_szLeaveTrackName;
};
