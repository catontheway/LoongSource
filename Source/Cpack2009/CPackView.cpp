// CPackView.cpp : implementation of the CCPackView class
//

#include "stdafx.h"
#include "CPack.h"

#include "CPackDoc.h"
#include "CPackView.h"
#include "MainFrm.h"
#include "LeftView.h"
#include "ProgressDialog.h"

#include "cpkcore\disciomgr.h"
#include "cpkcore\cpkcore.h"
#include "cpkcore\util.h"
#include ".\cpackview.h"
#include "SortClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND  g_hWndFileName;
extern HWND  g_hWndProgress;
extern HWND  g_hWndInfo;

bool				g_bViewDestroyed = false;
std::list<CString>	g_sTempFile;

INT  CCPackView::m_iSortColumn = -1;
BOOL CCPackView::m_bAsc = TRUE;


#define    USER_INSERT_ITEM						WM_USER + 100


struct tagAddItem
{
	HWND  hWnd;
	INT   nCurrentThreadID;
};

volatile tagAddItem g_AddItemData;

struct tagInsertData
{
	INT nThreadID;
	INT nItemNum;
	DWORD *pdwCRCArray;
};


CRITICAL_SECTION  g_Lock;
UINT CALLBACK ThreadAddItem(LPVOID pData);


/////////////////////////////////////////////////////////////////////////////
// CCPackView

IMPLEMENT_DYNCREATE(CCPackView, CListView)

BEGIN_MESSAGE_MAP(CCPackView, CListView)
	//{{AFX_MSG_MAP(CCPackView)
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_NOTIFY_REFLECT(HDN_ITEMCHANGED, OnItemchanged)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	ON_WM_DROPFILES()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_MESSAGE( USER_INSERT_ITEM, OnAddItem )
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnLvnColumnclick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPackView construction/destruction

CCPackView::CCPackView()
{
	g_bViewDestroyed = false;
	m_nDirCounter = 0;
	m_nFileCounter = 0;
	m_nSelFileNum = 0;
	m_dwAllPackSize = 0;
	m_dwAllOriginSize = 0;

	InitializeCriticalSection(&g_Lock);
}

CCPackView::~CCPackView()
{
	g_bViewDestroyed = true;
	DeleteCriticalSection(&g_Lock);
}

BOOL CCPackView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// ɾ������Ԫ��
	g_sTempFile.clear();

    // Make sure the list control has the share image list style, so it won't
    // destroy the system image list when the dialog closes.
	cs.style |= LVS_REPORT | LVS_SHAREIMAGELISTS;
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCPackView drawing

void CCPackView::OnDraw(CDC* pDC)
{
	CCPackDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDC = pDC;
}



void CCPackView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	

	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
	DragAcceptFiles();
	
	CListCtrl& list = GetListCtrl();

	CCPackDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    list.SetImageList( pDoc->GetIconList(), LVSIL_SMALL );
	list.SetImageList( pDoc->GetBigIconList(), LVSIL_NORMAL );

    // Enable tooltips for items that aren't completely visible.
    list.SetExtendedStyle( LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT );

	while( list.DeleteColumn(0) );
	list.InsertColumn(0, "�ļ���", LVCFMT_LEFT, 270);
	list.InsertColumn(1, "ԭ��С", LVCFMT_RIGHT, 100);
	list.InsertColumn(2, "ѹ����", LVCFMT_RIGHT, 100);
	list.InsertColumn(3, "ѹ����", LVCFMT_RIGHT, 50);
	list.InsertColumn(4, "MD5ֵ", LVCFMT_RIGHT, 210);

	pDoc->UpdateAllViews(NULL);
}


void CCPackView::OnDestroy()
{
	CListView::OnDestroy();
	
	// ɾ������Ԥ����������ʱ�ļ�
	std::list<CString>::iterator p = g_sTempFile.begin();
	for(; p != g_sTempFile.end(); ++p )
	{
		CDiscIOMgr::DelFileOrDir((LPCTSTR)*p);
	}
}



