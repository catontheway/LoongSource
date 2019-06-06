//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_show_sum.h
// author: 
// actor:
// data: 2009-12-09
// last:
// brief: GM�������G������ʾ����֮�����Ϣ����
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/base_define.h"

#pragma pack(push, 1)

struct tagWorldNode
{
	DWORD			dwWorldID;
	INT				nOnline;
	TCHAR			szWorldName[X_SHORT_STRING];
	INT				nRealOnline;
};

struct tagSectionNode
{
	DWORD			dwSectionID;
	TCHAR			szSectionName[X_SHORT_STRING];	// ��������
	INT				nNum;							// ��������Ϸ�������
	tagWorldNode	World[1];						// ��������Ϸ������Ϣ
};


CMD_START(NSSC_Challenge)
CMD_END

CMD_START(NSSS_Challenge)
CMD_END

//-----------------------------------------------------------------------
// ��������GMServer�Ķ�̬��Ϣ
//-----------------------------------------------------------------------
CMD_START(NSSC_GetDynInfo)
CMD_END

CMD_START(NSSS_GetDynInfo)
	INT						nSectionCount;
	tagSectionNode			SectionInfo[1];	
CMD_END

#pragma pack(pop)