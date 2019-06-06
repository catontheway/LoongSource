#include "StdAfx.h"
#include "SumShower.h"
#include "RTServer.h"
#include "PageAssist.h"
#include "net_command_manager.h"
#include "data_define.h"
#include "UIWords.h"

#include "../ServerDefine/msg_rt_c.h"
#include "../ServerDefine/msg_rt_s.h"
#include "../ServerDefine/gm_tool_define.h"
#include "../ServerDefine/msg_gm_tool_c.h"

RTServer::RTServer() : page_assist_(0),net_(0),is_first_connect_(false),is_init_success(false)
{
	LOG("RTServer����")
}

RTServer::~RTServer()
{
	LOG("RTServer����")
	Destroy();
}

bool RTServer::Init()
{
	//����
	if (net_ == NULL)
	{
		net_ = new StreamTransport;
	}
	if (net_->Init(FALSE) == TRUE)
	{
		LOG("RTServer�����ʼ���ɹ�")
	}
	else
	{
		LOG("RTServer�����ʼ��ʧ��")
	}

	dwLatestUpdateTime_ = GetCurrentDWORDTime();
	
	//ע����Ϣ
	RegiserHandleMessage();
	return true;
}

void RTServer::Destroy()
{
	//ȡ��ע��������Ϣ
	if (net_ != NULL)
	{
		//��������
		net_->Destroy();
		delete net_;
		net_ = NULL;
	}
	net_command_manager_.UnRegisterAll();
}

BOOL RTServer::IsConnected()
{
	if (net_ != NULL)
	{
		return net_->IsConnected();
	}
	else
	{
		return FALSE;
	}
}

BOOL RTServer::IsTryingToConnent()
{
	if (net_ != NULL)
	{
		return net_->IsTryingToConnect();
	}
	else
	{
		return FALSE;
	}
}

void RTServer::Connected()
{
	if (net_ != NULL)
	{
		if(net_->IsConnected() != TRUE)
		{
			if (net_->IsTryingToConnect() == FALSE)
			{
				net_->TryToConnect(ip_.c_str(), port_);
			}
		}
	}
}

void RTServer::SendMsg(LPVOID msg, DWORD size)
{
	if (msg == NULL )
	{
		LOG("msg pointer is null...")
		return;
	}
	if (size < 0)
	{
		LOG("msg size is less than 0...")
		return;
	}
	if (size >= 0xFFFFFFFF)
	{
		LOG("msg size is greater than max...")
		return;
	}

	net_->Send(msg, size);
}

void RTServer::RegiserHandleMessage()
{
	//NRTS_LoginService
	net_command_manager_.Register("NRTS_LoginService", &RTServer::HandleLoginService);
	//NRTS_Challenge
	net_command_manager_.Register("NRTS_Challenge", &RTServer::HandleChanllenge);
	//NRTS_Close
	net_command_manager_.Register("NRTS_Close", &RTServer::HandleCloseServer);
	//NRTS_Start
	net_command_manager_.Register("NRTS_Start", &RTServer::HandleStartServer);
	//NRTS_AllServerStatInfo
	net_command_manager_.Register("NRTS_AllServerStatInfo", &RTServer::HandleAllServerStatInfo);
	//NRTS_AllServerDynaInfo
	net_command_manager_.Register("NRTS_AllServerDynaInfo", &RTServer::HandleAllServerDynaInfo);
	//NRTS_PrivilegeInfo
	net_command_manager_.Register("NRTS_PrivilegeInfo", &RTServer::HandlePrivilegeInfo);

	//NGMTS_GMCreate
	net_command_manager_.Register("NGMTS_GMCreate", &RTServer::HandleGMCreateResult);
	//NGMTC_GMModify
	net_command_manager_.Register("NRTS_BindIP", &RTServer::HandleGMModifyResult);
	//NGMTC_GMDelete
	net_command_manager_.Register("NGMTS_GMDelete", &RTServer::HandleGMDeleteResult);
}

