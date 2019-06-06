#pragma once
#include "lowerstate_lp.h"

/**	\class LowerStateStand_LP
	\brief ������ҿ���״̬
*/
class LowerStateStand_LP : public LowerState_LP
{
	enum EStep
	{
		EStep_Ready,//��ս
		EStep_Stand,//վ��
		EStep_Ease,//����
		EStep_Buff,//����Buff����
	};

public:
	LowerStateStand_LP(void);
	virtual ~LowerStateStand_LP(void);

	//--State_LP
	virtual void Active(tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole,tagPreState* pPreState=NULL);	
	virtual void Update(NavMap* pNav,LocalPlayer* pRole);							
	virtual void OnGameEvent(tagGameEvent* pEvent);	
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnExit();
	virtual void ReplayAni(LocalPlayer* pRole);
	virtual bool IfCanChange2UpperState(FSM_LP::EUpperState state,tagGameEvent* pEvent);
	virtual bool IfCanChange2LowerState(FSM_LP::ELowerState state,tagGameEvent* pEvent);

	//����ʱ����Ҫ���ж���,����ÿ�ε���֮ǰ�������ʱ�丳ֵΪ��ǰʱ��
	//���������ʱ�����ǲ��ܲ������ж���,ע��˷�������һ��ֻ�ܱ�֤����һ�����ж���
	//�����һֱ���������ж�����Ҫ���ϵĵ��ô˷������������Ҫ�ﵽһֱ���������ж�����
	//Ч����Ҫʹ�ô˷���
	void ResetLastEaseTime();

private:
	void PlayReadyAni(LocalPlayer* pRole,bool bCheckIfCanPlay=true);
	void PlayStandAni(LocalPlayer* pRole,bool bCheckIfCanPlay=true);
	void PlayEaseAni(LocalPlayer* pRole,bool bCheckIfCanPlay=true);
	bool PlayBuffAni(LocalPlayer* pRole,bool bCheckIfCanPlay=true);

private:
	DWORD						m_sprayEffectID;//ˮ����ЧID
	DWORD						m_lastPlayEaseTime;//�ϴβ������ж���ʱ��
	EStep						m_step;
};
