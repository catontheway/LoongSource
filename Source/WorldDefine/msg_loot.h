//-----------------------------------------------------------------------------
//!\file msg_loot.h
//!
//!\date 2008-10-22
//!
//!\brief �ͻ����������֮����ڵ������Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "loot.h"

#pragma pack(push, 1)

enum
{
	E_Loot_BAG_NOT_ENOUGH_SPACE = 1,	//����û���㹻�ռ�
	E_Loot_Item_Not_Found		= 2,	//ʰȡ��Ʒû�ҵ�
	E_Loot_Role_Not_Found		= 3,	//ʰȡ���û�ҵ�
	E_Loot_Map_Not_Found		= 4,	//ʰȡ������ڵ�ͼû�ҵ�
	E_Loot_Add_Bag_Not_Success  = 5,	//���뱳�����ɹ�
	E_Loot_Disard_Item_Not_Success  = 6,	//���뱳�����ɹ�
	E_Loot_Pick_Up_Too_Far			= 7,	//��Ҿ�����Ʒ̫Զ,�޷�ʰȡ
	E_Loot_Belong_To_Team			= 8,	//ֻ�б�С��������Ҳ���ʰȡ
	E_Loot_Belong_To_Teammate		= 9,	//ֻ�б�С����ָ������Ҳ���ʰȡ
	E_Loot_Belong_To_You			= 10,	//����Ʒ��������
};

//-----------------------------------------------------------------------------
//	��������ͬ��	S------>C
//-----------------------------------------------------------------------------
CMD_START(NS_SyncGroundItem)
	INT64	n64Serial;			// �������к�
	DWORD	dwPutDownUnitID;	// ������Ʒ��UintID
	DWORD	dwOwnerID;			// ������ɫ
	DWORD	dwTeamID;			// ����С��ID
	DWORD	dwGroupID;			// �����Ŷ�ID
	DWORD	dwTypeID;			// ����
	INT		nNum;				// ��Ŀ
	Vector3 vPos;				// ��Ʒ����λ��
	BOOL    bKilled;			// �����Ƿ񱻻�ɱ���������
CMD_END;

//-----------------------------------------------------------------------------
//	����������ʧ---���������ʱ�䵽 �䳤  S------>C
//-----------------------------------------------------------------------------
CMD_START(NS_RoleGroundItemDisappear)
	INT64	n64Serial[1];	// �������к�
CMD_END;

//-----------------------------------------------------------------------------
//	��Ʒ����---�����ﱻ��ɱ���������,��Ʒ���䵽����  S------>C
//-----------------------------------------------------------------------------
// CMD_START(NS_PutDownItem)
// 	INT64		n64Serial;	// �������к�
// 	DWORD		dwRoleID;	// ˭�����
// 	DWORD		dwOwnerID;	// ������ɫ
// 	DWORD		dwGroupID;	// ��������ID
// 	DWORD		dwTypeID;	// ����
// 	INT			nNum;		// ����
// 	Vector3		vPos;		//��Ʒ����λ��
// CMD_END

//-----------------------------------------------------------------------------
//	��������������Ʒ---�������ٴ���Ʒ 
//-----------------------------------------------------------------------------
CMD_START(NC_RolePutDownItem)
	INT64	n64Serial;	// �������к�
CMD_END;

CMD_START(NS_RolePutDownItem)
	DWORD	dwErrorCode;	//���ش�����
CMD_END

//-----------------------------------------------------------------------------
//	ʰȡ��Ʒ
//-----------------------------------------------------------------------------
CMD_START(NC_RolePickUpItem)
	INT64 n64Serial;		//��ƷID 
CMD_END

CMD_START(NS_RolePickUpItem)
	DWORD dwRoleID;			// ʰȡ��ID
	INT64 n64Serial;        // ��Ʒ64λID
	DWORD dwErrorCode;		// ���ش�����
CMD_END

//-----------------------------------------------------------------------------
// �õ��������������Ϣ
//-----------------------------------------------------------------------------
// CMD_START(NC_GetGroundItemInfo)
// 	INT64 n64Serial;		//��ƷID 
// CMD_END
// 
// CMD_START(NS_GetGroundItemInfo)
// 	INT64 n64Serial;        // ��Ʒ64λID
// 	DWORD dwErrorCode;		// ���ش�����
// CMD_END

#pragma pack(pop)
