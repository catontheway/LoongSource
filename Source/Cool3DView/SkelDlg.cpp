// SkelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Cool3DView.h"
#include "..\Cool3D\Cool3DEngine.h"
using namespace Cool3D;

#include "SkelDlg.h"

//--�ݹ���
HTREEITEM AddBone2Tree(CTreeCtrl *pTreeCtrl,int boneID,HTREEITEM hTreeItem, HTREEITEM hPreviousIetm,
				  const SkeletonStruct& skelStruct)
{
	ASSERT(boneID>=0 && boneID<(int)skelStruct.m_boneStruct.size());
	const BoneStruct& boneStruct=skelStruct.m_boneStruct[boneID];
	const TCHAR* szBoneName=skelStruct.m_boneNames[boneID].c_str();
	HTREEITEM hNewItem = pTreeCtrl->InsertItem( szBoneName,0, 0, hTreeItem, hPreviousIetm);
	HTREEITEM hNextItem = NULL;
	for(size_t i=0;i<boneStruct.children.size();i++)
	{
		hNextItem = AddBone2Tree(pTreeCtrl,boneStruct.children[i],hNewItem,hNextItem,skelStruct);
	}
	pTreeCtrl->Expand(hNewItem,TVE_EXPAND);
	return hNewItem;
}
//-- �����Ǽ����ͽṹ  �����ع�������
int BuildSkeletonTree(CTreeCtrl *pTreeCtrl,ResSkeleton *pRes)
{

	pTreeCtrl->DeleteAllItems();
	
	const SkeletonStruct& skelStruct=pRes->GetStruct();
	HTREEITEM hCountry = pTreeCtrl->InsertItem(pRes->GetName(), NULL, TVI_SORT);
	HTREEITEM hNextItem = NULL;

	for(size_t i=0;i<skelStruct.m_rootBones.size();i++)
	{
		AddBone2Tree(pTreeCtrl,skelStruct.m_rootBones[i],
			hCountry, hNextItem,skelStruct);
	}
	//- չ�����νṹ
	pTreeCtrl->Expand(hCountry,TVE_EXPAND);
	return (pTreeCtrl->GetCount() -1);
}

// CSkelDlg �Ի���

IMPLEMENT_DYNAMIC(CSkelDlg, CDialog)
CSkelDlg::CSkelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkelDlg::IDD, pParent)
	, m_TagNodes(_T(""))
{
	m_pRes=NULL;
}

CSkelDlg::~CSkelDlg()
{
}

void CSkelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TagNodes, m_TagNodes);
}


BEGIN_MESSAGE_MAP(CSkelDlg, CDialog)
END_MESSAGE_MAP()


// CSkelDlg ��Ϣ�������

BOOL CSkelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(m_pRes)
	{
		CRect rc;
		GetClientRect(rc);
		rc.bottom-=100;
		m_skelTree.Create(WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER
		| TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES
		| TVS_DISABLEDRAGDROP,
		rc, this, 0x1005);
		BuildSkeletonTree(&m_skelTree,m_pRes);

		//--��ʾtagnodes������
		SkeletonKeyFrame *pSF=m_pRes->GetFrame(0);
		if(pSF!=NULL)
		{
			m_TagNodes=_T("");
			const TagNodeFrame& tagNodes=pSF->m_tagNodes;
			for(size_t i=0;i<tagNodes.nodes.size();i++)
			{
				m_TagNodes+=_T('{');
				m_TagNodes+=_FourCC2Str(tagNodes.nodes[i].name).c_str();
				if(tagNodes.nodes[i].bMirrored)
					m_TagNodes+=_T(" Mirrored");
				m_TagNodes+=_T("}  ");
			}
			UpdateData(FALSE);
		}

		UpdateWindow();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
