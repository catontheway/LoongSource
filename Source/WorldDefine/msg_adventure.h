//-----------------------------------------------------------------------------
//!\file msg_adventure.h
//!
//!\date 2009-02-17
//!
//!\brief �ͻ����������֮�������������Ϣ
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push, 1)

// ����ģʽ
enum EAdventureType
{
	EAT_Instance,			// ��������
	EAT_GodLargess,			// ����͸�
	EAT_DestinyBox			// ���˱���
};

// ����͸�����
enum EGodLargessContent
{
	EGLC_Item,				// װ����Ʒ 
	EGLC_Task,				// ��������
	EGLC_Money,				// ��Ǯ����
	EGLC_Buf				// ����״̬
};


// ���˱�������
enum EDestinyBoxContent
{
	EDBC_MonsterSiege,		// ���﹥��
	EDBC_AwakenLich,		// �ų�����
	EDBC_Strong,			// ͭǽ����
	EDBC_FrontAndRear,		// �����ܵ�
	EDBC_Money,				// ��������
	EDBC_HelpWorld,			// �������
	EDBC_HarmWorld,			// ���֮��
};

//------------------------------------------------------------------------------
// ��ɫ��������
//------------------------------------------------------------------------------
CMD_START(NS_ChooseAdventure)
	EAdventureType	eType;
CMD_END

//------------------------------------------------------------------------------
// ����Ƿ�ͬ���������
//------------------------------------------------------------------------------
CMD_START(NC_AdventureAgree)
	EAdventureType eType;
	bool	bAgree;
CMD_END

//------------------------------------------------------------------------------
// ֪ͨ���ж�������Ƿ������������
//------------------------------------------------------------------------------
CMD_START(NS_AdventureNotify)
	DWORD	dwInviterID;
CMD_END

//------------------------------------------------------------------------------
// ͬ���������������֪ͨ
//------------------------------------------------------------------------------
CMD_START(NC_AdventureNotifyAgree)
	bool bAgree;
CMD_END

//------------------------------------------------------------------------------
// ���˱��䣬����ȫ���㲥
//------------------------------------------------------------------------------

CMD_START(NS_DestinyBoxBroadcast)
	TCHAR	PlayerName[X_SHORT_NAME];
	EDestinyBoxContent	eType;
	DWORD	dwID;				//��Ҫ��ʾID�Ĵ���ID������Ҫ��ʾ��ΪGT_INVALID
CMD_END

#pragma pack(pop)