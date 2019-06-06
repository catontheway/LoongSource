// TDecoPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TerrainEditor.h"
#include "TDecoPage.h"
#include ".\tdecopage.h"
#include "PlugTerrainEditor.h"


// TDecoPage �Ի���

IMPLEMENT_DYNAMIC(TDecoPage, CPropertyPage)
TDecoPage::TDecoPage()
	: CPropertyPage(TDecoPage::IDD)
	, m_szShowSize(_T(""))
{
}

TDecoPage::~TDecoPage()
{
}

void TDecoPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LayerList, m_layerList);
	DDX_Control(pDX, IDC_LayerProp, m_layerProp);
	DDX_Text(pDX, IDC_ShowSize, m_szShowSize);
}


BEGIN_MESSAGE_MAP(TDecoPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BtnAdd, OnBnClickedBtnadd)
	ON_BN_CLICKED(IDC_BtnRemove, OnBnClickedBtnremove)
	ON_LBN_SELCHANGE(IDC_LayerList, OnLbnSelchangeLayerlist)
	ON_BN_CLICKED(IDC_BtnApply, OnBnClickedBtnapply)
	ON_BN_CLICKED(IDC_BtnImportMap, OnBnClickedBtnimportmap)
	ON_EN_CHANGE(IDC_ShowSize, OnEnChangeShowsize)
	ON_BN_CLICKED(IDC_RefreshView, OnBnClickedRefreshview)
END_MESSAGE_MAP()


// TDecoPage ��Ϣ�������

void TDecoPage::OnBnClickedBtnadd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szName[256];
	_stprintf(szName,_T("%d"),GetTickCount());
	g_editor->GetEngine()->AddDecoLayer(szName);

	BuildLayerList();
}

void TDecoPage::OnBnClickedBtnremove()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString szName=GetSelLayerName();
	if(szName.GetLength() > 0)
	{
		g_editor->GetEngine()->RemoveDecoLayer(szName);
		BuildLayerList();
	}
}

CString TDecoPage::GetSelLayerName()
{
	int sel=m_layerList.GetCurSel();
	if(sel==-1)
		return _T("");
	else
	{
		CString ret;
		m_layerList.GetText(sel,ret);
		return ret;
	}
}

void TDecoPage::BuildLayerList()
{
	int sel=m_layerList.GetCurSel();
	m_layerList.ResetContent();
	for(int i=0;i<TerrainEditor::MAX_DECO_LAYER;i++)
	{
		tagTDecoLayer *pLayer=g_editor->GetEngine()->GetDecoLayer(i);
		if(pLayer!=NULL &&
			pLayer->szName[0]!=0)
		{
			m_layerList.AddString(pLayer->szName);
		}
	}
	if(sel!=-1)
		m_layerList.SetCurSel(sel);
}

void TDecoPage::UpdateLayerProp(tagTDecoLayer *pLayer,bool bSave)
{
	if(pLayer==NULL)
		return;

	if(!bSave)//������Ǵ�����,���ؽ�list
		m_layerProp.ResetContent();

	int p=0;
	m_layerProp.UpdateVal(_T("����"),pLayer->szName,sizeof(pLayer->szName),p++,bSave,false);
	m_layerProp.UpdateVal(_T("����ϵ��"),pLayer->fadeOut,p++,bSave);

	m_layerProp.UpdateVal(_T("����"),pLayer->type,p++,bSave);
	m_layerProp.UpdateVal(_T("�ļ���"),pLayer->szFileName,sizeof(pLayer->szFileName),p++,bSave,true);
	m_layerProp.UpdateVal(_T("x��С"),pLayer->size.x,p++,bSave);
	m_layerProp.UpdateVal(_T("y��С"),pLayer->size.y,p++,bSave);
	m_layerProp.UpdateVal(_T("z��С"),pLayer->size.z,p++,bSave);
	m_layerProp.UpdateVal(_T("��С���ϵ��"),pLayer->sizeRandRate,p++,bSave);
	m_layerProp.UpdateVal(_T("Alpha mask"),pLayer->alphaRef,p++,bSave);
	m_layerProp.UpdateVal(_T("x��ת���"),pLayer->rotateRand.x,p++,bSave);
	m_layerProp.UpdateVal(_T("y��ת���"),pLayer->rotateRand.y,p++,bSave);
	m_layerProp.UpdateVal(_T("z��ת���"),pLayer->rotateRand.z,p++,bSave);
	m_layerProp.UpdateVal(_T("���"),pLayer->wave,p++,bSave);
	m_layerProp.UpdateVal(_T("����"),pLayer->speed,p++,bSave);

	m_layerProp.UpdateVal(_T("����"),pLayer->diffuse,p++,bSave);
}

BOOL TDecoPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	BuildLayerList();

	m_szShowSize.Format(_T("%d"),TerrainEditor::OP_Deco_Draw_Size);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void TDecoPage::OnLbnSelchangeLayerlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString szSelName=GetSelLayerName();
	if(szSelName.GetLength() > 0)
	{
		tagTDecoLayer *pLayer=g_editor->GetEngine()->GetDecoLayer(szSelName);
		if(pLayer)
			UpdateLayerProp(pLayer,false);
		else
			AfxMessageBox(_T("Layer not found!"));
	}
}

void TDecoPage::OnBnClickedBtnapply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString szSelName=GetSelLayerName();
	if(szSelName.GetLength() > 0)
	{
		tagTDecoLayer *pLayer=g_editor->GetEngine()->GetDecoLayer(szSelName);
		if(pLayer)
		{
			UpdateLayerProp(pLayer,true);
			BuildLayerList();//name���Ա��޸�
			g_editor->GetEngine()->PostEditChangeDecoLayer(szSelName,true);
		}
		else
			AfxMessageBox(_T("Layer not found!"));
	}
}

void TDecoPage::OnBnClickedBtnimportmap()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString szSelName=GetSelLayerName();
	if(szSelName.GetLength() <= 0)
	{
		AfxMessageBox(_T("����ѡ��һ����!"));
		return;
	}

	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("BMP�ļ�|*.bmp|TGA�ļ�|*.tga||"),
		this,0);
	if(IDOK==dlg.DoModal())
	{
		CString szImgPath=dlg.GetPathName();
		g_editor->GetEngine()->ImportDensityMap(szSelName,szImgPath,
			g_editor->GetCamera());
	}
}

bool TDecoPage::IfEnableNoise()
{
	if(IsDlgButtonChecked(IDC_EnableNoise)>0)
		return true;
	else
		return false;
}
void TDecoPage::OnEnChangeShowsize()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CPropertyPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	TerrainEditor::OP_Deco_Draw_Size=_tstoi(m_szShowSize);
}

void TDecoPage::OnBnClickedRefreshview()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_editor->GetEngine()->RefreshDecoView(g_editor->GetCamera());
}

BOOL TDecoPage::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
	BuildLayerList();

	return CPropertyPage::OnSetActive();
}
