// Cool3DViewWnd.cpp : CCool3DViewWnd ���ʵ��
//

#include "stdafx.h"
#include "Cool3DView.h"

#include "Cool3DViewDoc.h"
#include "Cool3DViewWnd.h"

#include "Cool3DModels.h"
#include ".\Cool3Dviewwnd.h"

#include "AttachMdlDlg.h"
#include "SelUpperSkeAniDlg.h"
#include "BladeGlowDlg.h"
#include "DyeAlphaMapDlg.h"
#include "BlendTrackDlg.h"
#include "ModifyAttr.h"

#include "..\Cool3D\Cool3DEngine.h"
using namespace Cool3D;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCool3DViewWnd *g_viewWnd=NULL;

// CCool3DViewWnd

IMPLEMENT_DYNCREATE(CCool3DViewWnd, CFormView)

BEGIN_MESSAGE_MAP(CCool3DViewWnd, CFormView)
	ON_BN_CLICKED(IDC_BKCOLOR, OnBnClickedBkcolor)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_WIREFRAME, OnBnClickedWireframe)
	ON_BN_CLICKED(IDC_Ambient, OnBnClickedAmbient)
	ON_BN_CLICKED(IDC_Diffuse, OnBnClickedDiffuse)
	ON_BN_CLICKED(IDC_Specular, OnBnClickedSpecular)
	ON_BN_CLICKED(IDC_RemoveChild, OnBnClickedRemovechild)
	ON_LBN_SELCHANGE(IDC_TrackList, OnLbnSelchangeTracklist)
	ON_EN_CHANGE(IDC_Fov, OnEnChangeFov)
	ON_BN_CLICKED(IDC_ShowSkel, OnBnClickedShowskel)
	ON_COMMAND(ID_AttachMdl, OnAttachmdl)
	ON_BN_CLICKED(IDC_MsgTable, OnBnClickedMsgtable)
	ON_WM_CREATE()
	ON_EN_CHANGE(IDC_AniTimeScale, OnEnChangeAnitimescale)
	ON_BN_CLICKED(IDC_AniPause, OnBnClickedAnipause)
	ON_COMMAND(ID_BladeGlow, OnBladeglow)
	ON_COMMAND(ID_DyeAlphaMap, OnDyealphamap)
	ON_COMMAND(ID_SelectHorseman, OnSelecthorseman)
	ON_COMMAND(ID_SelectHorse, OnSelecthorse)
	ON_COMMAND(ID_SelUpperSkeAni, OnSelupperskeani)
	ON_EN_CHANGE(IDC_YAW, OnEnChangeYaw)
	ON_EN_CHANGE(IDC_PITCH, OnEnChangePitch)
	ON_EN_CHANGE(IDC_HEADYAW, OnEnChangeHeadyaw)
	ON_EN_CHANGE(IDC_HEADPITCH, OnEnChangeHeadpitch)
	ON_COMMAND(ID_LOADDEFAULTMANSKIN, OnLoaddefaultmanskin)
	ON_COMMAND(ID_LOADDEFAULTWOMANSKIN, OnLoaddefaultwomanskin)
	ON_COMMAND(ID_SEM, OnSem)
	ON_COMMAND(ID_CEM, OnCem)
	ON_BN_CLICKED(IDC_ArcBall, OnBnClickedArcball)
	ON_COMMAND(ID_ShowNormal, OnShowNormal)
	ON_COMMAND(ID_ShowTangent, OnShowTangent)
	ON_BN_CLICKED(IDC_DrawSkeleton, &CCool3DViewWnd::OnBnClickedDrawskeleton)
	ON_BN_CLICKED(IDC_EnableSnapshoter, &CCool3DViewWnd::OnBnClickedEnablesnapshoter)
	ON_BN_CLICKED(IDC_SavePhoto, &CCool3DViewWnd::OnBnClickedSavephoto)
	ON_COMMAND(ID_BlendTrack, &CCool3DViewWnd::OnBlendtrack)
	ON_COMMAND(ID_ModifyLightmap, &CCool3DViewWnd::OnModifylightmap)
	ON_COMMAND(ID_SplitSkel, &CCool3DViewWnd::OnSplitSkel)
	ON_COMMAND(ID_UpdateAABBTree, &CCool3DViewWnd::OnUpdateaabbtree)
	ON_COMMAND(ID_ModifySkin, &CCool3DViewWnd::OnModifyskin)
	ON_COMMAND(ID_CHECKSKEL, &CCool3DViewWnd::OnCheckskel)
