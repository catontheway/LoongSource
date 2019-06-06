#include "stdafx.h"
#include "process.h"
#include "launcher.h"

#include "launcherDlg.h"
#include "httpdown.h"
#include "cpkvfs.h"
#include "disciomgr.h"
#include "md5.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "algorithm"

Process::Process():m_Trunk(this)
{
	m_pHttp = new HttpSocket;
	m_hThread = 0;
	m_pUtil = new Util;
	m_bConfigLoaded = this->GetConfig(_T("launcher\\launcher.ini"));
}

Process::~Process()
{
	SAFE_DEL(m_pHttp);
	SAFE_DEL(m_pUtil);
}



//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
BOOL Process::Work(HWND hWndMain)
{
	m_bTerminateThread = FALSE;
	m_hWndMain = hWndMain;
	m_pHttp->SetWndMain(hWndMain);

	SendMessage(hWndMain, WM_USER_GAME_VER, (WPARAM)(LPCTSTR)m_strLocalGameVersion, 0);
	SendMessage(hWndMain, WM_USER_LAUNCHER_VER, (WPARAM)(LPCTSTR)m_strLocalLauncherVersion, 0);

	if( !m_bConfigLoaded )
		return FALSE;

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
		(&Process::WorkThread), 0, 0, NULL);

	return TRUE;
}



//-----------------------------------------------------------------------------
// �����߳�
//-----------------------------------------------------------------------------
UINT Process::WorkThread(void *pArg)
{
	BYTE digest[16];
	std::string strMD5;

	// ��ɾ��ԭ�ȵĸ�����Ϣ�ļ�patchinfo.txt�ͷ�������Ϣ�ļ�
	::DeleteFile(m_strDestPatchInfo);
	::DeleteFile(m_strDestServerInfo);

	// ���ظ�����Ϣ�ļ�patchinfo.txt
	m_pHttp->Download(m_strSrcPatchInfo, FALSE);
	m_pHttp->WaitForEnd(INFINITE);
	if( m_bTerminateThread )
		return GT_INVALID;
	if( !m_pHttp->GetResult() )
	{
		PostMessage(m_hWndMain, WM_USER_CAN_UPDATE, 0, 0);
		return GT_INVALID;
	}

	// �Ƿ���Ҫ�����Լ�
	INT nResult = this->ParseLauncherVersion();
	if( nResult != 0 )
	{
		::DeleteFile(m_strDestNewLauncher);

		// �����µ�launcher
		m_pHttp->Download(m_strSrcNewLauncher, FALSE);
		m_pHttp->WaitForEnd(INFINITE);
		if( m_bTerminateThread )
			return GT_INVALID;
		if( !m_pHttp->GetResult() )
		{
			PostMessage(m_hWndMain, WM_USER_CAN_UPDATE, 0, 0);
			return GT_INVALID;
		}

		// �����Լ�
		this->UpdateSelf();
		return 0;
	}

	// �������θ��������ļ�
	while( this->ParseGameVersion() == 1 )
	{
		// ���ر��θ��������ļ�
_download:
		m_pHttp->Download(m_strSrcGamePatch);
		m_pHttp->WaitForEnd(INFINITE);
		if( m_bTerminateThread )
			return GT_INVALID;
		if( !m_pHttp->GetResult() )
		{
			PostMessage(m_hWndMain, WM_USER_CAN_UPDATE, 0, 0);
			return GT_INVALID;
		}

		// ���MD5
		if( !m_strGamePatchMD5.empty() )
		{
			PostMessage(m_hWndMain, WM_USER_INFO, (WPARAM)g_StrTable[_T("CheckingMD5")], 1);
			CMD5::MD5ForFile(m_pHttp->GetDestFileName(), digest);
			CMD5::MD5ToString(strMD5, digest);
			if( strMD5 != m_strGamePatchMD5 )	// MD5��ƥ��
			{
				::DeleteFile(m_pHttp->GetDestFileName());
				goto _download;
			}
		}
		
		// ��ѹ���ͷ�,�����°汾��
		CStringA strDestFile = m_pUtil->UnicodeToAnsi(m_pHttp->GetDestFileName());
		PostMessage(m_hWndMain, WM_USER_INFO, (WPARAM)g_StrTable[_T("Decompressing")], 2);
		if( !this->Unpack(strDestFile) )
		{
			PostMessage(m_hWndMain, WM_USER_CAN_UPDATE, 0, 0);
			return GT_INVALID;
		}
	}

	// ���ط�������Ϣ�ļ�
	m_pHttp->Download(m_strSrcServerInfo, FALSE);
	m_pHttp->WaitForEnd(INFINITE);
	if( m_bTerminateThread )
		return GT_INVALID;
	if( !m_pHttp->GetResult() )
	{
		PostMessage(m_hWndMain, WM_USER_CAN_UPDATE, 0, 0);
		return GT_INVALID;
	}

	// ���ݷ�������Ϣ�ļ�
	::CopyFile(m_pHttp->GetDestFileName(), _T("launcher\\serverinfo_back.xml"), FALSE);

	PostMessage(m_hWndMain, WM_USER_PROGRESS_RANGE, 100, 0);
	PostMessage(m_hWndMain, WM_USER_PROGRESS_POS, 100, 0);
	PostMessage(m_hWndMain, WM_USER_INFO, (WPARAM)g_StrTable[_T("UpdateOK")], 3);
	PostMessage(m_hWndMain, WM_USER_CAN_RUN_GAME, 0, 0);
	return 0;

}


