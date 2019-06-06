#include "StdAfx.h"
#include "PageAssist.h"
#include "ShowPage.h"
#include "data_define.h"
#include "GameServerMgr.h"
#include "SumShower.h"
#include "UIWords.h"

PageAssist::PageAssist()
{
	user_info_ = new UserInfo;
}

PageAssist::~PageAssist()
{
	std::list<ShowPageData*>::iterator it = openandcloseserver_page_data_list_.begin();
	for (; it != openandcloseserver_page_data_list_.end(); ++it)
	{
		ShowPageData* data = *it;
		delete data;
		data = NULL;
	}
	if (user_info_ != NULL)
	{
		delete user_info_;
		user_info_ = NULL;
	}
}

void PageAssist::UpdateShowPage( BOOL is_static)
{
	if (ShowPage_fn != NULL)
	{
		(page_->*ShowPage_fn)(this, is_static);
	}
}

void PageAssist::SetUserInfo(const UserInfo *user_info)
{
	if (user_info != NULL && user_info_ != NULL)
	{
		user_info_->name = user_info->name;
		user_info_->password = user_info->password;
		user_info_->privilege = user_info->privilege;
	}
}

void PageAssist::AddShowPageDtatList(int index, EServerType type, wxString section_name,  DWORD section_id, 
												   wxString world_name,   DWORD world_id, wxString ip)
{
	ShowPageData * data = new ShowPageData;
	data->index = index;
	data->type = type;
	data->section_name = section_name;
	data->section_id = section_id;
	data->world_name = world_name;
	data->world_id = world_id;
	data->ip = ip;

	std::list<ShowPageData*>::iterator it = page_data_list_abnormal.begin();
	for (; it!=page_data_list_abnormal.end(); it++)
	{
		ShowPageData* pData = (ShowPageData*)(*it);
		if (pData->type == type && pData->section_id == section_id && pData->world_id == world_id)
		{
			page_data_list_abnormal.erase(it);
			break;
		}
	}
	page_data_list_abnormal.push_back(data);
}

