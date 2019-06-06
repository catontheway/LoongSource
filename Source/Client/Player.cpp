#include "StdAfx.h"
#include "Player.h"
#include "FSM_RP.h"
#include "AvatarCfg.h"
#include "MapMgr.h"
#include "MapLogicData.h"
#include "RoleEvent.h"
#include "..\WorldDefine\role_att.h"
#include "..\WorldDefine\move_define.h"
#include "..\WorldDefine\msg_pk.h"
#include "..\WorldDefine\msg_combat.h"
#include "PlayerNameTab.h"
#include "StallMgr.h"
#include "RoleMgr.h"
#include "Pet.h"
#include "CombatSysUtil.h"
#include "EffectMgr.h"

IMP_RTTI(Player,Role);

Player::Player(void)
: m_pAvatarNode(NULL)
, m_pFSM(NULL)
, m_roleState(0)
, m_ePKState(ERPKS_Peace)
, m_bAttributeInited(false)
, m_bHide(false)
, m_pMount(0)
, m_bunsBoneIndex(-1)
, m_dwGuildID(GT_INVALID)
, m_moveYaw(0)
, m_n8GuildPos(0)
, m_lastYaw(0.0f)
, m_lastPitch(0.0f)
, m_dwRebornMapID(GT_INVALID)
, m_vRebornPoint(0.0f, 0.0f, 0.0f)
, m_dwTransportID(GT_INVALID)
, m_vTransportPoint(0.0f, 0.0f, 0.0f)
, m_eClass(EV_Null)
, m_eClassEx(EHV_Base)
, m_dwLoverID(GT_INVALID)
, m_bHaveWedding(FALSE)
{
	m_aniPlayer.SetPlayer(this);
}

Player::~Player(void)
{
	// ������ڰ�̯״̬����ɾ��̯λ
	if(GetRoleState(ERS_Stall))
		SendEventStallAppear(false);

	SAFE_DELETE(m_pFSM);
	DetachShadow();
	SAFE_DELETE(m_pAvatarNode);

	m_pMount = NULL;
}

void Player::InitFSM()
{
	SAFE_DELETE(m_pFSM);
	m_pFSM=new FSM_RP(this);
	m_pFSM->Init();
}

void Player::AttachSceneNodeToSG( SceneGraph* pSG )
{
	ASSERT(m_pAvatarNode!=NULL);
	ASSERT(pSG!=NULL);
	pSG->AttachDynamicNode(m_pAvatarNode);
}

void Player::DetachSceneNodeFromSG( SceneGraph* pSG )
{
	ASSERT(m_pAvatarNode!=NULL);
	ASSERT(pSG!=NULL);
	pSG->DetachDynamicNode(m_pAvatarNode);
}


Vector3 Player::GetRoleSize()
{
	if( P_VALID(m_pMount) )
	{
		return m_pMount->GetRoleSize();
	}
	else
	{
		Vector3 roleSize;
		roleSize.x=X_DEF_ROLE_SIZE_X*(float(m_roledata[ERA_Shape])/10000.0f);
		roleSize.y=X_DEF_ROLE_SIZE_Y*(float(m_roledata[ERA_Shape])/10000.0f);
		roleSize.z=X_DEF_ROLE_SIZE_Z*(float(m_roledata[ERA_Shape])/10000.0f);

		// cxb-wy 2009/12/02 ��ֹ���Ǳ�С��ǽ
		if(RoleMgr::Inst()->GetLocalPlayerID() == this->GetID())
		{
			if( roleSize.x < X_DEF_ROLE_SIZE_X ||
				roleSize.y < X_DEF_ROLE_SIZE_Y ||
				roleSize.z < X_DEF_ROLE_SIZE_Z)
			{
				roleSize.x = X_DEF_ROLE_SIZE_X;
				roleSize.y = X_DEF_ROLE_SIZE_Y;
				roleSize.z = X_DEF_ROLE_SIZE_Z;
			}
		}

		return roleSize;
	}
}

void Player::LoadDefaultAvatar()
{
	ASSERT(m_pAvatarNode==NULL);//��ֹ�ظ�����

	m_pAvatarNode=new AvatarEquipNode;
	m_pAvatarNode->LoadDefault();

	// ���ͻ�װ��Ϸ�¼�
	tagUpdataAvatarEvent evt( _T("tagUpdataAvatarEvent"), NULL );
	evt.dwRoleID = m_id;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);

	m_pAvatarNode->Hide( m_bHide );
}

