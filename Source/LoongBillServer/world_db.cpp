//-----------------------------------------------------------------------------
//!\file world_db.cpp
//!\author Aslan
//!
//!\date 2009-06-26
//! last 2009-06-26
//!
//!\brief ��Ϸ�������ݿ�
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../WorldDefine/ItemDefine.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/msg_bill_s.h"

#include "world_db.h"
#include "login_db.h"

#include "world.h"
#include "world_mgr.h"

//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL WorldDB::Init(INT nIndex)
{
	TObjRef<VarContainer> pVar = "BillConfig";

	// ��ʼ�����ݿ�
	TCHAR szTemp[X_SHORT_NAME] = {_T('\0')};
	_stprintf(szTemp, _T("world_%d"), nIndex);

	BOOL bRet = m_DB.Init(pVar->GetString(_T("db_host"), szTemp), 
							pVar->GetString(_T("db_user"), szTemp),
							pVar->GetString(_T("db_psd"), szTemp), 
							pVar->GetString(_T("db_name"), szTemp),
							pVar->GetInt(_T("db_port"), szTemp));

	
	// �����ʼ���ɹ�����ѯ��ǰ���ݿ�����Ʒ���кŵ���СID
	if( bRet )
	{
		return GetCurMinItemSerialInDB(m_n64ItemSerial);
	}
	else
	{
		return bRet;
	}
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID WorldDB::Destroy()
{

}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID WorldDB::Update()
{
	// ����
	if( IsConLost() )
		Reconnect();

	// ��ȡcentralbilling����Ϣ
	INT nNum = 0;
	if (!GetCentralbillingInfo(nNum))
		return;

	// �����Ѿ���ȡ�Ļ�����Ϣ
	for (int n=0; n<nNum; n++)
	{
		BOOL bRet = TRUE;
		INT32 nCurYuanBao = 0;

		DWORD dwAccountID = GetAccountID(m_sBillInfo[n].szAccountName);
		INT64 n64Serial = GT_INVALID;

		//�����account_common����ò������� ���Զ����
		if( !P_VALID(dwAccountID) )
		{
			//������ ���û���ҵ���Ӧ����� ��Ҫ�Զ����
			LockTables("account_common");
			g_LoginDB.LockTables("account");

			//������֮ǰ���ж�world��֮����һ�����ݱ�������
			dwAccountID = GetAccountID(m_sBillInfo[n].szAccountName);
			if( P_VALID(dwAccountID) )
			{
				//����
				UnlockTables();
				g_LoginDB.UnlockTables();
				goto ACCOUNT_EXIST; //�˺Ŵ���
			}

			//�Զ����account
			CHAR szMibao[MIBAO_LEN];
			memset(szMibao,0,MIBAO_LEN);
			g_LoginDB.InsertAccout(m_sBillInfo[n].szAccountName,"",szMibao,FALSE);

			dwAccountID = g_LoginDB.GetAccountID(m_sBillInfo[n].szAccountName);

			//֮ǰ�Ĳ���ʧ��
			if( !P_VALID(dwAccountID) )
			{	
				bRet = FALSE;
				//����
				UnlockTables();
				g_LoginDB.UnlockTables();
				continue;
			}
			else
			{
				//����account_common�������
				bRet = InsertAccout(dwAccountID, m_sBillInfo[n].szAccountName);
			}

			//����
			UnlockTables();
			g_LoginDB.UnlockTables();
		}

//�˺Ŵ���
ACCOUNT_EXIST:

		//�õ���ҵ�ǰ��Ԫ����
		nCurYuanBao = GetCurAccountYuanbao(dwAccountID);
		if(nCurYuanBao == GT_INVALID)
		{
			continue;
		}

		// ��������
		Connection* pCon = m_DB.GetFreeConnection();

		if( !m_DB.BeginTransaction(pCon) )
		{
			m_DB.ReturnConnection(pCon);
			m_sBillInfo[n].Reset();
			continue;
		}

		// �ȸ��»�����
		if (FALSE == bRet 
			|| FALSE == SetCentralbillingState(m_sBillInfo[n].dwTokenID, EBST_Success, pCon)
			|| FALSE == MoveBillInfo2Log(m_sBillInfo[n].dwTokenID, pCon))
		{
			m_DB.RollBack(pCon);	// ����
			SetCentralbillingState(m_sBillInfo[n].dwTokenID, EBST_Null, pCon);
			m_sBillInfo[n].Reset();
			m_DB.EndTransaction(pCon);
			m_DB.ReturnConnection(pCon);
			continue;
		}
		else
		{
			// ��������Ϸ����
			switch (m_sBillInfo[n].eBillType)
			{
			case EBBT_Yuanbao:
				bRet = GiveYuanbaoToAccount(dwAccountID, m_sBillInfo[n].nNum, pCon);
				break;

			case EBBT_Item:
				bRet = GiveItemToAccountBaibao(dwAccountID, m_sBillInfo[n].dwTypeID, m_sBillInfo[n].nNum, pCon, n64Serial);
				break;
			}

			if (FALSE == bRet)
			{
				m_DB.RollBack(pCon);	// ����
				SetCentralbillingState(m_sBillInfo[n].dwTokenID, EBST_Null, pCon);
				m_sBillInfo[n].Reset();
				m_DB.EndTransaction(pCon);
				m_DB.ReturnConnection(pCon);
				continue;
			}
		}

		m_DB.EndTransaction(pCon);
		m_DB.ReturnConnection(pCon);

		// ͳ����Ϣ
		if (P_VALID(m_pWorld))
		{
			switch (m_sBillInfo[n].eBillType)
			{
			case EBBT_Yuanbao:
				m_pWorld->CountBillYuanbaoSuccessTimes();
				break;

			case EBBT_Item:
				m_pWorld->CountBillItemSuccessTimes();
				break;
			}
		}

		// ���loongworld���ӣ���֪ͨworldʵʱȥȡ
		if( P_VALID(m_pWorld) && m_pWorld->IsValid() )
		{
			tagNBW_SafePickBaiBao send;
			send.eBillType = m_sBillInfo[n].eBillType;
			send.n64Serial = n64Serial;
			send.dwAccountID = dwAccountID;
			send.nCurYuanBao = nCurYuanBao;
			send.dwIncYuanbao = m_sBillInfo[n].nNum;
			g_WorldMgr.SendMsg(m_pWorld->GetID(), &send, send.dwSize);
		}

		m_sBillInfo[n].Reset();
	}
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID WorldDB::Reconnect()
{
	if( m_DB.Reconnect() )
	{
		GetCurMinItemSerialInDB(m_n64ItemSerial);
	}
	else
	{
		m_n64ItemSerial = GT_INVALID;
	}
}
//--------------------------------------------------------------------------------
// Ԫ������log
//--------------------------------------------------------------------------------
BOOL WorldDB::InsertBillYuanbaoLog(const LPCSTR szTokenID,DWORD dwAccountID, INT nNum )
{
	if( IsConLost() ) return FALSE;		// �Ͽ�����

	MyStream* pStream  = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	Connection* pCon = m_DB.GetFreeConnection();

	char szDate[X_DATATIME_LEN + 1];
	ZeroMemory(szDate, sizeof(szDate));
	DwordTime2DataTime(szDate, X_DATATIME_LEN + 1, GetCurrentDWORDTime());

	pStream->SetInsert("bill_yuanbao_log");
	pStream->FillString("token_id='").FillString(szTokenID, pCon);
	pStream->FillString("',account_id=") << dwAccountID;
	pStream->FillString(",yuanbao=") << nNum;
	pStream->FillString(",time='").FillString(szDate, pCon).FillString("'");

	m_DB.ReturnConnection(pCon);

	BOOL bRet = m_DB.Execute(pStream);

	m_DB.ReturnStream(pStream);

	return bRet;
}
//--------------------------------------------------------------------------------
// ��Ʒ����log
//--------------------------------------------------------------------------------
BOOL WorldDB::InsertBillItemLog(const LPCSTR szTokenID,DWORD dwAccountID, DWORD dwItemTypeID,INT nNum )
{
	if( IsConLost() ) return FALSE;		// �Ͽ�����

	MyStream* pStream  = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	Connection* pCon = m_DB.GetFreeConnection();

	//�õ���ǰʱ��
	char szDate[X_DATATIME_LEN + 1];
	ZeroMemory(szDate, sizeof(szDate));
	DwordTime2DataTime(szDate, X_DATATIME_LEN + 1, GetCurrentDWORDTime());

	pStream->SetInsert("bill_item_log");
	pStream->FillString("token_id='").FillString(szTokenID, pCon);
	pStream->FillString("',account_id=") << dwAccountID;
	pStream->FillString(",item_id=") << dwItemTypeID;
	pStream->FillString(",item_num=") << nNum;
	pStream->FillString(",time='").FillString(szDate, pCon).FillString("'");

	m_DB.ReturnConnection(pCon);

	BOOL bRet = m_DB.Execute(pStream);

	m_DB.ReturnStream(pStream);

	return bRet;
}

//---------------------------------------------------------------------------------------
//  ��ѯ���ڵ�Ԫ��
//---------------------------------------------------------------------------------------
DWORD WorldDB::GetCurAccountYuanbao(DWORD dwAccountID)
{
	if( IsConLost() ) return GT_INVALID;		// �Ͽ�����

	MyStream* pStream  = m_DB.GetStream();
	if( !P_VALID(pStream) ) return GT_INVALID;

	pStream->SetSelect("account_common","BaiBaoYuanBao");
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;

	QueryResult* pResult = m_DB.Query(pStream);

	// ��ѯ���Ϊ��
	if( !P_VALID(pResult) || pResult->GetRowCount() <= 0) return GT_INVALID;

	return (*pResult)[0].GetDword();
}

//---------------------------------------------------------------------------------------
// ��������Ԫ��
//---------------------------------------------------------------------------------------
BOOL WorldDB::GiveYuanbaoToAccount(DWORD dwAccountID, INT nNum, Connection* pCon)
{
	if( IsConLost() ) return FALSE;		// �Ͽ�����

	MyStream* pStream  = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->SetUpdate("account_common");
	pStream->FillString("BaiBaoYuanBao=BaibaoYuanBao+") << nNum;
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;

	BOOL bRet = m_DB.WaitExecute(pStream, pCon);

	m_DB.ReturnStream(pStream);

	return bRet;
}

//----------------------------------------------------------------------------------------
// ����Ұٱ��������Ʒ
//----------------------------------------------------------------------------------------
BOOL WorldDB::GiveItemToAccountBaibao(DWORD dwAccountID, DWORD dwItemTypeID, INT nNum, Connection* pCon, OUT INT64& n64SerialID)
{
	if( IsConLost() ) return FALSE;		// �Ͽ�����

	// ��ȡһ��Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// �õ�һ��64λID
	INT64 n64Serial = GenerateNew64ItemSerial();
	if( GT_INVALID == n64Serial ) return FALSE;

	n64SerialID = n64Serial;

	// ����sql���
	pStream->SetInsert("item_baibao");
	pStream->FillString("SerialNum=")			<<	n64Serial;
	pStream->FillString(",TypeID=")				<<	dwItemTypeID;
	pStream->FillString(",Num=")				<<	nNum;
	pStream->FillString(",AccountID=")			<<	dwAccountID;
	pStream->FillString(",OwnerID=")			<<	(DWORD)GT_INVALID;
	pStream->FillString(",CreateID=")			<<	(DWORD)GT_INVALID;
	pStream->FillString(",CreatorID=")			<<	(DWORD)GT_INVALID;
	pStream->FillString(",Bind=")				<<	EBS_Unknown;
	pStream->FillString(",CreateMode=")			<<	EICM_GMTool;
	pStream->FillString(",ContainerTypeID=")	<<	EICT_Baibao;
	pStream->FillString(",Suffix=")				<<	0;
	pStream->FillString(",CreateTime=now()");

	BOOL bRet = m_DB.WaitExecute(pStream, pCon);

	// ���û����ɹ�������
	if( !bRet )
	{
		m_DB.ReturnStream(pStream);
		return FALSE;
	}

	// �����װ��
	if( MIsEquipment(dwItemTypeID) )
	{
		pStream->Clear();	// ���

		// ����sql���
		pStream->SetInsert("equip_baibao");

		pStream->FillString(" SerialNum=")	<<	n64Serial;
		pStream->FillString(",Quality=")	<<	(BYTE)GT_INVALID;
		pStream->FillString(",WuHun=")		<<	0;
		pStream->FillString(",MinDmg=")		<<	0;
		pStream->FillString(",MaxDmg=")		<<	0;
		pStream->FillString(",Armor=")		<<	0;
		pStream->FillString(",PotVal=")		<<	0;

		// ���в���
		bRet = m_DB.WaitExecute(pStream, pCon);

		if( !bRet )
		{
			// ɾ��֮ǰ�������Ʒ����
			pStream->SetDelete("item_baibao");
			pStream->SetWhere();
			pStream->FillString("SerialNum=") << n64Serial;

			bRet = m_DB.WaitExecute(pStream, pCon);

			m_DB.ReturnStream(pStream);
			return FALSE;
		}
	}

	m_DB.ReturnStream(pStream);

	return TRUE;
}

//-------------------------------------------------------------------------------------
// ��account_common������˺�
//-------------------------------------------------------------------------------------
BOOL WorldDB::InsertAccout(DWORD dwAccountID, LPCSTR szAccountName)
{
	if( !P_VALID(szAccountName) ) return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// ��������
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetInsert("account_common");
	pStream->FillString("AccountID=")<<dwAccountID;
	pStream->FillString(",AccountName='").FillString(szAccountName, pCon).FillString("'");

	// �ͷ�����
	m_DB.ReturnConnection(pCon);

	// ��ѯ
	BOOL bRet = m_DB.Execute(pStream);

	// �ͷ���
	m_DB.ReturnStream(pStream);

	return bRet;
}
//-------------------------------------------------------------------------------------
// �õ���ҵ�ID
//-------------------------------------------------------------------------------------
DWORD WorldDB::GetAccountID(const LPCSTR szAccountName)
{
	if( IsConLost() ) return GT_INVALID;		// �Ͽ�����

	MyStream* pStream  = m_DB.GetStream();
	if( !P_VALID(pStream) ) return GT_INVALID;

	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetSelect("account_common", "AccountID");
	pStream->SetWhere();
	pStream->FillString("AccountName='").FillString(szAccountName, pCon).FillString("'");

	m_DB.ReturnConnection(pCon);

	QueryResult* pResult = m_DB.Query(pStream);

	m_DB.ReturnStream(pStream);

	if( !P_VALID(pResult) ) return GT_INVALID;
	if(  1 != pResult->GetRowCount() )
	{
		m_DB.FreeQueryResult(pResult);
		return GT_INVALID;
	}

	DWORD dwAccountID = (*pResult)[0].GetDword();

	m_DB.FreeQueryResult(pResult);

	return dwAccountID;
}

//---------------------------------------------------------------------------------------
// �õ�������Ʒ������С��64λID
//---------------------------------------------------------------------------------------
BOOL WorldDB::GetCurMinItemSerialInDB(INT64& n64ItemSerial)
{
	n64ItemSerial = MAX_ITEM_SERIAL_BILL;

	INT64 nMinItem			=	MAX_ITEM_SERIAL_BILL;
	INT64 nMinItemDel		=	MAX_ITEM_SERIAL_BILL;
	INT64 nMinItemBaibao	=	MAX_ITEM_SERIAL_BILL;

	BOOL bRet = GetMinItemSerialInTable("item", nMinItem) &&
				GetMinItemSerialInTable("item_del", nMinItemDel) &&
				GetMinItemSerialInTable("item_baibao", nMinItemBaibao);

	if( !bRet )
	{
		n64ItemSerial = GT_INVALID;
		return FALSE;
	}

	INT64 nMin = min(nMinItem, nMinItemDel);
	nMin = min(nMin, nMinItemBaibao);

	n64ItemSerial = min(nMin, n64ItemSerial);

	return TRUE;
}

//---------------------------------------------------------------------------------------
// �õ�ĳ��������С��64λID
//---------------------------------------------------------------------------------------
BOOL WorldDB::GetMinItemSerialInTable(LPCSTR szTable, INT64& n64ItemSerial)
{
	if( IsConLost() ) return FALSE;		// �Ͽ�����

	MyStream* pStream  = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->SetSelect(szTable, "min(SerialNum)");
	pStream->SetWhere();
	pStream->FillString("SerialNum>") << 0;

	QueryResult* pResult = m_DB.Query(pStream);

	m_DB.ReturnStream(pStream);

	if( !P_VALID(pResult) ) return FALSE;

	if( 0 == pResult->GetRowCount() )
	{
		m_DB.FreeQueryResult(pResult);
		return TRUE;
	}

	if( (*pResult)[0].IsNull() )
	{
		m_DB.FreeQueryResult(pResult);
		return TRUE;
	}

	n64ItemSerial = (*pResult)[0].GetLong();

	m_DB.FreeQueryResult(pResult);

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ���� -- ���������Ϊ"t1,t2,t3"
//-------------------------------------------------------------------------------------------------------
BOOL WorldDB::LockTables(LPCSTR szTableNames)
{
	if( IsConLost() ) return FALSE;		// �Ͽ�����

	MyStream* pStream  = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->Clear();

	pStream->FillString("Lock Tables ");
	pStream->FillString(szTableNames);
	pStream->FillString(" WRITE");

	// ��ѯ
	BOOL bRet = m_DB.Execute(pStream);

	// �ͷ���
	m_DB.ReturnStream(pStream);

	return bRet;
}
//-------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------
BOOL WorldDB::UnlockTables()
{
	if( IsConLost() ) return FALSE;		// �Ͽ�����

	MyStream* pStream  = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->Clear();

	pStream->FillString("Unlock Tables");

	// ��ѯ
	BOOL bRet = m_DB.Execute(pStream);

	// �ͷ���
	m_DB.ReturnStream(pStream);

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL WorldDB::GetCentralbillingInfo( INT& nNum )
{
	if( IsConLost() || !P_VALID(m_pWorld)) return FALSE;		// �Ͽ�����

	MyStream* pStream  = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->SetSelect("centralbilling", "TOKENID, USERNAME, YUANORITEM, ITEMCODE, QTY");
	pStream->SetWhere();
	pStream->FillString("WORLDID=") << m_pWorld->GetID();
	pStream->FillString(" and STATE=") << EBST_Undeal;
	pStream->SetLimit(BILLINFO_PERTICK);

	QueryResult* pResult = m_DB.Query(pStream);

	m_DB.ReturnStream(pStream);

	if( !P_VALID(pResult) ) return FALSE;

	nNum = min(pResult->GetRowCount(), BILLINFO_PERTICK);
	for (int n=0; n<nNum; n++)
	{
		m_sBillInfo[n].dwTokenID	= (*pResult)[0].GetDword();
		m_sBillInfo[n].dwTypeID		= (*pResult)[3].GetDword();
		m_sBillInfo[n].eBillType	= (EBillType)(*pResult)[2].GetInt();
		m_sBillInfo[n].nNum			= (*pResult)[4].GetInt();
		memcpy(m_sBillInfo[n].szAccountName, (*pResult)[1].GetString(), (*pResult)[1].GetLen());

		pResult->NextRow();
	}

	m_DB.FreeQueryResult(pResult);

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ���û�����Ϣ״̬
//-------------------------------------------------------------------------------------------------------
BOOL WorldDB::SetCentralbillingState( DWORD dwTokenID, INT nState, Connection* pCon )
{
	if( IsConLost() ) return FALSE;		// �Ͽ�����

	MyStream* pStream  = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->SetUpdate("centralbilling");
	pStream->FillString("STATE=") << nState;
	pStream->SetWhere();
	pStream->FillString("TOKENID=") << dwTokenID;

	BOOL bRet = m_DB.WaitExecute(pStream, pCon);

	m_DB.ReturnStream(pStream);

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ���Ѿ�����Ļ���ת�Ƶ�log��
//-------------------------------------------------------------------------------------------------------
BOOL WorldDB::MoveBillInfo2Log( DWORD dwTokenID, Connection* pCon )
{
	if( IsConLost() ) return FALSE;		// �Ͽ�����

	MyStream* pStream  = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// ����������ļ�¼��log��
	pStream->SetCopyItem("centralbilling_log", "*", "centralbilling");
	pStream->SetWhere();
	pStream->FillString("TOKENID=") << dwTokenID;

	BOOL bRet = m_DB.WaitExecute(pStream, pCon);

	if (!bRet)
	{
		m_DB.ReturnStream(pStream);
		return FALSE;
	}

	// ɾ���м�������
	pStream->SetDelete("centralbilling");
	pStream->SetWhere();
	pStream->FillString("TOKENID=") << dwTokenID;

	bRet = m_DB.WaitExecute(pStream, pCon);

	m_DB.ReturnStream(pStream);

	return bRet;
}