//-----------------------------------------------------------------------------
// ��ȡ������Ϣ
//-----------------------------------------------------------------------------
BOOL Process::GetConfig(LPCTSTR szIniFileName)
{
	const int MAX_STRING_SIZE = 512;
	TCHAR szTemp[MAX_STRING_SIZE];

	// ��ȡEXE·��
	GetModuleFileName(NULL, szTemp, sizeof(szTemp)/sizeof(TCHAR));
	TCHAR *pFind = ::_tcsrchr(szTemp, _T('\\'));
	if( pFind )
		*(pFind+1) = 0;
	_tcslwr_s(szTemp, sizeof(szTemp)/sizeof(TCHAR));
	m_strAppPath = szTemp;
	
	m_strIniFileFullName = m_strAppPath + szIniFileName;
	if( !CDiscIOMgr::IsExist(m_strIniFileFullName) )
		return FALSE;

	// ��ȡpatchinfo.txtλ��
	::GetPrivateProfileString(_T("Launcher"), _T("PatchInfo"), NULL,  szTemp, MAX_STRING_SIZE, m_strIniFileFullName);
	m_strSrcPatchInfo = szTemp;
	this->GetSimpleFileName(szTemp, szTemp);
	m_strDestPatchInfo = m_strAppPath + _T("launcher\\") + szTemp;
	//!!! ���m_strSrcPatchInfo

	// ��ȡ��������ʾҳ��
	::GetPrivateProfileString(_T("Launcher"), _T("URL"), NULL,  szTemp, MAX_STRING_SIZE, m_strIniFileFullName);
	m_strURL = szTemp;

	// ��ȡ�ٷ��ֶ���������ҳ��
	::GetPrivateProfileString(_T("Launcher"), _T("ManualURL"), NULL,  szTemp, MAX_STRING_SIZE, m_strIniFileFullName);
	m_strManualURL = szTemp;

	// ��ȡGame EXEλ��
	::GetPrivateProfileString(_T("Launcher"), _T("GameExe"), NULL,  szTemp, MAX_STRING_SIZE, m_strIniFileFullName);
	m_strGameExe = m_strAppPath + szTemp;

	// ��ȡ��Ϸ�����ļ�·��
	::GetPrivateProfileString(_T("Launcher"), _T("GameConfig"), NULL,  szTemp, MAX_STRING_SIZE, m_strIniFileFullName);
	m_strCfgFileFullName = m_strAppPath + szTemp;

	// ��ȡserverinfo.txt������λ��
	::GetPrivateProfileString(_T("Launcher"), _T("ServerInfo"), NULL,  szTemp, MAX_STRING_SIZE, m_strIniFileFullName);
	m_strSrcServerInfo = szTemp;
	this->GetSimpleFileName(szTemp, szTemp);
	m_strDestServerInfo = m_strAppPath + _T("launcher\\") + szTemp;

	// ��ȡ�°汾launcher������λ��
	::GetPrivateProfileString(_T("Launcher"), _T("NewLauncher"), NULL,  szTemp, MAX_STRING_SIZE, m_strIniFileFullName);
	m_strSrcNewLauncher = szTemp;
	this->GetSimpleFileName(szTemp, szTemp);
	m_strDestNewLauncher = m_strAppPath + _T("launcher\\") + szTemp;	// �°汾launcher����λ��

	// ��ȡ��ǰlauncher�汾
	::GetPrivateProfileString(_T("Launcher"), _T("LocalLauncher"), NULL,  szTemp, MAX_STRING_SIZE, m_strIniFileFullName);
	m_strLocalLauncherVersion = szTemp;

	// ��ȡ��ǰ��Ϸ�汾
	::GetPrivateProfileString(_T("Launcher"), _T("LocalGame"), NULL,  szTemp, MAX_STRING_SIZE, m_strIniFileFullName);
	m_strLocalGameVersion = szTemp;

	return TRUE;
}