DWORD RTServer::HandleLoginService(tagNetCmd *command)
{
	tagNRTS_LoginService *recv = (tagNRTS_LoginService*)command;
	if (recv->dwErrorCode != 0)
	{
		LOG("��¼RTServerʧ��, ������: " << recv->dwErrorCode)
	}
	else
	{
		LOG("��¼RTServer�ɹ�")
		
		//���� ������Ϣ NRTS_ServerInfo
		UserInfo* user_info = NULL;
		user_info = page_assist_->GetUserInfo();
		if (user_info != NULL)
		{
			tagNRTC_Challenge send;
			wxStringToTCHAR(user_info->name,  send.szAccountName,  X_SHORT_NAME);
			TCHAR buff[X_SHORT_NAME];
			wxStringToTCHAR(user_info->password,  buff,  X_SHORT_NAME);
			send.dwPsdCrc = Crc32(buff);
			SendMsg(&send, send.dwSize);
			LOG("Send NRTS_Challenge...")
		}
	}
	return 0;
}
DWORD RTServer::HandleChanllenge(tagNetCmd *command)
{
	tagNRTS_Challenge *recv = (tagNRTS_Challenge*)command;

	switch (recv->dwErrorCode)
	{
	case E_GMTool_Success:
		//��֤�ɹ�
		LOG("��¼�ɹ�")
		//����Ȩ��
		SetPrivilege(recv->ePrivilege);
		// �л�����
		g_pLoginDialog->Hide();
		g_pLoginDialog->Clear();
		g_pMainFrame->GotoFirstPage();
		g_pMainFrame->CenterOnScreen();
		g_pMainFrame->Show();
		break;

	case E_RTClient_Ip_Err:
		wxMessageBox(w_BadIP, w_Error);
		break;
		
	default:
		wxMessageBox(w_LoginFailed, w_Error);
		break;
	}

	return 0;
}

DWORD RTServer::HandleCloseServer(tagNetCmd *command)
{
	tagNRTS_Close *recv = (tagNRTS_Close*)command;
	if (recv->dwErrorCode == 0)
	{
		//�رշ������ɹ�
		LOG("�رշ������ɹ�")
	}
	else
	{
		wxMessageBox(w_Error, w_CloseFailed);
	}
	return 0;
}

DWORD RTServer::HandleStartServer(tagNetCmd *command)
{
	tagNRTS_Start *recv = (tagNRTS_Start*)command;
	if (recv->dwErrorCode == 0)
	{
		//�����������ɹ�
		LOG("�����������ɹ�")
	}
	else
	{
		wxMessageBox(w_Error, w_StartFailed);
	}
	return 0;
}


DWORD RTServer::HandleAllServerStatInfo(tagNetCmd *command)
{
	if (page_assist_ == NULL)
	{
		LOG("page_assist_ ָ��Ϊ��")
		return 0;
	}

	tagNRTS_AllServerStatInfo *recv = (tagNRTS_AllServerStatInfo *)command;
	int section_num = recv->nSectionNum;
	LPBYTE tmp = recv->byInfo;

	BOOL bIPValid = TRUE;
	//ȡ��Ȩ��
	EPrivilege ePri = (EPrivilege)(page_assist_->GetUserInfo()->privilege);
	if (ePri == EP_Pri_Rt_C || ePri == EP_Pri_Rt_D)
		bIPValid = FALSE;

	if (section_num > 0)
	{
		int index = 0;
		for (int i = 0; i < section_num; i++)
		{
			SectionInfo section_info;

			TCHAR  buff[X_SHORT_NAME] = {0};
			CHAR    ip[X_SHORT_NAME] = {0};
			//������
			memcpy(buff, tmp, X_SHORT_NAME*sizeof(TCHAR) );
			section_info.section_name = buff;
			tmp +=X_SHORT_NAME*sizeof(TCHAR);

			//����ID
			section_info.id = *(DWORD*)tmp;
			tmp +=sizeof(DWORD);

			//LoginIP
			if(bIPValid)
				memcpy(ip, tmp, X_SHORT_NAME*sizeof(CHAR) );
			section_info.login_ip = ip;
			tmp +=X_SHORT_NAME*sizeof(CHAR);
			//���һ����������¼����
			page_assist_->AddShowPageDtatList(index++, EST_Login, section_info.section_name, section_info.id, wxT(""), 0,
				section_info.login_ip);

			//world����
			DWORD num = *(DWORD*)tmp;
			tmp +=sizeof(DWORD);
			for (int i = 0; i < (int)num;  i++)
			{
				//��Ϸ��������
				GameWorldInfo info;
				memcpy(buff, tmp, X_SHORT_NAME*sizeof(TCHAR) );
				info.world_name = buff;
				tmp +=X_SHORT_NAME*sizeof(TCHAR);

				//��Ϸ����ID
				info.id = *(DWORD*)tmp;
				tmp += sizeof(DWORD);

				//WorldIP
				if(bIPValid)
					memcpy(ip, tmp, X_SHORT_NAME*sizeof(CHAR) );
				info.world_ip = ip;
				tmp +=X_SHORT_NAME*sizeof(CHAR);
				page_assist_->AddShowPageDtatList(index++, EST_World, section_info.section_name,  section_info.id, info.world_name,
					info.id, info.world_ip);

				//DBIP
				if(bIPValid)
					memcpy(ip, tmp, X_SHORT_NAME*sizeof(CHAR) );
				info.db_ip = ip;
				tmp +=X_SHORT_NAME*sizeof(CHAR);
				section_info.game_world_infos.insert(std::pair<DWORD, GameWorldInfo>(info.id, info));
				page_assist_->AddShowPageDtatList(index++, EST_DB, section_info.section_name,  section_info.id, info.world_name,
					info.id, info.db_ip);
			}
		}
		page_assist_->UpdateShowPage(TRUE);
		is_init_success = true;
	}
	return 0;
}

