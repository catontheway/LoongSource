//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_item.h
// author: 
// actor:
// data: 2008-07-21
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ -- ��Ʒ/װ�����
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "ItemDefine.h"
#include "AvatarAtt.h"

#pragma pack(push, 1)
//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum
{
	// �ò��ַŵ�msg_common_errorcode.h�У��Ա��ģ��ͨ��
	//E_Item_LevelLimit				= 9,		// �ȼ�������Ҫ��
	//E_Item_SexLimit					= 10,		// �Ա�����Ҫ��
	//E_Item_ClassLimit				= 11,		// ְҵ������Ҫ��
	//E_Item_TalentLimit				= 12,		// ���ʲ�����Ҫ��
	//E_Item_ZoneRepLimit				= 13,		// ��������������Ҫ��
	//E_Item_OtherRepLimit			= 14,		// ��������������Ҫ��
	//E_Item_RoleStatusLimit			= 15,		// ���״̬(ս������ս��)������Ҫ��
	//E_Item_AttA_Limit				= 16,		// һ�����Բ�����

	E_Equip_OnFailed				= 100,		// װ��ʧ��
	E_Equip_NotRing					= 101,		// װ������ring
	E_Equip_InvalidPos				= 102,		// װ����λ��װ������Ҫ����λ����
	E_Equip_NotRingPos				= 103,		// װ����λ���ƶ���Ŀ��λ�ò���ring��λ
	E_Equip_SamePos					= 104,		// Ŀ��λ���뵱ǰλ��ͬ
	E_Equip_NotIdentify				= 105,		// װ��û�м���
	E_Equip_WeaponPos_Overlap		= 106,		// ����λ���ظ�
	E_Equip_Idendtifed				= 107,		// �����Ѽ�����
	E_Equip_Guard_DeadPunish		= 108,		// װ�����������ػ��������ԣ���ɫ����������
	E_Equip_Lock					= 109,		// ������Ʒ
	E_Equip_VocationLimit			= 110,		// ��ְҵ����װ����װ��

	E_BagPsd_Exist					= 150,		// ��������
	E_BagPsd_NoExist				= 151,		// δ������
	E_BagPsd_SafeCode_Error			= 152,		// ��ȫ�����
	E_BagPsd_Error					= 153,		// �������
	E_BagPsd_OK						= 154,		// ������ȷ

    E_Dye_OtherItem                 = 170,      // ֻ�ܶ�ʱװ����Ⱦɫ
    E_Dye_Exist                     = 171,      // ��ʱװ��Ϊ����ɫ������Ⱦɫ
    E_Dye_LvLimit                   = 172,      // Ⱦ�ϵȼ�����
	E_Dye_NotExist					= 173,		// װ������Ⱦ�ϲ�����

	E_GoldStone_Success				= 180,		// ���ʯ�����ɹ�
	E_GoldStone_SrcItemNotExist		= 181,		// ���ʯԭ�ϲ�����
	E_GoldStone_NoConfigFile		= 182,		// ���ʯ�������������ļ�������
	E_GoldStone_CreateDestItemFail	= 183,		// ������Ʒʧ��
	E_GoldStone_MissingMatch		= 184,		// ���㻯��Ʒ��������ʯ��ƥ��
	E_GoldStone_OtherError			= 185,		// ����δ֪����
};

//----------------------------------------------------------------------------
// װ�����������Ϣ(ע��ṹ���ֶ�˳����ñ�֤�ڴ����)
//----------------------------------------------------------------------------
CMD_START(NC_Equip)	// װ��
	EEquipPos	ePosDst;		// װ����λ(ע���ָװ����λ���ͻ��������жϴ���)
	INT64		n64Serial;		// װ��64λ���к�
CMD_END

CMD_START(NS_Equip) // ����װ����������Ӱ����������Ϣ����
	DWORD		dwErrorCode;
	INT64		n64Serial;		// װ��64λ���к�
CMD_END

CMD_START(NC_Unequip)
	INT16		n16PosDst;		// �����뱳����λ��
	INT16		n16Dummy;		// ������
	INT64		n64Serial;		// װ��64λ���к�
