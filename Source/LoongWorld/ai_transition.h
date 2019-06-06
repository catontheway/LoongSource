//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: ai_transition.h
// author: wjiang
// actor:
// data: 2009-5-31
// last:
// brief: AI״̬ת����
//-------------------------------------------------------------------------------------------------------
#pragma once
enum AIStateType;
class Creature;
class AIController;

//-----------------------------------------------------------------------------
// ״̬ת�����ӿ�
//-----------------------------------------------------------------------------
class AITransition
{
public:
	AITransition() {}
	virtual ~AITransition() {}

	virtual AIStateType Transition(AIController* pAI, AIStateType eCurState) = 0;
};

//-----------------------------------------------------------------------------
// ������״̬ת����
//-----------------------------------------------------------------------------
class AggressiveTransition : public AITransition
{
public:
	AggressiveTransition() {}
	~AggressiveTransition() {}

public:
	AIStateType	Transition(AIController* pAI, AIStateType eCurState);
	static AITransition* Instance();

private:
	AIStateType IdleTransition(AIController* pAI);
	AIStateType PursueTransition(AIController* pAI);
	AIStateType AttackTransition(AIController* pAI);
	AIStateType ReturnTransition(AIController* pAI);
};

//-----------------------------------------------------------------------------
// ������״̬ת����
//-----------------------------------------------------------------------------
class GuardTransition : public AITransition
{
public:
	GuardTransition() {}
	~GuardTransition() {}

public:
	AIStateType Transition(AIController* pAI, AIStateType eCurState);
	static AITransition* Instance();

private:
	AIStateType IdleTransition(AIController* pAI);
	AIStateType AttackTransition(AIController* pAI);
	AIStateType ReturnTransition(AIController* pAI);
};

//-----------------------------------------------------------------------------
// ������״̬ת����
//-----------------------------------------------------------------------------
class BarbetteTransition : public AITransition
{
public:
	BarbetteTransition() {}
	~BarbetteTransition() {}

public:
	AIStateType Transition(AIController* pAI, AIStateType eCurState);
	static AITransition* Instance();

private:
	AIStateType IdleTransition(AIController* pAI);
	AIStateType AttackTransition(AIController* pAI);
};

//-----------------------------------------------------------------------------
// ������״̬ת����
//-----------------------------------------------------------------------------
class LamsterTransition : public AITransition
{
public:
	LamsterTransition() {}
	~LamsterTransition() {}

public:
	AIStateType Transition(AIController* pAI, AIStateType eCurState);
	static AITransition* Instance();

private:
	AIStateType IdleTransition(AIController* pAI);
	AIStateType PursueTransition(AIController* pAI);
	AIStateType AttackTransition(AIController* pAI);
	AIStateType ReturnTransition(AIController* pAI);
	AIStateType	FleeTransition(AIController* pAI);
};

//-----------------------------------------------------------------------------
// ������״̬ת����
//-----------------------------------------------------------------------------
class CallHelpTransition : public AITransition
{
public:
	CallHelpTransition() {}
	~CallHelpTransition() {}

public:
	AIStateType Transition(AIController* pAI, AIStateType eCurState);
	static AITransition* Instance();

private:
	AIStateType IdleTransition(AIController* pAI);
	AIStateType PursueTransition(AIController* pAI);
	AIStateType AttackTransition(AIController* pAI);
	AIStateType ReturnTransition(AIController* pAI);
	AIStateType CallForHelpTransition(AIController* pAI);
};

//-----------------------------------------------------------------------------
// ������״̬ת����
//-----------------------------------------------------------------------------
class SpaceOutTransition : public AITransition
{
public:
	SpaceOutTransition() {}
	~SpaceOutTransition() {}

public:
	AIStateType Transition(AIController* pAI, AIStateType eCurState);
	static AITransition* Instance();

private:
	AIStateType IdleTransition(AIController* pAI);
	AIStateType PursueTransition(AIController* pAI);
	AIStateType AttackTransition(AIController* pAI);
	AIStateType ReturnTransition(AIController* pAI);
	AIStateType SpaceTransition(AIController* pAI);
};