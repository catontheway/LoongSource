//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: main.h
// author: Sxg
// actor:
// data: 2008-05-12
// last:
// brief: Ӧ�ò���ض���
//-----------------------------------------------------------------------------
#pragma once




struct tagLoongDBUI
{
//	char		szServerName[X_SHORT_NAME];				// ��������

	INT			nClientNum;								// �ͻ�������

	DWORD		dwSendPack;								// �����İ�����
	DWORD		dwRecvPack;								// �յ��İ�����
	DWORD		dwResendPack;							// �ط��İ�����
	DWORD		dwSendBytes;							// �������ֽ���
	DWORD		dwRecvBytes;							// �յ����ֽ���

//	tagUIWorld	SimpleWorld[CACHE_MAX_WORLD];			// ��Ϸ������Ϣ

};