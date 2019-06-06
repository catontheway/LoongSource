// EffectAttacherDoc.cpp :  CEffectAttacherDoc ���ʵ��
//

#include "stdafx.h"
#include "EffectAttacher.h"
#include "EffectAttacherDoc.h"

#include "Cool3DModels.h"

#include "EffectAttacherView.h"
#include ".\effectattacherdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEffectAttacherDoc

IMPLEMENT_DYNCREATE(CEffectAttacherDoc, CDocument)

BEGIN_MESSAGE_MAP(CEffectAttacherDoc, CDocument)
END_MESSAGE_MAP()


// CEffectAttacherDoc ����/����

CEffectAttacherDoc::CEffectAttacherDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CEffectAttacherDoc::~CEffectAttacherDoc()
{
}

BOOL CEffectAttacherDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}


// CEffectAttacherDoc ���л�

void CEffectAttacherDoc::Serialize(CArchive& ar)
{
	const CFile *pFile=ar.GetFile();
	CString szPath=pFile->GetFilePath();

	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
		Cool3DModels::Inst()->loadModel(szPath);
		int result  = szPath.Find(_T(".fskel"));
		if(result != -1)
			szPath.Replace(_T(".fskel"), _T(".csv"));
		result = szPath.Find(_T(".FSKEL"));
		if(result != -1)
			szPath.Replace(_T(".FSKEL"), _T(".csv"));
		// del by bwen: 2006.4.11
		//result = szPath.Find(".FAK");
		//if(result != -1)
		//	szPath.Replace(".FAK", ".csv");
		//result = szPath.Find(".fak");
		//if(result != -1)
		//	szPath.Replace(".fak", ".csv");

		if(szPath.Find(_T(".csv")) != -1)
		{
			if(m_cstrCSVFilePath != szPath)
			{
				m_aniMsgTable.Clear();
				//g_viewWnd->m_actMsgDlg.ClearAll();
			}

			m_aniMsgTable.LoadFromFile(Kernel::Inst()->DefaultFS(), szPath);
			m_cstrCSVFilePath = szPath;
		}
	}

	AnimationCtrl *pAni=Cool3DModels::Inst()->GetSceneNodeAni();

	/*
	//��ʼ��һ��Track
	if(pAni!=NULL)
	{
		//pAni->OpenTrack("walk", EAni_Loop);
		pAni->SetTimeScale(1);
	}
	else
		AfxMessageBox("77886699467");
	*/

	//CListBox * MainWndTrackList = (CListBox *) AfxGetApp()->GetMainWnd()->GetDescendantWindow(IDC_TRACK_LIST);
	//MainWndTrackList->AddString("123123");
}

AniMsgTable * CEffectAttacherDoc::GetAniMsgTable()
{
	return &m_aniMsgTable;
}

CString CEffectAttacherDoc::GetCSVFilePath()
{
	return m_cstrCSVFilePath;
}

// CEffectAttacherDoc ���

#ifdef _DEBUG
void CEffectAttacherDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEffectAttacherDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CEffectAttacherDoc ����

void CEffectAttacherDoc::OnCloseDocument()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(!Cool3DModels::Inst()->ApplicationClose())
	{
		AfxMessageBox(_T("�˳�ʱ�����������⣡"));
	}


	CDocument::OnCloseDocument();
}