/////////////////////////////////////////////////////////////////////////////
// CCPackView printing

BOOL CCPackView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCPackView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCPackView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCPackView diagnostics

#ifdef _DEBUG
void CCPackView::AssertValid() const
{
	CListView::AssertValid();
}

void CCPackView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CCPackDoc* CCPackView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCPackDoc)));
	return (CCPackDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCPackView message handlers
void CCPackView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
	lpStyleStruct = lpStyleStruct;
	nStyleType = nStyleType;
}


void CCPackView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	m_nSelFileNum = 0;
	m_dwAllPackSize = 0;
	m_dwAllOriginSize = 0;
	m_nFileCounter = 0;
	m_nDirCounter = 0;

	// TODO: Add your specialized code here and/or call the base class
	CCPackDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CPK* pCpk = pDoc->GetCpk();
	ASSERT(pCpk);

	CListCtrl& list = this->GetListCtrl();
	//list.DeleteAllItems();

	DWORD dwCurCPKDirCRC = pCpk->GetCurDirCRC();
	DWORD dwChildNum = 0;
	pCpk->_GetFirstLevelChild(dwCurCPKDirCRC, dwChildNum);

	// ���Ŀ¼���ļ�
	INT nTableIndex = 0;

	// ׼���߳�ȫ������
	g_AddItemData.hWnd = list.m_hWnd;
	g_AddItemData.nCurrentThreadID++;
	
	// ׼���߳�����
	tagInsertData* pInsertData = new tagInsertData;
	pInsertData->pdwCRCArray = new DWORD[dwChildNum];
	pInsertData->nThreadID = g_AddItemData.nCurrentThreadID;
	pInsertData->nItemNum = (INT)dwChildNum;

	for( INT n=0; n<(INT)dwChildNum; n++ )
	{
		nTableIndex = pDoc->GetCpk()->_GetTempArray(n);
		pInsertData->pdwCRCArray[n] = pCpk->_GetTable(nTableIndex).dwCRC;
		if( IS_DIR(pCpk->_GetTable(nTableIndex).dwFlag) )
			m_nDirCounter++;
	
	}
	m_nFileCounter = dwChildNum - m_nDirCounter;

	 //�����߳�
	HANDLE hInsertItem = (HANDLE)_beginthreadex(NULL, 0, ThreadAddItem, pInsertData, 0, NULL);
	CloseHandle(hInsertItem);

	// ��ʾ��ʾ��Ϣ
	TCHAR szTemp[256];
	wsprintf(szTemp, _T("��ǰ·������%d��Ŀ¼��%d���ļ�"), m_nDirCounter, m_nFileCounter);
	((CMainFrame*)this->GetParentFrame())->m_wndStatusBar.SetPaneText(0, szTemp);

	wsprintf(szTemp, _T("����%d����Ƭ,��Ƭ�ܿռ���%u byte"), 
		pCpk->GetFragmentNum(), pCpk->GetFragmentSpace());

	((CMainFrame*)this->GetParentFrame())->m_wndStatusBar.SetPaneText(1, szTemp);
}



// ��Ӵ���Ԫ�ص��߳�
UINT CALLBACK  ThreadAddItem(LPVOID pData)
{
	tagInsertData* pRealData = (tagInsertData*)pData;
	
	EnterCriticalSection(&g_Lock);
	::PostMessage(g_AddItemData.hWnd, LVM_DELETEALLITEMS, 0, 0L);

	// �ƺ�û��ʲôʵ������
	::SendMessage(g_AddItemData.hWnd, LVM_SETITEMCOUNT, pRealData->nItemNum, 0);

	// �������Ԫ��
	for(INT n=0; n<pRealData->nItemNum; n++ )
	{
		if(g_AddItemData.nCurrentThreadID != pRealData->nThreadID)
			goto thread_end;
	
		::PostMessage(g_AddItemData.hWnd, USER_INSERT_ITEM, pRealData->pdwCRCArray[n], n);
		Sleep(1);
	}

thread_end:
	delete[] pRealData->pdwCRCArray;
	delete pRealData;

	LeaveCriticalSection(&g_Lock);
	return 0;
}


