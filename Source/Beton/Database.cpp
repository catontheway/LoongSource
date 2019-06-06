//--------------------------------------------------------------------------------------------------
// File: Database.cpp
// Desc: Database manipulation class
// Time: 2008-05-04
// Auth: Aslan
//
// Copyright (c) 2008 Tengreng All rights reserved.
//---------------------------------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "Config.h"
#include "Database.h"
#include "MysqlStream.h"
#include "SafeStreamQueue.h"
#include "StreamPool.h"
#include "QueryResult.h"

namespace Beton
{

//----------------------------------------------------------------------------------------------------------------------------
// connection
//----------------------------------------------------------------------------------------------------------------------------
Connection::Connection()
{
	m_Mysql = NULL;
}

//----------------------------------------------------------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------------------------------------------------------
DataBase::DataBase() : m_Trunk(this), m_nPort(GT_INVALID), m_fnWarning(NULL), m_nConNum(0), m_Con(NULL), m_bConLost(TRUE),
					   m_hThreadAsynQuery(NULL), m_hThreadCheck(NULL), m_bTerminateAsynQuery(FALSE), m_bTerminateCheck(FALSE), m_pLog(NULL)
{
}

//----------------------------------------------------------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------------------------------------------------------
DataBase::~DataBase()
{
	ShutDown();
}

//----------------------------------------------------------------------------------------------------------------------------
// �������ݲ���
//----------------------------------------------------------------------------------------------------------------------------
VOID DataBase::SetParam(const char* szHost, const char* szUser, const char* szPassword, const char* szDatabase, INT nPort, INT nConNum)
{
	m_Hostname = std::string(szHost);
	m_Username = std::string(szUser);
	m_Password = std::string(szPassword);
	m_DatabaseName = std::string(szDatabase);
	m_nPort = nPort;
	m_nConNum = nConNum;
}

//----------------------------------------------------------------------------------------------------------------------------
// �������ݲ���
//----------------------------------------------------------------------------------------------------------------------------
VOID DataBase::SetParam(const WCHAR* szHost, const WCHAR* szUser, const WCHAR* szPassword, const WCHAR* szDatabase, INT nPort, INT nConNum)
{
	m_Hostname = std::string(m_pUtil->UnicodeToUnicode8(szHost));
	m_Username = std::string(m_pUtil->UnicodeToUnicode8(szUser));
	m_Password = std::string(m_pUtil->UnicodeToUnicode8(szPassword));
	m_DatabaseName = std::string(m_pUtil->UnicodeToUnicode8(szDatabase));
	m_nPort = nPort;
	m_nConNum = nConNum;
}

//----------------------------------------------------------------------------------------------------------------------------
// ��ʼ������
//----------------------------------------------------------------------------------------------------------------------------
BOOL DataBase::Init(LPCTSTR strHost, LPCTSTR strUser, LPCTSTR strPassword, LPCTSTR strDatabase, INT nPort, INT nConNum/*=2*/)
{
	if( nConNum <= 1 ) return FALSE;

	m_pUtil = "Util";
	
	SetParam(strHost, strUser, strPassword, strDatabase, nPort, nConNum);

	return Start();
}

//---------------------------------------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------------------------------------
BOOL DataBase::Reconnect()
{
	ShutDown();		// ��ȫ��ر�

	return Start();	// ����
}

//---------------------------------------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------------------------------------
BOOL DataBase::Start()
{
	// ��������
	m_Con = new Connection[m_nConNum];

	for(INT n = 0; n < m_nConNum; n++)
	{
		MYSQL* temp = mysql_init(NULL);
		if( mysql_options(temp, MYSQL_SET_CHARSET_NAME, "utf8") )
		{
			// ����log
		}

		my_bool my_true = true;
		if( mysql_options(temp, MYSQL_OPT_RECONNECT, &my_true) )
		{
			// ����log
		}

		if( NULL == mysql_real_connect(temp, m_Hostname.c_str(), m_Username.c_str(), m_Password.c_str(), m_DatabaseName.c_str(), m_nPort, NULL, 0) )
		{
			// д����־
			return false;
		}

		m_Con[n].m_Mysql = temp;
	}

	// ����DB����������log��������
	m_pLog = new Log;

	if( !P_VALID(m_pLog) )
	{
		return FALSE;
	}
	m_LogFileName = CreateLogFileName();
	if( FALSE == m_pLog->Create(m_LogFileName.c_str()) )
	{
		return FALSE;
	}

	// �����첽�����߳�
	m_bTerminateAsynQuery = FALSE;
	INT nRet = _beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1(&DataBase::ThreadAsynQuery), NULL, 0, NULL);
	if( 0 == nRet || -1 == nRet )
	{
		return FALSE;
	}
	m_hThreadAsynQuery = (HANDLE)nRet;

