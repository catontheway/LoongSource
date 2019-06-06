//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: marriage.h
// author: songg
// actor:
// data: 2009-09-10
// last:
// brief: ���ϵͳ����
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/msg_marriage.h"

//ǰ������

//-----------------------------------------------------------------------------
// ��ҽ��/����Ĺ�����
//-----------------------------------------------------------------------------
class MarriageMgr
{
public:
	MarriageMgr();
	~MarriageMgr();
	
public:
	VOID Init();//��ʼ��

public:

	VOID Update();

	BOOL IsHavingWedding()					{	return m_bHavingWedding;} //�Ƿ��л����ڽ���

	VOID BegWedding()						{	m_bHavingWedding = TRUE;	}//��ʼ����

	VOID EndWedding()						{	ClearWeddingState();	}//��������

	VOID EnableSysOfficiator()				{	m_bSysOfficiator = TRUE;	}

	VOID DisableSysOfficiator()				{	m_bSysOfficiator = FALSE;	}

public:
	//-----------------------------------------------------------------------------
	// ����set get
	//-----------------------------------------------------------------------------
	DWORD	GetOfficiatorRoleID()				{	return m_dwOfficiatorRoleID;}
	DWORD	GetApplicantRoleID()				{	return m_dwApplicantRoleID;}
	DWORD	GetTargetRoleID()					{	return m_dwTargetRoleID;}

	VOID	SetOfficiatorRoleID(DWORD OfficiatorRoleID) { m_dwOfficiatorRoleID = OfficiatorRoleID;}
	VOID	SetTargetRoleID(DWORD TargetRoleID)			{ m_dwTargetRoleID = TargetRoleID;}
	VOID	SetApplicantRoleID(DWORD ApplicantRoleID)	{ m_dwApplicantRoleID = ApplicantRoleID;}

private:
	//��ս��״̬��Ϣ
	VOID ClearWeddingState();

private:
	//-----------------------------------------------------------------------------
	// �������
	//-----------------------------------------------------------------------------
	BOOL m_bHavingWedding; //�Ƿ��л����ڽ���

	EWeddingStep m_eWeddingStep; //������

	TCHAR m_szMaleRolePledge[X_LONG_STRING];	//�з�����

	TCHAR m_szFemaleRolePledge[X_LONG_STRING];	//Ů������

	DWORD m_dwOfficiatorRoleID; //������

	DWORD m_dwTargetRoleID; //��������

	DWORD m_dwApplicantRoleID; //������

	BOOL m_bSysOfficiator; //�Ƿ���ϵͳ����


private:
	//-----------------------------------------------------------------------------
	// �ű��������
	//-----------------------------------------------------------------------------
	//���л�����Ҫ�Ľ�Ǯ
	DWORD m_dwWeddingDecreaseSilver; 
};
extern MarriageMgr g_MarriageMgr;