//-----------------------------------------------------------------------------
// ����launcher����汾,0:�Ѿ������°棬1:��Ҫ����
//-----------------------------------------------------------------------------
INT	Process::ParseLauncherVersion()
{
	const int MAX_STRING_SIZE = 512;
	TCHAR szTemp[MAX_STRING_SIZE];

	::GetPrivateProfileString(_T("version"), _T("latest_launch"), NULL,  szTemp, MAX_STRING_SIZE, m_strDestPatchInfo);
	m_strLatestLauncherVersion = szTemp;
	SendMessage(m_hWndMain, WM_USER_LATEST_LAUNCHER_VER, (WPARAM)(LPCTSTR)m_strLatestLauncherVersion, 0);

	if( m_strLatestLauncherVersion == m_strLocalLauncherVersion )
		return 0;	// ����Ҫ����
	else
		return 1;
}



//--------------------------------------------------------------------------------
// ����launcher�汾��
//--------------------------------------------------------------------------------
BOOL Process::RefreshLauncherVersion()
{
	const int MAX_STRING_SIZE = 512;
	TCHAR szTemp[MAX_STRING_SIZE];

	::GetPrivateProfileString(_T("version"), _T("latest_launch"), NULL,  szTemp, MAX_STRING_SIZE, m_strDestPatchInfo);

	// ���µ�ǰlauncher�汾
	::WritePrivateProfileString(_T("Launcher"), _T("LocalLauncher"), szTemp, m_strIniFileFullName);
	m_strLocalLauncherVersion = szTemp;

	SendMessage(m_hWndMain, WM_USER_LAUNCHER_VER, (WPARAM)(LPCTSTR)m_strLocalLauncherVersion, 0);
	return TRUE;
}


//-----------------------------------------------------------------------------
// ������Ϸ�汾,0:�Ѿ������°棬1:�ҵ��µĲ�����-1:�޷��ҵ����ʵĲ���
//-----------------------------------------------------------------------------
INT Process::ParseGameVersion()
{
	const int MAX_STRING_SIZE = 512;
	TCHAR szTemp[MAX_STRING_SIZE];

	::GetPrivateProfileString(_T("version"), _T("latest_game"), NULL,  szTemp, MAX_STRING_SIZE, m_strDestPatchInfo);
	m_strLatestGameVersion = szTemp;
	SendMessage(m_hWndMain, WM_USER_LATEST_GAME_VER, (WPARAM)(LPCTSTR)m_strLatestGameVersion, 0);

	if( m_strLatestGameVersion == m_strLocalGameVersion )
	{
		m_strToGameVersion.Empty();
		m_strSrcGamePatch.Empty();
		m_strGamePatchMD5.clear();
		return 0;
	}

	CString strFrom, strTo, strURL, strField;
	for(INT nPatch=0; ;nPatch++)
	{
		// Ѱ�Һ��ʵ���������
		strField.Format(_T("patch_%d"), nPatch);
		DWORD dwReturn = ::GetPrivateProfileString(strField, _T("from"), NULL,  szTemp, MAX_STRING_SIZE, m_strDestPatchInfo);
		if( 0 == dwReturn )
			break;

		strFrom = szTemp;
		if( strFrom == m_strLocalGameVersion )
		{
			::GetPrivateProfileString(strField, _T("to"), NULL,  szTemp, MAX_STRING_SIZE, m_strDestPatchInfo);
			m_strToGameVersion = szTemp;
			::GetPrivateProfileString(strField, _T("url"), NULL,  szTemp, MAX_STRING_SIZE, m_strDestPatchInfo);
			m_strSrcGamePatch = szTemp;
			::GetPrivateProfileString(strField, _T("md5"), NULL,  szTemp, MAX_STRING_SIZE, m_strDestPatchInfo);
			tstring strTemp = szTemp;
			m_pUtil->CutSpaceOfStringHeadAndTail(strTemp);	// ȥ����β�ո�
			m_strGamePatchMD5 = m_pUtil->UnicodeToUnicode8(strTemp.c_str());

			// ת��ΪСд
			transform(m_strGamePatchMD5.begin(), m_strGamePatchMD5.end(), m_strGamePatchMD5.begin(), tolower);

			return 1;
		}
	}

	// �Ҳ�����Ӧ����
	INT nRet = ::MessageBox(NULL, g_StrTable[_T("PatchNotFound")], NULL, MB_RETRYCANCEL);
	if( nRet == IDRETRY )
		return -1;

	_exit(0);	// ǿ���˳�
	return -1;
}


