// MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SFXEditor.h"
#include "MainDlg.h"
#include ".\maindlg.h"
#include "EffectPropDlg.h"
#include "SFXPlug.h"
#include "ClassList.h"

Vector3 g_vRotate = Vector3( 0.0f, _DegToRad( 45.0f ), 0.0f );

// CMainDlg �Ի���

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	m_pEditEffect=NULL;
	m_pPlayerMdl=NULL;
	m_pEquipMdl=NULL;
	m_bAttachedToSG=false;
	m_bUseActMsg=FALSE;
}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LibList, m_hRenderList);
	DDX_Control(pDX, IDC_PathList, m_hPathList);
	DDX_Control(pDX, IDC_SGEffectList, m_sgEffectList);
	DDX_Control(pDX, IDC_SGPathList, m_sgPathList);
	DDX_Control(pDX, IDC_PlayerAction, m_actList);
	DDX_Control(pDX, IDC_TimeS, m_hTimeScale);
	DDX_Control(pDX, IDC_PlayTime, m_hShowPlayTime);
	DDX_Control(pDX, IDC_ActLoop, m_hActLoop);
	
	DDX_Control(pDX, IDC_Comb_AvatarEle, m_Comb_AvatarEle);
	DDX_Control(pDX, IDC_Comb_BoneTagNodesForEquip, m_Comb_EquiptBoneTagNodes);
	DDX_Control(pDX, IDC_Comb_BoneTagNodes, m_Comb_EffectBoneTagNodes);
	DDX_Control(pDX, IDC_Comb_EquipTagNodes, m_Comb_EffectEquipTagNodes);
	DDX_Control(pDX, IDC_Comb_LOD, m_Comb_LOD);

	DDX_Control(pDX, IDC_FileName, m_hFileName);

	DDX_Control(pDX, IDC_MsgPlay, m_hMsgPlay);
	DDX_Control(pDX, IDC_MsgStop, m_hMsgStop);
	DDX_Control(pDX, IDC_Play, m_hPlay);
	DDX_Control(pDX, IDC_Pause, m_hPause);
	DDX_Control(pDX, IDC_Stop, m_hStop);

	DDX_Control(pDX, IDC_RoleScale, m_hRoleScale);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_LibAdd, OnBnClickedLibadd)
	ON_BN_CLICKED(IDC_LibSave, OnBnClickedLibsave)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_LibLoad, OnBnClickedLibload)
	ON_BN_CLICKED(IDC_LibDel, OnBnClickedLibdel)
	ON_LBN_SETFOCUS(IDC_LibList, OnLbnSetfocusLiblist)
	ON_LBN_SETFOCUS(IDC_PathList, OnLbnSetfocusPathlist)
	ON_BN_CLICKED(IDC_EftReset, OnBnClickedEftreset)
	ON_BN_CLICKED(IDC_EftAdd, OnBnClickedEftadd)
	ON_BN_CLICKED(IDC_EftDel, OnBnClickedEftdel)
	ON_BN_CLICKED(IDC_EftSave, OnBnClickedEftsave)
	ON_BN_CLICKED(IDC_EftLoad, OnBnClickedEftload)
	ON_LBN_DBLCLK(IDC_LibList, OnLbnDblclkLiblist)
	ON_LBN_DBLCLK(IDC_PathList, OnLbnDblclkPathlist)
	ON_BN_CLICKED(IDC_PathAdd, OnBnClickedPathadd)
	ON_BN_CLICKED(IDC_LibDelPath, OnBnClickedLibdelpath)
	ON_BN_CLICKED(IDC_PathDel, OnBnClickedPathdel)
	ON_LBN_DBLCLK(IDC_SGEffectList, OnLbnDblclkSgeffectlist)
	ON_LBN_DBLCLK(IDC_SGPathList, OnLbnDblclkSgpathlist)
	ON_LBN_SELCHANGE(IDC_SGEffectList, OnLbnSelchangeSgeffectlist)
	ON_BN_CLICKED(IDC_Play, OnBnClickedPlay)
	ON_BN_CLICKED(IDC_Pause, OnBnClickedPause)
	ON_BN_CLICKED(IDC_Stop, OnBnClickedStop)
	ON_BN_CLICKED(IDC_EftNew, OnBnClickedEftnew)
	ON_BN_CLICKED(IDC_LibClone, OnBnClickedLibclone)
	ON_BN_CLICKED(IDC_LibClonePath, OnBnClickedLibclonepath)
	ON_BN_CLICKED(IDC_LibImport, OnBnClickedLibimport)
	ON_EN_CHANGE(IDC_PosY, OnEnChangePosy)
	ON_EN_CHANGE(IDC_PosX, OnEnChangePosx)
	ON_EN_CHANGE(IDC_PosZ, OnEnChangePosz)
	ON_BN_CLICKED(IDC_EftClone, OnBnClickedEftclone)
	ON_BN_CLICKED(IDC_EftPathClone, OnBnClickedEftpathclone)
	ON_LBN_SELCHANGE(IDC_SGPathList, OnLbnSelchangeSgpathlist)
	ON_BN_CLICKED(IDC_LoadBoy, OnBnClickedLoadboy)
	ON_BN_CLICKED(IDC_LoadGirl, OnBnClickedLoadgirl)
	ON_CBN_SELCHANGE(IDC_PlayerAction, OnCbnSelchangePlayeraction)
	ON_EN_CHANGE(IDC_TimeS, OnEnChangeTimes)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_AttachMdl, &CMainDlg::OnBnClickedAttachmdl)
	ON_BN_CLICKED(IDC_BindToBone, &CMainDlg::OnBnClickedBindtobone)
	ON_BN_CLICKED(IDC_ChangeAvatar, &CMainDlg::OnBnClickedChangeavatar)
	ON_BN_CLICKED(IDC_BindToEquip, &CMainDlg::OnBnClickedBindtoequip)
	ON_CBN_SELCHANGE(IDC_Comb_LOD, &CMainDlg::OnCbnSelchangeCombLod)
	ON_EN_CHANGE(IDC_MsgPlay, &CMainDlg::OnEnChangeMsgplay)
	ON_EN_CHANGE(IDC_MsgStop, &CMainDlg::OnEnChangeMsgstop)
	ON_BN_CLICKED(IDC_UseActMsg, &CMainDlg::OnBnClickedUseactmsg)
	ON_BN_CLICKED(IDC_ReloadMsgTab, &CMainDlg::OnBnClickedReloadmsgtab)
	ON_BN_CLICKED(IDC_LoadNPC, &CMainDlg::OnBnClickedLoadnpc)
	ON_EN_CHANGE(IDC_RoleScale, &CMainDlg::OnEnChangeRolescale)
