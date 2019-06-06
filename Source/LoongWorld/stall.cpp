//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: stall.h
// author: Sxg
// actor:
// data: 2008-12-17
// last:
// brief: ��̯
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/stall_define.h"
#include "../WorldDefine/filter.h"
#include "../WorldDefine/msg_common_errorcode.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/role_data_define.h"

#include "stall.h"
#include "role.h"
#include "item_mgr.h"
#include "item_creator.h"
#include "vip_stall.h"

INT Stall::m_SzStallItem = sizeof(tagMsgStallGoods) - 1 + SIZE_ITEM;
INT Stall::m_SzStallEquip = sizeof(tagMsgStallGoods) - 1 + SIZE_EQUIP;
//-----------------------------------------------------------------------------
// ��̯��Ʒ�ṹ
//-----------------------------------------------------------------------------
struct tagStallGoods
{
	INT64	n64Serial;			// �����۵���Ʒ&װ��64λID
	INT64	n64UnitPrice;		// ����

	tagStallGoods() { n64Serial = GT_INVALID; n64UnitPrice = 0; }
};

//-------------------------------------------------------------------------------
// ����&����
//-------------------------------------------------------------------------------
Stall::Stall(Role *pRole, const tagRoleDataSave* pRoleData)
{
	ASSERT(P_VALID(pRole) && P_VALID(pRoleData));

	m_pRole			= pRole;
	m_nTotalTax		= pRoleData->nTotalTax;
	m_byLevel		= pRoleData->byStallLevel;
	m_nDailyExp		= pRoleData->nStallDailyExp;
	m_nCurExp		= pRoleData->nStallCurExp;
	m_bBroadcast	= FALSE;

	m_dwLastUpgradeTime = pRoleData->dwLastStallTime;

	// ���ݽ�˰�ܶ����̯λ����ģ�͵ȼ���˰�ռ���
	//MCalStallMode(m_byLevel, m_fDeTaxRate, nTotalTax);

	m_n8GoodsNum	= 0;
	m_fCityTaxRate	= 0.0f;
	m_fDeTaxRate	= 0.0f;
	m_strTitle		= NULL;
	m_strAd			= NULL;
	m_pStallGoods	= NULL;
}

Stall::~Stall()
{
	Destroy();
}

//-------------------------------------------------------------------------------
// ��̯
//-------------------------------------------------------------------------------
DWORD Stall::Init(FLOAT fCityTaxRate)
{
// 	// ������
// 	if (m_pRole->GetEarnRate() < 10000)
// 		return GT_INVALID;

	DWORD dwErrorCode = CanStall();
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}
	
	m_n8GoodsNum	= 0;
	m_fCityTaxRate	= fCityTaxRate;

	// �����ڴ�
	m_pStallGoods = new tagStallGoods[STALL_MAX_DISPLAY];
	if(!P_VALID(m_pStallGoods))
	{
		return GT_INVALID;
	}

	// ����Ϊ��̯����״̬
	m_pRole->SetRoleState(ERS_StallSet, FALSE);

	return E_Success;
}

//-------------------------------------------------------------------------------
// ��̯
//-------------------------------------------------------------------------------
DWORD Stall::Destroy()
{
	// �Ƿ���̯
	if(!m_pRole->IsInRoleStateAny(ERS_Stall | ERS_StallSet))
	{
		return E_Stall_Role_Pull;
	}
	
	// ���ð�̯״̬
	if(m_pRole->IsInRoleState(ERS_Stall))
	{
		m_pRole->UnsetRoleState(ERS_Stall);
	}
	
	if(m_pRole->IsInRoleState(ERS_StallSet))
	{
		// ȡ����̯����״̬������Ҫ����Χ��ҷ�����Ϣ
		m_pRole->UnsetRoleState(ERS_StallSet, FALSE);
	}

	// ֪ͨVIP̯λ״̬���
	tagVIPStall* pVIPStall = g_VIPStall.GetVIPStallInfo(m_pRole->GetID());
	if (P_VALID(pVIPStall))
	{
		EVIPStallStatus eStatus = EVSS_Close;
		g_VIPStall.AddEvent(m_pRole->GetID(), EVT_SetStallStatus, 1, &eStatus);
	}

	m_bBroadcast = FALSE;

	// ������Ʒʹ��״̬
	if(P_VALID(m_pStallGoods))
	{
		tagItem *pGoods;
		for(INT i=0; i<STALL_MAX_DISPLAY; ++i)
		{
			if(m_pStallGoods[i].n64Serial != GT_INVALID)
			{
				pGoods = m_pRole->GetItemMgr().GetBagItem(m_pStallGoods[i].n64Serial);
				if(P_VALID(pGoods))
				{
					pGoods->SetUsing(FALSE);
				}
			}
		}
	}

	m_n8GoodsNum	= 0;
	m_fCityTaxRate	= 0.0f;

	// �ͷ��ڴ�
	SAFE_DEL_ARRAY(m_strTitle);
	SAFE_DEL_ARRAY(m_strAd);
	SAFE_DEL_ARRAY(m_pStallGoods);

	// �Ƴ���̯buff
	m_pRole->RemoveBuff(Buff::GetIDFromTypeID(STALL_BUFF_ID), TRUE);

	return E_Success;
}