void CCPackView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	DWORD dwCurCPKDirCRC = 0;
	TCHAR szFullDirName[MAX_PATH];
	TCHAR szTemp[MAX_PATH];

	CCPackDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// for short
	CListCtrl& list = this->GetListCtrl();

	switch( nChar )
	{
	case VK_DELETE:	// del ������
		{
			dwCurCPKDirCRC = pDoc->GetCpk()->GetCurDirCRC();
			pDoc->GetCpk()->GetFullNameFromCRC(szFullDirName, dwCurCPKDirCRC);
			
			POSITION pos = this->GetListCtrl().GetFirstSelectedItemPosition();
			if( pos == NULL )	// û���κ���Ŀ��ѡ��
				return;
			else
			{
				while( pos )
				{
					int nItem = this->GetListCtrl().GetNextSelectedItem(pos);
					lstrcpy(szTemp, szFullDirName);

					// ����Ŀ¼������ڰ��У���Ŀ¼����Ҫ��"\"��
					if (lstrlen(szFullDirName) > 0)
						lstrcat(szTemp, _T("\\"));

					lstrcat(szTemp, this->GetListCtrl().GetItemText(nItem, 0));
					pDoc->GetCpk()->RemoveFile(szTemp);
				}
			}

			// ���µõ�Ŀ¼�ṹ
			((CMainFrame*)this->GetParentFrame())->GetLeftPane()->ReAddAllDir();
			pDoc->UpdateAllViews(NULL);
		}
		break;
	case VK_BACK:
		{
			TCHAR szFullName[MAX_PATH];

			DWORD dwCRC = pDoc->GetCpk()->GetCurDirCRC();
			DWORD dwFatherCRC = pDoc->GetCpk()->GetFatherCRC(dwCRC);

			BOOL bResult = FALSE;
			bResult = pDoc->GetCpk()->GetFullNameFromCRC(szFullName, dwFatherCRC);
			ASSERT(bResult);

			pDoc->GetCpk()->SetCurDir(szFullName);
			pDoc->UpdateAllViews(NULL); // ��߸��»��Զ������ұ߸���
		}
		break;
	case VK_RETURN:
		{
			DWORD dwResult = 0; 
			OnDblclk(NULL, (LRESULT*)&dwResult);
		}
		break;
	case 'A': // VK_A
		{
			// Ctrl+A ѡ������Ԫ��
			if( (GetKeyState(VK_LCONTROL) & 0x80)
				|| (GetKeyState(VK_RCONTROL) & 0x80) )
			{
				for(int n=0; n<list.GetItemCount(); n++)
					list.SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
			}
		}
		break;

	default:
		{
			int n = GetKeyState(VK_LCONTROL);
			n=3;
		}
		break;
	}
	
	
	CListView::OnKeyDown(nChar, nRepCnt, nFlags);
}


