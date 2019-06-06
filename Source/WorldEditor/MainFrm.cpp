#include "..\fchangeskin\mainfrm.h"
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "WorldEditor.h"

#include "MainFrm.h"
#include "Cool3DView.h"
#include "PluginMgr.h"
#include "WorldEditorMgr.h"
#include ".\mainfrm.h"
#include "ProgressDlg.h"
#include "MiniMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_Build, OnBuild)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_NoSleep, OnBnClickedNosleep)
	ON_COMMAND(ID_DisTerrain, OnID_DisTerrain)
	ON_MESSAGE(WM_DELMINIMAP_DLG, OnDelMiniMapDlgMsg)
	ON_UPDATE_COMMAND_UI(ID_DisTerrain, OnUpdateDisplayTerrain)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	m_bMiniMap = true;
	m_pMiniMapDlg = NULL;
}

CMainFrame::~CMainFrame()
{
}

bool g_bNoSleep=true;
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
		
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}
	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("δ�ܴ����Ի���\n");
		return -1;		// δ�ܴ���
	}
	m_wndDlgBar.CheckDlgButton(IDC_NoSleep,g_bNoSleep);

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("δ�ܴ��� Rebar\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	// TODO: �������Ҫ������ʾ���򽫴��Ƴ�
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	CenterWindow(NULL);
	
	// ����һ����ͼ��ռ�ÿ�ܵĹ�����
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 800, 600), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("δ�ܴ�����ͼ����\n");
		return -1;
	}
	
	Cool3DView::Inst()->InitEngine(m_wndView.GetSafeHwnd());
	WorldEditorMgr::Inst()->Create();
	PluginMgr::Inst()->LoadEditors();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MINIMIZEBOX | WS_SYSMENU;//WS_MAXIMIZEBOX

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	//--
	RECT rc;
	rc.left=0;
	rc.right=800;
	rc.top=0;
	rc.bottom=600+30;
	AdjustWindowRectEx(&rc,cs.style,TRUE,cs.dwExStyle);
	cs.x=rc.left;
	cs.y=rc.top;
	cs.cx=rc.right-rc.left;
	cs.cy=rc.bottom-rc.top;

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// ������ǰ�Ƶ���ͼ����
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ����ͼ��һ�γ��Ը�����
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// ����ִ��Ĭ�ϴ���
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnDestroy()
{
	// TODO: �ڴ˴������Ϣ����������
	
	CFrameWnd::OnDestroy();
}

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
	//return CFrameWnd::OnEraseBkgnd(pDC);
}

void CMainFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CFrameWnd::OnClose();
}

void CMainFrame::OnBuild()
{
	// TODO: �ڴ���������������
	CProgressDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	// TODO: �ڴ˴������Ϣ����������
	if(nState==WA_INACTIVE)
		CWorldEditorApp::m_bMyActive=false;
	else
		CWorldEditorApp::m_bMyActive=true;
}
/*
BOOL CMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CWnd* pParentWnd , LPCTSTR lpszMenuName , DWORD dwExStyle , CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
}
*/

void CMainFrame::OnBnClickedNosleep()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_bNoSleep=m_wndDlgBar.IsDlgButtonChecked(IDC_NoSleep)!=0;
}

void CMainFrame::OnID_DisTerrain()
{
	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	pMainFrame->m_bMiniMap = false;
	m_pMiniMapDlg = new MiniMapDlg(this);
	BOOL bShow = m_pMiniMapDlg->Create(MiniMapDlg::IDD, this);
	if(bShow)
		m_pMiniMapDlg->ShowWindow(SW_SHOW);
}

LONG CMainFrame::OnDelMiniMapDlgMsg(WPARAM wP,LPARAM lP)
{
	m_pMiniMapDlg = NULL;
	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	pMainFrame->m_bMiniMap = true;
	return 0;
}

void CMainFrame::OnUpdateDisplayTerrain(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bMiniMap);
	pCmdUI->SetCheck(!m_bMiniMap);
}

void CMainFrame::UpdateUI()
{
	CString str;
	UINT numLoading=ResMgr::Inst()->GetNumLoading();
	if(numLoading>0)
	{
		str.Format(_T("���ڼ��ص���Դ=%u"),numLoading);
	}
	else
	{
		str=_T("����");
	}
	m_wndStatusBar.SetPaneText(0,str);
}