//-------------------------------------------------------------------------------
// �㲥���
//-------------------------------------------------------------------------------
VOID Stall::Update()
{
	if (!m_pRole->IsInRoleState(ERS_Stall))
		return;

	// ���
	if(m_bBroadcast && m_strAd != NULL && g_world.GetWorldTick() - m_dwLastAdBroadcastTick >= STALL_BROADCAST_INTERVAL_TICK)
	{
		// �ڵ�ǰƵ���㲥
		if(P_VALID(m_pRole->GetSession()))
		{
			m_pRole->GetSession()->BroadcastCurrChannel(m_strAd);
		}

		m_dwLastAdBroadcastTick = g_world.GetWorldTick();
	}

	// ̯λ����
	if (CalcTimeDiff(GetCurrentDWORDTime(), m_dwLastUpgradeTime) >= STALL_GAINEXP_INTERVAL_TIME)
	{
		if (IsSameDay(m_dwLastUpgradeTime))
		{
			StallUpgrade(FALSE);
		}
		else
		{
			StallUpgrade(TRUE);
		}

		m_dwLastUpgradeTime = GetCurrentDWORDTime();
	}
}

//-------------------------------------------------------------------------------
// ���ñ���
//-------------------------------------------------------------------------------
DWORD Stall::SetTitle(LPCTSTR strTitle)
{
	if(!m_pRole->IsInRoleState(ERS_StallSet))
	{
		// �Ƿ���Ϣ�Ż�ִ�е��˴�
		ASSERT(0);
		return GT_INVALID;
	}
	
	INT nSzTitle = g_attRes.GetVariableLen().nStallTitleMax + 1;
	
	DWORD dwErrorCode = Filter::CheckName(strTitle, nSzTitle - 1);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	if(NULL == m_strTitle)
	{
		m_strTitle = new TCHAR[nSzTitle];
	}

	ZeroMemory(m_strTitle, nSzTitle * sizeof(TCHAR));
	_tcsncpy(m_strTitle, strTitle, nSzTitle - 1);

	return E_Success;
}

//-------------------------------------------------------------------------------
// ���ù��
//-------------------------------------------------------------------------------
DWORD Stall::SetAdText(LPCTSTR strAd)
{
	if(!m_pRole->IsInRoleState(ERS_StallSet))
	{
		// �Ƿ���Ϣ�Ż�ִ�е��˴�
		ASSERT(0);
		return GT_INVALID;
	}
	
	INT nSzAd = STALL_MAX_ADTEXT_NUM;

	DWORD dwErrorCode = Filter::CheckName(strAd, nSzAd - 1);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	if(NULL == m_strAd)
	{
		m_strAd = new TCHAR[nSzAd];
	}

	ZeroMemory(m_strAd, nSzAd * sizeof(TCHAR));
	_tcsncpy(m_strAd, strAd, nSzAd - 1);

	return E_Success;
}

//-------------------------------------------------------------------------------
// ���ù�沥�ű�־
//-------------------------------------------------------------------------------
DWORD Stall::SetAdFlag(bool bBroadcast)
{
	if(!m_pRole->IsInRoleStateAny(ERS_Stall | ERS_StallSet))
	{
		// �Ƿ���Ϣ�Ż�ִ�е��˴�
		ASSERT(0);
		return GT_INVALID;
	}

	m_bBroadcast = bBroadcast;

	return E_Success;
}