// �ұߴ��ڷ���˫���¼�
void CCPackView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	DWORD dwCurCPKDirCRC = 0;
	TCHAR szFullDirName[MAX_PATH];
	TCHAR szTemp[MAX_PATH];

	CCPackDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	dwCurCPKDirCRC = pDoc->GetCpk()->GetCurDirCRC();
	pDoc->GetCpk()->GetFullNameFromCRC(szFullDirName, dwCurCPKDirCRC);
	
	POSITION pos = this->GetListCtrl().GetFirstSelectedItemPosition();
	if (pos == NULL)	// û���κ���Ŀ��ѡ��
		return;
	else
	{
		int nItem = this->GetListCtrl().GetNextSelectedItem(pos);
		lstrcpy(szTemp, szFullDirName);
		
		if (lstrlen(szTemp) > 0)	// �����Ǹ�Ŀ¼���Ͳ�Ҫ��"\"
			lstrcat(szTemp, _T("\\"));
		
		lstrcat(szTemp, this->GetListCtrl().GetItemText(nItem, 0));
		
		DWORD dwCRC = 0;
		pDoc->GetCpk()->GetCRCFromCPKFullName(dwCRC, szTemp);
		
		// ��Ŀ¼�ʹ�
		if (pDoc->GetCpk()->IsDir(dwCRC))
		{
			pDoc->GetCpk()->SetCurDir(szTemp);
			pDoc->UpdateAllViews(this);
		}
		else	// ���ļ���ִ��
		{
			// ������ʱ�ļ���ִ��
			TCHAR szTempPath[512];
			::GetTempPath(512, szTempPath);
			pDoc->GetCpk()->ExtractFile(szTempPath, szTemp);
			pDoc->GetCpk()->GetRelativeNameFromCRC(szTemp, dwCRC, dwCurCPKDirCRC);
			DWORD dwResult = (DWORD)::ShellExecute(NULL, "open", szTemp, NULL, szTempPath, SW_SHOWNORMAL);

			// ��¼��ʱ�ļ�,�Ա��˳�ʱɾ��
			strcat(szTempPath, szTemp);
			CString cs(szTempPath);
			g_sTempFile.push_back(cs);
		}
	}
	
	*pResult = 0;
}



void CCPackView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW *pNM = (NMLISTVIEW*) pNMHDR;
	
	// TODO: Add your control notification handler code here
	DWORD dwCurCPKDirCRC = 0;
	TCHAR szFullDirName[MAX_PATH];
	TCHAR szTemp[MAX_PATH];
	
	DWORD dwPackSize = 0, dwOriginSize = 0, dwCRC = 0;
	int nItem = 0;

	CCPackDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	nItem = pNM->iItem;
	
	// ����Ƿ���ѡ��״̬�ı�
	if( (pNM->uNewState & LVIS_SELECTED) == (pNM->uOldState & LVIS_SELECTED) )
	{
		return;
	}

	POSITION pos = this->GetListCtrl().GetFirstSelectedItemPosition();

	dwCRC = this->GetListCtrl().GetItemData(nItem);
	pDoc->GetCpk()->GetFileSize(dwPackSize, dwOriginSize, dwCRC);
	
	if( pNM->uNewState & LVIS_SELECTED )
	{
		m_nSelFileNum++;
		m_dwAllPackSize += dwPackSize;
		m_dwAllOriginSize += dwOriginSize;
	}
	else
	{
		m_nSelFileNum--;
		m_dwAllPackSize -= dwPackSize;
		m_dwAllOriginSize -= dwOriginSize;
	}


	if( m_nSelFileNum > 0 )	// ����Ŀ��ѡ��
	{
		if( m_nSelFileNum > 1 )
		{
			_stprintf(szTemp, _T("��ѡ��%d������"), m_nSelFileNum);
		}
		else
		{
			dwCurCPKDirCRC = pDoc->GetCpk()->GetCurDirCRC();
			pDoc->GetCpk()->GetFullNameFromCRC(szFullDirName, dwCurCPKDirCRC);

			if( lstrlen(szFullDirName) > 0 )	// ��Ŀ¼���Ͳ�Ҫ��"\"
				lstrcat(szFullDirName, _T("\\"));
				lstrcpy(szTemp, szFullDirName);
				lstrcat(szTemp, this->GetListCtrl().GetItemText(nItem, 0));
		}
		((CMainFrame*)this->GetParentFrame())->m_wndStatusBar.SetPaneText(0, szTemp);
		
		if( m_dwAllOriginSize == 0 || m_dwAllPackSize == 0 )
			sprintf(szTemp, "�ļ�����Ϊ��");
		else
		{
			if( m_dwAllOriginSize < 1024 || m_dwAllPackSize < 1024 )
			{
				sprintf(szTemp, "%lu�ֽ� ԭ%lu�ֽڣ�ѹ����%lu%%", m_dwAllPackSize, 
					m_dwAllOriginSize, (DWORD)((float)m_dwAllPackSize/m_dwAllOriginSize*100));
			}
			else
			{
				sprintf(szTemp, "%luKB ԭ%luKB��ѹ����%lu%%", m_dwAllPackSize / 1024, 
					m_dwAllOriginSize / 1024, (DWORD)((float)m_dwAllPackSize/m_dwAllOriginSize*100));
			}
		}
		((CMainFrame*)this->GetParentFrame())->m_wndStatusBar.SetPaneText(1, szTemp);
	}
	else	// û����Ŀ��ѡ��
	{
		sprintf(szTemp, "��ǰ·������%d��Ŀ¼��%d���ļ�", m_nDirCounter, m_nFileCounter);
		((CMainFrame*)this->GetParentFrame())->m_wndStatusBar.SetPaneText(0, szTemp);
		((CMainFrame*)this->GetParentFrame())->m_wndStatusBar.SetPaneText(1, "");
	}

	*pResult = 0;
}




