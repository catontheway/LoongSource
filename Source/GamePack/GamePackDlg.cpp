// GamePackDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GamePack.h"
#include "GamePackDlg.h"
#include ".\gamepackdlg.h"

#include "cpkcore\cpkcore.h"
#include "memmgr\memmgr.h"
#include "cpkcore\disciomgr.h"
#include "filtermgr\filtermgr.h"
#include "cpkcore\util.h"
#include "SelfExtract.h"

#include "md5\md5.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMemMgr		MemMgr(TRUE);
CDiscIOMgr	DiscMgr();
CFilterMgr	FilterMgr();


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGamePackDlg �Ի���



CGamePackDlg::CGamePackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGamePackDlg::IDD, pParent), m_Trunk(this)
	, m_bAddexe(FALSE)
{
	m_strVersionSrc.Empty();
	m_strVersionDest.Empty();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CGamePackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_VERSION_MIN, m_EditVersionSrc);
	DDX_Control(pDX, IDC_EDIT_VERSION_MAX, m_EditVersionDest);
	DDX_Text(pDX, IDC_EDIT_VERSION_MIN, m_strVersionSrc);
	DDX_Text(pDX, IDC_EDIT_VERSION_MAX, m_strVersionDest);
}

BEGIN_MESSAGE_MAP(CGamePackDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BT_NEWDIR, OnBnClickedBtNewdir)
	ON_BN_CLICKED(IDC_BT_OLDDIR, OnBnClickedBtOlddir)
	ON_BN_CLICKED(IDC_BT_SAVEDIR, OnBnClickedBtSavedir)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_UPDATE, OnBnClickedCheckUpdate)
	ON_STN_CLICKED(IDC_ST_FILE, OnStnClickedStFile)
	ON_EN_CHANGE(IDC_EDIT_VERSION_MIN, OnEnChangeEditVersionMin)
	ON_EN_CHANGE(IDC_EDIT_VERSION_MAX, OnEnChangeEditVersionMax)
	ON_EN_CHANGE(IDC_ET_SAVEDIR, OnEnChangeEtSavedir)
END_MESSAGE_MAP()


// CGamePackDlg ��Ϣ�������

BOOL CGamePackDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_Progress.Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH, CRect(40, 200, 397, 226), this, 1);
	m_Progress.SetRange(0, 100);
	m_Progress.SetPos(0);

	m_pETOld = (CEdit*)GetDlgItem(IDC_ET_OLDDIR);
	m_pETNew = (CEdit*)GetDlgItem(IDC_ET_NEWDIR);
	m_pETSave = (CEdit*)GetDlgItem(IDC_ET_SAVEDIR);
	m_pStatic = (CStatic*)GetDlgItem(IDC_ST_FILE);
	m_pSTimer = (CStatic*)GetDlgItem(IDC_ST_TIME);
//��ע����ȡ
	TCHAR szOldPath[MAX_PATH], szNewPath[MAX_PATH], szSavePath[MAX_PATH];
	ReadReg("SOFTWARE\\TENGREN\\LOONG\\UPDATE", "default_new_path", 
		szNewPath);
	ReadReg("SOFTWARE\\TENGREN\\LOONG\\UPDATE", "default_old_path", 
		szOldPath);
	
	ReadReg("SOFTWARE\\TENGREN\\LOONG\\UPDATE", "default_save_path", 
		szSavePath);

	CString strTemp;
	CString strPath = szSavePath;
	INT nPos = strPath.ReverseFind( _T('.'));
	if( nPos != -1 )
	{
		strTemp = strPath.Left( nPos + 1 );
		strTemp += "cpk";
		
	}

	m_pETOld->SetWindowText(szOldPath);
	m_pETNew->SetWindowText(szNewPath);

	m_pETSave->SetWindowText( strTemp.GetBuffer() );

	ASSERT( m_pETSave && m_pETNew && m_pETOld );
	//��ʼ��
	m_pCpk = new CSelfExtract;
	ASSERT( m_pCpk );
	m_pMD5 = new CMD5;
	ASSERT( m_pMD5 );
	m_pNewCpk = NULL;
	m_pOldCpk = NULL;
	m_bTerminateGamePack = FALSE;
	m_bPacking = FALSE;
	m_hGamePackThread = INVALID_HANDLE_VALUE;


	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CGamePackDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGamePackDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CGamePackDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//------------------------------------------------
