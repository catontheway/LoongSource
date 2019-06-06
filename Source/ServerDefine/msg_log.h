//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_log.h
// author: 
// actor:
// data: 2009-03-23
// last:
// brief: log���
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

#include "log_cmdid_define.h"
//-----------------------------------------------------------------------------

CMD_START(NDBC_LogSilver)		// ��Ǯ�仯
	tagLogSilver	sLogSilver;
CMD_END

CMD_START(NDBC_LogYuanBao)		// Ԫ���仯
	tagLogYuanBao	sLogYuanBao;
CMD_END

CMD_START(NDBC_LogExVolume)		// ����仯
	tagLogExVolume	sLogExVolume;
CMD_END

CMD_START(NDBC_LogTimeStat)		// ��ʱͳ��
	tagLogTimeStat	sLogTimeStat;
CMD_END

CMD_START(NDBC_LogItem)			// ��Ʒlog
	tagLogItem		sLogItem;
CMD_END

CMD_START(NDBC_LogItemTimes)	// ��Ʒlog
	tagLogItemTims	sLogItemTimes;
CMD_END

CMD_START(NDBC_LogMallSell)		// �̳ǳ�����Ʒlog
	tagLogMallSell	sLogMallSell;
CMD_END

CMD_START(NDBC_LogMallSellPack)		// �̳ǳ�����Ʒ��log
	tagLogMallSellPack	sLogMallSellPack;
CMD_END

//-----------------------------------------------------------------------------
// ����LOG��¼
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogFund)
tagLogFund			sLogFund;
CMD_END

CMD_START(NDBC_LogMaterial)
tagLogMaterial		sLogMaterial;
CMD_END

CMD_START(NDBC_LogReputation)
tagLogReputation	sLogReputation;
CMD_END

//-----------------------------------------------------------------------------
// ����LOG��¼
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogGMCmd)
	tagLogGMCmd		sLogGMCmd;
CMD_END

//-----------------------------------------------------------------------------
// ����LOG��¼
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogPet)
	tagLogPet	sLogPet;
CMD_END

//-----------------------------------------------------------------------------
#pragma pack(pop)