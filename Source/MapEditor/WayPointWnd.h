#pragma once
#include "afxwin.h"


// WayPointWnd �Ի���

class WayPointWnd : public CDialog
{
	DECLARE_DYNAMIC(WayPointWnd)

public:
	WayPointWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~WayPointWnd();

// �Ի�������
	enum { IDD = IDD_WayPoint };
	CString GetWayPointName(){return m_strWayPtName;}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditWayPoint;
	CString m_strWayPtName;
	afx_msg void OnBnClickedOk();
};