	// ���ɼ���߳�
	m_bTerminateCheck = FALSE;
	nRet = _beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1(&DataBase::ThreadCheck), NULL, 0, NULL);
	if( 0 == nRet || -1 == nRet )
	{
		return FALSE;
	}
	m_hThreadCheck = (HANDLE)nRet;

	// �������ӳɹ�
	m_bConLost = FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------------------------------------------
// �ر�
//---------------------------------------------------------------------------------------------------------------
VOID DataBase::ShutDown()
{
	// ���ȵȴ��첽�����߳̽��������̻߳���������Ŀǰ�����е�sql��䣬����stream�黹����,����������ʱ����ͷ���Դ��
	InterlockedExchange((LPLONG)&m_bTerminateAsynQuery, TRUE);
	InterlockedExchange((LPLONG)&m_bTerminateCheck, TRUE);

	WaitForSingleObject(m_hThreadAsynQuery, INFINITE);
	WaitForSingleObject(m_hThreadCheck, INFINITE);

	SAFE_CLOSE_HANDLE(m_hThreadAsynQuery);
	SAFE_CLOSE_HANDLE(m_hThreadCheck);

	// �ر���������
	if( P_VALID(m_Con) )
	{
		for(INT n = 0; n < m_nConNum; n++)
		{
			if( m_Con[n].m_Mysql )
				mysql_close(m_Con[n].m_Mysql);
		}
		SAFE_DEL_ARRAY(m_Con);
	}

	// �ر�log
	if( P_VALID(m_pLog) )
	{
		m_pLog->Close();
		SAFE_DEL(m_pLog);
	}
}

//-----------------------------------------------------------------------------------------------------------------
// �õ�һ�����е����ӣ������ⲿ�ֶ���õ����ӣ��ڲ�����󣬱������ReturnConnection�ͷ�����
//-----------------------------------------------------------------------------------------------------------------
Connection* DataBase::GetFreeConnection()
{
	DWORD n = 0;
	while(TRUE)
	{
		Connection* con = &m_Con[((n++) % m_nConNum)];
		if( con->m_mutex.TryAcquire() )
			return con;
	}

	// �����ܵ�����
	return NULL;
}

//------------------------------------------------------------------------------------------------------------------
// ʹ������ͷ�һ������
//------------------------------------------------------------------------------------------------------------------
VOID DataBase::ReturnConnection(Connection* con)
{
	if( NULL != con )
		con->m_mutex.Release();
}

//-------------------------------------------------------------------------------------------------------------------
// ��ָ�����ӵ�����ʱ��ѯ����
//-------------------------------------------------------------------------------------------------------------------
QueryResult* DataBase::Query(const char* szStr)
{
	if( NULL == szStr ) return NULL;

	// ������һ������
	Connection* con = GetFreeConnection();

	QueryResult* pRet = NULL;

	if( SendQuery(con, szStr, strlen(szStr)) )
	{
		pRet = StoreQueryResult(con);
	}

	ReturnConnection(con);
	return pRet;
}

QueryResult* DataBase::Query(const MyStream* pStream)
{
	if( NULL == pStream || NULL == pStream->GetBuf() || 0 == pStream->GetBufLen() )
		return NULL;

	// ������һ������
	Connection* con = GetFreeConnection();

	QueryResult* pRet = NULL;

	if( SendQuery(con, pStream->GetBuf(), pStream->GetBufLen()) )
	{
		pRet = StoreQueryResult(con);
	}

	ReturnConnection(con);
	return pRet;
}

//------------------------------------------------------------------------------------------------------------------------
// ָ�����ӵ�����ʽ��ѯ����
//------------------------------------------------------------------------------------------------------------------------
QueryResult* DataBase::WaitQuery(const char* szStr, Connection* con)
{
	if( NULL == szStr || NULL == con ) return NULL;

	QueryResult* pRet = NULL;

	if( SendQuery(con, szStr, strlen(szStr)) )
	{
		pRet = StoreQueryResult(con);
	}

	return pRet;
}

