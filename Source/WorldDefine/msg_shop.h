//-----------------------------------------------------------------------------
//!\file msg_shop.h
//!
//!\date 2008-09-27
//!
//!\brief �ͻ����������֮������̵����Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"

#pragma pack(push, 1)

//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum 
{
	E_Shop_NPCNotValid				= 1,	// NPC�Ƿ�
	E_Shop_NotValid					= 2,	// �̵�Ƿ�
	E_Shop_ItemNotFind				= 3,	// û���ҵ�ָ����Ʒ
	E_Shop_ItemCannotSell			= 4,	// ָ����Ʒ���ɳ���
	E_Shop_ItemNotEnough			= 5,	// ��Ʒ��������
	E_Shop_CreateItem_Failed		= 6,	// ����������Ʒʱʧ��(1.�ڴ�����ʧ��;2.û���ҵ���Ʒװ��ԭ��)
	E_Shop_Equip_Sold				= 7,	// ָ��װ���Ѿ�������
	E_Shop_CreateEquip_Failed		= 8,	// ����װ��ʱ,������װ��ʧ��(1.�ڴ�����ʧ��)
	E_Shop_NotEnough_SpaceInBag		= 9,	// ������û���㹻�洢�ռ�
	E_Shop_RepLevel_Low				= 10,	// �����׶β���
	E_Shop_NotEnough_SilverInBag	= 11,	// �����н�Ǯ����
	E_Shop_NotEnough_SpecCost		= 12,	// �������Ĳ���
	E_Shop_ItemMaxHold				= 13,	// ��Ʒ�ﵽ��ӵ������
	E_Shop_ItemNotActived			= 14,	// ���屦������Ʒû�м���
};


//-----------------------------------------------------------------------------
// �õ��̵�����Ʒ�б�
//-----------------------------------------------------------------------------
CMD_START(NC_GetShopItems)
	DWORD 			dwNPCID; 		// NPC ID
	BYTE			byShelf;		// ���ܺ�
CMD_END

CMD_START(NS_GetShopItems)
	DWORD 			dwNPCID; 		// NPC ID
	DWORD			dwShopID;		// shop ID
	BYTE			byShelf;		// ���ܺ�
	BYTE			byDummy;		// ������
	INT16			n16RareItemNum;	// ϡ����Ʒ����
	BYTE			byData[1];		// tagMsgShopItem
CMD_END

CMD_START(NC_GetShopEquips)
	DWORD 			dwNPCID; 		// NPC ID
	BYTE			byShelf;		// ���ܺ�
CMD_END

CMD_START(NS_GetShopEquips)
	DWORD 			dwNPCID; 		// NPC ID
	DWORD			dwShopID;		// shop ID
	BYTE			byShelf;		// ���ܺ�
	BYTE			byDummy;		// ������
	INT16			n16EquipNum;	// ����װ������
	BYTE			byData[1];		// tagMsgShopEquip
CMD_END

//-----------------------------------------------------------------------------
// ��Ҵ��̵�����Ʒ
//-----------------------------------------------------------------------------
CMD_START(NC_BuyShopItem)
	DWORD 			dwNPCID; 		// NPC ID
	BYTE			byShelf;		// ���ܺ�
	BYTE			byDummy;		// ������
	INT16			n16ItemNum;		// ��Ʒ����
	DWORD 			dwTypeID; 		// ���Ҫ�������ƷType ID
CMD_END

CMD_START(NS_BuyShopItem)
	DWORD 			dwNPCID; 		// NPC ID
	BYTE			byShelf;		// ���ܺ�
	BYTE			byIndex;		// ��Ӧλ��
	INT16			n16RemainNum;	// ʣ�����(��ֵΪGT_INVALIDʱ����ʾ����Ʒ������)
	DWORD 			dwTypeID;		// ���Ҫ�������ƷType ID
CMD_END

CMD_START(NC_BuyShopEquip)
	DWORD 			dwNPCID; 		// NPC ID
	INT64			n64Serial;		// Ҫ����װ����64λID
	DWORD			dwTypeID;		// ����װ����Type ID
	BYTE			byShelf;		// ���ܺ�
CMD_END

CMD_START(NS_BuyShopEquip)
	DWORD 			dwNPCID; 		// NPC ID
	BYTE			byShelf;		// ���ܺ�
	BYTE			byIndex;		// ��Ӧλ��
	INT16			n16RemainNum;	// ʣ�����(��ֵΪGT_INVALIDʱ����ʾ����Ʒ������)
	BYTE			byData[1];		// n16RemainNum != 0 && dwErrorCode == E_Successʱ, �����µĴ���װ�� -- tagEquip
CMD_END

//-----------------------------------------------------------------------------
//	������̵�����Ʒ
//-----------------------------------------------------------------------------
CMD_START(NC_SellToShop)
	DWORD 			dwNPCID; 		// NPC ID
	INT64			n64Serial; 		// ���Ҫ����64λ��ƷID
CMD_END

//-----------------------------------------------------------------------------
// �̵������Ϣ����ʧ�ܺ�,�������ͻ��˵���Ϣ�ṹ
//-----------------------------------------------------------------------------
CMD_START(NS_FeedbackFromShop)
	DWORD			dwErrorCode;
	DWORD 			dwNPCID; 		// NPC ID
CMD_END

#pragma pack(pop) 
