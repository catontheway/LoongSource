// ServerIpDesDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ServerIpDes.h"
#include "ServerIpDesDlg.h"
#include ".\ServerIpDesdlg.h"

#include "../vEngine/vEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BYTE byFilePath[56]	= { 0xfd,0x2b,0xc4,0x86,0xaf,0x8c,0x24,0xd6,0x2c,0xb6,0xc5,0x2d,0xb1,0x6c,0x9e,0x79,0x40,0x04,0xc5,0x88,0xce,0x0c,0x82,0x88,0x98,0x06,0xb5,0xa4,0xb6,0xf4,0xb2,0x21,0xdc,0xc2,0x6a,0x53,0x8a,0xb1,0x93,0x53,0x41,0x5a,0x60,0x36,0x54,0x3b,0x6b,0x88,0x69,0x16,0x76,0x1f,0xfb,0xb1,0x2a,0x9d };

//-----------------------------------------------------------------------------
// �Ӽ��ܵ��ֽڵõ����ܵ��ַ���
//-----------------------------------------------------------------------------
LPTSTR CServerIpDesDlg::GetStr( BYTE* pByte, INT nLen )
{
	LPTSTR pRet = new TCHAR[nLen / sizeof(TCHAR)];
	ZeroMemory(pRet, nLen);

	m_pUtil->Decrypt(pByte, pRet, nLen, 0);

	return pRet;
}

//-----------------------------------------------------------------------------
// �ͷŻ�õĽ����ַ���
//-----------------------------------------------------------------------------
VOID CServerIpDesDlg::RetStr(LPTSTR pRet)
{
	SAFE_DEL_ARRAY(pRet);
}

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


// CServerIpDesDlg �Ի���



CServerIpDesDlg::CServerIpDesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerIpDesDlg::IDD, pParent)
	, m_strFileName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	vEngine::InitEngine();
	m_pUtil = "Util";
}

CServerIpDesDlg::~CServerIpDesDlg()
{
	vEngine::DestroyEngine();
}

void CServerIpDesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILE_NAME, m_strFileName);
	DDV_MaxChars(pDX, m_strFileName, MAX_PATH - 1);
}

BEGIN_MESSAGE_MAP(CServerIpDesDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnBnClickedButtonBrowse)
END_MESSAGE_MAP()


// CServerIpDesDlg ��Ϣ�������

BOOL CServerIpDesDlg::OnInitDialog()
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
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CServerIpDesDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerIpDesDlg::OnPaint() 
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
HCURSOR CServerIpDesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerIpDesDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//��ȡ�������ļ���
	UpdateData();
	if("" == m_strFileName)
	{
		//û�����뱻�����ļ���ֱ�ӷ���
		return;
	}

	//����
	if(EncryptAndSave())
	{
		MessageBox(_T("�ļ����ܳɹ���"), _T("���ܹ���"), MB_OK);
	}
	else
	{
		MessageBox(_T("�ļ�����ʧ�ܣ�"), _T("���ܹ���"), MB_OK);
	}
#ifdef _DEBUG
	DecryptAndSave();	//������
#endif

	return;

	//������ɺ��˳�
//	OnOK();
}

//���ݱ������ļ������ɼ����ļ���(����׺��Ϊ.dat)
CString CServerIpDesDlg::GetSaveFileName() const
{
	ASSERT(m_strFileName != "");

	CString strFileName(m_strFileName);
	strFileName.Replace(_T(".ini"), _T(".ins"));

	return strFileName;
}

//������ɼ��ܺ�
INT CServerIpDesDlg::GenDesCode()
{
	return m_pUtil->Rand() % 20;
}

