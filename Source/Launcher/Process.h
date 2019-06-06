#pragma once



class Process  
{
public:
	BOOL Work(HWND hWndMain);
	
	HttpSocket*	GetHttp() { return m_pHttp; }
	LPCTSTR	GetURL() { return m_strURL; }
	LPCTSTR	GetManualURL() { return m_strManualURL; }
	

	VOID	Terminate();
	DWORD	WaitForEnd(DWORD dwTime) { return ::WaitForSingleObject(m_hThread, dwTime); }

	LPCTSTR GetGameFullName() { return (LPCTSTR)m_strGameExe; }
	LPCTSTR GetAppPath() { return (LPCTSTR)m_strAppPath; }
	LPCTSTR GetCfgFileFullName() { return (LPCTSTR)m_strCfgFileFullName; }

	Process();
	virtual ~Process();

protected:	
	
	// ��ȡlauncher.ini����
	BOOL	GetConfig(LPCTSTR szIniFileName);

	// ����launcher����汾,0:�Ѿ������°棬1:��Ҫ����
	INT		ParseLauncherVersion();
	// ����launcher�汾
	BOOL	RefreshLauncherVersion();

	// ������Ϸ�汾,0:�Ѿ������°棬1:�ҵ��µĲ�����-1:�޷��ҵ����ʵĲ���
	INT		ParseGameVersion();

	// �����߳�
	UINT	WorkThread(void *pArg);

	// �����Լ�
	BOOL	UpdateSelf();

	// ��ѹ��
	BOOL	Unpack(CStringA& strCPKFile);

	//��һ��cpk����ѹ��ָ����Ŀ¼
	BOOL	UnpackCpk(DWORD dwFatherCRC, DWORD& dwUnPackNum, CPKVfs *pReadCpk, CPKVfs *pWriteCpk);
	//��cpk�ļ���ѹ���������ļ�(������);
	BOOL	UnpackSimpleFile(DWORD dwCRC, DWORD& dwUnPackNum, CPKVfs *pReadCPK);
	//��һ��cpk����ѹ���������cpk����Ŀ¼����
	BOOL	UnpackCPKFromCPK(DWORD dwDirCRC, DWORD& dwUnPackNum, CPKVfs *pReadCPK, CPKVfs *pWriteCPK);
	//���°汾��
	BOOL	RefreshGameVersion(LPCTSTR szToVersion);
	// �������ϴ���Ŀ¼
	BOOL	CreateDirToDisc(LPCSTR szPath);
	// �������ϴ����ļ�
	DWORD	CreateFileToDisc(LPCSTR szFileName);
	// �������ļ����õ����ļ���(����·��)
	BOOL	GetSimpleFileName(LPTSTR pSimpleName, LPCTSTR pFullName);
	
	TSFPTrunk<Process>		m_Trunk;
	HttpSocket*				m_pHttp;
	Util*					m_pUtil;
	HWND					m_hWndMain;
	BOOL					m_bConfigLoaded;

	CString					m_strAppPath;
	CString					m_strGameExe;				// ��ϷEXEλ��
	CString					m_strIniFileFullName;		// INI ȫ��
	CString					m_strCfgFileFullName;		// �����ļ�ȫ��(һ��Ϊgamecfg.xml)
	CString					m_strSrcPatchInfo;			// patchinfo.txt������λ��
	CString					m_strDestPatchInfo;			// patchinfo.txt����λ��
	CString					m_strSrcServerInfo;			// serverinfo.txt������λ��
	CString					m_strDestServerInfo;		// serverinfo.txt����λ��
	CString					m_strSrcNewLauncher;		// �°汾launcher������λ��
	CString					m_strDestNewLauncher;		// �°汾launcher����λ��
	CString					m_strLocalLauncherVersion;	// ��ǰlauncher�汾
	CString					m_strLocalGameVersion;		// ��ǰ��Ϸ�汾
	CString					m_strLatestLauncherVersion;	// ����launcher�汾
	CString					m_strLatestGameVersion;		// ������Ϸ�汾

	CString					m_strURL;					// ��������ʾҳ��
	CString					m_strManualURL;				// �ٷ��ֶ���������ҳ��

	CString					m_strSrcGamePatch;			// ����������λ��
	CString					m_strToGameVersion;			// �°汾��
	std::string				m_strGamePatchMD5;			// ����MD5


	DWORD					m_dwTotalFileNum;			// �ܹ����ѹ���ļ�
	volatile	BOOL		m_bTerminateThread;
	HANDLE					m_hThread;
};