//-----------------------------------------------------------------------------
// �����Լ�,���������صĸ������ر��Լ�����
//-----------------------------------------------------------------------------
BOOL Process::UpdateSelf()
{
	const int MAX_STRING_SIZE = 512;
	TCHAR szTemp[MAX_STRING_SIZE];

	// ��ȡEXE·��
	GetModuleFileName(NULL, szTemp, sizeof(szTemp)/sizeof(TCHAR));

	CString strParam;
	HANDLE hProcessorig = ::OpenProcess(SYNCHRONIZE, TRUE, ::GetCurrentProcessId());
	strParam.Format(_T("%s %s %d"), m_strDestNewLauncher, szTemp, hProcessorig);

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi;
	::CreateProcess(m_strDestNewLauncher, (LPWSTR)(LPCTSTR)strParam, 0, 0, TRUE, 0, 0, m_strAppPath, &si, &pi);
	::CloseHandle(hProcessorig);


	RefreshLauncherVersion();

	_exit(0);
	return TRUE;
}



//-----------------------------------------------------------------------------
// ��ѹ��
//-----------------------------------------------------------------------------
BOOL Process::Unpack(CStringA& strCPKFile)
{
	BOOL bRet = FALSE;
	PostMessage(m_hWndMain, WM_USER_PROGRESS_RANGE, 100, 0);
	
	// ��Ϸ�ͻ�����������,�޷���ѹ��������,��Ҫ�ȹر���Ϸ�ͻ���
	// �������ظ�����
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, _T("Loong_client"));
	while( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		//�ó����Ѿ����ڻ��������ͬ����ĳ����Ѿ�����
		INT nRet = ::MessageBox(NULL, g_StrTable[_T("ClientRunning")], NULL, MB_RETRYCANCEL);
		if( nRet == IDRETRY )
		{
			hMutex = ::CreateMutex(NULL, FALSE, _T("Loong_client"));
			continue;
		}

		if( nRet == IDCANCEL )
			return FALSE;
	}

	CloseHandle(hMutex);

	// ��ʼ��ѹ�ļ�
	CPKVfs* pReadCPK = new CPKVfs;
	CPKVfs* pCPK = new CPKVfs;

	if( !pReadCPK->Load(strCPKFile) )
	{
		//���ز����ļ�ʧ��
		SAFE_DEL(pReadCPK);
		SAFE_DEL(pCPK);
		return FALSE;
	}

	tagCPKHeader *pCPKHeader = pReadCPK->GetCPKHeader();
	tagCPKTable  *pTable = pReadCPK->GetCPKTable();
	m_dwTotalFileNum = pCPKHeader->dwFileNum;

	// ����
	DWORD dwUnPackNum = 0;
	bRet = UnpackCpk(0, dwUnPackNum, pReadCPK, pCPK);

	//�رո��ļ�
	pReadCPK->Unload();
	pCPK->Unload();
	SAFE_DEL(pReadCPK);
	SAFE_DEL(pCPK);

	if( FALSE == bRet )
		return FALSE;

	// ���еĶ���ѹ�ɹ�
	// ���°汾���ļ�
	RefreshGameVersion(m_strToGameVersion);

	//ɾ������ļ�
	::DeleteFileA(strCPKFile);
	return TRUE;
}