//��ȡ�������ļ��������ܺ󱣴�
BOOL CServerIpDesDlg::EncryptAndSave()
{
	//��ü��ܺ��ļ���(���ݱ������ļ�������)
	CString strSaveFileName = GetSaveFileName();
	//������ɼ��ܺ�
	INT nDesCode = GenDesCode();

	TObjRef<DiscIOMgr>	pIO = "DiscIOMgr";
	DWORD dwHFileIn, dwHFileEnc;		//��������ļ����
	DWORD dwWritten = 0, dwInFileSize;	//һ��д���ļ��Ĵ�С(�ַ�)

	//�򿪱������ļ�
	dwHFileIn = pIO->Create(m_strFileName,
			GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 
			NULL);
	if(dwHFileIn == INVALID_FILE_HANDLE)
	{
		return FALSE;
	}

	//�򿪼��ܺ������ļ�
		dwHFileEnc = pIO->Create(strSaveFileName,
			GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CREATE_ALWAYS, 
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 
			NULL);
	if(dwHFileEnc == INVALID_FILE_HANDLE)
	{
		pIO->Close(dwHFileIn);
		return FALSE;
	}

	//����Կ��Ӧ�ĺ��뱣�浽�ļ���(��Сռsizeof(nDesCode)�ַ�)//??
	pIO->Write(dwHFileEnc, &nDesCode, sizeof(nDesCode), &dwWritten, NULL);

	//�õ��������ļ���С
	dwInFileSize = pIO->GetSize(dwHFileIn);
	//������СΪ8��������(�����ֶ�Ҫ��)
	dwInFileSize = (((dwInFileSize - 1) >> 3) << 3) + 8;
	
	//�����ȡ�ڴ�
	char *pReadBuf = new char[dwInFileSize];
	if(NULL == pReadBuf)
	{
		pIO->Close(dwHFileIn);
		pIO->Close(dwHFileEnc);
		return FALSE;
	}
	memset(pReadBuf, ' ', dwInFileSize);
	
	//��ȡ�ļ������ܺ󱣴�
	pIO->Read(dwHFileIn, pReadBuf, dwInFileSize);
	m_pUtil->Encrypt(pReadBuf, pReadBuf, dwInFileSize, nDesCode, IsLegal() ? 0 : 2);
	pIO->Write(dwHFileEnc, pReadBuf, dwInFileSize, &dwWritten, NULL);

	//�����ڴ棬���ر��ļ�
	delete []pReadBuf;
	pIO->Close(dwHFileIn);
	pIO->Close(dwHFileEnc);
	
	return TRUE;
}

//��ȡ�����ļ��������ܺ󱣴�(������)
BOOL CServerIpDesDlg::DecryptAndSave()
{
	//��ü��ܺ��ļ���(���ݱ������ļ�������)
	CString strSaveFileName = GetSaveFileName();

	TObjRef<DiscIOMgr>	pIO = "DiscIOMgr";
	DWORD dwHFileIn, dwHFileDec;		//��������ļ����
	DWORD dwWritten = 0, dwInFileSize;	//һ��д���ļ��Ĵ�С(�ַ�)

	//�򿪱������ļ�
		dwHFileIn = pIO->Create(strSaveFileName,
			GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 
			NULL);

	if(dwHFileIn == INVALID_FILE_HANDLE)
	{
		return FALSE;
	}

	//�򿪼��ܺ������ļ�
	dwHFileDec = pIO->Create(_T("out_dec.ini"),
		GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 
		NULL);

	if(dwHFileDec == INVALID_FILE_HANDLE)
	{
		pIO->Close(dwHFileIn);
		return FALSE;
	}

	//�õ��������ļ���С
	dwInFileSize = pIO->GetSize(dwHFileIn);
	
	//dwInFileSize = n * 8 + 4
	ASSERT(((dwInFileSize >> 3) << 3) + 4 == dwInFileSize);

	//�����ȡ�ڴ�
	char *pReadBuf = new char[dwInFileSize];
	if(NULL == pReadBuf)
	{
 		pIO->Close(dwHFileIn);
 		pIO->Close(dwHFileDec);
		return FALSE;
	}

	//��ȡ�ļ�
	pIO->Read(dwHFileIn, pReadBuf, dwInFileSize);
	//���ܺ󱣴�
	m_pUtil->Decrypt(pReadBuf + 4, pReadBuf + 4, dwInFileSize - 4, *(INT*)pReadBuf, 0);
	pIO->Write(dwHFileDec, pReadBuf + 4, dwInFileSize - 4, &dwWritten, NULL);

	//�����ڴ棬���ر��ļ�
	delete []pReadBuf;
	pIO->Close(dwHFileIn);
	pIO->Close(dwHFileDec);

	return TRUE;
}

//���ñ������ļ�
void CServerIpDesDlg::OnBnClickedButtonBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		szFilter=_T("INI�ļ�(*.ini)|*.ini||");   
	CFileDialog	dlgFile(TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, AfxGetMainWnd());   

	INT_PTR nResponse = dlgFile.DoModal();
	if (nResponse == IDOK)
	{
		m_strFileName = dlgFile.GetPathName();
		UpdateData(FALSE);
	}
}

BOOL CServerIpDesDlg::IsLegal()
{
	TObjRef<DiscIOMgr> pDisc;

	LPTSTR pFilePath = GetStr(byFilePath, sizeof(byFilePath));
	BOOL bRtv = pDisc->IsFileExist(pFilePath);	
	RetStr(pFilePath);

	return bRtv;
}