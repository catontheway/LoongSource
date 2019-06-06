// EffectPropDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SFXEditor.h"
#include "EffectPropDlg.h"
#include ".\effectpropdlg.h"

void Deg2Rad3f(Vector3& rot)
{
	rot.x=_DegToRad(rot.x);
	rot.y=_DegToRad(rot.y);
	rot.z=_DegToRad(rot.z);
}

// CEffectPropDlg �Ի���

IMPLEMENT_DYNAMIC(CEffectPropDlg, CDialog)
CEffectPropDlg::CEffectPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEffectPropDlg::IDD, pParent)
	, m_szStateIndex(_T("0"))
{
	m_bPrivew=true;
	m_pEditEffect=NULL;
	m_numStates=1;
	m_pEftPath=NULL;
	m_pEftRender=NULL;
}

CEffectPropDlg::~CEffectPropDlg()
{
}

void CEffectPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MtlProp, m_mtlProp);
	DDX_Control(pDX, IDC_Preview, m_hPreviewWnd);
	DDX_Text(pDX, IDC_Index, m_szStateIndex);
	DDX_Control(pDX, IDC_State, m_stateProp);
	DDX_Control(pDX, IDC_Time, m_hShowTime);
	DDX_Control(pDX, IDC_ActList, m_actListBox);
	DDX_Control(pDX, IDC_TimeScale, m_edTimeScale);
	DDX_Control(pDX, IDC_DrawBox, m_hDrawBox);
}


BEGIN_MESSAGE_MAP(CEffectPropDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(ID_ApplyEdit, OnBnClickedApplyedit)
	ON_EN_CHANGE(IDC_Index, OnEnChangeIndex)
	ON_BN_CLICKED(IDC_IndexInc, OnBnClickedIndexinc)
	ON_BN_CLICKED(IDC_IndexDec, OnBnClickedIndexdec)
	ON_BN_CLICKED(IDC_DelState, OnBnClickedDelstate)
	ON_BN_CLICKED(IDC_AddState, OnBnClickedAddstate)
	ON_BN_CLICKED(IDC_OpenMdl, OnBnClickedOpenmdl)
	ON_BN_CLICKED(IDC_WireFrame, OnBnClickedWireframe)
	ON_BN_CLICKED(IDC_Play, OnBnClickedPlay)
	ON_BN_CLICKED(IDC_Pause, OnBnClickedPause)
	ON_CBN_SELCHANGE(IDC_ActList, OnCbnSelchangeActlist)
	ON_EN_CHANGE(IDC_TimeScale, OnEnChangeTimescale)
	ON_BN_CLICKED(IDC_ShowMdl, OnBnClickedShowmdl)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BKColor, OnBnClickedBkcolor)
	ON_LBN_SELCHANGE(IDC_State, OnLbnSelchangeState)
	ON_LBN_SELCHANGE(IDC_MtlProp, OnLbnSelchangeMtlprop)
	ON_BN_CLICKED(IDC_DrawBox, &CEffectPropDlg::OnBnClickedDrawbox)
END_MESSAGE_MAP()


// CEffectPropDlg ��Ϣ�������

void CEffectPropDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DetachEffectFromPreView();

	OnOK();
}

void CEffectPropDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DetachEffectFromPreView();

	OnCancel();
}

BOOL CEffectPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString szTitle=_T("��ЧԪ������ - ");
	if(m_pEditEffect!=NULL)
	{
		szTitle+=m_pEditEffect->GetClassName();
		SetWindowText(szTitle);

		CopyEftData(m_pEditEffect);
	}


	m_mtlProp.SetFileFilter(_T("TGA�ļ�|*.tga||"));
	
	if(m_bPrivew)
	{
		SetTimer(1,20,0);
		m_preView.Init(m_pEftRender);
		m_preView.GetEffectNode()->AddEffectPath(m_pEftPath);
		m_preView.LoadDefaultAvatar(1);
		m_preView.UpdateActList(&m_actListBox);

		CRect rc;
		m_hPreviewWnd.GetWindowRect(rc);
		int h=rc.Width()*600/800;
		rc.bottom=rc.top+h;
		ScreenToClient(rc);
		m_hPreviewWnd.MoveWindow(rc,FALSE);
	}

	if(!m_pEditEffect->IS_KIND_OF(EffectPathCtrl))
		UpdateMtlProp(false);
	UpdateStateProp(_tstoi(m_szStateIndex),false);

	SetTimer(2,1000,0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CEffectPropDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch(nIDEvent)
	{
	case 1:
		{
			HWND hWnd=m_hPreviewWnd.GetSafeHwnd();
			if(::IsWindow(hWnd))
				m_preView.Render(hWnd,false);

			CString szTime;
			szTime.Format(_T("%.2f"),m_preView.GetEffectNode()->GetRunTime());
			m_hShowTime.SetWindowText(szTime);
		}
		break;
	case 2:
		{
			int sel=m_mtlProp.GetCurSel();
			if(sel!=-1)
				ShowSelectedPropInfo(&m_mtlProp);
			else
				ShowSelectedPropInfo(&m_stateProp);
		}
		break;
	}//endof switch

	CDialog::OnTimer(nIDEvent);
}

void CEffectPropDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_preView.OnLButtonDown(nFlags,point);

	CDialog::OnLButtonDown(nFlags, point);
}

void CEffectPropDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_preView.OnLButtonUp(nFlags,point);

	CDialog::OnLButtonUp(nFlags, point);
}

void CEffectPropDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_preView.OnMouseMove(nFlags, point);

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CEffectPropDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_preView.OnMouseWheel(nFlags, zDelta, pt);

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CEffectPropDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_preView.OnRButtonDown(nFlags, point);

	CDialog::OnRButtonDown(nFlags, point);
}

void CEffectPropDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_preView.OnRButtonUp(nFlags, point);

	CDialog::OnRButtonUp(nFlags, point);
}

template<typename EffectType,typename prop,typename stateArray>
void _BuildEffect(EffectType *pEftType,prop myProp,stateArray myStates)
{
	pEftType->BeginBuild(myProp);
	for(size_t i=0;i<myStates.size();i++)
	{
		pEftType->AddKeyState(myStates[i]);
	}
	pEftType->EndBuild();
}

