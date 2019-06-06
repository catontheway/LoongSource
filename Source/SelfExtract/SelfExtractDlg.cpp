// SelfExtractDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SelfExtract.h"
#include "SelfExtractDlg.h"
#include "ExtractHanle.h"
#include "CPKCore/DiscIOMgr.h"
#include "MemMgr/ts_memmgr.h"
#include ".\selfextractdlg.h"
#include "ProcessWrapper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define PROGRESS_WIDTH		370
#define VERSION_LABLE			0x0b0c

struct tagVersion
{
	DWORD	dwLable;		//0x0b0c
	DWORD	dwVersion;		//�汾��
};
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


// CSelfExtractDlg �Ի���



CSelfExtractDlg::CSelfExtractDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelfExtractDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSelfExtractDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FILE, m_staticFile);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_STA_TITLE, Title);
	DDX_Control(pDX, IDC_UPDATE, Update);
	DDX_Control(pDX, IDCANCEL, Cancel);
}

BEGIN_MESSAGE_MAP(CSelfExtractDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_UPDATE, OnBnClickedUpdate)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CSelfExtractDlg ��Ϣ�������

BOOL CSelfExtractDlg::OnInitDialog()
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

	m_Progress.SetRange( 0, 100 );

	m_bTerminateThread = FALSE;
	// TODO: �ڴ���Ӷ���ĳ�ʼ����
	Check();

	GetDlgItem(IDC_STA_TITLE)->SetWindowText(Translate("<��>�ֶ�����"));
	GetDlgItem(IDC_STATIC_FILE)->SetWindowText(Translate("׼������"));
	GetDlgItem(IDC_UPDATE)->SetWindowText(Translate("����"));
	GetDlgItem(IDCANCEL)->SetWindowText(Translate("ȡ��"));
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CSelfExtractDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSelfExtractDlg::OnPaint() 
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