void Player::LoadAvatar(const tagAvatarAtt& att, const tagAvatarEquip& ep, const tagDisplaySet& displaySet)
{
	ASSERT(m_pAvatarNode==NULL);//��ֹ�ظ�����

	m_pAvatarNode = new AvatarEquipNode;
	m_pAvatarNode->SetDisplaySet( displaySet );
	m_pAvatarNode->LoadAvatar( att, ep );

	// ���ͻ�װ��Ϸ�¼�
	tagUpdataAvatarEvent evt( _T("tagUpdataAvatarEvent"), NULL );
	evt.dwRoleID = m_id;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);

	m_pAvatarNode->Hide( m_bHide );
}

void Player::UpdateAvatar(const tagAvatarAtt& att, const tagAvatarEquip& ep, SceneGraph* pSG)
{
	//�Ա����,���¼���ģ��
	if( m_pAvatarNode->GetAtt().bySex!=att.bySex )				
	{
		ASSERT(m_pAvatarNode!=NULL);
		ASSERT(pSG!=NULL);
		//�ӳ�����ɾ��
		pSG->DetachDynamicNode(m_pAvatarNode);
		tagDisplaySet displaySet = m_pAvatarNode->GetDisplaySet();
		SAFE_DELETE(m_pAvatarNode);

		//���¼���ģ��
		LoadAvatar(att,ep,displaySet);

		ASSERT(m_pAvatarNode!=NULL);
		ASSERT(pSG!=NULL);
		//���¼��ص�������
		pSG->AttachDynamicNode(m_pAvatarNode);		
	}
	else
	{
		ASSERT(m_pAvatarNode!=NULL);
		m_pAvatarNode->UpdateAllAtt( att );
		m_pAvatarNode->UpdateAllEquip( ep );
	}

	// ���ͻ�װ��Ϸ�¼�
	tagUpdataAvatarEvent evt( _T("tagUpdataAvatarEvent"), NULL );
	evt.dwRoleID = m_id;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);

	m_pAvatarNode->Hide( m_bHide );

	// ��������һ��͸����
	if(m_pAvatarNode)
		m_pAvatarNode->SetTransparent(m_fTransparent);
}

void Player::SetAvatarEquip( const EAvatarElement eAvaEnt, const tagEquipDisplay& ed ) 
{ 
	ASSERT(m_pAvatarNode!=NULL);
	m_pAvatarNode->SetEquip( eAvaEnt, ed );

	// ���ͻ�װ��Ϸ�¼�
	tagUpdataAvatarEvent evt( _T("tagUpdataAvatarEvent"), NULL );
	evt.dwRoleID = m_id;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);

	// ��������������Ϸ�¼�
	if( EAE_RWeapon == eAvaEnt || EAE_LWeapon == eAvaEnt )
	{
		tagWeaponChangeEvent evt( _T("tagWeaponChangeEvent"), NULL );
		evt.dwRoleID = m_id;
		evt.bRightWeapon = ( EAE_RWeapon == eAvaEnt );
		TObjRef<GameFrameMgr>()->SendEvent(&evt);
	}

	// ��������һ��͸����
	if(m_pAvatarNode)
		m_pAvatarNode->SetTransparent(m_fTransparent);
}

const tagEquipDisplay& Player::GetAvatarEquip( const EAvatarElement eAvaEnt ) const 
{ 
	ASSERT(m_pAvatarNode!=NULL);
	return m_pAvatarNode->GetEquip( eAvaEnt ); 
}

const tagAvatarAtt& Player::GetAvatarAtt() const 
{ 
	ASSERT(m_pAvatarNode!=NULL);
	return m_pAvatarNode->GetAtt(); 
}

void Player::HandUpWeapon( const bool bHandUp )
{
	if( NULL == m_pAvatarNode )
		return;
	m_pAvatarNode->HandUpWeapon( bHandUp );
}

EItemTypeEx Player::GetRWeaponType() const
{
	if(m_pAvatarNode==NULL)
		return EITE_Null;
	return m_pAvatarNode->GetRWeaponType();
}

EItemTypeEx Player::GetLWeaponType() const
{
	if(m_pAvatarNode==NULL)
		return EITE_Null;
	return m_pAvatarNode->GetLWeaponType();
}

void Player::OpenAllEquip( const bool bOpen )
{
	if( NULL == m_pAvatarNode )
		return;
	m_pAvatarNode->OpenAllEquip( bOpen );
}

bool Player::IsAllEquipOpened()
{
	if( NULL == m_pAvatarNode )
		return false;
	return m_pAvatarNode->IsAllEquipOpened();
}

bool Player::IsAllEquipClosed()
{
	if( NULL == m_pAvatarNode )
		return true;
	return m_pAvatarNode->IsAllEquipClosed();
}

