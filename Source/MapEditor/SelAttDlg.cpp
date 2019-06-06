// SelAttDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MapEditor.h"
#include "SelAttDlg.h"
#include ".\selattdlg.h"
#include "PlugMapEditor.h"


// CSelAttDlg �Ի���

IMPLEMENT_DYNAMIC(CSelAttDlg, CDialog)
CSelAttDlg::CSelAttDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelAttDlg::IDD, pParent)
{
	m_szTitle=_T("_RuntimeTitle");
	m_sel=0;
	m_dwLastNPCAttID=-1;
}

CSelAttDlg::~CSelAttDlg()
{
}

void CSelAttDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AttList, m_hList);
}


BEGIN_MESSAGE_MAP(CSelAttDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_AttList, OnLvnItemchangedAttlist)
END_MESSAGE_MAP()


// CSelAttDlg ��Ϣ�������

BOOL CSelAttDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText(m_szTitle);
	m_hList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_hList.InsertColumn(0, _T("����ID"),	LVCFMT_LEFT,  64);
	m_hList.InsertColumn(1, _T("��������"),	LVCFMT_LEFT,  190);

	CreatureDataReader* pDB = g_mapEditor->GetMap()->GetAttDataBase();
	ASSERT(pDB!=NULL);
	
	int lastSel=-1;
	switch(m_objType)
	{
	case MapObjEdit::NPC:
		{
			CString attID;
			map<DWORD,tagCreatureProto> creatureMap;
			pDB->GetCreatureMap(creatureMap);
			int n=0;
			for(map<DWORD,tagCreatureProto>::iterator iter=creatureMap.begin();
				iter!=creatureMap.end(); iter++)
			{
				tagCreatureProto* pAtt = &iter->second;
				attID.Format(_T("%d"),pAtt->dwTypeID);
				m_hList.InsertItem(n,attID);
				m_hList.SetItemText(n,1,pAtt->szName);

				//--���ϴ�ѡ�е��Ǹ�
				if(pAtt->dwTypeID == m_dwLastNPCAttID)
				{
					lastSel=n;
				}
				n++;
			}
		}
		break;
	}

	if(lastSel!=-1)
	{
		m_hList.SetItemState(lastSel,LVIS_SELECTED|LVIS_DROPHILITED,LVIS_SELECTED|LVIS_DROPHILITED);
		
		CRect rect;
		m_hList.GetItemRect(0, &rect, LVIR_BOUNDS);
		int itemheight=rect.bottom-rect.top;

		CSize size;
		size.cx=0;
		size.cy=lastSel*itemheight;
		m_hList.Scroll(size);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSelAttDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos=m_hList.GetFirstSelectedItemPosition();
	if(pos==NULL)
	{
		AfxMessageBox(_T("����ѡ��һ������!"));
		return;
	}
	int item=m_hList.GetNextSelectedItem(pos);

	 CString szID=m_hList.GetItemText(item,0);
	 m_sel=_tstoi(szID);
	
	OnOK();
}

void CSelAttDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CSelAttDlg::readstr(FILE *f,TCHAR *szString)
{
	do
	{
	   if(feof(f))  return;
	   ZeroMemory(szString,sizeof(szString));
	   _fgetts(szString, 255, f);
	} 
	while (((szString[0] == _T('/')) && (szString[1] == _T('/'))) || (szString[0] == _T('\n')));
	return;
}


void CSelAttDlg::OnLvnItemchangedAttlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos=m_hList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int item=m_hList.GetNextSelectedItem(pos);
		for(int i=0;i<m_hList.GetItemCount();i++)
		{
			if(i!=item)
				m_hList.SetItemState(i,0, LVIS_SELECTED|LVIS_DROPHILITED);	
		}
		
	}
	
	*pResult = 0;
}