END_MESSAGE_MAP()


// CMainDlg ��Ϣ�������

void CMainDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CMainDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

EffectBase *CMainDlg::NewEftAndEdit(bool bLib)
{
	EffectBase *pRet=NULL;
	BeginMFCRes();
	CClassList selType;
	selType.DoModal();
	if(!selType.m_szSelectClass.IsEmpty())
	{
		EffectBase *pNewEffect=(EffectBase *)RTTFactory::Inst()->CreateObj(selType.m_szSelectClass);
		CEffectPropDlg dlg;
		bool bOk=false;
		if(bLib)
			bOk=SetEditEffect_Lib(&dlg,pNewEffect);
		else
			bOk=SetEditEffect_SFX(&dlg,pNewEffect);
		if(bOk)
		{
			if(IDOK==dlg.DoModal())
			{
				pRet=pNewEffect;
			}
			else
				SAFE_DELETE(pNewEffect);
		}
	}
	EndMFCRes();

	return pRet;
}

void CMainDlg::OnBnClickedLibadd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EffectBase *pNewEffect=NewEftAndEdit(true);
	if(pNewEffect)
	{
		m_eftLib.AddEft(pNewEffect);
		m_eftLib.UpdateList(&m_hRenderList,&m_hPathList);
		if(pNewEffect->IS_KIND_OF(EffectPathCtrl))
			m_hPathList.SelectString(0,pNewEffect->GetName());
		else
			m_hRenderList.SelectString(0,pNewEffect->GetName());
	}
}

void CMainDlg::OnBnClickedLibsave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		NULL,
		this,0);
	if(IDOK==dlg.DoModal())
	{
		m_eftLib.SaveFile(dlg.GetPathName());
	}
	
}

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	EffectBase::s_reloadTex=true;

	CString szTitle;
	GetWindowText(szTitle);
	CString szVer;
#ifdef _DEBUG
	szVer.Format(_T("--Build %s Debug"),__TDATE__);
#else
	szVer.Format(_T("--Build %s Release"),__TDATE__);
#endif
	szTitle+=szVer;
	SetWindowText(szTitle);

	m_hTimeScale.SetWindowText(_T("1.0"));
	
	m_hShowPlayTime.SetWindowText(_T("0.0"));
	SetTimer(1,10,0);//��ʱ��������ˢ��play time��ʾ

	m_hActLoop.SetCheck(1);

	for( int i = 0; i <= SGEffectNode::MAX_LOD; i++ )
	{
		CString str;
		str.Format( _T("%d"), i );
		m_Comb_LOD.AddString( str );
	}
	m_Comb_LOD.SetCurSel( SGEffectNode::sUserLod );
	m_hMsgPlay.EnableWindow( FALSE );
	m_hMsgStop.EnableWindow( FALSE );
	m_hRoleScale.SetWindowText( _T("1.0") );
	CheckDlgButton( IDC_Pick, TRUE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

EffectBase *CMainDlg::GetSelectedLibEffect(int focusList)
{
	CListBox *pFocusList=NULL;
	if(focusList==1)
		pFocusList=&m_hPathList;
	else
		pFocusList=&m_hRenderList;

	int sel=pFocusList->GetCurSel();
	if(sel==-1)
	{
		AfxMessageBox(_T("����ѡ��һ����ЧԪ��"));
		return NULL;
	}

	CString selName;
	pFocusList->GetText(sel,selName);
	EffectBase *pSelEft=m_eftLib.FindEft(selName);
	return pSelEft;
}

void CMainDlg::Libedit(int focusList)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EffectBase *pSelEft=GetSelectedLibEffect(focusList);
	if(pSelEft)
	{
		BeginMFCRes();
		CEffectPropDlg dlg;
		if(SetEditEffect_Lib(&dlg,pSelEft))
		{
			dlg.DoModal();
			EndMFCRes();
		}

		m_eftLib.UpdateList(&m_hRenderList,&m_hPathList);//name���ܸı�

		if(pSelEft->IS_KIND_OF(EffectPathCtrl))
			m_hPathList.SelectString(0,pSelEft->GetName());
		else
			m_hRenderList.SelectString(0,pSelEft->GetName());
	}
}

