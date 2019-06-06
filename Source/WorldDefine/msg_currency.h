//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_currency.h
// author: 
// actor:
// data: 2008-11-06
// last:
// brief: ������Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"

#pragma pack(push, 1)
//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum
{

};

//----------------------------------------------------------------------------
// ��Ϣ�ṹ
//----------------------------------------------------------------------------
CMD_START(NS_BagSilver)
	INT64		n64CurSilver;		// ��ǰ�ܵĽ�Ǯ
	INT64		n64ChangeSilver;	// �仯�Ľ�Ǯ(��: ���; ��: ʧȥ)
CMD_END;

CMD_START(NS_WareSilver)
	INT64		n64CurSilver;		// ��ǰ�ܵĽ�Ǯ
	INT64		n64ChangeSilver;	// �仯�Ľ�Ǯ(��: ���; ��: ʧȥ)
CMD_END;

CMD_START(NS_BagYuanBao)
	INT32		nCurYuanBao;		// ��ǰ�ܵ�Ԫ��
	INT32		nChangeYuanBao;		// �仯��Ԫ��
CMD_END

CMD_START(NS_BaiBaoYuanBao)
	INT32		nCurYuanBao;		// ��ǰ�ܵ�Ԫ��
	INT32		nChangeYuanBao;		// �仯��Ԫ��
CMD_END

CMD_START(NS_ExchangeVolume)
	INT32		nCurExVolume;		// ��ǰ�ܵ�����
	INT32		nChangeExVolume;	// �仯������
CMD_END

CMD_START(NS_ClanCon)
	BYTE		byClanType;			// ��������
	INT32		nCurClanCon;		// ��ǰ�ܵĹ���
	INT32		nChangeClanCon;		// �仯�Ĺ���
CMD_END

//----------------------------------------------------------------------------
#pragma pack(pop)