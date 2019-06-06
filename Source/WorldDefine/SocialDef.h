#pragma once

#pragma pack(push, 1)

const INT MAX_FRIENDNUM					= 100;		// ����������
const INT MAX_BLACKLIST					= 20;		// �����������
const INT MAX_FRIENDVAL					= 9999;		// ���Ѷ����ֵ
const INT MAX_ENEMYNUM					= 20;		// ���������
const INT GIFT_STORE_TIME				= 60 * TICK_PER_SECOND;	// �����ڹܵ��ﱣ��ʱ��

struct tagFriend
{
	DWORD	dwFriendID;								// ����ID
	DWORD	dwFriVal;								// ���Ѷ�
	BYTE	byGroup;								// ����
	tagFriend():dwFriendID(GT_INVALID),dwFriVal(0),byGroup(1){}
};

struct tagFriendInfo
{
	DWORD	dwFriendID;								// ����ID
	DWORD	dwFriVal;								// ���Ѷ�
	BYTE	byGroup;								// ����
	BOOL    bOnline;								// �����Ƿ�����
	INT		nLevel;									// ��ɫ�ȼ�
};

struct tagSendGift				
{
	DWORD	dwSrcRoleID;							// ������
	DWORD	dwDestRoleID;							// ������
	DWORD	dwTypeID;								// ����TypeID
	INT 	nStoreTick;								// ����ʱ��
	tagSendGift(DWORD dwSrcRoleID, DWORD dwDestRoleID, DWORD dwTypeID):
		dwSrcRoleID(dwSrcRoleID),dwDestRoleID(dwDestRoleID),dwTypeID(dwTypeID)
	{
		nStoreTick = GIFT_STORE_TIME;
	}
};

struct tagFriUpdate
{
	DWORD   dwRoleID;								// ����ID
	BOOL	bOnline;								// �Ƿ�����
	INT		nLevel;									// �ȼ�
};

#pragma pack(pop)