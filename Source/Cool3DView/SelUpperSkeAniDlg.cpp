// SelUpperSkeAniDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Cool3DView.h"
#include "SelUpperSkeAniDlg.h"
#include ".\selupperskeanidlg.h"


// CSelUpperSkeAniDlg �Ի���

IMPLEMENT_DYNAMIC(CSelUpperSkeAniDlg, CDialog)
CSelUpperSkeAniDlg::CSelUpperSkeAniDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelUpperSkeAniDlg::IDD, pParent)
	, m_szUpperSkeAniName(_T(""))
	, m_szLowerSkeAniName(_T(""))
	, m_bIsOneTrack(FALSE)
{
}

CSelUpperSkeAniDlg::~CSelUpperSkeAniDlg()
{
}

void CSelUpperSkeAniDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_UpperSkeAniName, m_szUpperSkeAniName);
	DDX_Text(pDX, IDC_LowerSkeAniName, m_szLowerSkeAniName);
	DDX_Check(pDX, IDC_CHECKALL, m_bIsOneTrack);
	DDX_Control(pDX, IDC_UpperSkeAniName, m_UpperSkeAniEdit);
}


BEGIN_MESSAGE_MAP(CSelUpperSkeAniDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CSelUpperSkeAniDlg ��Ϣ�������

void CSelUpperSkeAniDlg::OnBnClickedOk()
{
	UpdateData();
	if (m_bIsOneTrack)
	{
		if (m_szLowerSkeAniName.IsEmpty())
		{
			AfxMessageBox(_T("���������Ʋ���Ϊ��"));
			return;
		}
	}
	else if (m_szUpperSkeAniName.IsEmpty() && m_szLowerSkeAniName.IsEmpty())
	{
		AfxMessageBox(_T("��������һ����������"));
		return;
	}

	OnOK();
}