void PageAssist::UpdateShowPageDtatList(SectionInfo* section_info)
{
	if (section_info == NULL)
	{
		LOG("SectionInfo ָ��Ϊ��")
		return;
	}
	std::map<DWORD, GameWorldInfo>& info = section_info->game_world_infos;
	std::map<DWORD, GameWorldInfo>::iterator it2;

	//ȡ��Ȩ��
	EPrivilege ePri = (EPrivilege)(GetUserInfo()->privilege);

	//�������������������б�
	std::list<ShowPageData*>::iterator it = openandcloseserver_page_data_list_.begin();
	for (; it != openandcloseserver_page_data_list_.end(); ++it)
	{
		ShowPageData* data = *it;
		if (data  == NULL)
		{
			LOG("ShowPageData ָ��Ϊ��")
			continue;
		}

		if (data->section_id == section_info->id)
		{
			//������ͬ
			if(data->type == EST_Login)
			{
				//����Login״̬
				int PrevState = data->status;
				data->status = section_info->login_status;
				if (data->status == EWS_InitNotDone || data->status == EWS_SystemError) // ���״̬�����ñ�Ϊ���ϻ���δ��ʼ�����򱨾�
				{
					if (PrevState == EWS_Well)
					{
						// TODO: Add your command handler code here
						MCI_OPEN_PARMS mciOpenParms;
						MCI_PLAY_PARMS PlayParms;
						mciOpenParms.dwCallback=0;
						mciOpenParms.lpstrElementName=_T("error.wav");
						mciOpenParms.wDeviceID=0;
						mciOpenParms.lpstrDeviceType=_T("waveaudio");
						mciOpenParms.lpstrAlias=_T(" ");
						PlayParms.dwCallback=0;
						PlayParms.dwTo=0;
						PlayParms.dwFrom=0;
						mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_TYPE|MCI_OPEN_ELEMENT,(DWORD)(LPVOID)&mciOpenParms);//����Ƶ�豸��
						mciSendCommand(mciOpenParms.wDeviceID,MCI_PLAY,MCI_WAIT,(DWORD)(LPVOID)&PlayParms);//����WAVE�����ļ���
						mciSendCommand(mciOpenParms.wDeviceID,MCI_CLOSE,NULL,NULL);//�ر���Ƶ�豸��
		
					}
				}
				if (ePri == EP_Pri_Rt_A || ePri == EP_Pri_Rt_C)
				{
					data->online_number = section_info->nPlayerOnline;
					data->max_online = section_info->nMaxPlayerNum;
				}
			}
			else if (data->type == EST_World)
			{
				//����World״̬
				it2 = info.find(data->world_id);
				if (it2 != info.end())
				{
					//�ҵ���ͬworld
					GameWorldInfo info = it2->second;
					//����world״̬���������������������
					int PrevState = data->status;
					data->status = info.world_status;
					if (data->status == EWS_InitNotDone || data->status == EWS_SystemError) // ���״̬�����ñ�Ϊ���ϻ���δ��ʼ�����򱨾�
					{
						if (PrevState == EWS_Well)
						{
							MCI_OPEN_PARMS mciOpenParms;
							MCI_PLAY_PARMS PlayParms;
							mciOpenParms.dwCallback=0;
							mciOpenParms.lpstrElementName=_T("error.wav");
							mciOpenParms.wDeviceID=0;
							mciOpenParms.lpstrDeviceType=_T("waveaudio");
							mciOpenParms.lpstrAlias=_T(" ");
							PlayParms.dwCallback=0;
							PlayParms.dwTo=0;
							PlayParms.dwFrom=0;
							mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_TYPE|MCI_OPEN_ELEMENT,(DWORD)(LPVOID)&mciOpenParms);//����Ƶ�豸��
							mciSendCommand(mciOpenParms.wDeviceID,MCI_PLAY,MCI_WAIT,(DWORD)(LPVOID)&PlayParms);//����WAVE�����ļ���
							mciSendCommand(mciOpenParms.wDeviceID,MCI_CLOSE,NULL,NULL);//�ر���Ƶ�豸��
						}
					}
					if (ePri == EP_Pri_Rt_A || ePri == EP_Pri_Rt_C)
					{
						data->online_number = info.nPlayerOnline;
						data->max_online = info.nMaxPlayerNum;
					}
				}
				else
				{
					LOG("world " << data->world_name <<"û�ҵ�")
					continue;
				}
			}
			else if (data->type == EST_DB)
			{
				//����DB״̬
				it2 = info.find(data->world_id);
				if (it2 != info.end())
				{
					//�ҵ���ͬworld
					GameWorldInfo info = it2->second;
					//����db״̬
					int PrevState = data->status;
					data->status = info.db_status;
					if (data->status == EWS_InitNotDone || data->status == EWS_SystemError) // ���״̬�����ñ�Ϊ���ϻ���δ��ʼ�����򱨾�
					{
						if (PrevState == EWS_Well)
						{
							MCI_OPEN_PARMS mciOpenParms;
							MCI_PLAY_PARMS PlayParms;
							mciOpenParms.dwCallback=0;
							mciOpenParms.lpstrElementName=_T("error.wav");
							mciOpenParms.wDeviceID=0;
							mciOpenParms.lpstrDeviceType=_T("waveaudio");
							mciOpenParms.lpstrAlias=_T(" ");
							PlayParms.dwCallback=0;
							PlayParms.dwTo=0;
							PlayParms.dwFrom=0;
							mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_TYPE|MCI_OPEN_ELEMENT,(DWORD)(LPVOID)&mciOpenParms);//����Ƶ�豸��
							mciSendCommand(mciOpenParms.wDeviceID,MCI_PLAY,MCI_WAIT,(DWORD)(LPVOID)&PlayParms);//����WAVE�����ļ���
							mciSendCommand(mciOpenParms.wDeviceID,MCI_CLOSE,NULL,NULL);//�ر���Ƶ�豸��
						}
					}
				}
				else
				{
					LOG("world " << data->world_name <<"û�ҵ�")
						continue;
				}
			}
			if (data->status != EWS_Well)
			{
				SwitchDatalist(data);
			}
		}
	}

	//���������������������б�
	it = page_data_list_abnormal.begin();
	for (; it != page_data_list_abnormal.end(); ++it)
	{
		ShowPageData* data = *it;
		if (data  == NULL)
		{
			LOG("ShowPageData ָ��Ϊ��")
				continue;
		}

		if (data->section_id == section_info->id)
		{
			//������ͬ
			if(data->type == EST_Login)
			{
				//����Login״̬
				data->status = section_info->login_status;
				if (ePri == EP_Pri_Rt_A || ePri == EP_Pri_Rt_C)
				{
					data->online_number = section_info->nPlayerOnline;
					data->max_online = section_info->nMaxPlayerNum;
				}
			}
			else if (data->type == EST_World)
			{
				//����World״̬
				it2 = info.find(data->world_id);
				if (it2 != info.end())
				{
					//�ҵ���ͬworld
					GameWorldInfo info = it2->second;
					//����world״̬���������������������
					data->status = info.world_status;
					if (ePri == EP_Pri_Rt_A || ePri == EP_Pri_Rt_C)
					{
						data->online_number = info.nPlayerOnline;
						data->max_online = info.nMaxPlayerNum;
					}
				}
				else
				{
					LOG("world " << data->world_name <<"û�ҵ�")
						continue;
				}
			}
			else if (data->type == EST_DB)
			{
				//����DB״̬
				it2 = info.find(data->world_id);
				if (it2 != info.end())
				{
					//�ҵ���ͬworld
					GameWorldInfo info = it2->second;
					//����db״̬
					data->status = info.db_status;
				}
				else
				{
					LOG("world " << data->world_name <<"û�ҵ�")
						continue;
				}
			}
			if (data->status == EWS_Well)
			{
				SwitchDatalist(data);
			}
		}
	}

	ResortDatalist();
}