DWORD	CSelfExtractDlg::GetUnPackSize( CPKVfs* pCpK )
{
	ASSERT( pCpK != NULL );

	tagCPKHeader* pHeader = pCpK->GetCPKHeader();
	ASSERT( pHeader != NULL );
	
	tagCPKTable* pTable = pCpK->GetCPKTable();
	ASSERT( pTable != NULL );

	DWORD dwUnPackSize = 0;

	for( DWORD i =0 ; i < pHeader->dwValidTableNum; i++ )
	{
		dwUnPackSize += pTable[ i ].dwOriginSize;
	}
	
	return dwUnPackSize;

}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CSelfExtractDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD CSelfExtractDlg::ThreadUnPack( LPVOID* pParam )
{
	CSelfExtractDlg*	pDlg = (CSelfExtractDlg*)pParam;
	CString				strVersionPath;
	std::string			strLocalGameVersion;
	CTSMemMgr			TSMemMgr;
	CExtractHanle*		pReadCPK = new CExtractHanle(&TSMemMgr);
	CPKVfs*				pWriteCPK = new CPKVfs( &TSMemMgr );
	CONST INT			MAX_STRING_SIZE = 512;
	TCHAR				szTemp[MAX_STRING_SIZE];

	// ���벹��
	GetModuleFileName( NULL, szTemp, MAX_STRING_SIZE );
	if( FALSE == pReadCPK->LoadExtractFile( szTemp ) )
	{
		//���ز����ļ�ʧ��
		pDlg->MessageBox(Translate("û���ҵ������ļ�,����ʧ��!"));
		goto __UnPack_End;
	}

	// ��ͨ����ȡע���������Ϸ·��
	if( pDlg->ReadReg("SOFTWARE\\TENGREN\\LOONG", "InstallDir", szTemp) ) // ע�������˼�
	{
		pDlg->m_strAppPath = szTemp;
		strVersionPath = pDlg->m_strAppPath  + "\\launcher\\launcher.ini";
		if( CDiscIOMgr::IsExist(strVersionPath.GetBuffer()) )	// �ļ��Ƿ���ʵ����
		{
			::GetPrivateProfileString(_T("Launcher"), _T("LocalGame"), NULL,  szTemp, MAX_STRING_SIZE, strVersionPath.GetBuffer());
			strLocalGameVersion = szTemp;

			// �Ƚϸ��³������Ϸ����İ汾��
			if( pReadCPK->IsVersionCorrect( strLocalGameVersion ) )
				goto _version_ok;
		}
	}
		
	// ͨ��ע��������Ϸ�Լ��汾ʧ�ܣ� ���ֶ�ѡ����Ϸ����·��
	if( !pDlg->SelectPath() )
	{
		pDlg->GetDlgItem( IDC_UPDATE )->EnableWindow( TRUE );
		CloseHandle( pDlg->m_Mutex );
		goto __UnPack_End;
	}

	// ��ȡ��ǰ��Ϸ�汾
	strVersionPath = pDlg->m_strAppPath  + "\\launcher\\launcher.ini";
	if( !CDiscIOMgr::IsExist( strVersionPath.GetBuffer() ) )
	{
		pDlg->MessageBox(Translate("��ó���汾��Ϣ������ȷ��ѡ���Ŀ¼Ϊ��ϷĿ¼"), "Error");
		pDlg->GetDlgItem( IDC_UPDATE )->EnableWindow( TRUE );
		goto __UnPack_End;
	}
	::GetPrivateProfileString(_T("Launcher"), _T("LocalGame"), NULL,  szTemp, MAX_STRING_SIZE, strVersionPath.GetBuffer());
	strLocalGameVersion = szTemp;

	//�Ƚϸ��³������Ϸ����İ汾��
	if( !pReadCPK->IsVersionCorrect( strLocalGameVersion ) )
	{
		pDlg->MessageBox(Translate("�˸��³������Ϸ�İ汾��ƥ��"), "Error");
		pDlg->GetDlgItem( IDC_UPDATE )->EnableWindow( TRUE );
		goto __UnPack_End;
	}


_version_ok:

	INT nPos = strVersionPath.Find( _T(":\\") );
	strVersionPath.Delete( nPos + 2, strVersionPath.GetLength() - nPos - 2);
	//�Ƿ����㹻�Ŀռ�
	if( FALSE == CDiscIOMgr::ClueFreeSpace(strVersionPath.GetBuffer(), pDlg->GetUnPackSize( pReadCPK ) ) )
	{
		pDlg->MessageBox("���̿ռ䲻��", "Error");
		goto __UnPack_End;		
	}

	//��ȡ������Ϣ
	{
		ReloadTranslate(pDlg->m_strAppPath.GetBuffer());
		pDlg->GetDlgItem(IDC_STA_TITLE)->SetWindowText(Translate("<��>�ֶ�����"));
		pDlg->GetDlgItem(IDC_STATIC_FILE)->SetWindowText(Translate("׼������"));
		pDlg->GetDlgItem(IDC_UPDATE)->SetWindowText(Translate("����"));
		pDlg->GetDlgItem(IDCANCEL)->SetWindowText(Translate("ȡ��"));
	}


	//��ѹ�ļ�
	tagCPKHeader *pCPKHeader = pReadCPK->GetCPKHeader();
	tagCPKTable  *pTable = pReadCPK->GetCPKTable();

	pDlg->m_dwTotalFileNum = pCPKHeader->dwFileNum;
	DWORD dwUnPackNum = 0;
	BOOL bRet = pDlg->UnPackCpk(0, dwUnPackNum, pReadCPK, pWriteCPK);
	 

	if( TRUE == bRet )
	{
		//�������
		pDlg->SetProgressPos(100);

		//ɾ�������ļ�
		//::DeleteFile(m_szDiscoFileName);
		//���°汾���ļ�
		pDlg->RefreshVersion(pReadCPK->GetVersionDest());

		//
		pDlg->m_staticFile.SetWindowText( Translate("�������") );

		pDlg->GetDlgItem( IDCANCEL )->SetWindowText( Translate("�˳�") );
		//������ʾ��Ϣ
		//pDlg->MessageBox("�����ɹ� !");
		//ShowWindow(GetDlgItem(m_hWnd, ID_BUTN_UPDATE), SW_NORMAL);
		//ShowWindow(GetDlgItem(m_hWnd, ID_BUTN_CANCEL), SW_HIDE);

	}
	else
	{
		pDlg->MessageBox(Translate("��ѹ�������ж�, ����ʧ�� !"), "Error");	
	}

__UnPack_End:

	// �ر��ļ�
	pReadCPK->Unload();
	FREE(pReadCPK);
	pWriteCPK->Unload();
	FREE(pWriteCPK);
	CloseHandle( pDlg->m_Mutex );
	return TRUE;	
}

