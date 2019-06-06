//--------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: suit.h
// author: Sxg
// actor:
// data: 2008-12-2
// last:
// brief: ��װ��
//--------------------------------------------------------------------------------------------
#pragma once

struct tagEquip;

class Role;
//--------------------------------------------------------------------------------------------

/******************************* �ඨ�� **********************************/
class Suit
{
public:
	Suit(Role *pRole);
	~Suit();

	VOID Add(const tagEquip *pEquip, INT16 n16EquipPos, BOOL bSend2Client = TRUE);
	VOID Remove(const tagEquip *pEquip, INT16 n16OldIndex);

public:
	INT GetSuitNum();
	INT GetSuitEquipNum(DWORD dwSuitID);
	VOID InitSendInitState(BYTE *pData);

private:
	//--------------------------------------------------------------------------------------------
	// �жϸü���װ�����Ƿ���Ҫͳ��
	//--------------------------------------------------------------------------------------------
	BOOL IsNeedCount(const tagEquip *pEquip, const tagEquipProto *pEquipProto, 
					INT nSuitIndex, INT16 n16IndexOther);

	//--------------------------------------------------------------------------------------------
	// �ж��Ƿ�����װ����
	//--------------------------------------------------------------------------------------------
	BOOL IsSuitPart(const tagEquip *pEquip, const tagEquipProto *pEquipProto, INT nSuitIndex, INT16 n16OldIndex);

	//--------------------------------------------------------------------------------------------
	// ������������߽�ָ���õ���һ������λ��
	//--------------------------------------------------------------------------------------------
	INT16 GetOtherEquipPos(const tagEquip *pEquip, INT16 n16OldIndex);

private:
	Role				*m_pRole;
	TMap<DWORD, INT>	m_mapSuitNum;	// <dwSuitID, nEquipNum>
};


/******************************* ����ʵ�� **********************************/

//--------------------------------------------------------------------------------------------
// ��װ����
//--------------------------------------------------------------------------------------------
inline INT Suit::GetSuitNum()
{
	return m_mapSuitNum.Size();
}

//--------------------------------------------------------------------------------------------
// ĳ����װ��װ������
//--------------------------------------------------------------------------------------------
inline INT Suit::GetSuitEquipNum(DWORD dwSuitID)
{
	return m_mapSuitNum.Peek(dwSuitID);
}

//--------------------------------------------------------------------------------------------
// �ж��Ƿ�����װ����
//--------------------------------------------------------------------------------------------
inline BOOL Suit::IsSuitPart(const tagEquip *pEquip, const tagEquipProto *pEquipProto, 
							 INT nSuitIndex, INT16 n16OldIndex)
{
	// �ж����Ʒ��
	if(pEquipProto->bySuitMinQlty[nSuitIndex] > pEquip->equipSpec.byQuality)
	{
		return FALSE;
	}

	// ������������߽�ָ������Ҫ�ж���һ��װ��λ��װ��
	INT16 n16IndexOther = GetOtherEquipPos(pEquip, n16OldIndex);
	if(n16IndexOther != GT_INVALID
		&& !IsNeedCount(pEquip, pEquipProto, nSuitIndex, n16IndexOther))
	{
		return FALSE;
	}

	return TRUE;
}

//--------------------------------------------------------------------------------------------
// ������������߽�ָ���õ���һ������λ��
//--------------------------------------------------------------------------------------------
inline INT16 Suit::GetOtherEquipPos(const tagEquip *pEquip, INT16 n16OldIndex)
{
	if(MIsRing(pEquip))
	{
		return (EEP_Finger1 == n16OldIndex ? EEP_Finger2 : EEP_Finger1);
	}
	else if(MIsWeapon(pEquip))
	{
		return (EEP_RightHand == n16OldIndex ? EEP_LeftHand : EEP_RightHand);
	}

	return GT_INVALID;
}