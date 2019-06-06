//-----------------------------------------------------------------------------
//!\file	client_db.h
//!\brief	�ͻ������ݿ������
//!
//!\date	2009-04-20
//! last	2009-04-20
//!\author	zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/base_define.h"

#define PRIVILEGE_TABLE_NAME	""
const INT nPrivilegeCount = 3;

//Log�ṹ
enum EActionType
{
	EAT_Null,

	EAT_CreateGM = 1,			// ����GM����
	EAT_ModifyGM,				// ����GM�޸�
	EAT_DeleteGM,				// ����GMɾ��

	EAT_ResumeRole,				// ��ɫ�ָ�
	EAT_ProModify,				// �����޸�

	EAT_ItemCreate,				// ��������
	EAT_ItemDelete,				// ����ɾ��
	EAT_ItemResume,				// ���߻ָ�

	EAT_DoubleStart,			// ����˫��
	EAT_DoubleEnd,				// ˫��ɾ��

	EAT_AutoNoticeStart,		// �Զ�ѭ������
	EAT_AutoNoticeEnd,			// �Զ�ѭ������ɾ��

	EAT_NoticeStart,			// ���½ǹ���
	EAT_NoticeEnd,				// ���½ǹ���ɾ��

	EAT_AutoChatStart,			// ����Ƶ������
	EAT_AutoChatEnd,			// ����Ƶ������ɾ��

	EAT_StartServer,			// ����������
	EAT_CloseServer,			// �رշ�����
	EAT_KillServer,				// ǿ�ƹرշ�����

	EAT_IpBind,					// ��IP

	EAT_SendSystemCmd,			// ����ϵͳ��Ϣ

	EAT_End
};

#define IsActionValid(x) ((x)>EAT_Null && (x)<EAT_End)

struct tagLogRecord 
{
	DWORD		dwClientID;
	DWORD		dwTime;
	EActionType	eActionType;
	TCHAR		szName[X_SHORT_NAME];
	TCHAR		szIP[X_SHORT_NAME];
	TCHAR		szDetails[X_LONG_STRING];
};

//-----------------------------------------------------------------------------
// �ͻ������ݿ������
//-----------------------------------------------------------------------------
class ClientDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ClientDB();
	~ClientDB() {}

	//-------------------------------------------------------------------------
	// ��ʼ��������
	//-------------------------------------------------------------------------
	BOOL	Init(LPCTSTR strHost, LPCTSTR strUser, LPCTSTR strPassword, LPCTSTR strDatabase, INT nPort);
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// ���ݿ���ش���
	//-------------------------------------------------------------------------
	BOOL	DBQueryClientData(LPCTSTR szName, DWORD dwPsdCrc, DWORD& dwClientID, INT& nPrivilege);
	BOOL	DBCheckIPBind(LPCTSTR szName, DWORD dwClientIP);
	BOOL	DBCreateGameMaster(LPCTSTR szName, DWORD dwPsdCrc, INT nPrivilege);
	BOOL	DBDeleteGameMaster(LPCTSTR szName);
	BOOL	DBChangeGMPrivilege(LPCTSTR szName, INT nNewPrivilege);
	BOOL	DBQueryPrivilegeInfo(TMap<DWORD, DWORD>& mapPrivilege);
	BOOL	DBQueryAccountPrivilege(LPCTSTR szName, INT& nPrivilege);
	BOOL	DBGMExist(LPCTSTR szName);

	BOOL	DBChangeBindIp(LPCTSTR szName, DWORD dwIP);
	BOOL	DBQueryBindIp(LPCTSTR szName, DWORD& dwIP);
	BOOL	DBReplaceBindIp(LPCTSTR szName, DWORD dwIP);

	BOOL	DBLogGMAction(DWORD dwAccountID, LPCTSTR szName, LPCTSTR szIP, EActionType eActionType, LPCTSTR szDetails);

private:
	DataBase		m_DB;			// Mysql���ݿ�
	
};