void Player::UpdataLongHunEffect( const EAvatarElement eAvaEnt, const BYTE byEquipEffect )
{
	if( NULL == m_pAvatarNode )
		return;
	if( byEquipEffect == m_pAvatarNode->GetEquip( eAvaEnt ).byEquipEffect )
		return;
	m_pAvatarNode->UpdataLongHunEffect( eAvaEnt, byEquipEffect );
	
	// ���ͻ�װ��Ϸ�¼�
	tagUpdataAvatarEvent evt( _T("tagUpdataAvatarEvent"), NULL );
	evt.dwRoleID = m_id;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);
}

void Player::UpdataSuitEffect( const DWORD dwSuitID )
{
	if( NULL == m_pAvatarNode )
		return;
	if( dwSuitID == m_pAvatarNode->GetSuitID() )
		return;
	m_pAvatarNode->UpdataSuitEffect( dwSuitID );

	// ���ͻ�װ��Ϸ�¼�
	tagUpdataAvatarEvent evt( _T("tagUpdataAvatarEvent"), NULL );
	evt.dwRoleID = m_id;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);
}

void Player::SetPlayerName(LPCTSTR szName, DWORD dwNameCrc /* = GT_INVALID */)
{
	m_strRoleName = szName;
	if (GT_INVALID == dwNameCrc)//û����������Crc�����Լ���
		m_dwRoleNameCrc = TObjRef<Util>()->Crc32(szName);
	else
		m_dwRoleNameCrc = dwNameCrc;	

	//�������ָ����¼�
	
}

void Player::SetPlayerClass(EClassType eClass)
{
    if (m_eClass == eClass)
        return;

	m_eClass = eClass;

	//����ר��ְҵ�����¼�

    // ������Ч
    if (m_bAttributeInited)
        EffectMgr::Inst()->PlayRoleEffect( m_id, _T("Txxy09") );
}

void Player::SetPlayerClassEx( EClassTypeEx eClassEx )
{
    m_eClassEx = eClassEx;

    //����Ӣ��ְҵ�����¼�
}

void Player::Update()
{
	//--������Ϣ��
	NodeAnimationCtrl* pAniCtrl = GetAniCtrl();
	HandleMsgCode(pAniCtrl);

	//--update rotater
	m_rotaterYaw.Update();
	m_rotaterPitch.Update();
	m_upperRotater.Update();
	m_headRotater.Update();
	m_scaler.Update();

	//--update FSM
	if( P_VALID(m_pFSM) )
		m_pFSM->Update();

	//--update animation player
	m_aniPlayer.Update();

	//--update world matrix
	if( NULL != m_pAvatarNode )
	{
		if( P_VALID(m_pMount) )//���״̬����Ҫ���¼����������
		{
			if( m_pMount->GetOwnerID() == m_id )//�����״̬
			{
				Matrix4 matRide;
				if( CalRideMat(m_pMount,matRide,0) )
				{
					m_pAvatarNode->SetWorldMat( &matRide );
				}
			}
			else//���״̬
			{
				Matrix4 matRide;
				if( CalRideMat(m_pMount,matRide,1) )
				{
					m_pAvatarNode->SetWorldMat( &matRide );
				}
			}
		}
		else//��ͨ״̬
		{
			m_pAvatarNode->SetWorldMat(m_space.GetMatrix());
		}
	}
}

void Player::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_SetRoleState"))
	{
		tagNS_SetRoleState* pCmd=(tagNS_SetRoleState*)pNetCmd;
		SetRoleState(pCmd->eState, true);
	}
	else if(pNetCmd->dwID==pNetCmd->Crc32("NS_UnSetRoleState"))
	{
		tagNS_UnSetRoleState* pCmd=(tagNS_UnSetRoleState*)pNetCmd;
		SetRoleState(pCmd->eState, false);
	}
	else if(pNetCmd->dwID==pNetCmd->Crc32("NS_RolePKStateChange"))
	{
		tagNS_RolePKStateChange* pCmd=(tagNS_RolePKStateChange*)pNetCmd;
		m_ePKState=pCmd->eState;
	}
	m_pFSM->OnNetCmd(pNetCmd);
}

void Player::OnGameEvent( tagGameEvent* pGameEvent )
{
	m_pFSM->OnGameEvent(pGameEvent);
}

bool Player::IsReadyState()
{
	return (m_roleState&ERS_Combat)!=0;
}

bool Player::IsStandState()
{
	return m_pFSM->IsStandState();
}

bool Player::IsStyleActionState()
{
	return m_pFSM->IsStyleActionState();
}

bool Player::IsIdleState()
{
	return m_pFSM->IsUpperIdleState()
		&& (m_pFSM->IsStandState()||m_pFSM->IsFloatState()||m_pFSM->IsWaterStandState());
}

bool Player::IsDead()
{
	return m_pFSM->IsDeadState();
}

bool Player::IsAttributeInited() const
{
	return m_bAttributeInited;
}