//-------------------------------------------------------------------------------
//��һ��cpk����ѹ��ָ����Ŀ¼
//-------------------------------------------------------------------------------
BOOL Process::UnpackCpk(DWORD dwFatherCRC, DWORD& dwUnPackNum, CPKVfs *pReadCpk, CPKVfs *pWriteCpk)
{
	if( dwFatherCRC != 0 )
	{
		//�Ȳ��Ҹ���
		INT nTableIndex = pReadCpk->GetTableIndexFromeCrc(dwFatherCRC);
		if( nTableIndex == GT_INVALID )
		{
			ErrMsg(g_StrTable[_T("ItemNotExistInCPK")]);
			return TRUE;
		}

		//���ļ�ӳ����
		DWORD dwFileCPKHandle = pReadCpk->Open(nTableIndex);	//��
		if( 0 == dwFileCPKHandle )
		{
			ErrMsg(g_StrTable[_T("FailInOpenCPKItem")]);
			return TRUE;
		}

		CPKFile *pCPKFileHandle = (CPKFile*)dwFileCPKHandle;

		//��Ȼ��·�����������cpk��ʽ����˵���ڴ�������cpk�ļ�
		CHAR szFullName[MAX_PATH];
		LPSTR lpExtName = NULL;

		strcpy(szFullName, pCPKFileHandle->szFullName);
		lpExtName = strrchr(szFullName, '.');

		//�ж��ǲ���cpk�ļ�
		if( lpExtName && _stricmp(lpExtName, ".cpk") == 0 )
		{
			dwUnPackNum++;
			pWriteCpk->Unload();
			if( FALSE == CDiscIOMgr::IsExist(pCPKFileHandle->szFullName) )
			{
				//�ȴ�������Ӧ��·��
				char szTemp[MAX_PATH];
				for(int i=0; pCPKFileHandle->szFullName[i] != '\0'; i++)
				{
					if( pCPKFileHandle->szFullName[i] == '\\' && i > 0 )
					{
						ZeroMemory(szTemp, sizeof(szTemp));
						strncpy(szTemp, pCPKFileHandle->szFullName, i);
						::CreateDirectoryA(szTemp, NULL);
					}
				}

				// ����cpk�ļ�
				if( FALSE == pWriteCpk->Create(pCPKFileHandle->szFullName) )
				{
					// �����ļ�ʧ��
					ErrMsg(g_StrTable[_T("CreateCPKFailed")]);
					pReadCpk->Close(dwFileCPKHandle);
					return TRUE;
				}
			}
			else
			{
				// ������
				if( FALSE == pWriteCpk->Load(pCPKFileHandle->szFullName) )
				{
					pReadCpk->Close(dwFileCPKHandle);
					return TRUE;
				}
			}

			// �رոþ��
			pReadCpk->Close(dwFileCPKHandle);

			// ����һ��cpk�ļ�,���ǰ������е���Ŀ¼�ŵ���CPK�ļ�����ȥ
			return UnpackCPKFromCPK(dwFatherCRC, dwUnPackNum, pReadCpk, pWriteCpk);
		}
		else
		{
			// ����cpk�ļ�,��ôֱ���ڴ����ϴ���Ŀ¼
			CreateDirToDisc(pCPKFileHandle->szFullName);
			dwUnPackNum++;
		}

		//�μ�����֮��رվ��
		pReadCpk->Close(dwFileCPKHandle);
	}

	//��ͨ��Ŀ¼,����ֻ��Ҫ�õ���һ������ļ����ļ���
	DWORD* pdwChildArray = NULL;
	DWORD dwChildDirNum = 0;

	//�õ�һ����Ŀ¼
	pReadCpk->GetFirstLevelChildDir(dwFatherCRC, dwChildDirNum);

	DWORD dwChildNum = 0;
	//�õ�һ�����ļ�
	pReadCpk->GetFirstLevelChild(dwFatherCRC, dwChildNum);

	//�����ڴ�ȡ�ϴ���
	if( dwChildDirNum > dwChildNum )
	{
		pdwChildArray = new DWORD[dwChildDirNum];
	}
	else
	{
		pdwChildArray = new DWORD[dwChildNum];
	}

	if( dwChildDirNum > 0 )
	{
		if( TRUE == m_bTerminateThread )
		{
			SAFE_DEL_ARRAY(pdwChildArray);
			return FALSE;
		}

		pReadCpk->GetFirstLevelChildDir(dwFatherCRC, dwChildDirNum, pdwChildArray);
		for(DWORD i=0; i<dwChildDirNum; i++)
		{
			//��������Ŀ¼, Ƕ�׵��ý�ѹ
			if( FALSE == UnpackCpk(pdwChildArray[i], dwUnPackNum, pReadCpk, pWriteCpk) )
			{
				SAFE_DEL_ARRAY(pdwChildArray);
				return FALSE;
			}
		}
	}

	if( dwChildNum > 0 )
	{
		pReadCpk->GetFirstLevelChild(dwFatherCRC, dwChildNum, pdwChildArray);
		for(DWORD i=0; i<dwChildNum; i++)
		{
			if( TRUE == m_bTerminateThread )
			{
				SAFE_DEL_ARRAY(pdwChildArray);
				return FALSE;
			}
			//������һ�����ļ�, ֱ�ӵ����ļ���ѹ����
			if( FALSE == UnpackSimpleFile(pdwChildArray[i], dwUnPackNum, pReadCpk) )
			{
				SAFE_DEL_ARRAY(pdwChildArray);
				return FALSE;
			}
		}
	}

	SAFE_DEL_ARRAY(pdwChildArray);
	return TRUE;
}
//-------------------------------------------------------------------------------
//��cpk�ļ���ѹ���������ļ�(������);
//-------------------------------------------------------------------------------
BOOL Process::UnpackSimpleFile(DWORD dwCRC, DWORD& dwUnPackNum, CPKVfs *pReadCPK)
{
	dwUnPackNum++;

	// ���Ȳ���tableindex
	INT nTableIndex = pReadCPK->GetTableIndexFromeCrc(dwCRC);
	if( nTableIndex == GT_INVALID )
	{
		ErrMsg(g_StrTable[_T("ItemNotExistInCPK")]);
		return TRUE;
	}

	// ���ļ�ӳ����
	DWORD dwCPKFileHandle = pReadCPK->Open(nTableIndex);	//��
	if( 0 == dwCPKFileHandle )
	{
		ErrMsg(g_StrTable[_T("FailInOpenCPKItem")]);
		return TRUE;
	}
	CPKFile *pCPKFileHandle = (CPKFile*)dwCPKFileHandle;

__loop_retry:		//���³��Եı�ǩ
	DWORD dwFileHandle = CreateFileToDisc(pCPKFileHandle->szFullName);
	if( INVALID_FILE_HANDLE == dwFileHandle )
	{
		INT nRet = MessageBox(NULL, m_pUtil->AnsiToUnicode(pCPKFileHandle->szFullName), 
			g_StrTable[_T("CreateFileFailed")], MB_ABORTRETRYIGNORE);
		if( nRet == IDABORT )
		{
			//��ֹ
			pReadCPK->Close(dwCPKFileHandle);
			return FALSE;
		}
		else if( nRet == IDRETRY )
		{
			//���³���
			goto __loop_retry;
		}
		else if( nRet == IDIGNORE )
		{
			//���ԣ��Ա����
			pReadCPK->Close(dwCPKFileHandle);
			return TRUE;
		}

		pReadCPK->Close(dwCPKFileHandle);
		return TRUE;
	}

	// �Դ��ļ����в���ӳ�䣬ѭ��д
	if( pCPKFileHandle->bCompressed == false && pCPKFileHandle->dwMapSize < pCPKFileHandle->dwFileSize  )
	{     
		DWORD dwTemp = 0;
		DWORD dwIncrease = 0;

		while(dwTemp != pCPKFileHandle->dwFileSize)
		{
			if(pCPKFileHandle->dwFileSize -  dwTemp >= GT_CPK_RAW_BUFFER_SIZE)
			{//����ÿ��Ҫд�Ĵ�С
				dwIncrease = GT_CPK_RAW_BUFFER_SIZE;
			}
			else
			{//���ʣ�µĲ���GT_CPK_RAW_BUFFER_SIZE����ֻдʣ�µ�
				dwIncrease = pCPKFileHandle->dwFileSize - dwTemp;
			}
			if( FALSE == CDiscIOMgr::Write(dwFileHandle, pCPKFileHandle->lpMem, dwIncrease, 0) )
			{
				//�ر���ؾ��
				CDiscIOMgr::Close(dwFileHandle);
				pReadCPK->Close(dwCPKFileHandle);
				return FALSE;
			}
			dwTemp += dwIncrease;
			pReadCPK->Close(dwCPKFileHandle);
			dwCPKFileHandle = pReadCPK->Open(nTableIndex, dwTemp);           //ӳ���ļ�����һ����
			pCPKFileHandle = (CPKFile*)dwCPKFileHandle;
		}
	}
	else
	{
		if( FALSE == CDiscIOMgr::Write(dwFileHandle, pCPKFileHandle->lpMem, pCPKFileHandle->dwFileSize, 0) )
		{
			//�ر���ؾ��
			CDiscIOMgr::Close(dwFileHandle);
			pReadCPK->Close(dwCPKFileHandle);
			return FALSE;
		}

	}

	PostMessage(m_hWndMain, WM_USER_PROGRESS_POS, (INT)((100 * dwUnPackNum) / m_dwTotalFileNum ), 0);

	//�ر���ؾ��
	CDiscIOMgr::Close(dwFileHandle);
	pReadCPK->Close(dwCPKFileHandle);

	return TRUE;
}