DWORD RTServer::HandleAllServerDynaInfo(tagNetCmd *command)
{
	if (page_assist_ == NULL)
	{
		LOG("page_assist_ ָ��Ϊ��")
			return 0;
	}
	
	// ��������յ�update��Ϣ��ʱ��
	dwLatestUpdateTime_ = GetCurrentDWORDTime();

	tagNRTS_AllServerDynaInfo *recv = (tagNRTS_AllServerDynaInfo *)command;
	int section_num = recv->nSectionNum;
	LPBYTE tmp = recv->byInfo;
	if (section_num > 0)
	{
		 for (int i = 0; i < section_num; i++)
		 {
			 SectionInfo section_info;
			 //����ID
			 section_info.id = *(DWORD*)tmp;
			 tmp += sizeof(DWORD);
			 //Login״̬
			 tagServerInfo server_info = *(tagServerInfo*)tmp;
			 section_info.login_status = server_info.eStatus;
			 section_info.nPlayerOnline = server_info.nPlayerOnline;
			 section_info.nMaxPlayerNum = server_info.nMaxPlayerNum;
			 tmp += sizeof(tagServerInfo);
			 //world ����
			DWORD world_num = *(DWORD*)tmp;
			tmp += sizeof(DWORD);

			for (int i = 0; i < (int)world_num;  i++)
			{
				//GameWorld ��̬��Ϣ
				GameWorldInfo info;

				//world ID
				DWORD world_id = *(DWORD*)tmp;
				info.id = world_id;
				tmp += sizeof(DWORD);
				
				//world info
				tagServerInfo world_info = *(tagServerInfo*)tmp;
				info.world_status = world_info.eStatus;
				info.nPlayerOnline = world_info.nPlayerOnline;
				info.nMaxPlayerNum = world_info.nMaxPlayerNum;

				tmp += sizeof(tagServerInfo);

				//DB info
				tagServerInfo db_info = *(tagServerInfo*)tmp;
				info.db_status = db_info.eStatus;
				tmp += sizeof(tagServerInfo);

				section_info.game_world_infos.insert(std::pair<DWORD, GameWorldInfo>(info.id, info));
			}

			//���´�����Ϣ
			page_assist_->UpdateShowPageDtatList(&section_info);
		 }
		 page_assist_->UpdateShowPage(TRUE);
	}
	return 0;
}

DWORD RTServer::HandlePrivilegeInfo(tagNetCmd *command)
{
	tagNRTS_PrivilegeInfo *recv = (tagNRTS_PrivilegeInfo*)command;
	int num = recv->nNum;
	if (num > 0)
	{
		DWORD *tmp = recv->dwInfo;
		DWORD msg_id , privilege; 
		for (int i = 0; i < num; i++)
		{
			msg_id = *tmp;
			tmp++;
			privilege = *tmp;
			tmp++;
			privilege_.insert(std::pair<DWORD, DWORD>(msg_id, privilege));
		}
	}

	// ��ʾ����
	//g_pMainFrame->ShowGMMgrPage( VerifyGMPrivilege(Crc32("NGMTC_GMCreate"), GetPrivilege()) && VerifyGMPrivilege(Crc32("NGMTC_GMDelete"), GetPrivilege()) && VerifyGMPrivilege(Crc32("NRTC_BindIP"), GetPrivilege()));

	/*g_pMainFrame->ShowPage( VerifyGMPrivilege(Crc32("NRTC_Start"), GetPrivilege()) && VerifyGMPrivilege(Crc32("NRTC_Close"), GetPrivilege()), 
		VerifyGMPrivilege(Crc32("NRTC_SystemCmd"), GetPrivilege()),
		VerifyGMPrivilege(Crc32("NRTC_MaxNum"), GetPrivilege()),
		VerifyGMPrivilege(Crc32("NRTC_UpdateMall"), GetPrivilege()));*/
	return 0;
}

// GM������Ϣ��Ӧ
DWORD RTServer::HandleGMCreateResult( tagNetCmd* command )
{
	tagNGMTS_GMCreate* pRecv = (tagNGMTS_GMCreate*)command;

	switch(pRecv->dwErrorCode)
	{
	case E_GMTool_Success:
		wxMessageBox(w_CreateSuccessfully, wxT(""), wxOK|wxCENTRE);
		break;

	case E_GMTool_GM_AlreadyExist:
		wxMessageBox(w_AccoutUsed, wxT(""), wxOK|wxCENTRE);
		break;

	case E_GMTool_Err:
	case E_GMTool_GM_CreateFail:
		wxMessageBox(w_CreateFailed, wxT(""), wxOK|wxCENTRE);
		break;
	}

	return 0;
}