END_MESSAGE_MAP()

// CCool3DViewWnd ����/����

CCool3DViewWnd::CCool3DViewWnd()
	: CFormView(CCool3DViewWnd::IDD)
	, m_bWireFrame(false)
	, m_bArcBall(false)
	, m_yaw(_T("0"))
	, m_pitch(_T("0"))
	, m_headYaw(_T("0"))
	, m_headPitch(_T("0"))
	, m_bDarwSkel(TRUE)
{
	// TODO: �ڴ˴���ӹ������
	m_bDrag=FALSE;
	g_viewWnd=this;
}

CCool3DViewWnd::~CCool3DViewWnd()
{
}

void CCool3DViewWnd::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DrawWnd, m_drawWnd);
	DDX_Check(pDX, IDC_ArcBall, m_bArcBall);
	DDX_Control(pDX, IDC_Ambient, m_amb);
	DDX_Control(pDX, IDC_Diffuse, m_diff);
	DDX_Control(pDX, IDC_Specular, m_spec);
	DDX_Control(pDX, IDC_ChildList, m_childList);
	DDX_Control(pDX, IDC_TrackList, m_trackList);
	DDX_Control(pDX, IDC_Fov, m_hFov);
	DDX_Control(pDX, IDC_AniTimeScale, m_hAniTimeScale);
	DDX_Control(pDX, IDC_AniPlayTime, m_hPlayTime);
	DDX_Control(pDX, IDC_AniLoop, m_hLoopAni);
	DDX_Control(pDX, IDC_AniPause, m_hAniPause);
	DDX_Text(pDX, IDC_YAW, m_yaw);
	DDX_Text(pDX, IDC_PITCH, m_pitch);
	DDX_Control(pDX, IDC_YAW, m_hYaw);
	DDX_Control(pDX, IDC_PITCH, m_hPitch);
	DDX_Text(pDX, IDC_HEADYAW, m_headYaw);
	DDX_Text(pDX, IDC_HEADPITCH, m_headPitch);
	DDX_Control(pDX, IDC_HEADYAW, m_hHeadYaw);
	DDX_Control(pDX, IDC_HEADPITCH, m_hHeadPitch);
	DDX_Check(pDX, IDC_DrawSkeleton, m_bDarwSkel);
	DDX_Control( pDX, IDC_EnableSnapshoter, m_hCBnEnableSnapShoter );
	DDX_Control( pDX, IDC_SavePhotoInfo, m_hStSavePhotoInfo );
	DDX_Control( pDX, IDC_PhotoUseMdlPath, m_hCBnPhotoUseMdlPath );
	DDX_Control( pDX, IDC_SavePhoto, m_hBnSavePhoto );
}

BOOL CCool3DViewWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ
	m_bladeGlowDlg.Create(IDD_BladeGlow,this);
	return CFormView::PreCreateWindow(cs);
}

void CCool3DViewWnd::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	static bool first=true;
	if(first)
	{
		first=false;
		CRect rc;
		rc.left=0;
		rc.top=0;
		rc.right=1000;
		rc.bottom=700;
		MoveWindow(rc);

		GetClientRect(rc);
		rc.left=rc.right-800;
		rc.top=4;
		rc.bottom=600+rc.top;
		AdjustWindowRect(rc,m_drawWnd.GetStyle(),FALSE);
		m_drawWnd.MoveWindow(rc);

		//GetParentFrame()->RecalcLayout();
		//ResizeParentToFit();

		Cool3DModels::Inst()->InitEngine(m_drawWnd.GetSafeHwnd());
	}
	SceneGraph *pSG=Cool3DModels::Inst()->GetSceneGraph();
	const tagLight& lgt=pSG->GetSunLight();
	m_amb.SetColor(RGB(lgt.ambient.R,lgt.ambient.G,lgt.ambient.B));
	m_diff.SetColor(RGB(lgt.diffuse.R,lgt.diffuse.G,lgt.diffuse.B));
	m_spec.SetColor(RGB(lgt.specular.R,lgt.specular.G,lgt.specular.B));

	CString szFov;
	szFov.Format(_T("%.2f"),_RadToDeg(Cool3DModels::Inst()->GetCamera()->GetFOV()));
	m_hFov.SetWindowText(szFov);

	m_hAniTimeScale.SetWindowText(_T("1.0"));
	m_hLoopAni.SetCheck(1);

	// ��ʼ�����տؼ�
	m_hCBnPhotoUseMdlPath.SetCheck( 1 );
	m_hCBnEnableSnapShoter.SetCheck( 0 );
	OnBnClickedEnablesnapshoter();
}


