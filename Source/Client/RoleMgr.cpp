#include "StdAfx.h"
#include "RoleMgr.h"
#include "Role.h"
#include "LocalPlayer.h"
#include "NPC.h"
#include "Door.h"
#include "Pet.h"
#include "PetManager.h"
#include "MapMgr.h"
#include "RoleEvent.h"
#include "CreatureData.h"
#include "PetProtoData.h"
#include "ShadowPool.h"
#include "SGDynamicProjector.h"
#include "..\WorldDefine\exchange_define.h"
#include "OnlineTips.h"
#include "PetEvent.h"
#include "WorldPickerFrame.h"
#include "CombatSys.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\msg_map.h"
#include "RoleLevelUpPromptFrame.h"
#include "..\WorldDefine\selectrole.h"

const INT32 MAX_EXCHANGE_DIST = 10 * TILE_SCALE;	// ��ҽ�������������ң�NPC��̯λ���������Զ����
const INT32 MAX_FACE_DIST_SQ = 400 * TILE_SCALE * TILE_SCALE;
const INT32 MIN_FACE_DIST_SQ = 1 * TILE_SCALE * TILE_SCALE;

RoleMgr::RoleMgr(void):m_trunk(this)
{
	m_pLocalPlayer=NULL;
	m_pShadowPool=NULL;
	m_bEnalbeShadow=FALSE;
	
	m_paReqRole.first = 0;
	m_paReqRole.second.clear();

	m_bHideOtherPlayers = false;
	m_viewDistLevel = 0;
	m_fViewDist = 0.0f;
}

RoleMgr::~RoleMgr(void)
{
}

RoleMgr g_roleMgr;
RoleMgr* RoleMgr::Inst()
{
	return &g_roleMgr;
}

