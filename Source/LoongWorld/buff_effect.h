//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: buff_effect.h
// author: Aslan
// actor:
// data: 2008-11-26
// last:
// brief: ״̬����Ч��
//-------------------------------------------------------------------------------
#pragma once


class Unit;
class BuffEffect;

typedef VOID (*BUFFEFFECTRUTINE)(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet, const tagBuffProto* pProto);

class BuffEffect
{
public:
	static VOID Init();

	//------------------------------------------------------------------------------
	// ע��Buff����Ч��������
	//------------------------------------------------------------------------------
	static VOID RegisterBuffEffectRutine();

	//-------------------------------------------------------------------------------
	// ����BuffЧ��
	//-------------------------------------------------------------------------------
	static VOID CalBuffEffect(Unit* pTarget, Unit* pSrc, EBuffEffectType eEffect, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);

private:
	//-----------------------------------------------------------------------------------------------------------------
	// ��
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectNull(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ˲��
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectTeleport(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// �������
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectInterruptSpell(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ѣ��
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectDizzy(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectRepel(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ���
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectAssault(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// �⼼
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectNoSkill(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ��˯
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectSpor(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectTie(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ��ɢ
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectDispel(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ȥ��
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectCancel(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);

	//-----------------------------------------------------------------------------------------------------------------
	// �޵�
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectInvincible(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectInstantDie(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ��ק
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectPull(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ת��
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectHPTransfer(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ת��
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectMPTransfer(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectRevive(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ˲����ȴ
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectInstantCD(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectLurk(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectFly(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// �ı���
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectChangeEnmity(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectTransmit(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// �ɼ�
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectGather(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ��е
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectDisArm(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ����λ��
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectExchangePos(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectExplode(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// �־�
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectFunk(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ׷��
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectPursue(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ������ʱ��
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectNoPrepare(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// ˮ������
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectOnWater(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	//-----------------------------------------------------------------------------------------------------------------
	// �ƶ���Ѫ
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectMoveHPDmg(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);

	//-----------------------------------------------------------------------------------------------------------------
	// ���ӻ���
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectIgnoreArmor(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);

	//-----------------------------------------------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectSneer(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);

	//-----------------------------------------------------------------------------------------------------------------
	// ���
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectMount(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);

	//-----------------------------------------------------------------------------------------------------------------
	// �������
	//-----------------------------------------------------------------------------------------------------------------
	static VOID BuffEffectMountInvite(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);


private:
	static VOID GatherEffect(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	static VOID InvesEffect(Unit* pTarget, Unit* pSrc, DWORD dwEffectMisc1, DWORD dwEffectMisc2, BOOL bSet=TRUE, const tagBuffProto* pProto=NULL);
	
	
	//  ��������
protected:
	// �ͷ���buffer�����Ƿ�����������
	static BOOL EndbuffContinueIsAttack(Unit* pTarget, Unit* pSrc, BOOL bSet); 

private:
	static BUFFEFFECTRUTINE	m_Effect[EBET_End];
};