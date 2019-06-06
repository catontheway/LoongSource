#pragma once
#include "upperstate_rp.h"
#include "ActionUseItemAttack_RP.h"
#include "ActionUseItemSpell_RP.h"

/** \class UpperStateUseItem_RP
	\brief Զ�����ʹ����Ʒ״̬
*/
class UpperStateUseItem_RP :
	public UpperState_RP
{
	enum EStep
	{
		EStep_Spell,
		EStep_Attack,
	};
public:
	UpperStateUseItem_RP(void);
	virtual ~UpperStateUseItem_RP(void);

	//--UpperState_RP
	virtual void Active(tagNetCmd* pNetCmd,NavMap* pNav,Player* pRole);	
	virtual void OnExit();
	virtual void Update(NavMap* pNav,Player* pRole);							
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual bool IfCanPlayLowerAni();
	virtual bool IfLowerCanSetYaw();

private:
	ActionUseItemSpell_RP		m_actSpell;
	ActionUseItemAttack_RP		m_actAttack;
	EStep						m_step;
	DWORD						m_targetID;
	DWORD						m_itemTypeID;
	INT64						m_itemID;
	DWORD						m_serialID;
	bool						m_bMoveable;	//�Ƿ�����ƶ�����
	bool						m_bAniBlend;	//�Ƿ�������ֿ�
};	