QueryResult* DataBase::WaitQuery(const MyStream* pStream, Connection* con)
{
	if( NULL == pStream || NULL == pStream->GetBuf() || 0 == pStream->GetBufLen() || NULL == con )
		return NULL;

	QueryResult* pRet = NULL;

	if( SendQuery(con, pStream->GetBuf(), pStream->GetBufLen()) )
	{
		pRet = StoreQueryResult(con);
	}

	return pRet;
}

//-----------------------------------------------------------------------------------------------------------
// ��ָ�����ӵ�����ʽ���ݿ�ִ�в���
//-----------------------------------------------------------------------------------------------------------
BOOL DataBase::Execute(const char* szStr)
{
	if( NULL == szStr ) return FALSE;

	// ������һ������
	Connection* con = GetFreeConnection();

	BOOL bRet = SendQuery(con, szStr, strlen(szStr));

	ReturnConnection(con);
	return bRet;
}

BOOL DataBase::Execute(const MyStream* pStream)
{
	if( NULL == pStream || NULL == pStream->GetBuf() || 0 == pStream->GetBufLen() )
		return FALSE;

	// ������һ������
	Connection* con = GetFreeConnection();

	BOOL bRet = SendQuery(con, pStream->GetBuf(), pStream->GetBufLen());

	ReturnConnection(con);
	return bRet;
}

//-------------------------------------------------------------------------------------------------------------
// ָ�����ӵ�����ʽ���ݿ�ִ�в���
//--------------------------------------------------------------------------------------------------------------
BOOL DataBase::WaitExecute(const char* szStr, Connection* con)
{
	if( NULL == szStr || NULL == con ) return FALSE;

	return SendQuery(con, szStr, strlen(szStr));

}

BOOL DataBase::WaitExecute(const MyStream* pStream, Connection* con)
{
	if( NULL == pStream || NULL == pStream->GetBuf() || 0 == pStream->GetBufLen() || NULL == con )
		return FALSE;

	return SendQuery(con, pStream->GetBuf(), pStream->GetBufLen());
}

//---------------------------------------------------------------------------------------------------------------
// ��ָ�����ӵ�����ʽ���ݿ���ִ�в���������ɹ�������Ӱ��ļ�¼���������򷵻�-1
//---------------------------------------------------------------------------------------------------------------
INT DataBase::CheckExecute(const char* szStr)
{
	if( NULL == szStr ) return GT_INVALID;

	// ������һ������
	Connection* con = GetFreeConnection();

	BOOL bRet = SendQuery(con, szStr, strlen(szStr));

	ReturnConnection(con);

	return (bRet ? (INT)mysql_affected_rows(con->m_Mysql) : GT_INVALID);
}

INT DataBase::CheckExecute(const MyStream* pStream)
{
	if( NULL == pStream || NULL == pStream->GetBuf() || 0 == pStream->GetBufLen() )
		return GT_INVALID;

	// ������һ������
	Connection* con = GetFreeConnection();

	BOOL bRet = SendQuery(con, pStream->GetBuf(), pStream->GetBufLen());

	ReturnConnection(con);

	return (bRet ? (INT)mysql_affected_rows(con->m_Mysql) : GT_INVALID);
}

//---------------------------------------------------------------------------------------------------------------
// ָ�����ӵ�����ʽ���ݿ���ִ�в���������ɹ�������Ӱ��ļ�¼���������򷵻�-1
//---------------------------------------------------------------------------------------------------------------
INT DataBase::CheckWaitExecute(const char* szStr, Connection* con)
{
	if( NULL == szStr || NULL == con ) return GT_INVALID;

	BOOL bRet = SendQuery(con, szStr, strlen(szStr));

	return (bRet ? (INT)mysql_affected_rows(con->m_Mysql) : GT_INVALID);
}

INT DataBase::CheckWaitExecute(const Beton::MyStream *pStream, Beton::Connection* con)
{
	if( NULL == pStream || NULL == pStream->GetBuf() || 0 == pStream->GetBufLen() || NULL == con )
		return GT_INVALID;

	BOOL bRet = SendQuery(con, pStream->GetBuf(), pStream->GetBufLen());

	return (bRet ? (INT)mysql_affected_rows(con->m_Mysql) : GT_INVALID);
}

