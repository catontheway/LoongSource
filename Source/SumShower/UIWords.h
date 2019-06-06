#pragma once

//---------------------------------------------------------------------------/
//									 ���İ�									 /
//---------------------------------------------------------------------------/

#ifdef CHINESE_VERSION

#define w_UserName			wxT("�û���:")
#define w_Password			wxT(" ����  :")
#define w_Login				wxT("��½")
#define w_Logout			wxT("�˳�")
#define w_Privilege			wxT("Ȩ��")
#define w_PlsWait			wxT("�����У����Ժ򡭡�")
#define w_ConnectFailed		wxT("����ʧ��")
#define w_RemoteTool		wxT("��ά����")
#define w_OpenCloseServer	wxT("���ط�����")
#define w_GMManager			wxT("GM����")
#define w_GoodServerGroup	wxT("������������")
#define w_BadServerGroup	wxT("�쳣��������")
#define w_OpenCloseSomeServer	wxT("�������ط�")
#define w_OpenSomeServer	wxT("��������")
#define w_CloseSomeServer	wxT("�����ط�")
#define w_Select			"ѡ��"
#define w_ServerName		wxT("����������")
#define w_Section			wxT("��Ϸ����")
#define w_GameWorld			wxT("��Ϸ����")
#define w_ServerState		wxT("״̬")
#define w_Actions			wxT("����")
#define w_Online			wxT("��������")
#define w_MaxOnline			wxT("��������")
#define w_SelectAll			wxT("ȫѡ")
#define w_UnSelectAll		wxT("ȡ��ȫѡ")

#define w_CreateGM			wxT("����GM")
#define w_NullAccount		wxT("�˺Ų���Ϊ��")
#define w_NullPsd			wxT("���벻��Ϊ��")
#define w_NullIP			wxT("��IP����Ϊ��")
#define w_InvalidIP			wxT("�������IP���Ϸ�")
#define w_Attention			wxT("����")
#define w_NoAuthCreateGM	wxT("����ǰȨ���޷�����GM")
#define w_NoAuthDeleteGM	wxT("����ǰȨ���޷�ɾ��GM")
#define w_NoAuthBindIP		wxT("����ǰȨ���޷����GM��IP")
#define	w_NoAuthQuery		wxT("����ǰȨ���޷����в�ѯ")
#define	w_NoAuthOpenSome	wxT("����ǰȨ���޷�ʹ�ö�����������")
#define	w_NoAuthCloseSome	wxT("����ǰȨ���޷�ʹ�ö����ط�����")
#define	w_AskForOpen		wxT("��ȷ��Ҫ����������?")
#define	w_AskForClose		wxT("��ȷ��Ҫ�����ط���?")
#define w_ChangeBindIP		wxT("�޸İ�IP")
#define w_DeleteGM			wxT("ɾ��GM")
#define w_GMAccount			wxT("GM�˺�")
#define w_LoginPsd			wxT("��½����")
#define w_IPBinded			wxT("��IP")
#define w_CreateSuccessfully	wxT("����GM�ɹ�")
#define w_CreateFailed		wxT("����GMʧ��")
#define w_DelSuccessfully	wxT("ɾ��GM�ɹ�")
#define w_DelFailed			wxT("ɾ��GMʧ��")
#define w_AccoutUsed		wxT("�˺��Ѵ���")
#define w_BadIP				wxT("��IP��֤ʧ��")
#define w_BindIPSuccess		wxT("�޸İ�IP�ɹ�")
#define w_BindIPFailed		wxT("�޸İ�IPʧ��")
#define w_GMNotExist		wxT("��GM������")

#define w_Error				wxT("����")
#define w_LoginFailed		wxT("��½ʧ��")
#define w_BadConnection		wxT("��GMServer�����ӶϿ�")


#define w_NoInited			"δ��ʼ�����"
#define w_BrokenDown		"ϵͳ���ֹ���"
#define w_WorkNormally		"����"
#define w_ProofError		"��֤����������"

#define w_Start				wxT("����")
#define w_Close				wxT("�ر�")
#define w_CloseForced		wxT("ǿ�ƹر�")
#define w_CloseSuccessfully	wxT("�رշ������ɹ�")
#define w_CloseFailed		wxT("�رշ�����ʧ��")
#define w_StartSuccessfully	wxT("�����������ɹ�")
#define w_StartFailed		wxT("����������ʧ��")

#define w_ConfirmOpen		wxT("����ȷ��");
#define	w_AskForOpen		wxT("��ȷ��Ҫ����")
#define w_ConfirmClose		wxT("�ط�ȷ��");
#define	w_AskForClose		wxT("��ȷ��Ҫ�ر�")
#define	w_Forced			wxT("ǿ��")
#define	w_DoClose			wxT("�ر�")

#endif

//---------------------------------------------------------------------------/
//									English									 /
//---------------------------------------------------------------------------/

