#ifndef PAGEASSIST_H_
#define PAGEASSIST_H_

#include "../ServerDefine/rt_define.h"

class ShowPage;
struct UserInfo;
//���ط�����ҳ������
struct ShowPageData;
struct SectionInfo;

class PageAssist;
typedef void (ShowPage::* OACSUPDATEFUNC)(PageAssist*, BOOL is_static);

class RTServer;

//Ϊ��ʹ��������ݷ���
class PageAssist
{
public:

	PageAssist();
	~PageAssist();
	//�õ���¼�û���Ϣ
	void SetUserInfo(const UserInfo* user_info);
	UserInfo* GetUserInfo() { return user_info_; }

	//�õ�ShowPageDataList
	std::list<ShowPageData*>& GetShowPageDtatList() { return openandcloseserver_page_data_list_; }
	std::list<ShowPageData*>& GetAbnormalServerPageDtatList() { return page_data_list_abnormal; }
	void AddShowPageDtatList(int index, EServerType type, wxString section_name,  DWORD section_id, 
														wxString world_name,   DWORD world_id, wxString ip);
	void UpdateShowPageDtatList(SectionInfo* section_info);
	void ClearShowPageDtatList() 
	{ 
		openandcloseserver_page_data_list_.clear();
		page_data_list_abnormal.clear();
		temp_moving_data_list.clear();
	}

	void SetShowPageFunc(ShowPage *page, OACSUPDATEFUNC fn ) { page_ = page; ShowPage_fn = fn; }
	void UpdateShowPage( BOOL is_static);

	//void SendStartServerMessage(ShowPageData* data, BOOL alarm);
	//void SendCloseServerMessage(BOOL is_force, ShowPageData* data, BOOL alarm);

	void SwitchDatalist(ShowPageData* server_data);
	void ResortDatalist();

	/*void SendSystemCmdMessage(ShowPageData* data, BOOL alarm, LPCTSTR szCmd);

	BOOL SendChangeMaxMessage(ShowPageData* data, BOOL alarm, LPCTSTR szCmd);
	
	BOOL UpdateMall(ShowPageData* data, BOOL alarm);*/

	//GM����ҳ�湦��
	/*void SendCreateGMMessage(LPCTSTR szName, LPCTSTR szPwd, EPrivilege ePrivilege);
	void SendModifyGMMessage(LPCTSTR szName, DWORD dwIP);
	void SendDeleteGMMessage(LPCTSTR szName);
	void SendQueryGMMessage();*/

private:
	OACSUPDATEFUNC ShowPage_fn;
	ShowPage *page_;
	std::list<ShowPageData*> openandcloseserver_page_data_list_;
	std::list<ShowPageData*> page_data_list_abnormal;
	UserInfo *user_info_;
	RTServer* server_;

	BOOL bListChange;
	std::list<ShowPageData*> temp_moving_data_list;
};


#endif  /* PAGEASSIST_H_ */