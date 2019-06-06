// SetChopFieldType.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MapEditor.h"
#include "SetChopFieldType.h"


// CSetChopFieldType �Ի���

IMPLEMENT_DYNAMIC(CSetChopFieldType, CDialog)
CSetChopFieldType::CSetChopFieldType(CWnd* pParent /*=NULL*/)
	: CDialog(CSetChopFieldType::IDD, pParent)
{
}

CSetChopFieldType::~CSetChopFieldType()
{
}

void CSetChopFieldType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOBOX_CHOPFIELDTYPE, m_ChopFieldType);
}


BEGIN_MESSAGE_MAP(CSetChopFieldType, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CSetChopFieldType ��Ϣ�������
BOOL CSetChopFieldType::OnInitDialog()
{
	CDialog::OnInitDialog();
	//��ʼ�����������б�
	m_ChopFieldType.InsertString(0,_T("��ľ"));
	m_ChopFieldType.InsertString(1,_T("����"));	
	m_ChopFieldType.SetCurSel(0);

	return TRUE;
}

int CSetChopFieldType::GetChopFieldType()
{
	if(m_szChopFieldType==_T("��ľ"))
		return 0;
	else if(m_szChopFieldType==_T("����"))
		return 1;

	return -1;
}

void CSetChopFieldType::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ChopFieldType.GetWindowText(m_szChopFieldType);
	OnOK();
}