// CCool3DViewWnd ���

#ifdef _DEBUG
void CCool3DViewWnd::AssertValid() const
{
	CFormView::AssertValid();
}

void CCool3DViewWnd::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCool3DViewDoc* CCool3DViewWnd::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCool3DViewDoc)));
	return (CCool3DViewDoc*)m_pDocument;
}
#endif //_DEBUG


// CCool3DViewWnd ��Ϣ�������

void CCool3DViewWnd::OnBnClickedBkcolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CColorDialog dlg(0,0,this);
	if(IDOK==dlg.DoModal())
	{
		COLORREF c=dlg.GetColor();
		Color4ub Cool3Dc(255,GetRValue(c),GetGValue(c),GetBValue(c));
		Cool3DModels::Inst()->SetBkColor(Cool3Dc);
	}
}

BOOL CCool3DViewWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CFormView::PreTranslateMessage(pMsg);
}

void CCool3DViewWnd::OnLButtonDown(UINT nFlags, CPoint _point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UpdateData();
	if(m_bArcBall)
	{
		POINT point;
		point.x=_point.x;
		point.y=_point.y;
		ClientToScreen(&point);
		m_drawWnd.ScreenToClient(&point);
		CRect rc;
		m_drawWnd.GetClientRect(rc);
		if(rc.PtInRect(point))
			Cool3DModels::Inst()->GetArcBall()->OnMouseBtnDown(point.x,point.y);
	}
	else
	{
		m_DragPt=_point;
		ClientToScreen(&m_DragPt);
		m_drawWnd.ScreenToClient(&m_DragPt);
	}
	m_bDrag=TRUE;
	SetCapture();

	CFormView::OnLButtonDown(nFlags, _point);
}

void CCool3DViewWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_bArcBall)
		Cool3DModels::Inst()->GetArcBall()->OnMouseBtnUp();
	
	m_bDrag=FALSE;
	ReleaseCapture();

	CFormView::OnLButtonUp(nFlags, point);
}

void CCool3DViewWnd::OnMouseMove(UINT nFlags, CPoint _point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_bDrag)
	{
		if(m_bArcBall)
		{
			POINT point;
			point.x=_point.x;
			point.y=_point.y;

			ClientToScreen(&point);
			m_drawWnd.ScreenToClient(&point);
			CRect rc;
			m_drawWnd.GetClientRect(rc);
			if(rc.PtInRect(point))
				Cool3DModels::Inst()->GetArcBall()->OnMouseMove(point.x,point.y);
		}
		else
		{
			CPoint pt=_point;
			ClientToScreen(&pt);
			m_drawWnd.ScreenToClient(&pt);
			float x=float(pt.x-m_DragPt.x);
			float y=float(pt.y-m_DragPt.y);

			CameraEuler *pCamera=Cool3DModels::Inst()->GetCamera();
			if(nFlags&MK_CONTROL)//��סCtrl,�ƶ�Camera
			{
				Vector3 pos(-x,y,0);
				pos*=1.5f;
				pCamera->Move(pos);
			}
			else
			{
				pCamera->SetYaw(pCamera->GetYaw()+_DegToRad(-x*0.15f));
				pCamera->SetPitch(pCamera->GetPitch()+_DegToRad(-y*0.15f));
			}

			m_DragPt=pt;
		}
	}

	CFormView::OnMouseMove(nFlags, _point);
}

BOOL CCool3DViewWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	float dist=Cool3DModels::Inst()->GetCamera()->GetDist();
	Cool3DModels::Inst()->GetCamera()->SetDist(dist+(zDelta/500.0f*dist));

	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}

void CCool3DViewWnd::OnBnClickedWireframe()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(IsDlgButtonChecked(IDC_WIREFRAME)>0)
		m_bWireFrame=true;
	else
		m_bWireFrame=false;
		
	if(m_bWireFrame)
		Device()->SetFillMode(EFill_Wireframe);
	else
		Device()->SetFillMode(EFill_Solid);
}

