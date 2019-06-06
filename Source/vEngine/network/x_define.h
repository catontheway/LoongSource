//-----------------------------------------------------------------------------
//!\file x_define.h
//!\author Lyp
//!
//!\date 2004-07-02
//! last 2009-04-03
//!
//!\brief �������˻�������
//-----------------------------------------------------------------------------
#pragma once

#include "memory\safe_mem_pool.h"
#include "x_queue.h"
#include "x_list.h"


namespace vEngine {
//-----------------------------------------------------------------------------
// �������˻�������
//-----------------------------------------------------------------------------
#define X_SLEEP_TIME			500


//-----------------------------------------------------------------------------
// server unit ״̬
//-----------------------------------------------------------------------------
enum EXState
{
	EXS_Null = 0,
	EXS_Accept,
	EXS_RecvLength,
	EXS_Recv,
	EXS_Send,
	EXS_End 
};


//-----------------------------------------------------------------------------
//!	unit����
//-----------------------------------------------------------------------------
struct tagUnit
{
	WSAOVERLAPPED	ov;						// �ص�IO�ṹ
	EXState			eState;					// ����״̬
	SafeMemPool*	pPool;					// ��¼���ĸ�mem_pool����
	tagUnit*		pNext;					// ָ����һ��unit,����safe_unit_queue
	DWORD			dwBytesReady;			// �Ѿ��ж����ֽڴ������
	DWORD			dwSize;					// ��Ч���ݴ�С,����ʵ���ڴ�ռ��С
	CHAR			pBuffer[sizeof(DWORD)];
};


struct tagRawUnit
{
	SafeMemPool*	pPool;					// ��¼���ĸ�mem_pool����
	DWORD			dwSize;					// ��Ч���ݴ�С,����ʵ���ڴ�ռ��С
	tagRawUnit*		pNext;					// ָ����һ��unit,����safe_unit_queue
	CHAR			pBuffer[sizeof(DWORD)];
};


// ����ʵ������unit�Ĵ�С
#define UNIT_SIZE(size) (sizeof(tagUnit)+(size))

// LZO����ʹ����ѹ������
#define LZO_ADD(size) (((size)/64) + 16 + 3)

// ����ʵ������unit�Ĵ�С
#define RAWUNIT_SIZE(size) (sizeof(tagRawUnit)+(size))


//-----------------------------------------------------------------------------
//!	����ڵ����ͻ��˵Ķ���
//-----------------------------------------------------------------------------
struct tagXClient
{
	BOOL VOLATILE			bShutDown;		// �Ƿ��Ѿ���shutdown
	LONG VOLATILE			lSendCast;		// �Ѿ�������send,��δ�յ�֪ͨ��
	LONG VOLATILE			lRecvCast;		// �Ѿ�������recv,��δ�յ�֪ͨ��

	SOCKET					sock;			// �ͻ��˶�Ӧ��socket
	DWORD					dwClientID;		// �ͻ���ID,�ϲ�ȷ�ϵ�һ�����Ժ����Ч
	DWORD					dwConnectTime;	// ����ȷ���߳���ʱ��δ��֤�ͻ���
	DWORD					dwSendSize;		// �ȴ����ʹ�С

	XQueue<tagRawUnit*>* 	pSendQueue;		// ��������,��ƴ��ѹ��
	XQueue<tagUnit*>* 		pRecvQueue;		// �հ�����,���ϲ���ȡ
};



//-----------------------------------------------------------------------------
// ��½������������Ϊ�ͻ��˵�½ʱ �ϲ���²��֮����Ϣ�������ֶ�
//-----------------------------------------------------------------------------
struct tagXLoginParam
{
	DWORD	dwHandle;	// ÿ�ͻ���handle
	DWORD	dwAddress;	// Զ�˵�ַ
	DWORD	dwPort;		// Զ�˶˿�
};

#define PARAM_SIZE (((sizeof(sockaddr_in)+16)*2)+sizeof(tagXLoginParam))
#define PARAM_INDEX ((sizeof(sockaddr_in)+16)*2)

//-----------------------------------------------------------------------------
// ��������������
//-----------------------------------------------------------------------------
// �����̺߳�������
typedef UINT (WINAPI* XLOGINCALLBACK)(tagUnit*, tagXLoginParam*);
typedef UINT (WINAPI* XLOGOUTCALLBACK)(DWORD);

struct tagXServerConfig
{
	XLOGINCALLBACK	fnLogIn;		// ��½�ص�
	XLOGOUTCALLBACK	fnLogOut;		// �ǳ��ص�

	INT	nPort;						// �˿�
	INT nMaxServerLoad;				// ��������������������
	INT nPerCpuWorkerThread;		// ÿCPU�����߳�
	INT nAdditionWorkerThread;		// ���ӹ����߳��� 
	INT nAcceptExNum;				// ������acceptex����
	DWORD dwPoolSize;				// ÿ���ڴ�صĴ�С���ڴ���������߳�������
	DWORD dwAuthWaitTime;			// ��֤��ȴ�ʱ��(����)
	DWORD dwPerTickSendSize;		// ÿ�ͻ���ÿ������������ֽ���
	DWORD dwMaxRecvSize;			// �����ܰ�����
	bool bReusePort;				// �Ƿ����ö˿�
	bool bNagle;					// �Ƿ�ʹ��Nagle�㷨

	tagXServerConfig()
	{ 
		fnLogIn = NULL;
		fnLogOut = NULL;
		nPort = 6008; 
		nMaxServerLoad = 4000; 
		nPerCpuWorkerThread = 2;
		nAdditionWorkerThread = 2;
		nAcceptExNum = 32; 
		
		dwPoolSize = 4*1024*1024;
		dwAuthWaitTime = 20*1000;
		dwPerTickSendSize = 4*1024*1024;
		dwMaxRecvSize = 512;
		
		bReusePort = false;
		bNagle = false;
	}
};


} // namespace vEngine {




