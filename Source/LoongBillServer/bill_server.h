//-----------------------------------------------------------------------------
//!\file bill_server.h
//!\author Aslan
//!
//!\date 2009-06-09
//! last 2009-06-09
//!
//!\brief ֱ�仮��������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
// ֱ�廮��������
//-----------------------------------------------------------------------------
class BillServer
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	BillServer() : m_Trunk(this), m_pGUIRender(NULL), m_dwSectionID(GT_INVALID),
					m_dwViewWidth(0), m_dwViewHeight(0), m_nCPUNum(0), m_dwTotalPhys(0), m_dwAvailPhys(0),
					m_nQuotaNonPagedPoolUsage(0), m_nTick(0), m_bIsInitOK(FALSE), m_bIsShutingDown(FALSE) {}
	~BillServer() {}

	//-------------------------------------------------------------------------
	// ��ʼ������ѭ��������
	//-------------------------------------------------------------------------
	BOOL		Init(HINSTANCE hInst);
	VOID		MainLoop();
	VOID		Destroy();

	//-------------------------------------------------------------------------
	// �����͹ر�
	//-------------------------------------------------------------------------
	BOOL		IsInitOK()					{ return m_bIsInitOK; }
	BOOL		IsShutingDown()				{ return m_bIsShutingDown; }
	VOID		InitOK()					{ InterlockedExchange((LPLONG)&m_bIsInitOK, TRUE); }
	VOID		ShutDown()					{ InterlockedExchange((LPLONG)&m_bIsShutingDown, TRUE); }

	//-------------------------------------------------------------------------
	// ����Get
	//-------------------------------------------------------------------------
	DWORD		GetSectionID()				{ return m_dwSectionID; }
	DWORD		GetTick()					{ return m_nTick++; }
	INT			GetCPUNum()					{ return m_nCPUNum; }
	DWORD		GetTotalPhyMem()			{ return m_dwTotalPhys; }
	DWORD		GetAvailPhyMem()			{ return m_dwAvailPhys; }
	INT			GetNonPagedPoolMem()		{ return m_nQuotaNonPagedPoolUsage; }

	//-------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------
	TObjRef<Util>	GetUtil()				{ return m_pUtil; }
	TObjRef<Log>	GetLog()				{ return m_pLog; }

private:
	//-------------------------------------------------------------------------
	// �����߳�
	//-------------------------------------------------------------------------
	VOID		ThreadUpdate();

	//-------------------------------------------------------------------------
	// �����ڴ�ռ��
	//-------------------------------------------------------------------------
	VOID		UpdateMemoryUsage();

	//-------------------------------------------------------------------------
	// ��ʾ����ʱ��Ϣ
	//-------------------------------------------------------------------------
	VOID		DisplayRunTimeInfo();

private:
	
	TSFPTrunk<BillServer>	m_Trunk;

	TObjRef<GDIRender>		m_pRender;
	TObjRef<WindowWrap>		m_pWindowWrap;
	TObjRef<GUISystem>		m_pGUI;
	TObjRef<Util>			m_pUtil;
	TObjRef<Console>		m_pConsole;
	TObjRef<Log>			m_pLog;

	IGUIRenderNative*		m_pGUIRender;

	//--------------------------------------------------------------------------
	// ���ڴ�С
	//--------------------------------------------------------------------------
	DWORD					m_dwViewWidth;
	DWORD					m_dwViewHeight;

	//--------------------------------------------------------------------------
	// ״̬
	//--------------------------------------------------------------------------
	volatile BOOL			m_bIsInitOK;						// ��ʼ���ɹ�
	volatile BOOL			m_bIsShutingDown;					// ���ڹر�

	//--------------------------------------------------------------------------
	// ����ID
	//--------------------------------------------------------------------------
	DWORD					m_dwSectionID;						// ����ID

	//--------------------------------------------------------------------------
	// ͳ����Ϣ
	//--------------------------------------------------------------------------
	INT						m_nCPUNum;							// CPU����
	DWORD					m_dwTotalPhys;						// �ܹ��������ڴ�
	DWORD					m_dwAvailPhys;						// �ܹ��������ڴ�
	INT						m_nQuotaNonPagedPoolUsage;			// ��ҳ�滺���ڴ�
	volatile DWORD			m_nTick;							// ������
};

extern BillServer g_Server;					// ȫ�ֶ���

#define IUTIL	(g_Server.GetUtil())
#define ILOG	(g_Server.GetLog())