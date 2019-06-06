//�����������Ҫ�����ݽṹ.
//2009-04-09

#ifndef DATA_DEFINE_H_ 
#define DATA_DEFINE_H_

#include "../ServerDefine/login_define.h"
#include "../ServerDefine/rt_define.h"

class PageAssist;

//��¼�û���Ϣ
struct UserInfo
{
	wxString name;
	wxString password;
	int privilege;
};

struct GameWorldInfo
{
	//��Ϸ��������,world name
	wxString world_name;
	//world id
	DWORD id;
	//world ip
	wxString world_ip;
	//db ip
	wxString db_ip;
	//world ״̬
	EWorldStatus	world_status;
	//DB ״̬
	EWorldStatus	db_status;
	// ��������
	INT					nPlayerOnline;
	// ����������
	INT					nMaxPlayerNum;	
};

//������Ϣ
struct SectionInfo
{
	//��������
	wxString section_name;
	//����Crc
	DWORD  id;
	//Login ip
	wxString login_ip;
	//Login ״̬
	EWorldStatus	login_status;
	//Login����World��������Ϣ
	INT nPlayerOnline;
	INT nMaxPlayerNum;

	std::map<DWORD, GameWorldInfo> game_world_infos;
};

//���ط�����ҳ������
struct OpenAndCloseServerPageData 
{
	//����������  world db Login
	EServerType type;
	//��������
	wxString section_name;
	DWORD section_id;

	//��Ϸ��������
	wxString world_name;
	DWORD world_id;

	//������ip
	wxString ip;
	//������״̬
	EWorldStatus status;
	//��������
	int online_number;
	//��������
	int max_online;
	//��������
	wxString operate_name;
	//�Ƿ�ѡ��
	bool is_check;
	//PageAssistָ��
	PageAssist *page_assist;
	//�б���˳���ʶ
	int index;

	OpenAndCloseServerPageData()
	{
		page_assist = NULL;
		section_id = 0;
		world_id = 0;
		section_name = wxT("");
		world_name = wxT("");
		ip = wxT("");
		world_name = wxT("");
		status = EWS_InitNotDone;
		online_number = 0;
		max_online = 0;
		operate_name = wxT("");
		is_check = false;
		index = 0;
	}
};

//�º�����
class CmpData{   
public:   
	CmpData() {};   
	bool operator()(OpenAndCloseServerPageData*& x, OpenAndCloseServerPageData*& y) const
	{   
		return x->index < y->index; 
	}
};  

#endif /* DATA_DEFINE_H_ */