void CCool3DViewWnd::OnBnClickedAmbient()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CColorDialog dlg(0,0,this);
	if(IDOK==dlg.DoModal())
	{
		COLORREF c=dlg.GetColor();
		m_amb.SetColor(c);
		Color4f Cool3Dc(1,GetRValue(c)/255.0f,GetGValue(c)/255.0f,GetBValue(c)/255.0f);
		SceneGraph *pSG=Cool3DModels::Inst()->GetSceneGraph();
		tagLight lgt=pSG->GetSunLight();
		lgt.ambient=Cool3Dc;
		pSG->SetSunLight(lgt);
	}
}

void CCool3DViewWnd::OnBnClickedSpecular()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CColorDialog dlg(0,0,this);
	if(IDOK==dlg.DoModal())
	{
		COLORREF c=dlg.GetColor();
		m_spec.SetColor(c);
		Color4f Cool3Dc(1,GetRValue(c)/255.0f,GetGValue(c)/255.0f,GetBValue(c)/255.0f);
		SceneGraph *pSG=Cool3DModels::Inst()->GetSceneGraph();
		tagLight lgt=pSG->GetSunLight();
		lgt.specular=Cool3Dc;
		pSG->SetSunLight(lgt);
	}
}

void CCool3DViewWnd::OnBnClickedDiffuse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CColorDialog dlg(0,0,this);
	if(IDOK==dlg.DoModal())
	{
		COLORREF c=dlg.GetColor();
		m_diff.SetColor(c);
		Color4f Cool3Dc(1,GetRValue(c)/255.0f,GetGValue(c)/255.0f,GetBValue(c)/255.0f);
		SceneGraph *pSG=Cool3DModels::Inst()->GetSceneGraph();
		tagLight lgt=pSG->GetSunLight();
		lgt.diffuse=Cool3Dc;
		pSG->SetSunLight(lgt);
	}
}

void CCool3DViewWnd::BuildChildrenList()
{
	//ֻ��AvatarNode���ж��ӽڵ�
	SceneNode *pNode=Cool3DModels::Inst()->GetAvatarNode();
	if(pNode==NULL)
		return;
	//--build child list
	m_childList.ResetContent();
	int n=pNode->GetNumChildren();
	int i;
	for(i=0;i<n;i++)
	{
		const TCHAR *szName=pNode->GetChild(i)->GetUserData();
		ASSERT(szName!=NULL);
		m_childList.AddString(szName);
	}
}

void CCool3DViewWnd::BuildTrackList()
{
	//--build ani track list
	m_trackList.ResetContent();
	AnimationCtrl *pAni=Cool3DModels::Inst()->GetSceneNodeAni();

	if(pAni)
	{
		vector<tstring> tracks;
		pAni->GetTrackSet(tracks);
		sort( tracks.begin(), tracks.end() );
		for(vector<tstring>::iterator iter=tracks.begin();
			iter!=tracks.end();iter++)
		{
			m_trackList.AddString(iter->c_str());
		}
	}
}

void CCool3DViewWnd::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	BuildChildrenList();
	BuildTrackList();
}

void CCool3DViewWnd::OnBnClickedRemovechild()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//ֻ��AvatarNode�����ӽڵ�
	SceneNode *pNode=Cool3DModels::Inst()->GetAvatarNode();
	if(pNode==NULL)
		return;
	int selIndex=m_childList.GetCurSel();
	if(selIndex==-1)
		return;
	CString szSel;
	m_childList.GetText(selIndex,szSel);
	int i;
	for(i=0;i<pNode->GetNumChildren();i++)
	{
		SceneNode *pChild=const_cast<SceneNode*>(pNode->GetChild(i));
		const TCHAR *szName=pChild->GetUserData();
		if(szSel==szName)
		{
			pNode->DetachChild(pChild);
			delete pChild;
		}
	}

	//--update list
	BuildChildrenList();
}

void CCool3DViewWnd::OnLbnSelchangeTracklist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int selIndex=m_trackList.GetCurSel();
	if(selIndex==-1)
		return;
	CString szSel;
	m_trackList.GetText(selIndex,szSel);

	AnimationCtrl *pAni=Cool3DModels::Inst()->GetSceneNodeAni();
	if(pAni==NULL)
		return;

	if(m_hLoopAni.GetCheck())
		pAni->OpenTrack(szSel,true);
	else
		pAni->OpenTrack(szSel,false);
}