//void PageAssist::SendStartServerMessage(ShowPageData* data, BOOL alarm)
//{
//	if (!data) return;
//
//	if (data->status != EWS_InitNotDone)
//		return;
//
//	if (alarm)
//	{
//		wxString caption = w_ConfirmOpen;
//		wxString content = wxT("");
//		content << w_AskForOpenQ << data->section_name << wxT("-") << data->world_name << wxT("-");
//
//		switch (data->type)
//		{
//		case EST_Login:
//			content << wxT("LoginServer");
//			break;
//
//		case EST_DB:
//			content << wxT("DBServer");
//			break;
//
//		case EST_World:
//			content << wxT("WorldServer");
//			break;
//		}
//		content << wxT("?");
//
//		if (wxMessageBox(content, caption, wxYES_NO|wxICON_QUESTION) == wxYES)
//		{
//			sGameServerMgr.SendStartServerMessage(data->section_id, data->world_id, data->type);
//		}
//
//	}
//	else
//	{
//		sGameServerMgr.SendStartServerMessage(data->section_id, data->world_id, data->type);
//	}
//}	
//
//void PageAssist::SendCloseServerMessage(BOOL is_force, ShowPageData* data, BOOL alarm)
//{
//	if (!data) return;
//
//	if (data->status == EWS_InitNotDone)
//		return;
//
//	if (alarm)
//	{
//		wxString caption = w_ConfirmClose;
//		wxString content = wxT("");
//
//		content << w_AskForCloseQ;
//		if (is_force)
//		{
//			content << w_Forced;
//		}
//		content << w_DoClose << data->section_name << wxT("-") << data->world_name << wxT("-");
//
//		switch (data->type)
//		{
//		case EST_Login:
//			content << wxT("LoginServer");
//			break;
//
//		case EST_DB:
//			content << wxT("DBServer");
//			break;
//
//		case EST_World:
//			content << wxT("WorldServer");
//			break;
//		}
//		content << wxT("?");
//
//		if (wxMessageBox(content, caption, wxYES_NO|wxICON_QUESTION) == wxYES)
//		{
//			sGameServerMgr.SendCloseServerMessage(is_force, data->section_id, data->world_id, data->type);
//		}
//	}
//	else
//	{
//		sGameServerMgr.SendCloseServerMessage(is_force, data->section_id, data->world_id, data->type);
//	}
//}

void PageAssist::SwitchDatalist( ShowPageData* server_data )
{
	temp_moving_data_list.push_back(server_data);

	bListChange = TRUE;
}

void PageAssist::ResortDatalist()
{
	if (!bListChange)
	{
		return;
	}

	std::list<ShowPageData*>::iterator it = temp_moving_data_list.begin();
	for (; it != temp_moving_data_list.end(); it++)
	{
		ShowPageData* data = *it;
		if (data->status == EWS_Well)
		{
			openandcloseserver_page_data_list_.push_back(data);
			page_data_list_abnormal.remove(data);
		}
		else
		{
			page_data_list_abnormal.push_back(data);
			openandcloseserver_page_data_list_.remove(data);
		}
	}

	openandcloseserver_page_data_list_.sort(CmpData());
	page_data_list_abnormal.sort(CmpData());

	temp_moving_data_list.clear();
	bListChange = FALSE;
}

