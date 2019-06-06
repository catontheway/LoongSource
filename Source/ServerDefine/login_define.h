//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: login_define.h
// author: 
// actor:
// data: 2008-06-04
// last:
// brief: ��½������������ݽṹ
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/time.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// ��Ϸ����״̬
//-----------------------------------------------------------------------------
enum EWorldStatus
{
	EWS_Well			=	0,		// ����
	EWS_InitNotDone		=	1,		// δ��ʼ�����
	EWS_SystemError		=	2,		// ϵͳ���ֹ���
	EWS_ProofError		=	3,		// ��֤����������
	EWS_DBBlock			=	4,		// ���ݿ�����
};

//------------------------------------------------------------------------------
// ��֤����
//------------------------------------------------------------------------------
enum EProofPolicy
{
	EPP_Null	=	-1,
	EPP_Test	=	0,		// ����ר��
	EPP_Own		=	1,		// �Լ���˾��֤����
	EPP_XunLei	=	2,		// xunlei
	EPP_Orange	=	3,		// game orange

	EPP_End		=	4		
};

//------------------------------------------------------------------------------
// ��ҵ�¼ʱ��״̬
//------------------------------------------------------------------------------
enum  EPlayerLoginStatus
{
	EPLS_Null		=	-1,		// ��Ч
	EPLS_OffLine	=	0,		// ����
	EPLS_Online		=	1,		// ����
	EPLS_Loging		=	2,		// ��¼��
	EPLS_Unknown	=	3,		// δ֪
};

//------------------------------------------------------------------------------
// ��ҷ�ͣ����  ��Ա���ķ�ͣ0x01��GM���߷�ͣ0x02���ֻ���0x04���ܱ���0x08
//------------------------------------------------------------------------------
enum  EPlayerFrobidMask
{
	EPLM_MemberCenter	=	0x01,		// ��Ա����
	EPLM_GMTool			=	0x02,		// GM����
	EPLM_CellPhone		=	0x04,		// �ֻ���
	EPLM_MiBao			=	0x08,		// �ܱ�����ͣ
	EPLM_WaiGua			=	0x10,		// ������Զ���ͣ
	EPLM_Level			=	0x20,		// �ȼ��쳣
};
//-----------------------------------------------------------------------------
// ��֤����ṹ
//-----------------------------------------------------------------------------
struct tagProofResult
{
	DWORD					dwClientID;			// �ͻ���ID
	DWORD					dwAccountID;		// �ʺ�ID
	BOOL					bGuard;				// ������
	INT						nGuardAccuTime;		// �������ۻ�ʱ��
	EPlayerLoginStatus		eLoginStatus;		// ��½״̬
	DWORD					dwWorldNameCrc;		// ��ǰ��½����Ϸ����
	DWORD					dwFrobidMask;		// ��ͣ���� ÿ1λ��Ӧһ�ַ�ͣ����
	BYTE					byPrivilege;		// Ȩ��
	bool					bNeedMibao;			// �Ƿ�ʹ���ܱ�
	DWORD					dwPreLoginIP;		// �ϴε�¼ip
	tagDWORDTime			dwPreLoginTime;		// �ϴε�¼ʱ��
	BYTE					byReserved[2];		// ����������

	tagProofResult()
	{
		dwClientID		=	GT_INVALID;
		dwAccountID		=	GT_INVALID;
		bGuard			=	FALSE;
		nGuardAccuTime	=	0;
		eLoginStatus	=	EPLS_OffLine;
		byPrivilege		=	0;
		bNeedMibao		=	false;
		dwFrobidMask	=   0;
		dwPreLoginIP    =	0;
		dwPreLoginTime	=   0;
	}
};

struct tagProofResultFull : public tagProofResult
{
	INT nRet;		// ���ؽ��
};

//-----------------------------------------------------------------------------
// ��֤�ص�����
//-----------------------------------------------------------------------------
typedef UINT (WINAPI* PROOFCALLBACK)(INT nRet, tagProofResult* pResult);


#pragma pack(pop)