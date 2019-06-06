//-----------------------------------------------------------------------------
//!\file broadcast.h
//!\author Lyp
//!
//!\date 2004-08-04
//! last 2004-08-04
//!
//!\brief �������㲥����
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {


//-----------------------------------------------------------------------------
//!\brief �������㲥����
//!
//!
//-----------------------------------------------------------------------------
class Broadcast
{
public:
	//! ��ʼ���㲥����
	//! \param szLocalIP �󶨵�����ʲôIP������Ϊ��
	BOOL Init(INT nPort, LPCSTR szLocalIP=NULL);
	VOID Destroy();

	//! ��ʼ�㲥�߳�
	BOOL StartBroadcast();
	//! ��ʼ�����㲥�߳�
	BOOL StartRecvBroadcast();

	//�������չ㲥�߳�
	BOOL TerminateRecvBroadcast(DWORD dwTimeOut=0);
	//�����㲥�߳�
	BOOL TerminateBroadcast(DWORD dwTimeOut=0);

	//! �㲥
	VOID Broad(LPBYTE pMsg, DWORD dwMsgSize);
	//! �õ����յ��Ĺ㲥��Ϣ��û�оͷ���NULL
	LPBYTE RecvBroadcast(DWORD& dwMsgSize);
	//! ��Ϊ�ڴ����ڲ�����ģ�����Ӧ�ý����ڲ����ͷ�
	VOID FreeRecved(LPBYTE pMsg);
	
	Broadcast();
	~Broadcast();


private:
	TSFPTrunk<Broadcast>	m_Trunk;
	
	MsgQueueTS*		m_pSendMsgQueue;
	MsgQueueTS*		m_pRecvMsgQueue;

	HANDLE			m_hThreadBroadcast;
	HANDLE			m_hThreadRecvBroadcast;
	volatile BOOL	m_bTerminateBroadcast;
	volatile BOOL	m_bTerminateRecvBroadcast;

	volatile INT	m_nPort;
	volatile TCHAR	m_szLocalIP[X_LONG_STRING];

	UINT ThreadBroadcast(LPVOID);		// �㲥�߳�
	UINT ThreadRecvBroadcast(LPVOID);	// ���㲥�߳�
};






} // namespace vEngine {