void Player::Hide( const bool bHide )
{
	if( bHide == m_bHide )
		return;
	m_bHide = bHide;
	if( P_VALID( m_pAvatarNode ) )
		m_pAvatarNode->Hide( m_bHide );
}

NodeAnimationCtrl* Player::GetAniCtrl()
{
	if(m_pAvatarNode==NULL
		||NULL==m_pAvatarNode->GetResource()
		||!m_pAvatarNode->GetResource()->IsCreated())
		return NULL;

	return m_pAvatarNode->GetSkeletonAniCtrl();
}

void Player::GetHeadPos( Vector3& out )
{
	out=m_space.GetPos();
	if(P_VALID(m_pAvatarNode))
	{
		AABBox box;
		m_pAvatarNode->GetBox(box);
		DWORD bunsBoneIndex=-1;
		ResSkeleton *pSkel = const_cast<ResSkeleton*>(m_pAvatarNode->GetResource());
		if(pSkel!=NULL && pSkel->IsCreated())
		{
			// ��ͬ���Ա�ȡ��ͬ�Ĺ���ID
			bunsBoneIndex = pSkel->GetBoneID(_T("Bip01 HeadNub"));

			if (bunsBoneIndex == -1)
				bunsBoneIndex = pSkel->GetBoneID(_T("Bip01 HeadNub01"));
		}

		if (bunsBoneIndex != -1)
		{
			NodeAnimationCtrl *pAniCtrl = m_pAvatarNode->GetSkeletonAniCtrl();
			if(pAniCtrl != NULL)
			{
				/*out.x = box.min.x+pAniCtrl->GetBoneSpace(bunsBoneIndex)->_41;
				out.y = box.min.y+pAniCtrl->GetBoneSpace(bunsBoneIndex)->_42;
				out.z = box.min.z+pAniCtrl->GetBoneSpace(bunsBoneIndex)->_43;*/
				const Matrix4* pMatrix4 = pAniCtrl->GetBoneSpace(bunsBoneIndex);
				Vector3 vec3(pMatrix4->_41, pMatrix4->_42, pMatrix4->_43);
				out = vec3 * m_pAvatarNode->GetWorldMat();
			}
			else
				out.y = box.max.y;

		}
	}
	else
	{
		out.y += 180;
	}
}

