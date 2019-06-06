#pragma once

#include "..\WorldDefine\ItemDefine.h"

#pragma pack(push, 1)

class ItemMgr;

enum EItemState// ��Ʒ��ǰ״̬
{
	EIS_Inoperable	=	0x1,	// ���ɲ�����
	EIS_Unmovable	=	0x2,	// �����ƶ���
	EIS_Unwearable	=	0x4,	// ����װ����
	EIS_Unuseable	=	0x8,	// ����ʹ�õ�
	EIS_Undroppable	=	0x10,	// ���ɶ�����
	EIS_Unvendible	=	0x20,	// ���ɳ��۵�
	EIS_Untradeable	=	0x40,	// ���ɽ��׵�
	EIS_Unstackable	=	0x80,	// ���ɶѵ���
};

/** class Item
	brief ��Ϸ�е�����Ʒ��
*/
class Item
{
public:
	Item();
	virtual ~Item();
	/** \���캯��
	*/
	Item(tagItem Data);
	
	/** \�������캯��
	*/
	Item(const Item&);
	
	/** \��Ʒ�����Ƿ���Ч
	*/
	virtual BOOL IsItem();
	//---------------------------------------------------
	// ��ȡ
	//--------------------------------------------------- 
	/** \��ȡ��������
	*/
	const tagItem* GetData(){return &m_data;}
	/** \��ȡԭʼ����
	*/
	const tagItemProto* GetProto(){return m_pProto;}
	/** \��ȡ��ʾ����
	*/
	const tagItemDisplayInfo* GetDisplay(){return m_pProto->pDisplayInfo;}
	/** \��ȡ��Ʒ������������
	*/
	EItemConType GetConType(){return m_data.eConType;}
	/** \��ȡλ��
	*/
	INT16 GetPos(){return m_data.n16Index;}
	/** \��ȡ��ƷID
	*/
	INT64 GetItemId(){return m_data.n64Serial;}
	/** \��ȡ����ID
	*/
	DWORD GetItemTypeID(){return m_data.dwTypeID;}
	/** \��ȡ��Ʒ�ڶ�����
	*/
	EItemTypeEx GetItemTypeEx(){return m_pProto->eTypeEx;}
	/**	\��ȡ��Ʒ����
	*/
	INT	GetItemQuantity(){return m_data.n16Num;}
	/** \��ȡ��ƷTips
	*/
	tstring GetItemTips();
	/** \��ȡ��Ʒ����
	*/
	tstring GetItemName(){return m_pProto->pDisplayInfo->szName;}
	/** \��ȡ��Ʒ��ʾ����
	*/
	tstring GetItemDisplayType(){return m_pProto->pDisplayInfo->szType;}
	/** \��ȡ��Ʒ��������
	*/
	EStuffType GetItemStuffType(){return m_pProto->eStuffType;}
	/** \��ȡ��Ʒ����ʹ������
	*/
	EItemSpecFunc GetItemSpecFunc(){return m_pProto->eSpecFunc;}
	/** \��ȡ��Ʒ��ȴʱ��
	*/
	DWORD GetItemCDTime(){return m_pProto->dwCooldownTime;}
	/** \��ȡ��Ʒ��״̬
	*/
	bool IsItemBinded(){return m_data.byBind == 2 ? true : false;}
	/** \��ȡ��Ʒ�Ƿ�����
	*/
	bool IsItemLocked(){return m_data.bLock;}
	/** \��ȡ��Ʒ��ǰʹ�ô���
	*/
	INT GetItemUseTimes(){return m_data.nUseTimes;}
	/** \��ȡ��Ʒ������ID
	*/
	DWORD GetItemCreatorID(){return m_data.dwCreatorID;}
	/** \��ȡ��ƷƷ��
	*/
	virtual BYTE GetItemQuality(){return m_pProto->byQuality;}
	/** \��ȡ��Ʒ�۸�
	*/
	virtual INT GetItemPrice(){return m_pProto->nBasePrice*m_data.n16Num;}
	/** \��ȡ��Ʒ�ȼ�
	*/
	BYTE GetItemLevel(){return m_pProto->byLevel;}
    /** \��ȡ��Ʒ�󶨽�ɫID
    */
    DWORD GetItemBindingRole() { return m_data.dwOwnerID; }
	//---------------------------------------------------
	// ��ֵ
	//--------------------------------------------------- 
	/** \������ƷID
	*/
	VOID SetItemId(INT64 n64Id){m_data.n64Serial = n64Id;}
	/** \������Ʒ����
	*/
	VOID SetConType(EItemConType eType){m_data.eConType = eType;}
	/** \����λ��
	*/
	VOID SetPos(INT16 n16Pos){m_data.n16Index = n16Pos;}

