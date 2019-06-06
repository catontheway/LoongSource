#include "StdAfx.h"
#include "role.h"
#include "map.h"
#include "../WorldDefine/msg_pet_exchange.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../WorldDefine/msg_pet.h"
#include "pet_pocket.h"
#include "pet_soul.h"
#include "pet_define.h"
//------------------------------------------------------------------------
// ������
//------------------------------------------------------------------------
DWORD Role::ProcPetExchangeReq(OUT Role* &pTarget, DWORD dwTgtRoleID)
{
	// �Ƿ��Լ����Լ�����
	if(GetID() == dwTgtRoleID)
	{
		return GT_INVALID;
	}

	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	// �������Ƿ���Է�����
	if(!CanPetExchange())
	{
		return E_Pet_Exchange_SelfBusy;
	}

	// �������߼��
	pTarget = pMap->FindRole(dwTgtRoleID);
	if(!P_VALID(pTarget))
	{
		// �������߲�����,���������߲���ͬһ�ŵ�ͼ��
		return E_Pet_Exchange_NotInSame_Map;
	}

	// ���������Ƿ�ɽ���
	if(!pTarget->CanPetExchange())
	{
		return E_Pet_Exchange_RoleBusy;
	}

	// ���׾���
	if(!IsInDistance(*pTarget, MAX_EXCHANGE_DISTANCE))
	{
		return E_Pet_Exchange_OutOfRange;
	}

	// ���������߽���״̬
	BeginPetExchange(dwTgtRoleID);

	// ���ñ�������״̬
	pTarget->GetPetExchMgr().SetTgtRoleID(GetID());

	return E_Pets_Success;
}

//------------------------------------------------------------------------
// ��Ҷ������׵ķ���
//------------------------------------------------------------------------
DWORD Role::ProcPetExchangeReqRes(OUT Role* &pApplicant, DWORD dwTgtRoleID, DWORD dwErrorCode)
{
	// �Ƿ��Լ����Լ�����
	if(GetID() == dwTgtRoleID)
	{
		return GT_INVALID;
	}

	// �жϽ��׶����Ƿ���ȷ
	if(GetPetExchMgr().GetTgtRoleID() != dwTgtRoleID)
	{
		return GT_INVALID;
	}

	//// �ٴ��ж��Ƿ�æ
	//if(!CanExchange())
	//{
	//	return E_Exchange_RoleBusy;
	//}

	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	// ���������߼��
	pApplicant = pMap->FindRole(dwTgtRoleID);
	if(!P_VALID(pApplicant))
	{
		// ���������ߣ����߸�����ͼ
		return E_Pet_Exchange_NotInSame_Map;
	}

	// �����ߵĽ���Ŀ���Ƿ�仯
	PetExchangeMgr &exchMgrApp = pApplicant->GetPetExchMgr();
	if(pApplicant->IsPetExchanging() && exchMgrApp.GetTgtRoleID() != GetID())
	{
		return E_Pet_Exchange_ApplicantTgt_Change;
	}

	// ��齻�׷�����
	if(dwErrorCode != E_Pet_Exchange_Accept)
	{
		// �������׷����ߵĽ���״̬
		pApplicant->EndPetExchange();
		return dwErrorCode;
	}

	// �����߽���״̬���
	if(!pApplicant->IsPetExchanging())
	{
		exchMgrApp.SetTgtRoleID(GT_INVALID);
		return GT_INVALID;
	}

	// ���׾���
	if(!IsInDistance(*pApplicant, MAX_EXCHANGE_DISTANCE))
	{
		return E_Pet_Exchange_OutOfRange;
	}

	// ���������߽���״̬
	BeginPetExchange(dwTgtRoleID);

	return E_Pet_Exchange_Accept;
}

