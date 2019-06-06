//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: currency.h
// author: Sxg
// actor:
// data: 2008-08-14
// last:
// brief: ������ -- �������ֿ��еĽ�ǮԪ��
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/cost_type_define.h"

class Role;
//-----------------------------------------------------------------------------
// log����������ö��
//-----------------------------------------------------------------------------
enum ELogConType
{
	ELCT_Null		= 0,

	ELCT_Bag		= 1,	// ����
	ELCT_RoleWare	= 2,	// ��ɫ�ֿ�
	ELCT_BaiBao		= 3,	// �ٱ���
};

//-----------------------------------------------------------------------------
// ģ��
//-----------------------------------------------------------------------------
template<class T>
class Currency
{
public:
	Currency(T nMoney, T nMaxMoney);

public:
	T	Gain(T nMoney);
	T	Spend(T nMoney);

public:
	T	GetCur() const { return m_nMoney; }
	T	GetMax() const { return m_nMaxMoney; }

private:
	volatile T	m_nMoney;
	T	m_nMaxMoney;
};
class ClanData;
class ItemMgr;
//-----------------------------------------------------------------------------
// ������ -- �����ֿ��еĽ�ǮԪ��
//-----------------------------------------------------------------------------
class CurrencyMgr
{
friend class ItemMgr;

public:
	CurrencyMgr(Role *pRole, INT32 nBagGold, INT32 nBagSilver, INT32 nBagYuanBao, 
				INT64 nWareSilver, INT32 nBaiBaoYuanBao, INT32 nExVolume);

	VOID Update();

public:
	// ��ǰ�����Ƿ�����Ҫ��
	BOOL IsEnough(ECurCostType eCurType, INT32 nNeed);
	// �۳�ָ������
	BOOL DecCurrency(ECurCostType eCurType, INT32 nNeed, DWORD dwCmdID);

public:
	// ��õ�ǰ��Ǯ��	
	INT64 GetBagSilver()		const { return m_BagSilver.GetCur(); }
	INT32 GetBagYuanBao()		const { return m_BagYuanBao.GetCur(); }
	INT64 GetWareSilver()		const { return m_WareSilver.GetCur(); }
	INT32 GetBaiBaoYuanBao()	const { return m_BaiBaoYuanBao.GetCur(); }
	INT32 GetExchangeVolume()	const { return m_ExchangeVolume.GetCur(); }

	// ��ÿɴ洢����Ǯ��
	INT64 GetMaxBagSilver()			const { return m_BagSilver.GetMax(); }
	INT32 GetMaxBagYuanBao()		const { return m_BagYuanBao.GetMax(); }
	INT64 GetMaxWareSilver()		const { return m_WareSilver.GetMax(); }
	INT32 GetMaxBaiBaoYuanBao()		const { return m_BaiBaoYuanBao.GetMax(); }
	INT32 GetMaxExchangeVolume()	const { return m_ExchangeVolume.GetMax(); }

	// �����Դ���Ľ�Ǯ��Ŀ
	INT64 GetCanIncBagSilver()		const { return GetMaxBagSilver() - GetBagSilver(); }
	INT32 GetCanIncBagYuanBao()		const { return GetMaxBagYuanBao() - GetBagYuanBao(); }
	INT64 GetCanIncWareSilver()		const { return GetMaxWareSilver() - GetWareSilver(); }
	INT32 GetCanIncBaiBaoYuanBao()	const { return GetMaxBaiBaoYuanBao() - GetBaiBaoYuanBao(); }
	INT32 GetCanIncExchangeVolume()	const { return GetMaxExchangeVolume() - GetExchangeVolume(); }

	// ��ý�Ǯ��Ԫ��
	BOOL IncBagSilver(INT64 n64Silver, DWORD dwCmdID, DWORD dwRoleIDRel = GT_INVALID);
	BOOL IncBagYuanBao(INT32 nYuanBao, DWORD dwCmdID);
	BOOL IncWareSilver(INT64 n64Silver, DWORD dwCmdID);
	BOOL IncBaiBaoYuanBao(INT32 nYuanBao, DWORD dwCmdID, BOOL bSaveDB = TRUE);
	BOOL IncExchangeVolume(INT32 nExVolume, DWORD dwCmdID);

