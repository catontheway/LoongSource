#pragma once
#include "fatigue_define.h"
#pragma pack(push,1)

//-------------------------------------------------------------------------
// login�����ļ���������ķ�������Ϣ
//-------------------------------------------------------------------------
CMD_START(NC_LOGIN_CM)
	DWORD	dwSectionID;		// �������
CMD_END

CMD_START(NC_ANTIQUERYSTATE)
	CHAR	szAccountName[16];	// �ʺ���
CMD_END

CMD_START(NC_ANTIENTHRALL)
	DWORD	dwAccountNum;		// �ʺ���Ŀ
	CHAR	szAccountName[1];	// �ʺ���
CMD_END

CMD_START(NS_ANTIENTHRALL)
	CHAR	szAccountName[16];	// �ʺ���
	BYTE	byColor;			// ״̬
	DWORD	dwAccOLSeconds;		// �ۼ�����ʱ����
CMD_END

//-------------------------------------------------------------------------
// login ��world�ķ�������Ϣ
//-------------------------------------------------------------------------
CMD_START(NLW_FatigueNotify)
	INT				nNum;
	tagFatigueNotify	notify[1];
CMD_END

#pragma pack(pop)