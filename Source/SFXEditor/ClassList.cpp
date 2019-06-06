// ClassList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SFXEditor.h"
#include "ClassList.h"
#include ".\classlist.h"
#include "SFXPlug.h"


// CClassList �Ի���

IMPLEMENT_DYNAMIC(CClassList, CDialog)
CClassList::CClassList(CWnd* pParent /*=NULL*/)
	: CDialog(CClassList::IDD, pParent)
{
}

CClassList::~CClassList()
{
}

void CClassList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ClassList, m_hList);
}


BEGIN_MESSAGE_MAP(CClassList, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CClassList ��Ϣ�������

BOOL CClassList::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	const TCHAR* const szSuperClass=_T("EffectBase");
	const TCHAR* const szPathSuperClass=_T("EffectPathCtrl");
	for(UINT i=0;i<RTTFactory::Inst()->GetNumClass();i++)
	{
		RTTInfo* pClass=const_cast<RTTInfo*>(RTTFactory::Inst()->GetClass(i));
		if(pClass->IsKindOf(szSuperClass)
			&& _tcsicmp(pClass->m_szClassName,szSuperClass)!=0)//�ų������
		{
			if(pClass->IsKindOf(szPathSuperClass))
			{
				if(_tcsicmp(pClass->m_szClassName,szPathSuperClass)!=0)
				{
					CString szTmp;//"·��: ";
					szTmp+=pClass->m_szClassName;
					m_hList.AddString(szTmp);
				}
			}
			else
			{
				CString szTmp;//="Ч��: ";
				szTmp+=pClass->m_szClassName;
				m_hList.AddString(szTmp);
			}
		}
	}
	m_hList.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CClassList::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel=m_hList.GetCurSel();
	if(sel==-1)
	{
		AfxMessageBox(_T("�����б���ѡ��һ������"));
		return;
	}

	m_hList.GetText(sel,m_szSelectClass);

	OnOK();
}
