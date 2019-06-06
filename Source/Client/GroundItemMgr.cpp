//-----------------------------------------------------------------------------
//!\file GroundItemMgr.cpp
//!\author Lyp
//!
//!\date 2008-10-27
//! last 2008-10-27
//!
//!\brief ������Ʒ����
//!
//!	Copyright (c) 2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "GroundItemMgr.h"

#include "..\WorldDefine\msg_loot.h"
#include "..\WorldDefine\ItemDefine.h"
#include "item.h"
#include "MapMgr.h"
#include "ItemProtoData.h"
#include "RoleMgr.h"
#include "CombatSysUtil.h"
#include "Role.h"

#include "AudioSys.h"
#include "EffectMgr.h"
#include "TeamSys.h"

#include "PetManager.h"

const float DEF_GRAVITATION	 = 1000.0f;		//�������ٶ�
const float	DEF_YSPEED		 = 1000.0f;		//��ֱ�����ϵ��ٶ� 
const float DEF_ROTATERADIAN = 0.01f;		//Ĭ��ÿ������ת�Ƕ�	
//-----------------------------------------------------------------------------
// construction /destruction
//-----------------------------------------------------------------------------
GroundItemMgr::GroundItemMgr():m_Trunk(this)
{
}

GroundItemMgr::~GroundItemMgr()
{
}
GroundItemMgr* GroundItemMgr::Inst()
{
	static GroundItemMgr s_GroundItemMgr;
	return &s_GroundItemMgr;
}