void RoleMgr::Init(DWORD localPlayerID)
{
	m_pCmdMgr = TObjRef<NetCmdMgr>();
	m_pSession = TObjRef<NetSession>();
    m_pFrameMgr = TObjRef<GameFrameMgr>();
	m_pUtil	= TObjRef<Util>();

	//Destroy();
	
	//--����������ҽ�ɫ
	m_pLocalPlayer=new LocalPlayer;
	m_pLocalPlayer->SetID(localPlayerID);
	m_pLocalPlayer->InitFSM();

	//--ע��������Ϣ������
	m_pCmdMgr->Register("NS_LPRename",					(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetRename),				_T("NS_LPRename"));
	m_pCmdMgr->Register("NS_GetRoleInitState_Att",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetInitLocalPlayerAtt),		_T("NS_GetRoleInitState_Att"));
	m_pCmdMgr->Register("NS_GetRemoteRoleState",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetGetRemoteRoleState),		_T("NS_GetRemoteRoleState"));
	m_pCmdMgr->Register("NS_GetRemoteCreatureState",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetGetRemoteCreatureState),	_T("NS_GetRemoteCreatureState"));
	m_pCmdMgr->Register("NS_RemoveRemote",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoveRemote),			_T("NS_RemoveRemote"));
	m_pCmdMgr->Register("NS_RoleAttChangeSingle",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleAttChangeSingle),		_T("NS_RoleAttChangeSingle"));
	m_pCmdMgr->Register("NS_RoleAttChangeMutiple",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleAttChangeMutiple),	_T("NS_RoleAttChangeMutiple"));
	m_pCmdMgr->Register("NS_RemoteAttChangeSingle",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoteAttChangeSingle),	_T("NS_RemoteAttChangeSingle"));
	m_pCmdMgr->Register("NS_RemoteAttChangeMutiple",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoteAttChangeMutiple),	_T("NS_RemoteAttChangeMutiple"));
	m_pCmdMgr->Register("NS_AvatarEquipChange",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetAvatarEquipChange),		_T("NS_AvatarEquipChange"));
	m_pCmdMgr->Register("NS_AvatarEquip",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetAvatarEquip),				_T("NS_AvatarEquip"));
	m_pCmdMgr->Register("NS_AddRoleBuff",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetAddRoleBuff),				_T("NS_AddRoleBuff"));
	m_pCmdMgr->Register("NS_RemoveRoleBuff",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoveRoleBuff),			_T("NS_RemoveRoleBuff"));
	m_pCmdMgr->Register("NS_UpdateRoleBuff",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetUpdateRoleBuff),			_T("NS_UpdateRoleBuff"));
	m_pCmdMgr->Register("NS_RoleAttPointAddChange",	    (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleAttPointAddChange),	_T("NS_RoleAttPointAddChange"));
	m_pCmdMgr->Register("NS_RoleBidAttPoint",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleBidAttPoint),			_T("NS_RoleBidAttPoint"));
	m_pCmdMgr->Register("NS_RoleClearAttPoint",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleClearAttPoint),		_T("NS_RoleClearAttPoint"));
	m_pCmdMgr->Register("NS_SetRoleStateEx",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetSetRoleStateEx),			_T("NS_SetRoleStateEx"));
	m_pCmdMgr->Register("NS_UnSetRoleStateEx",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetUnsetRoleStateEx),		_T("NS_UnSetRoleStateEx"));
	m_pCmdMgr->Register("NS_RoleSetDisplay",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleSetDisplay),			_T("NS_RoleSetDisplay"));
	m_pCmdMgr->Register("NS_SetPersonalSet",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetSetPersonalSet),			_T("NS_SetPersonalSet"));
    m_pCmdMgr->Register("NS_GetRemotePetState",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetGetRemotePetState),		_T("NS_GetRemotePetState"));
	m_pCmdMgr->Register("NS_EquipEffectChange",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetEquipEffectChange),		_T("NS_EquipEffectChange"));
	m_pCmdMgr->Register("NS_SuitEffect",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetSuitEffectChange),		_T("NS_SuitEffect"));
    m_pCmdMgr->Register("NS_RoleTitleChangeBroadcast",  (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleTitleChange),		    _T("NS_RoleTitleChangeBroadcast"));
	m_pCmdMgr->Register("NS_RemoteRoleGuildInfoChange", (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoteGuildInfoChange),	_T("NS_RemoteRoleGuildInfoChange"));
	m_pCmdMgr->Register("NS_CreatureTaggedOwnerChange", (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetCreatureTaggedOwnerChange),	_T("NS_CreatureTaggedOwnerChange"));
    m_pCmdMgr->Register("NS_SetTransportMap",           (NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_SetTransportMap),	    _T("NS_SetTransportMap"));
	m_pCmdMgr->Register("NS_RoleLevelChange",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_RoleLevelChange),		_T("NS_RoleLevelChange"));
	m_pCmdMgr->Register("NS_RoleRename", (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleRenameBroadcast), _T("NS_RoleRename") );

    // ע����Ϸ�¼�
    m_pFrameMgr->RegisterEventHandle(_T("tagEnterMount"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnGetPetMount));
    m_pFrameMgr->RegisterEventHandle(_T("tagLeaveMount"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnLeavePetMount));
	m_pFrameMgr->RegisterEventHandle(_T("tagLPRoleStateChangeEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnLpChangeStateEvent));

	// ע������/��ʾ��������ȼ���Ӧ����
	TObjRef<GameInputMap>()->Register(0, g_StrTable[_T("Hotkey_HidePlayer")], (INPUTMAPEVENTHANDLE)m_trunk.sfp2(&RoleMgr::OnHideOtherPlayers), DIK_F9, TRUE, TRUE, 1);

	// ��ʼ����ɫ������ʱ��
	m_paReqRole.first = 0;
}

void RoleMgr::Destroy()
{
	//--�ͷű�����ҽ�ɫ
	SAFE_DELETE(m_pLocalPlayer);

	//--�ͷ�������ɫ
	for(map<DWORD,Role*>::iterator iter=m_roleMap.begin();
		iter!=m_roleMap.end();++iter)
	{
		Role* pRole=iter->second;
		delete pRole;
	}
	m_roleMap.clear();

	//--�ͷ���������
	for(list<Role*>::iterator iter=m_listDeadCreature.begin();
		iter!=m_listDeadCreature.end();++iter)
	{
		Role* pRole=(*iter);
		delete pRole;
	}
	m_listDeadCreature.clear();

	SAFE_DELETE( m_pShadowPool );

	//--��������б�
	m_paReqRole.first = 0;
	m_paReqRole.second.clear();

	m_bHideOtherPlayers = false;

	//--ע��������Ϣ������
	m_pCmdMgr->UnRegister("NS_LPRename",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNetRename));
	m_pCmdMgr->UnRegister("NS_GetRoleInitState_Att",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetInitLocalPlayerAtt));
	m_pCmdMgr->UnRegister("NS_GetRemoteRoleState",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetGetRemoteRoleState));
	m_pCmdMgr->UnRegister("NS_GetRemoteCreatureState",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetGetRemoteCreatureState));
	m_pCmdMgr->UnRegister("NS_RemoveRemote",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoveRemote));
	m_pCmdMgr->UnRegister("NS_RoleAttChangeSingle",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleAttChangeSingle));
	m_pCmdMgr->UnRegister("NS_RoleAttChangeMutiple",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleAttChangeMutiple));
	m_pCmdMgr->UnRegister("NS_RemoteAttChangeSingle",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoteAttChangeSingle));
	m_pCmdMgr->UnRegister("NS_RemoteAttChangeMutiple",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoteAttChangeMutiple));
	m_pCmdMgr->UnRegister("NS_AvatarEquipChange",	    (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetAvatarEquipChange));
	m_pCmdMgr->UnRegister("NS_AvatarEquip",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetAvatarEquip));
	m_pCmdMgr->UnRegister("NS_AddRoleBuff",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetAddRoleBuff));
	m_pCmdMgr->UnRegister("NS_RemoveRoleBuff",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoveRoleBuff));
	m_pCmdMgr->UnRegister("NS_UpdateRoleBuff",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetUpdateRoleBuff));
	m_pCmdMgr->UnRegister("NS_RoleAttPointAddChange",	(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleAttPointAddChange));
	m_pCmdMgr->UnRegister("NS_RoleBidAttPoint",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleBidAttPoint));
	m_pCmdMgr->UnRegister("NS_RoleClearAttPoint",		(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleClearAttPoint));
	m_pCmdMgr->UnRegister("NS_SetRoleStateEx",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetSetRoleStateEx));
	m_pCmdMgr->UnRegister("NS_UnSetRoleStateEx",	    (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetUnsetRoleStateEx));
	m_pCmdMgr->UnRegister("NS_RoleSetDisplay",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleSetDisplay));
	m_pCmdMgr->UnRegister("NS_SetPersonalSet",	        (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetSetPersonalSet));
    m_pCmdMgr->UnRegister("NS_GetRemotePetState",	    (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetGetRemotePetState));
	m_pCmdMgr->UnRegister("NS_EquipEffectChange",	    (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetEquipEffectChange));
	m_pCmdMgr->UnRegister("NS_SuitEffect",				(NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetSuitEffectChange));
	m_pCmdMgr->UnRegister("NS_RoleTitleChangeBroadcast",  (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleTitleChange));
	m_pCmdMgr->UnRegister("NS_RemoteRoleGuildInfoChange", (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRemoteGuildInfoChange));
	m_pCmdMgr->UnRegister("NS_CreatureTaggedOwnerChange", (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetCreatureTaggedOwnerChange));
    m_pCmdMgr->UnRegister("NS_SetTransportMap",           (NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_SetTransportMap));
	m_pCmdMgr->UnRegister("NS_RoleLevelChange",			(NETMSGPROC)m_trunk.sfp2(&RoleMgr::OnNS_RoleLevelChange));
	m_pCmdMgr->UnRegister( "NS_RoleRename", (NETMSGPROC)m_trunk.sfp2(&RoleMgr::NetRoleRenameBroadcast) );


    // ע����Ϸ�¼�
    m_pFrameMgr->UnRegisterEventHandler(_T("tagEnterMount"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnGetPetMount));
    m_pFrameMgr->UnRegisterEventHandler(_T("tagLeaveMount"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnLeavePetMount));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagLPRoleStateChangeEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&RoleMgr::OnLpChangeStateEvent));

	TObjRef<GameInputMap>()->SetEnable(g_StrTable[_T("Hotkey_HidePlayer")], false);
}