void CEffectPropDlg::OnBnClickedApplyedit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateStateProp(_tstoi(m_szStateIndex),true);//�洢��ǰ����ҳ������
	if(!m_pEditEffect->IS_KIND_OF(EffectPathCtrl))
		UpdateMtlProp(true);//�洢��������

	if(!(m_pEditEffect->IS_STATIC_CLASS(EffectTarget))
		&& !(m_pEditEffect->IS_STATIC_CLASS(EffectExplosive))
		&& !(m_pEditEffect->IS_STATIC_CLASS(EffectBladeFlare)))
	{
		if(m_numStates<=1)
		{
			AfxMessageBox(_T("�����������һ��״̬"));
			return;
		}
	}

	if(m_pEditEffect->IS_STATIC_CLASS(EffectQuad))
		_BuildEffect((EffectQuad *)m_pEditEffect,m_quadProp,m_quadStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectCylinder))
		_BuildEffect((EffectCylinder *)m_pEditEffect,m_cyProp,m_cyStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectGrid))
		_BuildEffect((EffectGrid *)m_pEditEffect,m_gridProp,m_gridStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectBeam))
		_BuildEffect((EffectBeam *)m_pEditEffect,m_beamProp,m_beamStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectSceneDecal))
		_BuildEffect((EffectSceneDecal *)m_pEditEffect,m_scenedecalProp,m_scenedecalStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectMotion))
		_BuildEffect((EffectMotion *)m_pEditEffect,m_mtProp,m_mtStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectParticleEmitter))
		_BuildEffect((EffectParticleEmitter *)m_pEditEffect,m_parProp,m_parStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectMsg))
		_BuildEffect((EffectMsg *)m_pEditEffect,m_msgProp,m_msgStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectMesh))
		_BuildEffect((EffectMesh *)m_pEditEffect,m_meshProp,m_meshStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectSound))
		_BuildEffect((EffectSound *)m_pEditEffect,m_soundProp,m_soundStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectCross))
		_BuildEffect((EffectCross *)m_pEditEffect,m_crossProp,m_crossStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectSphere))
		_BuildEffect((EffectSphere *)m_pEditEffect,m_sphereProp,m_sphereStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectExplosive))
	{
		EffectExplosive *pExp=(EffectExplosive*)m_pEditEffect;
		pExp->Build(m_expProp);
	}
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectBladeFlare))
	{
		EffectBladeFlare *pBladeFlare=(EffectBladeFlare*)m_pEditEffect;
		pBladeFlare->Build(m_bladeFlareProp);
	}
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectTarget))
	{
		EffectTarget *pTargetPath=(EffectTarget*)m_pEditEffect;
		pTargetPath->SetProperty(m_targetProp);
	}
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectCamera))
		_BuildEffect((EffectCamera *)m_pEditEffect,m_cameraProp,m_cameraStates);

	m_pEditEffect->CreateMtl(m_mtlDesc);

	m_preView.UpdateEffect();//Effect�ؽ���,����֪ͨpre view

	SGEffectNode *pNode=m_preView.GetEffectNode();
	if(pNode==NULL)
		return;
	pNode->Play();
	GetDlgItem(IDC_Play)->SetWindowText(_T("ֹͣ"));
}

void CEffectPropDlg::UpdateMtlProp(bool bSave)
{
	if(!bSave)//������Ǵ�����,�򴴽�List
		m_mtlProp.ResetContent();

	int pi=0;
	m_mtlProp.UpdateVal(_T("��ͼ�ļ�"),m_mtlDesc.szTexName,sizeof(m_mtlDesc.szTexName),pi++,bSave,true);
	m_mtlProp.UpdateVal(_T("���ģʽ"),m_mtlDesc.blendType,pi++,bSave);
	m_mtlProp.UpdateVal(_T("Zֵ����"),m_mtlDesc.bTestZ,pi++,bSave);
	m_mtlProp.UpdateVal(_T("��������֡��"),m_mtlDesc.frmU,pi++,bSave);
	m_mtlProp.UpdateVal(_T("��������֡��"),m_mtlDesc.frmV,pi++,bSave);
	m_mtlProp.UpdateVal(_T("���������ٶ�"),m_mtlDesc.fps,pi++,bSave);
	m_mtlProp.UpdateVal(_T("��������һ��"),m_mtlDesc.bPlayOnce,pi++,bSave);

	m_mtlProp.UpdateVal(_T("��������ٶ�"),m_mtlDesc.fScrollSpeedU,pi++,bSave);
	m_mtlProp.UpdateVal(_T("��������ٶ�"),m_mtlDesc.fScrollSpeedV,pi++,bSave);
	m_mtlProp.UpdateVal(_T("�Ƿ��ȴ���"),m_mtlDesc.bHeatHaze,pi++,bSave);
	m_mtlProp.UpdateVal(_T("�ȴ����Ŷ�ϵ��"),m_mtlDesc.fHeatOffsetFator,pi++,bSave);
	m_mtlProp.UpdateVal(_T("�Ƿ��ܹ���Ӱ��"),m_mtlDesc.bEnableLight,pi++,bSave);
}


int CEffectPropDlg::UpdateBaseProp(EffectBase::tagProp *pProp,int stateIndex,bool bSave)
{
	ASSERT(stateIndex==0);
	CPropertyItem *pItem=NULL;
	CString szTmp;
	int pi=0;

	m_stateProp.UpdateVal(_T("ϸ�ڵȼ�"),pProp->m_nLOD,pi++,bSave);
	m_stateProp.UpdateVal(_T("����"),pProp->m_szName,sizeof(m_quadProp.m_szName),pi++,bSave,false);
	m_stateProp.UpdateVal(_T("����"),pProp->m_bActive,pi++,bSave);
	m_stateProp.UpdateVal(_T("����ʱ��"),pProp->m_startTime,pi++,bSave);
	m_stateProp.UpdateVal(_T("ѭ������"),pProp->m_loop,pi++,bSave);
	m_stateProp.UpdateVal(_T("�رչ��̳���"),pProp->m_closeTime,pi++,bSave);

	return pi;
}

