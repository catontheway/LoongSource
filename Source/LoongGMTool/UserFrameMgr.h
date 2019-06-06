#pragma once
#include "Singleton.h"
#include "UserEvent.h"
//--------------------------------------------------------------------
// ע���û������¼�
//--------------------------------------------------------------------
class UserFrame;



typedef DWORD (WINAPI* FRAMEEVENTPROC)(tagUserEvent* pEvent);

class UserFrameMgr : public Singleton<UserFrameMgr>
{
public:
	UserFrameMgr();
	~UserFrameMgr();

	BOOL Init();
	BOOL Destroy();

	VOID SendEvent(tagUserEvent* pEvent);

	// ע��Frame�¼�������
	BOOL RegisterEventHandle(LPCTSTR szEvent, FRAMEEVENTPROC fp);
	BOOL UnRegisterEventHandler(LPCTSTR szEvent, FRAMEEVENTPROC fp);

private:

	struct tagEventHandle	// �¼�����ṹ����
	{
		tstring					strName;
		FRAMEEVENTPROC			fp;
	};

	std::map<DWORD, tagEventHandle*>	m_mapHandle;	// �¼�������
};

#define sUserFrameMgrPgr UserFrameMgr::GetSingletonPtr()