//ѡ��汾�ȶԵ��°汾Ŀ¼
//------------------------------------------------
void CGamePackDlg::OnBnClickedBtNewdir()
{
	CString sFolderPath;
	BROWSEINFO bi;
	TCHAR szPath[MAX_PATH];
	ZeroMemory(szPath, sizeof(szPath));
	m_pETNew->GetWindowText(szPath, MAX_PATH);
	//��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;			//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = "��ѡ���°汾·��";  //������
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.iImage	=	IDR_MAINFRAME;
	//��ʼ����ڲ���bi����

	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if(pIDList)
	{
		//ȡ���ļ���·����Buffer��
		SHGetPathFromIDList(pIDList, szPath);

		m_pETNew->SetWindowText(szPath);
	}
	
	LPMALLOC lpMalloc;
	if(FAILED(SHGetMalloc(&lpMalloc))) 
		return;
	//�ͷ��ڴ�
	lpMalloc->Free(pIDList);
	lpMalloc->Release();
	
}
//------------------------------------------------
//ѡ��汾�ȶԵ��ϰ汾Ŀ¼
//------------------------------------------------
void CGamePackDlg::OnBnClickedBtOlddir()
{
	CString sFolderPath;
	BROWSEINFO bi;
	TCHAR szPath[MAX_PATH];
	ZeroMemory(szPath, sizeof(szPath));

	//��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = "��ѡ���ϰ汾·��";		//
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.iImage	=	IDR_MAINFRAME;
	//��ʼ����ڲ���bi����

	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if(pIDList)
	{
		//ȡ���ļ���·����Buffer��
		SHGetPathFromIDList(pIDList, szPath);
		m_pETOld->SetWindowText(szPath);
	}

	LPMALLOC lpMalloc;
	if(FAILED(SHGetMalloc(&lpMalloc)))
		return;
	//�ͷ��ڴ�
	lpMalloc->Free(pIDList);
	lpMalloc->Release();	
}
//------------------------------------------------
//ѡ���������Ĵ洢Ŀ¼
//------------------------------------------------
void CGamePackDlg::OnBnClickedBtSavedir()
{
	CString sFolderPath;
	BROWSEINFO bi;
	TCHAR szPath[MAX_PATH];
	ZeroMemory(szPath, sizeof(szPath));

	//��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = "��ѡ���������洢·��";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.iImage	=	IDR_MAINFRAME;
	//��ʼ����ڲ���bi����

	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if(pIDList)
	{
		//ȡ���ļ���·����Buffer��
		SHGetPathFromIDList(pIDList, szPath);
		INT nLen = _tcslen(szPath);
		ASSERT( nLen > 0 );
		if( szPath[nLen-1] != '\\' )
		{
			strcat(szPath, "\\");
		}

		strcat(szPath, "update.cpk");

		m_pETSave->SetWindowText(szPath);
	}

	LPMALLOC lpMalloc;
	if(FAILED(SHGetMalloc(&lpMalloc)))
		return;
	//�ͷ��ڴ�
	lpMalloc->Free(pIDList);
	lpMalloc->Release();	
}
//------------------------------------------------
//��ʼ���
//------------------------------------------------
void CGamePackDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	m_Progress.SetPos(0);

	m_pStatic->SetWindowText("");
	m_pSTimer->SetWindowText("0 ms");

	if( m_bAddexe )
	{
	/*	CString	strPath, strTemp, strVersionMin, strVersionMax;
		m_pETSave->GetWindowText( strPath );;
		m_EditVersionMin.GetWindowText( strVersionMin );
		m_EditVersionMax.GetWindowText( strVersionMax );
		INT nPos = strPath.ReverseFind( _T('.') ) ;
		strTemp = strPath.Left( nPos );
		strTemp += " ";
		strTemp += strVersionMin;
		strTemp += "-";
		strTemp += strVersionMax;s
		strTemp += strPath.Right( strPath.GetLength() - nPos );

		m_pETSave->SetWindowText( strTemp.GetBuffer() );*/
		if(  m_strVersionSrc.IsEmpty() || m_strVersionDest.IsEmpty() )
		{
			MessageBox( "�汾�Ų���Ϊ��" );
			return ;
		}
	}

	GetDlgItem(IDOK)->EnableWindow(FALSE);	
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	
	//��������߳�
	::InterlockedExchange((LONG*)&m_bTerminateGamePack, FALSE);
	m_hGamePackThread = (HANDLE)_beginthreadex
		(NULL, 0, (THREADPROC)m_Trunk.sfp1(&CGamePackDlg::GamePackThread), 
		NULL, 0, NULL);
	ASSERT( m_hGamePackThread != (HANDLE)(-1L) );
}
//------------------------------------------------
//ȡ�����
//------------------------------------------------
void CGamePackDlg::OnBnClickedCancel()
{
	::InterlockedExchange((LONG*)&m_bTerminateGamePack, TRUE);
}
//------------------------------------------------
//�رճ���
//------------------------------------------------
void CGamePackDlg::OnClose()
{
	if( TRUE == m_bPacking )
	{
		::InterlockedExchange((LONG*)&m_bTerminateGamePack, TRUE);
		Sleep(50);
	}
	FREE(m_pCpk);
	FREE(m_pMD5);
	FREE(m_pNewCpk);
	FREE(m_pOldCpk);
	m_Trunk.sfp_clear();

	CString csOldPath, csNewPath, csSavePath;
	m_pETOld->GetWindowText(csOldPath);
	m_pETNew->GetWindowText(csNewPath);
	m_pETSave->GetWindowText(csSavePath);

	WriteReg("SOFTWARE\\TENGREN\\LOONG\\UPDATE", "default_new_path", 
		csNewPath.GetBuffer());
	WriteReg("SOFTWARE\\TENGREN\\LOONG\\UPDATE", "default_old_path", 
		csOldPath.GetBuffer());
	
	WriteReg("SOFTWARE\\TENGREN\\LOONG\\UPDATE", "default_save_path", 
		csSavePath.GetBuffer());
	CDialog::DestroyWindow();
}
//------------------------------------------------
//�ؼ�����:�汾����߳�
//------------------------------------------------
static INT s_nProcessPos = 0;
static INT s_dwTick		 = 0;
//------------------------------------------------
UINT CGamePackDlg::GamePackThread(LPVOID)
{
	m_bPacking = TRUE;


	//��ȡ�����汾��Ŀ¼���������洢·��
	TCHAR szOldPath[MAX_PATH];
	TCHAR szNewPath[MAX_PATH];
	TCHAR szSavePath[MAX_PATH];
	TCHAR szCurrentPath[MAX_PATH];
	TCHAR szSaveFullPath[MAX_PATH];

	ZeroMemory(szOldPath, sizeof(szOldPath));
	ZeroMemory(szNewPath, sizeof(szNewPath));
	ZeroMemory(szSavePath, sizeof(szSavePath));
	ZeroMemory(szSaveFullPath, sizeof(szSaveFullPath));
	ZeroMemory(szCurrentPath, sizeof(szCurrentPath));

	//��Ҫȥ��·��ĩβ��'\\'
	INT nStrSize = m_pETOld->GetWindowText(szOldPath, MAX_PATH);
	if( nStrSize && szOldPath[nStrSize-1] == '\\' )
		szOldPath[nStrSize-1] = 0;
	nStrSize = m_pETNew->GetWindowText(szNewPath, MAX_PATH);
	if( nStrSize && szNewPath[nStrSize-1] == '\\' )
		szNewPath[nStrSize-1] = 0;

	nStrSize = m_pETSave->GetWindowText(szSaveFullPath, MAX_PATH);
	if( 0 == nStrSize )
	{
		//������Ŀ¼������
		AfxMessageBox(_T("�������ļ������ò���Ϊ��"));

		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_bPacking = FALSE;
		return 0;
	}

	INT i=0;
	for(i=nStrSize-1; i>0; i--)
	{
		if( szSaveFullPath[i] == '\\' )
			break;
	}
	memcpy(szSavePath, szSaveFullPath, i);
	if( i && szSavePath[i-1] == '\\' )
		szSavePath[i-1] = 0;
	

	if( FALSE == CDiscIOMgr::IsDir(szOldPath) )
	{
		//������Ŀ¼������
		AfxMessageBox(_T("ָ���ľ�Ŀ¼������"));

		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_bPacking = FALSE;
		return 0;
	}

	if( FALSE == CDiscIOMgr::IsDir(szNewPath) )
	{
		//������Ŀ¼������
		AfxMessageBox(_T("ָ������Ŀ¼������"));

		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_bPacking = FALSE;
		return 0;
	}

	if( FALSE == CDiscIOMgr::IsDir(szSavePath) )
	{
		//������Ŀ¼������
		AfxMessageBox(_T("ָ�����������ļ�����Ŀ¼������"));

		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_bPacking = FALSE;
		return 0;
	}


	std::string strOldPath(szOldPath);
	std::string strNewPath(szNewPath);
	
	if( strOldPath.find(strNewPath, 0) != strOldPath.npos ||
		strNewPath.find(strOldPath, 0) != strOldPath.npos )
	{
		//����¾ɰ汾Ŀ¼�а�����ϵ���ƺ����������
		strOldPath.resize(0);
		strNewPath.resize(0);
		AfxMessageBox(_T("�¾ɰ汾����Ŀ¼�������ϵ, �޷��ȶ�"));

		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_bPacking = FALSE;

		return FALSE;
	} 

	//���Կ�ʼ�ȶ���

	if( TRUE == CDiscIOMgr::IsExist(szSaveFullPath) )
	{ 
		INT nRet = MessageBox(_T("�ò����ļ��Ѿ�����, �Ƿ񸲸���������?"), _T("GamePack"), MB_OKCANCEL);
		if( nRet == IDCANCEL )
		{
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_bPacking = FALSE;
			return FALSE;			
		}

		//ɾ���ɵ�������
		BOOL bRet = CDiscIOMgr::DelFileOrDir(szSaveFullPath);
		ASSERT( bRet );
		if( bRet == FALSE )
		{
			MessageBox(_T("�����ļ��Ѵ���,��������ʹ�ã��޷�ɾ��"), _T("GamePack"), MB_OK);
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_bPacking = FALSE;
			return FALSE;
		}
	}

	if( FALSE == m_pCpk->CreateSaveFile(szSaveFullPath, m_bAddexe) )
	{
		MessageBox(_T("�޷����������ļ�"), _T("GamePack"), MB_OK);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_bPacking = FALSE;
		return FALSE;
	}


	DWORD dwNewFileNum = CDiscIOMgr::GetFileNumInDir(szNewPath);
	m_fIntePos = ((FLOAT)100) / dwNewFileNum;  //Ԥ��������
	m_nFileNum = 0;

	::GetCurrentDirectory(MAX_PATH, szCurrentPath);
	::SetCurrentDirectory(szNewPath);  //���ù���·��Ϊ��ǰ�°汾·��

	//***----------------------****************------------------------***/
	s_nProcessPos = 0;
	s_dwTick = GetTickCount();
	m_dwTick = GetTickCount();

	BOOL ret = GamePack("", (TCHAR*)szOldPath);	//������
	m_pCpk->WriteTail( m_strVersionSrc, m_strVersionDest );						//д���ļ���ȡ��ַ
	m_pCpk->Unload();
	//***----------------------****************------------------------***/

	if( FALSE == m_bTerminateGamePack )		//ȷ���������,���⸡�����������
		m_Progress.SetPos(100);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	::SetCurrentDirectory(szCurrentPath);	//�ָ�����·��

	m_bPacking = FALSE;

	CString strTxtPath = szSaveFullPath;
	INT nPos = strTxtPath.ReverseFind( '\\' );
	if( nPos == -1 )
	{
		return ret;
	}
	INT nExtPos = strTxtPath.ReverseFind( '.' );
	if( nExtPos == -1 )
	{
		return ret;
	}

	strTxtPath.Delete( nExtPos - 1, strTxtPath.GetLength() - nExtPos + 1 );
	strTxtPath += "_md5.txt";

	//���㲹������MD5ֵ����д���ļ�
	DWORD dwHandle = CDiscIOMgr::Create( strTxtPath.GetBuffer(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
	if( dwHandle == GT_INVALID )
	{
		MessageBox( "����md5ֵ�ļ��������ֹ�����" );
		return ret;
	}

	//��ʼ����MD5ֵ szSaveFullPath;

	BYTE  byMd5[16] = {0};
	BOOL bRet =  CMD5::MD5ForFile( szSaveFullPath, byMd5 );
	if( !bRet )
	{
		MessageBox( "����MD5ֵ����" );
		return ret;
	}

	char szMess[512] = {0};
	for(int i=0; i<16; i++)
		sprintf(szMess, "%s%02x", szMess, byMd5[i]);

	//��MD5д���ļ�
	bRet = CDiscIOMgr::Write( dwHandle, szMess, strlen(szMess) );
	if( !bRet )
	{
		MessageBox( "��md5 д���ļ�����" );
		return ret;
	}

	CDiscIOMgr::Close( dwHandle );
	return ret;
}
//------------------------------------------------
//�ؼ�����:�汾�ȶ�
//------------------------------------------------

static TCHAR szTime[MAX_PATH];

BOOL	CGamePackDlg::GamePack(TCHAR *szNewPath, TCHAR *szOldPath)
{
	BOOL bResult = FALSE;
	TCHAR szTemp[MAX_PATH];
	TCHAR szOldTemp[MAX_PATH];
	WIN32_FIND_DATA FindData;
	HANDLE hHandle = INVALID_HANDLE_VALUE;
	BYTE byDigest_new[16], byDigest_old[16];
	DWORD dwTick = 0;
	INT nPos = 0;

	//���Ԥ����
	lstrcpy(szTemp, szNewPath);
	if( szTemp[0] == '\0' )
		lstrcat(szTemp, _T("*.*"));
	else 
	{
		if( szTemp[_tcslen(szTemp)-1] != '\\' )
			lstrcat(szTemp, _T("\\"));
		lstrcat(szTemp, _T("*.*"));
	}
	hHandle = ::FindFirstFile(szTemp, &FindData);
	if( hHandle == INVALID_HANDLE_VALUE )
		return FALSE;

	while( FALSE == m_bTerminateGamePack && TRUE == ::FindNextFile(hHandle, &FindData) )
	{
		if( FindData.cFileName[0] == _T('.') )
			continue;

		lstrcpy(szTemp, szNewPath);
		if( szTemp[0] != '\0' )
		{
			lstrcat(szTemp, _T("\\"));
		}
		lstrcat(szTemp, FindData.cFileName);
		lstrcpy(szOldTemp, szOldPath);
		lstrcat(szOldTemp, _T("\\"));
		lstrcat(szOldTemp, FindData.cFileName);

		if( FILE_ATTRIBUTE_DIRECTORY & FindData.dwFileAttributes )
		{//��Ŀ¼
			if( FALSE ==  CDiscIOMgr::IsDir(szOldTemp) )
			{
				m_pCpk->AddDir(szTemp, 0);
			}
			GamePack(szTemp, szOldTemp);
			goto __Continue;
		}
		m_nFileNum++;
		m_pStatic->SetWindowText(szTemp);

		if( FALSE == CDiscIOMgr::IsExist(szOldTemp) )
		{
			//�Ǹ��ļ��ϰ汾������,ֱ�Ӽ���������
			if( AddFileAndDir(szTemp) == FALSE )
				break;
		}
		else
		{
			//����ļ�С�ڣ�m�����Ǿ��ȱȽϣ�䣵
			DWORD dwNewFileSize = CDiscIOMgr::GetSize(szTemp);
			DWORD dwOldFileSize = CDiscIOMgr::GetSize(szOldTemp);
							
			if( dwNewFileSize != dwOldFileSize )
			{
				////�ļ���С����
				//if( dwNewFileSize <= 5242880 ) // 5M
				//{

				//	//�ļ���С����һ�����ļ���Ȼ�Ͳ�һ��
				//	if( AddFileAndDir(szTemp) == FALSE )
				//		break;
				//}
				//else
				{
					//�ļ�������,�������Ȳ鿴�ǲ���cpk�ļ�
					BOOL bRetOld = IsCPKFile(szOldTemp) && m_pCpk->IsValidCPK(szOldTemp);
					BOOL bRetNew = IsCPKFile(szTemp) && m_pCpk->IsValidCPK(szTemp);

					if( TRUE == bRetOld && TRUE == bRetNew )
					{
						//����cpk�ļ����Ҳ���
						//ֻ�ã����ļ��ȶ���
						CompareCPK(szTemp, szOldTemp);
					}
					else
					{
						//�ļ���С����һ�����ļ���Ȼ�Ͳ�һ��
						if( AddFileAndDir(szTemp) == FALSE )
							break;
					}
				}
			}
			else
			{
				//�ļ���Сһ��, ��Ҫ��һ������
				//if( dwNewFileSize <= 5242880 ) // 5M
				//{
				//	bResult = m_pMD5->MD5ForFile(szTemp, byDigest_new);
				//	ASSERT( bResult );
				//	bResult = m_pMD5->MD5ForFile(szOldTemp, byDigest_old);
				//	ASSERT( bResult );
				//	if( FALSE == IsEqualForMD5(byDigest_new, byDigest_old) )
				//	{ 
				//		// md5��һ�£��ļ���ͬ
				//		if( AddFileAndDir(szTemp) == FALSE )
				//			break;
				//	}
				//}
				//else
				{
					// > 5M,�������Ȳ鿴�ǲ���cpk�ļ�
					BOOL bRetOld = IsCPKFile(szOldTemp) && m_pCpk->IsValidCPK(szOldTemp);
					BOOL bRetNew = IsCPKFile(szTemp) && m_pCpk->IsValidCPK(szTemp);

					if( bRetOld && bRetNew)
					{
						//�ȶ�cpk�ļ�

						bResult = m_pMD5->MD5ForFile(szTemp, byDigest_new);
						ASSERT( bResult );
						bResult = m_pMD5->MD5ForFile(szOldTemp, byDigest_old);
						ASSERT( bResult );
						if( FALSE == IsEqualForMD5(byDigest_new, byDigest_old) )
						{ 
							// md5��һ�£��ļ����ݲ�һ����ͬ
							//ֻ�ã����ļ��ȶ���
							CompareCPK(szTemp, szOldTemp);
						}
					}
					else if( bRetOld || bRetNew )
					{
						//һ����cpk�ļ�,һ������,��Ȼ�������ļ���ͬ
						if( AddFileAndDir(szTemp) == FALSE )
							break;
					}
					else
					{
						//������cpk�ļ����Ǻð죬ֱ�Ӽ����䣵���ȶԾ���

						bResult = m_pMD5->MD5ForFile(szTemp, byDigest_new);
						ASSERT( bResult );
						bResult = m_pMD5->MD5ForFile(szOldTemp, byDigest_old);
						ASSERT( bResult );
						if( FALSE == IsEqualForMD5(byDigest_new, byDigest_old) )
						{ 
							// md5��һ�£��ļ���ͬ����Ҫ���
							if( AddFileAndDir(szTemp, 0) == FALSE )
								break;
						}
					} // if (�Ƿ���cpk�ļ�)
				}  //if (�Ƿ�<5M)

			}	//if (�¾��ļ��Ƿ����)
		}

__Continue:
		dwTick = GetTickCount() - m_dwTick;
		if( dwTick - s_dwTick > 300 )
		{
			//��ʾ����ʱ��
			sprintf(szTime, "%ld,%ld ms", dwTick / 1000, dwTick % 1000);
			m_pSTimer->SetWindowText(szTime);
			s_dwTick = dwTick;

			nPos = (INT)(m_fIntePos*m_nFileNum);
			if( nPos != s_nProcessPos )
			{	
				m_Progress.SetPos(nPos);
				s_nProcessPos = nPos;
			}
		}
	
		Sleep( 10 );
	}

	::FindClose(hHandle);
	
	return TRUE;
}
//------------------------------------------------
//�Ƿ�cpk�ļ�
//------------------------------------------------
BOOL	CGamePackDlg::IsCPKFile(TCHAR *szFileName)
{
	WORD wNameLen = _tcslen(szFileName);
	if( wNameLen <= 4 )
		return FALSE;
	if( (szFileName[wNameLen-1] == 'k' || szFileName[wNameLen-1] == 'K') &&
		(szFileName[wNameLen-2] == 'p' || szFileName[wNameLen-2] == 'P') &&
		(szFileName[wNameLen-3] == 'c' || szFileName[wNameLen-1] == 'C') &&
		(szFileName[wNameLen-4] == '.') )
		return TRUE;

	return FALSE;
}
//-----------------------------------------------------------------------------
//�Ƚ�����MD5�Ƿ����
//----------------------------------------------------------------------------
BOOL	CGamePackDlg::IsEqualForMD5(BYTE digest1[16], BYTE digest2[16])
{
	return ( memcmp(digest1, digest2, 16) == 0 );
}
//----------------------------------------------------------------------------
//�ȶ��¾������汾��cpk�ļ�
//----------------------------------------------------------------------------
BOOL	CGamePackDlg::CompareCPK(TCHAR *szNewCPK, TCHAR *szOldCPK)
{
	//׼������
	if( NULL == m_pNewCpk )
		m_pNewCpk = new CPK;
	if( NULL == m_pOldCpk )
		m_pOldCpk = new CPK;

	m_pNewCpk->Unload();
	m_pOldCpk->Unload();

	if( FALSE == m_pNewCpk->Load(szNewCPK) || FALSE == m_pOldCpk->Load(szOldCPK) )
		return FALSE;
	
	DWORD dwFatherCrc = m_pCpk->AddDir(szNewCPK, 0);
	if( GT_INVALID == dwFatherCrc )
		return FALSE;


	//�õ���Ӧ�ı����ַ
	tagCPKHeader *pNewCPKHeader = m_pNewCpk->GetCPKHeader();
	tagCPKTable	 *pNewCPKTable = m_pNewCpk->GetCPKTable();
	tagCPKHeader *pOldCPKHeader = m_pOldCpk->GetCPKHeader();
	tagCPKTable	 *pOldCPKTable = m_pOldCpk->GetCPKTable();
	
	DWORD dwFileMap = m_pNewCpk->CreateMap();
	ASSERT( dwFileMap != 0 );

	//��ʼ����ļ��ȶ�
	LPVOID p = NULL;
	LPVOID pMapAddress = NULL;
	BYTE digest_new[16], digest_old[16];
	TCHAR szTmp[MAX_PATH];
	TCHAR szTempDir[MAX_PATH];

	for(DWORD i=0; i<pNewCPKHeader->dwMaxTableNum; i++)
	{
		if( FALSE == m_pNewCpk->GetMD5FromTableIndex(digest_new, i) )
			continue;

		if( FALSE == m_pOldCpk->GetMD5FromCrc(digest_old, pNewCPKTable[i].dwCRC) || \
			FALSE == IsEqualForMD5(digest_new, digest_old) )
		{	
			//�ɰ汾��û�и��ļ�
			if( FALSE == m_pNewCpk->GetFullNameFromCRC(szTmp, pNewCPKTable[i].dwCRC) )
				continue;

			if( FALSE != IS_DIR(pNewCPKTable[i].dwFlag) )
				DWORD dwCurrentCrc = m_pCpk->AddDir(szTmp, dwFatherCrc);
			else
			{
				//�����ȴ�����·��,�õ�crc��֮���ڴ˻���������ļ�
				DWORD dwCurrentCrc = dwFatherCrc;
				INT nLen = _tcslen(szTmp);
				ASSERT( nLen >= 1 );
				for(INT k=nLen-1; k>=0; k--)
				{
					if( szTmp[k] == '\\' && k > 0 )
					{
						ZeroMemory(szTempDir, sizeof(szTempDir));
						memcpy(szTempDir, szTmp, k);
						dwCurrentCrc = m_pCpk->AddDir(szTempDir, dwCurrentCrc);
						break;
					}
				}

				
				p = m_pNewCpk->MapFile(dwFileMap, i, pMapAddress);
				if( NULL == p )
				{
					MessageBox(_T("�ļ�ӳ��ʧ��!!"));
					goto __End;
				}
				m_pCpk->AddFile(szTmp, dwCurrentCrc, pNewCPKTable+i, p); //...
				m_pNewCpk->UnMapFile(pMapAddress);
				p = pMapAddress = NULL;
			}
			continue;
		}
	}

__End:
	//ж��
	m_pNewCpk->CloseMap(dwFileMap);
	m_pNewCpk->Unload();
	m_pOldCpk->Unload();

	return TRUE;
}
//----------------------------------------------------------------------------
// -->> ��ָ����ֵд��ע�����ָ��·��,���ָ��·��û��,�򴴽���д��
//----------------------------------------------------------------------------
BOOL CGamePackDlg::WriteReg(TCHAR* KEY_ROOT/*ע����е�·��*/, TCHAR* KEY_NAME, LPCSTR sz)
{
	if ( KEY_ROOT == NULL ) return FALSE;
	if ( KEY_NAME == NULL ) return FALSE;

	HKEY hKey;
	DWORD dwDisposition = REG_CREATED_NEW_KEY;


	TCHAR buf[256]={0};

	long ret0 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, KEY_ROOT, 0, KEY_READ, &hKey);
	RegCloseKey(hKey);
	if ( ret0 != ERROR_SUCCESS )
	{
		ret0 = RegCreateKeyEx(HKEY_LOCAL_MACHINE, KEY_ROOT, 0, NULL,
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
		if ( ret0 != ERROR_SUCCESS )
			return FALSE;

		RegCloseKey(hKey);
	}
	// -- д��
	RegOpenKeyEx( HKEY_LOCAL_MACHINE, KEY_ROOT, 0, KEY_WRITE, &hKey );
	ret0 = RegSetValueEx(hKey, KEY_NAME, NULL, REG_SZ, (const BYTE*)sz, _tcslen(sz)+sizeof(sz[0]));
	if ( ret0 != ERROR_SUCCESS )
		return FALSE;

	RegCloseKey(hKey);
	return true;
}
//----------------------------------------------------------------------------
// -->> ��ȡע�����ָ��·���ļ�ֵ
//----------------------------------------------------------------------------
BOOL CGamePackDlg::ReadReg(TCHAR *KEY_ROOT, TCHAR* KEY_NAME, DWORD& key_value)
{
	HKEY hKey;
	long ret0 = RegOpenKeyEx( HKEY_LOCAL_MACHINE, KEY_ROOT, 0, KEY_QUERY_VALUE, &hKey );
	if( ret0 != ERROR_SUCCESS )
		return FALSE;

	DWORD dwBufLen=32;
	RegQueryValueEx( hKey, KEY_NAME, NULL, NULL, (LPBYTE)&key_value, &dwBufLen);
	RegCloseKey(hKey);

	return TRUE;
}
//----------------------------------------------------------------------------
BOOL CGamePackDlg::ReadReg(TCHAR* KEY_ROOT, TCHAR* KEY_NAME, TCHAR outstr[])
{
	ASSERT( outstr != NULL );
	HKEY hKey;
	long ret0 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, KEY_ROOT, 0, KEY_READ, &hKey);
	if ( ret0 != ERROR_SUCCESS )
	{
		outstr[0] = 0;
		return FALSE;
	}
	DWORD dwLen = MAX_PATH, dwType = REG_SZ;
	// if TCHAR* (LPBYTE)TCHAR*
	// if TCHAR [] (LPBYTE)&TCHAR
	RegQueryValueEx(hKey, KEY_NAME, NULL, &dwType, (LPBYTE)outstr, &dwLen);

	RegCloseKey(hKey);
	return TRUE;
}
//----------------------------------------------------------------------------

void CGamePackDlg::OnBnClickedBtSavedir2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sFolderPath;
	CFileDialog  OpenDlg( TRUE, "*.exe", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"��ִ�г���(*.exe)|*.exe");
	INT nRet = OpenDlg.DoModal();
	if( nRet == IDOK )
	{
		m_pExePath.SetWindowText( OpenDlg.GetPathName() );
	}
	
}