//-------------------------------------------------------------------------------
//��һ��cpk����ѹ���������cpk����Ŀ¼����
//-------------------------------------------------------------------------------
BOOL Process::UnpackCPKFromCPK(DWORD dwDirCRC, DWORD& dwUnPackNum, CPKVfs *pReadCPK, CPKVfs *pWriteCPK)
{
	DWORD* pdwChildArray = NULL;

	//��ȡһ����Ŀ¼
	DWORD dwChildDirNum = 0;
	pReadCPK->GetFirstLevelChildDir(dwDirCRC, dwChildDirNum);

	//��ȡһ�����ļ�
	DWORD dwChildNum = 0;
	pReadCPK->GetFirstLevelChild(dwDirCRC, dwChildNum);

	//�����ڴ�ȡ�ϴ���
	if( dwChildDirNum > dwChildNum )
	{
		pdwChildArray = new DWORD[dwChildDirNum];
	}
	else
	{
		pdwChildArray = new DWORD[dwChildNum];
	}

	if( dwChildNum > 0 )
	{
		//������һ�����ļ�, ����ֱ�Ӱ��������cpk�ļ�����ȥ

		pReadCPK->GetFirstLevelChild(dwDirCRC, dwChildNum, pdwChildArray);
		for(DWORD i=0; i<dwChildNum; i++)
		{
			if( TRUE == m_bTerminateThread )
			{
				SAFE_DEL_ARRAY(pdwChildArray);
				return FALSE;
			}

			Sleep( 0 );		//��Ϣ����
			dwUnPackNum++;

			INT nTableIndex = pReadCPK->GetTableIndexFromeCrc(pdwChildArray[i]);
			if( nTableIndex == GT_INVALID )
			{
				ErrMsg(g_StrTable[_T("ItemNotExistInCPK")]);
				continue;
			}

			//���ļ�ӳ����
			DWORD dwCPKFileHandle = pReadCPK->Open(nTableIndex, 0, true );	//��
			if( 0 == dwCPKFileHandle )
			{
				ErrMsg(g_StrTable[_T("FailInOpenCPKItem")]);
				continue;
			}

			CPKFile *pCPKFileHandle = (CPKFile*)dwCPKFileHandle;

			// ȷʵ���ļ�,���ǿ����ȴ�����·��,Ȼ���ټ����ļ�
			// ����һ��һ������·��,����c:\bin\tidy, c:\, c:\bin, c:\bin\tidy
			CHAR szFileName[MAX_PATH], szCPKName[MAX_PATH];
			ZeroMemory(szFileName, sizeof(szFileName));
			ZeroMemory(szCPKName, sizeof(szCPKName));

			BOOL bRet = pReadCPK->IsFileForCPK(pCPKFileHandle->szFullName, szCPKName, szFileName);
			if( !bRet )
			{
				ErrMsg(g_StrTable[_T("FailInCheckCPKFile")]);
			}
			

			char szPathFile[MAX_PATH];
			DWORD dwCPKFatherCRC = 0;

			INT j=0;
			for(int k=0, j=0; szFileName[k] != '\0'; k++)
			{
				if( szFileName[k] == '\\' && k > 0 )
				{
					ZeroMemory(szPathFile, sizeof(szPathFile));
					strncpy(szPathFile, szFileName+j, k-j);
					dwCPKFatherCRC = pWriteCPK->CreateDir(szPathFile, dwCPKFatherCRC);

					j = k+1;
				}
			}

			//���ȼ�����Ҫӳ��Ĵ�С
			tagCPKTable *pTable = pReadCPK->GetCPKTable();
			DWORD dwMapSize = pTable[nTableIndex].dwPackedSize + pTable[nTableIndex].dwExtraInfoSize;


			INT nWriteTableIndex = pWriteCPK->InitFileCPKToCPK(szFileName, dwCPKFatherCRC, pCPKFileHandle->pTableItem);
			ASSERT( nWriteTableIndex != GT_INVALID );


			DWORD dwHaveMapSize = 0 ;
			for( ; ; )
			{
				//д������
				if( FALSE == pWriteCPK->WriteFileCPKToCPK(nWriteTableIndex, dwHaveMapSize, 
					pCPKFileHandle->lpStartAddress, pCPKFileHandle->dwMapSize) )
				{
					ErrMsg(g_StrTable[_T("FailInWriteToCPK")]);
					return FALSE;
				}

				dwHaveMapSize += pCPKFileHandle->dwMapSize;
				//�رվ��
				pReadCPK->Close(dwCPKFileHandle);

				if( dwHaveMapSize >= dwMapSize )
					break;

				Sleep( 1 );

				dwCPKFileHandle = pReadCPK->Open(nTableIndex, dwHaveMapSize);	//��
				ASSERT( dwCPKFileHandle != GT_INVALID );
				pCPKFileHandle = (CPKFile*)dwCPKFileHandle;
			}
			//��������ļ�
			pWriteCPK->FinishFileCPKToCPK(nWriteTableIndex);

			PostMessage(m_hWndMain, WM_USER_PROGRESS_POS, (INT)((100L * dwUnPackNum) / m_dwTotalFileNum), 0);
		}
	}



	if( dwChildDirNum > 0 )
	{
		//�������ļ���, ����Ƕ�׵��ô˺���, ������ѹչ��

		pReadCPK->GetFirstLevelChildDir(dwDirCRC, dwChildDirNum, pdwChildArray);

		for(DWORD i=0; i<dwChildDirNum; i++)
		{
			if( TRUE == m_bTerminateThread )
			{
				SAFE_DEL_ARRAY(pdwChildArray);
				return FALSE;
			}

			dwUnPackNum++;

			INT nTableIndex = pReadCPK->GetTableIndexFromeCrc(pdwChildArray[i]);
			if( nTableIndex == GT_INVALID )
			{
				ErrMsg(g_StrTable[_T("ItemNotExistInCPK")]);
				continue;
			}

			//���ļ�ӳ����
			DWORD dwCPKFileHandle = pReadCPK->Open(nTableIndex);	//��
			if( 0 == dwCPKFileHandle )
			{
				ErrMsg(g_StrTable[_T("FailInOpenCPKItem")]);
				continue;
			}

			CPKFile *pCPKFileHandle = (CPKFile*)dwCPKFileHandle;

			//�ȴ���
			char szFileName[MAX_PATH], szCPKName[MAX_PATH];
			ZeroMemory(szFileName, sizeof(szFileName));
			ZeroMemory(szCPKName, sizeof(szCPKName));

			BOOL bRet = pReadCPK->IsFileForCPK(pCPKFileHandle->szFullName, szCPKName, szFileName);
			if( !bRet )
			{
				ErrMsg(g_StrTable[_T("FailInCheckCPKFile")]);
			}

			if( bRet == TRUE )
			{
				//ֻ��·��
				pWriteCPK->AddDirCyc(szFileName, 0); 
			}

			//�ر��ļ�ӳ����
			pReadCPK->Close(dwCPKFileHandle);

			if( FALSE == UnpackCPKFromCPK(pdwChildArray[i], dwUnPackNum, pReadCPK, pWriteCPK) )
			{
				SAFE_DEL_ARRAY(pdwChildArray);
				return FALSE;
			}
		}
	}

	SAFE_DEL_ARRAY(pdwChildArray);
	return TRUE;
}



