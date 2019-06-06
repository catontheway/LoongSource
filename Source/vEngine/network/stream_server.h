//-----------------------------------------------------------------------------
//!\file stream_server.h
//!\author Lyp
//!
//!\date 2007-11-27
//!
//!\brief tcp_socket ��������
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {

// �����̺߳�������
typedef UINT (WINAPI* TCPLOGINCALLBACK)(LPBYTE pByte, DWORD dwSize);
typedef UINT (WINAPI* TCPLOGOUTCALLBACK)(DWORD dwClientID);


//-----------------------------------------------------------------------------
// ���̷߳������ˣ�Ϊÿ�����ӿ��ٶ�д�̣߳������ڴ�����������ͨѶ
// ���õ�½/�ǳ��ص�����,�ⲿ��Ҫ��֤�����ص������̰߳�ȫ
// ע�⵱�û���¼ʱ,�Ὣ��һ������Ϊ��������fnLogIn, fnLogIn���뷵��һ��ID
// ����ϲ㷵��GT_INVALID,��ܾ��˿ͻ��˵�½
// ���û��ǳ�ʱ,�Ὣ�ϲ����ID��Ϊ��������fnLogOut
//-----------------------------------------------------------------------------
class VENGINE_API StreamServer
{
public:
	BOOL Init(TCPLOGINCALLBACK fnLogIn, TCPLOGOUTCALLBACK fnLogOut, INT nPort, 
		BOOL bReUsePort=TRUE, BOOL bPackageMode=TRUE);
	BOOL Destroy();

	//-----------------------------------------------------------------------------
	// �����������
	//-----------------------------------------------------------------------------
	// ���ط��ͻ���Ŀǰ��С
	INT Send(DWORD dwClientID, LPVOID pMsg, DWORD dwSize);
	// nUnRecved=Ŀǰ�ײ��հ�����İ�����
	LPBYTE Recv(DWORD dwClientID, DWORD& dwSize, INT& nUnRecved);
	// ��Ϊ�ڴ����ڲ�����ģ������հ���Ӧ�ý����ڲ����ͷ�
	VOID FreeRecved(DWORD dwClientID, LPBYTE pMsg) ;
	
	BOOL Kick(DWORD dwClientID);	// ����FALSE:�Ҳ���ָ�����


	//-----------------------------------------------------------------------------
	// ��Ϣͳ���Լ����ߺ���
	//-----------------------------------------------------------------------------
	// �õ��������˿ں�
	INT GetPort() { return m_nPort; }
	// �õ������ϵĿͻ�����Ŀ
	INT	GetClientNum() { return m_mapClient.Size(); }  
	// ���ָ��ID�Ŀͻ��ǲ��Ǵ���
	BOOL IsClientExsit(DWORD dwID) { return m_mapClient.IsExist(dwID); }
	// �õ�ָ��ID�ͻ��˵�sock name
	BOOL GetClientSockName(sockaddr_in* pAddr, DWORD dwClientID);
	// �ܷ�����
	DWORD GetMsgSend() {return m_dwMsgSend;}
	// ���հ���
	DWORD GetMsgRecv() {return m_dwMsgRecv;}
	// �����ֽ�����
	DWORD GetBytesSend() {return m_dwBytesSend;}
	// ���ֽ�����
	DWORD GetBytesRecv() {return m_dwBytesRecv;}


	StreamServer();
	~StreamServer();

private:
	struct tagClient	// ÿ�ͻ�������
	{
		DWORD			dwID;
		SOCKET			Socket;
		VOLATILE BOOL	bTerminate;
		HANDLE			hThreadSend;
		HANDLE			hThreadRecv;
		MsgQueueTS*		pSendMsgQueue;
		MsgQueueTS*		pRecvMsgQueue;
		DWORD			dwConnectTime;
		INT				nRecvPackageNum;

		tagClient():dwID(GT_INVALID),Socket(0),bTerminate(FALSE),
			hThreadSend(0),hThreadRecv(0),pSendMsgQueue(0),pRecvMsgQueue(0),
			dwConnectTime(0),nRecvPackageNum(0){}
	};

	TSFPTrunk<StreamServer>				m_Trunk;
	SafeMemPool*						m_pMemPool;
	CRITICAL_SECTION					m_DisconnectLock;
	BOOL								m_bPackageMode;			// ��ģʽ
	TCPLOGINCALLBACK					m_fnLogIn;				// ���ֻص�����
	TCPLOGOUTCALLBACK					m_fnLogOut;

	VOLATILE BOOL						m_bTerminateAccept;		// �Ƿ���Ҫ�ر�Accept�߳�
	VOLATILE BOOL						m_bTerminateDisconnect;	// �Ƿ���Ҫ�رն����߳�
	TSafeMap<DWORD, tagClient*>			m_mapClient;			// ��¼�û�nCDIndex��ID���
	TSafeList<tagClient*>				m_listDisClient;		// ��Ҫɾ���Ŀͻ��˶���
	INT									m_nPort;				// �˿�
	sockaddr_in							m_Address;				// server address
	SOCKET								m_Socket;				// ����Accept��socket
	HANDLE								m_hThreadAccept;		// �߳̾��
	HANDLE								m_hThreadDisconnect;	// �߳̾��

	volatile DWORD						m_dwMsgSend;			// ͳ������
	volatile DWORD						m_dwMsgRecv;
	volatile DWORD						m_dwBytesSend;
	volatile DWORD						m_dwBytesRecv;


	// �ڲ��߳�
	UINT ThreadAccept(LPVOID);
	UINT ThreadDisconnect(LPVOID);
	// ÿ�ͻ����߳�
	UINT ThreadSend(tagClient*);
	UINT ThreadRecv(tagClient*);
	UINT ThreadStreamRecv(tagClient*);

	VOID DisconnetClient(tagClient* pClient);

	
};


} // namespace wEngine {