void CMainDlg::OnDestroy()
{
	CDialog::OnDestroy();

	m_eftLib.Destroy();

	// Detach ��ɾ��SceneNode
	DetachFromSG();
	if( NULL != m_pPlayerMdl )
	{
		g_editor->GetSceneGraph()->DetachDynamicNode(m_pPlayerMdl);
		SAFE_DELETE( m_pPlayerMdl );
		m_pEquipMdl = NULL;
	}
	SAFE_DELETE( m_pEditEffect );
	// TODO: �ڴ˴������Ϣ����������
}

void CMainDlg::OnBnClickedLibload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		NULL,
		this,0);
	if(IDOK==dlg.DoModal())
	{
		m_eftLib.LoadFile(dlg.GetPathName());
		m_eftLib.UpdateList(&m_hRenderList,&m_hPathList);
	}
}

void CMainDlg::OnBnClickedLibimport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		NULL,
		this,0);
	if(IDOK==dlg.DoModal())
	{
		m_eftLib.LoadFile(dlg.GetPathName(),false);
		m_eftLib.UpdateList(&m_hRenderList,&m_hPathList);
	}
}

void CMainDlg::OnBnClickedLibdel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel=m_hRenderList.GetCurSel();
	if(sel==-1)
	{
		AfxMessageBox(_T("����ѡ��һ����ЧԪ��"));
	}

	CString selName;
	m_hRenderList.GetText(sel,selName);
	
	m_eftLib.RemoveEft(selName);
	m_eftLib.UpdateList(&m_hRenderList,&m_hPathList);
}


void CMainDlg::OnBnClickedLibdelpath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel=m_hPathList.GetCurSel();
	if(sel==-1)
	{
		AfxMessageBox(_T("����ѡ��һ����ЧԪ��"));
	}

	CString selName;
	m_hPathList.GetText(sel,selName);
	
	m_eftLib.RemoveEft(selName);
	m_eftLib.UpdateList(&m_hRenderList,&m_hPathList);
}

void CMainDlg::OnLbnSetfocusLiblist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CMainDlg::OnLbnSetfocusPathlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

bool CMainDlg::SetEditEffect_SFX(CEffectPropDlg* pDlg,EffectBase *pEffect)
{
	bool bPath=pEffect->IS_KIND_OF(EffectPathCtrl);
	if(bPath)
	{
		int sel=m_sgEffectList.GetCurSel();
		if(sel==-1)
		{
			AfxMessageBox(_T("�༭·��������ѡ��һ����Ч��ȾԪ��"));
			return false;
		}
		CString selName;
		m_sgEffectList.GetText(sel,selName);
		EffectPathCtrl *pPath=(EffectPathCtrl *)pEffect;
		pDlg->SetEditEffect(m_pEditEffect->FindEffect(selName),pPath,true);
	}
	else
		pDlg->SetEditEffect(pEffect,NULL,false);
	return true;
}

bool CMainDlg::SetEditEffect_Lib(CEffectPropDlg* pDlg,EffectBase *pEffect)
{
	bool bPath=pEffect->IS_KIND_OF(EffectPathCtrl);
	if(bPath)
	{
		int sel=m_hRenderList.GetCurSel();
		if(sel==-1)
		{
			AfxMessageBox(_T("�༭·��������ѡ��һ����Ч��ȾԪ��"));
			return false;
		}
		CString selName;
		m_hRenderList.GetText(sel,selName);
		EffectPathCtrl *pPath=(EffectPathCtrl *)pEffect;
		pDlg->SetEditEffect(m_eftLib.FindEft(selName),pPath,true);
	}
	else
	{
		int sel=m_hPathList.GetCurSel();
		if(sel!=-1)
		{
			CString selName;
			m_hPathList.GetText(sel,selName);
			pDlg->SetEditEffect(pEffect,(EffectPathCtrl *)m_eftLib.FindEft(selName),false);
		}
		else
		{
			//todo:�õ�eft���󶨵�path
			pDlg->SetEditEffect(pEffect,NULL,false);
		}
	}
	return true;
}
void CMainDlg::OnBnClickedEftreset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pEditEffect!=NULL)
	{
		DetachFromSG();
		SAFE_DELETE(m_pEditEffect);
	}
}

