// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NavMapEditor.h"
#include "PlugNavMapEditor.h"
#include "MainDlg.h"


// CMainDlg dialog

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
	, m_szInner(_T("3"))
	, m_szOuter(_T("6"))
	, m_bCango(FALSE)
	, m_bNpcNav(FALSE)
	, m_bPlayerNav(FALSE)
	, m_bEditCango(FALSE)
	, m_bEditPlayerRoad(FALSE)
	, m_bPlayerRoad(FALSE)
	, m_bNPCWayPoint(FALSE)
{

}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BRUSHSHAPE, m_shapeTypeList);
	DDX_Text(pDX, IDC_INNER, m_szInner);
	DDX_Text(pDX, IDC_OUTER, m_szOuter);
	DDX_Check(pDX, IDC_CHECK_PreLook, m_bCango);
	DDX_Check(pDX, IDC_CHECK_NPCNavPreLook, m_bNpcNav);
	DDX_Check(pDX, IDC_CHECK_PlayerNavPreLook, m_bPlayerNav);
	DDX_Check(pDX, IDC_Edit, m_bEditCango);
	DDX_Check(pDX, IDC_EditManualPlayerNav, m_bEditPlayerRoad);
	DDX_Check(pDX, IDC_CHECK_PreLook_ManualPlayerNav, m_bPlayerRoad);
	DDX_Check(pDX, IDC_CHECK_PreLook_NPCWayPoint, m_bNPCWayPoint);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_BN_CLICKED(IDC_SAVE, &CMainDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_LOAD, &CMainDlg::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_CLEAR, &CMainDlg::OnBnClickedClear)
	ON_EN_CHANGE(IDC_ShowDist, &CMainDlg::OnEnChangeShowdist)
	ON_BN_CLICKED(IDC_BUTTON2_PlayerNavLoad, &CMainDlg::OnBnClickedPlayerNavLoad)
	ON_BN_CLICKED(IDC_BUTTON1_NPCNavLoad, &CMainDlg::OnBnClickedNpcNavLoad)
	ON_BN_CLICKED(IDC_BUTTON_NPCNavLook, &CMainDlg::OnBnClickedButtonNpcNavlook)
	ON_BN_CLICKED(IDC_BUTTON_PlayerNavLook, &CMainDlg::OnBnClickedButtonPlayerNavLook)
	ON_BN_CLICKED(IDC_ClearManualPlayerNav, &CMainDlg::OnBnClickedClearPlayerRoad)
	ON_BN_CLICKED(IDC_BUTTON_NPCWayPointLoad, &CMainDlg::OnBnClickedButtonNpcwaypointload)
	ON_BN_CLICKED(IDC_BUTTON_CangoLook, &CMainDlg::OnBnClickedButtonCangolook)
	ON_BN_CLICKED(IDC_BUTTON_RoadLook, &CMainDlg::OnBnClickedButtonRoadlook)
END_MESSAGE_MAP()


// CMainDlg message handlers

void CMainDlg::OnBnClickedSave()
{
	if(IDYES!=AfxMessageBox(_T("���̽������ϴα��������,������?"),MB_YESNO|MB_ICONWARNING))
		return;

	//--����
	if(g_navEditor->Save())
		AfxMessageBox(_T("���̳ɹ�"));
}

void CMainDlg::OnBnClickedLoad()
{
	if(IDYES!=AfxMessageBox(_T("���̽�������ڱ༭������,������?"),MB_YESNO|MB_ICONWARNING))
		return;

	//--����
	if(g_navEditor->Load())
		AfxMessageBox(_T("���̳ɹ�"));
}

void CMainDlg::OnBnClickedClear()
{
	// TODO: Add your control notification handler code here

	//--�������ͼ
	if(IDYES!=AfxMessageBox(_T("��ǰ�༭�Ŀ��������򽫱�ȫ�����,������?"),MB_YESNO|MB_ICONWARNING))
		return;
	g_navEditor->ClearCangoMap();
}

void CMainDlg::OnBnClickedClearPlayerRoad()
{
	if(IDYES!=AfxMessageBox(_T("��ǰ�༭�����Ѱ·��·���򽫱�ȫ�����,������?"),MB_YESNO|MB_ICONWARNING))
		return;

	g_navEditor->ClearPlayerRoadMap();
}

void CMainDlg::GetBrushRadius( int& inner,int& outer )
{
	UpdateData();
	inner=_tstoi(m_szInner);
	outer=_tstoi(m_szOuter);
}

int CMainDlg::GetBrushShape()
{
	return m_shapeTypeList.GetCurSel();
}

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(FALSE);

	TCHAR szTxt[512];
	_stprintf(szTxt,_T("%d"),g_drawSize);
	GetDlgItem(IDC_ShowDist)->SetWindowText(szTxt);

	m_shapeTypeList.AddString(_T("Բ��"));
	m_shapeTypeList.AddString(_T("����"));
	m_shapeTypeList.AddString(_T("����"));
	m_shapeTypeList.AddString(_T("ƽ��"));
	m_shapeTypeList.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
}
void CMainDlg::OnEnChangeShowdist()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString szTxt;
	GetDlgItem(IDC_ShowDist)->GetWindowText(szTxt);
	if(!szTxt.IsEmpty())
	{
		g_drawSize=_tstoi(szTxt);
	}
}

void CMainDlg::OnBnClickedPlayerNavLoad()
{
	// TODO: Add your control notification handler code here
	if(g_navEditor->LoadPlayerNavMap())
		AfxMessageBox(_T("��ȡ���Ѱ·����ͼ�ɹ�!"));
}

void CMainDlg::OnBnClickedNpcNavLoad()
{
	// TODO: Add your control notification handler code here
	//--����
	if(g_navEditor->LoadNPCNavMap())
		AfxMessageBox(_T("��ȡNPC���ӵ���ͼ�ɹ�!"));
}

void CMainDlg::OnBnClickedButtonNpcNavlook()
{
	// TODO: Add your control notification handler code here
	g_navEditor->DisplayNPCCangoMap();
}

void CMainDlg::OnBnClickedButtonPlayerNavLook()
{
	// TODO: Add your control notification handler code here
	g_navEditor->DisplayPlayerCangoMap();
}


void CMainDlg::OnBnClickedButtonCangolook()
{
	// TODO: Add your control notification handler code here
	g_navEditor->DisplayManuelCangoMap();
}

void CMainDlg::OnBnClickedButtonRoadlook()
{
	// TODO: Add your control notification handler code here
	g_navEditor->DisplayRoadMap();
}


void CMainDlg::OnBnClickedButtonNpcwaypointload()
{
	try
	{
		g_navEditor->LoadNPCNavGraph();
		AfxMessageBox(_T("��ȡNPC·�㵼��ͼ�ɹ�!"));
	}
	catch (...)
	{}
}
