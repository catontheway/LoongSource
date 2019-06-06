//-----------------------------------------------------------------------------
//!\file World.h
//!\author Aslan
//!
//!\date 2008-06-10
//! last 2008-06-10
//!
//!\brief ����������������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "mutex.h"

//-----------------------------------------------------------------------------
// ����������������
//-----------------------------------------------------------------------------
class World
{

public:
	World();
	~World();

	BOOL Init(HINSTANCE hInst);
	VOID Mainloop();
	VOID Destroy();

	BOOL	IsOverload() const			{ return m_bOverloadAlert; }
	DWORD	GetMaxTickTime() const		{ return m_dwMaxTickTime; }
	DWORD	GetIdleTime() const			{ return m_dwIdleTime; }
	DWORD	GetTotalRunMinute() const	{ return m_dwTotalRunMinute; }
	BYTE	GetSaveOneTicks() const		{ return m_bySaveOneNeedTicks; }
	DWORD	GetGoldenCode() const		{ return m_dwGoldenCode; }
	LPCTSTR	GetWorldName() const		{ return m_strWorldName.c_str(); }
	INT		GetCpuNum()	const			{ return m_nCpuNum; }
	DWORD	GetTotalMemory() const		{ return m_dwTotalPhys; }
	DWORD	GetAvailMemory() const		{ return m_dwAvailPhys; }
	INT		GetQuotaNonPagedPoolUsage()	{ return m_nQuotaNonPagedPoolUsage; }

	VOID	ShutDown()					{ InterlockedExchange((LONG*)&m_bShutingdown, TRUE); }
	BOOL	IsShutingdown() const		{ return m_bShutingdown; }

	VOID	SwitchGameGuarder(BOOL bOn);
	BOOL	IsWell() const;
	VOID	UpdateMemoryUsage();	

	DWORD	GetWorldTick()				{ return m_dwWorldTick; }
	VOID	SetWorldTickInc()			{ InterlockedExchangeAdd((LPLONG)&m_dwWorldTick, 1); }

	DWORD	GetMaxMsgSize()				{ return m_dwMaxMsgSize; }
	VOID	SetMaxMsgSize(DWORD dwMsgSize)	{ InterlockedExchange((LONG*)&m_dwMaxMsgSize, dwMsgSize); }

	tagDWORDTime GetWorldTime()			{ return m_CurrentTime; }
	VOID	UpdateWorldTime()			{ m_CurrentTime = GetCurrentDWORDTime(); }

	VOID	LockNetWork()				{ m_MutexNetwork.Acquire(); }
	VOID	UnlockNetWork()				{ m_MutexNetwork.Release(); }

public:
	FLOAT	GetLaxCheatDistanceSQ()		{ return m_fLaxCheatDistanceSQ; }
	FLOAT	GetStrictCheatDistanceSQ()	{ return m_fStrictCheatDistanceSQ; }

	TObjRef<Util>&		GetUtil()			{ return m_pUtil; }
	TObjRef<FastCode>&	GetFastCode()		{ return m_pFastCode; }
	TObjRef<Log>&		GetLog()			{ return m_pLog; }

public:
	// crc32������װ -- ��ת��ΪСд�ټ���
	DWORD	LowerCrc32(LPCTSTR str);

private:
	// ����������Ϸ������������
	VOID	SaveWorldInfoToDB();
	VOID	WaitDBTerminate();
private:
	TSFPTrunk<World>		m_Trunk;

	TObjRef<Thread>			m_pThread;
	TObjRef<GDIRender>		m_pRender;
	TObjRef<WindowWrap>		m_pWindowWrap;
	TObjRef<GUISystem>		m_pGUI;
	TObjRef<Util>			m_pUtil;
	TObjRef<FastCode>		m_pFastCode;
	TObjRef<Console>		m_pConsole;
	TObjRef<Log>			m_pLog;

	IGUIRenderNative*		m_pGUIRender;

	volatile BOOL			m_bShutingdown;						// �������Ƿ����ڹر�
	volatile BOOL			m_bTerminateUpdate;					// �������߳�ֹͣλ
	volatile BOOL			m_bOverloadAlert;					// ���������ؾ���
	volatile DWORD			m_dwIdleTime;						// �ϴη�������ѭ������ʱ��
	volatile DWORD			m_dwMaxTickTime;					// ����Tickʱ��
	volatile BYTE			m_bySaveOneNeedTicks;				// ����Tick����һ�����

	volatile DWORD			m_dwMaxMsgSize;

	DWORD					m_dwViewWidth;
	DWORD					m_dwViewHeight;

	INT						m_nCpuNum;
	DWORD					m_dwTotalPhys;
	DWORD					m_dwAvailPhys;
	INT						m_nQuotaNonPagedPoolUsage;

	tstring					m_strWorldName;						// ��Ϸ��������
	DWORD					m_dwGoldenCode;						// �����������golden_code

	DWORD					m_dwLastMinute;
	DWORD					m_dwLastHour;
	DWORD					m_dwTotalRunMinute;
	volatile DWORD			m_dwWorldTick;						// ��Ϸ��������
	tagDWORDTime			m_CurrentTime;						// ��ǰϵͳʱ��
	DWORD					m_dwLastSaveTick;					// ��һ�α�����Ҽ�¼����

	FLOAT					m_fLaxCheatDistanceSQ;				// ���������µľ���ƽ��
	FLOAT					m_fStrictCheatDistanceSQ;			// ǿ�������µľ���ƽ��

	Mutex					m_MutexNetwork;						// ���������������ײ��logincallback��logoutcallback���л���

private:
	// Update�߳�
	DWORD ThreadUpdate(LPVOID);
	// ��¼���������ߵ��ö�ջ��Ϣ
//	DWORD RecordStack(LPCSTR);
	// �޸Ķ��ٸ�tick����һ���������
	DWORD ChangeSaveTicks(LPCSTR szTicks);
	// ÿ����ִ��һ��
	VOID OnMinute();
	// ÿСʱִ��һ��
	VOID OnHour();
	// ÿ����ִ��һ��
	VOID OnClock(BYTE byHour);
	// ���������Ϣ
	VOID SaveRolePerTick();
};

extern World g_world;

#define	IUTIL		(g_world.GetUtil())
#define IFASTCODE	(g_world.GetFastCode())
#define ILOG		(g_world.GetLog())