void RoleMgr::OnOpenMap(SceneGraph* pSG, const Vector3& localPlayerPos, const Vector3& localPlayerFaceto)
{
	//--���������SceneNode��ӵ�SceneGraph������Ⱦ
	if( NULL != m_pLocalPlayer && NULL != m_pLocalPlayer->GetSceneNode() )
		m_pLocalPlayer->AttachSceneNodeToSG(pSG);

	//--���ý�ɫλ�á������״̬
	m_pLocalPlayer->OnGotoMap(localPlayerPos,localPlayerFaceto);
}

void RoleMgr::OnCloseMap(SceneGraph* pSG)
{
	//--����SceneGraph��ǰ�Ƚ��������SceneNode�����Ƴ�
	if( NULL != m_pLocalPlayer->GetSceneNode() )
		m_pLocalPlayer->DetachSceneNodeFromSG(pSG);

	//--�ͷ����н�ɫ
	for(map<DWORD,Role*>::iterator iter=m_roleMap.begin();
		iter!=m_roleMap.end();++iter)
	{
		Role* pRole=iter->second;
		if( NULL != pRole->GetSceneNode() )
			pRole->DetachSceneNodeFromSG(pSG);
		delete pRole;
	}
	m_roleMap.clear();

	//--�ͷ���������
	for(list<Role*>::iterator iter=m_listDeadCreature.begin();
		iter!=m_listDeadCreature.end();++iter)
	{
		Role* pRole=(*iter);
		if( NULL != pRole->GetSceneNode() )
			pRole->DetachSceneNodeFromSG(pSG);
		delete pRole;
	}
	m_listDeadCreature.clear();

	//--��������б�
	m_paReqRole.first = 0;
	m_paReqRole.second.clear();
}

DWORD RoleMgr::OnHideOtherPlayers( DWORD dwID, BOOL bDown )
{
	if( bDown )
		m_bHideOtherPlayers = !m_bHideOtherPlayers;
	return 0;
}

Role* RoleMgr::FindRole( DWORD roleID,bool bAddReqIfNotFind/*=false*/,bool bFindDeadCreatureList/*=false*/ )
{
	if(roleID==m_pLocalPlayer->GetID())
		return m_pLocalPlayer;

	map<DWORD,Role*>::iterator iter=m_roleMap.find(roleID);
	if(iter!=m_roleMap.end())
	{
		return iter->second;
	}

	if(bAddReqIfNotFind)
	{
		Role* pRole=NULL;
		if(IS_PLAYER(roleID))
		{
			Player* pPlayer=new Player;
			pPlayer->SetID(roleID);
			pPlayer->InitFSM();	
			pRole=pPlayer;
		}
		else if (IS_CREATURE(roleID))
		{
			ASSERT(IS_CREATURE(roleID));
			if(IS_NORMAL_CREATURE(roleID))
			{
				NPC* pNPC=new NPC;
				pNPC->SetID(roleID);
				pNPC->InitFSM();
				pRole=pNPC;
			}
			else
			{
				ASSERT(IS_DOOR(roleID));
				Door* pDoor=new Door;
				pDoor->SetID(roleID);
				pRole=pDoor;
			}
		}
        else if (IS_PET(roleID))
        {
            Pet* pPet = new Pet();
            pPet->SetID(roleID);
            pPet->InitFSM();
            pRole = pPet;
        }
        else
        {
        }
        
        if (P_VALID(pRole))
            m_roleMap.insert(make_pair(roleID, pRole));
        else
            ASSERT(P_VALID(pRole));
		
		AddReqRole(roleID);
		return pRole;
	}
	
	if( bFindDeadCreatureList )
	{
		list<Role*>::iterator it = m_listDeadCreature.begin();
		for (; it != m_listDeadCreature.end(); ++it)
		{
			if (((Role*)(*it))->GetID() == roleID)
				return *it;
		}
	}

	return NULL;
}

void RoleMgr::FreeRole(DWORD roleID,SceneGraph* pSG)
{
	if(roleID == m_pLocalPlayer->GetID())
		return;

	map<DWORD,Role*>::iterator iter=m_roleMap.find(roleID);
	if(iter!=m_roleMap.end())
	{
		Role* pRole=iter->second;

		// ������û�ж��������ʵ���������������Ҫɾ������
		if( pRole->IS_STATIC_CLASS(Player) )
		{
			Player *pPlayer = static_cast<Player*>(pRole);
			Pet *pPet = pPlayer->GetMount();
			if( P_VALID( pPet ) )
			{
				if( pPet->GetRidersCount() <= 1 )
				{
					if( NULL != pPet->GetSceneNode() )
						pPet->DetachSceneNodeFromSG(pSG);
					m_roleMap.erase( pPet->GetID() );
					delete pPet;
				}
				else
				{
					pPet->RidersCountDec();
				}
			}
		}

		if( NULL != pRole->GetSceneNode() )
			pRole->DetachSceneNodeFromSG(pSG);
		delete pRole;
		m_roleMap.erase(iter);
	}
}

