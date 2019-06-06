#pragma once
#include "afxcmn.h"


// CSkelDlg �Ի���
class Cool3D::ResSkeleton;
class CSkelDlg : public CDialog
{
	DECLARE_DYNAMIC(CSkelDlg)

public:
	CSkelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSkelDlg();

// �Ի�������
	enum { IDD = IDD_SkelDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	

public:
	Cool3D::ResSkeleton *m_pRes;
	virtual BOOL OnInitDialog();
	CTreeCtrl m_skelTree;
	CString m_TagNodes;
};
