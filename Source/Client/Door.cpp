#include "StdAfx.h"
#include "Door.h"
#include "MapMgr.h"
#include "..\worlddefine\role_att.h"
#include "..\WorldDefine\creature_define.h"
#include "CreatureData.h"


IMP_RTTI(Door,NPC);

Door::Door(void)
{
	m_dwMapObjID=GT_INVALID;
}

Door::~Door(void)
{
}

void Door::InitFSM()
{
	//no FSM
}

void Door::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_OpenDoor"))//����
	{
		OpenDoor(true);		
	}
	else if(pNetCmd->dwID==pNetCmd->Crc32("NS_CloseDoor"))//����
	{
		CloseDoor(true);
	}
}

void Door::OnGameEvent( tagGameEvent* pGameEvent )
{}



bool Door::IsDead()
{
	//return m_bState;
	return false;
}

void Door::InitAttribute( tagNS_GetRemoteCreatureState* pCmd )
{
	// ȷ��SeneneNode�Ѿ�����
	ASSERT( P_VALID( m_pEntityNode ) );

	// ���ý�ɫλ�á����źͳ���
	const tagMapDoor* pMapDoor=MapMgr::Inst()->GetGameMap().FindMapDoor(pCmd->CreatureData.dwMapObjID);
	ASSERT( P_VALID( pMapDoor ) );

	Vector3 vScale(pMapDoor->fScale[0], pMapDoor->fScale[1], pMapDoor->fScale[2]);

	SetPos(pMapDoor->vPos);
	SetYaw(pMapDoor->fYaw);
	m_space.SetScale(vScale);

	// ��¼��ͼ����ID
	m_dwMapObjID = pCmd->CreatureData.dwMapObjID;

	// ���ý�ɫ����
	INT i;
	for (i=0; i<ERRA_End; ++i)
	{
		// �����shape��������
		if ((ERemoteRoleAtt)i != ERRA_Shape)
			SetRemoteRoleAtt((ERemoteRoleAtt)i, pCmd->CreatureData.nAtt[i], true);			
	}
	m_dwTypeID = pCmd->CreatureData.dwTypeID;

	// ����NPC���͵�
	const tagCreatureProto* pProto = CreatureData::Inst()->FindNpcAtt( m_dwTypeID );
	m_pProto = (tagCreatureProto*)pProto;

	// ���ý�ɫ�ȼ�
	SetRoleLevel(pCmd->CreatureData.nLevel);

	// ���ý�ɫ����
	SetRoleName(pProto->szName);

	// ���ý�ɫ״̬
	m_state.Init(pCmd->CreatureData.dwState);

	// ���buff�б�
	FreeRoleBuff();
	for(INT j=0; j<pCmd->CreatureData.nBuffNum; ++j)
	{
		AddRoleBuff(pCmd->CreatureData.Buff[j]);
	}

	// ���ó�ʼ״̬
	if(pCmd->CreatureData.bOpen)
	{
		OpenDoor(false);
	}
	else
	{
		CloseDoor(false);
	}

	//--�����������
	if(NULL!=m_pEntityNode )
		m_pEntityNode->SetWorldMat(m_space.GetMatrix());

	m_bGetAttribute=true;

	if (pCmd->CreatureData.bOpen == TRUE)
	{
		MapMgr::Inst()->GetNavMap()->GetCollider()->EnableDoorCollide(m_dwMapObjID,false);
	}
	else
	{
		MapMgr::Inst()->GetNavMap()->GetCollider()->EnableDoorCollide(m_dwMapObjID,true);
	}


}

void Door::OpenDoor(bool bPlayAction)
{
	m_bState = true;
	//--�ر���ײ
	MapMgr::Inst()->GetNavMap()->GetCollider()->EnableDoorCollide(m_dwMapObjID,false);

	AnimationCtrl *pAniCtrl = GetAniCtrl();
	if (NULL == pAniCtrl)
		return;
	//--���Ŷ���
	if(bPlayAction)
		pAniCtrl->OpenTrack(_T("open"),false,0.0f, 0.0f);
	else
	{
		pAniCtrl->OpenTrack(_T("open"),false,2.0f, 0.0f);
	}
}

void Door::CloseDoor(bool bPlayAction)
{
	m_bState = false;
	//--������ײ
	MapMgr::Inst()->GetNavMap()->GetCollider()->EnableDoorCollide(m_dwMapObjID,true);

	AnimationCtrl *pAniCtrl = GetAniCtrl();
	if (NULL == pAniCtrl)
		return;
	//--���Ŷ���
	if(bPlayAction)
	{
		pAniCtrl->OpenTrack(_T("clos"),false,0.0f, 0.0f);
	}
	else
	{
		pAniCtrl->OpenTrack(_T("clos"),false,2.0f, 0.0f);
	}
}