//����RTServer,����

#ifndef RTSERVERMANAGER_H_
#define RTSERVERMANAGER_H_

#include "Singleton.h"
#include "../ServerDefine/rt_define.h"

class RTServer;
class PageAssist;

class RTServerManager : public Singleton<RTServerManager>
{
public:
	RTServerManager();
	~RTServerManager();

	//��ʼ��
	bool Init();

	//��ȡ�����ļ�
	bool ReadConfigFile();

	//����
	void Destroy();

	//����RTServer
	void ConnectRTServer();

	//������Ϣ�����߳�
	void HandleMessages();

	//�����������Ϣ
	void QueryServerInfo();

	//����PageAssistָ��
	void SetPageAssistPtr(PageAssist *page_assist);

	//�Ƿ��Ѿ�����RTServer
	BOOL IsConnected();

	//�Ƿ��Ѿ���ʼ�����
	bool IsInitSuccess();

	RTServer* GetRtServerPtr() { return rt_server_; }

	void SendStartServerMessage(DWORD section_id, DWORD world_id, EServerType server_type);
	void SendCloseServerMessage(BOOL is_force, DWORD section_id, DWORD world_id, EServerType server_type);
	void SendSystemCmdMessage(DWORD section_id, DWORD world_id, EServerType server_type, LPCTSTR szCmd);
	void SendChangeMaxMessage(DWORD section_id, DWORD world_id, LPCTSTR szCmd);
	void SendUpdateMallMessage(DWORD section_id, DWORD world_id);

	//GM����
	void SendCreateGMMessage(LPCTSTR szName, DWORD dwPswCRC, EPrivilege ePrivilege);
	void SendModifyGMMessage(LPCTSTR szName, DWORD dwIP);
	void SendDeleteGMMessage(LPCTSTR szName);
	void SendQueryGMMessage();

	//GMȨ���ж�
	bool VerifyGMPrivilege( DWORD dwMsgCrc, EPrivilege ePrivilege = EP_Null );

	//IP�Ϸ����ж�
	DWORD IsIPValid(LPSTR szIP);

	//�߳̿���
	void TerminateMsgHandleThread() { ::InterlockedExchange((LONG*)&thread_update_terminate_, TRUE); }
	HANDLE			thread_update_handle_;
	volatile BOOL	thread_update_terminate_;

private:
	//��ȡ�����ļ���������
	void RTServerManager::ReadRtServer(TiXmlElement* element);

	//�����߳�
	static UINT ThreadUpdate(LPVOID param);

	//����RTServer���б�
	RTServer* rt_server_;

	//PageAssist
	PageAssist *page_assist_;
};

//��������
#define sRTServerManager RTServerManager::getSingleton()

#endif /* RTSERVERMANAGER_H_ */