// WayPointWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MapEditor.h"
#include "WayPointWnd.h"
#include ".\waypointwnd.h"


// WayPointWnd �Ի���

IMPLEMENT_DYNAMIC(WayPointWnd, CDialog)
WayPointWnd::WayPointWnd(CWnd* pParent /*=NULL*/)
	: CDialog(WayPointWnd::IDD, pParent)
	, m_strWayPtName(_T(""))
{
}

WayPointWnd::~WayPointWnd()
{
}

void WayPointWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_WAYPOINTNAME, m_EditWayPoint);
	DDX_Text(pDX, IDC_EDIT_WAYPOINTNAME, m_strWayPtName);
}


BEGIN_MESSAGE_MAP(WayPointWnd, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// WayPointWnd ��Ϣ�������

void WayPointWnd::OnBnClickedOk()
{
	((CEdit*)(GetDlgItem(IDC_EDIT_WAYPOINTNAME)))->GetWindowText(m_strWayPtName);
	OnOK();
}
