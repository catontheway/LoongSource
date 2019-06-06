#pragma once
#include "lowerstate_rp.h"

/**	\class LowerStateStand_RP
	\brief Զ�����վ��״̬
*/
class LowerStateStand_RP : public LowerState_RP
{
	enum EStep
	{
		EStep_Ready,//��ս
		EStep_Stand,//վ��
		EStep_Ease,//����
		EStep_Buff,//����Buff����
	};
public:
	LowerStateStand_RP(void);
	virtual ~LowerStateStand_RP(void);

	//--LowerState_RP
	virtual void Active(tagNetCmd* pNetCmd,NavMap* pNav,Player* pRole);		
	virtual void Update(NavMap* pNav,Player* pRole);							
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void OnExit();
	virtual void ReplayAni(Player* pRole);
	virtual bool IfCanChange2UpperState(FSM_RP::EUpperState state);

private:
	void PlayReadyAni(Player* pRole,bool bCheckIfCanPlay=true);
	void PlayStandAni(Player* pRole,bool bCheckIfCanPlay=true);
	void PlayEaseAni(Player* pRole,bool bCheckIfCanPlay=true);
	bool PlayBuffAni(Player* pRole,bool bCheckIfCanPlay=true);

private:
	DWORD m_sprayEffectID;//ˮ����ЧID
	DWORD m_lastPlayEaseTime;//�ϴβ������ж���ʱ��
	EStep m_step;
};
