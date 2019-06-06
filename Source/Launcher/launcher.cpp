// launcher.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "launcher.h"
#include "launcherDlg.h"

#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ClauncherApp
BEGIN_MESSAGE_MAP(ClauncherApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
// construction / destruction
//-----------------------------------------------------------------------------
ClauncherApp::ClauncherApp()
{
	g_StrTable.Init(NULL, _T("launcher\\launcher_table.xml"));
}

ClauncherApp::~ClauncherApp()
{
	g_StrTable.Destroy();
}


// Ψһ��һ�� ClauncherApp ����
ClauncherApp theApp;

// ClauncherApp ��ʼ��
BOOL ClauncherApp::InitInstance()
{
	Util util;
	std::vector<tstring> tokens;
	util.StringToToken(tokens, AfxGetApp()->m_lpCmdLine);

	// ����Ǹ��ƽ��̣���Ҫ�ȴ�ԭlauncher.exe�˳���Ȼ�󸲸ǲ�����֮������˳�
    //ԭ����tokens.size()>1 �޸�==2  begin modify by alex  @ 2009.12.16
    //tw��beanfun����lancher�Ĳ���Ϊ tw uid pwd
	//if( tokens.size() > 1 )
    if( tokens.size()==2 )  
	{
		// �ȴ�ԭlauncher.exe�˳�
		HANDLE hProcessorig = (HANDLE)_ttoi(tokens[1].c_str());
		::WaitForSingleObject(hProcessorig, INFINITE);
		::CloseHandle(hProcessorig);

		// ��ȡ����EXE·��
		const int MAX_STRING_SIZE = 512;
		TCHAR szTemp[MAX_STRING_SIZE];
		GetModuleFileName(NULL, szTemp, sizeof(szTemp)/sizeof(TCHAR));

		// ���Ƹ���
		::CopyFile(szTemp, tokens[0].c_str(), FALSE);
		::Sleep(3000);	// �ȴ�һ��

		// ����֮
		ShellExecute( NULL, _T("open"), tokens[0].c_str(), NULL, NULL, SW_SHOWNORMAL );
		return FALSE;	// �˳�
	}

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Loong Launcher"));


	// ������Ա� �Ƿ���ȷ����
	if( !g_StrTable.IsLoaded() )
	{
		//�ó����Ѿ����ڻ��������ͬ����ĳ����Ѿ�����
		MessageBox(NULL, _T("File: launcher\\launcher_table.xml could not found!"), _T("Error"), MB_OK);
		return FALSE;
	}


	// �������ظ�����
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, _T("Loong_launcher"));
	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		//�ó����Ѿ����ڻ��������ͬ����ĳ����Ѿ�����
		MessageBox(NULL, g_StrTable[_T("AlreadyRunning")], g_StrTable[_T("Warning")], MB_OK);
		return FALSE;
	}

	CLauncherDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}