void CEffectPropDlg::UpdateBeamState(int stateIndex,bool bSave)
{
	if(stateIndex==0)//0�Ļ���ʾtagProp
	{
		CPropertyItem *pItem=NULL;
		CString szTmp;
		int pi=UpdateBaseProp(&m_beamProp,stateIndex,bSave);

		m_stateProp.UpdateVal(_T("��·��"),m_beamProp.szPathName,sizeof(m_beamProp.szPathName),pi++,bSave,false);
		m_stateProp.UpdateVal(_T("�ָ����"),m_beamProp.div,pi++,bSave);
		m_stateProp.UpdateVal(_T("����ʱ����"),m_beamProp.sampleTime,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ͼ����V����"),m_beamProp.divVScale,pi++,bSave);
	}
	else
	{
		int i=stateIndex-1;
		ASSERT((i>=0));
		if(i>=(int)m_beamStates.size())//�Ƿ�Խ��
		{
			return;
		}
		EffectBeam::tagState *pState=&m_beamStates[i];
		int pi=0;
		
		m_stateProp.UpdateVal(_T("ʱ��"),pState->lastTime,pi++,bSave);
		m_stateProp.UpdateVal(_T("���"),pState->width,pi++,bSave);
		m_stateProp.UpdateVal(_T("�������"),pState->lenWidthInc,pi++,bSave);
		
		m_stateProp.UpdateVal(_T("ͷ����ɫ"),pState->headColor,pi++,bSave);
		m_stateProp.UpdateVal(_T("ͷ��͸����"),pState->headColor.A,pi++,bSave);
		
		m_stateProp.UpdateVal(_T("β����ɫ"),pState->endColor,pi++,bSave);
		m_stateProp.UpdateVal(_T("β��͸����"),pState->endColor.A,pi++,bSave);
	}
}

void CEffectPropDlg::UpdateSceneDecalState(int stateIndex,bool bSave)
{
	if(stateIndex==0)//0�Ļ���ʾtagProp
	{
		CPropertyItem *pItem=NULL;
		CString szTmp;
		int pi=UpdateBaseProp(&m_scenedecalProp,stateIndex,bSave);
		m_stateProp.UpdateVal(_T("��·��"),m_scenedecalProp.szPathName,sizeof(m_scenedecalProp.szPathName),pi++,bSave,false);
		m_stateProp.UpdateVal(_T("λ��x"),m_scenedecalProp.offset.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("λ��y"),m_scenedecalProp.offset.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("λ��z"),m_scenedecalProp.offset.z,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��ת"),m_scenedecalProp.initRot,pi++,bSave);
		//m_stateProp.UpdateVal(_T("�Ƿ�ʹ��Ĭ����Ƭ"),m_scenedecalProp.bUseDefaultMesh,pi++,bSave);
		//m_stateProp.UpdateVal(_T("�Ƿ�ֻʹ��Ĭ����Ƭ"),m_scenedecalProp.bOnlyUseDefaultMesh,pi++,bSave);
		m_stateProp.UpdateVal(_T("������Χ��ǰ����"),m_scenedecalProp.rangeFront,pi++,bSave);
		m_stateProp.UpdateVal(_T("������Χ���󷽣�"),m_scenedecalProp.rangeBack,pi++,bSave);
	}
	else
	{
		int i=stateIndex-1;
		ASSERT((i>=0));
		if(i>=(int)m_scenedecalStates.size())//�Ƿ�Խ��
		{
			return;
		}
		EffectSceneDecal::tagState *pState=&m_scenedecalStates[i];
		int pi=0;

		m_stateProp.UpdateVal(_T("ʱ��"),pState->lastTime,pi++,bSave);

		m_stateProp.UpdateVal(_T("��ɫ"),pState->color,pi++,bSave);
		m_stateProp.UpdateVal(_T("͸����"),pState->color.A,pi++,bSave);

		m_stateProp.UpdateVal(_T("��Сx"),pState->size.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("��Сy"),pState->size.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ת�ٶ�"),pState->rotVel,pi++,bSave);
	}
}

void CEffectPropDlg::UpdateGridState(int stateIndex,bool bSave)
{
	if(stateIndex==0)//0�Ļ���ʾtagProp
	{

		CPropertyItem *pItem=NULL;
		CString szTmp;
		int pi=UpdateBaseProp(&m_gridProp,stateIndex,bSave);

		m_stateProp.UpdateVal(_T("��·��"),m_gridProp.szPathName,sizeof(m_gridProp.szPathName),pi++,bSave,false);
		m_stateProp.UpdateVal(_T("x�ָ����"),m_gridProp.xDiv,pi++,bSave);
		m_stateProp.UpdateVal(_T("y�ָ����"),m_gridProp.yDiv,pi++,bSave);

		m_stateProp.UpdateVal(_T("λ��x"),m_gridProp.offset.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("λ��y"),m_gridProp.offset.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("λ��z"),m_gridProp.offset.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("��ʼ��תx"),m_gridProp.initRot.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��תy"),m_gridProp.initRot.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��תz"),m_gridProp.initRot.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("��������"),m_gridProp.faceType,pi++,bSave);
	}
	else
	{
		int i=stateIndex-1;
		ASSERT((i>=0));
		if(i>=(int)m_gridStates.size())//�Ƿ�Խ��
		{
			return;
		}
		EffectGrid::tagState *pState=&m_gridStates[i];
		int pi=0;
		
		m_stateProp.UpdateVal(_T("ʱ��"),pState->lastTime,pi++,bSave);
		
		m_stateProp.UpdateVal(_T("��ɫ"),pState->color,pi++,bSave);
		m_stateProp.UpdateVal(_T("͸����"),pState->color.A,pi++,bSave);

		m_stateProp.UpdateVal(_T("��ͼu�ظ�����"),pState->uvScale.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ͼv�ظ�����"),pState->uvScale.y,pi++,bSave);

		m_stateProp.UpdateVal(_T("x��С"),pState->size.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("y��С"),pState->size.y,pi++,bSave);

		m_stateProp.UpdateVal(_T("x��ת�ٶ�"),pState->rotVel.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("y��ת�ٶ�"),pState->rotVel.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("z��ת�ٶ�"),pState->rotVel.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("����"),pState->waveSpeed,pi++,bSave);
		m_stateProp.UpdateVal(_T("x���"),pState->wavePower.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("y���"),pState->wavePower.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("z���"),pState->wavePower.z,pi++,bSave);
	}
}

void CEffectPropDlg::UpdateCylinderState(int stateIndex,bool bSave)
{
	if(stateIndex==0)//0�Ļ���ʾtagProp
	{

		CPropertyItem *pItem=NULL;
		CString szTmp;
		int pi=UpdateBaseProp(&m_cyProp,stateIndex,bSave);

		m_stateProp.UpdateVal(_T("��·��"),m_cyProp.szPathName,sizeof(m_cyProp.szPathName),pi++,bSave,false);
		m_stateProp.UpdateVal(_T("�ָ����"),m_cyProp.div,pi++,bSave);
		m_stateProp.UpdateVal(_T("λ��x"),m_cyProp.offset.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("λ��y"),m_cyProp.offset.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("λ��z"),m_cyProp.offset.z,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ͼ�ظ�����"),m_cyProp.uScale,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��תx"),m_cyProp.initRot.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��תy"),m_cyProp.initRot.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��תz"),m_cyProp.initRot.z,pi++,bSave);
	}
	else
	{	
		int i=stateIndex-1;
		ASSERT((i>=0));
		if(i>=(int)m_cyStates.size())//�Ƿ�Խ��
		{
			return;
		}
		EffectCylinder::tagState *pState=&m_cyStates[i];
		int pi=0;
		
		m_stateProp.UpdateVal(_T("ʱ��"),pState->lastTime,pi++,bSave);
		m_stateProp.UpdateVal(_T("Ȧ��"),pState->numCircle,pi++,bSave);
		m_stateProp.UpdateVal(_T("�߶�"),pState->high,pi++,bSave);
		m_stateProp.UpdateVal(_T("���뾶"),pState->topRadius,pi++,bSave);
		m_stateProp.UpdateVal(_T("�װ뾶"),pState->bottomRadius,pi++,bSave);
		m_stateProp.UpdateVal(_T("�ָ�ʱ���뾶����"),pState->divTopRadiusInc,pi++,bSave);
		m_stateProp.UpdateVal(_T("�ָ�ʱ�װ뾶����"),pState->divBottomRadiusInc,pi++,bSave);

		m_stateProp.UpdateVal(_T("������"),pState->topShrink,pi++,bSave);
		m_stateProp.UpdateVal(_T("������"),pState->bottomShrink,pi++,bSave);
		m_stateProp.UpdateVal(_T("�ָ�ʱ������"),pState->divTopRise,pi++,bSave);
		m_stateProp.UpdateVal(_T("�ָ�ʱ������"),pState->divBottomRise,pi++,bSave);

		m_stateProp.UpdateVal(_T("����ɫ"),pState->topColor,pi++,bSave);
		m_stateProp.UpdateVal(_T("��͸����"),pState->topColor.A,pi++,bSave);
		m_stateProp.UpdateVal(_T("����ɫ"),pState->bottomColor,pi++,bSave);
		m_stateProp.UpdateVal(_T("��͸����"),pState->bottomColor.A,pi++,bSave);

		m_stateProp.UpdateVal(_T("x��ת�ٶ�"),pState->rotVel.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("y��ת�ٶ�"),pState->rotVel.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("z��ת�ٶ�"),pState->rotVel.z,pi++,bSave);
	}

}

void CEffectPropDlg::UpdateMsgState(int stateIndex,bool bSave)
{
	if(stateIndex==0)
	{
		int pi=UpdateBaseProp(&m_msgProp,stateIndex,bSave);
	}
	else
	{
		int i=stateIndex-1;
		ASSERT((i>=0));
		if(i>=(int)m_msgStates.size())//�Ƿ�Խ��
		{
			return;
		}
		EffectMsg::tagState *pState=&m_msgStates[i];

		int pi=0;
		m_stateProp.UpdateVal(_T("ʱ��"),pState->msgTime,pi++,bSave);
		m_stateProp.UpdateVal(_T("��Ϣ����"),pState->szMsg,sizeof(pState->szMsg),pi++,bSave,false);
		m_stateProp.UpdateVal(_T("��Ϣ����1"),pState->szMsgParam1,sizeof(pState->szMsgParam1),pi++,bSave,false);
		m_stateProp.UpdateVal(_T("��Ϣ����2"),pState->szMsgParam2,sizeof(pState->szMsgParam2),pi++,bSave,false);
	}
}

void CEffectPropDlg::UpdateMotionState(int stateIndex,bool bSave)
{
	if(stateIndex==0)//0�Ļ���ʾtagProp
	{

		CPropertyItem *pItem=NULL;
		CString szTmp;
		int pi=UpdateBaseProp(&m_mtProp,stateIndex,bSave);
		m_stateProp.UpdateVal(_T("��ʼλ��x"),m_mtProp.initPos.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼλ��y"),m_mtProp.initPos.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼλ��z"),m_mtProp.initPos.z,pi++,bSave);
		m_stateProp.UpdateVal(_T("�Ƿ���㳯��"),m_mtProp.bCalcRot,pi++,bSave);
	}
	else
	{	
		int i=stateIndex-1;
		ASSERT((i>=0));
		if(i>=(int)m_mtStates.size())//�Ƿ�Խ��
		{
			return;
		}
		EffectMotion::tagState *pState=&m_mtStates[i];

		int pi=0;
		m_stateProp.UpdateVal(_T("ʱ��"),pState->lastTime,pi++,bSave);
		
		m_stateProp.UpdateVal(_T("�ƶ��ٶ�x"),pState->vel.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("�ƶ��ٶ�y"),pState->vel.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("�ƶ��ٶ�z"),pState->vel.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("��ת�ٶ�x"),pState->revVel.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ת�ٶ�y"),pState->revVel.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ת�ٶ�z"),pState->revVel.z,pi++,bSave);
	}
}

