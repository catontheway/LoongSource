#pragma once
#include "afxwin.h"

//--�ر����Ͷ���
enum SURFACE_TYPE
{
	SURFACETYPE_None		=0,			//��
	SURFACETYPE_Snow,					//ѩ��
	SURFACETYPE_Block,					//ʯ��
	SURFACETYPE_Earth,					//��
	SURFACETYPE_Grass,					//�ݵ�
	SURFACETYPE_Sand,					//ɳ��
	SURFACETYPE_Ice,					//����
	SURFACETYPE_Metal,					//����
	SURFACETYPE_WOOD,					//ľͷ
	SURFACETYPE_NUM,
};

// CLayerAttDlg �Ի���

class CLayerAttDlg : public CDialog
{
	DECLARE_DYNAMIC(CLayerAttDlg)

public:
	CLayerAttDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLayerAttDlg();

// �Ի�������
	enum { IDD = IDD_LayerAtt };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedFiledlg();
	afx_msg void OnBnClickedOk();

public:
	bool	m_bMod;//�Ƿ����޸Ĳ�����
	CString m_szName;
	CString m_szTex;
	CString m_szUVSetIndex;
	int		m_typeIndex;
	virtual BOOL OnInitDialog();
	CComboBox m_typeList;
};
