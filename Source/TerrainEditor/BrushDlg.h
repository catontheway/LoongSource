#pragma once
#include "afxwin.h"

// CBrushDlg �Ի���

enum EEditorLerpType
{
	EELerp_None=0,		//�޲�ֵ
	EELerp_Linear,		//���Բ�ֵ
	EELerp_Cosine,		//cos��ֵ
	EELerp_Quadratic,	//���β�ֵ
};
enum EEditorShapeType
{
	EEShape_Circle=0,	//Բ�α�ˢ
	EEShape_Quad,		//���α�ˢ��������������Quad
	EEShape_Line,		//���ͱ�ˢ��
	EEShape_Plane,		//ƽ����ˢ
	EEShape_TypeDef,	//�Զ����ı��Σ����ڱ༭�ı��������б��
};
struct tagBrushOpt
{
	int					inner;		//��ˢ�ھ���С
	int					outer;		//��ˢ�⾶��С
	int					strength;
	EEditorShapeType	shape;
	EEditorLerpType		lerp;
};

class CBrushDlg : public CDialog
{
	DECLARE_DYNAMIC(CBrushDlg)

public:
	CBrushDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBrushDlg();

// �Ի�������
	enum { IDD = IDD_BRUSH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	void GetNoiseSet(float& freq);
	void GetSetting(int& inner,int& outer,int *pStr=NULL);
	int GetLerpType();
	int GetShape();

	tagBrushOpt GetBrushOpt() const;
	void SetBrushOpt(const tagBrushOpt& brushOpt);

protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRenoise();

	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	CString m_szInner;
	CString m_szOuter;
	CString m_szStr;
	BOOL m_bLock;
	CString m_szFreq;

	tagBrushOpt m_brushOpt;
public:
	int m_lerpType;
	CComboBox m_hLerpType;
	CComboBox m_hShapeType;
};