void CEffectPropDlg::UpdateSphereState(int stateIndex,bool bSave)
{
	if(stateIndex==0)//0�Ļ���ʾtagProp
	{

		CPropertyItem *pItem=NULL;
		CString szTmp;
		int pi=UpdateBaseProp(&m_sphereProp,stateIndex,bSave);


		m_stateProp.UpdateVal(_T("��·��"),m_sphereProp.szPathName,sizeof(m_sphereProp.szPathName),pi++,bSave,false);
		m_stateProp.UpdateVal(_T("λ��x"),m_sphereProp.offset.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("λ��y"),m_sphereProp.offset.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("λ��z"),m_sphereProp.offset.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("��ʼ��תx"),m_sphereProp.initRot.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��תy"),m_sphereProp.initRot.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��תz"),m_sphereProp.initRot.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("�뾶"),m_sphereProp.radius,pi++,bSave);
		m_stateProp.UpdateVal(_T("pitch��Χmin"),m_sphereProp.pitchRange.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("pitch��Χmax"),m_sphereProp.pitchRange.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("yaw��Χmin"),m_sphereProp.yawRange.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("yaw��Χmax"),m_sphereProp.yawRange.y,pi++,bSave);

		m_stateProp.UpdateVal(_T("�ָ����"),m_sphereProp.slices,pi++,bSave);

		m_stateProp.UpdateVal(_T("��ͼu�ظ�����"),m_sphereProp.uvScale.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ͼv�ظ�����"),m_sphereProp.uvScale.y,pi++,bSave);
	}
	else
	{
		int i=stateIndex-1;
		ASSERT((i>=0));
		if(i>=(int)m_sphereStates.size())//�Ƿ�Խ��
		{
			return;
		}
		EffectSphere::tagState *pState=&m_sphereStates[i];

		int pi=0;
		m_stateProp.UpdateVal(_T("ʱ��"),pState->lastTime,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ɫ"),pState->color,pi++,bSave);
		m_stateProp.UpdateVal(_T("͸����"),pState->color.A,pi++,bSave);

		m_stateProp.UpdateVal(_T("x��ת�ٶ�"),pState->rotVel.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("y��ת�ٶ�"),pState->rotVel.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("z��ת�ٶ�"),pState->rotVel.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("x����"),pState->sacle.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("y����"),pState->sacle.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("z����"),pState->sacle.z,pi++,bSave);
	}
}

void CEffectPropDlg::UpdateCrossState(int stateIndex,bool bSave)
{
	if(stateIndex==0)//0�Ļ���ʾtagProp
	{

		CPropertyItem *pItem=NULL;
		CString szTmp;
		int pi=UpdateBaseProp(&m_crossProp,stateIndex,bSave);


		m_stateProp.UpdateVal(_T("��·��"),m_crossProp.szPathName,sizeof(m_crossProp.szPathName),pi++,bSave,false);
		m_stateProp.UpdateVal(_T("λ��x"),m_crossProp.offset.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("λ��y"),m_crossProp.offset.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("λ��z"),m_crossProp.offset.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("��ʼ��תx"),m_crossProp.initRot.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��תy"),m_crossProp.initRot.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��תz"),m_crossProp.initRot.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("�����г���"),m_crossProp.numFace,pi++,bSave);
		m_stateProp.UpdateVal(_T("����������"),m_crossProp.bHFace,pi++,bSave);
	}
	else
	{	
		int i=stateIndex-1;
		ASSERT((i>=0));
		if(i>=(int)m_crossStates.size())//�Ƿ�Խ��
		{
			return;
		}
		EffectCross::tagState *pState=&m_crossStates[i];

		int pi=0;
		m_stateProp.UpdateVal(_T("ʱ��"),pState->lastTime,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ɫ"),pState->color,pi++,bSave);
		m_stateProp.UpdateVal(_T("͸����"),pState->color.A,pi++,bSave);

		m_stateProp.UpdateVal(_T("x��С"),pState->size.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("y��С"),pState->size.y,pi++,bSave);

		m_stateProp.UpdateVal(_T("x��ת�ٶ�"),pState->rotVel.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("y��ת�ٶ�"),pState->rotVel.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("z��ת�ٶ�"),pState->rotVel.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("����"),pState->faceDist,pi++,bSave);
		m_stateProp.UpdateVal(_T("��������"),pState->faceDistInc,pi++,bSave);
		m_stateProp.UpdateVal(_T("x��С����"),pState->faceSizeInc.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("y��С����"),pState->faceSizeInc.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("��͸���Ƚ���"),pState->faceAlphaDec,pi++,bSave);
	}
}

void CEffectPropDlg::UpdateQuadState(int stateIndex,bool bSave)
{
	if(stateIndex==0)//0�Ļ���ʾtagProp
	{

		CPropertyItem *pItem=NULL;
		CString szTmp;
		int pi=UpdateBaseProp(&m_quadProp,stateIndex,bSave);


		m_stateProp.UpdateVal(_T("��·��"),m_quadProp.szPathName,sizeof(m_quadProp.szPathName),pi++,bSave,false);
		m_stateProp.UpdateVal(_T("λ��x"),m_quadProp.offset.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("λ��y"),m_quadProp.offset.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("λ��z"),m_quadProp.offset.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("��ʼ��תx"),m_quadProp.initRot.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��תy"),m_quadProp.initRot.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��תz"),m_quadProp.initRot.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("��Ƭ����"),m_quadProp.faceType,pi++,bSave);
		m_stateProp.UpdateVal(_T("��Ӱ����"),m_quadProp.tailSize,pi++,bSave);
		m_stateProp.UpdateVal(_T("��Ӱ���ʱ��"),m_quadProp.tailInterval,pi++,bSave);
		m_stateProp.UpdateVal(_T("��Ӱ����ʱ��"),m_quadProp.tailLife,pi++,bSave);
	}
	else
	{	
		int i=stateIndex-1;
		ASSERT((i>=0));
		if(i>=(int)m_quadStates.size())//�Ƿ�Խ��
		{
			return;
		}
		EffectQuad::tagState *pState=&m_quadStates[i];

		int pi=0;
		m_stateProp.UpdateVal(_T("ʱ��"),pState->lastTime,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ɫ"),pState->color,pi++,bSave);
		m_stateProp.UpdateVal(_T("͸����"),pState->color.A,pi++,bSave);

		m_stateProp.UpdateVal(_T("x��С"),pState->size.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("y��С"),pState->size.y,pi++,bSave);

		m_stateProp.UpdateVal(_T("x��ת�ٶ�"),pState->rotVel.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("y��ת�ٶ�"),pState->rotVel.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("z��ת�ٶ�"),pState->rotVel.z,pi++,bSave);
	}
}

void CEffectPropDlg::UpdateParticleState(int stateIndex,bool bSave)
{
	if(stateIndex==0)//0�Ļ���ʾtagProp
	{

		CPropertyItem *pItem=NULL;
		CString szTmp;
		int pi=UpdateBaseProp(&m_parProp,stateIndex,bSave);

		m_stateProp.UpdateVal(_T("��·��"),m_parProp.szPathName,sizeof(m_parProp.szPathName),pi++,bSave,false);
		m_stateProp.UpdateVal(_T("��״"),m_parProp.shape,pi++,bSave);
		m_stateProp.UpdateVal(_T("���������"),m_parProp.maxParticles,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��������"),m_parProp.initParNum,pi++,bSave);

		m_stateProp.UpdateVal(_T("λ��x"),m_parProp.pathOffset.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("λ��y"),m_parProp.pathOffset.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("λ��z"),m_parProp.pathOffset.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("�����ٶȷ�ʽ"),m_parProp.velType,pi++,bSave);
		m_stateProp.UpdateVal(_T("���Ӷ��������"),m_parProp.polygonType,pi++,bSave);
		m_stateProp.UpdateVal(_T("����ϵ��"),m_parProp.gForce,pi++,bSave);
		m_stateProp.UpdateVal(_T("��������ϵ��"),m_parProp.airForce,pi++,bSave);

		m_stateProp.UpdateVal(_T("��ʼ��תx"),m_parProp.initRot.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��תy"),m_parProp.initRot.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��תz"),m_parProp.initRot.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("�����Զ�����"),m_parProp.bParAutoFade,pi++,bSave);
		m_stateProp.UpdateVal(_T("���ӷ��䵽������"),m_parProp.bToWorld,pi++,bSave);
	}
	else
	{	
		int i=stateIndex-1;
		ASSERT((i>=0));
		if(i>=(int)m_parStates.size())//�Ƿ�Խ��
		{
			return;
		}
		EffectParticleEmitter::tagState *pState=&m_parStates[i];

		int pi=0;
		m_stateProp.UpdateVal(_T("ʱ��"),pState->lastTime,pi++,bSave);
		m_stateProp.UpdateVal(_T("��״����1"),pState->shapeParam.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("��״����2"),pState->shapeParam.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("��״����3"),pState->shapeParam.z,pi++,bSave);
		m_stateProp.UpdateVal(_T("��״����4"),pState->shapeParam.w,pi++,bSave);

		m_stateProp.UpdateVal(_T("x��ת�ٶ�"),pState->rotVel.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("y��ת�ٶ�"),pState->rotVel.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("z��ת�ٶ�"),pState->rotVel.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("���Ӳ������ٶ�"),pState->emitSpeed,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.��������Max"),pState->parLifeTimeMax,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.��������Min"),pState->parLifeTimeMin,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.�ٶȷ���x"),pState->parVelDir.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.�ٶȷ���y"),pState->parVelDir.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.�ٶȷ���z"),pState->parVelDir.z,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.����Max"),pState->parSpeedMax,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.����Min"),pState->parSpeedMin,pi++,bSave);

		m_stateProp.UpdateVal(_T("����.�ڶ�Max"),pState->parSwingMax,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.�ڶ�Min"),pState->parSwingMin,pi++,bSave);

		m_stateProp.UpdateVal(_T("����.��תMax.x"),pState->parRotSpeedMax.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.��תMax.y"),pState->parRotSpeedMax.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.��תMax.z"),pState->parRotSpeedMax.z,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.��תMin.x"),pState->parRotSpeedMin.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.��תMin.y"),pState->parRotSpeedMin.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.��תMin.z"),pState->parRotSpeedMin.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("����.��СMax"),pState->parSizeMax,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.��СMin"),pState->parSizeMin,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.��С�仯�ٶ�"),pState->parSizeVel,pi++,bSave);

		m_stateProp.UpdateVal(_T("����.��ɫ"),pState->parColor,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.͸����"),pState->parColor.A,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.����ٶ�"),pState->parColorVel.R,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.�����ٶ�"),pState->parColorVel.G,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.�����ٶ�"),pState->parColorVel.B,pi++,bSave);
		m_stateProp.UpdateVal(_T("����.��͸���ٶ�"),pState->parColorVel.A,pi++,bSave);
	}
}

