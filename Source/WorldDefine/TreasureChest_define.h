//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: TreasureChest_define.h
// author: twan
// actor:
// data: 2009-07-29
// last:
// brief: �������Զ���
//-----------------------------------------------------------------------------

#pragma once

const INT ROLE_CHSET_RATE		=	100;			// ��ɫ�ڵ�
const INT SERVER_CHEST_RATE		=	1000;			// �������ڵ�

//--------------------------------------------------------------------------------------------
// ���ֵ���ö��
//--------------------------------------------------------------------------------------------
enum ERateType
{
	ERTT_BeginRate			= 0,	// ��ʼ����
	ERTT_RoleRate			= 1,	// ��ɫ�ڵ����
	ERTT_ServerRate			= 2,	// �������ڵ����
	ERTT_NormalRate			= 3,	// ��ͨ����
};

//	��������Ʒ����
struct tagChestItem 
{
	DWORD				dwTypeID;			//��Ʒid
	INT					nTableID;			//�ڱ�����Ʒ�е�id���ô�id����100��ȷ������Ʒ�������ֱ���
	INT					nNum;				//��Ʒ����������
	FLOAT				fBeginRate;			//��ʼ����
	FLOAT				fRoleRate;			//��ɫ�ڵ����
	FLOAT				fServerRate;		//�������ڵ����
	FLOAT				fNormalRate;		//��ͨ����
};

// ������״̬
struct tagChestState
{	
	tagChestItem		ChestItem;		// ��ҿ���������п�������Ʒ
	INT64				nChestSerial;		// ��ҿ���������б����serialID
	INT64				nKeySerial;		// ��ҿ����������Կ�׵�serialID
	DWORD				dwChestTypeID;	// �����typeid
	DWORD				dwKeyTypeID;		// Կ�׵�typeid	
};