void Player::InitAttribute(tagNS_GetRemoteRoleState* pCmd)
{
	// ȷ��SeneneNode�Ѿ�����
	ASSERT( P_VALID( m_pAvatarNode ) );

	// ���ý�ɫλ�úͳ���
	Vector3 Pos(pCmd->RoleData.fPos[0], pCmd->RoleData.fPos[1], pCmd->RoleData.fPos[2]);
	Vector3 FaceTo(pCmd->RoleData.fFaceTo[0], pCmd->RoleData.fFaceTo[1], pCmd->RoleData.fFaceTo[2]);
	SetPos(Pos);
	SetYaw(CalcYaw(FaceTo));
	SetMoveYaw(GetYaw());

	// ���ý�ɫ����
	INT i;
	for (i=0; i<ERRA_End; ++i)
	{
		SetRemoteRoleAtt((ERemoteRoleAtt)i, pCmd->RoleData.nAtt[i], true);			
	}
	
	// ���ý�ɫ�ȼ�
	SetRoleLevel(pCmd->RoleData.nLevel);

	// ���ý�ɫ����
	tstring strName = PlayerNameTab::Inst()->FindNameByID(m_id);
	if(strName != _T(""))
	{
		SetRoleName(strName);
	}

	// �������ְҵ
	//SetPlayerClass(pCmd->RoleData.eClass, pCmd->RoleData.eClassEx);

    // ���õ�ǰ�ƺ�id
    SetPlayerTitle(pCmd->RoleData.u16CurActTitleID);

    // ����ְҵ
    SetPlayerClass(pCmd->RoleData.eClassType);
    SetPlayerClassEx(pCmd->RoleData.eClassTypeEx);

    // ������ż
    SetPlayerLoverID(pCmd->RoleData.dwLoverID);
    SetPlayerIsHasWedding(pCmd->RoleData.bHaveWedding);

	// �������״̬
	SetRoleState(pCmd->RoleData.dwRoleState);

	// �ж��Ƿ�Ϊ��̯״̬
	if (pCmd->RoleData.dwRoleState & ERS_Stall)
	{
		SetStallLevel(pCmd->RoleData.byStallLevel);
		this->SendEventStallAppear(true, pCmd->RoleData.byStallLevel);
	}

	// ����PK״̬
	m_ePKState=pCmd->RoleData.ePKState;

	// ���ý�ɫ״̬
	m_state.Init(pCmd->RoleData.dwState);

	// �����ɫ����ʱ��������ֱ�ӽ�������״̬
	if(IsInState(ES_Dead))
	{
		m_pFSM->Change2LowerState_AutoStand();
		m_pFSM->Change2UpperState_Dead(false);
	}

	// �������������Ϣ
	SetRoleGuildID(pCmd->RoleData.dwGuildID);
	SetRoleGuildPos(pCmd->RoleData.n8GuildPos);

	// ���buff�б�
	FreeRoleBuff();
	for(INT j=0; j<pCmd->RoleData.nBuffNum; ++j)
	{
		AddRoleBuff(pCmd->RoleData.Buff[j]);
	}

	// ��״̬��α��AddBuff��Ϣ
	tagNS_AddRoleBuff tmpCmd;
	for(i=0; i<pCmd->RoleData.nBuffNum; ++i)
	{
		tmpCmd.dwRoleID=GetID();
		tmpCmd.Buff=pCmd->RoleData.Buff[i];
		m_pFSM->OnNetCmd(&tmpCmd);
	}

	// �ж��Ƿ���Ҫֱ�ӽ���ս��״̬
	tagNS_SetRoleState setRoleStateCmd;
	setRoleStateCmd.dwRoleID=GetID();
	if(m_roleState&ERS_Combat)
	{
		setRoleStateCmd.eState=ERS_Combat;
		m_pFSM->ActiveGlobalState(FSM_RP::EGlobalState_Combat,&setRoleStateCmd,1);
	}

	// ��״̬��α��SetState��Ϣ
	tagNS_SetState setStateCmd;
	setStateCmd.dwRoleID=GetID();
	if(IsInState(ES_Dizzy))
	{
		setStateCmd.eState=ES_Dizzy;
		m_pFSM->OnNetCmd(&setStateCmd);
	}
	if(IsInState(ES_Tie))
	{
		setStateCmd.eState=ES_Tie;
		m_pFSM->OnNetCmd(&setStateCmd);
	}			
	if(IsInState(ES_Spor))
	{
		setStateCmd.eState=ES_Spor;
		m_pFSM->OnNetCmd(&setStateCmd);
	}			
	if(IsInState(ES_Invincible))
	{
		setStateCmd.eState=ES_Invincible;
		m_pFSM->OnNetCmd(&setStateCmd);
	}			
	if(IsInState(ES_Lurk))
	{
		setStateCmd.eState=ES_Lurk;
		m_pFSM->OnNetCmd(&setStateCmd);
	}			
	if(IsInState(ES_DisArm))
	{
		setStateCmd.eState=ES_DisArm;
		m_pFSM->OnNetCmd(&setStateCmd);
	}			
	if(IsInState(ES_NoSkill))
	{
		setStateCmd.eState=ES_NoSkill;
		m_pFSM->OnNetCmd(&setStateCmd);
	}

	// ��������

	m_bAttributeInited = true;
}

void Player::HideWeapon( bool bHide )
{
	if( P_VALID( m_pAvatarNode ) )
		m_pAvatarNode->HideWeapon( bHide );
}

void Player::HideWeaponPertain(bool bHide)
{
	if (P_VALID(m_pAvatarNode))
		m_pAvatarNode->HideWeaponPertain(bHide);
}


void Player::PlaySuitEffect(bool bPlay)
{
	if (P_VALID(m_pAvatarNode))
		m_pAvatarNode->PlaySuitEffect(bPlay);
}

void Player::SetTransparent( float alpha )
{
	if( P_VALID( m_pAvatarNode) )
		m_pAvatarNode->SetTransparent(alpha);
	m_fTransparent = alpha;
}

void Player::SetRoleState(DWORD dwValue)
{
	const DWORD dwOldState = m_roleState;
	m_roleState = dwValue;

	//// ̯λ�ı�
	//if ((dwOldState&ERS_Stall) != (m_roleState&ERS_Stall) )
	//{
	//	SendEventStallAppear( (m_roleState&ERS_Stall) != 0 );
	//}

	// ����״̬�ı���Ϸ�¼�
	if( dwOldState != m_roleState )
		SendEventRoleStateChange( dwOldState );
}

void Player::SetRoleState( ERoleState eType, bool bFlag )
{
	const DWORD dwOldState = m_roleState;

	if (bFlag)
		m_roleState |= eType;
	else
		m_roleState ^= eType;
	
	//if (eType == ERS_Stall)// ��̯�ı���Ҫ����Ϸ�¼�
	//{
	//	SendEventStallAppear( bFlag );
	//}

	// ����״̬�ı���Ϸ�¼�
	if( dwOldState != m_roleState )
	{
		SendEventRoleStateChange( dwOldState );
	}
}

