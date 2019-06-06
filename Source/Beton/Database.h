//--------------------------------------------------------------------------------------------------
// File: Database.h
// Desc: Database structure
// Time: 2008-05-05
// Auth: Aslan
//
// Copyright (c) 2008 Tengreng All rights reserved.
//---------------------------------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "Config.h"
#include "Mutex.h"
#include "MysqlStream.h"
#include "SafeStreamQueue.h"
#include "StreamPool.h"
#include "QueryResult.h"

namespace Beton {

class Mutex;
class QueryResult;
class MyStream;
class SafeStreamQueue;
class StreamPool;

class DataBase;

typedef VOID (WINAPI* WARNINGCALLBACK)(DataBase*, INT nReason, INT nParam);

//----------------------------------------------------------------------------------------------------
/**	Database�� - �ṩ���ݿ�����࣬�������ӵĽ�����ά�������ֲ�ѯ�����²������Ƶȵ�
1. Database���������ݿ�һһ��Ӧ������ϲ�Ӧ�ó�����Ҫ����⣬Ҫ�������Database����
1. ���ӳػ��ƣ�������̵߳Ĳ�������
2. �ṩ�����ݿ����Ӳ����Լ���ǰ���ݿ�״̬��Ϣ����ȡ
3. �ṩ���ֲ�ѯ��������ʽ��������ʽ�������ص�ʽ�������޷��ز���
4. �ṩ�򵥵���������ƣ��պ��������������֧��
*/
//----------------------------------------------------------------------------------------------------
const INT WARNING_QUEUE_NUM = 1000;		// �첽�������е��������������ֵ���򾯱�
const INT WARNING_POOL_NUM = 20000;		// �ײ�Mystream�ص�������������޶ȣ��򾯱�

struct BETON_API Connection
{
	Mutex		m_mutex;	// ������
	MYSQL*		m_Mysql;

	Connection();
};

class BETON_API DataBase
{
public:
	typedef enum			// ���ݿ⾯��ö����Ϣ��Ŀǰ������
	{
		EDBE_System,		// mysql����������
		EDBE_QueueFull,		// ���������ݹ��࣬�鿴���ݿ��Ƿ�����
		EDBE_PoolFull,		// �ײ�Stream�ط�����ڴ������
	} EDBError;

public:
	DataBase();
	virtual ~DataBase();
	
private:
	DataBase(const DataBase&);
	DataBase& operator = (const DataBase&);

public:
	VOID SetWarningCallBack(WARNINGCALLBACK fnWarning) { m_fnWarning = fnWarning; }

public:

	//---------------------------------------------------------------------------
	// ��ʼ��������
	//---------------------------------------------------------------------------
	BOOL Init(LPCTSTR strHost, LPCTSTR strUser, LPCTSTR strPassword, LPCTSTR strDatabase, INT nPort, INT nConNum=2);
	VOID ShutDown();

	//---------------------------------------------------------------------------
	// �������
	//---------------------------------------------------------------------------
	BETON_INLINE Connection* GetFreeConnection();
	BETON_INLINE VOID ReturnConnection(Connection* con);

	//---------------------------------------------------------------------------
	// �鿴���Ӷ�ʧ������
	//---------------------------------------------------------------------------
	BOOL Reconnect();
	BETON_INLINE BOOL IsConnLost()		{ return m_bConLost; }


	//---------------------------------------------------------------------------
	// Stream���
	//---------------------------------------------------------------------------
	BETON_INLINE MyStream* GetStream() { return m_StreamPool.AllocStream(); }
	BETON_INLINE VOID ReturnStream(MyStream* pStream) { m_StreamPool.FreeStream(pStream); }

	//----------------------------------------------------------------------------
	// ��ѯ���
	//----------------------------------------------------------------------------
	BETON_INLINE QueryResult* Query(const char* szStr);
	BETON_INLINE QueryResult* Query(const MyStream* pStream);
	BETON_INLINE QueryResult* WaitQuery(const char* szStr, Connection* con);
	BETON_INLINE QueryResult* WaitQuery(const MyStream* pStream, Connection* con);

	//-----------------------------------------------------------------------------
	// �������
	//-----------------------------------------------------------------------------
	BETON_INLINE BOOL Execute(const char* szStr);
	BETON_INLINE BOOL Execute(const MyStream* pStream);
	BETON_INLINE BOOL WaitExecute(const char* szStr, Connection* con);
	BETON_INLINE BOOL WaitExecute(const MyStream* pStream, Connection* con);

