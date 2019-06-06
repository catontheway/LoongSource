//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_vcard.h
// author: 
// actor:
// date: 2009-02-04
// last:
// brief: ����������������
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push)
#pragma pack(1)
#include "..\WorldDefine\vcard_define.h"
//-----------------------------------------------------------------------------
// NDBC:world->DB
// NDBS:DB->world
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ���������Զ�����Ϣ
//-----------------------------------------------------------------------------
CMD_START(NDBC_VCardChg)
	DWORD				dwRoleID;			// ��ɫID
	tagVCardData		VCardData;	// �Զ�����Ϣ
CMD_END

//-----------------------------------------------------------------------------
// ��ȡ�����������
//-----------------------------------------------------------------------------
CMD_START(NDBC_GetOffLineVCard)
	DWORD				dwRoleID;			// ����ѯ��ID
	DWORD				dwQueryID;			// ��ѯ��ID
CMD_END

CMD_START(NDBS_GetOffLineVCard)
	DWORD				dwRoleID;			// ����ѯ��ID
	DWORD				dwQueryID;			// ��ѯ��ID
	DWORD				dwErrCode;
	BYTE				byData[1];	
CMD_END


#pragma pack(pop)
