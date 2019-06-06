//------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: loongdb_common.cpp
// author: Sxg
// actor:
// data: 2008-05-12
// last:
// brief: ���ݿ����Ӧ�ò�ʵ�� -- �����������
//------------------------------------------------------------------------------
#include "StdAfx.h"

#include "LoongDB.h"
#include "../WorldDefine/currency_define.h"
#include "../ServerDefine/role_data_define.h"

/************************* ��ȡ *****************************/

//------------------------------------------------------------------------------
// ��ȡ�˺Ź�����Ϣ
//------------------------------------------------------------------------------
BOOL CLoongDB::LoadAccountCommon(LPVOID pData, DWORD dwAccountID)
{
	return Load(pData, NULL, dwAccountID, GT_INVALID, 
				&CLoongDB::FormatLoadAccountCommon, 
				&CLoongDB::ProcResLoadAccountCommon);
}

VOID CLoongDB::FormatLoadAccountCommon(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("account_common", 
		"SafeCodeCrc,ResetTime,BagPsdCrc,BaiBaoYuanBao,WareSize,WareSilver");
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

VOID CLoongDB::ProcResLoadAccountCommon(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(p, pData, tagAccountCommon);

	p->sSafeCode.dwSafeCodeCrc	= (*pRes)[0].GetDword();
	p->sSafeCode.dwTimeReset	= (*pRes)[1].GetDword();
	p->dwBagPsdCrc				= (*pRes)[2].GetDword();
	p->nBaiBaoYuanBao			= (*pRes)[3].GetInt();
	p->n16WareSize				= (*pRes)[4].GetInt();
	p->n64WareSilver			= (*pRes)[5].GetLong();
}

//------------------------------------------------------------------------------
// ��סaccount_common
//------------------------------------------------------------------------------
BOOL CLoongDB::LockAccountCommon()
{
	return LockTables("account_common");
}

/************************* ���� *****************************/

//------------------------------------------------------------------------------
// �����ʺż�¼
//------------------------------------------------------------------------------
BOOL CLoongDB::InsertAccountCommon(DWORD dwAccountID,const CHAR* szAccount)
{
	return Update(dwAccountID, (PVOID)szAccount, 1, 0, (&CLoongDB::FormatInsertAccountCommon));
}

VOID CLoongDB::FormatInsertAccountCommon(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	const CHAR* szAccount = (CHAR*)pData;

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetInsert("account_common");
	pStream->FillString("AccountID=") << dwAccountID;
	pStream->FillString(",AccountName='").FillString(szAccount, pCon).FillString("'");

	m_pDBLoong->ReturnConnection(pCon);
}

//------------------------------------------------------------------------------
// ���°�ȫ��
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateSafeCode(DWORD dwAccountID, DWORD dwSafeCodeCrc)
{
	return Update(dwAccountID, &dwSafeCodeCrc, 1, sizeof(DWORD), (&CLoongDB::FormatUpdateSafeCode));
}

VOID CLoongDB::FormatUpdateSafeCode(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	pStream->SetUpdate("account_common");
	pStream->FillString("SafeCodeCrc=") << *(DWORD*)pData;
	pStream->FillString(",ResetTime=") << (DWORD)GT_INVALID;

	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

//------------------------------------------------------------------------------
// ���°�ȫ������ʱ��
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateSafeCodeResetTime(DWORD dwAccountID, DWORD dwTimeReset)
{
	return Update(dwAccountID, &dwTimeReset, 1, sizeof(DWORD), (&CLoongDB::FormatUpdateSafeCodeResetTime));
}

VOID CLoongDB::FormatUpdateSafeCodeResetTime(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	pStream->SetUpdate("account_common");
	pStream->FillString("ResetTime=") << *(DWORD*)pData;

	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

//------------------------------------------------------------------------------
// �����ʺ��±�������
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateBagPsd(DWORD dwAccountID, DWORD dwNewBagPsd)
{
	return Update(dwAccountID, &dwNewBagPsd, 1, 0, (&CLoongDB::FormatUpdateBagPsd));
}

VOID CLoongDB::FormatUpdateBagPsd(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	pStream->SetUpdate("account_common");
	pStream->FillString("BagPsdCrc=") << *(DWORD*)pData;

	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

//------------------------------------------------------------------------------
// ���½�ɫ�ֿ��С
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateWareSize(DWORD dwAccountID, LPVOID pData)
{
	return Update(dwAccountID, pData, 1, 0, (&CLoongDB::FormatUpdateWareSize));
}

VOID CLoongDB::FormatUpdateWareSize(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	pStream->SetUpdate("account_common");
	pStream->FillString("WareSize=") << *(INT16*)pData;

	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

//------------------------------------------------------------------------------
// ���½�ɫ�ֿ��Ǯ
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateWareMoney(DWORD dwAccountID, INT64 n64WareSilver)
{
	return Update(dwAccountID, (LPVOID)&n64WareSilver, 1, 0, (&CLoongDB::FormatUpdateWareMoney));
}

VOID CLoongDB::FormatUpdateWareMoney(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	pStream->SetUpdate("account_common");
	pStream->FillString("WareSilver=WareSilver+") << *(INT64*)pData;
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

//------------------------------------------------------------------------------
// ���°ٱ���Ԫ��
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateBaiBaoYuanBao(DWORD dwAccountID, LPVOID pData)
{
	return Update(dwAccountID, pData, 1, 0, (&CLoongDB::FormatUpdateBaiBaoYuanBao));
}

VOID CLoongDB::FormatUpdateBaiBaoYuanBao(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	pStream->SetUpdate("account_common");
	pStream->FillString("BaiBaoYuanBao=BaiBaoYuanBao+") << *(INT*)pData;

	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}