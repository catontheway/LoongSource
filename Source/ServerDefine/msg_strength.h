//-----------------------------------------------------------------------------
// filename: msg_strength.h
// author: 
// actor:
// data: 2009-09-09
// last: 
// brief: ʵ��ϵͳ�����Ϣ��ʽ
//-----------------------------------------------------------------------------
#pragma once
#include "../WorldDefine/msg_strength.h"
#include "strength_define.h"

//-----------------------------------------------------------------------------
// ���ԣ�world ������db server �����ʼ��ʵ�����а�
//-----------------------------------------------------------------------------
CMD_START(NDBC_InitStrengthRankings)
CMD_END;

//-----------------------------------------------------------------------------
// ���ԣ�db server ������world ��ʵ�����а񷢸�world
//-----------------------------------------------------------------------------
CMD_START(NDBS_StrengthRankings)
	INT	nSum;														// �ܼ�¼������Ҳ�������а����������500
	tagStrengthRoleNode		Rankings[X_STRENGTH_RANKINGS_LEN];		// �����η��͸���ɫ����Ϣ
CMD_END;

CMD_START(NDBC_UpdateRankings)
	ERankingType	eType;
CMD_END;

CMD_START(NDBS_UpdateRankings)
	DWORD dwErrorCode;
CMD_END;

