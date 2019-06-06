//-----------------------------------------------------------------------------
//!\file stream_transport.h
//!\author Lyp
//!
//!\date 2003-03-17
//! last 2008-04-17
//!
//!\brief �������ͻ���ͨѶ�ײ�
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {

enum EStreamTransportMode
{
	ESTM_Package = 0,
	ESTM_Stream = 1,
	ESTM_Text = 2
};

//-----------------------------------------------------------------------------
//! Provides sequenced, reliable, two-way, connection-based virtual package 
//! streams transmission mechanism. Uses TCP for the Internet address family.
//-----------------------------------------------------------------------------
class VENGINE_API StreamTransport
{
public:
	BOOL Init(BOOL bNagle=TRUE, EStreamTransportMode eMode=ESTM_Package);
	VOID Destroy();

	//-----------------------------------------------------------------------------
	// �����������
	//-----------------------------------------------------------------------------
	VOID TryToConnect(LPCSTR szIP, INT nPort=0); // ��������ָ����ַ
	BOOL IsTryingToConnect();	// �Ƿ����ڳ�������
	VOID StopTryingConnect();	// ������������
	VOID Disconnect(); // �Ͽ�����
	BOOL IsConnected() { return m_bConnected; }

	// �������FALSE,����û�����ӻ�û���ڴ�ռ���������msg
	BOOL Send(LPVOID pMsg, DWORD dwMsgSize);
	// �������NULL��ʾû�����ӻ�δ�յ�����
	LPBYTE Recv(DWORD& dwMsgSize);
	// ��Ӧ��Recv(),��Ϊ�ڴ����ڲ�����ģ�����Ӧ�ý����ڲ����ͷ�
	VOID FreeRecved(LPBYTE pMsg);

	BOOL SetPackageSizeEndian(BOOL bBigEndian);	// ���ð�ģʽ��SIZEͷ���ֽ���

	//-----------------------------------------------------------------------------
	// ��Ϣͳ���Լ����ߺ���
	//-----------------------------------------------------------------------------
	INT	GetUnsendPackageNum();	// �õ�δ���͵İ������������Դ����ж����縺��
	INT	GetReceivedPackageNum(); // �õ��ѽ��ܵ�δ���ϲ㴦��İ�����
	DWORD GetTotalSendPackNum() { return m_dwSendPackNum; }	// ��ȡ�ܷ�����(��δ�����Ļ���ʧ�ܵ�)
	DWORD GetTotalRecvPackNum() { return m_dwRecvPackNum; }	// �������հ���
	DWORD GetBytesFlowOut(){ return m_dwByteOut; } // �õ�ÿ��������
	DWORD GetBytesFlowIn(){ return m_dwByteIn; } // �õ�ÿ��������

	LPCSTR GetRemoteIPString() { return m_strIP.c_str(); }// �õ��Է�IP��ַ�ַ���
	BOOL GetHostName(std::string& strName);	// �õ�������������
	BOOL GetIPByHostName(DWORD& dwIP, LPCSTR lpHostName);	// �õ�ָ��������IP
	BOOL GetLocalLANIP(DWORD& dwIP);	// �õ�������������ַ
	BOOL GetLocalWANIP(DWORD& dwIP);	// �õ�������������ַ
	CHAR* IP2String(DWORD dwIP);	// ������IP��ַת��ΪIP��ַ�ַ���
	DWORD StringIP2IP(CHAR* szIP);	// ��IP��ַ�ַ���ת��Ϊ����IP��ַ

	BOOL SwitchTransportMode(EStreamTransportMode eMode); // �ı�ģʽ����ģʽ֮���л����ʺ�HTTP����


	StreamTransport();
	~StreamTransport();

private:
	TSFPTrunk<StreamTransport>	m_Trunk;
	EStreamTransportMode		m_eTransportMode;	// ����ģʽ
	BOOL						m_bBigEndian;		// ��ͷ�ֽ���

	MemPool*				m_pMemPool;

	// ���ͽ��հ�ȫ����
	MsgQueueTS*				m_pSendMsgQueue;
	MsgQueueTS*				m_pRecvMsgQueue;
	
	// ��������
	std::string				m_strIP;
	INT						m_nPort;
	SOCKET					m_Sock;
	BOOL					m_bNagle;

	// �ڲ��߳̾��
	HANDLE					m_hConnect;
	HANDLE					m_hSend;
	HANDLE					m_hReceive;

	// �ڲ��߳̿���
	volatile BOOL			m_bTerminateConnect;
	volatile BOOL			m_bTerminateSend;
	volatile BOOL			m_bTerminateReceive;
	volatile BOOL			m_bConnected;		// �Ƿ�������

	//! ÿ������ͳ��
	volatile DWORD			m_dwLastStatInTime;	// �ϴ�Inͳ��ʱ��
	volatile DWORD			m_dwLastStatOutTime;// �ϴ�Outͳ��ʱ��
	volatile DWORD			m_dwByteOut;		// ��ǰ���
	volatile DWORD			m_dwByteIn;			// ��ǰ����
	volatile DWORD			m_dwByteOutCounter;
	volatile DWORD			m_dwByteInCounter;


	//�շ���������Ϣ(���ӿ�ʼ--���ӽ���)
	volatile DWORD			m_dwSendPackNum;	// �ܷ�����(��������ʧ�ܺͻ�δ����ȥ��)
	volatile DWORD			m_dwRecvPackNum;    // ���հ���(������δ�����)

	// �ڲ�����
	VOID Clear();
	BOOL GracefulCloseSocket(SOCKET& socket, INT nRetryTimes);
	BOOL GracefulCloseThread(HANDLE& thread, INT nRetryTime);

	// �ڲ��߳�
	UINT ThreadConnect(LPVOID);	// connect thread function
	UINT ThreadSend(LPVOID);	// send thread function
	UINT ThreadReceive(LPVOID);	// receive thread function
	UINT ThreadStreamReceive(LPVOID);

};




} // namespace vEngine {
