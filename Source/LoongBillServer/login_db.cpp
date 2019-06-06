//-----------------------------------------------------------------------------
//!\file login_db.h
//!\author songg
//!
//!\date 2009-08-31
//! last 2009-08-31
//!
//!\brief login���������ݿ�
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "login_db.h"
#include "../ServerDefine/bill_define.h"
//------------------------------------------------------------------------------
// ȫ�ֱ�������
//------------------------------------------------------------------------------
LoginDB g_LoginDB;
//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL LoginDB::Init()
{
	TObjRef<VarContainer> pVar = "BillConfig";

	// ��ʼ�����ݿ�
	BOOL bRet = m_DB.Init(pVar->GetString(_T("db_host"), _T("login")), 
		pVar->GetString(_T("db_user"), _T("login")),
		pVar->GetString(_T("db_psd"), _T("login")), 
		pVar->GetString(_T("db_name"), _T("login")),
		pVar->GetInt(_T("db_port"), _T("login")));


	return bRet;
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID LoginDB::Destroy()
{

}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID LoginDB::Update()
{
	// ����
	if( IsConLost() )
		Reconnect();
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID LoginDB::Reconnect()
{
	m_DB.Reconnect();
}
//-------------------------------------------------------------------------
// ������Ҽ�¼
//-------------------------------------------------------------------------
BOOL LoginDB::InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
{
	if( !P_VALID(szAccountName) || !P_VALID(szPsd) ) return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// ��������
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetInsert("account");
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("',");
	pStream->FillString("psd='").FillString(szPsd, pCon).FillString("',");
	pStream->FillString("mibao='").FillBlob(szMiBao, MIBAO_LEN, pCon).FillString("',");
	pStream->FillString("guard=") << bGuard;

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
DWORD LoginDB::GetAccountID(const LPCSTR szAccountName)
{
	if( IsConLost() ) return GT_INVALID;		// �Ͽ�����

	MyStream* pStream  = m_DB.GetStream();
	if( !P_VALID(pStream) ) return GT_INVALID;

	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetSelect("account", "id");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");

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
//-------------------------------------------------------------------------------------------------------
// ���� -- ���������Ϊ"t1,t2,t3"
//-------------------------------------------------------------------------------------------------------
BOOL LoginDB::LockTables(LPCSTR szTableNames)
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
BOOL LoginDB::UnlockTables()
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