//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_purchase..cpp
// author: sunnee
// actor:
// data: 2009-07-13
// last:
// brief: �����Ź���
//-------------------------------------------------------------------------------------------------------

#include "StdAfx.h"

#include "../WorldDefine/mall_define.h"
#include "../WorldDefine/msg_mall.h"
#include "../WorldDefine/ItemDefine.h"

#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_mall.h"
#include "../ServerDefine/msg_log.h"

#include "guild_purchase.h"
#include "guild.h"
#include "att_res.h"
#include "item_creator.h"
#include "mall.h"
#include "role_mgr.h"
#include "currency.h"
#include "role.h"
#include "guild_mgr.h"

//-------------------------------------------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------------------------------------------
GuildPurchase::GuildPurchase()
{
	m_pGuild = NULL;
	m_mapGPInfo.Clear();
}

GuildPurchase::~GuildPurchase()
{
	Destory();
}

//-------------------------------------------------------------------------------------------------------
// ��ʼ�������¡�����
//-------------------------------------------------------------------------------------------------------
BOOL GuildPurchase::Init( DWORD dwGuildID )
{
	m_mapGPInfo.Clear();

	// �ϲ��豣֤pGuild�ĺϷ���
	ASSERT(GT_VALID(dwGuildID));
	m_pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(m_pGuild))
	{
		return FALSE;
	}

	return TRUE;
}

VOID GuildPurchase::Update(DWORD dwTime)
{
	INT64 nMapKey = GT_INVALID;
	tagGroupPurchase* pGPInfo = NULL;

	MapGPInfo::TMapIterator iter = m_mapGPInfo.Begin();
	while (m_mapGPInfo.PeekNext(iter, nMapKey, pGPInfo))
	{
		// ����24Сʱ���Ź�ʧ��
		pGPInfo->dwRemainTime -= dwTime;
		if ((INT32)(pGPInfo->dwRemainTime) <= 0)
		{
			RemoveGroupPurchaseInfo(pGPInfo, FALSE);
		}
	}

	// �ϲ��ж�map�Ƿ�Ϊ��
}

VOID GuildPurchase::Destory()
{
	tagGroupPurchase* pGPInfo = NULL;
	MapGPInfo::TMapIterator iter = m_mapGPInfo.Begin();
	while (m_mapGPInfo.PeekNext(iter, pGPInfo))
	{
		SAFE_DEL(pGPInfo);
	}
	m_mapGPInfo.Clear();
}

//-------------------------------------------------------------------------------------------------------
// �Ź���Ϣ����
//-------------------------------------------------------------------------------------------------------
BOOL GuildPurchase::Add( tagGroupPurchase* pGPInfo, BOOL bNotify2DB /*= TRUE*/ )
{
	ASSERT(pGPInfo);
	if (!P_VALID(pGPInfo))
	{
		return FALSE;
	}

	// ��֤����
	if (pGPInfo->dwGuildID != m_pGuild->GetGuildAtt().dwID)
	{
		return FALSE;
	}

	// ����
	INT64 nKey = GetKey(pGPInfo->dwRoleID, pGPInfo->dwMallID);
	if (m_mapGPInfo.IsExist(nKey))
	{
		return FALSE;
	}

	m_mapGPInfo.Add(nKey, pGPInfo);

	// ֪ͨ���ݿ�
	if (bNotify2DB)
	{
		AddGPInfo2DB(pGPInfo);
	}

	return TRUE;
}