//--------------------------------------------------------------------------------
// ���°汾��
//--------------------------------------------------------------------------------
BOOL Process::RefreshGameVersion(LPCTSTR szToVersion)
{
	::WritePrivateProfileString(_T("Launcher"), _T("LocalGame"), szToVersion, m_strIniFileFullName);
	m_strLocalGameVersion = szToVersion;

	SendMessage(m_hWndMain, WM_USER_GAME_VER, (WPARAM)(LPCTSTR)m_strLocalGameVersion, 0);
	return TRUE;
}


//------------------------------------------------------------------------------
// �������ϴ���Ŀ¼
//------------------------------------------------------------------------------
BOOL Process::CreateDirToDisc(LPCSTR szPath)
{
	ASSERT(szPath);

	//����һ��һ������·��,����c:\bin\tidy, c:\, c:\bin\tidy
	CHAR szTemp[MAX_PATH];
	for(int i=0; szPath[i] != '\0'; i++)
	{
		if( szPath[i] == '\\' && i > 0 )
		{
			ZeroMemory(szTemp, sizeof(szTemp));
			strncpy(szTemp, szPath, i);

			::CreateDirectoryA(szTemp, NULL);
		}
	}

	return ::CreateDirectoryA(szPath, NULL);
}


//------------------------------------------------------------------------------
// �������ϴ����ļ�
//------------------------------------------------------------------------------
DWORD Process::CreateFileToDisc(LPCSTR szFileName)
{
	ASSERT( szFileName );
	if( TRUE == CDiscIOMgr::IsExist(szFileName) )
	{
		DWORD fileAttributes = GetFileAttributesA(szFileName);
		fileAttributes &= ~FILE_ATTRIBUTE_READONLY;
		SetFileAttributesA(szFileName, fileAttributes);
		if( FALSE == ::DeleteFileA(szFileName) )
		{
			ErrMsg(g_StrTable[_T("DeleteFileFailed")], m_pUtil->AnsiToUnicode(szFileName));
			return INVALID_FILE_HANDLE;
		}
	}

	INT nLen = (INT)strlen(szFileName);
	if( nLen == 0 ) return FALSE;

	char szTemp[MAX_PATH];
	ZeroMemory(szTemp, sizeof(szTemp));
	for(INT i=nLen-1; i>=0; i--)
	{
		if( szFileName[i] == '\\' && i>0 )
		{
			memcpy(szTemp, szFileName, i);
			CreateDirToDisc(szTemp);
			break;
		}
	}

	return CDiscIOMgr::Create(szFileName,
		GENERIC_READ | GENERIC_WRITE,
		0, // 
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 
		NULL);
}


//-----------------------------------------------------------------------------
// �������ļ����õ����ļ���(����·��)
//-----------------------------------------------------------------------------
BOOL Process::GetSimpleFileName(LPTSTR pSimpleName, LPCTSTR pFullName)
{
	ASSERT(pSimpleName);
	ASSERT(pFullName);

	LPCTSTR lpSimpleName = _tcsrchr(pFullName, _T('\\'));
	if (NULL == lpSimpleName)
	{
		lpSimpleName = _tcsrchr(pFullName, _T('/'));
		if ( NULL == lpSimpleName )
			lstrcpy(pSimpleName, pFullName);
		else
			lstrcpy(pSimpleName, lpSimpleName+1);
	}
	else
	{
		lstrcpy(pSimpleName, lpSimpleName+1);
	}
	return TRUE;
}


//-----------------------------------------------------------------------------
// ǿ�ƽ���
//-----------------------------------------------------------------------------
VOID Process::Terminate()
{
	m_bTerminateThread = TRUE; 
	m_pHttp->Terminate();
}