void CGamePackDlg::OnBnClickedCheckUpdate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_bAddexe = !m_bAddexe;
	CString  strPath, strTemp;
	m_pETSave->GetWindowText(strPath);
	
	INT nPos = strPath.ReverseFind( _T('.') ) ;
	strTemp = strPath.Left( nPos );

	if( m_bAddexe )
	{
		strTemp += ".exe";
	}
	else
	{
	    strTemp += ".cpk";
	}

	m_pETSave->SetWindowText( strTemp.GetBuffer() );
	
	m_EditVersionSrc.EnableWindow( m_bAddexe );
	m_EditVersionDest.EnableWindow( m_bAddexe );

}

void CGamePackDlg::OnStnClickedStFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CGamePackDlg::OnEnChangeEditVersionMin()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CString strVersionMin, strVersionMax, strSavePath, strTemp, strFileExt;
	//m_EditVersionMin.GetWindowText( strVersionMin );
	//m_EditVersionMax.GetWindowText( strVersionMax);
	//m_pETSave->GetWindowText( strSavePath );


	//INT nPos = strSavePath.ReverseFind( _T('\\') );

	//strTemp = strSavePath.Left( nPos + 1);
	//
	//m_strVersion.Empty();
	//m_strVersion += strVersionMin;
	//m_strVersion += _T( "-" );
	//m_strVersion += strVersionMax;
	//
	//strTemp += m_strFileName;
	//strTemp += m_strVersion;
	//strTemp += m_strExt;

	//m_pETSave->SetWindowText( strTemp.GetBuffer() );


}

