#include "StdAfx.h"
#include "player_session.h"
#include "../WorldDefine/exchange_define.h"
#include "../WorldDefine/msg_pet_exchange.h"
#include "map.h"
#include "role.h"

//------------------------------------------------------------------------
// ������
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRolePetExchangeReq(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_PetExchangeReq);

	if(GT_INVALID == pRecv->dwDstID)
	{
		return GT_INVALID;
	}

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role* pTarget = NULL;
	DWORD dwErrorCode = pRole->ProcPetExchangeReq(pTarget, pRecv->dwDstID);
	if(E_Success == dwErrorCode)
	{
		if(P_VALID(pTarget))
		{
			// �������߷���Ϣ
			tagNS_PetExchangeReq send;
			send.dwSrcID = pRole->GetID();
			pTarget->SendMessage(&send, send.dwSize);
		}
		else
		{
			// ��Զ����ִ�е���
			ASSERT(P_VALID(pTarget));
		}
	}
	else if(dwErrorCode != GT_INVALID)
	{
		//// �������߷���
		//tagNS_ExchangeFinish send;
		//send.dwErrorCode	= dwErrorCode;
		//send.dwFailedRoleID	= pRecv->dwDstID;
		//SendMessage(&send, send.dwSize);

		tagNS_PetExchangeReqRes send;
		send.dwErrorCode	= dwErrorCode;
		send.dwSrcID		= pRole->GetID();
		send.dwDstID		= pRecv->dwDstID;
		SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ��Ҷ������׵ķ���
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRolePetExchangeReqRes(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_PetExchangeReqRes);

	if(GT_INVALID == pRecv->dwDstID)
	{
		return GT_INVALID;
	}

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role *pApplicant = NULL;
	DWORD dwErrorCode = pRole->ProcPetExchangeReqRes(pApplicant, pRecv->dwDstID, pRecv->dwErrorCode);

	// ����Ƿ���Ҫ���ý���״̬
	if(dwErrorCode != E_Pet_Exchange_Accept)
	{
		pRole->GetPetExchMgr().SetTgtRoleID(GT_INVALID);
	}

	// ����Ƿ���Ҫ��������Ϣ
	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	// Ϊ�򻯷��������߼��������������˫��������Ϣ���ɿͻ��˴���
	tagNS_PetExchangeReqRes send;
	send.dwSrcID = pRole->GetID();
	send.dwDstID = pRecv->dwDstID;
	send.dwErrorCode = dwErrorCode;

	if(E_Pet_Exchange_Accept == pRecv->dwErrorCode)
	{
		SendMessage(&send, send.dwSize);
	}

	if(P_VALID(pApplicant))
	{
		pApplicant->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ��ӽ�����Ʒ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRolePetExchangeAdd(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_PetExchangeAdd);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role	*pTarget	= NULL;
	DWORD	dwErrorCode = pRole->ProcPetExchangeAdd(pTarget, pRecv->dwPetID);

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	if(E_Success == dwErrorCode)
	{
		tagNS_PetExchangeAdd_2Dst send2Dst;
		send2Dst.dwSrcID	= pRole->GetID();
		send2Dst.dwPetID	= pRecv->dwPetID;

		pTarget->SendMessage(&send2Dst, send2Dst.dwSize);
	}

	tagNS_PetExchangeAdd_2Src send2Src;
	send2Src.dwDstID		= P_VALID(pTarget) ? pTarget->GetID() : GT_INVALID;
	send2Src.dwPetID		= pRecv->dwPetID;
	send2Src.dwErrorCode	= dwErrorCode;

	SendMessage(&send2Src, send2Src.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ȡ��������Ʒ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRolePetExchangeDec(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_PetExchangeDec);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role *pTarget = NULL;
	DWORD dwErrorCode = pRole->ProcPetExchangeDec(pTarget, pRecv->dwPetID);
	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_PetExchangeDec send;
	send.dwSrcID	= pRole->GetID();
	send.dwDstID	= P_VALID(pTarget) ? pTarget->GetID() : GT_INVALID;
	send.dwPetID	= pRecv->dwPetID;

	// �ӽ����嵥��ɾ��
	send.dwErrorCode = dwErrorCode;

	SendMessage(&send, send.dwSize);

	if(P_VALID(pTarget))
	{
		tagNS_PetExchangeDec_2Dst send2Dst;
		send2Dst.dwSrcID	= pRole->GetID();
		send2Dst.dwPetID	= pRecv->dwPetID;

		pTarget->SendMessage(&send2Dst, send2Dst.dwSize);
	}

	return dwErrorCode;
}

//------------------------------------------------------------------------
// �޸Ľ��׽�Ǯ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRolePetExchangeMoney(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_PetExchangeMoney);

	if(pRecv->n64Money < 0)
	{
		return GT_INVALID;
	}

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role *pTarget = NULL;
	DWORD dwErrorCode = pRole->ProcPetExchangeMoney(pTarget, pRecv->n64Money);
	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_PetExchangeMoney send;
	send.dwSrcID	= pRole->GetID();
	send.n64Money	= pRecv->n64Money;
	send.dwErrorCode= dwErrorCode;

	SendMessage(&send, send.dwSize);
	if(P_VALID(pTarget))
	{
		pTarget->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ��������
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRolePetExchangeLock(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_PetExchangeLock);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role *pTarget = NULL;
	DWORD dwErrorCode = pRole->ProcPetExchangeLock(pTarget);
	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_PetExchangeLock send;
	send.dwSrcID = pRole->GetID();
	send.dwErrorCode = dwErrorCode;

	SendMessage(&send, send.dwSize);
	if(P_VALID(pTarget))
	{
		pTarget->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ���ȡ������
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRolePetExchangeCancel(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_PetExchangeCancel);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role *pTarget = NULL;
	DWORD dwErrorCode = pRole->ProcPetExchangeCancel(pTarget);

	tagNS_PetExchangeCancel send;
	send.dwSrcID = pRole->GetID();

	SendMessage(&send, send.dwSize);
	if(P_VALID(pTarget) && E_Success == dwErrorCode)
	{
		pTarget->SendMessage(&send, send.dwSize);
	}

	return E_Success;
}

//------------------------------------------------------------------------
// ȷ�Ͻ���
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRolePetExchangeVerify(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_PetExchangeVerify);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role *pTarget = NULL;
	DWORD dwFailedRoleID = GT_INVALID;
	DWORD dwErrorCode = pRole->ProcPetExchangeVerify(pTarget, dwFailedRoleID);
	if(GT_INVALID == dwErrorCode)
	{
		// ������ͻ��˷���
		return dwErrorCode;
	}

	tagNS_PetExchangeFinish send;
	send.dwErrorCode	= dwErrorCode;
	send.dwFailedRoleID	= dwFailedRoleID;

	SendMessage(&send, send.dwSize);
	if(P_VALID(pTarget))
	{
		pTarget->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}