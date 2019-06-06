//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: activity.h
// author: wjiang
// actor:
// data: 2009-03-27
// last:
// brief: ��Դ���������ݽṹ����
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// �����
//-----------------------------------------------------------------------------
enum EActMode
{
	EAM_Day,				// ÿ�ջ
	EAM_Week,				// ÿ�ܻ
	EAM_Month,				// ÿ�»
	EAM_Festival,			// ���ջ
};

//-----------------------------------------------------------------------------
// �㲥����
//-----------------------------------------------------------------------------
enum EActBroadMode
{
	EABM_AboutStart,		// ������ʼ
	EABM_Start,				// ��ʼ
	EABM_AboutEnd,			// ��������
	EABM_End,				// ����
};

//-----------------------------------------------------------------------------
// ���ڼ�
//-----------------------------------------------------------------------------
enum EWeekday
{
	EW_Sunday = 0,
	EW_Monday,
	EW_Tuesday,
	EW_Wednesday,
	EW_Thursday,
	EW_Friday,
	EW_Saturday,
};

//-----------------------------------------------------------------------------
// �̶�������ʱ��ṹ��ÿ�ܵ����ڼ��ļ��㵽���㣩
//-----------------------------------------------------------------------------
struct tagActTime
{
	EWeekday		eWeekday;					// ���ڼ�

	tagDWORDTime	startTime;					// ��ʼʱ��
	tagDWORDTime	endTime;					// ����ʱ��
	BYTE			byStartBroadH;				// �㲥��ʼ��ʱ�䣨Сʱ��
	BYTE			byStartBroadM;				// �㲥��ʼ��ʱ�䣨���ӣ�
	BYTE			byStartInterval;			// �㲥��ʼǰ��ʱ���������ӣ�
	BYTE			byEndBroadH;				// �㲥������ʱ�䣨Сʱ��
	BYTE			byEndBroadM;				// �㲥������ʱ�䣨���ӣ�
	BYTE			byEndInterval;				// �㲥����ǰ��ʱ���������ӣ�
};

struct tagActInfo
{
	DWORD			dwID;						// �ID
	TCHAR			szName[X_SHORT_NAME];		// �����
	EActMode		eActMode;					// �����
	tagActTime		ActTime;					// �̶�������ʱ��ṹ
};

struct tagActivityData
{
	DWORD							dwID;		// �ID
	tagScriptData<ESD_Activity>		Data;		// ��ű�����
};

CMD_START(NDBC_LoadActivityData)					// �����ű�����
CMD_END

CMD_START(NDBS_LoadActivityData)					// ��ȡ��ű�����
	INT								nNum;
	tagActivityData					Activity[1];
CMD_END

CMD_START(NDBC_SaveActivityData)
	tagActivityData					Activity;
CMD_END

#pragma pack(pop)




