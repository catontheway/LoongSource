#pragma once
#include "..\WorldDefine\chat.h"
class ChatShowPop
{
public:
	ChatShowPop(void);
	~ChatShowPop(void);
	static ChatShowPop* Inst();


	//��ҷ���ʱ���ã�����ҵ��򴴽���������
	void PushChatInfo(DWORD dwRoleID,const TCHAR* szText);
	
	//����������Ϣ
	void NetRecvRoleChat(tagNS_RoleChat* pMsg);

	void DrawInfo();

	void Destroy();

private:
	//ɾ��ĳ�˵ĺ�����Ϣ����,ͬʱ���ٴ���
	void DeleteChatShow(DWORD dwRoleID);
	
private:
	GUIWnd*						m_pWnd;
	
	map<DWORD,float>			m_mapChatShow;			//�����ݱ����ڶ�Ӧ��xui�ļ��У����ﻹ��Ҫ���溰����ʱ��
};