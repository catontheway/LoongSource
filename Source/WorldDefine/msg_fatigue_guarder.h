//-----------------------------------------------------------------------------
// filename: msg_fatigue_guarder.h
// author: client/server hyu/xlguo
// actor:
// data: 2009-02-05
// last: 2009-02-05
// brief: ������
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)

enum 
{
	E_FatigueLimit_None		= 0,
	E_FatigueLimit_Quest	= 1,
	E_FatigueLimit_Stall	= 2,
};

CMD_START(NS_FatigueLimit)
	BYTE	byLimitCode;		// ����������
CMD_END

//----------------------------------------------------------------------------
// ���Ŀǰ��������Ϣ
//----------------------------------------------------------------------------
CMD_START(NC_GetFatigueInfo)
CMD_END

CMD_START(NS_GetFatigueInfo)
	BYTE			byState;		// ��ǰ״̬
	DWORD			dwCurOLSec;		// ��ǰ�ۼ�����ʱ��
CMD_END

#pragma pack(pop)