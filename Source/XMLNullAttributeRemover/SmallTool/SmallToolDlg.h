// SmallToolDlg.h : ͷ�ļ�
//

#pragma once



#include "tinystr.h"
#include "tinyxml.h"
#include <string>
#include <list>
#include "afxcmn.h"



// CSmallToolDlg �Ի���
class CSmallToolDlg : public CDialog
{
// ����
public:
	CSmallToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SMALLTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CMenu m_menu;
	CProgressCtrl m_ProgressBar;
	CString m_szInfo;
	BOOL m_bBackUp;
	std::list<CString> m_listFilePaths;
	BOOL m_bNotifyThreadEnd;
	HANDLE m_hThread;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
	void HandleOneXmlFile(const char *filename);
	void RemoveNullAttribute(TiXmlElement *child);
	void SetInfo(const char *filename, ULONGLONG oldSize, ULONGLONG newSize);
	void SearchAndAddToFileLists(CString szFile);
	static DWORD WINAPI ThreadFunc(PVOID param);
public:
	afx_msg LRESULT OnUpdateDataMessage(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnThreadEnd(WPARAM wparam, LPARAM lparam);
	afx_msg void OnFileOpensinglefile();
	afx_msg void OnFileFolder();
	afx_msg void OnBnClickedCheckBackup();
	afx_msg void OnAbout();
	afx_msg void OnClose();
};