void Player::SendEventStallAppear( bool bAppear, BYTE byStallLevel )
{
	if (bAppear)
	{
		tagStallAppearEvent event(_T("StallEvent_StallAppear"), NULL);
		event.dwRoleID = m_id;
		event.vPos = m_space.GetPos();
		event.fYaw = m_space.GetYaw();
		event.byStallLevel = byStallLevel;
		TObjRef<GameFrameMgr>()->SendEvent( &event );
	}
	else
	{
		tagStallAppearEvent event(_T("StallEvent_StallDisappear"), NULL);
		event.dwRoleID = m_id;
		event.vPos = m_space.GetPos();
		event.fYaw = m_space.GetYaw();
		event.byStallLevel = byStallLevel;
		TObjRef<GameFrameMgr>()->SendEvent(&event);
	}
}

void Player::SendEventRoleStateChange(const DWORD dwOldState)
{
	if( RoleMgr::Inst()->GetLocalPlayerID() == m_id )
	{
		tagLPRoleStateChangeEvent evt( _T("tagLPRoleStateChangeEvent"), NULL );
		evt.dwOldState = dwOldState;
		TObjRef<GameFrameMgr>()->SendEvent(&evt);
	}
	else
	{
		tagRPRoleStateChangeEvent evt( _T("tagRPRoleStateChangeEvent"), NULL );
		evt.dwRoleID = m_id;
		evt.dwOldState = dwOldState;
		TObjRef<GameFrameMgr>()->SendEvent(&evt);
	}
}

const tagDisplaySet& Player::GetDisplaySet()
{
	ASSERT( P_VALID( m_pAvatarNode ) );
	return m_pAvatarNode->GetDisplaySet();
}

void Player::SetDisplaySet( const tagDisplaySet& ds )
{
	ASSERT( P_VALID( m_pAvatarNode ) );
	if( 0 == memcmp( &GetDisplaySet(), &ds, sizeof(ds) ) )
		return;
	m_pAvatarNode->SetDisplaySet( ds );

	// ������ʾ���ø�����Ϸ�¼�
	tagUpdataAvatarDisplaySetEvent evt( _T("tagUpdataAvatarDisplaySetEvent"), NULL );
	evt.dwRoleID = m_id;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);

	// ��������һ��͸����
	if(m_pAvatarNode)
		m_pAvatarNode->SetTransparent(m_fTransparent);
}

void Player::SetPos( const Vector3& pos )
{
	m_space.SetPos(pos);
}

void Player::SetYaw( float angle,bool bShade/*=true*/ )
{
	if(bShade)
		m_rotaterYaw.Start(angle);
	else
		m_space.SetYaw(angle);
}

float Player::GetMoveSpeed()
{
	if( P_VALID(m_pMount) )
		return Role::GetMoveSpeed(TRUE);
	else
		return Role::GetMoveSpeed(FALSE);
}

