// Cool3DViewWnd.h : CCool3DViewWnd ��Ľӿ�
//


#pragma once
#include "afxwin.h"
#include "ColorButton.h"
#include "ActMsgTableDlg.h"
#include "BladeGlowDlg.h"
#include "afxcmn.h"
class CCool3DViewDoc;


class CCool3DViewWnd : public CFormView
{
protected: // �������л�����
	CCool3DViewWnd();
	DECLARE_DYNCREATE(CCool3DViewWnd)

public:
	enum{ IDD = IDD_Cool3DVIEW_FORM };

// ����
public:
	CCool3DViewDoc* GetDocument() const;

// ����
public:

// ��д
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CCool3DViewWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
private:
	CStatic m_drawWnd;
public:
	afx_msg void OnBnClickedBkcolor();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedWireframe();
	bool m_bWireFrame;
	CActMsgTableDlg	m_actMsgDlg;
	CBladeGlowDlg m_bladeGlowDlg;
	CListBox		m_trackList;
protected:
	BOOL			m_bDrag;
	CPoint			m_DragPt;
	BOOL			m_bArcBall;
	CColorButton	m_amb;
	CColorButton	m_diff;
	CColorButton	m_spec;
	CListBox		m_childList;
	CEdit			m_hFov;
	CEdit			m_hAniTimeScale;

	afx_msg void OnBnClickedAmbient();
	afx_msg void OnBnClickedDiffuse();
	afx_msg void OnBnClickedSpecular();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	afx_msg void OnBnClickedRemovechild();
	afx_msg void OnLbnSelchangeTracklist();
	afx_msg void OnEnChangeFov();

	void BuildChildrenList();
	void BuildTrackList();

	afx_msg void OnBnClickedShowskel();
	afx_msg void OnAttachmdl();
	afx_msg void OnBnClickedMsgtable();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEnChangeAnitimescale();
	afx_msg void OnShowNormal();
	afx_msg void OnShowTangent();
public:
	CEdit m_hPlayTime;
	CButton m_hLoopAni;
	afx_msg void OnBnClickedAnipause();
	CButton m_hAniPause;
	afx_msg void OnBladeglow();
	afx_msg void OnDyealphamap();
	afx_msg void OnSelecthorseman();
	afx_msg void OnSelecthorse();
	afx_msg void OnSelupperskeani();
	afx_msg void OnNMReleasedcaptureSlideryaw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderpitch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlideryaw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeYaw();
	CString m_yaw;
	CString m_pitch;
	CEdit m_hYaw;
	CEdit m_hPitch;
	afx_msg void OnEnChangePitch();
	CString m_headYaw;
	CString m_headPitch;
	CEdit m_hHeadYaw;
	CEdit m_hHeadPitch;
	afx_msg void OnEnChangeHeadyaw();
	afx_msg void OnEnChangeHeadpitch();
	afx_msg void OnLoaddefaultmanskin();
	afx_msg void OnLoaddefaultwomanskin();
	afx_msg void OnSem();
	afx_msg void OnCem();
	afx_msg void OnBnClickedArcball();
	BOOL m_bDarwSkel;
	afx_msg void OnBnClickedDrawskeleton();

	CButton m_hCBnEnableSnapShoter;				// ��/�ر����չ���
	CStatic m_hStSavePhotoInfo;					// ���ձ�����Ϣ
	CButton	m_hCBnPhotoUseMdlPath;				// �Ƿ�ʹ��ģ��·�����ļ���
	CButton m_hBnSavePhoto;						// ������Ƭ
	afx_msg void OnBnClickedEnablesnapshoter();
	afx_msg void OnBnClickedSavephoto();
	afx_msg void OnBlendtrack();
	afx_msg void OnModifylightmap();
	afx_msg void OnSplitSkel();
	afx_msg void OnUpdateaabbtree();
	afx_msg void OnModifyskin();
	afx_msg void OnCheckskel();
};

extern CCool3DViewWnd *g_viewWnd;

#ifndef _DEBUG  // Cool3DViewWnd.cpp �ĵ��԰汾
inline CCool3DViewDoc* CCool3DViewWnd::GetDocument() const
   { return reinterpret_cast<CCool3DViewDoc*>(m_pDocument); }
#endif