DWORD RTServer::HandleGMModifyResult( tagNetCmd* command )
{
	tagNRTS_BindIP* pRecv = (tagNRTS_BindIP*)command;

	switch(pRecv->dwErrorCode)
	{
	case E_GMTool_Success:
		wxMessageBox(w_BindIPSuccess, wxT(""), wxOK|wxCENTRE);
		break;

	case E_GMTool_GM_NotExist:
		wxMessageBox(w_GMNotExist, wxT(""), wxOK|wxCENTRE);
		break;

	case E_GMTool_Err:
		wxMessageBox(w_BindIPFailed, wxT(""), wxOK|wxCENTRE);
		break;
	}

	return 0;
}

DWORD RTServer::HandleGMDeleteResult( tagNetCmd* command )
{
	tagNGMTS_GMDelete* pRecv = (tagNGMTS_GMDelete*)command;

	switch(pRecv->dwErrorCode)
	{
	case E_GMTool_Success:
		wxMessageBox(w_DelSuccessfully, wxT(""), wxOK|wxCENTRE);
		break;

	case E_GMTool_GM_NotExist:
		wxMessageBox(w_GMNotExist, wxT(""), wxOK|wxCENTRE);
		break;

	case E_GMTool_Err:
		wxMessageBox(w_DelFailed, wxT(""), wxOK|wxCENTRE);
		break;
	}

	return 0;
}

void RTServer::RecvMessage()
{
	//��ȡ��Ϣ
	while(true)
	{
		DWORD msg_total_size = 0;
		LPBYTE recv = net_->Recv(msg_total_size);
		if (!P_VALID(recv))
		{
			tagDWORDTime dwNow = GetCurrentDWORDTime();
			// ָ����ʱ�������û���յ�����GMserver�Ķ�̬��Ϸ��������Ϣ
			if (IncreaseTime(dwLatestUpdateTime_, MAX_WAIT_TIME) <= dwNow)
			{
				// ��ʾGM�������ص�¼����
				if (g_pMainFrame->IsShownOnScreen())
				{		
					wxMessageBox(w_BadConnection, w_Error);
					this->Disconnect();
					g_pMainFrame->GetPageAssistPtr()->ClearShowPageDtatList();
					g_pMainFrame->Hide();
					g_pLoginDialog->CenterOnScreen();
					g_pLoginDialog->Show();
				}
			}

			break;
		}
		
		LPBYTE next_msg = recv;
		DWORD msg_size = 0;

		while (next_msg  < recv + msg_total_size )
		{
			tagNetCmd *command = (tagNetCmd*)next_msg;
			msg_size = command->dwSize;
			net_command_manager_.HandleCommand(command, msg_size, this);
			next_msg += msg_size;
		}
		net_->FreeRecved(recv);
	}
}

bool RTServer::VerifyGMPrivilege( DWORD dwMsgCrc, EPrivilege ePrivilege )
{
	EPrivilege ePri = GetPrivilege();
	if (!MIS_RTCLIENT_PRI(ePri))
	{
		return false;
	}

	if (ePrivilege != EP_Null && ePri != ePrivilege)
	{
		return false;
	}

	std::map<DWORD, DWORD>::iterator it = privilege_.find(dwMsgCrc);
	if( it != privilege_.end() )
	{
		if( it->second & ( 1 << ePri ) )
			return true;
		else 
			return false;
	}

	return false;
}

DWORD RTServer::IsIPValid( LPSTR szIP )
{
	if (!net_)	return INADDR_NONE;

	return net_->StringIP2IP(szIP);
}

void RTServer::Disconnect()
{
	if (net_)
	{
		net_->Disconnect();
	}
}

void RTServer::SetPrivilege( EPrivilege ePri )
{
	if (page_assist_ != NULL)
	{
		UserInfo* pUserInfo = page_assist_->GetUserInfo();
		if (!pUserInfo)
		{
			return;
		}
		pUserInfo->privilege = ePri;
		page_assist_->SetUserInfo(pUserInfo);
	}
}

EPrivilege RTServer::GetPrivilege()
{
	if (page_assist_ != NULL)
	{
		UserInfo* pUserInfo = page_assist_->GetUserInfo();
		if (!pUserInfo)
		{
			return EP_Null;
		}

		return (EPrivilege)(pUserInfo->privilege);
	}

	return EP_Null;
}