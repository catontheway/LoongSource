//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: stall.h
// author: Sxg
// actor:
// data: 2008-12-17
// last:
// brief: ��̯
//-------------------------------------------------------------------------------
#pragma once

class Role;

struct tagRoleDataSave;
struct tagStallGoods;
//-------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------
const INT STALL_BROADCAST_INTERVAL_TICK	= 2 * 60 * TICK_PER_SECOND;	// 2����
const INT STALL_GAINEXP_INTERVAL_TIME	= 60;						// 60��
const INT STALL_MAX_DAILY_EXP			= 1000;
const BYTE STALL_MAX_LEVEL				= 10;
const BYTE STALL_EXP_LEVEL_LIMIT[STALL_MAX_LEVEL]	= {1, 20, 20, 40, 40, 60, 60, 80, 80, 100};

//-------------------------------------------------------------------------------
class Stall
{
public:
	Stall(Role *pRole, const tagRoleDataSave* pRoleData);
	~Stall();

public:
	DWORD Init(FLOAT fCityTaxRate);
	DWORD Destroy();
	VOID  Update();

	DWORD SetTitle(LPCTSTR strTitle);
	DWORD SetAdText(LPCTSTR strAd);
	DWORD SetAdFlag(bool bBroadcast);
	DWORD SetGoods(const INT64 n64Serial, const INT64 n64UnitPrice, const BYTE byIndex);
	DWORD UnSetGoods(const BYTE byIndex);
	DWORD SetFinish();

	BOOL  IsEmpty()			const { return 0 >= m_n8GoodsNum; }
	BYTE  GetModeLevel()	const { return m_byLevel; }
	//INT8  GetGoodsNum()		const { return m_n8GoodsNum ; }

public:
	DWORD GetStallTitle(OUT LPTSTR pSzTitle);
	DWORD GetSpecGoods(BYTE byIndex, OUT LPVOID pData, OUT INT &nGoodsSz);
	DWORD GetGoods(OUT LPVOID pData, OUT BYTE &byGoodsNum, OUT INT &nGoodsSz);
	DWORD Sell(Role *pRole, INT64 n64UnitSilver, INT64 n64Serial, BYTE byIndex, INT16 n16Num, OUT INT16 &n16RemainNum);
	INT32 CalMemSzGoodsUsed() const;

public:
	DWORD GainExp(INT32 nExp);	// GM������

public:
	VOID  Save2DB(tagRoleDataSave* pRoleData);

private:
	DWORD CanStall();
	VOID  UnSet(const BYTE byIndex);

private:
	VOID  StallUpgrade(BOOL bNextDay);
	BOOL  IsSameDay(DWORD dwTime);

private:
	// ��ɫ��ʼ��ʱȷ��������
	Role			*m_pRole;
	FLOAT			m_fDeTaxRate;		// ˰�ռ���
	INT				m_nTotalTax;		// ��˰�ܶ�(��λ����)	-- ����ֵ�豣�浽���ݿ���
	BYTE			m_byLevel;			// ̯λģ�͵ȼ�
	INT32			m_nDailyExp;		// �����̯����
	INT32			m_nCurExp;			// ��ǰ�ȼ�����
	bool			m_bBroadcast;		// �Ƿ���Ҫ�㲥���

	// ��̯ʱ����ȷ��������
	INT8			m_n8GoodsNum;		// ��ǰ̯λ�ϵĴ�����Ʒ
	FLOAT			m_fCityTaxRate;		// ����˰��
	TCHAR			*m_strTitle;		// ����
	TCHAR			*m_strAd;			// �����
	tagStallGoods	*m_pStallGoods;		// ̯λ��Ʒ

	// ��ͻ��˴���ʱ����Ʒ�ṹ��С
	static INT		m_SzStallItem;		// tagMsgStallGoods�ṹ������ΪItemʱ�Ĵ�С
	static INT		m_SzStallEquip;		// tagMsgStallGoods�ṹ������ΪEquipʱ�Ĵ�С

	// ���¼�ʱ
	DWORD			m_dwLastAdBroadcastTick;	// ��һ�β��Ź���tick
	DWORD			m_dwLastUpgradeTime;		// ��һ�λ�þ����ʱ��
};