bool Player::CalRideMat( Pet *pMount, Matrix4& outRide, UINT index )
{
	if( pMount->GetAniCtrl() == NULL
		|| pMount->GetAniCtrl()->GetRes() == NULL
		|| !pMount->GetAniCtrl()->GetRes()->IsCreated() )
	{
		return false;
	}

	tstring strTag;
	if( index == 0 )//����
		strTag = _T("tag_Ride");
	else if( index == 1 )//����
		strTag = _T("tag_R2nd");
	else
		return false;

	// �ҳ����ص���ת��λ�Ʊ任����
	Matrix4 matTag;
	AnimationCtrl* pAniCtrl = pMount->GetAniCtrl();
	if( pAniCtrl->IS_STATIC_CLASS(NodeAnimationCtrl) )//��������
	{
		NodeAnimationCtrl *pNodeAni = (NodeAnimationCtrl*)pAniCtrl;
		matTag = *pNodeAni->GetBoneSpace( strTag.c_str() );
		matTag._11 = 1.0f;	matTag._12 = 0.0f;	matTag._13 = 0.0f;
		matTag._21 = 0.0f;	matTag._22 = 1.0f;	matTag._23 = 0.0f;
		matTag._31 = 0.0f;	matTag._32 = 0.0f;	matTag._33 = 1.0f;
	}
	else if( pAniCtrl->IS_STATIC_CLASS(VertexAnimationCtrl) )//�ؼ�֡����
	{
		VertexAnimationCtrl *pVertexAni = (VertexAnimationCtrl*)pAniCtrl;
		if( !pVertexAni->GetTagSpace( strTag.c_str(), matTag ) )//������ride���ص�
		{
			return false;
		}
	}

	if( index == 0 )
	{
		matTag._41 *= pMount->GetObjectSpace().GetScale().x;
		matTag._42 *= pMount->GetObjectSpace().GetScale().y;
		matTag._43 *= pMount->GetObjectSpace().GetScale().z;

		// ����������������
		{
			Transform trans(Transform::ECM_ToRight);
			// ����
			trans.Scale( m_space.GetScale().x, m_space.GetScale().y, m_space.GetScale().z );
			// ��ת��λ��
			trans.CombineTransRight( &matTag );
			// λ��
			trans.Translate( Vector3( 0, -GetBunsHeight()*m_space.GetScale().y , 0 ) );
			// ��ת(�¶�)
			trans.Rotate( m_space.GetYaw(), pMount->GetPitch(), 0 );
			// ����λ��
			trans.Translate( m_space.GetPos() );

			outRide = *trans.GetMatrix();
		}
	}
	else
	{
		Role *pRole = NULL;
		if( pMount->GetOwnerID() == RoleMgr::Inst()->GetLocalPlayerID() )
		{
			pRole = (Role*)RoleMgr::Inst()->GetLocalPlayer();
		}
		else
		{
			const map<DWORD,Role*>& roleMap = RoleMgr::Inst()->GetRoleMap();
			map<DWORD,Role*>::const_iterator finder = roleMap.find( pMount->GetOwnerID() );
			if( finder != roleMap.end() )
			{
				pRole = finder->second;
			}
		}

		if( P_VALID(pRole) )
		{
			m_space.SetPos( pRole->GetObjectSpace().GetPos() );
			m_space.SetYaw( pRole->GetObjectSpace().GetYaw() );

			matTag._41 *= pMount->GetObjectSpace().GetScale().x;
			matTag._42 *= pMount->GetObjectSpace().GetScale().y;
			matTag._43 *= pMount->GetObjectSpace().GetScale().z;

			// ����������������
			{
				Transform trans(Transform::ECM_ToRight);
				// ����
				trans.Scale( m_space.GetScale().x, m_space.GetScale().y, m_space.GetScale().z );
				// ��ת��λ��
				trans.CombineTransRight( &matTag );
				// λ��
				trans.Translate( Vector3( 0, -GetBunsHeight()*m_space.GetScale().y , 0 ) );
				// ��ת(�¶�)
				trans.Rotate( m_space.GetYaw(), pMount->GetPitch(), 0 );
				// ����λ��
				trans.Translate( m_space.GetPos() );

				outRide = *trans.GetMatrix();
			}
		}
	}
	pMount->SetPos( m_space.GetPos() );
	pMount->SetYaw( m_space.GetYaw(), false );
	pMount->NPC::Update();

	return true;
}

float Player::GetBunsHeight( void )
{
	float height = 70.0f;

	if( P_VALID(m_pAvatarNode) )
	{
		if( m_bunsBoneIndex == -1 )
		{
			ResSkeleton *pSkel = const_cast<ResSkeleton*>(((AvatarNode*)m_pAvatarNode)->GetResource());
			if( P_VALID(pSkel) && pSkel->IsCreated() )
			{
				m_bunsBoneIndex = pSkel->GetBoneID( _T("Bip01 Pelvis") );
			}
		}

		if( m_bunsBoneIndex != -1 )
		{
			NodeAnimationCtrl *pAniCtrl = ((AvatarNode*)m_pAvatarNode)->GetSkeletonAniCtrl();
			if( P_VALID(pAniCtrl) )
			{
				float y = pAniCtrl->GetBoneSpace(m_bunsBoneIndex)->_42;
				if( y > 0 )
				{
					height = y - 10.0f;
				}
			}
		}
	}

	return height;
}

EActionState Player::GetActionState()
{
	return m_pFSM->GetActionState();
}