BOOL GuildPurchase::Remove( tagGroupPurchase* pGPInfo, BOOL bNotify2DB /*= TRUE*/ )
{
	ASSERT(pGPInfo);
	if (!P_VALID(pGPInfo))
	{
		return FALSE;
	}

	// ��֤����
	if (pGPInfo->dwGuildID != m_pGuild->GetGuildAtt().dwID)
	{
		return FALSE;
	}

	// ����
	BOOL bRet = m_mapGPInfo.Erase(GetKey(pGPInfo->dwRoleID, pGPInfo->dwMallID));

	// ֪ͨ���ݿ�
	if (bRet && bNotify2DB)
	{
		RemoveGPInfo2DB(pGPInfo);
	}

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD GuildPurchase::GetAllSimGPInfo( INT &nGPInfoNum, tagSimGPInfo* pData )
{
	if (m_mapGPInfo.Empty())
	{
		return E_GroupPurchase_NoInfo;
	}

	tagGroupPurchase* pGPInfo = NULL;
	nGPInfoNum = 0;

	MapGPInfo::TMapIterator iter = m_mapGPInfo.Begin();
	while (m_mapGPInfo.PeekNext(iter, pGPInfo))
	{
		if (!P_VALID(pGPInfo))
		{
			// ������
			ASSERT(pGPInfo);
			IMSG(_T("\nthere is a error GP Info in map.\n"));
			continue;
		}
		pData[nGPInfoNum].dwGuildID			= pGPInfo->dwGuildID;
		pData[nGPInfoNum].dwRoleID			= pGPInfo->dwRoleID;
		pData[nGPInfoNum].dwMallID			= pGPInfo->dwMallID;
		pData[nGPInfoNum].nPrice			= pGPInfo->nPrice;
		pData[nGPInfoNum].nParticipatorNum	= pGPInfo->nParticipatorNum;
		pData[nGPInfoNum].nRequireNum		= pGPInfo->nRequireNum;
		pData[nGPInfoNum].dwRemainTime		= pGPInfo->dwRemainTime;

		nGPInfoNum++;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡָ���Ź�����Ӧ���б�
//-------------------------------------------------------------------------------------------------------
DWORD GuildPurchase::GetParticipators( DWORD dwID, DWORD dwRoleID, DWORD *pData )
{
	if (m_mapGPInfo.Empty())
	{
		return E_GroupPurchase_NoInfo;
	}

	tagGroupPurchase* pGPInfo = NULL;
	INT64 nMapKey = GetKey(dwRoleID, dwID);

	pGPInfo = m_mapGPInfo.Peek(nMapKey);

	if (!P_VALID(pGPInfo) || !P_VALID(pGPInfo->listParticipators))
	{
		return E_GroupPurchase_NoInfo;
	}

	// û����Ӧ�ߵ��Ź����ݣ���Ӧ�ô���
	if (pGPInfo->listParticipators->Empty())
	{
		return E_GroupPurchase_NoInfo;
	}

	INT i = 0;
	pGPInfo->listParticipators->ResetIterator();
	while (pGPInfo->listParticipators->PeekNext(pData[i++]));

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�Ź���Ϣ����
//-------------------------------------------------------------------------------------------------------
INT GuildPurchase::GetGroupPurchaseInfoNum()
{
	return m_mapGPInfo.Size();
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��Ӧ������
//-------------------------------------------------------------------------------------------------------
INT GuildPurchase::GetParticipatorNum( DWORD dwID, DWORD dwRoleID )
{
	if (m_mapGPInfo.Empty())
	{
		return 0;
	}

	tagGroupPurchase* pGPInfo = NULL;
	INT64 nMapKey = GetKey(dwRoleID, dwID);

	pGPInfo = m_mapGPInfo.Peek(nMapKey);

	if (!P_VALID(pGPInfo) || !P_VALID(pGPInfo->listParticipators))
	{
		return 0;
	}

	return pGPInfo->listParticipators->Size();
}

//-------------------------------------------------------------------------------------------------------
// �����Ź�
//-------------------------------------------------------------------------------------------------------
DWORD GuildPurchase::LaunchGroupPurchase( Role *pRole, DWORD dwID, BYTE byScope, BYTE byIndex, INT nUnitPrice, OUT tagGroupPurchase* &pGPInfo, OUT DWORD& dwItemTypeID )
{
	ASSERT(P_VALID(pRole));
	ASSERT(g_mall.IsInitOK());
	ASSERT(nUnitPrice > 0);

	DWORD dwErrorCode = E_Success;

	// ��������Ϸ���
	const tagMallItemProto *pProto = g_mall.GetMallItem(byIndex, EMIT_Item)->pMallItem;
	if (!P_VALID(pProto))
	{
		return GT_INVALID;
	}

	// ������Ʒ�Ƿ�����Ź�
	switch(byScope)
	{
	case EGPS_SMALL:
		if (pProto->bySmallGroupDiscount == (BYTE)GT_INVALID)
		{
			return E_GroupPurchase_ItemNotAllowable;
		}
		break;

	case EGPS_MEDIUM:
		if (pProto->byMediumGroupDiscount == (BYTE)GT_INVALID)
		{
			return E_GroupPurchase_ItemNotAllowable;
		}
		break;

	case EGPS_BIG:
		if (pProto->byBigGroupDiscount == (BYTE)GT_INVALID)
		{
			return E_GroupPurchase_ItemNotAllowable;
		}
		break;

	default:
		return GT_INVALID;
		break;
	}

	// �ϲ㸺�����������
	INT32 nGroupPurchase = m_pGuild->GetGuildAtt().nGroupPurchase;

	// id
	if(pProto->dwID != dwID)
	{
		return E_Mall_ID_Error;
	}

	// ����Ƿ��Ѿ�����������Ʒ���Ź�
	INT64 nMapKey = GetKey(pRole->GetID(), dwID);
	if (m_mapGPInfo.IsExist(nMapKey))
		return E_GroupPurchase_AlreadyInitiate;

	INT nPrice = 0;

	// ����
	if(pProto->dwTimeSaleEnd != GT_INVALID && g_world.GetWorldTime() < pProto->dwTimeSaleEnd)
	{
		// ����
		nPrice = pProto->nSalePrice;
	}
	else
	{
		// ����
		nPrice = pProto->nPrice;
	}

	// ��Ǯ
	if(nPrice != nUnitPrice)
	{
		return E_Mall_YuanBao_Error;
	}

	// �����Ź��۸�
	nPrice *= pProto->byGroupPurchaseAmount;

	switch(byScope)
	{
	case EGPS_SMALL:
		nPrice = (INT)(nPrice * (pProto->bySmallGroupDiscount/100.0f - 0.2f*nGroupPurchase/100000));
		break;

	case EGPS_MEDIUM:
		nPrice = (INT)(nPrice * (pProto->byMediumGroupDiscount/100.0f - 0.2f*nGroupPurchase/100000));
		break;

	case EGPS_BIG:
		nPrice = (INT)(nPrice * (pProto->byBigGroupDiscount/100.0f - 0.2f*nGroupPurchase/100000));
		break;
	}

	// ������Ԫ���Ƿ��㹻
	if(nPrice > pRole->GetCurMgr().GetBagYuanBao() || nPrice <= 0)
	{
		return E_BagYuanBao_NotEnough;
	}

	// ���ô�������
	pGPInfo = new tagGroupPurchase;
	if (!P_VALID(pGPInfo))
	{
		ASSERT(pGPInfo);
		return GT_INVALID;
	}

	pGPInfo->dwGuildID		= pRole->GetGuildID();
	pGPInfo->dwRoleID		= pRole->GetID();
	pGPInfo->dwMallID		= dwID;
	pGPInfo->nPrice			= nPrice;
	pGPInfo->dwRemainTime	= pProto->dwPersistTime * 60 * 60; // ����Ϊ��λ

	switch (byScope)
	{
	case EGPS_SMALL:
		pGPInfo->nRequireNum = pProto->bySmallGroupHeadcount;
		break;

	case EGPS_MEDIUM:
		pGPInfo->nRequireNum = pProto->byMediumGroupHeadcount;
		break;

	case EGPS_BIG:
		pGPInfo->nRequireNum = pProto->byBigGroupHeadcount;
		break;
	}
	pGPInfo->nParticipatorNum = 1;
	pGPInfo->listParticipators = new TList<DWORD>;
	if (!P_VALID(pGPInfo->listParticipators))
	{
		ASSERT(pGPInfo->listParticipators);
		SAFE_DEL(pGPInfo);
		return GT_INVALID;
	}
	pGPInfo->listParticipators->PushBack(pGPInfo->dwRoleID);

	// �����Ź���Ϣ��������
	Add(pGPInfo);

	// ������ƷTypeID
	dwItemTypeID = pProto->dwTypeID;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��Ӧ�Ź�
//-------------------------------------------------------------------------------------------------------
DWORD GuildPurchase::RespondGroupPurchase( Role *pRole, DWORD dwID, DWORD dwRoleID, INT nPrice, OUT tagGroupPurchase* &pGPInfo )
{
	// ���������Ϣ
	if (!GT_VALID(pRole->GetGuildID()))
		return E_GroupPurchase_NotInGuild;

	if (!P_VALID(m_pGuild->GetMember(pRole->GetID())))
		return E_GroupPurchase_NotMember;

	// �����Ƿ��Ƿ�����
	if (dwRoleID == pRole->GetID())
	{
		return E_GroupPurchase_IsInitiate;
	}

	// ����ָ�����Ź��Ƿ��Ѿ�����
	INT64 nMapKey = GetKey(dwRoleID, dwID);
	pGPInfo = m_mapGPInfo.Peek(nMapKey);

	if (!P_VALID(pGPInfo) || (INT32)(pGPInfo->dwRemainTime) <= 0)
		return E_GroupPurchase_AlreadyEnd;

	// �Ƿ���ϢӦ��ɾ��
	if (!P_VALID(pGPInfo->listParticipators))
		return E_GroupPurchase_AlreadyEnd;

	// �����Ƿ��Ѿ���Ӧ������Ź�
	DWORD dwTmpRoleID = pRole->GetID();
	if (pGPInfo->listParticipators->IsExist(dwTmpRoleID))
	{
		return E_GroupPurchase_AlreadyInitiate;
	}

	// ��Ǯ
	if(nPrice != pGPInfo->nPrice)
	{
		return E_Mall_YuanBao_Error;
	}

	// ������Ԫ���Ƿ��㹻
	if(nPrice > pRole->GetCurMgr().GetBagYuanBao() || nPrice <= 0)
	{
		return E_BagYuanBao_NotEnough;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �����Ź���Ʒ
//-------------------------------------------------------------------------------------------------------
DWORD GuildPurchase::CreateGPItems( DWORD dwID, OUT tagMallItemSell &itemSell )
{
	ASSERT(GT_VALID(dwID));

	//��֤��Ʒ�ĺϷ���
	const tagMallItemProto *pProto = g_attRes.GetMallItemProto(dwID);

	if (!P_VALID(pProto))
	{
		return E_Mall_ID_Error;
	}

	// ������Ʒ
	tagItem *pItemNew = ItemCreator::CreateEx(EICM_Mall, GT_INVALID, pProto->dwTypeID, pProto->byGroupPurchaseAmount, EIQ_White);
	if(!P_VALID(pItemNew))
	{
		ASSERT(P_VALID(pItemNew));
		return E_Mall_CreateItem_Failed;
	}

	// �������Ʒ����������Ʒ
	tagItem *pPresentNew = NULL;
	if(pProto->dwPresentID != GT_INVALID)
	{
		pPresentNew = ItemCreator::CreateEx(EICM_Mall, GT_INVALID, 
			pProto->dwPresentID, (INT16)pProto->byPresentNum * pProto->byGroupPurchaseAmount, EIQ_White);
		if(!P_VALID(pPresentNew))
		{
			::Destroy(pItemNew);
			ASSERT(P_VALID(pPresentNew));
			return E_Mall_CreatePres_Failed;
		}
	}

	// ��������
	if (pProto->byExAssign >= 0)
	{
		itemSell.nExVolumeAssign = pProto->byExAssign;
	}

	// ���ô�������
	itemSell.pItem			= pItemNew;
	itemSell.pPresent		= pPresentNew;
	itemSell.nYuanBaoNeed	= 0;				// �Ѿ�Ԥ����
	itemSell.byRemainNum	= GT_INVALID;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ɾ��ָ���Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
VOID GuildPurchase::RemoveGroupPurchaseInfo( tagGroupPurchase* &pGPInfo, BOOL bSuccess /*= TRUE*/ )
{
	ASSERT(pGPInfo);

	if (!P_VALID(pGPInfo))
		return;

	// ɾ���������е���Ϣ
	INT64 nMapKey = GetKey(pGPInfo->dwRoleID, pGPInfo->dwMallID);
	m_mapGPInfo.Erase(nMapKey);

	// �����Ź�ʧ���򷵻����Ԫ��
	if (!bSuccess)
	{
		ReturnCost2Participator(pGPInfo);

		m_pGuild->ModifyGroupPurchase(FALSE);

		// �㲥�Ź�ʧ����Ϣ
		const tagMallItemProto *pProto = g_attRes.GetMallItemProto(pGPInfo->dwMallID);

		if (!P_VALID(pProto))
		{
			// ���ﲻ�ᷢ���ɣ��Է���һ
			ASSERT(pProto);
			return;
		}

		tagNS_RespondBroadCast send;
		send.eType = ERespondBroadCast_Lose;
		send.dwRoleID = pGPInfo->dwRoleID;
		send.dwTypeID = pProto->dwTypeID;
		m_pGuild->SendGuildMsg(&send, send.dwSize);
	}

	// ɾ�����ݿ��е���Ϣ
	RemoveGPInfo2DB(pGPInfo);

	// ɾ����Ϣ����
	SAFE_DEL(pGPInfo);

	// �ϲ����Ƿ�ɾ��this����
}

//-------------------------------------------------------------------------------------------------------
// ɾ���ð��������Ź���Ϣ(���ɽ�ɢ)
//-------------------------------------------------------------------------------------------------------
VOID GuildPurchase::RemoveGroupPurchaseInfo()
{
	// �ð��������Ź�ʧ��
	tagGroupPurchase *pGPInfo = NULL;
	if (!m_mapGPInfo.Empty())
	{
		MapGPInfo::TMapIterator iter = m_mapGPInfo.Begin();
		while (m_mapGPInfo.PeekNext(iter, pGPInfo))
		{
			ReturnCost2Participator(pGPInfo);
			SAFE_DEL(pGPInfo);
		}
	}
	m_mapGPInfo.Clear();

	// ɾ�����ݿ��иð��ɵ��Ź���Ϣ
	RemoveGuildGPInfo2DB();

	// �ϲ����Ƿ���Ҫɾ��this����
}

//-------------------------------------------------------------------------------------------------------
// �������Ԫ��(�Ź�ʧ��)
//-------------------------------------------------------------------------------------------------------
VOID GuildPurchase::ReturnCost2Participator( tagGroupPurchase* pGPInfo )
{
	if (!P_VALID(pGPInfo) || !P_VALID(pGPInfo->listParticipators))
		return;

	DWORD tmpRoleID = GT_INVALID;

	pGPInfo->listParticipators->ResetIterator();
	while(pGPInfo->listParticipators->PeekNext(tmpRoleID))
	{
		// �������ǹ�����
		Role* pRole = g_roleMgr.GetRolePtrByID(tmpRoleID);
		if (!P_VALID(pRole))
		{
			// ��������ҷ���Ԫ��
			CurrencyMgr::ModifyBaiBaoYuanBao(tmpRoleID, pGPInfo->nPrice, ELCID_GroupPurchase_Faild);
		}
		else
		{
			// ��������ҷ���Ԫ��
			pRole->GetCurMgr().IncBaiBaoYuanBao(pGPInfo->nPrice, ELCID_GroupPurchase_Faild);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// ���ݿ����
//-------------------------------------------------------------------------------------------------------
VOID GuildPurchase::AddGPInfo2DB( tagGroupPurchase* pGPInfo )
{
	ASSERT(P_VALID(pGPInfo));
	if (!P_VALID(pGPInfo))
		return;

	tagNDBC_AddGPInfo send;

	send.GPInfo.dwGuildID			= pGPInfo->dwGuildID;
	send.GPInfo.dwRoleID			= pGPInfo->dwRoleID;
	send.GPInfo.dwMallID			= pGPInfo->dwMallID;
	send.GPInfo.nPrice				= pGPInfo->nPrice;
	send.GPInfo.nParticipatorNum	= pGPInfo->nParticipatorNum;
	send.GPInfo.nRequireNum			= pGPInfo->nRequireNum;
	send.GPInfo.nRemainTime			= pGPInfo->dwRemainTime;
	send.GPInfo.dwParticipators[0]	= pGPInfo->dwRoleID;

	g_dbSession.Send(&send, send.dwSize);
}

VOID GuildPurchase::UpdateGPInfo2DB( tagGroupPurchase* pGPInfo )
{
	ASSERT(P_VALID(pGPInfo));
	if (!P_VALID(pGPInfo))
		return;

	tagNDBC_UpdateGPInfo send;

	send.GpInfoKey.dwGuildID	= pGPInfo->dwGuildID;
	send.GpInfoKey.dwRoleID		= pGPInfo->dwRoleID;
	send.GpInfoKey.dwMallID		= pGPInfo->dwMallID;
	send.dwNewParticipator		= pGPInfo->listParticipators->GetList().back();

	g_dbSession.Send(&send, send.dwSize);
}

VOID GuildPurchase::RemoveGPInfo2DB( tagGroupPurchase* pGPInfo )
{
	ASSERT(P_VALID(pGPInfo));
	if (!P_VALID(pGPInfo))
		return;

	tagNDBC_RemoveGPInfo send;

	send.GpInfoKey.dwGuildID	= pGPInfo->dwGuildID;
	send.GpInfoKey.dwRoleID		= pGPInfo->dwRoleID;
	send.GpInfoKey.dwMallID		= pGPInfo->dwMallID;

	g_dbSession.Send(&send, send.dwSize);
}

VOID GuildPurchase::RemoveGuildGPInfo2DB()
{
	ASSERT(P_VALID(m_pGuild));
	if (!P_VALID(m_pGuild))
	{
		return;
	}

	tagNDBC_RemoveGuildGPInfo send;

	send.dwGuildID = m_pGuild->GetGuildAtt().dwID;

	g_dbSession.Send(&send, send.dwSize);
}

//-------------------------------------------------------------------------------------------------------
// ���ɼ�ֵ
//-------------------------------------------------------------------------------------------------------
INT64 GuildPurchase::GetKey( DWORD dwRoleID, DWORD dwID )
{
	ASSERT(GT_VALID(dwRoleID) && GT_VALID(dwID));
	if (!GT_VALID(dwRoleID) || !GT_VALID(dwID))
		return GT_INVALID;

	INT64 n64Key = dwRoleID;

	n64Key = (n64Key << 32) | dwID;

	return n64Key;
}

//-------------------------------------------------------------------------------------------------------
// �Ź��ɹ�����
//-------------------------------------------------------------------------------------------------------
DWORD GuildPurchase::DoSuccessStuff( tagGroupPurchase* pGPInfo )
{
	// ��һ�¼���֤
	if (m_pGuild->GetGuildAtt().dwID != pGPInfo->dwGuildID)
	{
		return GT_INVALID;
	}

	DWORD dwItemTypeID		= GT_INVALID;
	DWORD dwLaunchRoleID	= pGPInfo->dwRoleID;

	// �������б��е���ҷ�����Ʒ
	DWORD tmpRoleID = GT_INVALID;
	TList<DWORD>::TListIterator iter = pGPInfo->listParticipators->Begin();	// �ϲ��Ѿ���֤���б�ĺϷ���
	
	while(pGPInfo->listParticipators->PeekNext(iter, tmpRoleID))
	{
		// ������Ʒ
		tagMallItemSell	sItemSell;
		DWORD dwErrorCode = CreateGPItems(pGPInfo->dwMallID, sItemSell);
		if (dwErrorCode != E_Success)
		{
			return dwErrorCode;
		}
		else if (P_VALID(sItemSell.pItem))
		{
			dwItemTypeID = sItemSell.pItem->dwTypeID;
		}
		else
		{
			return GT_INVALID;
		}

		// ������
		if(P_VALID(sItemSell.pItem))
		{		
			INT64 n64Serial = sItemSell.pItem->n64Serial;
			DWORD dwFstGainTime = g_world.GetWorldTime();
			INT16 n16BuyNum = sItemSell.pItem->n16Num;

			// �ٱ�����ʷ��¼
			ItemMgr::ProcBaiBaoRecord(sItemSell.pItem->dwTypeID, tmpRoleID, GT_INVALID, EBBRT_GroupPurchase, dwFstGainTime);

			// log
			g_mall.LogMallSell(tmpRoleID, GT_INVALID, *sItemSell.pItem, n64Serial, n16BuyNum, 
				dwFstGainTime, sItemSell.nYuanBaoNeed, 0, ELCLD_GroupPurchase_BuyItem);

			// ����Ʒ�ŵ��ٱ�����
			Role *pTmpRole = g_roleMgr.GetRolePtrByID(tmpRoleID);
			if(P_VALID(pTmpRole))
			{
				pTmpRole->GetItemMgr().Add2BaiBao(sItemSell.pItem, ELCLD_GroupPurchase_BuyItem);
			}
			else
			{
				// �洢��item_baibao����
				ItemMgr::InsertBaiBao2DB(sItemSell.pItem, tmpRoleID, ELCLD_GroupPurchase_BuyItem);

				// ɾ����Ʒ
				::Destroy(sItemSell.pItem);
			}

			// �������Ʒ����ŵ��ٱ�����
			if(P_VALID(sItemSell.pPresent))
			{
				// �ٱ�����ʷ��¼
				ItemMgr::ProcBaiBaoRecord(sItemSell.pPresent->dwTypeID, tmpRoleID, GT_INVALID, EBBRT_Mall, dwFstGainTime);

				if(P_VALID(pTmpRole))
				{
					pTmpRole->GetItemMgr().Add2BaiBao(sItemSell.pPresent, ELCLD_GroupPurchase_BuyItem_Add);
				}
				else
				{
					// �洢��item_baibao����
					ItemMgr::InsertBaiBao2DB(sItemSell.pPresent, tmpRoleID, ELCLD_GroupPurchase_BuyItem_Add);

					// ɾ����Ʒ
					::Destroy(sItemSell.pItem);
				}
			}

			// ��������
			if (P_VALID(pTmpRole) && sItemSell.nExVolumeAssign > 0)
			{
				pTmpRole->GetCurMgr().IncExchangeVolume(sItemSell.nExVolumeAssign, ELCLD_GroupPurchase_BuyItem);
			}
		}
	}

	// �����Ź�ָ��+1
	m_pGuild->ModifyGroupPurchase(TRUE);

	// ɾ�������Ź���Ϣ��¼
	RemoveGroupPurchaseInfo(pGPInfo);

	// �����ڹ㲥�ɹ���Ϣ
	tagNS_RespondBroadCast send;
	send.eType = ERespondBroadCast_Success;
	send.dwRoleID = dwLaunchRoleID;
	send.dwTypeID = dwItemTypeID;
	m_pGuild->SendGuildMsg(&send, send.dwSize);

	return E_Success;
}