//---------------------------------------------------------------------------------------------------------------
// ������ز���
//---------------------------------------------------------------------------------------------------------------
BOOL DataBase::BeginTransaction(Connection* con)
{
	return WaitExecute("START TRANSACTION", con);
}

BOOL DataBase::EndTransaction(Connection* con)
{
	return WaitExecute("COMMIT", con);
}

BOOL DataBase::RollBack(Connection* con)
{
	return WaitExecute("ROLLBACK", con);
}

//------------------------------------------------------------------------------------------------------------------
// ��ʽ���Ͳ�ѯ���
//------------------------------------------------------------------------------------------------------------------
BOOL DataBase::SendQuery(Connection* con, const char* szSql, INT nLen, BOOL bSelf/* =FALSE */)
{
	INT nResult = mysql_real_query(con->m_Mysql, szSql, nLen);
	if( nResult != 0 )
	{
		if( bSelf == FALSE && HandleError(con, mysql_errno(con->m_Mysql), szSql) )
		{
			// �����Ϊ���Ӷ�ʧ
			return SendQuery(con, szSql, nLen, TRUE);
		}
		else
		{
			// ���������Ӷ�ʧ�ˣ�������Ҫ�����ϲ�Ļص�������֪ͨ�ϲ�
		}
	}

	return (nResult == 0 ? TRUE : FALSE);
}

//-------------------------------------------------------------------------------------------------------------------
// �õ���ѯ�����ֻ������ݿ�Ķ������������ݿ��д�������ú���ʼ�շ���NULL
//-------------------------------------------------------------------------------------------------------------------
QueryResult* DataBase::StoreQueryResult(Connection* con)
{
	if( NULL == con ) return NULL;

	QueryResult* pResult = NULL;

	MYSQL_RES* pRes = mysql_store_result(con->m_Mysql);
	INT nRows = (INT)mysql_affected_rows(con->m_Mysql);
	INT nFields = (INT)mysql_field_count(con->m_Mysql);

	if( 0 == nFields || NULL == pRes )
	{
		if( pRes != NULL )
			mysql_free_result(pRes);

		return NULL;
	}

	pResult = new QueryResult(pRes, nRows, nFields);

	pResult->NextRow();

	return pResult;
}

//----------------------------------------------------------------------------------------------------------------------
// ����sql������
//----------------------------------------------------------------------------------------------------------------------
BOOL DataBase::HandleError(Connection* con, DWORD dwErrorCode, const char* szSql)
{
	if( NULL == con ) return FALSE;

	// д����־
	m_pLog->Write(_T("Error: %u\r\n"), dwErrorCode);
	m_pLog->Write(_T("Reason: %s\r\n"), m_pUtil->Unicode8ToUnicode(mysql_error(con->m_Mysql)));
	m_pLog->Write(_T("Sql: %s\r\n\r\n"), m_pUtil->AnsiToUnicode(szSql));

	switch(dwErrorCode)
	{
	case 2006:  // Mysql server has gone away
	case 2008:  // Client ran out of memory
	case 2013:  // Lost connection to sql server during query
	case 2055:  // Lost connection to sql server - system error
		{
			// ����������Ϸ����룬�����ӿ��ܶ�ʧ����ʱ��������
			return Reconnect(con);
		}
		break;
	}

	return FALSE;
}

