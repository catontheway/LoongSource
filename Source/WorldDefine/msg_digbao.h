//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_mall.h
// author: 
// actor:
// data: 2009-02-9
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ -- �ڱ�����
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)

#include "msg_common_errorcode.h"
#define MAX_CHEST_NUM 16

//��ͨ�ر�ͼ������ʾ
enum ENormalTreasureMap 
{
	ENTM_NULL		= 1,
	ENTM_Nothing,					//һ������
	ENTM_GainMoney,					//��Ǯ����
	ENTM_ConfrontTrap,				//��������
	ENTM_Billiken,					//�����·�
	ENTM_GainOlMap,					//��ù��ϲر�ͼ
	ENTM_GainKey,					//���Կ��
	ENTM_GainOldChest,				//��þ�����
	ENTM_DriftBottle,				//Ư��ƿ
	ENTM_GainLoongSquamaChest,		//������۱���
	ENTM_BadOldSeal,				//�ڻ��Ϲŷ�ӡ
	ENTM_Plutus,					//�����·�
};

//���ϲر�ͼ������ʾ
enum EOldTreasureMap
{
	EOTM_NULL			=1,
	EOTM_Billiken,					//�����·�
	EOTM_GainLoongSquamaChest,		//������۱���
	EOTM_GainChest,					//�������
	EOTM_GainLoongChest,			//�����������
	EOTM_GainFourTreasures,			//����ķ��ı�
	EOTM_GainKey,					//���Կ��
	EOTM_GainRareTreaMap,			//��þ����ر�ͼ
	EOTM_Plutus,					//�ڵ�����ү
	EOTM_DungeonEnter,				//�����ܾ����
	EOTM_BadLoongPulse,				//�������
	EOTM_BadOldSeal,				//�ڻ��Ϲŷ�ӡ
	EOTM_AwakenGods,				//�����Ϲ�֮��
};

//�����ر�ͼ������ʾ
enum ERareTreasureMap
{
	ERTM_NULL			= 1,
	ERTM_GainMoney,					//��Ǯ����
	ERTM_GainRareStuff,				//���ϡ�в�������
	ERTM_GainDelicacyChest,			//��þ��µ�����
	ERTM_GainLoongChest,			//�����������
	ERTM_GainLoongSquamaChest,		//������۱���
	ERTM_GodsOfPractice,			//��֮����
	ERTM_MonsterAttackCity,			//���Ź��﹥�ǻ
	ERTM_DungeonEnter,				//�����ܾ����
	ERTM_AwakenGods,				//�����Ϲ�֮��
};

CMD_START(NC_DiaBao)
	DWORD				dwPlayerID;				//�ڱ���ID
	DWORD				dwItemID;				//����ID
	DWORD				dwItemTypdID;			//�����TypeID
CMD_END

CMD_START(NS_DiaBao)
	DWORD				dwErrorCode;			//������
	Vector3				v3Pos;					//�����λ��(������ʧ��,������Ч)
CMD_END

//��ͨ�ر�ͼ������Ϣ
CMD_START(NS_NormalTreasureMap)
	ENormalTreasureMap	eType;					//��ͨ�ر�ͼ������ʾ
CMD_END

//���ϲر�ͼ������Ϣ
CMD_START(NS_OldTreasureMap)
	EOldTreasureMap		eType;					//���ϲر�ͼ������ʾ
CMD_END

//�����ر�ͼ������Ϣ
CMD_START(NS_RareTreasureMap)
	ERareTreasureMap	eType;					//�����ر�ͼ������ʾ
CMD_END

//��������FB
CMD_START(NC_EnterAdventureFB)
CMD_END

CMD_START(NS_EnterAdventureFB)
CMD_END

//����
CMD_START(NC_Experience)
CMD_END

CMD_START(NS_Experience)
CMD_END

//--------------------------------------------------------------------------------
//����Ϊ������
//--------------------------------------------------------------------------------
enum EChestType
{
	EChestType_NULL			= 0,
	EChestType_LoongSquamaChest,		//���۱���
	EChestType_WackeLoongChest,			//��������
	EChestType_GodLargessChest,			//����͸�����
	EChestType_DestinyBoxChest,			//���˱���
};

CMD_START(NC_TreasureChest)
	INT64			n64ChestID;			//����(���۱������������)
	INT64			n64KeyID;			//Կ��
CMD_END

CMD_START(NS_TreasureChest)
	DWORD			dwChestTypeID;				//�����TypdID
	DWORD			dwTypeID[MAX_CHEST_NUM];	//��ƷID
	INT				nNum[MAX_CHEST_NUM];		//����������
	DWORD			dwErrorCode;
CMD_END


CMD_START(NC_StopTreasureChest)
	INT64			n64ChestID;			//����
	INT64			n64KeyID;			//Կ��
CMD_END

CMD_START(NS_StopTreasureChest);
	DWORD			dwTypeID;			//��������Ʒ
	INT				nNum;				//����������
	DWORD			dwErrorCode;
CMD_END

CMD_START(NC_AgainTreasureChest)
	INT64			n64ChestID;			//����
	INT64			n64KeyID;			//Կ��
CMD_END

CMD_START(NS_AgainTreasureChest)
	BYTE			byDestroy;			//�ٻ�Ϊ1,û�ٻ�Ϊ0
	DWORD			dwErrorCode;
CMD_END

CMD_START(NC_ChestGetItem)
	DWORD			dwTypeID;
CMD_END

CMD_START(NS_ChestGetItem)
	DWORD			dwErrorCode;
CMD_END

#pragma pack(pop)