//------------------------------------------------------------------------
// ��ӽ�����Ʒ
//------------------------------------------------------------------------
DWORD Role::ProcPetExchangeAdd( OUT Role* &pTarget, DWORD dwPetID )
{
	DWORD dwErrorCode = ProcPreparePetExchange(pTarget);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	// ȡ�ý�����Ʒ
	PetSoul* pSoul = GetPetPocket()->GetPetSoul(dwPetID);
	if(!P_VALID(pSoul))
	{
		ASSERT(P_VALID(pSoul));
		return GT_INVALID;
	}

	// ��Ʒ�Ƿ�ɱ���
	DWORD dwRtv = GetPetPocket()->CanExchange(dwPetID);
	if(dwRtv != E_Success)
	{
		return dwRtv;
	}

	if( pSoul->GetProto()->nRoleLvlLim > pTarget->GetLevel())
	{	
		return E_Pet_Exchange_RoleLvlNotEnough;
	}

	// ���뵽��������Ʒ��
	dwErrorCode = GetPetExchMgr().AddPet(dwPetID);
	if(E_Success != dwErrorCode)
	{
		// ���ڽ����б��У�������Ʒ�����Ѵﵽ����
		return GT_INVALID;
	}

	// ���Ŀ������״̬
	pTarget->GetPetExchMgr().Unlock();

	return E_Success;
}

//------------------------------------------------------------------------
// ȡ��������Ʒ
//------------------------------------------------------------------------
DWORD Role::ProcPetExchangeDec( OUT Role* &pTarget, DWORD dwPetID )
{
	DWORD dwErrorCode = ProcPreparePetExchange(pTarget);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	// ���Ŀ������״̬
	pTarget->GetPetExchMgr().Unlock();

	return GetPetExchMgr().DecPet(dwPetID);
}

//------------------------------------------------------------------------
// �޸Ľ��׽�Ǯ
//------------------------------------------------------------------------
DWORD Role::ProcPetExchangeMoney(OUT Role* &pTarget, INT64 n64Silver)
{
	DWORD dwErrorCode = ProcPreparePetExchange(pTarget);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	// ���Ŀ������״̬
	pTarget->GetPetExchMgr().Unlock();

	// �������Ƿ����㹻�Ľ�Ǯ
	CurrencyMgr &CurMgr = GetCurMgr();
	if(n64Silver > CurMgr.GetBagSilver())
	{
		return E_Pet_Exchange_NotEnough_Money;
	}

	GetPetExchMgr().ResetMoney(n64Silver);

	return E_Success;
}

//------------------------------------------------------------------------
// ��������
//------------------------------------------------------------------------
DWORD Role::ProcPetExchangeLock(OUT Role* &pTarget)
{
	DWORD dwErrorCode = ProcPreparePetExchange(pTarget);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	GetPetExchMgr().Lock();

	return E_Success;
}

//------------------------------------------------------------------------
// ���ȡ������
//------------------------------------------------------------------------
DWORD Role::ProcPetExchangeCancel(OUT Role* &pTarget)
{
	// �Ƿ��ڽ���״̬
	if(!IsPetExchanging())
	{
		return GT_INVALID;
	}

	// ��ý��׹������ͽ��׶���id
	PetExchangeMgr &exchMgr = GetPetExchMgr();
	DWORD dwTargetRoleID = exchMgr.GetTgtRoleID();

	// ȡ������
	EndPetExchange();

	//// ���������Ƿ�������״̬
	//if(exchMgr.IsLock())
	//{
	//	return GT_INVALID;
	//}

	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	pTarget = pMap->FindRole(dwTargetRoleID);
	if(!P_VALID(pTarget))
	{
		// �������߲�����,���������߲���ͬһ�ŵ�ͼ��
		return E_Pet_Exchange_NotInSame_Map;
	}

	// Ŀ��û�д��ڽ���״̬�������ںͱ��˽���
	if(!pTarget->IsPetExchanging() || pTarget->GetPetExchMgr().GetTgtRoleID() != GetID())
	{
		return E_Pet_Exchange_ApplicantTgt_Change;
	}

	pTarget->EndPetExchange();

	return E_Success;
}