BOOL CSelfExtractDlg::UnPackCpk(DWORD dwFatherCRC, DWORD& dwUnPackNum, CPKVfs *pReadCpk, CPKVfs *pWriteCpk)
{
	Sleep( 1 );		//��Ϣ����

	CString strPath;
	CString strWndText;

	if( dwFatherCRC != 0 )
	{
		//�Ȳ��Ҹ���
		INT nTableIndex = pReadCpk->GetTableIndexFromeCrc(dwFatherCRC);
		if( nTableIndex == GT_INVALID )
		{
			ASSERT( FALSE && Translate("cpk�ļ���δ�ҵ�����") );
			return TRUE;
		}

		//���ļ�ӳ����
		//DWORD dwFileCPKHandle = pReadCpk->GetFileInfo(nTableIndex);	//��
		//if( 0 == dwFileCPKHandle )
		//{
		//	ASSERT( FALSE && "cpk���ļ�ʧ��" );
		//	return TRUE;
		//}

		char lpFileName[MAX_PATH];
		BOOL	bRet = pReadCpk->GetFileOrDirName( nTableIndex, lpFileName );
		if( bRet == FALSE )
		{
			ASSERT( FALSE && Translate("cpk���ļ�ʧ��") );
			return TRUE;
		}

//		CPKFile *pCPKFileHandle = (CPKFile*)dwFileCPKHandle;
		
		CString strPath;
		CString strWndText = m_strAppPath + "\\" + lpFileName;//pCPKFileHandle->szFullName ;
		strPath.Format( "%s %s ...",Translate("��ѹ"),strWndText.GetBuffer() );
		m_staticFile.SetWindowText( strWndText.GetBuffer() );

		//��Ȼ��·����������ǣ����ʽ����˵���ڴ�������cpk�ļ�
		TCHAR szFullName[MAX_PATH];
		LPTSTR lpExtName = NULL;

		lstrcpy(szFullName, lpFileName );//pCPKFileHandle->szFullName);
		lpExtName = _tcsrchr(szFullName, _T('.'));

		//�ж��ǲ���cpk�ļ�
		if( lpExtName && _tcsicmp(lpExtName, _T(".cpk")) == 0 )
		{
			dwUnPackNum++;

			pWriteCpk->Unload();

			if( FALSE == CDiscIOMgr::IsExist(strWndText.GetBuffer()) )
			{
				//�ȴ�������Ӧ��·��
				char szTemp[MAX_PATH];
				for(int i=0; strWndText.GetAt(i) != '\0'; i++)
				{
					if( strWndText.GetAt(i) == '\\' && i > 0 )
					{
						ZeroMemory(szTemp, sizeof(szTemp));
						strncpy(szTemp, strWndText.GetBuffer(), i);

						::CreateDirectory(szTemp, NULL);
					}
				}

				//����cpk�ļ�
				if( FALSE == pWriteCpk->Create( lpFileName ) )//pCPKFileHandle->szFullName) )
				{
					//�����ļ�ʧ��
					ASSERT( FALSE && Translate("����CPK�ļ�ʧ��"));

					//pReadCpk->Close(dwFileCPKHandle);
					return TRUE;
				}
			}
			else
			{
				//������
				if( FALSE == pWriteCpk->Load(strWndText.GetBuffer()) )
				{
					//INT nRet = MessageBox(NULL, "���ļ�%sʧ��", fp->szFullName, NULL, MB_ABORTRETRYIGNORE);

					//pReadCpk->Close(dwFileCPKHandle);
					return TRUE;
				}
			}

			//�رոþ��
			//pReadCpk->Close(dwFileCPKHandle);

			//����һ��cpk�ļ�,���ǰ������е���Ŀ¼�ŵ���CPK�ļ�����ȥ
			return UnPackCPKFromCPK(dwFatherCRC, dwUnPackNum, pReadCpk, pWriteCpk);
		}
		else
		{
			//����cpk�ļ�,��ôֱ���ڴ����ϴ���Ŀ¼
			CreateDirToDisc(strWndText.GetBuffer());
			dwUnPackNum++;
		}

		//�μ�����֮��رվ��
		//pReadCpk->Close(dwFileCPKHandle);
	}

	//��ͨ��Ŀ¼,����ֻ��Ҫ�õ���һ������ļ����ļ���
	DWORD* pdwChildArray = NULL;
	DWORD dwChildDirNum = 0;

	//�õ�һ����Ŀ¼
	pReadCpk->GetFirstLevelChildDir(dwFatherCRC, dwChildDirNum);

	DWORD dwChildNum = 0;
	//�õ�һ�����ļ�
	pReadCpk->GetFirstLevelChild(dwFatherCRC, dwChildNum);

	//�����ڴ�ȡ�ϴ���
	if( dwChildDirNum > dwChildNum )
	{
		pdwChildArray = new DWORD[dwChildDirNum];
	}
	else
	{
		pdwChildArray = new DWORD[dwChildNum];
	}

	if( dwChildDirNum > 0 )
	{
		if( TRUE == m_bTerminateThread )
		{
			FREEARR(pdwChildArray);
			return FALSE;
		}

		pReadCpk->GetFirstLevelChildDir(dwFatherCRC, dwChildDirNum, pdwChildArray);
		for( DWORD i=0; i<dwChildDirNum; i++ )
		{
			//��������Ŀ¼, Ƕ�׵��ý�ѹ
			if( FALSE == UnPackCpk(pdwChildArray[i], dwUnPackNum, pReadCpk, pWriteCpk) )
			{
				FREEARR(pdwChildArray);
				return FALSE;
			}
		}
	}

	if( dwChildNum > 0 )
	{
		pReadCpk->GetFirstLevelChild(dwFatherCRC, dwChildNum, pdwChildArray);
		for(DWORD i=0; i<dwChildNum; i++)
		{
			if( TRUE == m_bTerminateThread )
			
			{
				FREEARR(pdwChildArray);
				return FALSE;
			}
			//������һ�����ļ�, ֱ�ӵ����ļ���ѹ����
			if( FALSE == UnPackSimpleFile(pdwChildArray[i], dwUnPackNum, pReadCpk) )
			{	FREEARR(pdwChildArray);
				return FALSE;
			}
		}
	}

	FREEARR(pdwChildArray);
	return TRUE;
}


