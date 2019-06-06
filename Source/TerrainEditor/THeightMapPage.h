#pragma once
#include "afxwin.h"

const int HEditToolsNum=5;
const TCHAR* const HEditToolsName[]=
{
	_T("Brush"),
	_T("Flatten"),
	_T("Smooth"),
	_T("Noise"),
	_T("Apply")
};

const TCHAR* const HEditToolsInfo[]=
{
	_T("Brush:\t������������ڵ�������϶�,����߶�ͼӦ�ñ�ˢ;������Ϊ��ˢ��Ӧ��,����Ҽ�Ϊ��Ӧ��"),
	_T("Flatten:\tʹ�ñ�ˢ��ȷ����������ƽ̹"),
	_T("SMooth:\tʹ�ñ�ˢ��ȷ����������ƽ��"),
	_T("Noise:\t�ڱ�ˢ��ȷ����������������Ӹߵͱ仯"),
	_T("Apply:\t���ñ�ˢ��ȷ��������Ϊָ��ֵ")
};

// THeightMapPage �Ի���

class THeightMapPage : public CPropertyPage
{
	DECLARE_DYNAMIC(THeightMapPage)

public:
	THeightMapPage();
	virtual ~THeightMapPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_HEIGHTMAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedImporthightmap();
	afx_msg void OnBnClickedCreate();
	CListBox m_editTools;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeEdittools();

	CString m_szEditToolInfo;
	BOOL m_bWireFrame;
protected:
	void SetDrawOption();
public:
	afx_msg void OnBnClickedWireframe();
	CString m_szName;
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	CString m_szBuildPath;
	afx_msg void OnBnClickedBuild();
	CButton m_btnBuild;
	CButton m_btnShowEdge;
	CString m_szShowDist;
	CString m_szEditDist;
	afx_msg void OnEnChangeShowdist();
	afx_msg void OnEnChangeEditdist();
	afx_msg void OnBnClickedMod();
	afx_msg void OnBnClickedEdituvset();
	afx_msg void OnBnClickedExporthightmap();
	afx_msg void OnBnClickedUndo();
	afx_msg void OnBnClickedRedo();
};
