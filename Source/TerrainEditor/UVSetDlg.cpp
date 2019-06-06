// UVSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TerrainEditor.h"
#include "UVSetDlg.h"
#include "PlugTerrainEditor.h"
#include ".\uvsetdlg.h"


// CUVSetDlg �Ի���

IMPLEMENT_DYNAMIC(CUVSetDlg, CDialog)
CUVSetDlg::CUVSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUVSetDlg::IDD, pParent)
{
}

CUVSetDlg::~CUVSetDlg()
{
}

void CUVSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UVSet, m_hUVList);
}


BEGIN_MESSAGE_MAP(CUVSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CUVSetDlg ��Ϣ�������

void CUVSetDlg::UpdateUVList(bool bSave)
{
	int i;
	Vector2 uvSet[TRN_MAX_UV_SET];
	for(i=0;i<TRN_MAX_UV_SET;i++)
		uvSet[i]=g_editor->GetEngine()->GetLayerUVScale(i);

	//--
	if(!bSave)
		m_hUVList.ResetContent();

	int pi=0;
	for(i=0;i<TRN_MAX_UV_SET-1;i++)//��һ��uv set��Ҫ����alpha map��shadowmap
	{
		TCHAR szName[64];
		_stprintf(szName,_T("U_%d"),i);
		m_hUVList.UpdateVal(szName,uvSet[i].x,pi++,bSave);
		_stprintf(szName,_T("V_%d"),i);
		m_hUVList.UpdateVal(szName,uvSet[i].y,pi++,bSave);
	}

	//--
	if(bSave)
	{
		for(i=0;i<TRN_MAX_UV_SET;i++)
			g_editor->GetEngine()->SetLayerUVScale(i,uvSet[i]);
	}
}
void CUVSetDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateUVList(true);

	OnOK();
}

BOOL CUVSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateUVList(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