#define w_UserName			wxT("Account: ")
#define w_Password			wxT("Password:")
#define w_Login				wxT("Sign in")
#define w_Logout			wxT("Sign out")
#define w_Privilege			wxT("Privilege")
#define w_PlsWait			wxT("Connecting...")
#define w_ConnectFailed		wxT("Connection Failed")
#define w_SumShow			wxT("Online Sum Show")
#define w_OpenCloseServer	wxT("Show Page")
#define w_GMManager			wxT("GM Manager")
#define w_GoodServerGroup	wxT("")
#define w_BadServerGroup	wxT("Abnormal Server Group")
#define w_OpenCloseSomeServer	wxT("Open or Close Selected Servers")
#define w_OpenSomeServer	wxT("Open Selected Servers")
#define w_CloseSomeServer	wxT("Close Selected Servers")
#define w_SetSystemCmd		wxT("Send System Command to Selected Servers")
#define	w_NoAuthSend		wxT("You don't have privilege to send system command")
#define	w_AskForSend		wxT("Are you sure to send command to selected servers?")
#define	w_AskForSendQ		wxT("Are you sure to send command to ")
#define w_SystemCmd			wxT("Command:")
#define w_SendCmd			wxT("Send")
#define w_Select			"Select"
#define w_ServerType		wxT("Server Type")
#define w_Section			wxT("Section")
#define w_GameWorld			wxT("Game World")
#define w_ServerState		wxT("Server Status")
#define w_Actions			wxT("Operations")
#define w_Online			wxT("User Online")
#define w_Sum				wxT("Sum")
#define w_SelectAll			wxT("Select All")
#define w_UnSelectAll		wxT("Cancel Select")

#define w_CreateGM			wxT("Create GM Account")
#define w_NullAccount		wxT("Account field cannot be left blank")
#define w_NullPsd			wxT("Password field cannot be left blank")
#define w_NullIP			wxT("IP field cannot be left blank ")
#define w_InvalidIP			wxT("Illegal IP")
#define w_Attention			wxT("Warning")
#define w_NoAuthCreateGM	wxT("You don't have privilege to create GM account")
#define w_NoAuthDeleteGM	wxT("You don't have privilege to delete GM account")
#define w_NoAuthBindIP		wxT("You don't have privilege to modify IP binded")
#define	w_NoAuthQuery		wxT("You don't have privilege to do query")
#define	w_NoAuthOpenSome	wxT("You don't have privilege to open selected servers")
#define	w_NoAuthCloseSome	wxT("You don't have privilege to close selected servers")
#define	w_AskForOpen		wxT("Are you sure to open selected servers?")
#define	w_AskForClose		wxT("Are you sure to close selected servers?")
#define w_ChangeBindIP		wxT("Modify IP Binding")
#define w_DeleteGM			wxT("Delete GM Account")
#define w_GMAccount			wxT("GM Account")
#define w_LoginPsd			wxT("Password")
#define w_IPBinded			wxT("Binding IP")
#define w_CreateSuccessfully	wxT("Create GM account successfully")
#define w_CreateFailed		wxT("Fail to create GM account")
#define w_DelSuccessfully	wxT("Success to delete")
#define w_DelFailed			wxT("Fail to delete")
#define w_AccoutUsed		wxT("This account already exist")
#define w_BadIP				wxT("Illegal IP")
#define w_BindIPSuccess		wxT("Success to modify IP binding")
#define w_BindIPFailed		wxT("Fail to modify IP binding")
#define w_GMNotExist		wxT("This account doesn't exist")

#define w_Error				wxT("Error")
#define w_LoginFailed		wxT("Fail to Sign in")
#define w_BadConnection		wxT("Disconnected from GMserver")


#define w_NoInited			"Uninitialized"
#define w_BrokenDown		"system failure"
#define w_WorkNormally		"Well"
#define w_ProofError		"ProofError"

#define w_Start				wxT("Open")
#define w_Close				wxT("Close")
#define w_CloseForced		wxT("Force Close")
#define w_SetMaxOnline		wxT("..")
#define w_CloseSuccessfully	wxT("Close Successfully")
#define w_CloseFailed		wxT("Fail to close")
#define w_StartSuccessfully	wxT("Start Successfully")
#define w_StartFailed		wxT("Fail to start")

#define w_ConfirmOpen		wxT("Confirm to open ");
#define	w_AskForOpenQ		wxT("Are you sure to open ")
#define w_ConfirmClose		wxT("Confirm to close ");
#define	w_AskForCloseQ		wxT("Are you sure to ")
#define w_ConfirmSend		wxT("Confirm to send command ");
#define	w_Forced			wxT("passively ")
#define	w_DoClose			wxT("close ")

#define w_MaxNum			wxT("Reset Max User Online:")
#define w_SetMaxOK			wxT("Reset")
#define	w_NoAuthChangeMax	wxT("You don't have privilege to change it")
#define	w_AskForChangeMax	wxT("Are you sure to change max user online?")
#define w_ConfirmChangeMax	wxT("Confirm to change ");
#define	w_AskForChangeMaxQ	wxT("Are you sure to change max user online of ")

#define w_UpdateMall			wxT("Update Mall:")
#define w_UpdateMallOK			wxT("Update")
#define	w_NoAuthUpdataMall		wxT("You don't have privilege to update mall.")
#define	w_ErrorType				wxT("You can't do this on ")
#define	w_AskForUpdateMall		wxT("Are you sure to update mall of selected worlds?")
#define w_DoUpdate				wxT("Update Mall of Selected Worlds.")
#define w_ConfirmUpdateMall		wxT("Confirm to update mall ");
#define w_Warning4RType			wxT("Selected server with unexpected type.");
#define	w_AskForUpdateMallQ		wxT("Are you sure to update mall of ")