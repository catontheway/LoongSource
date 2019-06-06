// AttachMdlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Cool3DView.h"
#include "AttachMdlDlg.h"
#include ".\attachmdldlg.h"


// CAttachMdlDlg �Ի���

IMPLEMENT_DYNAMIC(CAttachMdlDlg, CDialog)
CAttachMdlDlg::CAttachMdlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAttachMdlDlg::IDD, pParent)
	, m_szMdlPath(_T(""))
	, m_szTagNodeName(_T(""))
{
}

CAttachMdlDlg::~CAttachMdlDlg()
{
}

void CAttachMdlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MdlPath, m_szMdlPath);
	DDX_Text(pDX, IDC_TagNodeName, m_szTagNodeName);
}


BEGIN_MESSAGE_MAP(CAttachMdlDlg, CDialog)
	ON_BN_CLICKED(IDC_MdlFile, OnBnClickedMdlfile)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CAttachMdlDlg ��Ϣ�������

void CAttachMdlDlg::OnBnClickedMdlfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		NULL,
		this,0);
	if(IDOK==dlg.DoModal())
	{
		UpdateData();
		m_szMdlPath=dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CAttachMdlDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if(m_szMdlPath.IsEmpty())
	{
		AfxMessageBox(_T("�ļ�·������Ϊ��"));
		return;
	}
	if(m_szTagNodeName.IsEmpty())
	{
		AfxMessageBox(_T("���ص����Ʋ���Ϊ��"));
		return;
	}

	OnOK();
}
