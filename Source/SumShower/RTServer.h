#ifndef RTSERVER_H_
#define RTSERVER_H_

#include "../ServerDefine/rt_define.h"

#define MAX_WAIT_TIME 60*10	// ��λ���룬�ȴ�����GMserver�Ķ�̬��Ϸ��������Ϣ�����ʱ��

class StreamTransport;
class PageAssist;
class RTServer
{
public:
	RTServer();
	~RTServer();
	
	//��ʼ��
	bool Init();
	//����
	void Destroy();

	//ע����Ϣ������
	void RegiserHandleMessage();
	
	void RecvMessage();

	//��������
	PageAssist* GetPageAssist() { return page_assist_; }
	void SetPageAssist(PageAssist* page_assist) { page_assist_ = page_assist; }

	void SetIP(const char* ip) { ip_ = ip; }
	wxString& GetIP() { return ip_; }

	void SetPort(int port) { port_ = port; }
	int GetPort() { return port_; }

	bool IsFirstConnected() { return is_first_connect_; }
	void SetFirstConnected( bool first_connect ) { is_first_connect_ = first_connect; }
	BOOL IsConnected();
	BOOL IsTryingToConnent();
	bool IsInitSuccess() { return is_init_success; }
	void Connected();
	void Disconnect();
    void SendMsg(LPVOID msg, DWORD size);

	//--��ǰGMȨ��
	void SetPrivilege( EPrivilege ePri );
	EPrivilege GetPrivilege();
	bool VerifyGMPrivilege( DWORD dwMsgCrc, EPrivilege ePrivilege = EP_Null );
	DWORD IsIPValid(LPSTR szIP);

private:
	//����NRTS_LoginService
	DWORD HandleLoginService(tagNetCmd *command);
	//����NRTS_Challenge
	DWORD HandleChanllenge(tagNetCmd *command);
	//����NRTS_Close
	DWORD HandleCloseServer(tagNetCmd *command);
	//����NRTS_Start
	DWORD HandleStartServer(tagNetCmd *command);
	//����NRTS_AllServerStatInfo
	DWORD HandleAllServerStatInfo(tagNetCmd *command);
	//����NRTS_AllServerDynaInfo
	DWORD HandleAllServerDynaInfo(tagNetCmd *command);
	//����NRTS_PrivilegeInfo
	DWORD HandlePrivilegeInfo(tagNetCmd *command);
	
	//����NGMTS_GMCreate
	DWORD HandleGMCreateResult(tagNetCmd* command);
	//����NGMTS_GMModify
	DWORD HandleGMModifyResult(tagNetCmd* command);
	//����NGMTS_GMDelete
	DWORD HandleGMDeleteResult(tagNetCmd* command);

private:
	//PageAssist
	PageAssist *page_assist_;
	//����
	wxString ip_;
	int port_;
	bool is_connect_;
	bool is_first_connect_;
	bool is_init_success;
	StreamTransport* net_;
	NetCommandManager<RTServer> net_command_manager_;
	//Ȩ�ޱ�,RTServerͨ��������Ϣ����������map,�Ժ����й���Ȩ�޵Ĳ����������Ϊ׼
	std::map<DWORD, DWORD> privilege_;
	tagDWORDTime dwLatestUpdateTime_; //��¼���һ���յ�����GMserver�Ķ�̬��Ϸ��������Ϣ��ʱ��
};



#endif /* RTSERVER_H_ */