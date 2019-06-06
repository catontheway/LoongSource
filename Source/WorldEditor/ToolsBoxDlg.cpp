// ToolsBoxDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "ToolsBoxDlg.h"
#include ".\toolsboxdlg.h"
#include "PluginMgr.h"


// CToolsBoxDlg �Ի���

IMPLEMENT_DYNAMIC(CToolsBoxDlg, CDialog)
CToolsBoxDlg::CToolsBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolsBoxDlg::IDD, pParent)
{
}

CToolsBoxDlg::~CToolsBoxDlg()
{
}

void CToolsBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ToolsList, m_list);
}


BEGIN_MESSAGE_MAP(CToolsBoxDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_ToolsList, OnLbnSelchangeToolslist)
END_MESSAGE_MAP()


// CToolsBoxDlg ��Ϣ�������

void CToolsBoxDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CToolsBoxDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CToolsBoxDlg::OnLbnSelchangeToolslist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel=m_list.GetCurSel();
	if(sel!=-1)
	{
		PluginMgr::Inst()->SetActive(sel);
	}
}