//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
VOID GroundItemMgr::Init()
{
	ClearAll();

	// ע��������Ϣ������
	m_pCmdMgr->Register("NS_RoleGroundItemDisappear",
		(NETMSGPROC)m_Trunk.sfp2(&GroundItemMgr::RoleGroundItemDisappear), _T("NS_RoleGroundItemDisappear"));

	m_pCmdMgr->Register("NS_RolePickUpItem",
		(NETMSGPROC)m_Trunk.sfp2(&GroundItemMgr::RolePickUpItem), _T("NS_RolePickUpItem"));
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID GroundItemMgr::Destroy()
{
	ClearAll();

	// ע��������Ϣ������
	m_pCmdMgr->UnRegister("NS_RoleGroundItemDisappear", (NETMSGPROC)m_Trunk.sfp2(&GroundItemMgr::RoleGroundItemDisappear));
	m_pCmdMgr->UnRegister("NS_RolePickUpItem", (NETMSGPROC)m_Trunk.sfp2(&GroundItemMgr::RolePickUpItem));
}


//-----------------------------------------------------------------------------
// ������е�������
//-----------------------------------------------------------------------------
VOID GroundItemMgr::ClearAll()
{
	m_mapGroundItem.ResetIterator();
	tagGroundItem* pItem = NULL;
	while(m_mapGroundItem.PeekNext(pItem))
		SAFE_DEL(pItem);
	m_mapGroundItem.Clear();

	m_listParabolaItem.ResetIterator();
	tagParabolaItem* pPI = NULL;
	while(m_listParabolaItem.PeekNext(pPI))
		SAFE_DEL(pPI);
	m_listParabolaItem.Clear();
}


//-----------------------------------------------------------------------------
// ���߼��
//-----------------------------------------------------------------------------
tagGroundItem* GroundItemMgr::PickItem(const Ray& rayMouse)
{
	tagGroundItem* pItem = NULL;
	tagGroundItem* pNearestItem = NULL;
	float fMinDistSq = FLOAT_MAX;

	m_mapGroundItem.ResetIterator();
	while(m_mapGroundItem.PeekNext(pItem))
	{
		if( !P_VALID(pItem->pNode) )
			continue;

		AABBox box;
		pItem->pNode->GetBox(box);
		TResult tr = rayMouse.Intersect(box);
		if(tr.first)
		{
			float fDistSq = Vec3DistSq( rayMouse.origin, pItem->space.GetPos() );
			if( fDistSq < fMinDistSq )
			{
				fMinDistSq = fDistSq;
				pNearestItem = pItem;
			}
		}
	}

	return pNearestItem;
}


//-----------------------------------------------------------------------------
// PutDownItem
//-----------------------------------------------------------------------------
VOID GroundItemMgr::PutDownItem(Item* pItem, FLOAT fX, FLOAT fY, FLOAT fZ)
{
	tagNC_RolePutDownItem send;
	send.n64Serial = pItem->GetItemId();
	m_pSession->Send(&send);
}


//-----------------------------------------------------------------------------
// PickUpItem
//-----------------------------------------------------------------------------
BOOL GroundItemMgr::PickUpItem(INT64 n64Serial, DWORD dwNum)
{
	tagGroundItem* pItem = m_mapGroundItem.Peek(n64Serial);
	if( !P_VALID(pItem) )
		return FALSE;

	if( pItem->bChecked )	// �Ѿ��������δ����
		return TRUE;

	tagNC_RolePickUpItem send;
	send.n64Serial = n64Serial;
	
	m_pSession->Send(&send);
	pItem->bChecked = TRUE;
	return TRUE;
}


//-----------------------------------------------------------------------------
// RolePickUpItem
//-----------------------------------------------------------------------------
DWORD GroundItemMgr::RolePickUpItem(tagNS_RolePickUpItem* pRecv, DWORD dwParam)
{
	if( pRecv->dwRoleID != RoleMgr::Inst()->GetLocalPlayerID() )
		return 0;

	tagGroundItem* pItem = m_mapGroundItem.Peek(pRecv->n64Serial);
	if( P_VALID(pItem) )
	{
		pItem->bChecked = FALSE;	// �յ�����
	}

	if( E_Success != pRecv->dwErrorCode )
	{
		ShowErrMsg(pRecv->dwErrorCode);
	}

	return 0;
}


//-----------------------------------------------------------------------------
// RoleSyncGroundItem
//-----------------------------------------------------------------------------
DWORD GroundItemMgr::RoleSyncGroundItem(tagNS_SyncGroundItem* pRecv, DWORD dwParam)
{
	tagGroundItem* pItem = m_mapGroundItem.Peek(pRecv->n64Serial);
	if( !P_VALID(pItem) )
	{
		pItem = new tagGroundItem;
		if( P_VALID(pItem) )
			m_mapGroundItem.Add(pRecv->n64Serial, pItem);
	}
	
	if( P_VALID(pItem) )
	{
		pItem->n64Serial = pRecv->n64Serial;
		pItem->dwRoleID = pRecv->dwPutDownUnitID;
		pItem->dwOwnerID = pRecv->dwOwnerID;
		pItem->dwGroupID = pRecv->dwGroupID;
		pItem->dwTeamID = pRecv->dwTeamID;
		pItem->dwTypeID = pRecv->dwTypeID;
		pItem->nNum = pRecv->nNum;
		pItem->bChecked = FALSE;
		pItem->space.SetPos(pRecv->vPos);
		pItem->space.SetYaw(0);
		
		if (pRecv->bKilled)
		{
			SetPutDownAction(pItem);
		}

		// ��ʾ����
		if( FALSE == this->ShowItem(pItem) )
		{
			m_mapGroundItem.Erase(pRecv->n64Serial);
			SAFE_DEL(pItem);
			return GT_INVALID;
		}

		// ����Ҫ���ŵ��䶯������Ʒ������������Ч
		if( !pRecv->bKilled && GT_INVALID == pItem->dwEffectID && CanPickUp( pItem ) )
			pItem->dwEffectID = EffectMgr::Inst()->PlayEffect( pRecv->vPos, 0.0f, _T("txqt08") );
	}

	return 0;
}



//-----------------------------------------------------------------------------
// RoleGroundItemDisappear
//-----------------------------------------------------------------------------
DWORD GroundItemMgr::RoleGroundItemDisappear(tagNS_RoleGroundItemDisappear* pRecv, DWORD dwParam)
{
	INT nNum = ((pRecv->dwSize - sizeof(tagNS_RoleGroundItemDisappear))
		/sizeof(pRecv->n64Serial[0])) + 1;

	for(INT n=0; n<nNum; n++)
	{
		tagGroundItem* pItem = m_mapGroundItem.Peek(pRecv->n64Serial[n]);
		if( P_VALID(pItem) )
		{
			// ����
			this->HideItem(pItem);
			m_mapGroundItem.Erase(pRecv->n64Serial[n]);
			SAFE_DEL(pItem);
		}
	}
	return 0;
}


//-----------------------------------------------------------------------------
// ShowItem
//-----------------------------------------------------------------------------
BOOL GroundItemMgr::ShowItem(tagGroundItem* pItem)
{
	SceneGraph* pScene = MapMgr::Inst()->GetSceneGraph();
	if( !P_VALID(pItem->pNode) )
		pItem->pNode = new EntityNode;

	if( !P_VALID(pItem->pNode) )
	{
		ASSERT(0);
		return FALSE;
	}

	tstring strMod;
	if( TYPE_ID_MONEY == pItem->dwTypeID )
	{
		strMod = _T("data\\model\\item\\dl09");
	}
	else
	{
		const tagItemDisplayInfo* pDisplayInfo = ItemProtoData::Inst()->FindItemDisplay(pItem->dwTypeID);
		if( !P_VALID(pDisplayInfo) )
		{
			ASSERT(0);
			SAFE_DEL(pItem->pNode);
			return FALSE;
		}
		strMod = _T("data\\model\\item\\");
		strMod += pDisplayInfo->szDropMod;
		
	}

	strMod += _T(".FSM");

	pItem->pNode->LoadRes(strMod.c_str());
	pItem->pNode->SetWorldMat(pItem->space.GetMatrix());
	pScene->AttachDynamicNode(pItem->pNode);

	return TRUE;
}



//-----------------------------------------------------------------------------
// HideItem
//-----------------------------------------------------------------------------
VOID GroundItemMgr::HideItem(tagGroundItem* pItem)
{
	if( !P_VALID(pItem->pNode) )
	{
		ASSERT(0);
		return;
	}

	SceneGraph* pScene = MapMgr::Inst()->GetSceneGraph();
	pScene->DetachDynamicNode(pItem->pNode);
	SAFE_DEL(pItem->pNode);
	pItem->pNode = NULL;

	// �ر���Ч
	EffectMgr::Inst()->StopEffect( pItem->dwEffectID );
	pItem->dwEffectID = GT_INVALID;
}


//-----------------------------------------------------------------------------
// find item
//-----------------------------------------------------------------------------
tagGroundItem* GroundItemMgr::FindItem( INT64 n64Serial )
{
	return m_mapGroundItem.Peek(n64Serial);
}

//-----------------------------------------------------------------------------
// GetNearestCanPickUpItem
//-----------------------------------------------------------------------------
tagGroundItem* GroundItemMgr::GetNearestCanPickUpItem(const Vector3& vCenter, const float fRange)
{
	const float fRangeSq = fRange * fRange;
	float fDistRefSq = FLOAT_MAX;
	tagGroundItem* pNearestItem = NULL;

	tagGroundItem* pItem = NULL;
	m_mapGroundItem.ResetIterator();
	while(m_mapGroundItem.PeekNext(pItem))
	{
		if( !P_VALID(pItem->pNode) )
			continue;
		if( !CanPickUp( pItem ) )
			continue;

		const float fDistSq = Vec3DistSq( vCenter, pItem->space.GetPos() );
		if( fDistSq > fRangeSq )
			continue;
		if( fDistRefSq > fDistSq )
		{
			fDistRefSq = fDistSq;
			pNearestItem = pItem;
		}
	}
	return pNearestItem;
}

tagGroundItem* GroundItemMgr::GetNearestCanPickUpItem( const Vector3& vCenter, const float fRange,
               bool bMoney, bool bEquip,
               bool bMaterial, bool bTrophy, bool bMedicine, bool bOthers )
{
    const float fRangeSq = fRange * fRange;
    float fDistRefSq = FLOAT_MAX;
    tagGroundItem* pNearestItem = NULL;

    tagGroundItem* pItem = NULL;
    m_mapGroundItem.ResetIterator();
    while(m_mapGroundItem.PeekNext(pItem))
    {
        if( !P_VALID(pItem->pNode) )
            continue;
        if( !CanPickUp( pItem ) )
            continue;

        // �ȿ�������ʰȡ�Զ���������Ƿ���
        if (PetManager::Inst()->IsExistInErrGroundItem(pItem->n64Serial))
            continue;

        // �������͹���
        const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto(pItem->dwTypeID);
        const tagEquipProto* pEquipProto = ItemProtoData::Inst()->FindEquipProto(pItem->dwTypeID);
        
        if (TYPE_ID_MONEY == pItem->dwTypeID )
        {
            // Ǯ
            if (!bMoney)
                continue;
        }
        else if (P_VALID(pEquipProto))
        {
            // ��װ��
            if (!bEquip)
                continue;
        }
        else if (EITE_Capture == pItemProto->eTypeEx )
        {
            // ս��Ʒ
            if (!bTrophy)
                continue;
        }
        else if (EITE_Material == pItemProto->eTypeEx)
        {
            // ����
            if (!bMaterial)
                continue;
        }
        else if (EITE_Resume == pItemProto->eTypeEx ||
            EITE_Advance == pItemProto->eTypeEx)
        {
            // ҩƷ
            if (!bMedicine)
                continue;
        }
        else
        {
            // ����
            if (!bOthers)
                continue;
        }
        

        const float fDistSq = Vec3DistSq( vCenter, pItem->space.GetPos() );
        if( fDistSq > fRangeSq )
            continue;
        if( fDistRefSq > fDistSq )
        {
            fDistRefSq = fDistSq;
            pNearestItem = pItem;
        }
    }
    return pNearestItem;
}
//-----------------------------------------------------------------------------
// play drop sound
//-----------------------------------------------------------------------------
VOID GroundItemMgr::PlayDropSound(DWORD dwTypeID)
{
	if( TYPE_ID_MONEY == dwTypeID )
	{
		TObjRef<AudioSys>()->Play2DSound(_T("drop_gold"), SOUND_WISPER);
		return;
	}

	const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(dwTypeID);
	if( !P_VALID(pProto) )
		return;

	if( EIT_Weapon == pProto->eType // �������߷���
		|| EIT_ClothArmor == pProto->eType 
		|| EIT_Decoration == pProto->eType 
		|| EIT_Fashion == pProto->eType 
		|| EIT_Armor == pProto->eType )
	{
		TObjRef<AudioSys>()->Play2DSound(_T("drop_equip"), SOUND_WISPER);
	}
	else if( pProto->eSpecFunc == EISF_HoleGem 
		|| pProto->eStuffType == EST_Stone 
		|| pProto->eStuffType == EST_Jade 
		|| pProto->eStuffType == EST_Iron 
		|| pProto->eStuffType == EST_Silver 
		|| pProto->eStuffType == EST_Gold )	// ��ʯ ��ʯ
	{
		TObjRef<AudioSys>()->Play2DSound(_T("drop_metal"), SOUND_WISPER);
	}
	else
	{
		TObjRef<AudioSys>()->Play2DSound(_T("drop_item"), SOUND_WISPER);
	}
}
//-------------------------------------------------------------------------------------------------
// ���������յ����������XZƽ����ٶȺ�ʱ�䣨Y���ٶ�ȷ����
//-------------------------------------------------------------------------------------------------
inline bool _CalcParabolaXZSpeed(Vector3 startV, Vector3 intersectV, OUT float &time, OUT float &xzSpeed)
{
	float a = DEF_GRAVITATION;
	float b = -DEF_YSPEED;
	float c = intersectV.y-startV.y;
	float d = b*b-4.0f*a*c;
	if(d>0)
	{
		time = (-b+sqrtf(d))/(2.0f*a);
		
		//���������������ˮƽ����
		startV.y = 0;
		intersectV.y = 0;
		float dist = Vec3Dist(startV, intersectV);
		//����ˮƽ�ٶ�
		xzSpeed = dist/time;
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------
// ������㣬�յ㣬XZƽ����ٶȺ͵�ǰʱ�����������ߵ�ǰʱ����λ�ã�Y���ٶ�ȷ����
//-------------------------------------------------------------------------------------------------
inline bool _CalcParabolaPos(float xzSpeed, float time, Vector3 startV, Vector3 intersectV, OUT Vector3 &retV)
{
	float a = DEF_GRAVITATION;
	float b = DEF_YSPEED;
	float height = b*time - a*time*time;
	Vector3 dir	 = startV - intersectV;
	dir.y = 0;
	Vec3Normalize(dir);
	retV = startV - dir*xzSpeed*time;
	retV.y = startV.y + height;
	
	return true;
}

inline bool _CalcObjectRotateRadian(float time, Vector3 retV, Vector3 axis, float rad, OUT Transform &trans)
{
	trans.LoadIdentity();
	trans.Translate(retV);
	trans.RotateAxis(axis, time*rad);

	return true;
}

//----------------------------------------------------------------------
// Update
//----------------------------------------------------------------------
VOID GroundItemMgr::Update()
{
	m_listParabolaItem.ResetIterator();
	tagParabolaItem* pNode = NULL;
	while(m_listParabolaItem.PeekNext(pNode))
	{
		if(P_VALID(pNode))
		{
			tagGroundItem* pItem = m_mapGroundItem.Peek(pNode->n64Serail);
			if(P_VALID(pItem))
			{
				FLOAT fPostTime = (FLOAT)(INT)(Kernel::Inst()->GetAccumTimeDW() - pNode->dwStartTime);
				Transform trans;
				Vector3 vRet;
				
				if(fPostTime < 0)
				{
					pItem->pNode->Hide(true);
					break;
				}
				
				if(fPostTime >= pNode->fTotalTime*1000.0f)
				{
					vRet = pNode->vEndPos;
					trans.LoadIdentity();
					trans.Translate(vRet);
					trans.Rotate(RandUnit(), 0.0f, 0.0f);

					m_listParabolaItem.Erase(pNode);
					SAFE_DEL(pNode);

					// ������Ч
					if( pItem->dwOwnerID == RoleMgr::Inst()->GetLocalPlayerID() 
						|| pItem->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
					{
						PlayDropSound(pItem->dwTypeID);
					}
					
					// ������Ч
					if( GT_INVALID == pItem->dwEffectID && CanPickUp( pItem ) )
						pItem->dwEffectID = EffectMgr::Inst()->PlayEffect( vRet, 0.0f, _T("txqt08") );
				}
				else
				{
					_CalcParabolaPos(pNode->fXZSpeed, fPostTime/1000.0f, pNode->vStartPos, pNode->vEndPos, vRet);
					_CalcObjectRotateRadian(fPostTime, vRet, pNode->vAxis, DEF_ROTATERADIAN, trans);
				}
				
				if(P_VALID(pItem->pNode))
				{
					pItem->space.SetPos(vRet);
					pItem->pNode->SetWorldMat(trans.GetMatrix());
					pItem->pNode->Hide(false);
				}
			}
			else
			{
				m_listParabolaItem.Erase(pNode);
				SAFE_DEL(pNode);
			}
		}
	}
}

VOID GroundItemMgr::SetPutDownAction( tagGroundItem* pItem )
{
	// ���ҵ�����Ʒ�Ľ�ɫ�Ƿ���ڣ������򲥷ŵ���������
	Role* pRole = RoleMgr::Inst()->FindRole(pItem->dwRoleID, false, true);
	if(P_VALID(pRole))
	{
		tagParabolaItem* pNode = new tagParabolaItem;
		pNode->n64Serail	= pItem->n64Serial;
		pNode->dwStartTime	= Kernel::Inst()->GetAccumTimeDW();
		/*if(IS_CREATURE(pItem->dwRoleID))
		{
			pNode->dwStartTime += GetRandom(0, 1500);
		}*/

		pNode->vStartPos	= pRole->GetPos();
		pNode->vAxis		= RandUnitVec3();
		/*pNode->vStartPos.y += pRole->GetRoleSize().y/2;*/
		pNode->vEndPos		= pItem->space.GetPos();
		if(!_CalcParabolaXZSpeed(pNode->vStartPos, pNode->vEndPos, pNode->fTotalTime, pNode->fXZSpeed))
		{
			SAFE_DEL(pNode);
			return;
		}
		
		pItem->space.SetPos(pNode->vStartPos);
		m_listParabolaItem.PushBack(pNode);
	}
}

bool GroundItemMgr::CanPickUp(const tagGroundItem* pItem)
{
	ASSERT( P_VALID(pItem) );

	// �κ��˶�����ʰȡ
	if( GT_INVALID == pItem->dwOwnerID && GT_INVALID == pItem->dwTeamID )
		return true;

	// ���ڱ�С�ӵ���Ʒ������С�������г�Ա������ʰȡ
	if( GT_INVALID == pItem->dwOwnerID 
		&& TeamSys::Inst()->GetTeamID() == pItem->dwTeamID )
	{
		return true;
	}

	// ���ڱ������ʰȡ����Ʒ
	if( RoleMgr::Inst()->GetLocalPlayerID() == pItem->dwOwnerID )
		return true;

	return false;
}

VOID GroundItemMgr::ShowErrMsg(DWORD dwErrCode)
{
	TCHAR szKey[100];
	_sntprintf( szKey, 100, _T("PickGroundItemErr%u"), dwErrCode );

	const TCHAR* szMsg = g_StrTable[szKey];
	CombatSysUtil::Inst()->ShowScreenCenterMsg( szMsg );
}