//void PageAssist::SendCreateGMMessage( LPCTSTR szName, LPCTSTR szPwd, EPrivilege ePrivilege )
//{
//	sGameServerMgr.SendCreateGMMessage(szName, Crc32(szPwd), ePrivilege);
//}
//
//void PageAssist::SendModifyGMMessage( LPCTSTR szName, DWORD dwIP )
//{
//	sGameServerMgr.SendModifyGMMessage(szName, dwIP);
//}
//
//void PageAssist::SendDeleteGMMessage( LPCTSTR szName )
//{
//	sGameServerMgr.SendDeleteGMMessage(szName);
//}
//
//void PageAssist::SendQueryGMMessage()
//{
//	sGameServerMgr.SendQueryGMMessage();
//}
//
//void PageAssist::SendSystemCmdMessage(ShowPageData* data, BOOL alarm, LPCTSTR szCmd)
//{
//	if (!data) return;
//
//	//if (data->status != EWS_Well)
//	//	return;
//
//	if (alarm)
//	{
//		wxString caption = w_ConfirmOpen;
//		wxString content = wxT("");
//		content << w_AskForSendQ << data->section_name << wxT("-") << data->world_name << wxT("-");
//
//		switch (data->type)
//		{
//		case EST_Login:
//			content << wxT("LoginServer");
//			break;
//
//		case EST_World:
//			content << wxT("WorldServer");
//			break;
//		
//		case EST_DB:
//			content << wxT("DBServer");
//			break;
//
//		default:
//
//			return;
//		}
//		content << wxT("?");
//
//		if (wxMessageBox(content, caption, wxYES_NO|wxICON_QUESTION) == wxYES)
//		{
//			sGameServerMgr.SendSystemCmdMessage(data->section_id, data->world_id, data->type, szCmd);
//		}
//
//	}
//	else
//	{
//		sGameServerMgr.SendSystemCmdMessage(data->section_id, data->world_id, data->type, szCmd);
//	}
//}
//
//BOOL PageAssist::SendChangeMaxMessage(ShowPageData *data, BOOL alarm, LPCTSTR szCmd)
//{
//	if (!data) return FALSE;
//
//	if (data->status != EWS_Well)
//		return FALSE;
//
//	
//	if( data->type != EST_World)
//	{
//		wxString caption = w_Warning4RType;
//		wxString content = wxT("");
//		if ( data->type == EST_Login ) 
//			content << w_ErrorType << data->section_name << wxT("-Login") ;
//		else if ( data->type == EST_DB )
//			content << w_ErrorType << data->section_name << wxT("-") << data->world_name << wxT("-DB") ;
//		wxMessageBox(content, caption, wxYES_NO|wxICON_QUESTION);
//
//		return TRUE;
//	}
//
//	wxString caption = w_ConfirmChangeMax;
//	if (wxMessageBox(w_AskForChangeMax, caption, wxYES_NO|wxICON_WARNING) != wxYES)
//		return TRUE;
//
//	if (alarm)
//	{
//		wxString caption = w_ConfirmChangeMax;
//		wxString content = wxT("");
//		content << w_AskForChangeMaxQ << data->section_name << wxT("-") << data->world_name << wxT("-");
//		content << wxT("WorldServer");
//		content << wxT("?");
//
//		if (wxMessageBox(content, caption, wxYES_NO|wxICON_QUESTION) == wxYES)
//		{
//			sGameServerMgr.SendChangeMaxMessage(data->section_id, data->world_id, szCmd);
//		}
//
//	}
//	else
//	{
//		sGameServerMgr.SendChangeMaxMessage(data->section_id, data->world_id, szCmd);
//	}
//
//	return TRUE;
//}
//
//BOOL PageAssist::UpdateMall(ShowPageData* data, BOOL alarm)
//{
//	if (!data) return FALSE;
//
//	if (data->status != EWS_Well)
//		return FALSE;
//
//	if( data->type != EST_World)
//	{
//		wxString caption = w_Warning4RType;
//		wxString content = wxT("");
//		if ( data->type == EST_Login ) 
//			content << w_ErrorType << data->section_name << wxT("-Login") ;
//		else if ( data->type == EST_DB )
//			content << w_ErrorType << data->section_name << wxT("-") << data->world_name << wxT("-DB") ;
//		wxMessageBox(content, caption, wxYES_NO|wxICON_QUESTION);
//
//		return TRUE;
//	}
//
//	wxString caption = w_ConfirmUpdateMall;
//	if (wxMessageBox(w_AskForUpdateMall, caption, wxYES_NO|wxICON_WARNING) != wxYES)
//		return TRUE;
//
//	if (alarm)
//	{
//		wxString caption = w_ConfirmUpdateMall;
//		wxString content = wxT("");
//		content << w_AskForUpdateMallQ << data->section_name << wxT("-") << data->world_name << wxT("-");
//		content << wxT("WorldServer");
//		content << wxT("?");
//
//		if (wxMessageBox(content, caption, wxYES_NO|wxICON_QUESTION) == wxYES)
//		{
//			sGameServerMgr.SendUpdateMallMessage(data->section_id, data->world_id);
//		}
//
//	}
//	else
//	{
//		sGameServerMgr.SendUpdateMallMessage(data->section_id, data->world_id);
//	}
//
//	return TRUE;
//}