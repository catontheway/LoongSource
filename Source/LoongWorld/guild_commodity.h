//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_commodity.h
// author: Sunnee
// actor:
// data: 2009-07-03
// last:
// brief: ��������������
//-----------------------------------------------------------------------------
#pragma once

#include "../ServerDefine/guild_define.h"

class Role;

class GuildCommodity
{
public:
	GuildCommodity();
	~GuildCommodity();

	BOOL	Init(DWORD dwRoleID, INT nLevel, const tagTaelInfo* pTaelInfo = NULL, const tagRedoundInfo* pRedoundInfo = NULL);
	VOID	Destory();

	// �����ݿ�������������Ϣ
	DWORD	LoadCommodityInfo(tagGuildCommerceInfo* pLoadInfo);

	// ��ȡ�������̻���Ϣ(��������)
	DWORD	GetCommodityInfo(tagCommerceGoodInfo* pGoods, INT& nGoodNum, INT32& nTael, INT32& nLevel);
	
	// ȡ������������Ϣ
	const tagTaelInfo* GetTaelInfo()		{ return &m_sTaelInfo; }
	const tagRedoundInfo* GetRedoundInfo()	{ return &m_sRedoundInfo; }
	INT   GetLevel()						{ return m_nLevel; }
	INT32 GetTael()							{ return m_nCurTael; }
	INT	  GetGoodsNum()						{ return m_mapGoods.Size(); }
	INT   GetTaelProgress()					{ return (INT)(((FLOAT)m_nCurTael / (FLOAT)m_sTaelInfo.nPurposeTael) * 100.0f); }
	INT32 GetGain();

	DWORD IsFull(DWORD dwGoodID, BYTE byNum);
	DWORD IsExist(DWORD dwGoodID, BYTE byNum);

	// ��ǰ��������(ֻ�����̻ύ��,��ʱ����¼Log)
	BOOL	IncTael(INT32 nTael);
	BOOL	DecTael(INT32 nTael);

	// ������Ʒ����
	BOOL	AddGood(DWORD dwGoodID, BYTE byNum, INT32 nPrice = 0);
	BOOL	RemoveGood(DWORD dwGoodID, BYTE byNum, INT32 nPrice = 0);

	// �����ͷ�
	VOID	DeadPenalty();

private:
	// ���ݿ����
	VOID	SaveTael2DB();
	VOID	SaveCommodity2DB();

private:
	typedef TMap<DWORD, tagCommerceGoodInfo*> MapGoods;

	DWORD					m_dwOwnerID;			// ������
	INT						m_nLevel;				// ��ȡ��������ʱ�ĵȼ�
	tagTaelInfo				m_sTaelInfo;			// ���������ʼ��Ϣ
	tagRedoundInfo			m_sRedoundInfo;			// ������Ϣ

	INT32					m_nCurTael;				// ��������������
	MapGoods				m_mapGoods;				// �̻���
};