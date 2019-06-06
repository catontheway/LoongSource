// CameraCtrlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "CameraCtrlDlg.h"
#include ".\cameractrldlg.h"


// CCameraCtrlDlg �Ի���

IMPLEMENT_DYNAMIC(CCameraCtrlDlg, CDialog)
CCameraCtrlDlg::CCameraCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraCtrlDlg::IDD, pParent)
	, m_nX(0)
	, m_nY(0)
{
	m_pCamera = NULL;
	//m_slider.SetRange(500, 100, TRUE);
}

CCameraCtrlDlg::~CCameraCtrlDlg()
{
}

void CCameraCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_nX);
	DDX_Text(pDX, IDC_EDIT_Y, m_nY);
	DDX_Control(pDX, IDC_SLIDER_CameraStrong, m_slider);
}


BEGIN_MESSAGE_MAP(CCameraCtrlDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_GOTO, OnBnClickedButtonGoto)
END_MESSAGE_MAP()

BOOL CCameraCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog(); 

	m_slider.SetRange(300, 5000); 
	m_slider.SetPos(500);

	return TRUE;
}


// CCameraCtrlDlg ��Ϣ�������

void CCameraCtrlDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CCameraCtrlDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CCameraCtrlDlg::OnBnClickedButtonGoto()
{
	if( !UpdateData( TRUE ) )
		return;

	ASSERT( m_pCamera );
	m_pCamera->SetLookAtPos( Vector3( (float)m_nX*50, 0, (float)m_nY*50 ) );
}
