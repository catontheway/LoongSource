//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: vip_stall.cpp
// author: sunnee
// actor:
// data: 2009-06-15
// last:
// brief: VIP̯λ��
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_vipstall.h"

#include "../WorldDefine/msg_stall.h"

#include "world.h"
#include "db_session.h"
#include "role_mgr.h"
#include "role.h"
#include "vip_stall.h"

//-------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------
const DWORD MAX_PERIOD_OF_VALIDITY = 60 * 60 * 24;

//-------------------------------------------------------------------------------------------------------
// ������������
//-------------------------------------------------------------------------------------------------------
VIPStall::VIPStall()
{
	m_bInit = FALSE;
	m_vecVIPStall.clear();
	m_mapVIPStall.Clear();

	m_dwSecTimer	= g_world.GetWorldTime();
	m_nMinuteTimer	= 0;
}

VIPStall::~VIPStall()
{
	Destroy();
}

//-------------------------------------------------------------------------------------------------------
// ��ʼ�������£�����
//-------------------------------------------------------------------------------------------------------
BOOL VIPStall::Init()
{
	m_vecVIPStall.resize(VIP_STALL_MAX_NUM);

	// ����̯λ�����Ϣ
	for (int n=0; n<VIP_STALL_MAX_NUM; n++)
	{
		m_vecVIPStall[n].nRent = g_attRes.GetVIPStallRent(n);
	}

	RegisterStallEventFunc();

	return TRUE;
}

VOID VIPStall::Update()
{
	// ��ʼ���ж�
	if (!m_bInit)
		return;

	// �����¼�
	EventMgr<VIPStall>::Update();

	// ̯λ���ڴ���
	if (g_world.GetWorldTime() != m_dwSecTimer)
	{
		m_dwSecTimer = g_world.GetWorldTime();

		for (int n=0; n<VIP_STALL_MAX_NUM; n++)
		{
			if (!GT_VALID(m_vecVIPStall[n].dwOwnerID))
				continue;

			m_vecVIPStall[n].nRemainTime--;
			if (m_vecVIPStall[n].nRemainTime <= 0)
			{
				// ����̯λ��Ϣ
				m_mapVIPStall.Erase(m_vecVIPStall[n].dwOwnerID);

				m_vecVIPStall[n].Reset();
				m_vecVIPStall[n].dwLastUpdateTime = m_dwSecTimer;
				m_dwVIPStallChange = m_dwSecTimer;

				// ֪ͨ���ݿ�
				tagNDBC_ApplyVIPStall send;
				send.VipStallInfo.byStallIndex	= n;
				send.VipStallInfo.dwOwnerID		= GT_INVALID;
				send.VipStallInfo.nRemainTime	= GT_INVALID;
				g_dbSession.Send(&send, send.dwSize);
			}
		}

		// ÿ���ӷ���ʱ�������Ϣ
		if (++m_nMinuteTimer > 60)
		{
			tagNDBC_UpdateVIPStall send;
			g_dbSession.Send(&send, send.dwSize);
			m_nMinuteTimer = 0;
		}
	}
}

VOID VIPStall::Destroy()
{
	m_vecVIPStall.clear();
	m_mapVIPStall.Clear();
	m_bInit = FALSE;
}

//-------------------------------------------------------------------------------------------------------
// �¼�ע��ʹ���
//-------------------------------------------------------------------------------------------------------
VOID VIPStall::RegisterStallEventFunc()
{
	RegisterEventFunc(EVT_SetStallTitle,			&VIPStall::OnSetStallTitleFunc);
	RegisterEventFunc(EVT_SetStallStatus,			&VIPStall::OnSetStallStatusFunc);
	RegisterEventFunc(EVT_ChangeStallGoods,			&VIPStall::OnChangeStallGoodsFunc);
}