void RoleMgr::Update( SceneGraph* pSG )
{
	// �������б�Ϊ�գ��Ҵ���ʱ�����500����ʱ������������ͽ�ɫ��������
	if( !m_paReqRole.second.empty()
		&& 500 < Kernel::Inst()->GetAccumTimeDW() - m_paReqRole.first)
		SendReqRoleList();

	m_pLocalPlayer->Update();
	UpdataHightLight( m_pLocalPlayer );

	for(map<DWORD,Role*>::iterator iter=m_roleMap.begin();
		iter!=m_roleMap.end();)
	{
		Role* pRole=iter->second;
		if( P_VALID(pRole) )
		{
			// ���������ƶ������������б�
			if( IS_CREATURE(iter->first) && pRole->IsDead() )
			{
				m_listDeadCreature.push_back(pRole);
				iter = m_roleMap.erase(iter);
				continue;
			}

			pRole->Update();

			// ����/��ʾ��ɫ
			const bool bHide = GetRoleHide( pRole );
			pRole->Hide( bHide );

			UpdataHightLight( pRole );
		}
		++iter;
	}

	// ����������ɫ
	for(list<Role*>::iterator iter=m_listDeadCreature.begin();iter!=m_listDeadCreature.end();)
	{
		Role* pRole=(*iter);
		if( P_VALID(pRole) )
		{
			pRole->Update();

			// �ͷ�������ɫ
			if( pRole->IsNeedToFree() )
			{
				if( NULL != pRole->GetSceneNode() )
					pRole->DetachSceneNodeFromSG(pSG);
				delete pRole;
				iter = m_listDeadCreature.erase(iter);
				continue;
			}

			// ����/��ʾ��ɫ
			const bool bHide = GetRoleHide( pRole );
			pRole->Hide( bHide );
		}
		iter++;
	}

	// ��ɫ��Ӱ
	UpdataRoleShadow();
}

DWORD RoleMgr::PickRole( const Ray& mouseRay, const BOOL bIgnorePlayer )
{
	DWORD dwRoleID = GT_INVALID;
	float fMinDistSq = FLOAT_MAX;

	for(map<DWORD,Role*>::iterator iter=m_roleMap.begin();
		iter!=m_roleMap.end();iter++)
	{
		
		if (iter->second->IS_STATIC_CLASS(Door))
		{
			// ����Ǵ��ŵ�״̬
			if (((Door*)(iter->second))->GetDoorState() == true)
				continue;
		}

		SceneNode* pSGNode=iter->second->GetSceneNode();
		if(pSGNode==NULL)
			continue;

		if( !iter->second->IsAttributeInited() )
			continue;

		if( iter->second->IsHide() )
			continue;

		if( IS_PLAYER( iter->first ) && bIgnorePlayer )
			continue;

		// ������ҵĳ��Զ����ҵĳ�������״̬������Զ�����---���Ըó���
		if( iter->second->IS_STATIC_CLASS(Pet) )
		{
			Pet* pPet = (Pet*)iter->second;
			if( pPet->GetOwnerID() == m_pLocalPlayer->GetID() || pPet->IsFSMEnabled() || bIgnorePlayer )
				continue;
		}

		AABBox box;
		pSGNode->GetBox(box);
		TResult tr=mouseRay.Intersect(box);
		if(tr.first)
		{
			const float fDistSq = Vec3DistSq( mouseRay.origin, iter->second->GetPos() );
			if( fDistSq < fMinDistSq )
			{
				fMinDistSq = fDistSq;

				// ���������򷵻������ID
				if( iter->second->IS_STATIC_CLASS(Pet) )
				{
					Pet* pPet = (Pet*)iter->second;
					dwRoleID = pPet->GetOwnerID();
				}
				else
				{
					dwRoleID = iter->first;
				}
			}
		}
	}
	return dwRoleID;
}

DWORD RoleMgr::GetLocalPlayerID()
{
	return m_pLocalPlayer->GetID();
}

void RoleMgr::AddReqRole(DWORD dwRoleID)
{
	//--��������б��в����ڣ������
	bool bFind=false;
	for(list<DWORD>::iterator iter=m_paReqRole.second.begin();
		iter!=m_paReqRole.second.end();++iter)
	{
		if(*iter==dwRoleID)
		{
			bFind=true;
			break;
		}
	}
	if(!bFind)
	{
		m_paReqRole.second.push_back(dwRoleID);
	}
}

void RoleMgr::SendReqRoleList()
{
	BYTE byTemp[sizeof(tagNC_GetRemoteRoleState)+sizeof(DWORD)*50];
	tagNC_GetRemoteRoleState* pSend = (tagNC_GetRemoteRoleState*)byTemp;
	pSend->dwID = pSend->Crc32("NC_GetRemoteRoleState");
	pSend->nRoleNum = 0;
	while( !m_paReqRole.second.empty() )
	{
		pSend->dwRoleID[pSend->nRoleNum++] = m_paReqRole.second.front();
		m_paReqRole.second.pop_front();
		
		if( pSend->nRoleNum >= 50 )	// һ����ഫ50��ID
			break;
	}

	if( pSend->nRoleNum > 0 )
	{
		pSend->dwSize = sizeof(tagNC_GetRemoteRoleState)
			+ sizeof(DWORD) * (pSend->nRoleNum - 1);
		m_pSession->Send(pSend);
		m_paReqRole.first = Kernel::Inst()->GetAccumTimeDW();
	}
}

DWORD RoleMgr::NetRemoveRemote( tagNS_RemoveRemote* pNetCmd, DWORD )
{
	int num = ( pNetCmd->dwSize - sizeof(tagNS_RemoveRemote) ) / sizeof(DWORD) + 1;
	for(int i=0;i<num;i++)
	{
		FreeRole(pNetCmd->dwRoleID[i],MapMgr::Inst()->GetSceneGraph());
	}

	return 0;
}

DWORD RoleMgr::NetInitLocalPlayerAtt( tagNS_GetRoleInitState_Att* pNetCmd, DWORD )
{
	if( P_VALID(m_pLocalPlayer) && NULL == m_pLocalPlayer->GetSceneNode() )
	{
		m_pLocalPlayer->LoadAvatar( pNetCmd->Avatar, pNetCmd->AvatarEquip, pNetCmd->DisplaySet );
		m_pLocalPlayer->AttachSceneNodeToSG( MapMgr::Inst()->GetSceneGraph() );
		m_pLocalPlayer->InitAttribute( pNetCmd );

		// ���ͳ�ʼ����Ϣ
		tagNC_GetRoleInitState m;
		m.eType = ERIT_Skill;
		m_pSession->Send(&m);
		
		// ����ȼ�С��40..��ʾonlinetips
		if (GetLocalPlayer()->GetRoleLevel() < 40)
		{
			OnlineTips *onlinetips = static_cast<OnlineTips*>(TObjRef<GameFrameMgr>()->GetFrame(_T("OnlineTips")));
			onlinetips->ShowWnd(TRUE);
		}

		// ������Ϸ�¼���onlinetipsˢ��
		TObjRef<GameFrameMgr>()->SendEvent(&tagGameEvent( _T("Refresh_Onlinetips"), NULL ) );

        // ������Ϸ�¼�ˢ��ְҵ
        TObjRef<GameFrameMgr>()->SendEvent(&tagGameEvent( _T("LocalPlayerSetClass"), NULL ) );
	}

	return 0;
}

