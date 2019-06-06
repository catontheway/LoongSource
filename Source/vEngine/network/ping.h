/***********************************************************************
 rawping.h - Declares the types, constants and prototypes required to
    use the rawping.cpp module.
***********************************************************************/
#pragma once
namespace vEngine {

//-----------------------------------------------------------------------------
// ���ְ汾��ping for winsock1.1 and winsock2
//-----------------------------------------------------------------------------
class VENGINE_API Ping
{
public:
	//! dwTimeOut = ping��ʱ,����Ϊ��λ
	//! bAutoUseSocket2 = �Ƿ��Զ�ʹ��socket2
	BOOL Init(DWORD dwTimeOut);
	VOID Destroy();

	 //! Ping����handle,ʧ�ܷ���GT_INVALID
	INT StartPing(LPCSTR szIP);
	//! ǿ�ƹر�����ping���߳�,ע������Ҳ��ɾ���Ѿ�ping�õĽ��
	VOID ForcedCloseAllPing();
	//! ���ping�Ƿ����;
	BOOL IsPingFinished(INT nHandle);
	//! �ɹ�����pingʱ��,ʧ�ܷ���GT_INVALID
	//! ʧ�����������:1.����ping��δ���� 2.Զ�������޷�pingͨ
	INT GetPingResult(INT nHandle);

	Ping();
	~Ping();



#pragma pack(1)
	// The IP header
	struct IPHeader {
		BYTE h_len:4;           // Length of the header in dwords
		BYTE version:4;         // Version of IP, 4 bits
		BYTE tos;               // Type of service
		USHORT total_len;       // Length of the packet in dwords
		USHORT ident;           // unique identifier
		USHORT flags;           // Flags
		BYTE ttl;               // Time to live
		BYTE proto;             // Protocol number (TCP, UDP etc)
		USHORT checksum;        // IP checksum
		ULONG source_ip;
		ULONG dest_ip;
	};
#pragma pack()


private:	// ------------------------------------------------------------
#pragma pack(1)
	// ICMP header
	struct ICMPHeader {
		BYTE type;          // ICMP packet type
		BYTE code;          // Type sub code
		USHORT checksum;
		USHORT id;
		USHORT seq;
		ULONG timestamp;    // not part of ICMP, but we need it
	};

	typedef struct {
		unsigned char Ttl;                         // Time To Live
		unsigned char Tos;                         // Type Of Service
		unsigned char Flags;                       // IP header flags
		unsigned char OptionsSize;                 // Size in bytes of options data
		unsigned char *OptionsData;                // Pointer to options data
	} IP_OPTION_INFORMATION, * PIP_OPTION_INFORMATION;

	typedef struct {
		DWORD Address;                             // Replying address
		unsigned long  Status;                     // Reply status
		unsigned long  RoundTripTime;              // RTT in milliseconds
		unsigned short DataSize;                   // Echo data size
		unsigned short Reserved;                   // Reserved for system use
		void *Data;                                // Pointer to the echo data
		IP_OPTION_INFORMATION Options;             // Reply options
	} IP_ECHO_REPLY, * PIP_ECHO_REPLY;
#pragma pack()

	struct tagThreadData
	{
		SOCKET	sock;
		INT		nSeqNum;
		char	szIP[40];
	};

	// function pointer for funtion in icmp.dll
	typedef HANDLE (WINAPI* pfnHV)(VOID);
	typedef BOOL (WINAPI* pfnBH)(HANDLE);
	typedef DWORD (WINAPI* pfnDHDPWPipPDD)(HANDLE, DWORD, LPVOID, WORD,
		PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD); 

	TSFPTrunk<Ping>		m_Trunk;

	BOOL					m_bUseRawSocket;
	volatile DWORD			m_dwTimeOut;
	volatile BOOL			m_bTerminateAllPingThread;
	std::vector<HANDLE>		m_vecThreadHandle;
	INT						m_nSeqNum;
	TSafeMap<INT, INT>		m_mapPingResult; // ��SeqNum��Ӧ��pingʱ��


	// ICMP.dll���
	HINSTANCE				m_hICMP;
	pfnHV					m_pIcmpCreateFile;
	pfnBH					m_pIcmpCloseHandle;
	pfnDHDPWPipPDD			m_pIcmpSendEcho;
	HANDLE					m_hPingService;

	// ping�߳�
	UINT ThreadPing(LPVOID);

	INT SetupForPing(LPCSTR host, SOCKET& sd, sockaddr_in& dest);
	INT SendPing(SOCKET sd, const sockaddr_in& dest, 
        ICMPHeader* send_buf, INT packet_size);
	INT DecodeReply(IPHeader* reply, INT bytes, sockaddr_in* from);
	VOID InitPingPacket(ICMPHeader* icmp_hdr, INT packet_size, INT seq_no);
	USHORT IpChecksum(USHORT* buffer, INT size);
	INT AllocateBuffers(ICMPHeader*& send_buf, IPHeader*& recv_buf, INT packet_size);

};

} // namespace vEngine {