BOOL	CSelfExtractDlg::CreateDirToDisc(LPCTSTR szPath)
{

	ASSERT(szPath);
	//����һ��һ������·��,����c:\bin\tidy, c:\, c:\bin\tidy

	char szTemp[MAX_PATH];
	for(int i=0; szPath[i] != '\0'; i++)
	{
		if( szPath[i] == '\\' && i > 0 )
		{
			ZeroMemory(szTemp, sizeof(szTemp));
			strncpy(szTemp, szPath, i);

			::CreateDirectory(szTemp, NULL);
		}
	}

	return ::CreateDirectory(szPath, NULL);

}

VOID	CSelfExtractDlg::SetProgressPos(INT nPos)
{
	//DWORD dwProgress = nPos * PROGRESS_WIDTH / 100;
	//if( m_dwProgress == dwProgress )
	//	return;

	//InterlockedExchange( (LONG*)&m_dwProgress , dwProgress );

	m_Progress.SetPos( nPos );

}

//-------------------------------------------------------------------------------
//��һ���������ѹ���������cpk����Ŀ¼����
//-------------------------------------------------------------------------------
BOOL	CSelfExtractDlg::UnPackCPKFromCPK(DWORD dwDirCRC, DWORD& dwUnPackNum, CPKVfs *pReadCPK, CPKVfs *pWriteCPK)
{
	DWORD* pdwChildArray = NULL;

	//��ȡһ����Ŀ¼
	DWORD dwChildDirNum = 0;
	pReadCPK->GetFirstLevelChildDir(dwDirCRC, dwChildDirNum);

	//��ȡһ�����ļ�
	DWORD dwChildNum = 0;
	pReadCPK->GetFirstLevelChild(dwDirCRC, dwChildNum);

	//�����ڴ�ȡ�ϴ���
	if( dwChildDirNum > dwChildNum )
	{
		pdwChildArray = new DWORD[dwChildDirNum];
	}
	else
	{
		pdwChildArray = new DWORD[dwChildNum];
	}

	if( dwChildNum > 0 )
	{
		//������һ�����ļ�, ����ֱ�Ӱ��������cpk�ļ�����ȥ

		pReadCPK->GetFirstLevelChild(dwDirCRC, dwChildNum, pdwChildArray);
		for(DWORD i=0; i<dwChildNum; i++)
		{
			if( TRUE == m_bTerminateThread )
			{
				FREEARR(pdwChildArray);
				return FALSE;
			}

			Sleep( 0 );		//��Ϣ����
			dwUnPackNum++;

			INT nTableIndex = pReadCPK->GetTableIndexFromeCrc(pdwChildArray[i]);
			if( nTableIndex == GT_INVALID )
			{
				ASSERT( FALSE && Translate("cpk�ļ���δ�ҵ�����") );
				continue;
			}

			//���ļ�ӳ����
			DWORD dwCPKFileHandle = pReadCPK->Open(nTableIndex, 0, true );	//��
			if( 0 == dwCPKFileHandle )
			{
				ASSERT( FALSE && Translate("���ļ�ʧ��") );
				continue;
			}

			CPKFile *pCPKFileHandle = (CPKFile*)dwCPKFileHandle;

			CString strWndText;
			strWndText.Format( "%s %s ...",Translate("��ѹ"), pCPKFileHandle->szFullName );
			m_staticFile.SetWindowText( strWndText.GetBuffer() );

			//ȷʵ���ļ�,���ǿ����ȴ�����·��,Ȼ���ڼ����ļ�
			//����һ��һ������·��,����c:\bin\tidy, c:\, c:\bin, c:\bin\tidy

			char szFileName[MAX_PATH], szCPKName[MAX_PATH];
			ZeroMemory(szFileName, sizeof(szFileName));
			ZeroMemory(szCPKName, sizeof(szCPKName));

			BOOL bRet = pReadCPK->IsFileForCPK(pCPKFileHandle->szFullName, szCPKName, szFileName);
			ASSERT( bRet == TRUE && Translate("������cpk�������ļ�, ���ȴ����"));

			char szPathFile[MAX_PATH];
			DWORD dwCPKFatherCRC = 0;

			INT j=0;
			for(int k=0, j=0; szFileName[k] != '\0'; k++)
			{
				if( szFileName[k] == '\\' && k > 0 )
				{
					ZeroMemory(szPathFile, sizeof(szPathFile));
					strncpy(szPathFile, szFileName+j, k-j);
					dwCPKFatherCRC = pWriteCPK->CreateDir(szPathFile, dwCPKFatherCRC);

					j = k+1;
				}
			}

			//���ȼ�����Ҫӳ��Ĵ�С
			tagCPKTable *pTable = pReadCPK->GetCPKTable();
			DWORD dwMapSize = pTable[nTableIndex].dwPackedSize + pTable[nTableIndex].dwExtraInfoSize;


			INT nWriteTableIndex = pWriteCPK->InitFileCPKToCPK(szFileName, dwCPKFatherCRC, pCPKFileHandle->pTableItem);
			ASSERT( nWriteTableIndex != GT_INVALID );


			DWORD dwHaveMapSize = 0 ;
			for( ; ; )
			{
				//д������
				if( FALSE == pWriteCPK->WriteFileCPKToCPK(nWriteTableIndex, dwHaveMapSize, 
					pCPKFileHandle->lpStartAddress, pCPKFileHandle->dwMapSize) )
				{
					ASSERT(FALSE && Translate("д���ļ�ʧ��"));
					return FALSE;
				}

				dwHaveMapSize += pCPKFileHandle->dwMapSize;
				//�رվ��
				pReadCPK->Close(dwCPKFileHandle);

				if( dwHaveMapSize >= dwMapSize )
					break;

				Sleep( 1 );

				dwCPKFileHandle = pReadCPK->Open(nTableIndex, dwHaveMapSize);	//��
				ASSERT( dwCPKFileHandle != GT_INVALID );
				pCPKFileHandle = (CPKFile*)dwCPKFileHandle;
			}
			//��������ļ�
			pWriteCPK->FinishFileCPKToCPK(nWriteTableIndex);

			SetProgressPos( (INT)((100L * dwUnPackNum) / m_dwTotalFileNum ) );
		}
	}



	if( dwChildDirNum > 0 )
	{
		//�������ļ���, ����Ƕ�׵��ô˺���, ������ѹչ��

		pReadCPK->GetFirstLevelChildDir(dwDirCRC, dwChildDirNum, pdwChildArray);

		for(DWORD i=0; i<dwChildDirNum; i++)
		{
			if( TRUE == m_bTerminateThread )
			{
				FREEARR(pdwChildArray);
				return FALSE;
			}

			dwUnPackNum++;

			INT nTableIndex = pReadCPK->GetTableIndexFromeCrc(pdwChildArray[i]);
			if( nTableIndex == GT_INVALID )
			{
				ASSERT( FALSE && Translate("cpk�ļ���δ�ҵ�����") );
				continue;
			}

			//���ļ�ӳ����
			//DWORD dwCPKFileHandle = pReadCPK->Open(nTableIndex);	//��
			//if( 0 == dwCPKFileHandle )
			//{
			//	ASSERT( FALSE && "���ļ�ʧ��" );
			//	continue;
			//}

			//CPKFile *pCPKFileHandle = (CPKFile*)dwCPKFileHandle;

			char lpFileName[MAX_PATH];
			BOOL	bRet = pReadCPK->GetFileOrDirName( nTableIndex, lpFileName );
			if( bRet == FALSE )
			{
				ASSERT( FALSE && Translate("cpk���ļ�ʧ��") );
				continue;
			}

			CString strWndText;
			strWndText.Format( "%s %s ...",Translate("��ѹ"),lpFileName );//pCPKFileHandle->szFullName );
			m_staticFile.SetWindowText( strWndText.GetBuffer() );


			//�ȴ���
			char szFileName[MAX_PATH], szCPKName[MAX_PATH];
			ZeroMemory(szFileName, sizeof(szFileName));
			ZeroMemory(szCPKName, sizeof(szCPKName));

			bRet = pReadCPK->IsFileForCPK(lpFileName, szCPKName, szFileName);
			ASSERT( bRet == TRUE && Translate("������cpk�������ļ���, ���֮���ֲ���"));

			if( bRet == TRUE )
			{
				//ֻ��·��
				pWriteCPK->AddDirCyc(szFileName, 0); 
			}

			//�ر��ļ�ӳ����
			//pReadCPK->Close(dwCPKFileHandle);

			if( FALSE == UnPackCPKFromCPK(pdwChildArray[i], dwUnPackNum, pReadCPK, pWriteCPK) )
			{
				FREEARR(pdwChildArray);
				return FALSE;
			}
		}
	}

	FREEARR(pdwChildArray);
	return TRUE;
}