//-------------------------------------------------------------------------------
// ��Ʒ�ϼ�
//-------------------------------------------------------------------------------
DWORD Stall::SetGoods(const INT64 n64Serial, const INT64 n64UnitPrice, const BYTE byIndex)
{
	if(!m_pRole->IsInRoleStateAny(ERS_Stall | ERS_StallSet))
	{
		// �Ƿ���Ϣ�Ż�ִ�е��˴�
		ASSERT(0);
		return GT_INVALID;
	}
	
	//// �Ƿ��ѳ�ʼ��
	//if(NULL == m_pStallGoods)
	//{
	//	// �Ƿ���Ϣ�Ż�ִ�е��˴�
	//	ASSERT(0);
	//	return GT_INVALID;
	//}

	// ��Ǯ�����Ϸ����ж�
	if(n64UnitPrice < STALL_MIN_SOLD_SLIVER 
		|| n64UnitPrice > STALL_MAX_SOLD_SLIVER)
	{
		// �Ƿ���Ϣ�Ż�ִ�е��˴�
		ASSERT(0);
		return GT_INVALID;
	}
	
	// �±��ж�
	if(byIndex >= STALL_MAX_DISPLAY)
	{
		// �Ƿ���Ϣ�Ż�ִ�е��˴�
		ASSERT(0);
		return GT_INVALID;
	}

	// ��λ�ж�
	if(m_pStallGoods[byIndex].n64Serial != GT_INVALID)
	{
		// �Ƿ���Ϣ�Ż�ִ�е��˴�
		ASSERT(0);
		return GT_INVALID;
	}
	
	// ȡ����Ʒ
	tagItem *pGoods = m_pRole->GetItemMgr().GetBagItem(n64Serial);
	if(!P_VALID(pGoods))
	{
		return E_Stall_ItemNotFound_InBag;
	}

	// �Ƿ��Ѵ���ʹ��״̬
	if(pGoods->bUsing)
	{
		return E_Stall_Goods_InUsed;
	}

	// �ж���Ʒ�Ƿ���Գ���
	if(!m_pRole->GetItemMgr().CanExchange(*pGoods))
	{
		return E_Stall_ItemCanNot_Sold;
	}

	// ���ð�̯��Ʒ
	m_pStallGoods[byIndex].n64Serial = n64Serial;
	m_pStallGoods[byIndex].n64UnitPrice = n64UnitPrice;

	pGoods->SetUsing(TRUE);

	// ����
	++m_n8GoodsNum;

	return E_Success;
}

//-------------------------------------------------------------------------------
// ��Ʒ�¼�
//-------------------------------------------------------------------------------
DWORD Stall::UnSetGoods(const BYTE byIndex)
{
	if(!m_pRole->IsInRoleStateAny(ERS_Stall | ERS_StallSet))
	{
		// �Ƿ���Ϣ�Ż�ִ�е��˴�
		ASSERT(0);
		return GT_INVALID;
	}

	// �±��ж�
	if(byIndex >= STALL_MAX_DISPLAY)
	{
		// �Ƿ���Ϣ�Ż�ִ�е��˴�
		ASSERT(0);
		return GT_INVALID;
	}

	// ��λ�ж�
	if(GT_INVALID == m_pStallGoods[byIndex].n64Serial)
	{
		// �Ƿ���Ϣ������Ʒ�Ѿ�����
		return E_Stall_Goods_BeSold;
	}

	// �����Ʒ������ʹ�ñ�־
	tagItem *pGoods = m_pRole->GetItemMgr().GetBagItem(m_pStallGoods[byIndex].n64Serial);
	if(P_VALID(pGoods))
	{
		pGoods->SetUsing(FALSE);
	}
	
	// �¼���Ʒ
	UnSet(byIndex);

	return E_Success;
}

//-------------------------------------------------------------------------------
// ��ȡ̯λ����
//-------------------------------------------------------------------------------
DWORD Stall::GetStallTitle(OUT LPTSTR pSzTitle)
{
	// �Ƿ���̯
	if(!m_pRole->IsInRoleState(ERS_Stall))
	{
		return E_Stall_Role_Pull;
	}

	if(NULL == m_strTitle)
	{
		pSzTitle[0] = _T('\0');
	}
	else
	{
		_tcsncpy(pSzTitle, m_strTitle, STALL_MAX_TITLE_NUM - 1);
		pSzTitle[STALL_MAX_TITLE_NUM - 1] = _T('\0');
	}

	return E_Success;
}

