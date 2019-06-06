//--------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: cost_type_define.h
// author: 
// actor:
// data: 2008-11-10
// last:
// brief: �������Ͷ���
//--------------------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------------------
// ��������ö��
//--------------------------------------------------------------------------------------------
enum ECurCostType
{
	ECCT_Null				= 0,	// ��

	ECCT_BagSilver			= 1,	// ��	-- ������
	ECCT_BagYuanBao			= 2,	// Ԫ�� -- ������
	ECCT_WareSilver			= 3,	// ��	-- ��ɫ�ֿ���
	ECCT_BaiBaoYuanBao		= 4,	// Ԫ�� -- ��ɫ�ֿ���
	ECCT_ExchangeVolume		= 5,	// �һ�ȯ -- ��ɫ�̳�

	ECCT_GuildContribe		= 10,	// ���ɹ���
	ECCT_GuildExploit		= 11,	// ���ɹ�ѫ

	ECCT_ClanConXuanYuan	= 120,	// ��ԯ����
	ECCT_ClanConShenNong	= 121,	// ��ũ����
	ECCT_ClanConFuXi		= 122,	// ���˹���
	ECCT_ClanConSanMiao		= 123,	// ���繱��
	ECCT_ClanConJiuLi		= 124,	// ���蹱��
	ECCT_ClanConYueZhi		= 125,	// ���Ϲ���
	ECCT_ClanConNvWa		= 126,	// Ů洹���
	ECCT_ClanConGongGong	= 127,	// ��������

	ECCT_ItemExchange		= 40,	// ��Ʒ�һ�
};

// ����ö��ת��Ϊ��������
#define MTRANS_ECCT2ECLT( ecct ) 	((ECLanType)((ecct) - ECCT_ClanConXuanYuan))
