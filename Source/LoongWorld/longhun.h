//------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: longhun.h
// author: Sxg
// actor:
// data: 2009-3-17
// last:
// brief: ��������������ƴ����� -- ����װ�����Ѻ�
//------------------------------------------------------------------------
#pragma once

class Role;

struct tagEquip;
struct tagLongHunProto;
//------------------------------------------------------------------------
class LongHun
{
public:
	LongHun(Role *pRole);
	~LongHun();

	VOID Add(const tagEquip *pEquip, const INT16 nEquipPos, BOOL bSend2Client);
	VOID Remove(const INT16 nEquipPos);
	VOID Swap(const tagEquip *pEquip1, const tagEquip *pEquip2, INT16 n16SrcPos1, INT16 n16DstPos1);

public:
	// ��ȡ�Ѽ����ָ����λ����������
	DWORD GetLongHunID(const INT16 n16EquipPos) const;
	DWORD GetRandomLongHunID();

	INT8 GetPassiveNum() const;
	INT8 GetActiveNum() const;
	VOID InitSendInitState(OUT INT16 n16Data[], INT8 n8Sz) const;

private:
	// �жϸü�װ���ǿɱ��񼤻�
	BOOL IsNeedCount(const INT16 n16EquipPos, const tagLongHunProto *pLongHunProto, const BOOL bOuterLongHun);

	// ������������ -- ����������
	VOID CalLongfuAtt(OUT BYTE byLongfuAtt[], const INT nSz, INT16 n16EquipPos);
	
	VOID Set(const tagEquip *pEquip, const INT16 n16EquipPos, BOOL bSend2Client);
	VOID Unset(const INT16 n16EquipPos, BOOL bSend2Client);

	// ��������������״̬�仯���͸��ͻ���
	VOID SendAdd2Client(const INT16 n16EquipPos);
	VOID SendRemove2Client(const INT16 n16EquipPos);
	VOID SendEquipEffect2Client(const INT16 n16EquipPos);

private:
	Role			*m_pRole;

	// �Ѽ������������: ��������������ʹ��
	DWORD			m_dwPassive[EEP_MaxEquip];
	DWORD			m_dwActive[EEP_MaxEquip];
	TList<DWORD>	m_listActive;

	INT8			m_n8NumPassive;
	INT8			m_n8NumActive;
};

//***************************** ����ʵ�� *********************************

//------------------------------------------------------------------------
// �Ի� -- ��ָ�䣬������
//------------------------------------------------------------------------
inline VOID LongHun::Swap(const tagEquip *pEquip1, const tagEquip *pEquip2, 
						  INT16 n16SrcPos1, INT16 n16DstPos1)
{
	if(P_VALID(pEquip1))
	{
		Remove(n16SrcPos1);
	}
	
	if(P_VALID(pEquip2))
	{
		Remove(n16DstPos1);
	}
	
	Add(pEquip1, n16DstPos1, TRUE);
	Add(pEquip2, n16SrcPos1, TRUE);
}

//------------------------------------------------------------------------
// ��ȡ�������������ĸ���
//------------------------------------------------------------------------
inline INT8 LongHun::GetPassiveNum() const
{
	return m_n8NumPassive;
}

//------------------------------------------------------------------------
// ����������������ĸ���
//------------------------------------------------------------------------
inline INT8 LongHun::GetActiveNum() const
{
	return m_n8NumActive;
}

//------------------------------------------------------------------------
// ��ȡ�Ѽ����ָ����λ���������� -- һ��װ����ֻ������һ����������
//------------------------------------------------------------------------
inline DWORD LongHun::GetLongHunID(const INT16 n16EquipPos) const
{
	if(m_dwActive[n16EquipPos] != GT_INVALID)
	{
		return m_dwActive[n16EquipPos];
	}
		
	if(m_dwPassive[n16EquipPos] != GT_INVALID)
	{
		return m_dwPassive[n16EquipPos];
	}

	return GT_INVALID;
}

//------------------------------------------------------------------------
// ���ȡ��һ��������������ID
//------------------------------------------------------------------------
inline DWORD LongHun::GetRandomLongHunID()
{
	DWORD dwLongHunID = GT_INVALID;
	if (!m_listActive.RandPeek(dwLongHunID))
		return GT_INVALID;

	return dwLongHunID;
}