void CCPackView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	this->GetListCtrl().Arrange(LVA_ALIGNTOP);
}



// ֧���û�ճ���ļ�������
void CCPackView::OnEditPaste() 
{
	// TODO: Add your command handler code here
	COleDataObject ole;
	
	// ole will auto deattach to clipboard at destruction time 
	BOOL bResult = ole.AttachClipboard();	
	if (bResult == FALSE)
		ASSERT(0);

	bResult = ole.IsDataAvailable(CF_HDROP);

	if (bResult == TRUE)	// have file paste
	{
		HGLOBAL h = ole.GetGlobalData(CF_HDROP);
		OnDropFiles((HDROP)h);
	}
}


HGLOBAL CCPackView::GetSelectedFileNameForDrop()
{
	HGLOBAL        hgDrop;
	DROPFILES*     pDrop;
	CStringList    lsDraggedFiles;
	POSITION       pos;
	INT            nSelItem;
	CString        sFile;
	UINT           uBuffSize = 0;
	TCHAR*         pszBuff;
	FORMATETC      etc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

	// ������ʱ�ļ�Ŀ¼��
	TCHAR szTempPath[512];
	::GetTempPath(512, szTempPath);
   
    // For every selected item in the list, put the filename into lsDraggedFiles.
    pos = GetListCtrl().GetFirstSelectedItemPosition();
    while( NULL != pos )
	{
		nSelItem = GetListCtrl().GetNextSelectedItem( pos );
		sFile = szTempPath;
		sFile += GetListCtrl().GetItemText( nSelItem, 0 );
        lsDraggedFiles.AddTail( sFile );

        // Calculate the # of chars required to hold this string.
        uBuffSize += lstrlen( sFile ) + 1;
	}

    // Add 2 extra for the final null char, and the size of the DROPFILES struct.
    uBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (uBuffSize + 2);

    // Allocate memory from the heap for the DROPFILES struct.
    hgDrop = GlobalAlloc( GHND | GMEM_SHARE, uBuffSize );
    if( NULL == hgDrop )
        return NULL;

    pDrop = (DROPFILES*) GlobalLock ( hgDrop );
    if( NULL == pDrop )
	{
		GlobalFree ( hgDrop );
		return NULL;
	}

    // Fill in the DROPFILES struct.
    pDrop->pFiles = sizeof(DROPFILES);
	pDrop->fWide = FALSE;
#ifdef _UNICODE
    pDrop->fWide = TRUE;
#endif

    // Copy all the filenames into memory after the end of the DROPFILES struct.
    pos = lsDraggedFiles.GetHeadPosition();
    pszBuff = (TCHAR*)(LPBYTE(pDrop) + sizeof(DROPFILES));
    while( NULL != pos )
	{
        lstrcpy( pszBuff, (LPCTSTR)lsDraggedFiles.GetNext(pos) );
        pszBuff = 1 + _tcschr( pszBuff, '\0' );
	}

    GlobalUnlock( hgDrop );
	return hgDrop;
}