DWORD RoleMgr::NetGetRemoteRoleState( tagNS_GetRemoteRoleState* pNetCmd, DWORD )
{
	Role* pRole = FindRole( pNetCmd->RoleData.dwID );
	if( P_VALID( pRole ) && pRole->IS_STATIC_CLASS(Player) && NULL == pRole->GetSceneNode() )
	{
		Player* pPlayer = (Player*)pRole;
		pPlayer->LoadAvatar( pNetCmd->RoleData.Avatar, pNetCmd->RoleData.AvatarEquip, pNetCmd->RoleData.sDisplaySet );
		pPlayer->AttachSceneNodeToSG( MapMgr::Inst()->GetSceneGraph() );
		pPlayer->InitAttribute( pNetCmd );
	}

	return 0;
}

DWORD RoleMgr::NetGetRemoteCreatureState( tagNS_GetRemoteCreatureState* pNetCmd, DWORD )
{
	Role* pRole = FindRole( pNetCmd->CreatureData.dwID );
	if( (P_VALID( pRole ) && pRole->IS_STATIC_CLASS(NPC) && NULL == pRole->GetSceneNode()) || 
		(P_VALID( pRole ) && pRole->IS_STATIC_CLASS(Door) && NULL == pRole->GetSceneNode()) )
	{
		NPC* pNPC = (NPC*)pRole;
		const tagCreatureProto* pProto = CreatureData::Inst()->FindNpcAtt( pNetCmd->CreatureData.dwTypeID );
		tstring  strFileName = CreatureData::Inst()->GetNpcMdlPath( pProto );
		pNPC->LoadEntity( strFileName.c_str() );
		if( pProto->eAnimationType == EAT_Skeleton )
		{
			int i=1;
			tstring szSkinPath = CreatureData::Inst()->GetNpcSkinPath(pProto,i++);
			IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
			DWORD fileSize = pIFS->GetSize(szSkinPath.c_str());
			while( 0 != fileSize && GT_INVALID != fileSize )
			{
				pNPC->LoadSkin(szSkinPath.c_str());
				szSkinPath = CreatureData::Inst()->GetNpcSkinPath(pProto,i++);
				fileSize = pIFS->GetSize(szSkinPath.c_str());
			}
			pNPC->UpdateBoundingBox();
		}

		pNPC->AttachSceneNodeToSG( MapMgr::Inst()->GetSceneGraph() );
		pNPC->InitAttribute( pNetCmd );
	}

	return 0;
}



DWORD RoleMgr::NetGetRemotePetState( tagNS_GetRemotePetState* pNetCmd, DWORD )
{
    Role* pRole = FindRole(pNetCmd->PetData.dwID);

    if (P_VALID(pRole) && pRole->IS_STATIC_CLASS(Pet) && NULL == pRole->GetSceneNode())
    {
        Pet* pPet = static_cast<Pet*>(pRole);
        pPet->InitPetAttribute(pNetCmd);
        const tagPetProto* petProto = pPet->GetProto();
        ASSERT(P_VALID(petProto));
        EPetModelType mdltype = EPMT_Follow;
        pPet->LoadEntity( petProto->strModelPath[mdltype].c_str());
        if (EAT_Skeleton == petProto->eAniType[mdltype])
        {
            for (list<tstring>::const_iterator it = petProto->lstSkinPath[mdltype].begin();
                it != petProto->lstSkinPath[mdltype].end(); ++it)
            {
                pPet->LoadSkin(it->c_str());
            }
            pPet->UpdateBoundingBox();
        }

        pPet->AttachSceneNodeToSG( MapMgr::Inst()->GetSceneGraph());
    }
    return 0;
}

DWORD RoleMgr::NetRoleAttChangeSingle( tagNS_RoleAttChangeSingle* pNetCmd, DWORD )
{
	m_pLocalPlayer->SetAttribute(pNetCmd->eType, pNetCmd->nValue, false);

	// ������Ϸ�¼���onlinetipsˢ��
	TObjRef<GameFrameMgr>()->SendEvent(&tagGameEvent( _T("Refresh_Onlinetips"), NULL ) );
	return 0;
}

DWORD RoleMgr::NetRoleAttChangeMutiple( tagNS_RoleAttChangeMutiple* pNetCmd, DWORD )
{
	for(INT i=0; i<pNetCmd->nNum; ++i)
	{
		m_pLocalPlayer->SetAttribute(pNetCmd->value[i].eType, pNetCmd->value[i].nValue, false);
	}

	// ������Ϸ�¼���onlinetipsˢ��
	TObjRef<GameFrameMgr>()->SendEvent(&tagGameEvent( _T("Refresh_Onlinetips"), NULL ) );
	return 0;
}

DWORD RoleMgr::NetRemoteAttChangeSingle( tagNS_RemoteAttChangeSingle* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if(P_VALID(pRole))
	{
		pRole->SetRemoteRoleAtt(pNetCmd->eType, pNetCmd->nValue, false);
	}

	return 0;
}

DWORD RoleMgr::NetRemoteAttChangeMutiple( tagNS_RemoteAttChangeMutiple* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if(P_VALID(pRole))
	{
		for(INT i=0; i<pNetCmd->nNum; ++i)
		{
			pRole->SetRemoteRoleAtt(pNetCmd->value[i].eType, pNetCmd->value[i].nValue, false);
		}
	}

	return 0;
}

