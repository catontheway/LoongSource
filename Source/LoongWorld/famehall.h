//-----------------------------------------------------------------------------
//!\file famehall.h
//!\author xlguo
//!
//!\date 2009-02-17
//! last 
//!
//!\brief ������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "famehall_part.h"
//----------------------------------------------------------------------------
//���ݿⱣ��취
//	���������ã�	���ݿ������Ϣ
//	���������䱦��	���ݿ������Ϣ
//	��������ʱ�䣺	���ݿ������Ϣ
//	��ɫ������		LoongWorld���ڱ���������Ϣ
//���ݿ��ȡ������	���ݿ���Ϣ

//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// ������
//----------------------------------------------------------------------------
class FameHall
{
public:// ���̵߳���
	FameHall()	{}
	~FameHall()	{	Destroy();	}
	BOOL Init();

	VOID Destroy(){}

	// ���ڸ��¸�����������
	VOID Update();

	// �������ݿ���Ϣ
	VOID SendLoadDBData();

	// ��������������Ϣ
	VOID HandleUpdateRepRank(tagNetCmd* pCmd);

	// �����ʼ�������䱦�б���Ϣ
	VOID HandleInitActTreasureList(tagNetCmd* pCmd);

	// ��ʼ�������ý������
	VOID HandleInitFameHallTop50(tagNetCmd* pCmd);

	// ��ʼ����������ʱ��
	VOID HandleInitRepRstTimeStamp(tagNetCmd* pCmd);

	// ���������䱦�����������̣߳�
	DWORD ActClanTreasure(Role* pRole, UINT16 u16TreasureID);

	// ��ɫ��ȡ���������ֵ
	VOID RoleRepUpdate(Role* pRole, ECLanType eClanType);

public:// ��ͼ�̵߳���
	// ���Խ���������
	BOOL TryEnterFameHall(Role* pRole, ECLanType eClanType);

	// ��ȡ���������ǰ50��nameid��
	VOID GetMemberTop50(BYTE* pData, ECLanType eClanType);

	// ��ȡ���������ǰ50������Ŀ
	INT32 GetMemberTop50Num(ECLanType byClanType);

	// �������������tagRepRankData��
	VOID GetRepRankTop(PVOID pData, ECLanType eClanType);

	// �������������Ŀ
	INT32 GetRepRankNum(ECLanType byClanType);

	// ����Ѽ��������䱦�б�
	VOID GetActClanTreasure(PVOID pData, ECLanType eClanType);

	// ����Ѽ��������䱦��Ŀ
	INT32 GetActClanTreasureNum(ECLanType byClanType);

	// ���������������ʱ��
	tagDWORDTime GetRepRankTimeStamp(ECLanType byClanType) const	{	return m_ArrClanTrunk[byClanType].GetRepRankUpdateTime();}

	// ����������б����ʱ��
	tagDWORDTime GetFameSnapTimeStamp(ECLanType byClanType) const	{	return m_ArrClanTrunk[byClanType].GetEnterSnapUpdateTime();}

private:
	ClanTrunk			m_ArrClanTrunk[ECLT_NUM];			// ������������
	BOOL				m_bInitOK;							// ��ʼ���ɹ�

private:// ���ÿ���
	FameHall(const FameHall& rhs);
	FameHall& operator = (const FameHall& rhs);
};


extern FameHall g_fameHall;