void CCool3DViewWnd::OnEnChangeFov()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CFormView::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString szFov;
	m_hFov.GetWindowText(szFov);

	Cool3DModels::Inst()->SetCameraFov(_DegToRad((float)_tstof(szFov)));
}
void CCool3DViewWnd::OnBnClickedShowskel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Cool3DModels::Inst()->ShowSkelDlg();
}

void CCool3DViewWnd::OnAttachmdl()
{
	// TODO: �ڴ���������������
	CAttachMdlDlg dlg;
	if(IDOK==dlg.DoModal())
	{
		Cool3DModels::Inst()->AttachMdl(dlg.m_szMdlPath,dlg.m_szTagNodeName);
	}
}

void CCool3DViewWnd::OnBnClickedMsgtable()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString szResName=Cool3DModels::Inst()->GetSceneNodeResName();
	AnimationCtrl *pAni=Cool3DModels::Inst()->GetSceneNodeAni();
	if(szResName.IsEmpty() || pAni==NULL)
		return;

	m_actMsgDlg.InitMsgTable(szResName,pAni);
	m_actMsgDlg.ShowWindow(SW_SHOW);
}

int CCool3DViewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_actMsgDlg.Create(IDD_ActMsgTable,this);
	m_actMsgDlg.ShowWindow(SW_HIDE);

	return 0;
}

void CCool3DViewWnd::OnEnChangeAnitimescale()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CFormView::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	AnimationCtrl *pAniCtrl=Cool3DModels::Inst()->GetSceneNodeAni();
	if(pAniCtrl!=NULL)
	{
		CString szVal;
		m_hAniTimeScale.GetWindowText(szVal);
		float t=(float)_tstof(szVal);
		pAniCtrl->SetTimeFactor(t);
	}
}

void CCool3DViewWnd::OnBnClickedAnipause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AnimationCtrl *pAniCtrl=Cool3DModels::Inst()->GetSceneNodeAni();
	if(pAniCtrl==NULL)
		return;

	CString szTxt;
	m_hAniPause.GetWindowText(szTxt);
	if(szTxt==_T("��ͣ"))
	{
		m_hAniPause.SetWindowText(_T("����"));
		pAniCtrl->SetTimeFactor(0.0f);
	}
	else
	{
		m_hAniPause.SetWindowText(_T("��ͣ"));
		OnEnChangeAnitimescale();
	}
}

void CCool3DViewWnd::OnBladeglow()
{
	if(m_bladeGlowDlg.Init())
		m_bladeGlowDlg.ShowWindow(SW_SHOW);
}

void CCool3DViewWnd::OnDyealphamap()
{
	CDyeAlphaMapDlg dlg;
	dlg.DoModal();
}