void CEffectPropDlg::UpdateSoundState(int stateIndex,bool bSave)
{
	if(stateIndex==0)//0�Ļ���ʾtagProp
	{

		CPropertyItem *pItem=NULL;
		CString szTmp;
		int pi=UpdateBaseProp(&m_soundProp,stateIndex,bSave);

		m_stateProp.UpdateVal(_T("ʹ��3D��Ч"),m_soundProp.b3DSound,pi++,bSave);
		m_stateProp.UpdateVal(_T("��·��"),m_soundProp.szPathName,sizeof(m_soundProp.szPathName),pi++,bSave,false);
		m_stateProp.UpdateVal(_T("ʱ����"),m_soundProp.timeLoop,pi++,bSave);
	}
	else
	{
		int i=stateIndex-1;
		ASSERT((i>=0));
		if(i>=(int)m_soundStates.size())//�Ƿ�Խ��
		{
			return;
		}
		EffectSound::tagState *pState=&m_soundStates[i];

		int pi=0;
		m_stateProp.UpdateVal(_T("ʱ��"),pState->soundTime,pi++,bSave);
		m_stateProp.UpdateVal(_T("ѭ������"),pState->bLoop,pi++,bSave);
		m_stateProp.UpdateVal(_T("�����ļ�"),pState->szSoundPath,sizeof(pState->szSoundPath),pi++,bSave,true);
		m_stateProp.UpdateVal(_T("��С����"),pState->minDist,pi++,bSave);
		m_stateProp.UpdateVal(_T("������"),pState->maxDist,pi++,bSave);
		m_stateProp.UpdateVal(_T("����"),pState->volume,pi++,bSave);
	}
}

void CEffectPropDlg::UpdateMeshState(int stateIndex,bool bSave)
{
	if(stateIndex==0)//0�Ļ���ʾtagProp
	{

		CPropertyItem *pItem=NULL;
		CString szTmp;
		int pi=UpdateBaseProp(&m_meshProp,stateIndex,bSave);

		m_stateProp.UpdateVal(_T("ģ���ļ�"),m_meshProp.szMeshName,sizeof(m_meshProp.szMeshName),pi++,bSave,true);
		m_stateProp.UpdateVal(_T("��·��"),m_meshProp.szPathName,sizeof(m_meshProp.szPathName),pi++,bSave,false);

		m_stateProp.UpdateVal(_T("��ʼλ��x"),m_meshProp.offset.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼλ��y"),m_meshProp.offset.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼλ��z"),m_meshProp.offset.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("��ʼ��תx"),m_meshProp.initRot.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��תy"),m_meshProp.initRot.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ʼ��תz"),m_meshProp.initRot.z,pi++,bSave);
	}
	else
	{
		int i=stateIndex-1;
		ASSERT((i>=0));
		if(i>=(int)m_meshStates.size())//�Ƿ�Խ��
		{
			return;
		}
		EffectMesh::tagState *pState=&m_meshStates[i];

		int pi=0;
		m_stateProp.UpdateVal(_T("ʱ��"),pState->lastTime,pi++,bSave);
		m_stateProp.UpdateVal(_T("͸����"),pState->alpha,pi++,bSave);

		m_stateProp.UpdateVal(_T("����x"),pState->scale.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("����y"),pState->scale.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("����z"),pState->scale.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("��ת�ٶ�x"),pState->rotVel.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ת�ٶ�y"),pState->rotVel.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("��ת�ٶ�z"),pState->rotVel.z,pi++,bSave);

		m_stateProp.UpdateVal(_T("�ƶ��ٶ�x"),pState->vel.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("�ƶ��ٶ�y"),pState->vel.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("�ƶ��ٶ�z"),pState->vel.z,pi++,bSave);
	}
}