//-------------------------------------------------------------------------------
//��cpk�ļ���ѹ���������ļ�(������);
//-------------------------------------------------------------------------------
BOOL CSelfExtractDlg::UnPackSimpleFile(DWORD dwCRC, DWORD& dwUnPackNum, CPKVfs *pReadCPK)
{
	dwUnPackNum++;

	//���Ȳ���tableindex
	INT nTableIndex = pReadCPK->GetTableIndexFromeCrc(dwCRC);
	if( nTableIndex == GT_INVALID )
	{
		ASSERT( FALSE && Translate("CPK�ļ���δ�ҵ�����"));
		return TRUE;
	}

	//���ļ�ӳ����
	DWORD dwCPKFileHandle = pReadCPK->Open(nTableIndex);	//��
	if( 0 == dwCPKFileHandle )
	{
		ASSERT( FALSE && Translate("CPK���ļ�ʧ��"));
		return TRUE;
	}
	CPKFile *pCPKFileHandle = (CPKFile*)dwCPKFileHandle;

	char szMess[1024];
	sprintf( szMess, "CPKFile: name: %s; addr: %ld; size: %ld\n", pCPKFileHandle->szFullName, (DWORD)pCPKFileHandle->lpMem,
		pCPKFileHandle->dwFileSize );
	OutputDebugString( szMess );

	CString strWndText;
	strWndText.Format( "%s %s ...",Translate("��ѹ"), pCPKFileHandle->szFullName );
	m_staticFile.SetWindowText( strWndText.GetBuffer() );

__loop_retry:		//���³��Եı�ǩ

	CString strFilePath = m_strAppPath + "\\" + pCPKFileHandle->szFullName;

	DWORD dwFileHandle = CreateFileToDisc(strFilePath.GetBuffer());
	if( INVALID_FILE_HANDLE == dwFileHandle )
	{
		INT nRet = MessageBox("fail to create file", \
			NULL, MB_ABORTRETRYIGNORE);

		if( nRet == IDABORT )
		{
			//��ֹ
			pReadCPK->Close(dwCPKFileHandle);
			return FALSE;
		}
		else if( nRet == IDRETRY )
		{
			//���³���
			goto __loop_retry;
		}
		else if( nRet == IDIGNORE )
		{
			//���ԣ��Ա����
			pReadCPK->Close(dwCPKFileHandle);
			return TRUE;
		}

		pReadCPK->Close(dwCPKFileHandle);
		return TRUE;
	}
	//�Դ��ļ����в���ӳ�䣬ѭ��д
	if( pCPKFileHandle->bCompressed == false && pCPKFileHandle->dwMapSize < pCPKFileHandle->dwFileSize  )
	{     
		DWORD dwTemp = 0;
		DWORD dwIncrease = 0;

		while(dwTemp != pCPKFileHandle->dwFileSize)
		{
			if(pCPKFileHandle->dwFileSize -  dwTemp >= GT_CPK_RAW_BUFFER_SIZE)
			{//����ÿ��Ҫд�Ĵ�С
				dwIncrease = GT_CPK_RAW_BUFFER_SIZE;
			}
			else
			{//���ʣ�µĲ���GT_CPK_RAW_BUFFER_SIZE����ֻдʣ�µ�
				dwIncrease = pCPKFileHandle->dwFileSize - dwTemp;
			}
			if( FALSE == CDiscIOMgr::Write(dwFileHandle, pCPKFileHandle->lpMem, dwIncrease, 0) )
			{
				//�ر���ؾ��
				CDiscIOMgr::Close(dwFileHandle);
				pReadCPK->Close(dwCPKFileHandle);
				return FALSE;
			}
			dwTemp += dwIncrease;
			pReadCPK->Close(dwCPKFileHandle);
			dwCPKFileHandle = pReadCPK->Open(nTableIndex, dwTemp);           //ӳ���ļ�����һ����
			pCPKFileHandle = (CPKFile*)dwCPKFileHandle;
		}
	}
	else
	{
		if( FALSE == CDiscIOMgr::Write(dwFileHandle, pCPKFileHandle->lpMem, pCPKFileHandle->dwFileSize, 0) )
		{
			//�ر���ؾ��
			CDiscIOMgr::Close(dwFileHandle);
			pReadCPK->Close(dwCPKFileHandle);
			return FALSE;
		}

	}

	SetProgressPos( (INT)((100L * dwUnPackNum) / m_dwTotalFileNum ) ); 
	//�ر���ؾ��
	CDiscIOMgr::Close(dwFileHandle);
	pReadCPK->Close(dwCPKFileHandle);

	return TRUE;
}