CMD_END

CMD_START(NS_Unequip)
	DWORD		dwErrorCode;
	INT64		n64Serial;		// װ��64λ���к�
CMD_END

CMD_START(NC_SwapWeapon)
CMD_END

CMD_START(NS_SwapWeapon)
	DWORD		dwErrorCode;
CMD_END

CMD_START(NS_AvatarEquipChange)	// ������۱仯
	DWORD			dwRoleID;
	tagEquipDisplay	EquipDisplay;	// װ�����
CMD_END

CMD_START(NS_EquipEffectChange)	// ��������Ч���仯
	DWORD			dwRoleID;
	BYTE			byDisplayPos;	// ģ��λ��
	BYTE			byEquipEffect;	// ����Ч��
CMD_END

CMD_START(NC_IdentifyEquip)	// ��������
	INT64		n64SerialReel;	// ��������
	INT64		n64SerialEquip;	// װ��
CMD_END

CMD_START(NS_IdentifyEquip)	// ��������
	DWORD		dwErrorCode;
	INT64		n64Serial;
CMD_END

CMD_START(NS_EquipChange)
	INT64			n64Serial;
	tagEquipSpec	equipSpec;
CMD_END

CMD_START(NS_SuitEffect)	// ��װ��Ч�ı�
	DWORD		dwRoleID;
	DWORD		dwSuitEffectID;
CMD_END

CMD_START(NS_SuitNum)		// ��װ�����ı�
	DWORD		dwSuitID;
	INT8		n8Num;
	INT8		n8Dummy[3];
CMD_END

//----------------------------------------------------------------------------
// ��Ʒλ�÷����仯
//----------------------------------------------------------------------------
CMD_START(NC_ItemPosChange) // ��Ʒλ���ƶ� -- ͬһ��������
	EItemConType	eConType;		// ��������
	INT64			n64Serial;		// �ƶ�λ�õ���Ʒ
	INT16			n16Num;			// �ƶ���Ʒ�ĸ���(0��ʾȫ���ƶ�)
	INT16			n16PosDst;		// ��Ʒ�ƶ���Ŀ��λ��
	DWORD			dwNPCID;		// ְ��NPC ID(����ֻ�вֿ�ʹ��)
CMD_END

CMD_START(NS_ItemPosChange) // ��Ʒλ���ƶ� -- ͬһ��������
	EItemConType	eConType;		// ��������
	INT64			n64Serial1;		// �ƶ�λ�õ���Ʒ1
	INT64			n64Serial2;		// ��Ӱ����Ʒ2, ��ΪGT_INVALIDʱ, ��ʾ����Ʒ1�ƶ���һ����λ
	INT16			n16PosDst1;		// ��Ʒ1�ƶ���Ŀ��λ��
	INT16			n16PosDst2;		// ��Ʒ2�ƶ���Ŀ��λ��
	INT16			n16Num1;		// ��Ʒ1���ո��� -- ����ֵΪ0ʱ,�轫����Ʒɾ��,������������Ϣ
	INT16			n16Num2;		// ��Ʒ2���ո���
	BOOL			bCreateItem;	// n64Serial2�Ƿ�Ϊ�´�������Ʒ�� -- Ϊ�½���ʱ����ͻ��˴���������������Ϣ
CMD_END

CMD_START(NC_ItemPosChangeEx) // ��Ʒλ���ƶ� -- ��ͬ������
	EItemConType	eConTypeSrc;	// Դ��������
	INT64			n64Serial1;		// �ƶ�λ�õ���Ʒ1
	EItemConType	eConTypeDst;	// Ŀ����������
	INT16			n16PosDst;		// ��Ʒ1�ƶ���Ŀ��λ��,ΪGT_INVALIDʱ��ʾû��ָ��Ŀ��λ��
	INT16			n16Dummy;
	DWORD			dwNPCID;		// ְ��NPC ID(����ֻ�вֿ�ʹ��)
CMD_END

