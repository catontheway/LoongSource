// GamePackDlg.h : ͷ�ļ�
//

#pragma once

#include "textprogress.h"
#include "sfp_trunk.h"
#include "afxwin.h"

// �����̺߳�������
typedef UINT (WINAPI* THREADPROC)(LPVOID);
class CPK;
class CSelfExtract;
class CMD5;
//-----------------------------------------------------------
// CGamePackDlg �Ի���
class CGamePackDlg : public CDialog 
{
// ����
public:
	CGamePackDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GAMEPACK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	CTextProgress				m_Progress;		//������
	TSFPTrunk<CGamePackDlg>		m_Trunk;
	HANDLE						m_hGamePackThread;	//����߳̾��
	CSelfExtract*				m_pCpk;
	CPK*						m_pNewCpk;
	CPK*						m_pOldCpk;
	CMD5*						m_pMD5;
	FLOAT						m_fIntePos;			//
	INT							m_nFileNum;			//�ȶԵ��ļ���
	DWORD						m_dwTick;
	volatile	BOOL			m_bTerminateGamePack;	//�����̱߳�־
	volatile	BOOL			m_bPacking;			//�Ƿ����ڴ��

	CEdit*						m_pETOld;		//�ϰ汾Ŀ¼
	CEdit*						m_pETNew;		//�°汾Ŀ¼
	CEdit*						m_pETSave;		//�������洢Ŀ¼
	CStatic*					m_pStatic;		
	CStatic*					m_pSTimer;		//��ʾ����ʱ��ľ�̬��
public:
	//�汾����߳�
	UINT	GamePackThread(LPVOID);
	BOOL	GamePack(TCHAR *szNewPath, TCHAR *szOldPath);
protected:
	//�����ļ���չ���ǲ���.cpk�����Ҫ��ʵ�жϣ�����Ҫ�ж����ļ�����
	BOOL	IsCPKFile(TCHAR *szFileName);
	//�Ƚ�����MD5�Ƿ����
	BOOL	IsEqualForMD5(BYTE digest1[16], BYTE digest2[16]);
	//�ȶ��¾������汾��cpk�ļ�
	BOOL	CompareCPK(TCHAR *szNewCPK, TCHAR *szOldCPK);
	// -->> ��ָ����ֵд��ע�����ָ��·��,���ָ��·��û��,�򴴽���д��
	BOOL	WriteReg(TCHAR* KEY_ROOT/*ע����е�·��*/, TCHAR* KEY_NAME, LPCSTR sz);
	// -->> ��ȡע�����ָ��·���ļ�ֵ
	BOOL	ReadReg(TCHAR *KEY_ROOT, TCHAR* KEY_NAME, DWORD& key_value);
	BOOL	ReadReg(TCHAR* KEY_ROOT, TCHAR* KEY_NAME, TCHAR outstr[]);
	
	//����False��ʾ�˳�
	BOOL	AddFileAndDir( LPCTSTR lpFullName, DWORD dwFatherCrc = 0 );
// ʵ��
protected:
	HICON m_hIcon;
	
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtNewdir();
	afx_msg void OnBnClickedBtOlddir();
	afx_msg void OnBnClickedBtSavedir();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtSavedir2();
	afx_msg void OnBnClickedCheckUpdate();
	afx_msg void OnStnClickedStFile();
	afx_msg void OnEnChangeEditVersionMin();
	afx_msg void OnEnChangeEditVersionMax();
	
	BOOL m_bAddexe;
	CString m_strVersionSrc;
	CString m_strVersionDest;

	CEdit m_EditVersionSrc;
	CEdit m_EditVersionDest;
	CEdit m_pExePath;
	afx_msg void OnEnChangeEtSavedir();

	CString		m_strVersion;
};



