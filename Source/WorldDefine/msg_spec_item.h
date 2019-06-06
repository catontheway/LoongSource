#pragma once
#pragma pack(push, 1)

enum ESpecItemUseError
{
	E_ItemLock_HaveLocked = 1,		//��Ʒ�Ѿ�������
	E_ItemLock_NoLocked,			//��Ʒ��δ������
	E_ItemLock_TargetError,			//Ŀ�겻�Ϸ�
	E_ItemLock_HaveNotItem,			//û���������߽��� ����
	E_ItemLock_OtherError,			//��������
};

CMD_START(NC_LockItem)
	INT64			n64Item;		// ���ߵ�64λID
	INT64			n64Target;		// Ҫ����Ʒ��64λID
CMD_END

CMD_START(NS_LockItem)
	DWORD			dwErrorCode;	// �������
	BYTE			byType;			// 0�����������ķ��أ�1�����������ķ���
	DWORD			dwUnlockTime;	// ��������ʱ�����ؽ���ʱ��
	INT64			n64Serial;		// Ŀ����ƷID
CMD_END

#pragma pack(pop)