	// ʧȥ��Ǯ��Ԫ��
	BOOL DecBagSilver(INT64 n64Silver, DWORD dwCmdID, DWORD dwRoleIDRel = GT_INVALID);
	BOOL DecBagYuanBao(INT32 nYuanBao, DWORD dwCmdID);
	BOOL DecWareSilver(INT64 n64Silver, DWORD dwCmdID);
	BOOL DecBaiBaoYuanBao(INT32 nYuanBao, DWORD dwCmdID);
	BOOL DecExchangeVolume(INT32 nExVolume, DWORD dwCmdID);

public:
	// ���������ҵĽӿ�
	static BOOL ModifyBaiBaoYuanBao(DWORD dwRoleID, INT32 nYuanBao, DWORD dwCmdID);
	static BOOL ModifyWareSilver(DWORD dwRoleID, INT64 n64Silver, DWORD dwCmdID);
private:
	static VOID SendBaiBaoYB2DB(DWORD dwAccountID, INT nBaiBaoYuanBao);
	static VOID LogBaiBaoYuanBao(const DWORD dwAccountID, const INT nNum, const DWORD dwCmdID);
	static VOID SendWareSilver2DB(DWORD dwAccountID, INT64 n64WareSilver);
	static VOID LogWareSilver(const DWORD dwAccountID, const INT64 n64Num, const DWORD dwCmdID);

public:
	// ���幱������
	BOOL DecClanCon(INT32 nClanCon, DWORD dwCmdID, ECLanType eClanType);
	BOOL IncClanCon(INT32 nIncr, DWORD dwCmdID, ECLanType eClanType);
	INT32 GetCanIncClanCon(ECLanType eClanType)	const;
	INT32 GetMaxClanCon(ECLanType eClanType) const;
	INT32 GetClanCon(ECLanType eClanType) const;

private:
	// ��ͻ��˷�����Ϣ
	VOID SendMessage(LPVOID pMsg, DWORD dwSize);

	// ��¼log����LoongDB����Ϣ
	VOID LogSilver(const ELogConType eLogConType, const INT64 n64Num, 
				  const INT64 n64TotalNum, const DWORD dwCmdID, const DWORD dwRoleIDRel = GT_INVALID);
	VOID LogYuanBao(const ELogConType eLogConType, const INT nNum, 
					const INT nTotalNum, const DWORD dwCmdID);
	VOID LogExVolume(const INT nNum, const INT nTotalNum, const DWORD dwCmdID);
	VOID LogTimeStat();

private:
	Role*				m_pRole;
	INT					m_nTimeStatCountDown;	// ��ʱͳ�Ƶ���ʱ

private:
	Currency<INT64>		m_BagSilver;
	Currency<INT32>		m_BagYuanBao;
	Currency<INT64>		m_WareSilver;
	Currency<INT32>		m_BaiBaoYuanBao;
	Currency<INT32>		m_ExchangeVolume;
	ClanData*			m_pRoleClanData;		// ���幱��
};


//-----------------------------------------------------------------------------
// ģ����ʵ��
//-----------------------------------------------------------------------------
template<class T>
Currency<T>::Currency(T nMoney, T nMaxMoney)
{
	m_nMoney	= nMoney;
	m_nMaxMoney = nMaxMoney;

	if(nMoney > nMaxMoney)
	{
		ASSERT(nMoney <= nMaxMoney);
		nMoney = nMaxMoney;
	}

	if(nMoney < 0)
	{
		ASSERT(nMoney >= 0);
		nMoney = 0;
	}
}

template<class T>
T Currency<T>::Gain(T nMoney)
{
	if(nMoney <= 0)
	{
		return 0;
	}

	T nChange = m_nMaxMoney - m_nMoney;

	if(nChange < nMoney)
	{
		ASSERT(nChange >= nMoney);
		m_nMoney = m_nMaxMoney;
	}
	else
	{
		m_nMoney += nMoney;
		nChange = nMoney;
	}

	return nChange;
}

template<class T>
T Currency<T>::Spend(T nMoney)
{
	if(nMoney <= 0)
	{
		return 0;
	}

	T nChange = nMoney;

	if(m_nMoney < nMoney)
	{
		ASSERT(m_nMoney >= nMoney);
		nChange = m_nMoney;
		m_nMoney = 0;
	}
	else
	{
		m_nMoney -= nMoney;
	}

	return nChange;
}

////-----------------------------------------------------------------------------
//class Currency
//{
//public:
//	Currency();
//	~Currency();
//
//public:
//	// ��ʼ�������ݿ���������
//	VOID Init(INT32 nBagGold, INT32 nBagSilver, INT32 nBagYuanBao, 
//		INT32 nWareGold, INT32 nWareSilver, INT32 nBaiBaoYuanBao);
//
//	// ��ý�Ǯ��Ԫ��
//	INT64 IncBagSilver(INT64 n64Silver);
//	INT32 IncBagYuanBao(INT32 nYuanBao);
//	INT64 IncWareSilver(INT64 n64Silver);
//	INT32 IncBaiBaoYuanBao(INT32 nYuanBao);
//
//	// ʧȥ��Ǯ��Ԫ��
//	INT64 DecBagSilver(INT64 n64Silver);
//	INT32 DecBagYuanBao(INT32 nYuanBao);
//	INT64 DecWareSilver(INT64 n64Silver);
//	INT32 DecBaiBaoYuanBao(INT32 nYuanBao);
//
//
//public:
//	INT64 GetBagSilver()	const { return m_n64BagSilver; }
//	INT32 GetBagYuanBao()	const { return m_nBagYuanBao; }
//	INT64 GetWareSilver()	const { return m_n64WareSilver; }
//	INT32 GetBaiBaoYuanBao()	const { return m_nBaiBaoYuanBao; }
//
//private:
//	INT64	m_n64BagSilver;
//	INT64	m_n64WareSilver;
//	INT32	m_nBagYuanBao;
//	INT32	m_nBaiBaoYuanBao;
//};