void CEffectPropDlg::UpdateExpState(int stateIndex,bool bSave)
{
	if(stateIndex!=0)//ֻ��Prop,û������state
		return;

	int pi=UpdateBaseProp(&m_expProp,stateIndex,bSave);
	
	m_stateProp.UpdateVal(_T("��·��"),m_expProp.szPathName,sizeof(m_expProp.szPathName),pi++,bSave,false);
	m_stateProp.UpdateVal(_T("λ��x"),m_expProp.offset.x,pi++,bSave);
	m_stateProp.UpdateVal(_T("λ��y"),m_expProp.offset.y,pi++,bSave);
	m_stateProp.UpdateVal(_T("λ��z"),m_expProp.offset.z,pi++,bSave);

	m_stateProp.UpdateVal(_T("��תx"),m_expProp.rot.x,pi++,bSave);
	m_stateProp.UpdateVal(_T("��תy"),m_expProp.rot.y,pi++,bSave);
	m_stateProp.UpdateVal(_T("��תz"),m_expProp.rot.z,pi++,bSave);

	m_stateProp.UpdateVal(_T("��Ƭ����"),m_expProp.faceType,pi++,bSave);
	m_stateProp.UpdateVal(_T("��������"),m_expProp.bodyType,pi++,bSave);

	m_stateProp.UpdateVal(_T("���β���1"),m_expProp.bodyParam.x,pi++,bSave);
	m_stateProp.UpdateVal(_T("���β���2"),m_expProp.bodyParam.y,pi++,bSave);
	m_stateProp.UpdateVal(_T("���β���3"),m_expProp.bodyParam.z,pi++,bSave);
	m_stateProp.UpdateVal(_T("���β���4"),m_expProp.bodyParam.w,pi++,bSave);

	m_stateProp.UpdateVal(_T("�����з�"),m_expProp.xDiv,pi++,bSave);
	m_stateProp.UpdateVal(_T("�����з�"),m_expProp.yDiv,pi++,bSave);

	m_stateProp.UpdateVal(_T("��ը˥��ϵ��"),m_expProp.powerDec,pi++,bSave);
	m_stateProp.UpdateVal(_T("��ը����Max"),m_expProp.maxPower,pi++,bSave);
	m_stateProp.UpdateVal(_T("��ը����Min"),m_expProp.minPower,pi++,bSave);

	m_stateProp.UpdateVal(_T("��������ϵ��"),m_expProp.airForce,pi++,bSave);
	m_stateProp.UpdateVal(_T("����ϵ��"),m_expProp.gForce,pi++,bSave);

	m_stateProp.UpdateVal(_T("��ը����x"),m_expProp.dir.x,pi++,bSave);
	m_stateProp.UpdateVal(_T("��ը����y"),m_expProp.dir.y,pi++,bSave);
	m_stateProp.UpdateVal(_T("��ը����z"),m_expProp.dir.z,pi++,bSave);

	m_stateProp.UpdateVal(_T("��ͼu�ظ�����"),m_expProp.uvScale.x,pi++,bSave);
	m_stateProp.UpdateVal(_T("��ͼv�ظ�����"),m_expProp.uvScale.y,pi++,bSave);

	m_stateProp.UpdateVal(_T("����ʱ��Max"),m_expProp.maxLifeTime,pi++,bSave);
	m_stateProp.UpdateVal(_T("����ʱ��Min"),m_expProp.minLifeTime,pi++,bSave);

}

void CEffectPropDlg::UpdateBladeFlareState(int stateIndex,bool bSave)
{
	if(stateIndex!=0)//ֻ��Prop,û������state
		return;

	int pi=0;

	m_stateProp.UpdateVal(_T("����"),m_bladeFlareProp.m_szName,sizeof(m_bladeFlareProp.m_szName),pi++,bSave,false);
	m_stateProp.UpdateVal(_T("����ʱ��"),m_bladeFlareProp.trackTimeLen,pi++,bSave);
	m_stateProp.UpdateVal(_T("�������"),m_bladeFlareProp.sampleTime,pi++,bSave);
	m_stateProp.UpdateVal(_T("��ɫ"),m_bladeFlareProp.color,pi++,bSave);
	m_stateProp.UpdateVal(_T("͸����"),m_bladeFlareProp.color.A,pi++,bSave);
	m_stateProp.UpdateVal(_T("���"),m_bladeFlareProp.nIndex,pi++,bSave);
	if( m_bladeFlareProp.nIndex > 9 || m_bladeFlareProp.nIndex < 0 )
		AfxMessageBox( _T("�����ű�����0-9֮��") );

	m_stateProp.UpdateVal(_T("��ƽ������"),m_bladeFlareProp.bNoSmooth,pi++,bSave);
	m_stateProp.UpdateVal(_T("�Զ�����ص�"),m_bladeFlareProp.bCustomTagNode,pi++,bSave);
	m_stateProp.UpdateVal(_T("���ص�����1"),m_bladeFlareProp.szTagNode[0],sizeof(m_bladeFlareProp.szTagNode[0]),pi++,bSave,false);
	m_stateProp.UpdateVal(_T("���ص�����2"),m_bladeFlareProp.szTagNode[1],sizeof(m_bladeFlareProp.szTagNode[0]),pi++,bSave,false);
}

void CEffectPropDlg::UpdateTargetState(int stateIndex,bool bSave)
{
	if(stateIndex!=0)//target ֻ��Prop,û������state
		return;

	CString szTmp;
	int pi=UpdateBaseProp(&m_targetProp,stateIndex,bSave);

	m_stateProp.UpdateVal(_T("�ٶ���������"),m_targetProp.speedType,pi++,bSave);
	m_stateProp.UpdateVal(_T("�˶�ʱ��"),m_targetProp.time,pi++,bSave);
	m_stateProp.UpdateVal(_T("�˶�����(��������ʱ��)"),m_targetProp.speed,pi++,bSave);
	
	m_stateProp.UpdateVal(_T("���x"),m_targetProp.startPt.x,pi++,bSave);
	m_stateProp.UpdateVal(_T("���y"),m_targetProp.startPt.y,pi++,bSave);
	m_stateProp.UpdateVal(_T("���z"),m_targetProp.startPt.z,pi++,bSave);
	
	m_stateProp.UpdateVal(_T("�յ�x"),m_targetProp.endPt.x,pi++,bSave);
	m_stateProp.UpdateVal(_T("�յ�y"),m_targetProp.endPt.y,pi++,bSave);
	m_stateProp.UpdateVal(_T("�յ�z"),m_targetProp.endPt.z,pi++,bSave);

	m_stateProp.UpdateVal(_T("�߶���������"),m_targetProp.upLineType,pi++,bSave);
	m_stateProp.UpdateVal(_T("�ڶ���������"),m_targetProp.sideLineType,pi++,bSave);
	m_stateProp.UpdateVal(_T("���߶�"),m_targetProp.upMax,pi++,bSave);
	m_stateProp.UpdateVal(_T("���ڶ�����"),m_targetProp.sideMax,pi++,bSave);
}