	//-----------------------------------------------------------------------------
	// ���������
	//-----------------------------------------------------------------------------
	BETON_INLINE INT CheckExecute(const char* szStr);
	BETON_INLINE INT CheckExecute(const MyStream* pStream);
	BETON_INLINE INT CheckWaitExecute(const char* szStr, Connection* con);
	BETON_INLINE INT CheckWaitExecute(const MyStream* pStream, Connection* con);

	//------------------------------------------------------------------------------
	// ��¼�����
	//------------------------------------------------------------------------------
	BETON_INLINE VOID FreeQueryResult(QueryResult* pRet) { SAFE_DEL(pRet); }

	//------------------------------------------------------------------------------
	// �첽�������
	//------------------------------------------------------------------------------
	BETON_INLINE VOID AddQuery(MyStream* pStream) { m_AsynStreamQueue.Add(pStream); }

	//------------------------------------------------------------------------------
	// �������
	//------------------------------------------------------------------------------
	BETON_INLINE BOOL BeginTransaction(Connection* con);
	BETON_INLINE BOOL EndTransaction(Connection* con);
	BETON_INLINE BOOL RollBack(Connection* con);

	//-------------------------------------------------------------------------------
	// ϵͳ��Ϣ���
	//-------------------------------------------------------------------------------
	BETON_INLINE const std::string& GetHostName() const { return m_Hostname; }
	BETON_INLINE const std::string& GetUserName() const { return m_Username; }
	BETON_INLINE const std::string& GetPassWord() const { return m_Password; }
	BETON_INLINE const std::string& GetDatabaseName() const { return m_DatabaseName; }
	BETON_INLINE const INT GetPort() const { return m_nPort; }

	//-------------------------------------------------------------------------------
	// ����ʱ��Ϣ���
	//-------------------------------------------------------------------------------
	BETON_INLINE INT GetAvailableStreamNum() { return m_StreamPool.GetAvailableStreamNum(); }
	BETON_INLINE INT GetAllStreamNum() { return m_StreamPool.GetAllStreamNum(); }
	BETON_INLINE INT GetUnhandledAsynStreamNum() { return m_AsynStreamQueue.Size(); }

	//-------------------------------------------------------------------------------
	// Log���
	//-------------------------------------------------------------------------------
	tstring CreateLogFileName();

private:
	VOID SetParam(const char* szHost, const char* szUser, const char* szPassword, const char* szDatabase, INT nPort, INT nConNum);
	VOID SetParam(const WCHAR* szHost, const WCHAR* szUser, const WCHAR* szPassword, const WCHAR* szDatabase, INT nPort, INT nConNum);

private:
	BOOL Start();

private:
	BETON_INLINE BOOL HandleError(Connection* con, DWORD dwErrorCode, const char* szSql);
	BETON_INLINE BOOL Reconnect(Connection* con);

	BETON_INLINE BOOL SendQuery(Connection* con, const char* szSql, INT nLen, BOOL bSelf=FALSE);
	BETON_INLINE QueryResult* StoreQueryResult(Connection* con);

private:
	UINT ThreadAsynQuery();
	UINT ThreadCheck();
	


private:
	TSFPTrunk<DataBase>			m_Trunk;
	TObjRef<Util>				m_pUtil;

	Log*						m_pLog;
	tstring						m_LogFileName;

	std::string					m_Hostname;				// mysql����������
	std::string					m_Username;				// �û���
	std::string					m_Password;				// ����
	std::string					m_DatabaseName;			// ���ݿ���
	INT							m_nPort;				// mysql�������˿ں�

	WARNINGCALLBACK				m_fnWarning;			// ֪ͨ�ϲ㱨���Ļص�����

	Connection*					m_Con;					// ���ӳ�
	INT							m_nConNum;				// ���ӳص���������
	BOOL						m_bConLost;				// ���Ӷ�ʧ

	StreamPool					m_StreamPool;			// �յ�sql����
	SafeStreamQueue				m_AsynStreamQueue;		// �������Ĳ���������

	HANDLE						m_hThreadAsynQuery;		// �첽�����߳�
	HANDLE						m_hThreadCheck;			// �������ʱϵͳ��Ϣ

	VOLATILE BOOL				m_bTerminateAsynQuery;	// �첽�����߳̽�����ʶλ
	VOLATILE BOOL				m_bTerminateCheck;		// ����߳̽�����ʶλ
};

} // namespace Beton