VOID VIPStall::OnSetStallTitleFunc( DWORD dwSenderID, VOID* pEventMessage )
{
	// pEventMessage <==> TCHAR*

	tagVIPStall* pInfo = GetVIPStallInfo(dwSenderID);

	if (!P_VALID(pInfo))
		return;

	// �ϲ㱣֤���ַ����Ľ�����
	TCHAR* pTitle = (TCHAR*)pEventMessage;

	// ��ȡ����޶ȵ��ַ���
	_tcsncpy(pInfo->szStallTitle, pTitle, STALL_MAX_TITLE_NUM-1);

	// ���ý�����
	pInfo->szStallTitle[STALL_MAX_TITLE_NUM-1] = '\0';

	pInfo->dwLastUpdateTime = m_dwSecTimer;
	m_dwVIPStallChange		= m_dwSecTimer;
}

VOID VIPStall::OnSetStallStatusFunc( DWORD dwSenderID, VOID* pEventMessage )
{
	// pEventMessage <==> BYTE*

	tagVIPStall* pInfo = GetVIPStallInfo(dwSenderID);

	if (!P_VALID(pInfo))
		return;

	pInfo->eStallStatus		= (EVIPStallStatus)(*((BYTE*)pEventMessage));
	pInfo->dwLastUpdateTime	= m_dwSecTimer;
	m_dwVIPStallChange		= m_dwSecTimer;
}

VOID VIPStall::OnChangeStallGoodsFunc( DWORD dwSenderID, VOID* pEventMessage )
{
	// pEventMessage <==> NULL

	tagVIPStall* pInfo = GetVIPStallInfo(dwSenderID);

	if (!P_VALID(pInfo))
		return;

	pInfo->dwLastGoodsTime = m_dwSecTimer;
}

