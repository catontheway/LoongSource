#pragma once
#include "..\WorldDefine\base_define.h"
#include "..\WorldDefine\move_define.h"
#include "CombatEvent.h"

class FSM_LP;
class LocalPlayer;
/** ǰ��״̬
*/
struct tagPreState
{
	enum EState					//״̬
	{
		EState_None=-1,			//��
		EState_MouseMove,		//����ƶ�/��Ӿ
		EState_KeyboardMove,	//�����ƶ�/��Ӿ
		EState_Pursue,			//׷��
	};

	EState					state;
	tagMouseMoveEvent		mouseMove;
	tagKeyboardMoveEvent	keyboardMove;
	tagPursueEvent			pursue;

	/** ���캯��
	*/
	tagPreState();
	/** �л���ǰ��״̬
	*/
	void Change2LowerState(FSM_LP* pFSM,LocalPlayer* pRole,bool bSwim,bool bOnWater);
	/** ǰ��״̬�Ƿ������ƶ���
	*/
	bool IsMoving();
};