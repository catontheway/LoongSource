// ModifyAttr.cpp : implementation file
//

#include "stdafx.h"
#include "Cool3DView.h"
#include "ModifyAttr.h"


// CModifyAttr dialog

IMPLEMENT_DYNAMIC(CModifyAttr, CDialog)

CModifyAttr::CModifyAttr(CWnd* pParent /*=NULL*/)
	: CDialog(CModifyAttr::IDD, pParent)
	, m_iLightmapSize(0)
{

}

CModifyAttr::~CModifyAttr()
{
}

void CModifyAttr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LightmapSize, m_iLightmapSize);
}


BEGIN_MESSAGE_MAP(CModifyAttr, CDialog)
	ON_BN_CLICKED(IDOK, &CModifyAttr::OnBnClickedOk)
END_MESSAGE_MAP()


// CModifyAttr message handlers

void CModifyAttr::OnBnClickedOk()
{
	UpdateData();

	//--lightmap��С������32��64��128��256��512��1024����Щֵ��
	if( m_iLightmapSize!=16
		&& m_iLightmapSize!=32
		&& m_iLightmapSize!=64
		&& m_iLightmapSize!=128
		&& m_iLightmapSize!=256
		&& m_iLightmapSize!=512
		&& m_iLightmapSize!=1024 )
	{
		AfxMessageBox(_T("Lightmap�Ĵ�С������16��64��128��256��512��1024�е�һ����"));
		return;
	}

	OnOK();
}