CMD_START(NS_ItemPosChangeEx) // ��Ʒλ���ƶ� -- ��ͬ������
	EItemConType	eConTypeSrc1;	// �������� -- n64Serial1��ԭ����
	EItemConType	eConTypeSrc2;	// �������� -- n64Serial2��ԭ����
	EItemConType	eConTypeDst1;	// �������� -- n64Serial1��Ŀ������
	EItemConType	eConTypeDst2;	// �������� -- n64Serial2��Ŀ������
	INT16			n16PosDst1;		// ��Ʒ1�ƶ���Ŀ��λ��
	INT16			n16PosDst2;		// ��Ʒ2�ƶ���Ŀ��λ��
	INT64			n64Serial1;		// �ƶ�λ�õ���Ʒ1, ��ΪGT_INVALIDʱ, ��ʾ��Ʒ2�ƶ���һ����λ
	INT64			n64Serial2;		// �ƶ�λ�õ���Ʒ2, ��ΪGT_INVALIDʱ, ��ʾ��Ʒ1�ƶ���һ����λ
	INT16			n16Num1;		// ��Ʒ1���ո��� -- ����ֵΪ0ʱ,�轫����Ʒɾ��,������������Ϣ
	INT16			n16Num2;		// ��Ʒ2���ո���
CMD_END

CMD_START(NC_ItemReorder)	// ��������
	EItemConType	eContainerType;	// ��������
	DWORD			dwNPCID;		// GT_INVALIDʱ��Ϊ�ֿ���������Ϊ��������
	INT16			n16ItemNum;		// ��Ʒ����
	INT16			n16Index[1];	// ����ǰ��Ʒ�������е�λ�ã���ǰ�����±�Ϊ�����˳��
CMD_END

CMD_START(NS_ItemReorder)
	DWORD			dwErrorCode;
	EItemConType	eConType;		// ��������
	INT16			n16ItemNum;		// ��Ʒ����
	INT16			n16OldIndex[1];	// ��ǰ�����±�Ϊ�����˳��
CMD_END

CMD_START(NC_ItemReorderEx)	// ��������
	EItemConType	eContainerType;	// ��������
	DWORD			dwNPCID;		// GT_INVALIDʱ��Ϊ�ֿ���������Ϊ��������
	INT16			n16ItemNum;		// ��Ʒ����
	BYTE			byData[1];		// tagItemOrder
CMD_END

CMD_START(NS_ItemReorderEx)
	DWORD			dwErrorCode;
	EItemConType	eConType;		// ��������
	INT16			n16ItemNum;		// ��Ʒ����
	BYTE			byData[1];		// tagItemOrder
CMD_END


//----------------------------------------------------------------------------
// ��Ʒλ�÷����仯
//----------------------------------------------------------------------------
CMD_START(NC_GroundItem)
	INT64			n64Serial;
CMD_END

CMD_START(NS_ItemAdd)
//	DWORD			dwErrorCode;
	INT64			n64Serial;
	EItemConType	eConType;
	INT16			n16Index;		// ������λ���±�
	INT16			n16Num;			// ����ǿɶѵ���Ʒ,��Ϊ�ö�����
	bool			bOverlap;		// �ص���Ʒ�Ƿ�������жѣ����true���迼���ͷ��ڴ�
CMD_END

CMD_START(NS_NewItemAdd)
	tagItem			Item;
CMD_END

CMD_START(NS_NewEquipAdd)
	tagEquip		Equip;
CMD_END

CMD_START(NC_ItemRemove)
	EItemConType	eConType;
	INT64			n64Serial;
CMD_END

CMD_START(NS_ItemRemove)
//	DWORD			dwErrorCode;
	EItemConType	eConType;
	INT64			n64Serial;
	INT16			n16Index;
	INT16			n16Num;			// ����ǿɶѵ���Ʒ,��Ϊʣ�����,Ϊ0��ʾȫ��ɾ��
CMD_END

CMD_START(NS_ItemCDUpdate)
	DWORD			dwTypeID;
	DWORD			dwCDTime;
CMD_END;


//----------------------------------------------------------------------------
// ���Ҽ������
//----------------------------------------------------------------------------
CMD_START(NC_SetBagPsd)		// ��������
	DWORD	dwBagPsdCrc;	// �����õ���������crcֵ
