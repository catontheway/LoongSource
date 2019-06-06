//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_bill_c.h
// author: 
// actor:
// data: 2009-06-09
// last:
// brief: ֱ�����������վ�ӿ�
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/msg_common_errorcode.h"

#pragma pack(push, 1)

//------------------------------------------------------------------------------
// ��½
//------------------------------------------------------------------------------
CMD_START(NBC_Login)
	DWORD	dwSectionID;		// ����ID
CMD_END

CMD_START(NBS_Login)
	BYTE	byResult;			// 0-�ɹ� 1-ʧ��
CMD_END

//------------------------------------------------------------------------------
// Ԫ������
//------------------------------------------------------------------------------
CMD_START(NBS_YuanBao)
	CHAR	szTokenID[50];		// ����ID
	DWORD	dwWorldID;			// ��Ϸ����ID
	CHAR	szAccountName[50];	// �ʺ���
	INT		nNum;				// Ԫ������
CMD_END

CMD_START(NBC_Yuanbao)
	CHAR	szTokenID[50];		// ����ID
	BYTE	byResult;			// 0-�ɹ� 1-ʧ��
CMD_END

//-------------------------------------------------------------------------------
// Ԫ������ȷ��
//-------------------------------------------------------------------------------
CMD_START(NBS_YuanbaoAck)
	CHAR	szTokenID[50];		// ����ID
	DWORD	dwWorldID;			// ��Ϸ����ID
CMD_END

//------------------------------------------------------------------------------
// ���߻���
//------------------------------------------------------------------------------
CMD_START(NBS_Item)
	CHAR	szTokenID[50];		// ����ID
	DWORD	dwWorldID;			// ��Ϸ����ID
	CHAR	szAccountName[50];	// �ʺ���
	DWORD	dwItemID;			// ��ƷID
	INT		nNum;				// ����
CMD_END

CMD_START(NBC_Item)
	CHAR	szTokenID[50];		// ����ID
	BYTE	byResult;			// 0-�ɹ� 1-ʧ��
CMD_END

//--------------------------------------------------------------------------------
// ���߻���ȷ��
//--------------------------------------------------------------------------------
CMD_START(NBS_ItemAck)
	CHAR	szTokenID[50];		// ����ID
	DWORD	dwWorldID;			// ��Ϸ����ID
CMD_END

#pragma pack(pop)