void CMainDlg::OnBnClickedEftadd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NewEditEffectIfNULL();

	EffectBase *pSelEft=GetSelectedLibEffect(0);
	if(pSelEft!=NULL)
	{
		EffectBase *pNewEft=m_eftLib.CloneEft(pSelEft->GetName());
		m_pEditEffect->AddEffect(pNewEft);
	}
	UpdateSGEffectList();
}

void CMainDlg::OnBnClickedEftdel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pEditEffect==NULL)
		return;

	int sel=m_sgEffectList.GetCurSel();
	if(sel!=-1)
	{
		CString szSel;
		m_sgEffectList.GetText(sel,szSel);
		m_pEditEffect->RemoveEffect(szSel,true);
		UpdateSGEffectList();
	}
}

void CMainDlg::OnBnClickedEftsave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(NULL==m_pEditEffect)
	{
		AfxMessageBox(_T("�༭��ЧΪNULL"));
		return;
	}

	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("��Ч�ļ�|*.sfx||"),
		this,0);
	if(IDOK==dlg.DoModal())
	{
		CString szFilePath=dlg.GetPathName();
		szFilePath.MakeLower();
		if(szFilePath.GetLength()<=4
			|| szFilePath.Right(4)!=_T(".sfx"))
			szFilePath+=_T(".sfx");
		FArchive ar;
		ar.OpenForWrite(szFilePath);
		m_pEditEffect->Serialize(ar);
		ar.Close();
	}
		
}

void CMainDlg::OnBnClickedEftload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("��Ч�ļ�|*.sfx||"),
		this,0);
	if(IDOK==dlg.DoModal())
	{
		OnBnClickedEftreset();
		ASSERT(m_pEditEffect==NULL);

		FArchive ar;
		ar.OpenForRead(Kernel::Inst()->DefaultFS(),dlg.GetPathName());
		NewEditEffectIfNULL();
		m_pEditEffect->Deserialize(ar);
		ar.Close();

		UpdateSGEffectList();
		UpdateSGEffectPathList();

		m_hFileName.SetWindowText( dlg.GetPathName() );
	}
}

void CMainDlg::OnLbnDblclkLiblist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Libedit(0);
}

void CMainDlg::OnLbnDblclkPathlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Libedit(1);
}

void CMainDlg::AttachToSG( SceneNode* pFather, CString strTagNodeName )
{
	if(m_pEditEffect==NULL)
		return;

	DetachFromSG();
	if( NULL != pFather )
	{
		if( !strTagNodeName.IsEmpty() )
			m_pEditEffect->BindToTagNode( strTagNodeName );
		else
			m_pEditEffect->UnbindToTagNode();
		pFather->AttachChild( m_pEditEffect );
	}
	else
	{
		g_editor->GetSceneGraph()->AttachDynamicNode(m_pEditEffect);
	}
	m_bAttachedToSG = true;
}

void CMainDlg::DetachFromSG()
{
	if(m_pEditEffect==NULL)
		return;

	SceneNode* pFather = (SceneNode*)m_pEditEffect->GetParent();
	if( NULL != pFather )
	{
		m_pEditEffect->UnbindToTagNode();
		pFather->DetachChild( m_pEditEffect );
	}
	else
	{
		g_editor->GetSceneGraph()->DetachDynamicNode(m_pEditEffect);
	}
	m_bAttachedToSG = false;
}

void CMainDlg::NewEditEffectIfNULL()
{
	if(m_pEditEffect==NULL)
	{
		m_pEditEffect=new SGAttachableEffectNode;
	}
}

void CMainDlg::OnBnClickedPathadd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NewEditEffectIfNULL();

	EffectBase *pSelEft=GetSelectedLibEffect(1);
	if(pSelEft!=NULL)
	{
		ASSERT(pSelEft->IS_KIND_OF(EffectPathCtrl));
		EffectBase *pNewEft=m_eftLib.CloneEft(pSelEft->GetName());
		m_pEditEffect->AddEffectPath((EffectPathCtrl*)pNewEft);
	}
	UpdateSGEffectPathList();
}


void CMainDlg::UpdateSGEffectList()
{
	m_sgEffectList.ResetContent();
	if(m_pEditEffect==NULL)
		return;
	vector<EffectBase*> eftArray;
	m_pEditEffect->GetEffectArray(eftArray);

	for(int i=0;i<(int)eftArray.size();i++)
	{
		m_sgEffectList.AddString(eftArray[i]->GetName());
	}
}

void CMainDlg::UpdateSGEffectPathList()
{
	m_sgPathList.ResetContent();
	if(m_pEditEffect==NULL)
		return;
	vector<EffectPathCtrl*> pathArray;
	m_pEditEffect->GetEffectPathArray(pathArray);

	for(int i=0;i<(int)pathArray.size();i++)
	{
		m_sgPathList.AddString(pathArray[i]->GetName());
	}
}