void CCool3DViewWnd::OnSelecthorseman()
{
	//ѡ��������
	CFileDialog dlg(TRUE,_T("FSKEL"),_T("*.FSKEL"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("�����ļ�(*.FSKEL)"),
		this,0);
	if(IDOK==dlg.DoModal())
	{
		UpdateData();

		CString szPath=dlg.GetPathName();

		Cool3DModels::Inst()->LoadHorsemanModel(szPath);

		BuildChildrenList();

		//Ϊģ��·��
		UpdateData(FALSE);
	}
}

void CCool3DViewWnd::OnSelecthorse()
{
	//ѡ����
	CFileDialog dlg(TRUE,_T("FSKEL"),_T("*.FSKEL"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("�����ļ�(*.FSKEL)"),
		this,0);
	if(IDOK==dlg.DoModal())
	{
		UpdateData();

		CString szPath=dlg.GetPathName();

		Cool3DModels::Inst()->LoadHorseModel(szPath);

		BuildChildrenList();
		BuildTrackList();

		UpdateData(FALSE);
	}
}

void CCool3DViewWnd::OnSelupperskeani()
{
	// TODO: �ڴ���������������
	CSelUpperSkeAniDlg dlg;
	if(IDOK==dlg.DoModal())
	{
		if (dlg.m_bIsOneTrack)
		{
			Cool3DModels::Inst()->UpdateAllSkeAni(dlg.m_szLowerSkeAniName);
		}
		else
		{
			if (!dlg.m_szUpperSkeAniName.IsEmpty())
			{
				Cool3DModels::Inst()->UpdateUpperSkeAni(dlg.m_szUpperSkeAniName);
			}
			if (!dlg.m_szLowerSkeAniName.IsEmpty())
			{
				Cool3DModels::Inst()->UpdateLowerSkeAni(dlg.m_szLowerSkeAniName);
			}
		}
	}
	//Cool3DModels::Inst()->RotateUpperSkeAni(m_yawRad/100.0f, m_pitchRad/100.0f, 0);
}

void CCool3DViewWnd::OnBlendtrack()
{
	// TODO: Add your command handler code here
	CBlendTrackDlg dlg;
	if(IDOK==dlg.DoModal())
	{
		if( !dlg.m_szUpperSkeAniName.IsEmpty() )
		{
			Cool3DModels::Inst()->BlendUpperSkeAni(dlg.m_szUpperSkeAniName,dlg.m_fUpperWeight);
		}
		if( !dlg.m_szLowerSkeAniName.IsEmpty() )
		{
			Cool3DModels::Inst()->BlendLowerSkeAni(dlg.m_szLowerSkeAniName,dlg.m_LowerWeight);
		}
		if( !dlg.m_szAllSkeAniName.IsEmpty() )
		{
			Cool3DModels::Inst()->BlendAllSkeAni(dlg.m_szAllSkeAniName,dlg.m_fAllWeight);
		}
	}
}


void CCool3DViewWnd::OnEnChangeYaw()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CFormView::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_hYaw.GetWindowText(m_yaw);

	Cool3DModels::Inst()->RotateUpperSkeAni(_DegToRad((float)_tstof(m_yaw)), _DegToRad((float)_tstof(m_pitch)), 0);
}

void CCool3DViewWnd::OnEnChangePitch()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CFormView::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_hPitch.GetWindowText(m_pitch);

	Cool3DModels::Inst()->RotateUpperSkeAni(_DegToRad((float)_tstof(m_yaw)), _DegToRad((float)_tstof(m_pitch)), 0);
}

void CCool3DViewWnd::OnEnChangeHeadyaw()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CFormView::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_hHeadYaw.GetWindowText(m_headYaw);

	Cool3DModels::Inst()->RotateHeadSkeAni(_DegToRad((float)_tstof(m_headYaw)), _DegToRad((float)_tstof(m_headPitch)), 0);
}

void CCool3DViewWnd::OnEnChangeHeadpitch()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CFormView::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_hHeadPitch.GetWindowText(m_headPitch);

	Cool3DModels::Inst()->RotateHeadSkeAni(_DegToRad((float)_tstof(m_headYaw)), _DegToRad((float)_tstof(m_headPitch)), 0);
}

void CCool3DViewWnd::OnLoaddefaultmanskin()
{
	// TODO: �ڴ���������������
	if (Cool3DModels::Inst()->LoadSkin(Cool3DModels::ST_BOY))
		BuildChildrenList();
}

void CCool3DViewWnd::OnLoaddefaultwomanskin()
{
	// TODO: �ڴ���������������
	if (Cool3DModels::Inst()->LoadSkin(Cool3DModels::ST_GIRL))
		BuildChildrenList();
}

void CCool3DViewWnd::OnSem()
{
	// TODO: �ڴ���������������
	Cool3DModels::Inst()->ChangeSEMMtl(_T("spheremap.bmp"));
}

void CCool3DViewWnd::OnCem()
{
	// TODO: �ڴ���������������
}

void CCool3DViewWnd::OnBnClickedArcball()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	m_bArcBall=!m_bArcBall;
}

void CCool3DViewWnd::OnShowNormal()
{
	Cool3DModels::Inst()->ShowNormal();
}

void CCool3DViewWnd::OnShowTangent()
{
	Cool3DModels::Inst()->ShowTangent();
}

void CCool3DViewWnd::OnBnClickedDrawskeleton()
{
	UpdateData();
	Cool3DModels::Inst()->SetShowSkel(m_bDarwSkel==TRUE);
}

void CCool3DViewWnd::OnBnClickedEnablesnapshoter()
{
	BOOL bEnable = m_hCBnEnableSnapShoter.GetCheck();
	Cool3DModels::Inst()->EnableSnapshoter( bEnable==TRUE );

	m_hBnSavePhoto.EnableWindow( bEnable );
	m_hCBnPhotoUseMdlPath.EnableWindow( bEnable );
	m_hStSavePhotoInfo.EnableWindow( bEnable );
}

