//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_title.h
// author: 
// actor:
// date: 2009-01-20
// last:
// brief: �ƺ����
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push)
#pragma pack(1)
#include "title_define.h"
//-----------------------------------------------------------------------------
// NDBC:world->DB
// NDBS:DB->world
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// ���ý�ɫʹ��ĳ�ƺţ�u16TitleID��Чʱ����ʾ��ʹ�óƺţ�
//-----------------------------------------------------------------------------
// CMD_START(NDBC_ActiveRoleTitle)
// 	DWORD			dwRoleID;		// ��ɫID
// 	UINT16			u16TitleID;		// ��ǰʹ�õĳƺ�ID
// CMD_END

//-----------------------------------------------------------------------------
// ��ɫ�ƺ�����(����֪ͨ���ݿ�)
//-----------------------------------------------------------------------------
CMD_START(NDBC_TitleInsert)
	DWORD			dwRoleID;		// ��ɫID
	tagTitleSave	titleSave;		// �ƺ���Ϣ
CMD_END

CMD_START(NDBC_TitleDelete)
	DWORD			dwRoleID;		// ��ɫID
	tagTitleSave	titleSave;		// �ƺ���Ϣ
CMD_END

#pragma pack(pop)
