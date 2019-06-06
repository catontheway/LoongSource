//-----------------------------------------------------------------------------
//!\file world_db.h
//!\author Aslan
//!
//!\date 2009-06-09
//! last 2009-06-09
//!
//!\brief ��Ϸ�������ݿ�
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "../ServerDefine/bill_define.h"

class World;

//-----------------------------------------------------------------------------
// ��Ϸ�������ݿ�
//-----------------------------------------------------------------------------
class WorldDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	WorldDB(World* pWorldID) :
	  m_n64ItemSerial(GT_INVALID), m_pWorld(pWorldID){}
	~WorldDB() {}

	//-------------------------------------------------------------------------
	// ��ʼ��������
	//-------------------------------------------------------------------------
	BOOL	Init(INT nIndex);
	VOID	Update();
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// �Ƿ�Ͽ�����
	//-------------------------------------------------------------------------
	BOOL	IsConLost()				{ return m_DB.IsConnLost(); }

	//--------------------------------------------------------------------------
	// ��������
	//--------------------------------------------------------------------------
	BOOL	GetCentralbillingInfo(INT& nNum);

	BOOL	GiveYuanbaoToAccount(DWORD dwAccountID, INT nNum, Connection* pCon);
	BOOL	GiveItemToAccountBaibao(DWORD dwAccountID, DWORD dwItemTypeID, INT nNum, Connection* pCon, OUT INT64& n64SerialID);

	BOOL	MoveBillInfo2Log(DWORD dwTokenID, Connection* pCon);

	BOOL	SetCentralbillingState(DWORD dwTokenID, INT nState, Connection* pCon);

	//���Ԫ������log
	BOOL	InsertBillYuanbaoLog(const LPCSTR szTokenID,DWORD dwAccountID, INT nNum );
	BOOL	InsertBillItemLog(const LPCSTR szTokenID,DWORD dwAccountID, DWORD dwItemTypeID,INT nNum );
	//--------------------------------------------------------------------------
	// ��������
	//--------------------------------------------------------------------------
	DWORD	GetAccountID(const LPCSTR szAccountName);

	BOOL	LockTables(LPCSTR szTableNames);
	BOOL	UnlockTables();
	//--------------------------------------------------------------------------
	// �˺ſ��ƺ���
	//--------------------------------------------------------------------------
	BOOL	InsertAccout(DWORD dwAccountID, LPCSTR szAccountName);

	//--------------------------------------------------------------------------
	// ��ƷID���
	//--------------------------------------------------------------------------
	BOOL	GetCurMinItemSerialInDB(INT64& n64ItemSerial);
	BOOL	GetMinItemSerialInTable(LPCSTR szTable, INT64& n64ItemSerial);
	INT64	GenerateNew64ItemSerial()	{ return --m_n64ItemSerial; }


	//�õ�������ڵ�Ԫ����
	DWORD	GetCurAccountYuanbao(DWORD dwAccountID);
private:
	//--------------------------------------------------------------------------
	// ����
	//--------------------------------------------------------------------------
	VOID	Reconnect();
	
private:
	DataBase				m_DB;				// Beton���ݿ�
	World*					m_pWorld;

	INT64					m_n64ItemSerial;	// ��Ʒ64λ���к�

	tagBillInfo				m_sBillInfo[BILLINFO_PERTICK];
};