DWORD RoleMgr::NetAvatarEquipChange( tagNS_AvatarEquipChange* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if(P_VALID(pRole) && pRole->IS_KIND_OF(Player) && pRole->IsAttributeInited() )
	{
		Player* pPlayer = (Player*)pRole;
		pPlayer->SetAvatarEquip( (EAvatarElement)pNetCmd->EquipDisplay.byDisplayPos, pNetCmd->EquipDisplay );
	}

	return 0;
}

DWORD RoleMgr::NetAvatarEquip(tagNS_AvatarEquip* pNetCmd, DWORD)
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if(P_VALID(pRole) && pRole->IS_KIND_OF(Player) && pRole->IsAttributeInited() )
	{
		Player* pPlayer = (Player*)pRole;
		tagDisplaySet displaySet = pPlayer->GetDisplaySet();
		displaySet.bFashionDisplay = pNetCmd->bFashion;
		pPlayer->SetDisplaySet( displaySet );
		pPlayer->UpdateAvatar( pPlayer->GetAvatarAtt(), pNetCmd->sAvatarEquip, MapMgr::Inst()->GetSceneGraph() );
	}

	return 0;
}

DWORD RoleMgr::NetEquipEffectChange(tagNS_EquipEffectChange* pNetCmd, DWORD)
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if(P_VALID(pRole) && pRole->IS_KIND_OF(Player) && pRole->IsAttributeInited() )
	{
		Player* pPlayer = (Player*)pRole;
		pPlayer->UpdataLongHunEffect( (EAvatarElement)pNetCmd->byDisplayPos, pNetCmd->byEquipEffect );
	}

	return 0;
}

DWORD RoleMgr::NetSuitEffectChange(tagNS_SuitEffect* pNetCmd, DWORD)
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if(P_VALID(pRole) && pRole->IS_KIND_OF(Player) && pRole->IsAttributeInited() )
	{
		Player* pPlayer = (Player*)pRole;
		pPlayer->UpdataSuitEffect( pNetCmd->dwSuitEffectID );
	}

	return 0;
}

DWORD RoleMgr::NetAddRoleBuff( tagNS_AddRoleBuff* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if(P_VALID(pRole))
	{
		pRole->AddRoleBuff(pNetCmd->Buff);
	}

	return 0;
}

DWORD RoleMgr::NetRemoveRoleBuff( tagNS_RemoveRoleBuff* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if(P_VALID(pRole))
	{
		pRole->RemoveRoleBuff(pNetCmd->dwBuffTypeID);
	}

	return 0;
}

DWORD RoleMgr::NetUpdateRoleBuff( tagNS_UpdateRoleBuff* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if(P_VALID(pRole))
	{
		pRole->UpdateRoleBuff(pNetCmd->Buff);
	}

	return 0;
}

DWORD RoleMgr::NetRoleAttPointAddChange( tagNS_RoleAttPointAddChange* pNetCmd, DWORD )
{
	for(INT i=0; i<X_ERA_ATTA_NUM; ++i)
	{
		m_pLocalPlayer->SetAttPointAdd((ERoleAttribute)i, pNetCmd->nAttPointAdd[i]);
	}
	
	return 0;
}

DWORD RoleMgr::NetRoleBidAttPoint( tagNS_RoleBidAttPoint* pNetCmd, DWORD )
{
	
	return 0;
}

DWORD RoleMgr::NetRoleClearAttPoint( tagNS_RoleClearAttPoint* pNetCmd, DWORD )
{
	
	return 0;
}

bool RoleMgr::IsOutValidDist( DWORD dwRoleID )
{
	Role* pRole = FindRole(dwRoleID);
	if (P_VALID(pRole))
	{
		if( Vec3DistSq(pRole->GetPos(), m_pLocalPlayer->GetPos()) > MAX_EXCHANGE_DIST*MAX_EXCHANGE_DIST )
			return TRUE;
		else
			return FALSE;
	}
	return TRUE;
}

