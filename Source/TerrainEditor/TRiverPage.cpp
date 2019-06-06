// TRiverPage.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "TerrainEditor.h"
#include "TRiverPage.h"
#include ".\triverpage.h"
#include "PlugTerrainEditor.h"
#include "..\Cool3D\EditorSupport\TRiverEditor.h"

// TRiverPage �Ի���

IMPLEMENT_DYNAMIC(TRiverPage, CPropertyPage)
TRiverPage::TRiverPage()
	: CPropertyPage(TRiverPage::IDD)
	, m_szHeight(_T("100"))
	, m_szShowDist(_T(""))
	, m_szEditDist(_T(""))
{
	//
}

TRiverPage::~TRiverPage()
{
}

void TRiverPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Height, m_szHeight);
	DDX_Control(pDX, IDC_Mtl, m_mtlProp);
	DDX_Control(pDX, IDC_LIST1, m_riverList);
	DDX_Control(pDX, IDC_EditTools, m_editTools);
	DDX_Text(pDX, IDC_ShowDist, m_szShowDist);
}


BEGIN_MESSAGE_MAP(TRiverPage, CPropertyPage)
	ON_EN_CHANGE(IDC_Height, OnEnChangeHeight)
	ON_BN_CLICKED(IDC_ApplyMtl, OnBnClickedApplymtl)
	ON_BN_CLICKED(IDC_NewRiver, OnBnClickedNewriver)
	ON_BN_CLICKED(IDC_DelRiver, OnBnClickedDelriver)
	ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
	ON_EN_CHANGE(IDC_ShowDist, &TRiverPage::OnEnChangeShowdist)
END_MESSAGE_MAP()


// TRiverPage ��Ϣ�������

BOOL TRiverPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	BuildRiverList();

	m_editTools.ResetContent();
	m_editTools.AddString(_T("Brush"));
	m_editTools.AddString(_T("Noise"));
	m_editTools.AddString(_T("Smooth"));
	m_editTools.SetCurSel(0);

	m_szShowDist.Format(_T("%d"),g_editor->GetEngine()->GetRiverEditor()->OP_Draw_Dist);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL TRiverPage::GetSelRiverIndex( UINT& riverIndex )
{
	if(m_riverList.m_hWnd==NULL)
		return FALSE;

	int selIndex=m_riverList.GetCurSel();
	if(selIndex==CB_ERR)
		return FALSE;
	riverIndex=selIndex;
	return TRUE;
}

void TRiverPage::UpdateMtlDesc(tagRiverMtlDesc *pMtl,bool bSave)
{
	if(!bSave)
		m_mtlProp.ResetContent();
	int p=0;
	m_mtlProp.UpdateVal(_T("͸����"),pMtl->alpha,p++,bSave);
	m_mtlProp.UpdateVal(_T("����ϵ��"),pMtl->power,p++,bSave);
	m_mtlProp.UpdateVal(_T("ˮ����ͼ����"),pMtl->szWaterTex,sizeof(pMtl->szWaterTex),p++,bSave,true);
	m_mtlProp.UpdateVal(_T("ˮ����ͼ֡��"),pMtl->waterFrm,p++,bSave);
	m_mtlProp.UpdateVal(_T("ˮ����ͼFPS"),pMtl->waterFps,p++,bSave);
	m_mtlProp.UpdateVal(_T("ˮ����ͼuTilesize"),pMtl->uTileSize,p++,bSave);
	m_mtlProp.UpdateVal(_T("ˮ����ͼvTilesize"),pMtl->vTileSize,p++,bSave);
	m_mtlProp.UpdateVal(_T("�����Ŷ���ͼ"),pMtl->szBumpMap,sizeof(pMtl->szBumpMap),p++,bSave,true);
	//m_mtlProp.UpdateVal(_T("������ͼ֡��"),pMtl->bumpFrm,p++,bSave);
	m_mtlProp.UpdateVal(_T("�Ŷ���ͼ�����ٶ�U"),pMtl->bumpUVScroll.x,p++,bSave);
	m_mtlProp.UpdateVal(_T("�Ŷ���ͼ�����ٶ�V"),pMtl->bumpUVScroll.y,p++,bSave);
	m_mtlProp.UpdateVal(_T("����ͼ��ϱ���"),pMtl->blendColor.A,p++,bSave);
	m_mtlProp.UpdateVal(_T("����ͼ�����ɫ"),pMtl->blendColor,p++,bSave);
	m_mtlProp.UpdateVal(_T("�����Ŷ�ϵ��"),pMtl->bumpScale,p++,bSave);
	m_mtlProp.UpdateVal(_T("����Ч������ϵ��"),pMtl->reflectPower,p++,bSave);
	m_mtlProp.UpdateVal(_T("����Ч���߹�����"),pMtl->reflectSpecularScale,p++,bSave);
	m_mtlProp.UpdateVal(_T("ˮ������ʼ�߶�"),pMtl->underWaterFogStart,p++,bSave);
	m_mtlProp.UpdateVal(_T("ˮ��������߶�"),pMtl->underWaterFogEnd,p++,bSave);
	m_mtlProp.UpdateVal(_T("ˮ������ɫ"),(Color4f&)pMtl->underWaterFogColor,p++,bSave);
	m_mtlProp.UpdateVal(_T("���������"),pMtl->diffuse,p++,bSave);
	m_mtlProp.UpdateVal(_T("���������"),pMtl->ambient,p++,bSave);
	m_mtlProp.UpdateVal(_T("�߹����"),pMtl->specular,p++,bSave);

	m_mtlProp.UpdateVal(_T("�������䡢���䡢�߹�Ч��"),pMtl->bEnableEffect,p++,bSave);
	m_mtlProp.UpdateVal(_T("�رձ�Ե����"),pMtl->bCloseEdgeFade,p++,bSave);
}

