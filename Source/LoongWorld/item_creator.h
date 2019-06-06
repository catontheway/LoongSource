//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: item_creator.cpp
// author: Sxg
// actor:
// data: 2008-07-29
// last:
// brief: ��Ʒ&װ�������� -- ��Ʒ&װ������,����������(��Ʒ����������)
//-----------------------------------------------------------------------------
#pragma once

#include "mutex.h"

struct tagEquip;
struct tagEquipProto;
struct tagIMEffect;
//-----------------------------------------------------------------------------
class ItemCreator
{
	friend class GMCommandMgr;

public:
	ItemCreator();
	~ItemCreator();

public:
	// �������ݿ��ȡ�����ݴ�����Ʒ
	static tagItem* CreateItemByData(PVOID pData);
	static tagEquip* CreateEquipByData(PVOID pData);
	// ����������Ʒ�����µĶ���Ʒ
	static tagItem* Create(const tagItem &item, INT16 n16Num);
	
	// ����δ������Ʒ&װ��
	static tagItem* Create(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT16 n16Num = 1,  
							DWORD dwCreator = GT_INVALID, INT16 n16QltyModPct = 0, INT16 n16QltyModPctEx = 0, INT16 n16PotValPct = 10000,INT nPotAdding = 0);
	// Jason 2009-12-1 ������ɫƷ��
	static tagItem* CreateByProduce(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID,EItemQuality eQlty, INT nPotAdding,DWORD dwPurpleQltyIdfPct = 0, INT16 n16Num = 1, 
		DWORD dwCreator = GT_INVALID, INT16 n16QltyModPct = 0, INT16 n16QltyModPctEx = 0, INT16 n16PotValPct = 10000);

	// ����δ�����䱦(��Ʒ&װ��)
	static tagItem* CreateTreasure(DWORD dwNameID, EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT16 n16Num = 1, 
							DWORD dwCreator = GT_INVALID, INT16 n16QltyModPct = 0, INT16 n16QltyModPctEx = 0);

	// ����ָ��Ʒ������Ʒ&װ��
	static tagItem* CreateEx(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, 
							INT16 n16Num = 1, EItemQuality eQlty = EIQ_Null, DWORD dwCreator = GT_INVALID, 
							const tagIMEffect *pIMEffect = NULL)
	{
		tagItem *pNewItem = Create(eCreateMode, dwCreateID, dwTypeID, n16Num);
		if(!P_VALID(pNewItem))
		{
			return NULL;
		}

		if(MIsEquipment(dwTypeID)/* && eQlty > EIQ_Start && eQlty < EIQ_End*/)
		{
			IdentifyEquip((tagEquip*)pNewItem, eQlty, pIMEffect);
		}

		return pNewItem;
	}

	// ����ָ��Ʒ�����䱦(��Ʒ&װ��)
	static tagItem* CreateTreasureEx(DWORD dwNameID, EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, 
		INT16 n16Num = 1, EItemQuality eQlty = EIQ_Null, DWORD dwCreator = GT_INVALID, 
		const tagIMEffect *pIMEffect = NULL);


	// ������Ʒװ�����
	static VOID SetItemSerial(INT64 n64Max, INT64 n64Min) { m_n64MaxSerial = n64Max; m_n64MinSerial = n64Min; }

	// ��������Ψһ��(ע��Ҫ����������)
	static VOID CreateItemSerial(INT64 &n64NewSerial);

public:
	static VOID IdentifyEquip(IN OUT tagEquip *pEquip, 
						EItemQuality eQlty = EIQ_Null, const tagIMEffect *pIMEffect = NULL);

private: 	
	// �����Ϸ������ϲ���ú�����֤
	static VOID InitItem(tagItem &item, EItemCreateMode eCreateMode, const tagItemProto *pProto, DWORD dwCreateID, INT64 n64Serial, INT16 n16Num, DWORD dwCreator, DWORD dwCreateTime, DWORD dwAccountID, DWORD dwOwnerID);
	static VOID InitEquipSpec(tagEquipSpec &equipSpec, INT16 n16QltyModPct = 0, INT16 n16QltyModPctEx = 0, INT16 n16PotValPct = 10000);

	static INT32 GenBaseEquipQlty(DWORD dwTypeID);
	static INT32 ModEquipQltyByProduce(const tagEquip *pEquip, INT32 nQuality);
	static DWORD GenLongHunRandID(INT nLongHunType, INT nEquipPos, INT nQlty);
	
	static VOID CreateEquipQltyRel(OUT tagEquip *pEquip, const tagEquipProto *pEquipProto, EItemQuality eQlty);
	static BOOL CreateFashionQltyRel(OUT tagEquip *pEquip, const tagEquipProto *pEquipProto, 
									EItemQuality eQlty, const tagIMEffect *pIMEffect = NULL);

	static VOID ProcEquipAttBySpecAtt(tagEquip *pEquip);

	static BOOL IsGMItemNoInit(tagItem* pTmpItem);
	static BOOL InitGMItem(tagItem* pTmpItem);


private:
	static INT64			m_n64MaxSerial;
	static INT64			m_n64MinSerial;
	static Mutex			m_Mutex;
};