//------------------------------------------------------------------------------
//�������ϴ����ļ�
//------------------------------------------------------------------------------
DWORD	CSelfExtractDlg::CreateFileToDisc(LPCTSTR szFileName)
{
	ASSERT( szFileName );

	if( TRUE == CDiscIOMgr::IsExist(szFileName) )
	{
		DWORD fileAttributes = GetFileAttributes(szFileName);
		fileAttributes &= ~FILE_ATTRIBUTE_READONLY;
		SetFileAttributes(szFileName, fileAttributes);
		if( FALSE == ::DeleteFile(szFileName) )
		{
			char mess[500];
			sprintf(mess, "fail to delete file:%s", szFileName);
			MessageBox( mess, "error", MB_OK);

			return INVALID_FILE_HANDLE;
		}

	}

	INT nLen = (INT)strlen(szFileName);
	if( nLen == 0 ) return FALSE;

	char szTemp[MAX_PATH];
	ZeroMemory(szTemp, sizeof(szTemp));
	for(INT i=nLen-1; i>=0; i--)
	{
		if( szFileName[i] == '\\' && i>0 )
		{
			memcpy(szTemp, szFileName, i);
			CreateDirToDisc(szTemp);
			/*
			char mess[500];
			sprintf(mess, "fail to create dir:%s", szTemp);
			MessageBox(m_hWnd, mess, "error", MB_OK);

			return INVALID_FILE_HANDLE;
			/**/
			break;
		}
	}

	return CDiscIOMgr::Create(szFileName,
		GENERIC_READ | GENERIC_WRITE,
		0, // 
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 
		NULL);
}