void CEffectPropDlg::UpdateCameraState(int stateIndex,bool bSave)
{
	if(stateIndex==0)//0�Ļ���ʾtagProp
	{

		CPropertyItem *pItem=NULL;
		CString szTmp;
		int pi=UpdateBaseProp(&m_cameraProp,stateIndex,bSave);
		m_stateProp.UpdateVal(_T("˥��ϵ��"),m_cameraProp.attenuationFactor,pi++,bSave);
		m_stateProp.UpdateVal(_T("Ӱ�췶Χ"),m_cameraProp.range,pi++,bSave);
	}
	else
	{	
		int i=stateIndex-1;
		ASSERT((i>=0));
		if(i>=(int)m_cameraStates.size())//�Ƿ�Խ��
		{
			return;
		}
		EffectCamera::tagState *pState=&m_cameraStates[i];

		int pi=0;
		m_stateProp.UpdateVal(_T("ʱ��"),pState->lastTime,pi++,bSave);
		m_stateProp.UpdateVal(_T("���x"),pState->swing.x,pi++,bSave);
		m_stateProp.UpdateVal(_T("���y"),pState->swing.y,pi++,bSave);
		m_stateProp.UpdateVal(_T("���z"),pState->swing.z,pi++,bSave);
	}
}

void CEffectPropDlg::UpdateStateProp(int stateIndex,bool bSave)
{
	if(m_pEditEffect==NULL)
		return;

	if(!bSave)//������Ǵ�����,���Ƿ�ҳ
		m_stateProp.ResetContent();

	if(m_pEditEffect->IS_STATIC_CLASS(EffectQuad))
		UpdateQuadState(stateIndex,bSave);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectCylinder))
		UpdateCylinderState(stateIndex,bSave);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectGrid))
		UpdateGridState(stateIndex,bSave);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectBeam))
		UpdateBeamState(stateIndex,bSave);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectSceneDecal))
		UpdateSceneDecalState(stateIndex,bSave);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectMotion))
		UpdateMotionState(stateIndex,bSave);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectParticleEmitter))
		UpdateParticleState(stateIndex,bSave);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectMsg))
		UpdateMsgState(stateIndex,bSave);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectMesh))
		UpdateMeshState(stateIndex,bSave);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectSound))
		UpdateSoundState(stateIndex,bSave);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectTarget))
		UpdateTargetState(stateIndex,bSave);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectCross))
		UpdateCrossState(stateIndex,bSave);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectSphere))
		UpdateSphereState(stateIndex,bSave);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectExplosive))
		UpdateExpState(stateIndex,bSave);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectBladeFlare))
		UpdateBladeFlareState(stateIndex,bSave);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectCamera))
		UpdateCameraState(stateIndex,bSave);
	else
	{
		ASSERT(0 && _T("Unknow effect type!"));
	}

}

void CEffectPropDlg::OnEnChangeIndex()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateStateProp(_tstoi(m_szStateIndex),true);
	UpdateData();
	UpdateStateProp(_tstoi(m_szStateIndex),false);
}

void CEffectPropDlg::OnBnClickedIndexinc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int n=_tstoi(m_szStateIndex);
	n++;
	if(n>=m_numStates)
		n=m_numStates-1;
	UpdateStateProp(_tstoi(m_szStateIndex),true);
	m_szStateIndex.Format(_T("%d"),n);
	UpdateData(FALSE);
	UpdateStateProp(_tstoi(m_szStateIndex),false);
}

void CEffectPropDlg::OnBnClickedIndexdec()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int n=_tstoi(m_szStateIndex);
	n--;
	if(n<0)
		n=0;
	UpdateStateProp(_tstoi(m_szStateIndex),true);
	m_szStateIndex.Format(_T("%d"),n);
	UpdateData(FALSE);
	UpdateStateProp(_tstoi(m_szStateIndex),false);
}

void CEffectPropDlg::OnBnClickedDelstate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int n=_tstoi(m_szStateIndex);
	if(n==0)
	{
		AfxMessageBox(_T("����ɾ����ʼ״̬"));
		return;
	}

	m_numStates--;
	if(m_pEditEffect->IS_STATIC_CLASS(EffectQuad))
		m_quadStates.erase(m_quadStates.begin()+n-1);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectCylinder))
		m_cyStates.erase(m_cyStates.begin()+n-1);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectGrid))
		m_gridStates.erase(m_gridStates.begin()+n-1);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectBeam))
		m_beamStates.erase(m_beamStates.begin()+n-1);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectSceneDecal))
		m_scenedecalStates.erase(m_scenedecalStates.begin()+n-1);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectMotion))
		m_mtStates.erase(m_mtStates.begin()+n-1);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectParticleEmitter))
		m_parStates.erase(m_parStates.begin()+n-1);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectMsg))
		m_msgStates.erase(m_msgStates.begin()+n-1);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectMesh))
		m_meshStates.erase(m_meshStates.begin()+n-1);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectSound))
		m_soundStates.erase(m_soundStates.begin()+n-1);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectCross))
		m_crossStates.erase(m_crossStates.begin()+n-1);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectSphere))
		m_sphereStates.erase(m_sphereStates.begin()+n-1);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectCamera))
		m_cameraStates.erase(m_cameraStates.begin()+n-1);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectTarget)
		|| m_pEditEffect->IS_STATIC_CLASS(EffectExplosive)
		|| m_pEditEffect->IS_STATIC_CLASS(EffectBladeFlare))
	{
		//do nothing
	}

	//--��ǰ��ҳ
	m_szStateIndex.Format(_T("%d"),n-1);
	UpdateData(FALSE);
	UpdateStateProp(_tstoi(m_szStateIndex),false);	
}

template<typename myState>
int _AddState(vector<myState>& states)
{
	myState newState;
	int s=states.size();
	if(s>0)
		newState=states[s-1];//������һ״̬
	states.push_back(newState);

	return states.size()+1;
}

void CEffectPropDlg::OnBnClickedAddstate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//--���浱ǰҳ
	UpdateStateProp(_tstoi(m_szStateIndex),true);
	
	//-- ���һ����ҳ
	if(m_pEditEffect->IS_STATIC_CLASS(EffectQuad))
		m_numStates=_AddState(m_quadStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectCylinder))
		m_numStates=_AddState(m_cyStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectGrid))
		m_numStates=_AddState(m_gridStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectBeam))
		m_numStates=_AddState(m_beamStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectSceneDecal))
		m_numStates=_AddState(m_scenedecalStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectMotion))
		m_numStates=_AddState(m_mtStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectParticleEmitter))
		m_numStates=_AddState(m_parStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectMsg))
		m_numStates=_AddState(m_msgStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectMesh))
		m_numStates=_AddState(m_meshStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectMesh))
		m_numStates=_AddState(m_meshStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectSound))
		m_numStates=_AddState(m_soundStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectCross))
		m_numStates=_AddState(m_crossStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectSphere))
		m_numStates=_AddState(m_sphereStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectCamera))
		m_numStates=_AddState(m_cameraStates);
	else if(m_pEditEffect->IS_STATIC_CLASS(EffectTarget)
		||m_pEditEffect->IS_STATIC_CLASS(EffectExplosive)
		||m_pEditEffect->IS_STATIC_CLASS(EffectBladeFlare))
	{
		//do nothing
	}
	
	//--�������ҳ(������)
	m_szStateIndex.Format(_T("%d"),m_numStates-1);
	UpdateData(FALSE);
	UpdateStateProp(_tstoi(m_szStateIndex),false);	
}

void CEffectPropDlg::OnBnClickedOpenmdl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	_T("��̬ģ���ļ�|*.fsm;*.fscn||"),
	this,0);
	if(IDOK==dlg.DoModal())
	{
		CString szPath=dlg.GetPathName();
		GetDlgItem(IDC_MdlPath)->SetWindowText(szPath);
		m_preView.LoadMdl(szPath);
	}
}

