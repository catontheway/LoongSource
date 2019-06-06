// THeightMapPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "TerrainEditor.h"
#include "THeightMapPage.h"
#include ".\theightmappage.h"
#include "PlugTerrainEditor.h"
#include "TImportDlg.h"
#include "TCreateDlg.h"
#include "UVSetDlg.h"


// THeightMapPage �Ի���

IMPLEMENT_DYNAMIC(THeightMapPage, CPropertyPage)
THeightMapPage::THeightMapPage()
	: CPropertyPage(THeightMapPage::IDD)
	, m_szEditToolInfo(_T(""))
	, m_bWireFrame(FALSE)
	, m_szName(_T("default"))
	, m_szBuildPath(_T(".\\Terrain\\"))
	, m_szShowDist(_T(""))
	, m_szEditDist(_T(""))
{
}

THeightMapPage::~THeightMapPage()
{
}

void THeightMapPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EditTools, m_editTools);
	DDX_Text(pDX, IDC_Info, m_szEditToolInfo);
	DDX_Check(pDX, IDC_Wireframe, m_bWireFrame);
	DDX_Text(pDX, IDC_Name, m_szName);
	DDX_Text(pDX, IDC_BuildPath, m_szBuildPath);
	DDX_Control(pDX, IDC_Build, m_btnBuild);
	DDX_Control(pDX, IDC_ShowPatchEdge, m_btnShowEdge);
	DDX_Text(pDX, IDC_ShowDist, m_szShowDist);
	DDX_Text(pDX, IDC_EditDist, m_szEditDist);
}


BEGIN_MESSAGE_MAP(THeightMapPage, CPropertyPage)
	ON_BN_CLICKED(IDC_ImportHightmap, OnBnClickedImporthightmap)
	ON_BN_CLICKED(IDC_Create, OnBnClickedCreate)
	ON_LBN_SELCHANGE(IDC_EditTools, OnLbnSelchangeEdittools)
	ON_BN_CLICKED(IDC_Wireframe, OnBnClickedWireframe)
	ON_BN_CLICKED(IDC_Save, OnBnClickedSave)
	ON_BN_CLICKED(IDC_Load, OnBnClickedLoad)
	ON_BN_CLICKED(IDC_Build, OnBnClickedBuild)
	ON_EN_CHANGE(IDC_ShowDist, OnEnChangeShowdist)
	ON_EN_CHANGE(IDC_EditDist, OnEnChangeEditdist)
	ON_BN_CLICKED(IDC_Mod, OnBnClickedMod)
	ON_BN_CLICKED(IDC_EditUVSet, OnBnClickedEdituvset)
	ON_BN_CLICKED(IDC_ExportHightmap, OnBnClickedExporthightmap)
	ON_BN_CLICKED(IDC_Undo, OnBnClickedUndo)
	ON_BN_CLICKED(IDC_Redo, OnBnClickedRedo)
END_MESSAGE_MAP()


// THeightMapPage ��Ϣ�������

void THeightMapPage::OnBnClickedImporthightmap()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	BeginMFCRes();
	TImportDlg dlg(m_szName);
	dlg.DoModal();
	EndMFCRes();
}

void THeightMapPage::OnBnClickedCreate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	BeginMFCRes();
	TCreateDlg dlg(m_szName);
	dlg.DoModal();
	EndMFCRes();
}

BOOL THeightMapPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_editTools.ResetContent();
	for(UINT i=0;i<HEditToolsNum;i++)
	{
		m_editTools.AddString(HEditToolsName[i]);
	}
	m_editTools.SetCurSel(0);

	bool t;
	bool bw=false;
	g_editor->GetEngine()->GetDrawOption(bw,t);
	m_bWireFrame=bw;

	m_szShowDist.Format(_T("%d"),g_editor->GetEngine()->OP_Draw_Dist);
	m_szEditDist.Format(_T("%d"),g_editor->GetEngine()->OP_Edit_Dist);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void THeightMapPage::OnLbnSelchangeEdittools()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel=m_editTools.GetCurSel();
	if(sel!=-1)
	{
		int mysel=sel;
		ASSERT(mysel>=0 && mysel<HEditToolsNum);
		UpdateData(TRUE);
		m_szEditToolInfo=HEditToolsInfo[mysel];
		UpdateData(FALSE);
	}
}

void THeightMapPage::SetDrawOption()
{
	UpdateData();
	g_editor->GetEngine()->SetDrawOption(m_bWireFrame==TRUE,true);
}

void THeightMapPage::OnBnClickedWireframe()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDrawOption();
}

void THeightMapPage::OnBnClickedSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!g_editor->GetEngine()->SaveInfo())
	{
		AfxMessageBox(Cool3D::Kernel::Inst()->GetLastError());
	}
	else
	{
		AfxMessageBox(_T("���̳ɹ�"));
	}
}

void THeightMapPage::OnBnClickedLoad()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	g_editor->GetEngine()->SetName(m_szName);
	if(!g_editor->GetEngine()->LoadInfo())
	{
		AfxMessageBox(Cool3D::Kernel::Inst()->GetLastError());
	}
}

void THeightMapPage::OnBnClickedBuild()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
}

void THeightMapPage::OnEnChangeShowdist()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CPropertyPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	UpdateData();
	int nShowDist=_tstoi(m_szShowDist);
	g_editor->GetEngine()->OP_Draw_Dist=nShowDist;
}

void THeightMapPage::OnEnChangeEditdist()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CPropertyPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	UpdateData();
	int nEditDist=_tstoi(m_szEditDist);
	g_editor->GetEngine()->OP_Edit_Dist=nEditDist;
}

void THeightMapPage::OnBnClickedMod()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	BeginMFCRes();
	TCreateDlg dlg(m_szName);
	dlg.m_bMod=true;
	dlg.DoModal();
	EndMFCRes();
}

void THeightMapPage::OnBnClickedEdituvset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BeginMFCRes();
	CUVSetDlg dlg;
	dlg.DoModal();
	EndMFCRes();
}

void THeightMapPage::OnBnClickedExporthightmap()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	_T("BMP�ļ�|*.bmp||"),
	this,0);
	if(IDOK==dlg.DoModal())
	{
		CString szPath = dlg.GetPathName();

		if(g_editor->GetEngine()->ExportHeightMap(szPath))
			AfxMessageBox(_T("�����ɹ���"));
		else
			AfxMessageBox(_T("����ʧ�ܣ�"));
	}
}

void THeightMapPage::OnBnClickedUndo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_editor->GetEngine()->UndoBrush();
}

void THeightMapPage::OnBnClickedRedo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_editor->GetEngine()->RedoBrush();
}
