#pragma once


class CProcessWrapper
{
public:
	CProcessWrapper(void);

	~CProcessWrapper(void);

public:

	bool		QuitProcess( const TCHAR* szProcessName, BOOL bFullPath = FALSE );

	bool		ShutDownProcess( const TCHAR* szProcessName, BOOL bFullPath = FALSE  );

	bool		ExecuteProcess( const TCHAR* szProcessName );


	//���ҽ��̣����ؽ��̵�ID
	HANDLE		FindProcess( DWORD& dwProcessID,  const TCHAR* szProcessName, BOOL bFullPath = FALSE );

	//����������ؽ��̣����ؽ���ID��Ϣ
	int			FindAllProcess( HANDLE* pProcessHandle, DWORD* dwID,  const TCHAR* szProcessName, BOOL bFullPath = FALSE );

private:

	HANDLE		m_hSnapShot;


};