//-----------------------------------------------------------------------------
//���°汾��
//-----------------------------------------------------------------------------
BOOL CSelfExtractDlg::RefreshVersion(const std::string& strVersion)
{
	// �汾���ļ�
	std::string strVersionPath = m_strAppPath  + "\\launcher\\launcher.ini";
	BOOL bResult = ::WritePrivateProfileString(_T("Launcher"), _T("LocalGame"), strVersion.c_str(), strVersionPath.c_str());

	return bResult;	
}


void CSelfExtractDlg::OnBnClickedUpdate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Check();

	//CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadUnPack, this, NULL, NULL );
}


void CSelfExtractDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::InterlockedExchange((LONG*)&m_bTerminateThread, TRUE);
	Sleep( 20 );
	OnCancel();
}

BOOL CSelfExtractDlg::SelectPath()
{

	BROWSEINFO bi;
	TCHAR szPath[MAX_PATH];
	ZeroMemory(szPath, sizeof(szPath));

	//��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;			//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = Translate("�Ҳ�����Ϸ·�������ֶ�ѡ����ϷĿ¼");  //������
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.iImage	=	IDR_MAINFRAME;
	//��ʼ����ڲ���bi����

	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if(pIDList)
	{
		//ȡ���ļ���·����Buffer��
		SHGetPathFromIDList(pIDList, szPath);
		m_strAppPath = szPath;
	}
	else
	{
		LPMALLOC lpMalloc;
		if(FAILED(SHGetMalloc(&lpMalloc))) 
			return FALSE;
		//�ͷ��ڴ�
		lpMalloc->Free(pIDList);
		lpMalloc->Release();

		return FALSE;
	}

	LPMALLOC lpMalloc;
	if(FAILED(SHGetMalloc(&lpMalloc))) 
		return TRUE;
	//�ͷ��ڴ�
	lpMalloc->Free(pIDList);
	lpMalloc->Release();

	return TRUE;
}