void CGamePackDlg::OnEnChangeEditVersionMax()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//CString strVersionMin, strVersionMax, strSavePath, strTemp, strFileExt;
	//m_EditVersionMin.GetWindowText( strVersionMin );
	//m_EditVersionMax.GetWindowText( strVersionMax);
	//m_pETSave->GetWindowText( strSavePath );


	//INT nPos = strSavePath.ReverseFind( _T('\\') );

	//strTemp = strSavePath.Left( nPos + 1);

	//m_strVersion.Empty();
	//m_strVersion += strVersionMin;
	//m_strVersion += _T( "-" );
	//m_strVersion += strVersionMax;

	//strTemp += m_strFileName;
	//strTemp += m_strVersion;
	//strTemp += m_strExt;

	//m_pETSave->SetWindowText( strTemp.GetBuffer() );


}

void CGamePackDlg::OnEnChangeEtSavedir()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	/*CString strPath;
	m_pETSave->GetWindowText(strPath);

	INT nPos = strPath.ReverseFind( _T('\\') );
	if( nPos != -1)
	{
		m_strSavePath = strPath.Left( nPos + 1 );
	}*/

}
BOOL CGamePackDlg::AddFileAndDir( LPCTSTR lpFullName, DWORD dwFatherCrc )
{
	ASSERT( m_pCpk != NULL );

__retry:
	BOOL bResult = m_pCpk->AddFileAndDir( lpFullName, dwFatherCrc );
	if( bResult == FALSE ) 
	{
		TCHAR szBuf[4096];	// FIXME: make the array safe
		LPVOID lpMsgBuf = NULL;

		DWORD dwErr = ::GetLastError();

		sprintf( szBuf, "����ļ�%sʧ��", lpFullName );

		// ����Ƿ���Դ�ϵͳ�õ�������Ϣ
		if( dwErr != 0 )
		{
			// Read Error message from system
			FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM 
				| FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErr,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf, 0, NULL);

			wsprintf(szBuf, "%s\nCode: %lu\nNews: %s", szBuf,
				dwErr, (const char*)lpMsgBuf);

			LocalFree( lpMsgBuf );
		}

		INT nRet = MessageBox( szBuf, " Error ", MB_ABORTRETRYIGNORE ) ;
		if( nRet == IDRETRY )
			goto __retry;
		else if( nRet == IDABORT )
			return FALSE;

	}

	return TRUE;
}