BOOL CMyOleSource::OnRenderGlobalData(LPFORMATETC lpFormatEtc,HGLOBAL* phGlobal)
{
	if( g_bViewDestroyed )
		return FALSE;

	if( m_bReDoDrop )
		m_pView->DoRealDrop();

	m_bReDoDrop = FALSE;
	*phGlobal = m_pView->GetSelectedFileNameForDrop();
	return TRUE;
}


CMyOleSource g_ole(NULL);
// ֧���û��Ӱ��п����ļ�
void CCPackView::OnEditCopy() 
{
	// TODO: Add your command handler code here
	// ole will auto deattach to clipboard at destruction time 
	FORMATETC etc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

	// �ⲿ���Զ��ͷ�
	CMyOleSource* ole = new CMyOleSource(this);
	ole->DelayRenderData( CF_HDROP, &etc );
	ole->SetClipboard();
}




SCODE CDragSource::GiveFeedback(DWORD dwEffect)
{    
	m_dwLastEffect = dwEffect;    
	return DRAGDROP_S_USEDEFAULTCURSORS;
} 

SCODE CDragSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
{    
	// If ESC was pressed, cancel the drag. If the left button was released,    
	// do the drop.    
	if ( fEscapePressed )        
		return DRAGDROP_S_CANCEL;    
	else if ( !(grfKeyState & MK_LBUTTON) )        
	{        
		// If the last effect returned was NONE, then we don't do anything        
		// here because the target rejected the drag.        
		if ( DROPEFFECT_NONE == m_dwLastEffect )            
			return DRAGDROP_S_CANCEL;         
		// ......        
		// Actually do the file copying here, so that when we return, the files        
		// are in the temp dir & ready for Explorer to copy.        
		// ......         
		m_pView->DoRealDrop();
		return DRAGDROP_S_DROP;        
	}    
	else        
		return S_OK;
}


// package -> temp ->(auto) explore
void CCPackView::DoRealDrop()
{
	// ������ʾ���ȵ�
	CProgressDialog pd;
	pd.Create(IDD_PROGRESSDIALOG, NULL);
	pd.GetDlgItem(IDC_FILENAME, &g_hWndFileName);
	pd.GetDlgItem(IDC_PROGRESS1, &g_hWndProgress);
	pd.GetDlgItem(IDC_INFO, &g_hWndInfo);
	pd.ShowWindow(SW_SHOW);
	pd.UpdateWindow();
	pd.SetWindowText("�ͷŵ�ǰĿ¼��ѡ�����ļ�");
	
	POSITION pos = GetListCtrl().GetFirstSelectedItemPosition();
	
	// ������ʱ�ļ�Ŀ¼��
	TCHAR szTempPath[512];
	::GetTempPath(512, szTempPath);
	
	CCPackDoc* pDoc = GetDocument();
	TCHAR szFullCPKDirName[MAX_PATH];
	DWORD dwCRC = pDoc->GetCpk()->GetCurDirCRC();
	BOOL bResult = pDoc->GetCpk()->GetFullNameFromCRC(szFullCPKDirName, dwCRC);
	
	
	TCHAR szTemp[MAX_PATH];
	while (pos)
	{
		int nItem = GetListCtrl().GetNextSelectedItem(pos);
		
		lstrcpy(szTemp, szFullCPKDirName);
		if (lstrlen(szTemp) > 0)	// ��Ŀ¼���Ͳ�Ҫ��"\"
			lstrcat(szTemp, _T("\\"));
		
		// now: szTemp == FullName
		lstrcat(szTemp, GetListCtrl().GetItemText(nItem, 0));
		
		// �ͷ��ļ�
		BOOL bResult = pDoc->GetCpk()->ExtractFile(szTempPath, szTemp);
		if( FALSE == bResult )
		{
			Msg(_T("Couldn't Extract %s to %s"), szTemp, szTempPath);
			ASSERT(0);
		}
		else
		{
			pDoc->GetCpk()->GetCRCFromCPKFullName(dwCRC, szTemp);
			DWORD dwFatherSrcCRC = pDoc->GetCpk()->GetFatherCRC(dwCRC);
			pDoc->GetCpk()->GetRelativeNameFromCRC(szTemp, dwCRC, dwFatherSrcCRC);

			TCHAR szTempFileToDel[MAX_PATH];
			lstrcpy(szTempFileToDel, szTempPath);
			// ���Ƿ���Ҫ���"\"
			if (szTempFileToDel[lstrlen(szTempFileToDel)-1] != _T('\\'))
				lstrcat(szTempFileToDel, _T("\\"));
			lstrcat(szTempFileToDel, szTemp);

			// ��¼��ʱ�ļ�,�Ա��˳�ʱɾ��
			CString cs(szTempFileToDel);
			g_sTempFile.push_back(cs);
		}
	}
}