//------------------------------------------------------------------------
// ȷ�Ͻ���
//------------------------------------------------------------------------
DWORD Role::ProcPetExchangeVerify(OUT Role* &pTarget, OUT DWORD &dwFailedRoleID)
{
	DWORD dwErrorCode = E_Success;

	// �Ƿ��ڽ���״̬
	if(!IsPetExchanging())
	{
		return GT_INVALID;
	}

	// ��ý��׹�����
	PetExchangeMgr &exchMgr = GetPetExchMgr();

	// ���������Ƿ�������״̬
	if(!exchMgr.IsLock())
	{
		return GT_INVALID;
	}

	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	pTarget = pMap->FindRole(exchMgr.GetTgtRoleID());
	if(!P_VALID(pTarget))
	{
		// �������߲�����,���������߲���ͬһ�ŵ�ͼ��
		dwFailedRoleID	= exchMgr.GetTgtRoleID();
		dwErrorCode		= E_Pet_Exchange_NotInSame_Map;
		goto Exit;
	}

	// Ŀ��û�д��ڽ���״̬�������ںͱ��˽���
	if(!pTarget->IsPetExchanging() || pTarget->GetPetExchMgr().GetTgtRoleID() != GetID())
	{
		//return E_Exchange_ApplicantTgt_Change;
		return GT_INVALID;
	}

	// Ŀ���Ƿ�������״̬
	if(!pTarget->GetPetExchMgr().IsLock())
	{
		return GT_INVALID;
	}

	exchMgr.Verify();
	if(!pTarget->GetPetExchMgr().IsVerify())
	{
		return GT_INVALID;
	}

	// ���׾���ȷ��
	if(!IsInDistance(*pTarget, MAX_EXCHANGE_DISTANCE))
	{
		dwErrorCode = E_Pet_Exchange_OutOfRange;
		goto Exit;
	}

	// �����ռ��� -- �����ռ䲻��ʱ���������������
	BYTE byMyPetNum	= exchMgr.GetPetNum();
	BYTE byTgtPetNum	= pTarget->GetPetExchMgr().GetPetNum();
	if(byMyPetNum > byTgtPetNum)
	{
		// ���Է�����
		if(byMyPetNum - byTgtPetNum > pTarget->GetPetPocket()->GetFreeSize())
		{
			// ����
			exchMgr.Unlock();
			pTarget->GetPetExchMgr().Unlock();

			dwFailedRoleID = pTarget->GetID();
			return E_Pet_Exchange_NotEnough_Space;
		}
	}
	else
	{
		// ����Լ�����
		if(byTgtPetNum - byMyPetNum >GetPetPocket()->GetFreeSize())
		{
			// ����
			exchMgr.Unlock();
			pTarget->GetPetExchMgr().Unlock();

			dwFailedRoleID = GetID();
			return E_Pet_Exchange_NotEnough_Space;
		}
	}

	/*************************************************
	* ������Ʒ����
	*************************************************/

	PetSoul *pSrcItem[MAX_EXCHANGE_ITEM];
	PetSoul *pDstItem[MAX_EXCHANGE_ITEM];
	ZeroMemory(pSrcItem, sizeof(tagItem*) * MAX_EXCHANGE_ITEM);
	ZeroMemory(pDstItem, sizeof(tagItem*) * MAX_EXCHANGE_ITEM);

	// ������Ʒ���
	dwErrorCode = VerifyPetExchangeData();
	if(dwErrorCode != E_Success)
	{
		dwFailedRoleID = GetID();
		goto Exit;
	}

	dwErrorCode = pTarget->VerifyPetExchangeData();
	if(dwErrorCode != E_Success)
	{
		dwFailedRoleID = pTarget->GetID();
		goto Exit;
	}
 
 	// 4.�ƶ���Ʒ

	// 4.1 ��Ǯ -- �ȼ���Ǯ,������ʧ
	GetCurMgr().DecBagSilver(exchMgr.GetMoney(), ELCID_Exchange_Verify, pTarget->GetID());
	pTarget->GetCurMgr().DecBagSilver(pTarget->GetPetExchMgr().GetMoney(), ELCID_Exchange_Verify, GetID());

	GetCurMgr().IncBagSilver(
		min(GetCurMgr().GetCanIncBagSilver(), pTarget->GetPetExchMgr().GetMoney()), 
		ELCID_Exchange_Verify, pTarget->GetID());

	pTarget->GetCurMgr().IncBagSilver(
		min(pTarget->GetCurMgr().GetCanIncBagSilver(), exchMgr.GetMoney()), 
		ELCID_Exchange_Verify, GetID());

	// 4.2 ��Ʒ

	// 4.2.1 �ȴ��������ɾ��������Ʒ -- ����ɶѵ���Ʒ�������
	GetPetPocket()->TakeFromPocket(pSrcItem, MAX_EXCHANGE_ITEM, exchMgr.GetPetIDs(), MAX_EXCHANGE_ITEM);
	pTarget->GetPetPocket()->TakeFromPocket(pDstItem, MAX_EXCHANGE_ITEM, pTarget->GetPetExchMgr().GetPetIDs(), MAX_EXCHANGE_ITEM);

	// 4.2.2 ��������Ʒ�ŵ��������
	GetPetPocket()->PutInPocket(pDstItem, MAX_EXCHANGE_ITEM);
	pTarget->GetPetPocket()->PutInPocket(pSrcItem, MAX_EXCHANGE_ITEM);

Exit:
	// �ͷŽ�����Դ
	EndPetExchange();

	if(P_VALID(pTarget))
	{
		pTarget->EndPetExchange();
	}
	else
	{
		// �������Ŀ����������ͼ���ô�Ҳ����ֱ�Ӳ���(���̵߳���)
	}

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ���������ϵĽ�����Ʒ�Ƿ���ȫ
//------------------------------------------------------------------------
DWORD Role::VerifyPetExchangeData()
{
	// ��ý��׹�����
	PetExchangeMgr &exchMgr = GetPetExchMgr();

	// 1.��Ǯ�Ƿ񻹹�
	if(GetCurMgr().GetBagSilver() < exchMgr.GetMoney())
	{
		return E_Pet_Exchange_NotEnough_Money;
	}

	//// 2.�����Ƿ����㹻�Ŀռ�
	//if(GetItemMgr().GetBagFreeSize() < exchMgr.GetItemTypeNum())
	//{
	//	return E_Exchange_NotEnough_BagSpace;
	//}

	// 3.��������Ʒ�Ƿ�������, ���ڣ��õ���Ʒָ��
	if(!GetPetPocket()->CheckExistInPocket(exchMgr.GetPetIDs(), MAX_EXCHANGE_ITEM))
	{
		return E_Pet_Exchange_PetCanNot_Find;
	}

	return E_Success;
}

//------------------------------------------------------------------------
// �޸Ľ�������ǰ�ļ�飬���õ���ض���
//------------------------------------------------------------------------
DWORD Role::ProcPreparePetExchange(OUT Role* &pTarget)
{
	// �Ƿ��ڽ���״̬
	if(!IsPetExchanging())
	{
		return GT_INVALID;
	}

	// ��ý��׹�����
	PetExchangeMgr &exchMgr = GetPetExchMgr();

	// ���������Ƿ�������״̬
	if(exchMgr.IsLock())
	{
		return GT_INVALID;
	}

	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	pTarget = pMap->FindRole(exchMgr.GetTgtRoleID());
	if(!P_VALID(pTarget))
	{
		// �������߲�����,���������߲���ͬһ�ŵ�ͼ��
		return E_Pet_Exchange_NotInSame_Map;
	}

	// Ŀ��û�д��ڽ���״̬�������ںͱ��˽���
	if(!pTarget->IsPetExchanging() || pTarget->GetPetExchMgr().GetTgtRoleID() != GetID())
	{
		return E_Pet_Exchange_ApplicantTgt_Change;
	}

	return E_Success;
}