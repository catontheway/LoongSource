//-----------------------------------------------------------------------------
//! \filename   msg_pet_skill.h
//! \author     hyu/xlguo
//! \date       2009-5-11
//! \last       2009-5-11
//! \brief      ���＼����Ϣ
//! \sa         pet_define.h
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push,1)

#include "pet_define.h"
#include "pet_equip_define.h"
#include "msg_common_errorcode.h"

// ����װ����Ʒ
CMD_START(NC_PetEquip)
    DWORD       dwPetID;        //!< ����ID
	INT8		n8DstPos;		//!< ����װ����λ��
    INT64       n64ItemID;      //!< 64λitemid
CMD_END

CMD_START(NS_PetEquip)
    DWORD               dwErrCode;      //!< ������
    DWORD               dwPetID;        //!< ����ID
    tagPetEquipMsgInfo  itemData;       //!< ����װ����Ϣ
CMD_END

// ����жװ��Ʒ
CMD_START(NC_PetUnequip)
	DWORD		dwPetID;
    INT16		n16PosDst;		//!< �����뱳����λ��
    INT64       n64ItemID;      //!< 64λitemid
CMD_END

CMD_START(NS_PetUnequip)
    DWORD       dwErrCode;      //!< ������
    DWORD		dwPetID;
    INT64       n64ItemID;      //!< 64λitemid
CMD_END

// ����װ��λ�ý���
CMD_START(NC_PetEquipPosSwap)
	DWORD		dwPetID;
	INT8		n8PosDst;		//!< װ���ƶ�����Ŀ��λ��
	INT64       n64ItemID;      //!< 64λitemid
CMD_END

CMD_START(NS_PetEquipPosSwap)
	DWORD       dwErrCode;      //!< ������
    DWORD		dwPetID;
	INT8		n8PosDst;		//!< װ���ƶ�����Ŀ��λ��
	INT8		n8PosSrc;		//!< װ��ԭλ��
	INT64       n64ItemID;      //!< 64λitemid
CMD_END

#pragma pack(pop)