bool RoleMgr::IsOutFaceDist( DWORD dwRoleID )
{
	Role* pRole = FindRole(dwRoleID);
	if (P_VALID(pRole))
	{
		if( Vec3DistSq(pRole->GetPos(), m_pLocalPlayer->GetPos()) > MAX_FACE_DIST_SQ
			|| Vec3DistSq(pRole->GetPos(), m_pLocalPlayer->GetPos()) < MIN_FACE_DIST_SQ )
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}

bool RoleMgr::IsAABBoxCollideRole( const AABBox &box, bool bOnlyNPC /*= true*/ )
{
	for(map<DWORD,Role*>::iterator iter=m_roleMap.begin();
		iter!=m_roleMap.end();++iter)
	{
		SceneNode* pSGNode=iter->second->GetSceneNode();
		if(pSGNode==NULL)
			continue;

		if( iter->second->IsHide() )
			continue;
		
		if( IS_PLAYER( iter->first ) && bOnlyNPC )
			continue;

        if (IS_PET(iter->first) && ((Pet*)(iter->second))->IsFSMEnabled())
            continue;

		AABBox other;
		pSGNode->GetBox(other);
		if(box.Intersects(other))
			return TRUE;
	}
	return FALSE;
}

VOID RoleMgr::UpdataRoleShadow()
{
	// ����Ӱ
	if( m_bEnalbeShadow && NULL == m_pShadowPool && SGDynamicProjector::IsHWSupport() )
	{
		m_pShadowPool=new ShadowPool;
		m_pShadowPool->CreateData();
	}

	// �ر���Ӱ
	if( !m_bEnalbeShadow && NULL != m_pShadowPool )
	{
		m_pLocalPlayer->DetachShadow();
		for(map<DWORD,Role*>::iterator iter=m_roleMap.begin();
			iter!=m_roleMap.end();++iter)
		{
			Role* pRole=iter->second;
			if( P_VALID(pRole) )
				pRole->DetachShadow();
		}
		SAFE_DELETE( m_pShadowPool );
	}

	// ������Ӱ
	if( P_VALID( m_pShadowPool ) )
		m_pShadowPool->AssignToRoles();
}

void RoleMgr::SetViewDistLevel( const int viewDistLevel )
{
	ASSERT( viewDistLevel >= 0 && viewDistLevel <= MAX_VIEW_DIST );
	m_viewDistLevel = viewDistLevel;
	const float fMaxViewDist = VIS_DIST * 2 * TILE_SCALE;
	const float fMinViewDist = 32 * TILE_SCALE;
	const float fStep = ( fMaxViewDist - fMinViewDist ) / MAX_VIEW_DIST;
	m_fViewDist = fMinViewDist + fStep * m_viewDistLevel;
}

bool RoleMgr::GetRoleHide( Role* pRole )
{
	// ��ɫ��Ұ֮��
	if( !IsInViewDist( pRole ) )
		return true;

	// ��̯��ң�������
	if( pRole->IS_KIND_OF(Player) )
	{
		Player* pPlayer = (Player*)pRole;
		if( pPlayer->GetRoleState( ERS_Stall ) )
			return false;
	}

	// ����������ң�F9��
	if( IS_PLAYER( pRole->GetID() ) && m_bHideOtherPlayers )
		return true;

	// ������ҵĳ���
	if (IS_PET(pRole->GetID()) &&
		PetManager::Inst()->GetPet(pRole->GetID()) == NULL &&
		m_bHideOtherPlayers)
		return true;

	return false;
}

bool RoleMgr::IsInViewDist( Role* pRole )
{
	if( NULL == m_pLocalPlayer )
		return false;

	const float fRoleDistSq = Vec3DistSq( m_pLocalPlayer->GetPos(), pRole->GetPos() );
	if( fRoleDistSq > m_fViewDist * m_fViewDist )
		return false;

	return true;
}

void RoleMgr::UpdataHightLight( Role* pRole )
{
	const DWORD dwMousePointRoleID = GetCurMousePointRoleID();
	const DWORD dwCurTargetID = GetCurTargetID();

	// ѡ�л����ָ�����
	if( dwMousePointRoleID == pRole->GetID() || dwCurTargetID == pRole->GetID() )
	{
		pRole->SetMulColor( 2.0f, 2.0f, 2.0f );
		return;
	}

	// ѡ�л����ָ����ҵ�����
	if( pRole->IS_KIND_OF(Pet) )
	{
		Pet* pPet = (Pet*)pRole;
		if( !pPet->IsFSMEnabled() && 
			( pPet->GetOwnerID() == dwCurTargetID || pPet->GetOwnerID() == dwMousePointRoleID ) )
		{
			pRole->SetMulColor( 2.0f, 2.0f, 2.0f );
			return;
		}
	}
	pRole->SetMulColor( 1.0f, 1.0f, 1.0f );
}

void RoleMgr::UpdataGameObjectQuestEffect()
{
	for(map<DWORD,Role*>::iterator iter=m_roleMap.begin();
		iter!=m_roleMap.end();iter++)
	{
		if( IS_CREATURE( iter->first ) )
		{
			NPC* pNPC = (NPC*)iter->second;
			pNPC->UpdateGameObjectQuestEffect();
		}
	}
}

DWORD RoleMgr::NetRemoteGuildInfoChange(tagNS_RemoteRoleGuildInfoChange *pNetCmd, DWORD)
{
	Role *pRole = FindRole(pNetCmd->dwRoleID);
	if (P_VALID(pRole))
	{
		ASSERT( pRole->IS_KIND_OF(Player) );
		Player* pPlayer = (Player*)pRole;
		pPlayer->SetRoleGuildID(pNetCmd->dwGuildID);
		pPlayer->SetRoleGuildPos(pNetCmd->n8GuildPos);
	}
	return 0;
}

DWORD RoleMgr::NetSetRoleStateEx( tagNS_SetRoleStateEx* pNetCmd, DWORD )
{
	m_pLocalPlayer->SetRoleStateEx(pNetCmd->eState, true);
	
	return 0;
}

DWORD RoleMgr::NetUnsetRoleStateEx( tagNS_UnSetRoleStateEx* pNetCmd, DWORD )
{
	m_pLocalPlayer->SetRoleStateEx(pNetCmd->eState, false);

	return 0;
}

DWORD RoleMgr::NetRoleSetDisplay( tagNS_RoleSetDisplay* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if( P_VALID(pRole) && pRole->IsAttributeInited() )
	{
		ASSERT( pRole->IS_KIND_OF(Player) );
		Player* pPlayer = (Player*)pRole;
		pPlayer->SetDisplaySet( pNetCmd->sDisplaySet );
	}
	return 0;
}

DWORD RoleMgr::NetSetPersonalSet( tagNS_SetPersonalSet* pNetCmd, DWORD )
{
	if( P_VALID(m_pLocalPlayer) && m_pLocalPlayer->IsAttributeInited() )
	{
		m_pLocalPlayer->SetPersonalSet( pNetCmd->PersonalSet );
	}

	return 0;
}

DWORD RoleMgr::NetRoleTitleChange( tagNS_RoleTitleChangeBroadcast* pNetCmd, DWORD )
{
    Role* pRole = FindRole(pNetCmd->dwRoleID);
    if( P_VALID(pRole) && pRole->IsAttributeInited() )
    {
        ASSERT( pRole->IS_KIND_OF(Player) );
        Player* pPlayer = (Player*)pRole;
        pPlayer->SetPlayerTitle(pNetCmd->dwTitleID);

        // ֪ͨ
        tagRoleTitleChange e(_T("tagRoleTitleChange"), NULL);
        e.dwRoleID  = pNetCmd->dwRoleID;
        e.dwTitleID = pNetCmd->dwTitleID;
        m_pFrameMgr->SendEvent(&e);
    }
    return 0;
}

DWORD RoleMgr::NetCreatureTaggedOwnerChange(tagNS_CreatureTaggedOwnerChange* pNetCmd, DWORD)
{
	Role* pRole = FindRole(pNetCmd->dwCreatureID);
	if( P_VALID(pRole) && pRole->IS_KIND_OF(NPC) )
	{
		NPC* pNpc = (NPC*)pRole;
		pNpc->SetTaggedOwnerID(pNetCmd->dwTaggedOwner);
	}
	return 0;
}

DWORD RoleMgr::OnGetPetMount( tagEnterMount* pGameEvent )
{
    Role* pRole = FindRole(pGameEvent->dwPetID,true);
    Role* pOwner = FindRole(pGameEvent->dwOwnerID);

    if (P_VALID(pRole) && pRole->IS_STATIC_CLASS(Pet) && NULL == pRole->GetSceneNode())
    {
        Pet* pPet = static_cast<Pet*>(pRole);

		pPet->SetFSMEnabled(false);

        // α��������Ϣ
        tagNS_GetRemotePetState fakeNetMsg;
        fakeNetMsg.PetData.dwID         =   pGameEvent->dwPetID;
        fakeNetMsg.PetData.dwProtoID    =   pGameEvent->dwTypeID;
        fakeNetMsg.PetData.dwRoleID     =   pGameEvent->dwOwnerID;
        fakeNetMsg.PetData.uState.bisRiding     =   true;
        pPet->InitPetAttribute(&fakeNetMsg);

        const tagPetProto* petProto = pPet->GetProto();
        EPetModelType mdltype = EPMT_Mount;
        pPet->LoadEntity( petProto->strModelPath[mdltype].c_str());
        if (EAT_Skeleton == petProto->eAniType[mdltype])
        {
            for (list<tstring>::const_iterator it = petProto->lstSkinPath[mdltype].begin();
                it != petProto->lstSkinPath[mdltype].end(); ++it)
            {
                pPet->LoadSkin(it->c_str());
            }
            pPet->UpdateBoundingBox();
        }

        pPet->SetScale(petProto->scalePercent);
        if (P_VALID(pOwner) && pOwner->IS_KIND_OF(Player))
        {
            Player* pRider = static_cast<Player*>(pOwner);
            pPet->SetCurrScale(pRider->GetDesScale());
        }

        pPet->AttachSceneNodeToSG( MapMgr::Inst()->GetSceneGraph());
    }
	return 0;
}

DWORD RoleMgr::OnLeavePetMount( tagLeaveMount* pGameEvent )
{
    FreeRole(pGameEvent->dwPetID, MapMgr::Inst()->GetSceneGraph());

	return 0;
}

DWORD RoleMgr::OnNS_SetTransportMap( tagNS_SetTransportMap* pNetCmd, DWORD )
{
    if (E_Success == pNetCmd->dwErrorCode)
    {
        LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
        if (P_VALID(pLp))
            pLp->SetTransportPoint(pNetCmd->dwTransportMapID, pNetCmd->vTransportPoint);
    }

    ShowSetTransportErrorMsg(pNetCmd->dwErrorCode);

    return 0;
}

DWORD RoleMgr::OnNS_RoleLevelChange( tagNS_RoleLevelChange* pNetCmd, DWORD )
{
	if( pNetCmd->dwRoleID != GetLocalPlayerID() )
		return 0;

	RoleLevelUpPromptFrame* pFrame = (RoleLevelUpPromptFrame*)(m_pFrameMgr->GetFrame(_T("RoleLevelUpPromptFrame")) );
	if( !P_VALID(pFrame) )
	{
		m_pFrameMgr->CreateFrame(_T("World"), _T("RoleLevelUpPromptFrame"), _T("RoleLevelUpPromptFrame"), 0 );
		pFrame = (RoleLevelUpPromptFrame*)m_pFrameMgr->GetFrame( _T("RoleLevelUpPromptFrame") );
	}
	
	if( P_VALID( pFrame ) )
	{
		pFrame->LevelUpTo( pNetCmd->nLevel );
	}

	return 0;
}

void RoleMgr::ShowSetTransportErrorMsg( DWORD dwErrCode )
{
    if (dwErrCode >= EST_SuccessSet && dwErrCode < EST_ErrEnd)
    {
        tstringstream tss;
        tss << _T("SetTransportMapError_") << dwErrCode;
        CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[tss.str().c_str()]);
    }
}