// �϶�
void CCPackView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW*    pNMLV = (NMLISTVIEW*) pNMHDR;
	COleDataSource datasrc;
	FORMATETC      etc = {CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
  
	*pResult = 0;   // return value ignored

	HGLOBAL hgDrop = this->GetSelectedFileNameForDrop();

    // Put the data in the data source.
    datasrc.CacheGlobalData( CF_HDROP, hgDrop, &etc );

	CDragSource source(this);
	DROPEFFECT dwEffect = datasrc.DoDragDrop
		( DROPEFFECT_COPY | DROPEFFECT_MOVE, NULL, &source );
}



void CCPackView::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	int nSize = DragQueryFile(hDropInfo, 0, NULL, 0);

	char* pTemp = new char[nSize + 100];
	UINT nFileNum = DragQueryFile(hDropInfo, 0xFFFFFFFF, pTemp, nSize + 100);

	CCPackDoc* pDoc = (CCPackDoc*)this->GetDocument();
	ASSERT(pDoc);
	DWORD dwFatherCRC = pDoc->GetCpk()->GetCurDirCRC();
	
	CProgressDialog pd;
	pd.Create(IDD_PROGRESSDIALOG, NULL);
	pd.GetDlgItem(IDC_FILENAME, &g_hWndFileName);
	pd.GetDlgItem(IDC_PROGRESS1, &g_hWndProgress);
	pd.GetDlgItem(IDC_INFO, &g_hWndInfo);
	pd.ShowWindow(SW_SHOW);
	pd.UpdateWindow();
	pd.SetWindowText("���������ļ�");

	for (int n=0; n<(int)nFileNum; n++)
	{
		DragQueryFile(hDropInfo, n, pTemp, nSize + 100);
		pDoc->GetCpk()->AddFile(pTemp, dwFatherCRC);
	}
	
	g_hWndFileName = NULL;
	g_hWndProgress = NULL;
	delete []pTemp;

	::DragFinish(hDropInfo);

	// ���µõ�Ŀ¼�ṹ
	((CMainFrame*)this->GetParentFrame())->GetLeftPane()->ReAddAllDir();
	pDoc->UpdateAllViews(NULL);	

	// ������Ĭ��ʵ��
	// CListView::OnDropFiles(hDropInfo);
}



void CCPackView::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	static bool bAscending = true;
	if( m_iSortColumn == pNMListView->iSubItem )
		bAscending = !bAscending;
	else
	{
		m_iSortColumn = pNMListView->iSubItem;
		bAscending = true;
	}
	
	if( pNMListView->iSubItem == 0 )
	{
		CSortClass csc( &(this->GetListCtrl()),  pNMListView->iSubItem, ESM_String);
		csc.Sort(bAscending);
	}
	else if( pNMListView->iSubItem == 1 )
	{
		CSortClass csc( &(this->GetListCtrl()), pNMListView->iSubItem, ESM_Value);
		csc.Sort(bAscending);
	}
	else if( pNMListView->iSubItem == 2 )
	{
		CSortClass csc( &(this->GetListCtrl()), pNMListView->iSubItem, ESM_Value);
		csc.Sort(bAscending);
	}
	else if( pNMListView->iSubItem == 3 )
	{
		CSortClass csc( &(this->GetListCtrl()), pNMListView->iSubItem, ESM_Percent);
		csc.Sort(bAscending);
	}

	*pResult = 0;

}