void CMainDlg::OnBnClickedPathdel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pEditEffect==NULL)
		return;

	int sel=m_sgPathList.GetCurSel();
	if(sel!=-1)
	{
		CString szSel;
		m_sgPathList.GetText(sel,szSel);
		m_pEditEffect->RemovePath(szSel,true);
		UpdateSGEffectPathList();
	}
}

void CMainDlg::OnLbnDblclkSgeffectlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pEditEffect==NULL)
		return;
	int sel=m_sgEffectList.GetCurSel();
	if(sel!=-1)
	{
		CString szSel;
		m_sgEffectList.GetText(sel,szSel);

		EffectBase *pEft=m_pEditEffect->FindEffect(szSel);
		ASSERT(pEft!=NULL);

		//��Ԫ�ر༭ʱ,�ȴ����༭����SceneGraph��detach
		DetachFromSG();

		BeginMFCRes();
		CEffectPropDlg dlg;
		if(SetEditEffect_SFX(&dlg,pEft))
		{
			dlg.DoModal();
			EndMFCRes();
		}
		m_pEditEffect->RebuildEffectRenderNodes();
		UpdateSGEffectList();//���ƿ��ܸı�
		m_sgEffectList.SetCurSel(sel);

		//Ԫ�ر༭����,��Ⱦ�ڵ�attach��sg
		//AttachToSG();
	}
}

void CMainDlg::OnLbnDblclkSgpathlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pEditEffect==NULL)
		return;
	int sel=m_sgPathList.GetCurSel();
	if(sel!=-1)
	{
		CString szSel;
		m_sgPathList.GetText(sel,szSel);

		CString szName;
		EffectPathCtrl *pEft=m_pEditEffect->FindPath(szSel);
		ASSERT(pEft!=NULL);
		szName=pEft->GetName();

		//��Ԫ�ر༭ʱ,�ȴ����༭����SceneGraph��detach
		DetachFromSG();

		//��Ϊ���ƿ��ܱ��༭�ı�,������ɾ����,���������
		m_pEditEffect->RemovePath(pEft->GetName(),false);
		BeginMFCRes();
		CEffectPropDlg dlg;
		if(SetEditEffect_SFX(&dlg,pEft))
		{
			dlg.DoModal();
			EndMFCRes();
		}
		m_pEditEffect->AddEffectPath(pEft);
		UpdateSGEffectPathList();

		//Ԫ�ر༭����,��Ⱦ�ڵ�attach��sg
		//AttachToSG();
	}
}

void CMainDlg::OnLbnSelchangeSgeffectlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CMainDlg::OnBnClickedPlay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( !m_bAttachedToSG )
		AttachToSG( NULL, NULL );
	if(m_pEditEffect)
		m_pEditEffect->Play();
	OnCbnSelchangePlayeraction();

	OnEnChangeTimes();
}

void CMainDlg::OnBnClickedPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pEditEffect)
		m_pEditEffect->SetTimeScale(0);
	if(m_pPlayerMdl)
	{
		m_pPlayerMdl->GetSkeletonAniCtrl()->SetTimeFactor(0);
	}
}

void CMainDlg::OnBnClickedStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pEditEffect)
		m_pEditEffect->Close();
}


void CMainDlg::OnBnClickedEftnew()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EffectBase *pNewEffect=NewEftAndEdit(false);
	if(pNewEffect)
	{
		NewEditEffectIfNULL();
		if(pNewEffect->IS_KIND_OF(EffectPathCtrl))
		{
			m_pEditEffect->AddEffectPath((EffectPathCtrl*)pNewEffect);
			UpdateSGEffectPathList();
		}
		else
		{
			m_pEditEffect->AddEffect(pNewEffect);
			UpdateSGEffectList();
		}
	}
}

void CMainDlg::OnBnClickedLibclone()
{
	LibCloneEffect(0);
}

void CMainDlg::LibCloneEffect(int focusList)
{
	EffectBase *pSelEft=GetSelectedLibEffect(focusList);
	if(pSelEft==NULL)
	{
		AfxMessageBox(_T("���ȴ��б���ѡ��һ����Ч."));
		return;
	}
	EffectBase *pNewEft=m_eftLib.CloneEft(pSelEft->GetName());
	if(pNewEft!=NULL)
	{
		CString szNewName;
		szNewName.Format(_T("%s_%d"),pNewEft->GetName(),GetTickCount);
		if(szNewName.GetLength()>=32)
			szNewName=szNewName.Mid(szNewName.GetLength()-32);
		pNewEft->Rename(szNewName);
		m_eftLib.AddEft(pNewEft);

		m_eftLib.UpdateList(&m_hRenderList,&m_hPathList);
		if(focusList==0)
			m_hRenderList.SelectString(0,pNewEft->GetName());
		else
			m_hPathList.SelectString(0,pNewEft->GetName());
	}
	else
	{
		AfxMessageBox(_T("��Ч����ʧ��."));
	}
}

void CMainDlg::OnBnClickedLibclonepath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LibCloneEffect(1);
}


