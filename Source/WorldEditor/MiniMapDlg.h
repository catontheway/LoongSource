#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// MiniMap �Ի���
#define WM_DELMINIMAP_DLG WM_USER+104

class EditorCameraCtrl;
class MiniMapDlg : public CDialog
{
	DECLARE_DYNAMIC(MiniMapDlg)
public:
	MiniMapDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MiniMapDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_Terrain };
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);		
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnPaint();
	//˫��ʵ���������λ
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CWnd*			m_pParent;
	int				m_nWidth;				//�߶�ͼ�ķ�Χ
	int				m_nHeight;
	TerrainEditor*	m_pTE;
	GameMapEditor*	m_pGameMapEditor;		
	EditorCameraCtrl*	m_pCamera;
	CClientDC*		m_ClientDC;
	CRect			m_rect;					//�ͻ�����Χ
	int				m_xScale;				//���ű���
	int				m_zScale;
	float			m_xGrid;				//���Ӵ�С
	float			m_zGrid;	
private:
	void GetRGB(float& gene, BYTE& r, BYTE& g, BYTE& b);  //���ݸ߶����ӵõ�RGB��ɫ		
	void DrawCamera();						//��С��ͼ�ϻ��������
	float HeightGene(float& h);				//�߶�����
	void Update();
	void DrawStaticObjOnMiniMap();
};