LRESULT	CCPackView::OnAddItem( WPARAM wParam, LPARAM lparam )
{
	DWORD dwCrc = (DWORD)wParam;
	INT	  nItem = (INT)lparam;

	char szName[MAX_PATH];
	BYTE digest[16];

	LVITEM item;
	item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	item.iItem = nItem;
	item.state = 0;
	item.stateMask = 0;
	item.lParam = dwCrc;
	item.pszText = szName;
	item.iImage = 0;

	SHFILEINFO sfi;
	CCPackDoc* pDoc = GetDocument();
	CPK* pCpk = pDoc->GetCpk();

	INT nTableIndex = pCpk->_GetTempArray(nItem);

	
	for( INT j = 0; j < 5; ++j )
	{

		item.iSubItem = j;
		if( j > 0 )
		{
			if( IS_DIR(pCpk->_GetTable(nTableIndex).dwFlag) )
			{
				if( j == 1 )
				{
					
					lstrcpy(szName, "Ŀ¼");
					GetListCtrl().SetItemText( nItem, j, szName );
				}
				continue;
			}

			DWORD dwBytes = 0;
			switch(j)
			{
			case 1: 
				dwBytes = pCpk->_GetTable(nTableIndex).dwOriginSize;
				if( dwBytes < 1024 )
					sprintf(szName, "1 KB");
				else
					sprintf(szName, "%lu KB", dwBytes/1024);
				break;
			case 2: 
				dwBytes = pCpk->_GetTable(nTableIndex).dwPackedSize; 
				if( dwBytes < 1024 )
					sprintf(szName, "1 KB");
				else
					sprintf(szName, "%lu KB", dwBytes/1024);
				break;
			case 3:
				if( pCpk->_GetTable(nTableIndex).dwPackedSize >= pCpk->_GetTable(nTableIndex).dwOriginSize )
					dwBytes = 0;
				else
					dwBytes = (DWORD)((float)(pCpk->_GetTable(nTableIndex).dwOriginSize - 
					pCpk->_GetTable(nTableIndex).dwPackedSize)
					/ pCpk->_GetTable(nTableIndex).dwOriginSize * 100.0f);
				sprintf(szName, "%lu%%", dwBytes);
				break;
			case 4:
				{
					ZeroMemory(szName, sizeof(szName));
					if( TRUE == pCpk->GetMD5FromTableIndex(digest, nTableIndex) )
					{
						for(int i=0; i<16; i++)
							sprintf(szName, "%s%02x", szName, digest[i]);
					}
				}
				break;
			}

			GetListCtrl().SetItemText( nItem, j, szName );
			continue;
		}


		pCpk->_GetSimpleFileName(szName, nTableIndex);

		if( IS_DIR(pCpk->_GetTable(nTableIndex).dwFlag) )
		{
			// �õ�Ŀ¼ͼ��
			SHGetFileInfo("*.", FILE_ATTRIBUTE_DIRECTORY | SFGAO_FOLDER, 
				&sfi, sizeof(sfi), SHGFI_SYSICONINDEX | SHGFI_ICON 
				| SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);

			item.iImage = sfi.iIcon;
			DestroyIcon(sfi.hIcon);
		}
		else
		{
			// ��ö�Ӧ�ļ����͵�ͼ��
			SHGetFileInfo(szName, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi),
				SHGFI_SYSICONINDEX | SHGFI_ICON | SHGFI_SMALLICON | 
				SHGFI_USEFILEATTRIBUTES);

			item.iImage = sfi.iIcon;
			DestroyIcon(sfi.hIcon);
		}
		GetListCtrl().InsertItem( &item );
		GetListCtrl().Update(nItem);
	}
	

	Sleep(0);
	return NULL;
}