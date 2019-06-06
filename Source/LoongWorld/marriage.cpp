//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: marriage.cpp
// author: songg
// actor:
// data: 2008-09-02
// last:
// brief: ���ϵͳ����
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "marriage.h"
#include "script_mgr.h"
#include "role_mgr.h"
#include "role.h"
#include "../WorldDefine/chat.h"
#include "../ServerDefine/base_define.h"

MarriageMgr g_MarriageMgr;
//-----------------------------------------------------------------------------
// ����&��������
//-----------------------------------------------------------------------------
MarriageMgr::MarriageMgr()
:m_bHavingWedding(FALSE),m_eWeddingStep(ES_Wedding_None),m_dwOfficiatorRoleID(0)
,m_dwTargetRoleID(0),m_dwApplicantRoleID(0),m_bSysOfficiator(FALSE),m_dwWeddingDecreaseSilver(0)
{

}

MarriageMgr::~MarriageMgr()
{

}
VOID MarriageMgr::ClearWeddingState()
{
	m_bHavingWedding = FALSE;
	m_eWeddingStep = ES_Wedding_None;
	ZeroMemory(m_szMaleRolePledge,sizeof(TCHAR)*X_LONG_STRING);
	ZeroMemory(m_szFemaleRolePledge,sizeof(TCHAR)*X_LONG_STRING);
	m_dwOfficiatorRoleID = 0;
	m_dwTargetRoleID = 0;
	m_dwApplicantRoleID = 0;
	m_bSysOfficiator = 0;

}
//-----------------------------------------------------------------------------
// ˢ��
//-----------------------------------------------------------------------------
VOID MarriageMgr::Update()
{

}
//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
VOID MarriageMgr::Init()
{
	//���л��������Ǯ
	g_ScriptMgr.GetGlobal("marriage_WeddingDecSilver", m_dwWeddingDecreaseSilver);

}