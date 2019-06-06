//-----------------------------------------------------------------------------
//!\file fatigue_guarder.h
//!\author xlguo
//!
//!\date 2009-02-05
//! last 
//!
//!\brief ������ϵͳ
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "..\WorldDefine\time.h"
#include "..\WorldDefine\base_define.h"
#include "..\WorldDefine\msg_fatigue_guarder.h"
#include "..\ServerDefine\fatigue_define.h"

//-----------------------------------------------------------------------------
// ���Է�����
//-----------------------------------------------------------------------------
class FatigueGuarder
{
	enum
	{
		NUM_POINTS_NOTIFY = 8,		// ֪ͨ�ͻ���ʱ���ĸ���
	};

	//-----------------------------------------------------------------------------
	// ״̬�ṹ
	//-----------------------------------------------------------------------------
	struct tagStateRangeRate
	{
		DWORD	dwBegin;
		DWORD	dwEnd;
		DWORD	dwEarnRate;
	};

public:
	FatigueGuarder(PlayerSession* pPlayerSession, BOOL bGuard = TRUE, DWORD dwAccOLAcc = 0)
		:m_bGuard(bGuard), m_dwAccOLMin(dwAccOLAcc), m_dwState(0), m_pPlayerSession(pPlayerSession)
	{
		ASSERT( P_VALID(m_pPlayerSession) );
	}

	BOOL IsGuard()	const {return m_bGuard;}
	//-----------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------
	VOID SetAccOLTimeMin(DWORD dwState, DWORD dwAccOLTimeMin);

	//-----------------------------------------------------------------------------
	// ������
	//-----------------------------------------------------------------------------
	DWORD GetEarnRate()	const 
	{	
		if ( !m_bGuard ) return 10000;
		ASSERT( EFS_VALID(m_dwState) );
		return m_arrStateRangeRate[m_dwState].dwEarnRate; 
	}

	//-----------------------------------------------------------------------------
	// ֪ͨ�ͻ���
	//-----------------------------------------------------------------------------
	void NotifyClient() const ;
private:
	//-----------------------------------------------------------------------------
	// ���״̬
	//-----------------------------------------------------------------------------
	static EnumFatigueState GetState(const DWORD& dwAccOLMin)
	{
		for (INT i = EFS_BEGIN; i < EFS_END; ++i)
		{
			if ( dwAccOLMin >= m_arrStateRangeRate[i].dwBegin&&
				 dwAccOLMin < m_arrStateRangeRate[i].dwEnd )
			{
				return static_cast<EnumFatigueState>(i);
			}
		}
		ASSERT( 0 );
		return static_cast<EnumFatigueState>(GT_INVALID);		
	}
	
	
private:

	BOOL						m_bGuard;									// �Ƿ��Ƿ������û�
	DWORD						m_dwState;									// ��Ϸ״̬
	DWORD						m_dwAccOLMin;								//  ��ǰ�ۼ�����ʱ��

	PlayerSession*				m_pPlayerSession;							// ��ǰ PlayerSession

	static tagStateRangeRate	m_arrStateRangeRate[EFS_NUM];				// �������б�
};