//-------------------------------------------------------------------------------
// ���̯λ��ָ��λ���ϵ���Ʒ
//-------------------------------------------------------------------------------
DWORD Stall::GetSpecGoods(BYTE byIndex, OUT LPVOID pData, OUT INT &nGoodsSz)
{
	nGoodsSz	= 0;

	if(GT_INVALID == m_pStallGoods[byIndex].n64Serial)
	{
		return E_Stall_Goods_BeSold;
	}

	MTRANS_POINTER(pCur, pData, tagMsgStallGoods);
	pCur->n64UnitPrice	= m_pStallGoods[byIndex].n64UnitPrice;
	pCur->byIndex		= byIndex;

	tagItem *pGoods = m_pRole->GetItemMgr().GetBagItem(m_pStallGoods[byIndex].n64Serial);
	if(!P_VALID(pGoods))
	{
		// û���ڱ������ҵ�(���ܱ����ʹ�á�������)
		UnSet(byIndex);
		return E_Stall_Goods_BeSold;
	}

	if(MIsEquipment(pGoods->dwTypeID))
	{
		pCur->byItem	= 0;
		IFASTCODE->MemCpy(pCur->byData, pGoods, SIZE_EQUIP);
		nGoodsSz = m_SzStallEquip;
	}
	else
	{
		pCur->byItem	= 1;
		IFASTCODE->MemCpy(pCur->byData, pGoods, SIZE_ITEM);
		nGoodsSz = m_SzStallItem;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------
// ���̯λ�ϵ�������Ʒ
//-------------------------------------------------------------------------------
DWORD Stall::GetGoods(OUT LPVOID pData, OUT BYTE &byGoodsNum, OUT INT &nGoodsSz)
{
	byGoodsNum	= 0;
	nGoodsSz	= 0;
	
	// �Ƿ���̯
	if(!m_pRole->IsInRoleState(ERS_Stall))
	{
		return E_Stall_Role_Pull;
	}

	INT8 n8Num = 0;
	MTRANS_POINTER(pCur, pData, BYTE);

	INT nGoodsSzTmp = 0;
	for(INT i=0; i<STALL_MAX_DISPLAY; ++i)
	{
		if(GetSpecGoods(i, pCur, nGoodsSzTmp) != E_Success)
		{
			continue;
		}

		pCur += nGoodsSzTmp;
		nGoodsSz += nGoodsSzTmp;

		++n8Num;
		if(m_n8GoodsNum == n8Num)
		{
			break;
		}
	}

	byGoodsNum	= m_n8GoodsNum;

	return E_Success;
}

//-------------------------------------------------------------------------------
// ������Ʒ
//-------------------------------------------------------------------------------
DWORD Stall::Sell(Role *pRole, INT64 n64UnitSilver, 
				  INT64 n64Serial, BYTE byIndex, INT16 n16Num, OUT INT16 &n16RemainNum)
{
	// �±��ж�
	if(byIndex >= STALL_MAX_DISPLAY || n16Num <= 0)
	{
		// �Ƿ���Ϣ�Ż�ִ�е��˴�
		ASSERT(0);
		return GT_INVALID;
	}
	
	// �Ƿ���̯
	if(!m_pRole->IsInRoleState(ERS_Stall))
	{
		return E_Stall_Role_Pull;
	}
	
	// ��λ�ж�
	if(GT_INVALID == m_pStallGoods[byIndex].n64Serial)
	{
		return E_Stall_Goods_BeSold;
	}

	// �����Ʒ
	if(n64Serial != m_pStallGoods[byIndex].n64Serial)
	{
		return E_Stall_Goods_Refresh;
	}

	// ����ۼ�
	if(m_pStallGoods[byIndex].n64UnitPrice != n64UnitSilver)
	{
		return E_Stall_Goods_Refresh_Price;
	}

	// ����˰�ʼ�����һ�õĽ�Ǯ
	INT64 n64Total		= n64UnitSilver * n16Num;
	INT64 n64TotalPay	= INT64((FLOAT)n64Total * (1.0f - max(0, (m_fCityTaxRate - m_fDeTaxRate))));
	INT32 nTax			= (INT32)(n64Total - n64TotalPay);

	// �������ҽ�Ǯ
	if(pRole->GetCurMgr().GetBagSilver() < n64TotalPay)
	{
		return E_Stall_CustomerMoney_NotEnough;
	}

	// �������ұ���
	if(pRole->GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Stall_CustomerBug_NoFreeSpace;
	}

	tagItem *pGoods = m_pRole->GetItemMgr().GetBagItem(n64Serial);
	if(!P_VALID(pGoods))
	{
		UnSet(byIndex);
		return E_Stall_Goods_NotInBag;
	}
	
	// �����Ʒ����
	if(pGoods->n16Num < n16Num)
	{
		return E_Stall_GoodsNum_NotEnough;
	}

	// ���ô�������
	n16RemainNum = pGoods->n16Num - n16Num;
	
	// ���� -- �۳���Ʒ�ͽ�Ǯ
	tagItem *pItem;
	DWORD	dwErrorCode;
	if(n16Num == pGoods->n16Num)
	{
		pItem = pGoods;

		UnSet(byIndex);
		dwErrorCode = m_pRole->GetItemMgr().TakeOutFromBag(pItem->n64Serial, ELCLD_Stall_BeSold, TRUE);
	}
	else
	{
		// �����µĶ���Ʒ
		pItem = ItemCreator::Create(*pGoods, n16Num);

		dwErrorCode = m_pRole->GetItemMgr().DelFromBag(n64Serial, (DWORD)ELCLD_Stall_BeSold, n16Num);
	}

	if(dwErrorCode != E_Success)
	{
		// ���ϲ��ж�����©����ʱ���ܻ����ڴ�й©
		ASSERT(0);
		return dwErrorCode;
	}

	if(!pRole->GetCurMgr().DecBagSilver(n64TotalPay, (DWORD)ELCLD_Stall_Buy))
	{
		// �Ƿ�黹��Ʒ//??
		
		// ���ϲ��ж�����©����ʱ���ܻ����ڴ�й©
		ASSERT(0);
		return dwErrorCode;
	}

	// ����ʹ��״̬
	pItem->SetUsing(FALSE);

	// ���� -- �����Ʒ�ͽ�Ǯ
	m_pRole->GetCurMgr().IncBagSilver(n64TotalPay, (DWORD)ELCLD_Stall_BeSold);
	pRole->GetItemMgr().Add2Bag(pItem, (DWORD)ELCLD_Stall_Buy, TRUE);

	// �ۼ�˰��
	if(nTax != 0)
	{
		ASSERT(m_nTotalTax < STALL_MAX_TAX);
		m_nTotalTax = min(m_nTotalTax + nTax, STALL_MAX_TAX);
	}
	
	
	return E_Success;
}

//-------------------------------------------------------------------------------
// �ж��Ƿ���԰�̯
//-------------------------------------------------------------------------------
DWORD Stall::CanStall()
{
	// �ж��Ƿ��Ѿ����ڰ�̯���̯����״̬
	if(m_pRole->IsInRoleStateAny(ERS_Stall | ERS_StallSet))
	{
		return E_Stall_Role_InStall;
	}

	//// �ж��Ƿ��ڱ�ս״̬
	//if(m_pRole->IsInRoleStateAny(ERS_Combat | ERS_PK | ERS_PVP))
	//{
	//	return E_Stall_RoleState_NotPermit;
	//}

	// �ȼ�>=30
	if(m_pRole->GetLevel() < STALL_MIN_ROLE_LEVEL)
	{
		return E_Stall_Role_Level_2Low;
	}
	
	// �������
	if(!m_pRole->IsInRoleState(ERS_StallArea))
	{
		return E_Stall_Area_NotAllowed;
	}

	// �ռ�4������ -- �ù����ɿͻ���ʵ��

	return E_Success;
}

//-------------------------------------------------------------------------------
// �¼���Ʒ
//-------------------------------------------------------------------------------
VOID Stall::UnSet(const BYTE byIndex)
{
	ASSERT(byIndex < STALL_MAX_DISPLAY);
	
	m_pStallGoods[byIndex].n64Serial = GT_INVALID;
	m_pStallGoods[byIndex].n64UnitPrice = 0;

	// ����
	--m_n8GoodsNum;
}

//-------------------------------------------------------------------------------
// �ϼ���Ʒ��ɣ�����Χ��ҹ㲥
//-------------------------------------------------------------------------------
DWORD Stall::SetFinish()
{ 
	// �Ƿ��Ѵ����̯״̬
	if(m_pRole->IsInRoleState(ERS_Stall))
	{
		return GT_INVALID;
	}
	
	// �Ƿ��ڰ�̯����״̬
	if(!m_pRole->IsInRoleState(ERS_StallSet))
	{
		return GT_INVALID;
	}

	if(IsEmpty())
	{
		return E_Stall_Goods_Empty;
	}

	// ������� -- �ٴ�ȷ���Ƿ��ڰ�̯����
	if(!m_pRole->IsInRoleState(ERS_StallArea))
	{
		return E_Stall_Area_NotAllowed;
	}

	// ��������״̬
	m_pRole->SetRoleState(ERS_Stall);
	m_pRole->UnsetRoleState(ERS_StallSet, FALSE);

	// �Ӱ�̯��buff
	m_pRole->TryAddBuff(m_pRole, g_attRes.GetBuffProto(STALL_BUFF_ID), NULL, NULL, NULL);

	return E_Success;
}

//-------------------------------------------------------------------------------
// ���Լ���̯λ�ϵ���Ʒ�����ڴ棬������tagMsgStallGoods�ṹʱ�Ĵ�С
//-------------------------------------------------------------------------------
INT32 Stall::CalMemSzGoodsUsed() const
{
	return m_n8GoodsNum * m_SzStallEquip;
}

//-------------------------------------------------------------------------------
// ̯λ����
//-------------------------------------------------------------------------------
VOID Stall::StallUpgrade(BOOL bNextDay)
{
	// ���쾭������
	if (bNextDay)
		m_nDailyExp = 0;

	// ̯λ�ȼ�����
	if (m_byLevel >= STALL_MAX_LEVEL)
		return;

	// ���վ�������
	if (m_nDailyExp >= STALL_MAX_DAILY_EXP)
		return;

	// ����ȼ�����
	if (m_pRole->GetLevel() < STALL_EXP_LEVEL_LIMIT[m_byLevel])
		return;

	// ��������
	m_nDailyExp++;
	m_nCurExp++;

	// �������
	INT nLevelExp = MCalStallLevelExp(m_byLevel);
	if (m_nCurExp >= nLevelExp)
	{
		m_byLevel++;
		m_nCurExp -= nLevelExp;
	}
}

//-------------------------------------------------------------------------------
// ����뵱ǰʱ���Ƿ���ͬһ��
//-------------------------------------------------------------------------------
BOOL Stall::IsSameDay( DWORD dwTime )
{
	tagDWORDTime dwSrcTime(dwTime);
	tagDWORDTime dwCurTime = GetCurrentDWORDTime();

	if ((dwCurTime.year == dwSrcTime.year) && (dwCurTime.month == dwSrcTime.month) && (dwCurTime.day == dwSrcTime.day))
	{
		return TRUE;
	}

	return FALSE;
}

//-------------------------------------------------------------------------------
// ����̯λ��Ϣ(ͬ��ɫһ�𱣴�)
//-------------------------------------------------------------------------------
VOID Stall::Save2DB( tagRoleDataSave* pRoleData )
{
	if (!P_VALID(pRoleData))	return;

	pRoleData->byStallLevel		= m_byLevel;
	pRoleData->nStallDailyExp	= m_nDailyExp;
	pRoleData->nStallCurExp		= m_nCurExp;
	pRoleData->dwLastStallTime	= m_dwLastUpgradeTime;
}

//-------------------------------------------------------------------------------
// ̯λ��þ���(GM������)
//-------------------------------------------------------------------------------
DWORD Stall::GainExp( INT32 nExp )
{
	// ����Ϊ-1ʱ����̯λ�ȼ�����
	if (!GT_VALID(nExp))
	{
		m_nCurExp	= 0;
		m_nDailyExp	= 0;
		m_byLevel	= 1;
		return E_Success;
	}

	// ��������ֵ�ж�Ϊ�Ƿ�
	if (nExp <= 0)
	{
		return GT_INVALID;
	}

	// ����ȼ�����
	if (m_pRole->GetLevel() < STALL_EXP_LEVEL_LIMIT[m_byLevel])
		return GT_INVALID;

	// ��������(���ﲻ����ÿ�վ�������)
	m_nCurExp += nExp;
	m_nCurExp = m_nCurExp < 0 ? 0 : m_nCurExp;

	// ���㵱ǰ������ʵĵȼ�
	while (m_byLevel < STALL_MAX_LEVEL)
	{
		INT nLevelExp = MCalStallLevelExp(m_byLevel);
		if ((m_nCurExp >= nLevelExp) && (m_pRole->GetLevel() >= STALL_EXP_LEVEL_LIMIT[m_byLevel]))
		{
			m_byLevel++;
			m_nCurExp -= nLevelExp;
		}
		else
		{
			break;
		}
	}

	// ���Ѿ���������վ���
	if (m_byLevel == STALL_MAX_LEVEL)
	{
		m_nCurExp	= 0;
		m_nDailyExp	= 0;
	}

	return E_Success;
}