void CMainDlg::SetEffectPos(const Vector3& pos,bool bUpdateEdit)
{
	float fRoleScale=1.0f;
	CString strScale;
	m_hRoleScale.GetWindowText( strScale );
	if( !strScale.IsEmpty() )
		fRoleScale=(float)_tstof(strScale);

	Transform trans;
	trans.Scale(fRoleScale,fRoleScale,fRoleScale);
	trans.Rotate( g_vRotate.y, g_vRotate.x, g_vRotate.z );
	trans.Translate(pos);

	if(m_pPlayerMdl!=NULL)
		m_pPlayerMdl->SetWorldMat(trans.GetMatrix());

	if(m_pEditEffect!=NULL)
		m_pEditEffect->SetWorldMat(trans.GetMatrix());

	if(bUpdateEdit)
	{
		CString szTmp;
		szTmp.Format(_T("%.2f"),pos.x);
		GetDlgItem(IDC_PosX)->SetWindowText(szTmp);
		szTmp.Format(_T("%.2f"),pos.y);
		GetDlgItem(IDC_PosY)->SetWindowText(szTmp);
		szTmp.Format(_T("%.2f"),pos.z);
		GetDlgItem(IDC_PosZ)->SetWindowText(szTmp);
	}
}

void CMainDlg::OnRender()
{
	if( IsDlgButtonChecked(IDC_DrawBox) && NULL != m_pEditEffect )
	{
		AABBox boxEff;
		m_pEditEffect->ReBuildLocalBox();
		m_pEditEffect->GetBox( boxEff );
		DrawX::DrawAABBox( boxEff );
	}
}

void CMainDlg::UpdatePosFromEdit()
{
	CString szX,szY,szZ;
	GetDlgItem(IDC_PosX)->GetWindowText(szX);
	GetDlgItem(IDC_PosY)->GetWindowText(szY);
	GetDlgItem(IDC_PosZ)->GetWindowText(szZ);
	Vector3 pos;
	pos.x=(float)_tstof(szX);
	pos.y=(float)_tstof(szY);
	pos.z=(float)_tstof(szZ);

	SetEffectPos(pos,false);
}

void CMainDlg::OnEnChangePosy()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdatePosFromEdit();
}

void CMainDlg::OnEnChangePosx()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdatePosFromEdit();
}

void CMainDlg::OnEnChangePosz()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdatePosFromEdit();
}

BOOL CMainDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CMainDlg::OnBnClickedEftclone()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pEditEffect==NULL)
		return;
	int sel=m_sgEffectList.GetCurSel();
	if(sel!=-1)
	{
		CString szSel;
		m_sgEffectList.GetText(sel,szSel);

		EffectBase *pEft=m_pEditEffect->FindEffect(szSel);
		ASSERT(pEft!=NULL);
		EffectBase *pNewEft=_CloneEffect(pEft);
		if(pNewEft!=NULL)
		{
			m_eftLib.AddEft(pNewEft);
			m_eftLib.UpdateList(&m_hRenderList,&m_hPathList);
		}
	}
}

void CMainDlg::OnBnClickedEftpathclone()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pEditEffect==NULL)
		return;
	int sel=m_sgPathList.GetCurSel();
	if(sel!=-1)
	{
		CString szSel;
		m_sgPathList.GetText(sel,szSel);

		EffectBase *pPath=m_pEditEffect->FindPath(szSel);
		if(pPath!=NULL)
		{
			EffectBase *pNewPath=_CloneEffect(pPath);
			m_eftLib.AddEft(pNewPath);
			m_eftLib.UpdateList(&m_hRenderList,&m_hPathList);
		}
	}
}

void CMainDlg::OnLbnSelchangeSgpathlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CMainDlg::OnBnClickedLoadboy()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pPlayerMdl!=NULL)
	{
		DetachFromSG();
		g_editor->GetSceneGraph()->DetachDynamicNode(m_pPlayerMdl);
		SAFE_DELETE(m_pPlayerMdl);
		m_pEquipMdl = NULL;
	}
	m_pPlayerMdl=_LoadDefaultAvatar(0);
	g_editor->GetSceneGraph()->AttachDynamicNode(m_pPlayerMdl);
	AttachToSG( NULL, NULL );
	_LoadAvatarActionList(m_pPlayerMdl,&m_actList);
	_LoadAvatarElementList(&m_Comb_AvatarEle);
	_LoadBoneTagNodeList(&m_Comb_EquiptBoneTagNodes);
	_LoadBoneTagNodeList(&m_Comb_EffectBoneTagNodes);
}

void CMainDlg::OnBnClickedLoadgirl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pPlayerMdl!=NULL)
	{
		DetachFromSG();
		g_editor->GetSceneGraph()->DetachDynamicNode(m_pPlayerMdl);
		SAFE_DELETE(m_pPlayerMdl);
		m_pEquipMdl = NULL;
	}
	m_pPlayerMdl=_LoadDefaultAvatar(1);
	g_editor->GetSceneGraph()->AttachDynamicNode(m_pPlayerMdl);
	AttachToSG( NULL, NULL );
	_LoadAvatarActionList(m_pPlayerMdl,&m_actList);
	_LoadAvatarElementList(&m_Comb_AvatarEle);
	_LoadBoneTagNodeList(&m_Comb_EquiptBoneTagNodes);
	_LoadBoneTagNodeList(&m_Comb_EffectBoneTagNodes);
}