	/** \������Ʒ����
	*/
	VOID SetItemQuantity(INT16 n16Num){m_data.n16Num = n16Num;}
	/** \����ʹ�ô���
	*/
	VOID SetItemUseTimes(INT nTimes){m_data.nUseTimes = nTimes;}
	/** \���ð�
	*/
	VOID SetItemBind(){m_data.byBind = 2;}
	
	//---------------------------------------------------
	// ״̬�ж�
	//--------------------------------------------------- 
	/** \�����Ƿ�ɲ���
	*/
	VOID SetOperable(BOOL bOperable);
	/** \��ȡ�Ƿ�ɲ���
	*/
	BOOL IsOperable();
	
	/** \�����Ƿ���ƶ�
	*/
	VOID SetMovable(BOOL bMovable);
	/** \��ȡ�Ƿ���ƶ�
	*/
	BOOL IsMovable();

	/**\ �����Ƿ��װ��
	*/
	VOID SetWearable(BOOL bWearable);
	/**\ ��ȡ�Ƿ��װ��
	*/
	BOOL IsWearable();

	/**\ �����Ƿ��ʹ��
	*/
	VOID SetUseable(BOOL bUseable);
	/**\ ��ȡ�Ƿ��ʹ��
	*/
	BOOL IsUseable();

	/**\ �����Ƿ�ɶ���
	*/
	VOID SetDroppable(BOOL bDroppable);
	/**\ ��ȡ�Ƿ�ɶ���
	*/
	BOOL IsDroppable();

	/**\ �����Ƿ���۳�
	*/
	VOID SetVendible(BOOL bVendible);
	/**\ ��ȡ�Ƿ���۳�
	*/
	BOOL IsVendible();

	/**\ �����Ƿ�ɽ���
	*/
	VOID SetTradeable(BOOL bTradeable);
	/**\ ��ȡ�Ƿ�ɽ���
	*/
	BOOL IsTradeable();

	/**\ �����Ƿ�ɶѵ�
	*/
	VOID SetStackable(BOOL bStackable);
	/**\ ��ȡ�Ƿ�ɶѵ�
	*/
	BOOL IsStackable(INT16 &n16Quantity);

    /**\ lock the item
         first, save the current item status, then set all item attribute disabled
    */
    void Lock();
    /**\ unlock the item
         first, pop the last item status, then set all item attribute enabled
    */
    void UnLock();

    bool IsLocked() {return m_bLocked;}

protected:
	DWORD						m_dwItemState;		// ��Ʒ״̬����¼����״̬
	const tagItemProto*			m_pProto;			// ��Ʒԭʼ�ṹ
	tagItem						m_data;				// ��Ʒ�����ṹ
    bool                        m_bLocked;          // is locked
    DWORD                       m_dwlastItemStus;   // last item status before locked
};



/** \class	Equipment
	\brief	��Ϸ�еĵ���װ����
*/
class Equipment : public Item
{
public:
	Equipment();
	virtual ~Equipment();

	Equipment(tagEquip Data);

	Equipment(const Equipment&);

	/** \��Ʒ�����Ƿ���Ч
	*/
	virtual BOOL IsItem();
	
	/** \װ�����Ա仯
	*/
	void UpdateEquipment(tagEquipSpec Data);