//----------------------------------------------------------------------------------------------------------------------
// ��������
//----------------------------------------------------------------------------------------------------------------------
BOOL DataBase::Reconnect(Connection* con)
{
	if( NULL == con ) return FALSE;

	MYSQL* temp = mysql_init(NULL);
	my_bool my_true = true;
	mysql_options(temp, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_options(temp, MYSQL_OPT_RECONNECT, &my_true);

	if( mysql_real_connect(temp, m_Hostname.c_str(), m_Username.c_str(), m_Password.c_str(), m_DatabaseName.c_str(), m_nPort, NULL , 0) )
	{
		mysql_close(temp);

		// �������ӶϿ�
		m_bConLost = TRUE;

		// ������Ҫ���ϲ㱨�棬�����ϲ�Ļص�����
		if( m_fnWarning )
			(*m_fnWarning)(this, EDBE_System, 0);
		return FALSE;
	}

	if( con->m_Mysql != NULL )
		mysql_close(con->m_Mysql);

	con->m_Mysql = temp;
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
// ���������ݿ��������
//----------------------------------------------------------------------------------------------------------------------
UINT DataBase::ThreadAsynQuery()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	// Ϊ���߳��ṩһ���̶�������
	Connection* con = GetFreeConnection();
	if( NULL == con ) return 1;

	while( FALSE == m_bTerminateAsynQuery )
	{
		while(TRUE)
		{
			MyStream* pStream = m_AsynStreamQueue.Get();
			if( NULL == pStream )
				break;

			WaitExecute(pStream, con);
			ReturnStream(pStream);
		}

		WaitForSingleObject(m_AsynStreamQueue.GetEvent(), 120);
	}

	// �ͷŵ����ܻ�ʣ�������
	while(TRUE)
	{
		MyStream* pStream = m_AsynStreamQueue.Get();
		if( NULL == pStream )
			break;

		WaitExecute(pStream, con);
		ReturnStream(pStream);
	}

	// ��������
	ReturnConnection(con);

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	_endthreadex(0);
	return 0;
}

//---------------------------------------------------------------------------------------------------------------------
// ����̣߳����ڼ�⻺���С����Դ�ش�С�ȵȣ��������һ��ֵ���ͱ����ϲ�
//---------------------------------------------------------------------------------------------------------------------
UINT DataBase::ThreadCheck()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	BOOL bWarningQueueFullNotified = FALSE;
	BOOL bWarningPoolFullNotified = FALSE;

	while( FALSE == m_bTerminateCheck )
	{
		// ���ȼ���첽�������������
		INT nQueryNum = m_AsynStreamQueue.Size();
		if( nQueryNum >= WARNING_QUEUE_NUM && FALSE == bWarningQueueFullNotified )
		{
			// �����ϲ�ص�����
			if( m_fnWarning )
				(*m_fnWarning)(this, EDBE_QueueFull, nQueryNum);
			bWarningQueueFullNotified = TRUE;
		}
		else if( nQueryNum < WARNING_QUEUE_NUM && TRUE == bWarningQueueFullNotified )
		{
			bWarningQueueFullNotified = FALSE;
		}

		// �ټ��ײ���Դ�ش�С
		INT nPoolNum = m_StreamPool.GetAllStreamNum();
		if( nPoolNum >= WARNING_POOL_NUM && FALSE == bWarningPoolFullNotified )
		{
			// �����ϲ�ص�����
			if( m_fnWarning )
				(*m_fnWarning)(this, EDBE_PoolFull, nPoolNum);
			bWarningPoolFullNotified = TRUE;
		}
		else if( nPoolNum < WARNING_POOL_NUM && TRUE == bWarningPoolFullNotified )
		{
			bWarningPoolFullNotified = FALSE;
		}

		Sleep(2000);
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	_endthreadex(0);
	return 0;
}

//------------------------------------------------------------------------------
// �������ݿ���־�ļ���
//------------------------------------------------------------------------------
tstring DataBase::CreateLogFileName()
{
	tstring strFileName = _T("log\\");

	// ���ɺ��ʵ�log�ļ���
	TCHAR szTime[MAX_PATH], szTemp[MAX_PATH];
	GetModuleFileName(NULL, szTemp, MAX_PATH);

	// ��ȥ·��
	TCHAR *pResult = _tcsrchr(szTemp, _T('\\'));
	pResult = pResult ?	pResult+1 :	pResult = (TCHAR *)szTemp;

	// ��ȥ�ļ���չ��
	TCHAR* pResult2 = _tcsrchr(pResult, _T('.'));
	if( pResult2 )
		*pResult2 = _T('\0');

	// ���ϳ����ļ���
	strFileName += pResult;
	strFileName += _T("_");

	// �������ݿ�����
	strFileName += m_pUtil->Unicode8ToUnicode(m_DatabaseName.c_str());
	strFileName += _T("_");

	// ����ʱ��
	FILETIME CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	szTime[0] = _T('\0');
	WORD wDate, wTime;
	if (FileTimeToLocalFileTime(&CurrentTime, &CurrentTime) &&
		FileTimeToDosDateTime(&CurrentTime, &wDate, &wTime))
	{
		// �������Լ�ʱ����ͽ���id
		wsprintf(szTime, _T("[%d-%d-%d %02d%02d%02d %05d_%d].dblog"),
			(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
			(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2, 
			GetCurrentProcessId(), rand());
	}

	strFileName += szTime;

	return strFileName;
}

} // namespace Beton