VOID Player::FaceTo( const Vector3& pos, BOOL bFaceRound )
{
	/*
		��yaw��һ����Χʱ��ת����������ת��ȫ��
		���� 25 0.436 �¿� -0.262 ���� 35 70 1.222f
	*/
	NodeAnimationCtrl *pNodeAniCtrl = GetAniCtrl();
	if( !P_VALID(pNodeAniCtrl) )
		return;

	Vector3 headPos;
	GetHeadPos(headPos);

	Vector3 dir = pos - headPos;
	float newYaw = CalcYaw(dir);
	float newPitch = CalcPitch(dir);

	// ����һ�����������������ٽ����Ƶ���л�
	if( fabsf(newYaw-m_lastYaw) < 0.0436f )
		return;
	else
		m_lastYaw = newYaw;

	float oldYaw;
	if( m_rotaterYaw.IsRotating() )//��������ת�������У�����Ҫȡ����yaw
		oldYaw = m_rotaterYaw.GetDestAngle();
	else
		oldYaw = GetYaw();
	float deltaYaw = newYaw - oldYaw;

	// ���ת����С��yaw
	if( deltaYaw > 3.14f )
		deltaYaw = -(FLOAT_PI*2.0f - deltaYaw);
	else if( deltaYaw < -3.14f )
		deltaYaw = FLOAT_PI*2.0f + deltaYaw;

	if( fabsf(newPitch-m_lastPitch) < 0.0436f )
		newPitch = m_lastPitch;
	else
		m_lastPitch = newPitch;

	// ����ͷ����pitch
	if( newPitch > 0.436f && newPitch < 3.14f )
	{
		newPitch = 0.436f;
	}
	else if( newPitch > 3.14f && newPitch < 6.018f )
	{
		newPitch = 6.018f;
	}

	if( bFaceRound == FALSE )//ת������
	{
		if( deltaYaw>=-1.222f && deltaYaw<=1.222f )
		{
			deltaYaw /= 2.0f;
		}
		else
		{
			deltaYaw = 0.0f;
			newPitch = 0.0f;
		}
		m_upperRotater.Start( deltaYaw );
		m_headRotater.Start( deltaYaw, newPitch );

	}
	else//ת������
	{
		if( deltaYaw>=-1.222f && deltaYaw<=1.222f )//ֻ��Ҫת������
		{
			deltaYaw /= 2.0f;
			m_upperRotater.Start( deltaYaw );
			m_headRotater.Start( deltaYaw, newPitch );
		}
		else
		{
			SetYaw(newYaw);
			m_headRotater.Start( 0.0f, newPitch );
		}
	}
}

VOID Player::FaceBack( void )
{
	NodeAnimationCtrl *pNodeAniCtrl = GetAniCtrl();
	if( P_VALID(pNodeAniCtrl) )
	{
		m_lastYaw = 0.0f;
		m_lastPitch = 0.0f;
		m_upperRotater.Start( 0.0f );
		m_headRotater.Start( 0.0f, 0.0f );
	}
}

void Player::SetPlayerTitle( DWORD dwTitleID )
{
    m_dwTitleID     =   dwTitleID;
    m_strTitleName  =   RoleTitleProtoData::Inst()->GetTitleNamebyID(dwTitleID);
}


void Player::SetStallLevel(BYTE byLevel)
{
	m_byStallLevel = byLevel;

}

void Player::ReplayLowerAni()
{
	m_pFSM->ReplayLowerAni();
}

void Player::OnMsgCode( const DWORD dwMsgCode )
{
	//--����/ֹͣ����
	if( _MakeFourCC( _T("bf") ) == dwMsgCode )
	{
		m_pAvatarNode->PlayBladeFlare( true );
	}
	else if( _MakeFourCC( _T("bfe") ) == dwMsgCode ||
		     _MakeFourCC( _T("BGN") ) == dwMsgCode )
	{
		m_pAvatarNode->PlayBladeFlare( false );
	}
	//--���Ż�������Ч
	else if( _MakeFourCC( _T("patk") ) == dwMsgCode )
	{
		CombatSysUtil::Inst()->PlayWeaponSound( this );
	}
}

void Player::SetMount( Pet *pPet )
{
	m_pMount = pPet;

	if( P_VALID(m_pMount) )
	{
		float destYaw;
		if( m_rotaterYaw.IsRotating() )//��������ת�������У�����Ҫȡ����yaw
			destYaw = m_rotaterYaw.GetDestAngle();
		else
			destYaw = m_space.GetYaw();
		m_pMount->SetYaw( destYaw ) ;
	}
}

void Player::SetRebornPoint( DWORD dwMapID, Vector3& dwRebornPoint )
{
    m_dwRebornMapID     = dwMapID;
    m_vRebornPoint      = dwRebornPoint;
}

tstring Player::GetRebornPoinName()
{
    if (GT_INVALID == m_dwRebornMapID)
        return _T("");

    return MapLogicData::Inst()->FindMapShowName(m_dwRebornMapID);
}

void Player::SetPlayerLoverID( DWORD dwLoverID )
{
    m_dwLoverID = dwLoverID;
    PlayerNameTab::Inst()->FindNameByID(dwLoverID);
}

void Player::SetTransportPoint( DWORD dwMapID, Vector3& dwPoint )
{
    m_dwTransportID     = dwMapID;
    m_vTransportPoint   = dwPoint;
}

tstring Player::GetTransportPointName()
{
    if (GT_INVALID == m_dwTransportID)
        return _T("");

    return MapLogicData::Inst()->FindMapShowName(m_dwTransportID);
}

Vector3 Player::GetTransportPoint()
{
    return m_vTransportPoint / 50;
}

void Player::SwitchMountPitch(bool bPitchEnabled)
{
    Pet* pPet = GetMount();
    if (P_VALID(pPet))
    {
        pPet->SetPetPitch(bPitchEnabled);
        pPet->SetPitch(0.0f);
    }
}

