//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: exchange_define.h
// author: Sxg
// actor:
// data: 2008-08-25
// last:
// brief: ��Ҽ佻��
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
const INT32 MAX_EXCHANGE_ITEM			= 12;	// һ����ཻ��12����Ʒ//??
const INT32 MAX_EXCHANGE_MONEY			= 0;	// 0
const INT32 MAX_EXCHANGE_DISTANCE		= 100;


//-----------------------------------------------------------------------------
struct tagExchangeData
{
	DWORD	dwRoleID;		// Ŀ����ң�GT_INVALID��ʾ�޽���Ŀ��
	BOOL	bLock;			// ����Ƿ�ȷ�Ͻ���
	BOOL	bVerify;		// ����Ƿ��ٴ�ȷ�Ͻ���
	BYTE	byTypeNum;		// ������Ʒ�������
	BOOL	bDummy[2];		// ������	
	INT64	n64Money;		// ���׵Ľ�Ǯ
	INT64	n64Serial[MAX_EXCHANGE_ITEM];	// ���׵���Ʒ��0��ʾû�н�����Ʒ
	INT16	n16ItemNum[MAX_EXCHANGE_ITEM];	// ��������Ʒ�ĸ���

	tagExchangeData(DWORD dwTgtRoleID) { ZeroMemory(this, sizeof(tagExchangeData)); dwRoleID = dwTgtRoleID;}
};