void CCool3DViewWnd::OnBnClickedSavephoto()
{
	CString strPointExt = _T(".");
	strPointExt += Cool3DModels::Inst()->GetPhotoFileExt();

	// ʹ��ģ��·�����ļ���
	if( 1 == m_hCBnPhotoUseMdlPath.GetCheck() )
	{
		CString strAppend = _T("_ico");                                    // ʹ��ģ���ļ���,׷��_ico
		tstring strFileName = Cool3DModels::Inst()->GetCurMdlPath();
		if( strFileName.empty() )
		{
			m_hStSavePhotoInfo.SetWindowText( _T("��û�д�ģ�ͣ�") );
			return;
		}

		tstring::size_type pos = strFileName.find_last_of( _T('\\') );
		if( tstring::npos != pos )
		{
			strFileName.erase( pos );
			pos = strFileName.find_last_of( _T('\\') );
			if( tstring::npos != pos )
			{
				tstring strName = strFileName.substr( pos + 1 );
				strFileName += _T("\\");
				strFileName += strName;
				strFileName += _T("_ico.tga");
			}
			else
			{
				m_hStSavePhotoInfo.SetWindowText( _T("ģ���ļ����쳣��") );
				return;
			}
		}
		else
		{
			m_hStSavePhotoInfo.SetWindowText( _T("ģ���ļ����쳣��") );
			return;
		}

		if( !Cool3DModels::Inst()->SavePhoto( strFileName.c_str() ) )
		{
			m_hStSavePhotoInfo.SetWindowText( _T("����ʧ��!") );
			return;
		}

		strFileName += _T("�ѱ���.");
		m_hStSavePhotoInfo.SetWindowText( strFileName.c_str() );
		return;
	}

	// �ֶ�ָ��·�����ļ���
	CString strFilter;
	strFilter.Format( _T("%s�ļ�|*%s||"), strPointExt, strPointExt );
	CFileDialog dlg( FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, this, 0 );

	if( IDOK == dlg.DoModal() )
	{
		CString strFileName = dlg.GetPathName();
		strFileName.MakeLower();
		if( -1 == strFileName.Find( strPointExt ) )
			strFileName += strPointExt;

		if( !Cool3DModels::Inst()->SavePhoto( strFileName ) )
		{
			m_hStSavePhotoInfo.SetWindowText( _T("����ʧ��!") );
			return;
		}

		strFileName += _T("�ѱ���.");
		m_hStSavePhotoInfo.SetWindowText( strFileName );
	}
}
void CCool3DViewWnd::OnModifylightmap()
{
	CModifyAttr dlg;
	dlg.m_iLightmapSize=Cool3DModels::Inst()->GetLightmapSize();
	if(IDOK==dlg.DoModal())
	{
		Cool3DModels::Inst()->ModifyLightmapSize(dlg.m_iLightmapSize);
	}
}
void CCool3DViewWnd::OnSplitSkel()
{
	Cool3DModels::Inst()->SplitSkel();
}

tstring GetPath()
{
	tstring strPath = _T("");
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner = NULL; //m_hWnd;
	bInfo.lpszTitle = _T("��ѡ��·��: ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;   

	LPITEMIDLIST lpDlist; //�������淵����Ϣ��IDList
	lpDlist = SHBrowseForFolder(&bInfo) ; //��ʾѡ��Ի���
	if(lpDlist != NULL)  //�û�����ȷ����ť
	{
		TCHAR chPath[255]; //�����洢·�����ַ���
		SHGetPathFromIDList(lpDlist, chPath); //����Ŀ��ʶ�б�ת�����ַ���
		strPath = chPath; //��TCHAR���͵��ַ���ת��ΪCString���͵��ַ���
	}
	return strPath;
}

void CCool3DViewWnd::OnUpdateaabbtree()
{
	tstring strDir = GetPath();

	if( !strDir.empty() )
	{
		Cool3DModels::Inst()->UpdateAABBTree( strDir.c_str() );
	}
}

void CCool3DViewWnd::OnModifyskin()
{
	tstring strDir = GetPath();

	if( !strDir.empty() )
	{
		Cool3DModels::Inst()->ModifySkin( strDir.c_str() );
	}
}

void CCool3DViewWnd::OnCheckskel()
{
	Cool3DModels::Inst()->CheckSkel();
}