void CEffectPropDlg::OnBnClickedWireframe()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(IsDlgButtonChecked(IDC_WireFrame))
		Device()->SetFillMode(EFill_Wireframe);
	else
		Device()->SetFillMode(EFill_Solid);
}

template<typename EffectType,typename prop,typename stateArray>
int _CopyEffectData(EffectType *pEftType,prop& myProp,stateArray& myStates)
{
	myProp=pEftType->GetProp();
	myStates.clear();

	for(int i=0;i<pEftType->GetNumStates();i++)
		myStates.push_back(pEftType->GetState(i));

	return myStates.size()+1;
}

void CEffectPropDlg::CopyEftData(EffectBase *pEft)
{
	m_mtlDesc=pEft->GetMtlDesc();

	if(pEft->IS_STATIC_CLASS(EffectQuad))
		m_numStates=_CopyEffectData((EffectQuad *)pEft,m_quadProp,m_quadStates);
	else if(pEft->IS_STATIC_CLASS(EffectCylinder))
		m_numStates=_CopyEffectData((EffectCylinder *)pEft,m_cyProp,m_cyStates);
	else if(pEft->IS_STATIC_CLASS(EffectGrid))
		m_numStates=_CopyEffectData((EffectGrid *)pEft,m_gridProp,m_gridStates);
	else if(pEft->IS_STATIC_CLASS(EffectBeam))
		m_numStates=_CopyEffectData((EffectBeam *)pEft,m_beamProp,m_beamStates);
	else if(pEft->IS_STATIC_CLASS(EffectSceneDecal))
		m_numStates=_CopyEffectData((EffectSceneDecal *)pEft,m_scenedecalProp,m_scenedecalStates);
	else if(pEft->IS_STATIC_CLASS(EffectMotion))
		m_numStates=_CopyEffectData((EffectMotion*)pEft,m_mtProp,m_mtStates);
	else if(pEft->IS_STATIC_CLASS(EffectParticleEmitter))
		m_numStates=_CopyEffectData((EffectParticleEmitter*)pEft,m_parProp,m_parStates);
	else if(pEft->IS_STATIC_CLASS(EffectMsg))
		m_numStates=_CopyEffectData((EffectMsg*)pEft,m_msgProp,m_msgStates);
	else if(pEft->IS_STATIC_CLASS(EffectMesh))
		m_numStates=_CopyEffectData((EffectMesh*)pEft,m_meshProp,m_meshStates);
	else if(pEft->IS_STATIC_CLASS(EffectSound))
		m_numStates=_CopyEffectData((EffectSound*)pEft,m_soundProp,m_soundStates);
	else if(pEft->IS_STATIC_CLASS(EffectCross))
		m_numStates=_CopyEffectData((EffectCross*)pEft,m_crossProp,m_crossStates);
	else if(pEft->IS_STATIC_CLASS(EffectSphere))
		m_numStates=_CopyEffectData((EffectSphere*)pEft,m_sphereProp,m_sphereStates);
	else if(pEft->IS_STATIC_CLASS(EffectCamera))
		m_numStates=_CopyEffectData((EffectCamera*)pEft,m_cameraProp,m_cameraStates);
	else if(pEft->IS_STATIC_CLASS(EffectExplosive))
	{
		m_numStates=1;
		m_expProp=((EffectExplosive*)pEft)->GetProperty();
	}
	else if(pEft->IS_STATIC_CLASS(EffectBladeFlare))
	{
		m_numStates=1;
		m_bladeFlareProp=((EffectBladeFlare*)pEft)->GetProp();
	}
	else if(pEft->IS_STATIC_CLASS(EffectTarget))
	{
		m_numStates=1;
		m_targetProp=((EffectTarget*)pEft)->GetProperty();
	}
}
void CEffectPropDlg::OnBnClickedPlay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SGEffectNode *pNode=m_preView.GetEffectNode();
	if(pNode==NULL)
		return;

	CString szBtnName;
	CWnd *pBtn=GetDlgItem(IDC_Play);
	pBtn->GetWindowText(szBtnName);
	if(szBtnName==_T("����"))
	{
		pNode->Play();
		OnCbnSelchangeActlist();//���²��Ŷ���
		pBtn->SetWindowText(_T("ֹͣ"));
	}
	else if(szBtnName==_T("ֹͣ"))
	{
		pNode->Close();
		pBtn->SetWindowText(_T("����"));
	}
}
void CEffectPropDlg::OnBnClickedPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		SGEffectNode *pNode=m_preView.GetEffectNode();
	if(pNode==NULL)
		return;

	CString szBtnName;
	CWnd *pBtn=GetDlgItem(IDC_Pause);
	pBtn->GetWindowText(szBtnName);
	if(szBtnName==_T("��ͣ"))
	{
		pNode->Pause();
		pBtn->SetWindowText(_T("����"));
	}
	else if(szBtnName==_T("����"))
	{
		pNode->Resume();
		pBtn->SetWindowText(_T("��ͣ"));
	}
}

void CEffectPropDlg::OnCbnSelchangeActlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel=m_actListBox.GetCurSel();
	if(sel!=-1)
	{
		CString szName;
		m_actListBox.GetLBText(sel,szName);
		m_preView.PlayAvatarAction(szName);
	}
}

void CEffectPropDlg::OnEnChangeTimescale()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString szTxt;
	m_edTimeScale.GetWindowText(szTxt);
	if(!szTxt.IsEmpty())
		m_preView.SetTimeScale((float)_tstof(szTxt));
}

void CEffectPropDlg::OnBnClickedShowmdl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_preView.SetShowMdl(IsDlgButtonChecked(IDC_ShowMdl)==1);
}

void CEffectPropDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DetachEffectFromPreView();

	CDialog::OnClose();
}

void CEffectPropDlg::DetachEffectFromPreView()
{
	SGEffectNode *pSGNode=m_preView.GetEffectNode();
	if(m_pEftPath)
		pSGNode->RemovePath(m_szEftPathName,false);
//����	if(m_pEditEffect)
//		pSGNode->RemoveEffect(m_pEditEffect->GetName());
	if(m_pEftRender)
		pSGNode->RemoveEffect(m_pEftRender->GetName(),false);
}

void CEffectPropDlg::OnBnClickedBkcolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CColorDialog dlg(0,0,this);
	if(IDOK==dlg.DoModal())
	{
		COLORREF c=dlg.GetColor();
		Color4ub Cool3Dc(255,GetRValue(c),GetGValue(c),GetBValue(c));
		m_preView.SetBkColor(Cool3Dc);
	}
}

void CEffectPropDlg::OnLbnSelchangeState()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowSelectedPropInfo(&m_stateProp);
}

void CEffectPropDlg::OnLbnSelchangeMtlprop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowSelectedPropInfo(&m_mtlProp);
}

void CEffectPropDlg::ShowSelectedPropInfo(CPropertyList* pList)
{
	int sel=pList->GetCurSel();
	if(sel==-1)
		return;
	CPropertyItem *pItem=(CPropertyItem *)pList->GetItemDataPtr(sel);
	CString szText=_T("NULL");
	if(pItem!=NULL)
		szText.Format(_T("%s: %s"),pItem->m_propName,pItem->m_curValue);
	GetDlgItem(IDC_ListInfo)->SetWindowText(szText);
}
BOOL CEffectPropDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialog::OnNotify(wParam, lParam, pResult);
}

BOOL CEffectPropDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CEffectPropDlg::OnBnClickedDrawbox()
{
	// TODO: Add your control notification handler code here
	m_preView.EnableDrawBox( TRUE == m_hDrawBox.GetCheck() );
}