//-------------------------------------------------------------------------------------------------------
// �����ݿ��ȡVIP̯λ��Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD VIPStall::LoadAllVIPStallInfo( tagVipStall2DB* pVIPStallInfo )
{
	ASSERT(pVIPStallInfo);

	if (!P_VALID(pVIPStallInfo))
		return GT_INVALID;

	for (int n=0; n<VIP_STALL_MAX_NUM; n++)
	{
		m_vecVIPStall[n].byStallIndex		= pVIPStallInfo[n].byStallIndex;
		m_vecVIPStall[n].dwOwnerID			= pVIPStallInfo[n].dwOwnerID;
		m_vecVIPStall[n].nRemainTime		= pVIPStallInfo[n].nRemainTime;
		m_vecVIPStall[n].dwLastUpdateTime	= m_dwSecTimer;
		m_dwVIPStallChange					= m_dwSecTimer;

		if (GT_VALID(pVIPStallInfo[n].dwOwnerID))
		{
			m_mapVIPStall.Add(pVIPStallInfo[n].dwOwnerID, n);

			Role* pRole = g_roleMgr.GetRolePtrByID(pVIPStallInfo[n].dwOwnerID);
			if (P_VALID(pRole) && pRole->IsInRoleState(ERS_Stall))
			{
				m_vecVIPStall[n].eStallStatus = EVSS_Open;
				m_vecVIPStall[n].dwLastGoodsTime = m_dwSecTimer;
				pRole->GetStallTitle(m_vecVIPStall[n].szStallTitle);
			}
			else
			{
				m_vecVIPStall[n].eStallStatus = EVSS_Close;
			}
		}
		else
		{
			m_vecVIPStall[n].eStallStatus = EVSS_ForHire;
		}
	}
	m_bInit = TRUE;

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ȡ��VIP̯λ��Ҫ��Ϣ
//-------------------------------------------------------------------------------------------------------
tagVIPStall* VIPStall::GetVIPStallInfo( BYTE index )
{
	//ASSERT( index >=0 && index < VIP_STALL_MAX_NUM);

	if (index < 0 || index >= VIP_STALL_MAX_NUM)
		return NULL;

	if (!m_bInit)	return NULL;
	
	return &(m_vecVIPStall[index]);
}

tagVIPStall* VIPStall::GetVIPStallInfo( DWORD dwRoleID )
{
	if (!GT_VALID(dwRoleID))
		return NULL;

	if(!m_bInit)	
		return NULL;

	BYTE byIndex = m_mapVIPStall.Peek(dwRoleID);

	if (!GT_VALID(byIndex))
		return NULL;
	
	return GetVIPStallInfo(byIndex);
}

Role* VIPStall::GetVIPStallRole( BYTE index )
{
	tagVIPStall* pInfo = GetVIPStallInfo(index);

	// ̯λ������
	if (!P_VALID(pInfo))
		return NULL;

	// ̯λ����
	if (!GT_VALID(pInfo->dwOwnerID))
		return NULL;

	return g_roleMgr.GetRolePtrByID(pInfo->dwOwnerID);
}

BOOL VIPStall::IsInStatus( BYTE index, EVIPStallStatus eStatus )
{
	tagVIPStall* pInfo = GetVIPStallInfo(index);
	if (!P_VALID(pInfo))
		return FALSE;

	if (pInfo->eStallStatus == eStatus)
		return TRUE;

	return FALSE;
}

DWORD VIPStall::GetSpecVIPStallGoodsTime( BYTE index )
{
	tagVIPStall* pInfo = GetVIPStallInfo(index);
	if (!P_VALID(pInfo))
		return GT_INVALID;

	return pInfo->dwLastGoodsTime;
}

DWORD VIPStall::SetVIPStallStatus( BYTE index, EVIPStallStatus eStatus )
{
	tagVIPStall* pInfo = GetVIPStallInfo(index);
	if (!P_VALID(pInfo))
	{
		return GT_INVALID;
	}

	pInfo->eStallStatus = eStatus;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ͼ�ϲ���Ϣ����
//-------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------
// ����VIP̯λ
//-------------------------------------------------------------------------------------------------------
DWORD VIPStall::ApplyVIPStall( Role* pRole, BYTE byIndex, INT32 nRent )
{
	if (!P_VALID(pRole))
		return GT_INVALID;

	if (!m_bInit)
		return E_Stall_VIP_Not_Init;

	// ��������Ƿ��Ѿ����޹�VIP̯λ
	if (m_mapVIPStall.IsExist(pRole->GetID()))
	{
		return E_Stall_VIP_Own_Another;
	}

	// ȡ��̯λ��Ϣ
	tagVIPStall* pInfo = GetVIPStallInfo(byIndex);
	if (!P_VALID(pInfo))
		return E_Stall_VIP_Info_NotExist;

	// ����̯λ�Ƿ��Ѿ�������
	if (GT_VALID(pInfo->dwOwnerID))
	{
		return E_Stall_VIP_Already_Rent;
	}

	if (pInfo->nRent != nRent)
		return E_Stall_VIP_Rent_Error;

	// ������Ԫ���Ƿ��㹻
	if(nRent > pRole->GetCurMgr().GetBagYuanBao() || nRent <= 0)
	{
		return E_BagYuanBao_NotEnough;
	}

	// �۳����Ԫ��
	pRole->GetCurMgr().DecBagYuanBao(nRent, ELCLD_VIP_Stall_Rent);

	// ����ɹ�
	pInfo->dwOwnerID		= pRole->GetID();
	pInfo->nRemainTime		= MAX_PERIOD_OF_VALIDITY;
	pInfo->dwLastUpdateTime	= m_dwSecTimer;
	m_dwVIPStallChange		= m_dwSecTimer;

	m_mapVIPStall.Add(pInfo->dwOwnerID, pInfo->byStallIndex);


	if (pRole->IsInRoleState(ERS_Stall))
	{
		pInfo->eStallStatus = EVSS_Open;
		pInfo->dwLastGoodsTime = m_dwSecTimer;
		pRole->GetStallTitle(pInfo->szStallTitle);
	}
	else
	{
		pInfo->eStallStatus = EVSS_Close;
	}

	// ֪ͨ���ݿ�
	tagNDBC_ApplyVIPStall send;
	send.VipStallInfo.byStallIndex	= pInfo->byStallIndex;
	send.VipStallInfo.dwOwnerID		= pInfo->dwOwnerID;
	send.VipStallInfo.nRemainTime	= pInfo->nRemainTime;
	g_dbSession.Send(&send, send.dwSize);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �ͻ��˻�ȡ����VIP̯λ��Ҫ��Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD VIPStall::GetAllVIPStallInfo( OUT tagVIPStall* pStallInfo, OUT DWORD& dwTime )
{
	if (!P_VALID(pStallInfo))
		return GT_INVALID;
	
	if (!m_bInit)
		return E_Stall_VIP_Not_Init;

	memcpy(pStallInfo, &m_vecVIPStall[0], sizeof(tagVIPStall)*VIP_STALL_MAX_NUM);
	dwTime = m_dwVIPStallChange;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �ͻ��˻�ȡĳʱ������µ�����VIP̯λ��Ҫ��Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD VIPStall::GetUpdatedStallInfo( OUT tagVIPStall* pStallInfo, INT32& nNum, DWORD& dwTime )
{
	if (!P_VALID(pStallInfo))
		return GT_INVALID;

	if (!m_bInit)
		return E_Stall_VIP_Not_Init;

	nNum = 0;

	if (dwTime >= m_dwVIPStallChange)
		return E_Success;

	for (int n=0; n<VIP_STALL_MAX_NUM; n++)
	{
		if (dwTime < m_vecVIPStall[n].dwLastUpdateTime)
		{
			memcpy(&(pStallInfo[nNum++]), &(m_vecVIPStall[n]), sizeof(tagVIPStall));
		}
	}
	dwTime = m_dwVIPStallChange;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �ͻ��˻�ȡĳVIP̯λ��Ʒ��Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD VIPStall::GetVIPStallGoods( OUT LPVOID pData, OUT BYTE &byGoodsNum, OUT INT &nGoodsSz, DWORD& dwRequestTime, BYTE byIndex )
{
	byGoodsNum = 0;
	nGoodsSz = 0;

	// ȡ��̯λ��Ҫ��Ϣ
	tagVIPStall* pInfo = GetVIPStallInfo(byIndex);
	if (!P_VALID(pInfo))
	{
		return E_Stall_VIP_Info_NotExist;
	}

	// �ж�̯λ�Ƿ���Ӫҵ��
	switch (pInfo->eStallStatus)
	{
	case EVSS_ForHire:
		return E_Stall_VIP_For_Hire;

	case EVSS_Close:
		return E_Stall_Role_Pull;
	}
	
	// �жϸ�̯λ��Ʒ�Ƿ���¹�
	if (dwRequestTime == pInfo->dwLastGoodsTime)
	{
		return E_Stall_VIP_GetGoods_Needless;
	}

	Role* pStallRole = g_roleMgr.GetRolePtrByID(pInfo->dwOwnerID);
	if (!P_VALID(pStallRole))
	{
		// �ϲ��Ѿ��жϹ��ˣ�����Ϊ�˱���
		return GT_INVALID;
	}

	DWORD dwErrorCode = pStallRole->GetStallGoods(pData, byGoodsNum, nGoodsSz);

	if (dwErrorCode == E_Success)
	{
		dwRequestTime = pInfo->dwLastGoodsTime;
	}
	
	return dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ɾ����ɫʱ��������
//-------------------------------------------------------------------------------------------------------
VOID VIPStall::RemoveRoleVIPStall( DWORD dwRoleID )
{
	tagVIPStall* pInfo = GetVIPStallInfo(dwRoleID);
	if (!P_VALID(pInfo))
	{
		return;
	}

	// ����̯λ��Ϣ
	m_mapVIPStall.Erase(pInfo->dwOwnerID);

	pInfo->Reset();
	pInfo->dwLastUpdateTime = m_dwSecTimer;
	m_dwVIPStallChange		= m_dwSecTimer;

	// ֪ͨ���ݿ�
	tagNDBC_ApplyVIPStall send;
	send.VipStallInfo.byStallIndex	= pInfo->byStallIndex;
	send.VipStallInfo.dwOwnerID		= GT_INVALID;
	send.VipStallInfo.nRemainTime	= GT_INVALID;
	g_dbSession.Send(&send, send.dwSize);
}

//-------------------------------------------------------------------------------------------------------
// ȫ�ֱ���
//-------------------------------------------------------------------------------------------------------
VIPStall g_VIPStall;
