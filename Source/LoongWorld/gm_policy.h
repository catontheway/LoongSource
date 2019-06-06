//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: gm_policy.h
// author: xlguo
// actor:
// data: 2009-06-08
// last:
// brief: GM������
//-----------------------------------------------------------------------------
#pragma once

#include "time_limit_mgr.h"
#include "world.h"
#include "gm_policy_part.h"


//-----------------------------------------------------------------------------
// GM˫������
//-----------------------------------------------------------------------------
class GMDoublePolicy
{
public:
	//-----------------------------------------------------------------------------
	// ����������
	//-----------------------------------------------------------------------------
	GMDoublePolicy();
	virtual ~GMDoublePolicy(){}

public:
	//-----------------------------------------------------------------------------
	// ��ȡ���ʣ���ͼ�̵߳��ã�
	//-----------------------------------------------------------------------------
	virtual FLOAT	GetExpRate() const 							
	{	
		return m_bDouble[EDoubleType_Exp] ? m_fCurDouble[EDoubleType_Exp] : 1.0f;
	}
	virtual FLOAT	GetLootRate(BOOL bNormalMap) const	
	{
		return m_bDouble[EDoubleType_Item] ? (bNormalMap ? m_fCurDouble[EDoubleType_Item] : 1.0f) : 1.0f;	
	}

public:
	//-----------------------------------------------------------------------------
	// ����˫�������̣߳�
	//-----------------------------------------------------------------------------
	virtual VOID	SetRate(EDoubleType eDoubleType, DWORD dwRate, DWORD dwStart, DWORD dwLast );				

	//-----------------------------------------------------------------------------
	// ˫������
	//-----------------------------------------------------------------------------
	virtual VOID	DoubleSwitch(EDoubleType eType, BOOL bFlag) { m_bDouble[eType] = bFlag; }		

	//-----------------------------------------------------------------------------
	// ���£����̣߳�
	//-----------------------------------------------------------------------------
	VOID	Update();

private:
	FLOAT	m_fCurDouble[NUM_EDT];		// ˫������ 0-5
	BOOL	m_bDouble[NUM_EDT];			// ˫������
	DWORD	m_dwEndTime[NUM_EDT];		// ˫�����ʵĽ�ֹʱ��
	
	//TimeLimitMgr<EDoubleAct> m_TLMgr;	// ʱ�޹�����
};

//-----------------------------------------------------------------------------
// GM����
//-----------------------------------------------------------------------------
class GMPolicy
{
public:
	//-----------------------------------------------------------------------------
	// ����������
	//-----------------------------------------------------------------------------
	GMPolicy()
	{
		m_pDoublePolicy = new GMDoublePolicy;
		//m_pNoticePolicy	= new GMNoticePolicy;
	}
	virtual ~GMPolicy()
	{
		SAFE_DEL(m_pDoublePolicy);
		//SAFE_DEL(m_pNoticePolicy);
	}

public:
	//-----------------------------------------------------------------------------
	// ��ȡ���ʣ���ͼ�̵߳��ã�
	//-----------------------------------------------------------------------------
	FLOAT	GetExpRate() const 							
	{	
		return m_pDoublePolicy->GetExpRate();
	}
	FLOAT	GetLootRate(BOOL bNormalMap = TRUE) const	
	{
		return m_pDoublePolicy->GetLootRate(bNormalMap);
	}

	//-----------------------------------------------------------------------------
	// ����˫�������̣߳�
	//-----------------------------------------------------------------------------
	VOID	SetRate(EDoubleType eDoubleType, DWORD dwRate, DWORD dwStart, DWORD dwLast )
	{
		m_pDoublePolicy->SetRate(eDoubleType, dwRate, dwStart, dwLast);
	}

	//-----------------------------------------------------------------------------
	// ˫������
	//-----------------------------------------------------------------------------
	VOID	DoubleSwitch(EDoubleType eType, BOOL bFlag) { m_pDoublePolicy->DoubleSwitch(eType, bFlag); }

public:
	//-----------------------------------------------------------------------------
	// ���£����̣߳�
	//-----------------------------------------------------------------------------
	VOID Update()
	{
		m_pDoublePolicy->Update();
		//m_pNoticePolicy->Update();
	}

private:
	GMDoublePolicy*		m_pDoublePolicy;
	//GMNoticePolicy*		m_pNoticePolicy;
};

extern GMPolicy g_GMPolicy;