void CMainDlg::OnBnClickedLoadnpc()
{
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("���������ļ�|*.fskel||"),
		this,0);
	if(IDOK==dlg.DoModal())
	{
		if(m_pPlayerMdl!=NULL)
		{
			DetachFromSG();
			g_editor->GetSceneGraph()->DetachDynamicNode(m_pPlayerMdl);
			SAFE_DELETE(m_pPlayerMdl);
			m_pEquipMdl = NULL;
		}
		m_pPlayerMdl=_LoadNPC(dlg.GetPathName());
		g_editor->GetSceneGraph()->AttachDynamicNode(m_pPlayerMdl);
		AttachToSG( NULL, NULL );
		_LoadAvatarActionList(m_pPlayerMdl,&m_actList);
		m_Comb_AvatarEle.ResetContent();
		m_Comb_EquiptBoneTagNodes.ResetContent();
		m_Comb_EffectBoneTagNodes.ResetContent();
	}
}

void CMainDlg::OnCbnSelchangePlayeraction()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int sel=m_actList.GetCurSel();
	if(sel!=-1)
	{
		CString szName;
		m_actList.GetLBText(sel,szName);
		_PlayAvatarAction(m_pPlayerMdl,szName,m_hActLoop.GetCheck()!=0);
	}
}

void CMainDlg::OnEnChangeTimes()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString szTxt;
	m_hTimeScale.GetWindowText(szTxt);
	float timescale=(float)_tstof(szTxt);
	if(m_pEditEffect)
		m_pEditEffect->SetTimeScale(timescale);
	if(m_pPlayerMdl)
	{
		m_pPlayerMdl->GetSkeletonAniCtrl()->SetTimeFactor(timescale);
	}
}

void CMainDlg::OnTimer(UINT nIDEvent)
{
	// ��Ч����ʱ��
	if(m_pEditEffect!=NULL)
	{
		CString szTime;
		szTime.Format(_T("%.2f"),m_pEditEffect->GetRunTime());
		m_hShowPlayTime.SetWindowText(szTime);
	}

	// ��������ʱ��
	if( NULL != m_pPlayerMdl )
	{
		NodeAnimationCtrl* pAniCtrl=m_pPlayerMdl->GetSkeletonAniCtrl();
		if(pAniCtrl!=NULL)
		{
			CString szTrackName;
			m_actList.GetWindowText( szTrackName );
			CString szTime;
			szTime.Format(_T("%.2f"),pAniCtrl->GetTrackPosition(szTrackName));
			SetDlgItemText( IDC_ActPlayTime, szTime );
		}
	}

	// ʹ�ö�����Ϣ�㲥��ֹͣ��Ч
	if( m_bUseActMsg && NULL != m_pPlayerMdl && NULL != m_pEditEffect )
	{
		NodeAnimationCtrl* pAniCtrl=m_pPlayerMdl->GetSkeletonAniCtrl();
		if(pAniCtrl!=NULL)
		{
			DWORD dwMsg=pAniCtrl->PeekFrameMsg();
			if( 0 != dwMsg )
			{
				TCHAR szAniMsg[5];
				ZeroMemory(szAniMsg,sizeof(szAniMsg));
				_FourCC2Str(dwMsg,szAniMsg);
				if( m_strMsgPlay == szAniMsg )
					m_pEditEffect->Play();
				else if( m_strMsgStop == szAniMsg )
					m_pEditEffect->Close();
				else if( 0 == _tcscmp( _T("BGN"), szAniMsg ) )
					m_pEditEffect->Close();
			}
		}
	}


	CDialog::OnTimer(nIDEvent);
}

void CMainDlg::OnBnClickedChangeavatar()
{
	if( NULL == m_pPlayerMdl )
	{
		AfxMessageBox( _T("��û�е����ɫģ�ͣ�") );
		return;
	}
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("��Ƥ�ļ�(*.fskin)|*.fskin|�����ļ�(*.*)|*.*||"),
		this,0);
	if(IDOK==dlg.DoModal())
	{
		CString strEleName;
		m_Comb_AvatarEle.GetWindowText( strEleName );
		if( !_ChangeSkin( m_pPlayerMdl, strEleName, dlg.GetPathName() ) )
		{
			AfxMessageBox( _T("������Ƥʧ�ܣ�") );
		}
	}
}

