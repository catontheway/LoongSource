#pragma once
#include "stdafx.h"

struct GameWorld
{
	enum EWorldStatus
	{
		EWS_Well			=	0,		// ����
		EWS_InitNotDone		=	1,		// δ��ʼ�����
		EWS_SystemError		=	2,		// ϵͳ���ֹ���
		EWS_ProofError		=	3,		// ��֤����������
		EWS_DBBlock			=	4,		// ���ݿ�����
	};
public:
	//void				PrintInfo(FLOAT fRate, LPCTSTR szSectionName);
	
	void				SetWorldID(DWORD dwID)			{ m_dwWorldID = dwID; }
	void				SetWorldName(CString szName)	{ m_strWorldName = szName;}
	void				SetOnline(INT nNum)				{ m_nOnline = nNum; }
	void				SetOnlintLimit(INT nNum)		{ m_nOnlineLimit = nNum;}
	void				SetStatus(BOOL bActive)			{ m_bActive = bActive; }
	void				SetWorldStatus(EWorldStatus s)	{ m_eWorldStatus = s;}
	void				SetDBStatus(EWorldStatus s)		{ m_eDBStatus = s;}

	DWORD				GetWorldID()					{ return m_dwWorldID; }
	CString				GetWorldName()					{ return m_strWorldName; }
	INT					GetOnline()						{ return m_nOnline; }
	INT					GetOnlineLimit()const			{ return m_nOnlineLimit; }
	EWorldStatus		GetWorldStatus()const			{ return m_eWorldStatus;}
	EWorldStatus		GetDBStatus()const				{ return m_eDBStatus;}
private:
	DWORD				m_dwWorldID;
	CString				m_strWorldName;
	INT					m_nOnline;
	INT					m_nOnlineLimit;
	EWorldStatus		m_eWorldStatus;
	EWorldStatus		m_eDBStatus;

	BOOL				m_bActive;
};



class Section
{
public:
	//void							PrintInfo(FLOAT fRate);

	void							SetSectionID(DWORD dwID) { m_dwSectionID = dwID; }
	void							SetSectionName(CString szName) { m_strSectionName=szName;}
	
	void							AddWorld(INT n, GameWorld* pWorld)	{ m_mapWorld.Add(n, pWorld); }
	CString							GetSectionName()	{ return m_strSectionName; }
	DWORD							GetSectionID()		{ return m_dwSectionID; }
	TMap<INT, GameWorld*>&			GetWorldMap()		{ return m_mapWorld; }
	
private:
	DWORD							m_dwSectionID;						// ����ID
	CString							m_strSectionName;					// ��������
	TMap<INT, GameWorld*>			m_mapWorld;	
};