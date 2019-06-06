//-----------------------------------------------------------------------------
//!\file server_define.h
//!\author Lyp
//!
//!\date 2004-07-02
//! last 2008-04-17
//!
//!\brief �������˻�������
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// �������˻�������
//-----------------------------------------------------------------------------
#ifndef VOLATILE
#define VOLATILE volatile
#endif
#define GT_MAX_PACKAGE_LEN		512	// max package length
#define MAX_UNIT_POOL_NUM		512
#define DEFAULT_SPIN_COUNT		4000

// #define _DEBUG_UNIT	// �Ƿ��� Unit



template<typename ITEM_POINTER>
class SafeQueue;
class UnitPool;
class CompletionTransport;
class CompletionSession;
class SectCrypt;


//-----------------------------------------------------------------------------
// ��������������
//-----------------------------------------------------------------------------
struct tagServerInitParam
{
	INT	nPort;						// �˿�
	INT nMaxServerLoad;				// ��������������������
	INT nPerCpuWorkerThread;		// ÿCPU�����߳�
	INT nAdditionalWorkerThread;	// ���ӹ����߳��� 
	INT nAuthWaitTime;				// ��֤��ȴ�ʱ��
	INT nUnitPerWorkerThread;		// ÿ�������̵߳�unit����
	INT nAcceptExNum;				// ������acceptex����
	INT nCloseSocketWaitTime;		// close queue �е�ÿ��pUnit�ȴ�ʱ��
	bool bReusePort;				// �Ƿ����ö˿�

	tagServerInitParam()
	{ 
		nPort = 6008; 
		nMaxServerLoad = 256; 
		nPerCpuWorkerThread = 2; 
		nAdditionalWorkerThread = 2; 
		nAuthWaitTime = 300000; 
		nUnitPerWorkerThread = 1000;
		nAcceptExNum = 32; 
		nCloseSocketWaitTime = 5000;
		bReusePort = false;
	}

	VOID SetLarge()
	{
		nMaxServerLoad = 20000;
		nUnitPerWorkerThread = 50000;
		nAcceptExNum = 128; 
	}

	VOID SetNormal()
	{
		nMaxServerLoad = 10000;
		nUnitPerWorkerThread = 10000;
		nAcceptExNum = 128; 
	}
};


//-----------------------------------------------------------------------------
// ��½������������Ϊ�ͻ��˵�½ʱ �ϲ���²��֮����Ϣ�������ֶ�
//-----------------------------------------------------------------------------
struct tagLoginParam
{
	DWORD	dwAddress;
	WORD	wPort;
};


//-----------------------------------------------------------------------------
// server exchage unit ״̬
//-----------------------------------------------------------------------------
enum ECSUnitState
{
	EUS_Null = 0,
	EUS_Accept,
	EUS_FirstRecv,
	EUS_RecvLength,
	EUS_RecvLengthComplete,
	EUS_Recv,
	EUS_RecvComplete,
	EUS_Send,
	EUS_SendComplete,
	EUS_Read,
	EUS_Write,
	EUS_End 
};



//-----------------------------------------------------------------------------
//!	���ƶ��壺���ÿ��unit, ������1����������Ϊwindows�����wbuf
//-----------------------------------------------------------------------------
struct tagUnitData
{
	WSAOVERLAPPED	ov;						// �ص�IO�ṹ
	WSABUF			wbuf;					// ��������

	ECSUnitState	eState;					// ����״̬
	INT				nCDIndex;				// point to client data array
	DWORD			dwBytesReady;			// �Ѿ��ж����ֽڴ������

	tagUnitData*	pNext;					// ָ����һ��unit,����safe_unit_queue

	bool			bFirstRecvUnit;			// �Ƿ��յ��ĵ�һ����
	bool			bFirstUnitOfPackage;	// �Ƿ���package�еĵ�һ��unit
	BYTE			byEncryptNum;			// ���ܺ�,����ͬһ��package������ͬ

	
};



//-----------------------------------------------------------------------------
//!	����ڵ����ͻ��˵Ķ���
//-----------------------------------------------------------------------------
struct tagClientData
{
	VOLATILE BOOL		bValid;
	VOLATILE BOOL		bShutDown;			// ����ȴ��ر�״̬
	VOLATILE BOOL		bClosed;			// �Ƿ�closesocket��
	VOLATILE BOOL		bReadyForRecv;		// �Ƿ�׼���ý�����Ϣ
	
	VOLATILE INT		nSendCast;			// �Ѿ�������wsasend,��δ�յ�֪ͨ��
	VOLATILE INT		nRecvCast;			// �Ѿ�������wsarecv,��δ�յ�֪ͨ��
	
	VOLATILE DWORD		dwConnectTime;		// ����ȷ���߳���ʱ��δ��֤�ͻ���
	VOLATILE DWORD		dwLogoutTime;		// ����ȷ��closesocketʱ��

	INT					nIndex;
	DWORD				dwClientID;			// �ͻ���ID,�ϲ�ȷ�ϵ�һ�����Ժ����Ч
	DWORD				dwSerialID;			// ���к�,����client id invalid�������

	SafeQueue<tagUnitData*>*  pRecvQueue;			// �հ�����

	SOCKET				sock;
	DWORD				dwAddress;
	WORD				wPort;
};





// �����̺߳�������
typedef UINT (WINAPI* LOGINCALLBACK)(tagUnitData*, tagLoginParam*);
typedef UINT (WINAPI* LOGOUTCALLBACK)(DWORD);
typedef UINT (WINAPI* CRYPTCALLBACK)(tagUnitData*);
typedef UINT (WINAPI* HACKREPORTCALLBACK)(DWORD);


} // namespace vEngine {