void CMainDlg::OnBnClickedAttachmdl()
{
	if( NULL == m_pPlayerMdl )
	{
		AfxMessageBox( _T("��û�е����ɫģ�ͣ�") );
		return;
	}

	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("��̬ģ���ļ�(*.fsm)|*.fsm|�ؼ�֡�ļ�(*.fak)|*.fak|�����ļ�(*.*)|*.*||"),
		this,0);
	if(IDOK==dlg.DoModal())
	{
		if( NULL != m_pEquipMdl )
		{
			DetachFromSG();
			m_pPlayerMdl->DetachChild( m_pEquipMdl );
			SAFE_DELETE( m_pEquipMdl );
			AttachToSG( NULL, NULL );
		}
		m_pEquipMdl=new SGAvatarAttachment;
		try
		{
			const bool bIOAsync = ResMgr::Inst()->IsIOAsync();
			ResMgr::Inst()->EnableIOAsync( false );
			m_pEquipMdl->LoadRes(dlg.GetPathName());
			ResMgr::Inst()->EnableIOAsync( bIOAsync );	
		}
		catch(...)
		{
			SAFE_DELETE( m_pEquipMdl );
			AfxMessageBox( _T("����װ��ģ��ʧ�ܣ�"));
			EndMFCRes();
			return;
		}

		CString strTagNodeName;
		m_Comb_EquiptBoneTagNodes.GetWindowText(strTagNodeName);
		if( !strTagNodeName.IsEmpty() )
			m_pEquipMdl->BindToTagNode(strTagNodeName);
		m_pPlayerMdl->AttachChild(m_pEquipMdl);

		_LoadEquipTagNodeList( m_pEquipMdl, &m_Comb_EffectEquipTagNodes );
	}	
}

void CMainDlg::OnBnClickedBindtobone()
{
	if( NULL == m_pPlayerMdl )
	{
		AfxMessageBox( _T("��û�е����ɫģ�ͣ�") );
		return;
	}

	DetachFromSG();
	CString strTagNodeName;
	m_Comb_EffectBoneTagNodes.GetWindowText(strTagNodeName);
	AttachToSG( m_pPlayerMdl, strTagNodeName );
}

void CMainDlg::OnBnClickedBindtoequip()
{
	if( NULL == m_pPlayerMdl )
	{
		AfxMessageBox( _T("��û�е����ɫģ�ͣ�") );
		return;
	}

	if( NULL == m_pEquipMdl )
	{
		AfxMessageBox( _T("û��װ�����Թ��أ�") );
		return;
	}

	DetachFromSG();
	CString strTagNodeName;
	m_Comb_EffectEquipTagNodes.GetWindowText(strTagNodeName);
	AttachToSG( m_pEquipMdl, strTagNodeName );
}
void CMainDlg::OnCbnSelchangeCombLod()
{
	// TODO: Add your control notification handler code here
	SGEffectNode::sUserLod = m_Comb_LOD.GetCurSel();
}

void CMainDlg::OnEnChangeMsgplay()
{
	m_hMsgPlay.GetWindowText( m_strMsgPlay );
}

void CMainDlg::OnEnChangeMsgstop()
{
	m_hMsgStop.GetWindowText( m_strMsgStop );
}

void CMainDlg::OnBnClickedUseactmsg()
{
	m_bUseActMsg = IsDlgButtonChecked(IDC_UseActMsg);
	if( m_bUseActMsg )
	{
		m_hMsgPlay.EnableWindow( TRUE );
		m_hMsgStop.EnableWindow( TRUE );

		m_hPlay.EnableWindow( FALSE );
		m_hPause.EnableWindow( FALSE );
		m_hStop.EnableWindow( FALSE );
	}
	else
	{
		m_hMsgPlay.EnableWindow( FALSE );
		m_hMsgStop.EnableWindow( FALSE );

		m_hPlay.EnableWindow( TRUE );
		m_hPause.EnableWindow( TRUE );
		m_hStop.EnableWindow( TRUE );
	}
	if( NULL != m_pEditEffect )
		m_pEditEffect->Close();
}

void CMainDlg::OnBnClickedReloadmsgtab()
{
	if( NULL != m_pPlayerMdl )
		m_pPlayerMdl->ReloadMsgTable();
}
void CMainDlg::OnEnChangeRolescale()
{
	CString strScale;
	m_hRoleScale.GetWindowText( strScale );
	if( strScale.IsEmpty() )
		return;

	float fRoleScale=(float)_tstof(strScale);
	if( NULL != m_pPlayerMdl )
	{
		const Matrix4 mat = m_pPlayerMdl->GetWorldMat();
		
		Transform trans;
		trans.Scale( fRoleScale, fRoleScale, fRoleScale );
		trans.Rotate( g_vRotate.y, g_vRotate.x, g_vRotate.z );
		trans.Translate( Vector3( mat._41, mat._42, mat._43 ) );
		m_pPlayerMdl->SetWorldMat(trans.GetMatrix());
	}
	if( NULL != m_pEditEffect )
	{
		const Matrix4 mat = m_pEditEffect->GetWorldMat();

		Transform trans;
		trans.Scale( fRoleScale, fRoleScale, fRoleScale );
		trans.Rotate( g_vRotate.y, g_vRotate.x, g_vRotate.z );
		trans.Translate( Vector3( mat._41, mat._42, mat._43 ) );
		m_pEditEffect->SetWorldMat(trans.GetMatrix());
	}
}