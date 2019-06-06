// WorldEditor.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "MainFrm.h"
#include ".\WorldEditor.h"
#include "Cool3DView.h"
#include "PluginMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


bool CWorldEditorApp::m_bMyActive=true;
// CWorldEditorApp

BEGIN_MESSAGE_MAP(CWorldEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
END_MESSAGE_MAP()


// CWorldEditorApp ����

CWorldEditorApp::CWorldEditorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	
	// ��ֹ��Ļ��������Դ����
	::SystemParametersInfo(SPI_SETLOWPOWERTIMEOUT, 0, NULL, 0);
	::SystemParametersInfo(SPI_SETPOWEROFFTIMEOUT, 0, NULL, 0);
	::SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT, 0, NULL, 0);
}


// Ψһ��һ�� CWorldEditorApp ����

CWorldEditorApp theApp;

// CWorldEditorApp ��ʼ��

BOOL CWorldEditorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// ���������ش�������Դ�Ŀ��
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// �������ں�׺ʱ�ŵ��� DragAcceptFiles��
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����

	_set_se_translator(Cool3D::Cool_SEH_Translation);
	return TRUE;
}

int CWorldEditorApp::Run()
{
#ifdef _DEBUG
	return CWinApp::Run();
#else
	try
	{
		return CWinApp::Run();
	}
	catch(Cool3D::Exception e)
	{
		AfxMessageBox(_T("�༭�������쳣���뽫log�ļ�����������Ա!"));
		exit(0);
	}
	return 0;
#endif
}


// CWorldEditorApp ��Ϣ�������



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CWorldEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CWorldEditorApp ��Ϣ�������


extern bool g_bNoSleep;
BOOL CWorldEditorApp::OnIdle(LONG lCount)
{
	// TODO: �ڴ����ר�ô����/����û���
	//if(AfxGetMainWnd()->IsWindowVisible())
	//ASSERT(m_pMainWnd->IS_STATIC_CLASS(CMainFrame));	
	CMainFrame* pFrame=(CMainFrame*)m_pMainWnd;
	pFrame->UpdateUI();

	Cool3DView::Inst()->Render();
	PluginMgr::Inst()->RenderPreview();

	//--����֡��������30FPS
	if(!g_bNoSleep)
	{
		int sleepMS=(1000/30)-Cool3D::Kernel::Inst()->GetDeltaTimeDW();
		if(sleepMS>0)
			Sleep(sleepMS);

		if(!m_bMyActive)
			Sleep(200);
	}
	//return CWinApp::OnIdle(lCount);
	return true;
}
