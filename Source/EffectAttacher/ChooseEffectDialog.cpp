// ChooseEffectDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EffectAttacher.h"
#include "ChooseEffectDialog.h"
#include ".\chooseeffectdialog.h"
#include "Cool3DModels.h"


// CChooseEffectDialog �Ի���

IMPLEMENT_DYNAMIC(CChooseEffectDialog, CDialog)
CChooseEffectDialog::CChooseEffectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CChooseEffectDialog::IDD, pParent)
	, m_EffectAddress(_T(""))
	, m_AttachPoint(_T(""))
{
}

CChooseEffectDialog::~CChooseEffectDialog()
{
}

void CChooseEffectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EFFECTADDRESS_EDIT, m_EffectAddress_Edit);
	DDX_Control(pDX, IDC_EFFECTADDRESS_BUTTON, m_AddAddress_Button);
	DDX_Control(pDX, IDC_ATTACHPOINT_LIST, m_AttachPoint_List);
}


BEGIN_MESSAGE_MAP(CChooseEffectDialog, CDialog)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_EFFECTADDRESS_BUTTON, OnBnClickedEffectaddressButton)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CChooseEffectDialog ��Ϣ�������

int CChooseEffectDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	
	return 0;
}

bool CChooseEffectDialog::UpdateAttachPointList(void)
{
	bool bRet =false;

	m_AttachPoint_List.ResetContent();
	AnimationCtrl *pAni=Cool3DModels::Inst()->GetSceneNodeAni();
	if( pAni == NULL )
		return bRet;

	if( pAni->IS_STATIC_CLASS(NodeAnimationCtrl) )
	{
		AvatarNode *pNode = static_cast<AvatarNode*>(Cool3DModels::Inst()->GetSceneNode());		;
		const SkeletonStruct& skelStruct=pNode->GetResource()->GetStruct();
		skelStruct.m_boneNames;
		for( UINT i=0; i<skelStruct.m_boneNames.size(); i++ )
		{
			m_AttachPoint_List.AddString( skelStruct.m_boneNames[i].c_str() );
		}

		bRet = true;
	}
	else if( pAni->IS_STATIC_CLASS(VertexAnimationCtrl) )
	{
		VertexAnimationCtrl *pVertexCtrl = static_cast<VertexAnimationCtrl*>(pAni);

		tstring tagNodeName;
		const vector<tagTagNodeSpace>& tagNodes=pVertexCtrl->GetTagNodes();
		for(vector<tagTagNodeSpace>::const_iterator iter=tagNodes.begin();
			iter!=tagNodes.end();++iter)
		{
			tagNodeName = _FourCC2Str(iter->name);
			m_AttachPoint_List.AddString(tagNodeName.c_str());
		}

		bRet = true;
	}

	return false;
}

void CChooseEffectDialog::OnBnClickedEffectaddressButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog addressDialog(TRUE, NULL, NULL,
								OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
								NULL,this,0);
	if(addressDialog.DoModal())
	{
		UpdateData();
		m_EffectAddress = addressDialog.GetPathName();
		UpdateData(FALSE);
	}

	m_EffectAddress_Edit.SetWindowText(m_EffectAddress);
}

void CChooseEffectDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool bOK=true;

	if(m_AttachPoint_List.GetCount())
	{ 
		if( m_AttachPoint_List.GetCurSel() == LB_ERR )
		{
			AfxMessageBox(_T("��ѡ��һ�����ص�!"));
			bOK=false;
		}
		else
            
			m_AttachPoint_List.GetText(m_AttachPoint_List.GetCurSel(),m_AttachPoint);
	}

	//ת�������·��
	m_EffectAddress.MakeLower();
	CString szAppPath=Kernel::Inst()->GetWorkPath();
	szAppPath.MakeLower();
	int num=m_EffectAddress.Replace(szAppPath,_T(""));
	if(num!=0)
	{
		UpdateData(FALSE);
	}

	if(bOK)
		OnOK();
}

void CChooseEffectDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	UpdateAttachPointList();
	// TODO: �ڴ˴������Ϣ����������
}