void TRiverPage::OnEnChangeHeight()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CPropertyPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UINT riverIndex;
	if( GetSelRiverIndex(riverIndex) )
	{
		TerrainEditor *pTE=g_editor->GetEngine();
		TRiverEditor *pRE=pTE->GetRiverEditor();
		if(pRE)
		{
			CString szHeight;
			GetDlgItem(IDC_Height)->GetWindowText(szHeight);
			int h=_tstoi(szHeight);
			pRE->ApplyHeight2River(riverIndex,h);
		}
	}
}

void TRiverPage::OnBnClickedApplymtl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UINT riverIndex;
	if( GetSelRiverIndex(riverIndex) )
	{
		TerrainEditor *pTE=g_editor->GetEngine();
		TRiverEditor *pRE=pTE->GetRiverEditor();
		if(pRE)
		{
			UpdateMtlDesc(pRE->GetRiverMtlDescByIndex(riverIndex),true);

			pRE->ApplyMtl2River(riverIndex);
		}
	}
}

void TRiverPage::OnBnClickedNewriver()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString szHeight;
	GetDlgItem(IDC_Height)->GetWindowText(szHeight);
	int h=_tstoi(szHeight);
	g_editor->GetEngine()->NewRiver(h);

	BuildRiverList();
}

void TRiverPage::OnBnClickedDelriver()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UINT riverIndex;
	if( GetSelRiverIndex(riverIndex) )
	{
		g_editor->GetEngine()->DelRiver(riverIndex);
		BuildRiverList();
	}
}

void TRiverPage::BuildRiverList()
{
	m_riverList.ResetContent();
	for(int i=0;i<g_editor->GetEngine()->GetRiverNum();i++)
	{
		CString szTmp;
		szTmp.Format(_T("%02d"),i);
		m_riverList.AddString(szTmp);
	}
	m_riverList.SetCurSel(0);
}

void TRiverPage::OnLbnSelchangeList1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UINT riverIndex;
	if( GetSelRiverIndex(riverIndex) )
	{
		TerrainEditor *pTE=g_editor->GetEngine();
		TRiverEditor *pRE=pTE->GetRiverEditor();
		if(pRE)
		{
			m_szHeight.Format(_T("%.2d"),pRE->GetRiverHeightByIndex(riverIndex));
			UpdateData(FALSE);

			UpdateMtlDesc(pRE->GetRiverMtlDescByIndex(riverIndex),false);
		}
	}
}

BOOL TRiverPage::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
	BuildRiverList();

	return CPropertyPage::OnSetActive();
}

void TRiverPage::OnEnChangeShowdist()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	UpdateData();
	int nShowDist=_tstoi(m_szShowDist);
	g_editor->GetEngine()->GetRiverEditor()->OP_Draw_Dist=nShowDist;
}