CMD_END;

//CMD_START(NS_SetBagPsd)
//	DWORD	dwErrorCode;
//CMD_END;

CMD_START(NC_UnsetBagPsd)	// ȡ������
	DWORD	dwSafeCodeCrc;	// ��ȫ���crcֵ
CMD_END;

//CMD_START(NS_UnsetBagPsd)
//	DWORD	dwErrorCode;
//CMD_END;

CMD_START(NC_OldBagPsd)		// ������
	DWORD	dwOldBagPsdCrc;
CMD_END;

//CMD_START(NS_OldBagPsd)
//	DWORD	dwErrorCode;
//CMD_END;

CMD_START(NC_ResetBagPsd)	// �޸�����
	DWORD	dwOldBagPsdCrc;
	DWORD	dwNewBagPsdCrc;
CMD_END;

//CMD_START(NS_ResetBagPsd)
//	DWORD	dwErrorCode;
//CMD_END;

CMD_START(NC_OpenBagPsd)	// �����ܣ��򿪱���ʱ���ȷ��͸���Ϣ
	DWORD	dwBagPsdCrc;
CMD_END;

//CMD_START(NS_OpenBagPsd)
//	DWORD	dwErrorCode;
//CMD_END;

CMD_START(NS_BagPsd)		// �������������Ϣ����
	DWORD	dwErrorCode;
	DWORD	dwRoleStateEx;	// ֻ������Լ�֪����״̬
CMD_END;


//----------------------------------------------------------------------------
// ��������ո�¶ȸ���
//----------------------------------------------------------------------------
CMD_START(NS_RWNewessChange)
	INT32	nAttackTimes;		// ����������ǰʹ�ô���
CMD_END

//----------------------------------------------------------------------------
// ��������
//----------------------------------------------------------------------------
CMD_START(NS_LongHunOn)		// ָ����λװ�������������Ѽ���
	INT16	n16EquipPos;
CMD_END

CMD_START(NS_LongHunOff)	// ָ����λװ������������δ����
	INT16	n16EquipPos;
CMD_END

//----------------------------------------------------------------------------
// ʹ��Ⱦ��Ϊʱװ��ɫ���
//----------------------------------------------------------------------------
CMD_START(NC_DyeFashion)
    INT64   n64DyeSN;       // Ⱦ��64λid
    INT64   n64EquipSerial; // ʱװװ��64λid
CMD_END

CMD_START(NS_DyeFashion)
    DWORD   dwErrCode;      // ������
CMD_END

//----------------------------------------------------------------------------
// �ٱ�����¼���
//----------------------------------------------------------------------------
CMD_START(NC_InitBaiBaoRecord)	// ����������ͳ�ʼ���ٱ�����¼����
CMD_END

CMD_START(NS_InitBaiBaoRecord)	// �������������е���Ϣ
	INT16	n16Num;				// ��¼����
	BYTE	byData[1];			// tagBaiBaoRecord�����ȼ���ȡn16Size����Ҫ��sizeof
CMD_END

CMD_START(NS_SingleBaiBaoRecord)// �������ʱ�����������͵����ٱ�����¼
	tagBaiBaoRecord	sRecord;	// ������¼
CMD_END

//----------------------------------------------------------------------------
// ������ֿ�
//----------------------------------------------------------------------------
CMD_START(NC_WareOpen)
	INT64	n64ItemID;
CMD_END

CMD_START(NS_WareOpen)
	DWORD	dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
//	���ʯ
//-----------------------------------------------------------------------------
CMD_START(NC_BeGoldStone)
	INT64		n64SrcItemID;	// ���ʯ
	INT64		n64DstItemID;	// ���ʯҪ��������Ʒ
CMD_END
CMD_START(NS_BeGoldStone)
	DWORD		dwErrCode;		// ���ʯ�������
CMD_END

//-----------------------------------------------------------------------------
//	��Ʒ��
//-----------------------------------------------------------------------------
CMD_START(NS_ItemBind)
	EItemConType	eConType;
	INT64			n64Serial;
CMD_END

#pragma pack(pop)