	/** \��ȡװ����չ�ṹ
	*/
	const tagEquipSpec* GetEquip(){return &m_equipex;}
	/** \��ȡװ��Ǳ��ֵ
	*/
	INT32 GetEquipPotVal(){return m_equipex.nPotVal;}
	/** \��ȡװ��Ǳ��ʹ��ֵ
	*/
	INT32 GetEquipUsedPotVal(){return m_equipex.nPotValUsed;}
    /** \��ȡװ����ǰǱ��ֵ����
    */
    INT32 GetEquipCurrPotMax(){return m_equipex.nPotVal + m_equipex.nPotValUsed;}
	/*	\ ���װ��Ǳ��ֵ������
	*/
	INT32 GetEquipPotMax() { return ((const tagEquipProto*)m_pProto)->nMaxPotVal;}
	/*	\ ���װ��Ǳ��ֵ�������Ĵ���
	*/
	INT32 GetEquipPotAdvanceTime() { return m_equipex.nPotIncTimes; }
	/** \��ȡװ����λ
	*/
	EEquipPos GetEquipPos(){return ((const tagEquipProto*)m_pProto)->eEquipPos;}
	/** \��ȡװ����ǰ�Կ̶���
	*/
	BYTE GetEquipEngraveTimes(){return m_equipex.byEngraveTimes;}
	/** \��ȡװ����ǰ���Ķ���
	*/
	BYTE GetEquipPosyTimes(){return m_equipex.byPosyTimes;}
	/** \��ǰװ����ӡ����
	*/
	BYTE GetEquipBrandLevel(){return m_equipex.byBrandLevel;}
	/** \��ǰװ�������Ƕ��
	*/
	BYTE GetEquipMaxHoleNum(){return m_equipex.byHoleNum;}
	/** \��ǰװ����ǰ��Ƕ��
	*/
	BYTE GetEquipHoleNum()
	{
		for(INT i=0; i<m_equipex.byHoleNum; ++i)
		{
			if(!P_VALID(m_equipex.dwHoleGemID[i]))
				return i;
		}
		return m_equipex.byHoleNum;
	}
	/** \��ȡ��ƷƷ��
	*/
	virtual BYTE GetItemQuality(){return m_equipex.byQuality;} 
	/** \��ȡ��Ʒ�۸�
	*/
	virtual INT GetItemPrice();
	/** \��ȡװ���Ƿ����
	*/
	BOOL IsNotIdetified(){return m_equipex.byQuality == (BYTE)GT_INVALID;}
	/** \��ȡװ��������ֵ
	*/
	BYTE GetEquipWuxing(EWuXing eType){return m_equipex.byLongfuAtt[eType];}
	/** \��ȡ������С����
	*/
	INT16 GetEquipMinDmg(){return m_equipex.n16MinDmg;}
	/** \��ȡ������󹥻�
	*/
	INT16 GetEquipMaxDmg(){return m_equipex.n16MaxDmg;}
	/** \��ȡ�������ֵ
	*/
	INT16 GetEquipWuhun(){return m_equipex.n16WuHun;}
	/** \��ȡ���߻���ֵ
	*/
	INT16 GetEquipArmor(){return m_equipex.n16Armor;}
	/** \��ȡ�������¶�
	*/
	INT GetEquipNewness(){return ((const tagEquipProto*)m_pProto)->n16Newness - m_data.nUseTimes/ABRASION2USETIMES;}
	/** \��ȡװ�����Լӳ�
	*/
	VOID GetEquipAttribute(TMap<ERoleAttribute, INT>& Map){Map = m_mapEquipAtt;}
	/** \��ȡװ�����԰ٷֱȼӳ�
	*/
	VOID GetEquipAttributePct(TMap<ERoleAttribute, INT>& Map){Map = m_mapEquipAttPct;}
	/** \��ȡװ������Ƕ��ʯID
	*/
	DWORD GetEquipHoldGemID(INT index){return m_equipex.dwHoleGemID[index];}
	/** \��ȡװ������������
	*/
	DWORD GetEquipOuterLonghun(){return m_equipex.dwLongHunOuterID;}
	/** \��ȡװ������������
	*/
	DWORD GetEquipInnerLonghun(){return m_equipex.dwLongHunInnerID;}
	/** \��ȡװ����������
	*/
	BYTE GetEquipSpecAttribute(){return m_equipex.bySpecAtt;}
	/** \�Ƿ�ɿ���
	*/
	BOOL IsCanChisel(){return m_equipex.bCanCut;};
	/** \��ȡװ����ǰ��������
	*/
	INT16 GetEquipAttLimit(ERoleAttribute eType);
	/** \�Ƿ��Ⱦɫ
	*/
	BOOL IsCanDye(){return ((const tagEquipProto*)m_pProto)->bCanDye;}

protected:
	/** \��ȡװ�����Լӳɵ��ӳ��б�
	*/
	VOID LoadEquipAttribute();
	/** \�������Լӳ��б�
	*/
	VOID AddEquipAttribute(ERoleAttribute eAtt, INT nInc);

protected:
	tagEquipSpec				m_equipex;			// װ����չ�ṹ
	TMap<ERoleAttribute, INT>	m_mapEquipAtt;		// װ�����Լӳ��б�
	TMap<ERoleAttribute, INT>	m_mapEquipAttPct;	// װ�����԰ٷֱȼӳ��б�
};

#pragma pack(pop)