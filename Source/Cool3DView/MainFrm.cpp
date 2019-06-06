#include "..\fworldeditor\mainfrm.h"
#include "..\fworldeditor\mainfrm.h"
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "Cool3DView.h"

#include "MainFrm.h"
#include ".\mainfrm.h"
#include "Cool3DModels.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	CRect rc;
	GetClientRect(rc);
	CenterWindow(NULL);

	return 0;
}

void _ShowNote(const TCHAR* szTxt)
{
	CMainFrame *pM=(CMainFrame *)::AfxGetMainWnd();
	pM->m_wndStatusBar.SetPaneText(1,szTxt);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ
	RECT rc;
	rc.left=0;
	rc.right=1010;
	rc.top=0;
	rc.bottom=650;
	AdjustWindowRect(&rc,cs.style,TRUE);
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


void CMainFrame::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	Cool3DModels::Inst()->Destroy();

	CFrameWnd::PostNcDestroy();
}