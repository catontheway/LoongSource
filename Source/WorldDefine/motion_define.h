#pragma once
#pragma pack(push,1)

enum EMotionType1
{
	EMT1_Null			= -1,
	EMT1_Self			= 0,
	EMT1_Interactive,
};


//-------------------------------------------------------------------------
// ���Զ���
//-------------------------------------------------------------------------
struct tagMotionProto
{
	DWORD			dwTypeID;		// id
	EMotionType1	eMotionType;	// ��������
	DWORD			dwFriendVal;		// �Ѻö�Ҫ��
};

#pragma pack(pop)