BOOL	CSelfExtractDlg::ReadReg(TCHAR* KEY_ROOT, TCHAR* KEY_NAME, TCHAR outstr[])
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

VOID	CSelfExtractDlg:: Check()
{

	//����Զ����³����Ƿ�������
	m_Mutex = ::CreateMutex( NULL, FALSE, "Loong_launcher" );
	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		CloseHandle( m_Mutex );
		MessageBox( Translate("�Զ����³����������У����ȹر��Զ����³���") );
		GetDlgItem( IDC_UPDATE )->EnableWindow( TRUE );
	}
	else
	{
		//�����Ϸ�Ƿ�������
		CProcessWrapper  Process;
		DWORD dwThreadID;
		HANDLE hKungfu = Process.FindProcess( dwThreadID,  "Client.exe" );
		if( hKungfu != INVALID_HANDLE_VALUE )
		{
			MessageBox( Translate("��Ϸ�������У����ȹر���Ϸ�ڽ��и���") );
			GetDlgItem( IDC_UPDATE )->EnableWindow( TRUE );
			CloseHandle( m_Mutex );
		}
		else
		{
			CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadUnPack, this, NULL, NULL );
			CWnd* pWnd = GetDlgItem( IDC_UPDATE );
			pWnd->EnableWindow( FALSE );
		}
	}
}

void CSelfExtractDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	SendMessage( WM_NCLBUTTONDOWN, HTCAPTION, (LPARAM)&point );

	CDialog::OnMouseMove(nFlags, point);
}
