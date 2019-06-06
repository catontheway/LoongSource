#pragma once

#include "..\WorldDefine\skill_define.h"
#include "..\WorldDefine\talent_define.h"

/** ������Ҽ��ܸ���
*/
struct tagUpdateLPSkillEvent : public tagGameEvent
{
	BOOL			bAdd;
	DWORD			dwID;
	ESkillUseType	eType;

	tagUpdateLPSkillEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{bAdd = FALSE; dwID = GT_INVALID;}
};

/** �������������Ͷ��������
*/
struct tagUpdateLPTalentPoint : public tagGameEvent
{
	ETalentType eType;
	INT			nPoint;
	INT			nMaxTalent;

	tagUpdateLPTalentPoint(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{eType = ETT_Null; nPoint = nMaxTalent = 0;}
};