DWORD RoleMgr::OnNetRename( tagNS_LPRename *pNetCmd, DWORD )
{
	if( pNetCmd->dwRoleID != GetLocalPlayerID() )
		return 0;

	switch(pNetCmd->dwErrorCode)
	{
	case E_Success:
		{
			m_pLocalPlayer->SetRoleName( pNetCmd->szNewName );
		}
		break;
	case E_CreateRole_NameExist:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CreateRole_NameExist")]);
		break;
	case E_Filter_Name_Illegal:
	case E_Filter_Name_InFilterFile:
	case E_CreateRole_NameInvalid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CreateRole_NameInvalid")]);
		break;
	case E_Filter_Name_TooLong:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Common_Error30")]);
		break;
	case E_Filter_Name_Null:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Common_Error31")]);
		break;
	}
	return 0;
}

DWORD RoleMgr::NetRoleRenameBroadcast( tagNS_RoleRename* pNetCmd, DWORD )
{
	Role* pRole = FindRole(pNetCmd->dwRoleID);
	if( P_VALID(pRole) && pRole->IsAttributeInited() )
	{
		ASSERT( pRole->IS_KIND_OF(Player) );
		Player* pPlayer = (Player*)pRole;
		pPlayer->SetRoleName( pNetCmd->szNewName );
	}
	return 0;
}

DWORD RoleMgr::OnLpChangeStateEvent( tagLPRoleStateChangeEvent *pEvent )
{
	bool bInSafeArea = m_pLocalPlayer->GetRoleState( ERS_SafeArea );
	DWORD dwHostility = m_pLocalPlayer->GetAttribute( ERA_Hostility );
	bool bPK = m_pLocalPlayer->GetRoleState( ERS_PK ) || m_pLocalPlayer->GetRoleState( ERS_PKEX );
	if( bInSafeArea && (dwHostility == 0) && !bPK && !( pEvent->dwOldState & ERS_SafeArea ) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Role_InSafeArea")]);
	}
	else if( !bInSafeArea && ( pEvent->dwOldState & ERS_SafeArea ) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Role_OutSafeArea")]);
	}
	return 0;
}