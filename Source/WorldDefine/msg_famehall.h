//-----------------------------------------------------------------------------
// filename: msg_reputation.h
// author: client/server hyu/xlguo
// actor:
// data: 2009-02-16
// last: 2009-02-16
// brief: ��ɫ�����ͻ��˷������Ϣ����
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)

#include "reputation.h"
#include "famehall_define.h"
//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum
{
	E_FrameHall_Success					= 0,	// �ɹ�
	E_FrameHall_MemberOrderUnchanged	= 1,	// �����ó�Ա�б��ޱ仯
	E_FrameHall_RepOrderUnchanged		= 2,	// ���������ޱ仯
	E_FrameHall_ItemIsNotTreasure		= 3,	// ��Ʒ�����䱦
	E_FrameHall_TreasureAlreadyAct		= 4,	// �䱦�Ѽ���
	E_FrameHall_ActCountNotEnough		= 5,	// �����������
	E_FrameHall_ClanColNotEnough		= 6,	// ���幱�ײ���
	E_FrameHall_UnknownError			,		// δ֪����
};


//----------------------------------------------------------------------------
// ��ȡ����������ǰ50����ɫ�б�
//----------------------------------------------------------------------------
CMD_START(NC_GetFameHallRoles)
	BYTE		byClanType;				// ��������
	DWORD		dwUpdateTime;			// ����ʱ��� tagDWORDTime
CMD_END

CMD_START(NS_GetFameHallRoles)
	BYTE		eClanType;				// ��������EClanType
	BYTE		byErrCode;				// �Ƿ��б��ޱ仯��ΪE_FrameHall_MemberOrderUnchanged
	DWORD		dwUpdateTime;			// ������ʱ���
	BYTE		byRoleNum;				// �����������б�����(����50)
	BYTE		byData[1];				// ��ԱnameID�б�����(DWORD)
CMD_END


//----------------------------------------------------------------------------
// ����ǰ50����ɫ�б�
//----------------------------------------------------------------------------
CMD_START(NC_GetReputeTop)
	BYTE 		byClanType;				// ��������
	DWORD		dwUpdateTime;			// ����ʱ��� tagDWORDTime
CMD_END

CMD_START(NS_GetReputeTop)
	BYTE		byClanType;				// ��������EClanType
	BYTE		byErrCode;				// �Ƿ��б��ޱ仯��ΪE_FrameHall_RepOrderUnchanged
	DWORD		dwUpdateTime;			// ������ʱ���
	BYTE		byRoleNum;				// ÿ�����������б�����(����50)
	BYTE		byData[1];				// �䳤�б�����(tagRepRankData)
CMD_END



//----------------------------------------------------------------------------
// ����������м����䱦
//----------------------------------------------------------------------------
CMD_START(NC_GetActClanTreasure)
	BYTE		byClanType;				// ��������EClanType
CMD_END

CMD_START(NS_GetActClanTreasure)
	BYTE		byClanType;				// ��������EClanType
	INT16		n16ActTreasureNum;		// �Ѽ����䱦����
	BYTE		byData[1];				// ����tagTreasureData
CMD_END

//----------------------------------------------------------------------------
// �¼��������䱦֪ͨ
//----------------------------------------------------------------------------
CMD_START(NS_NewActivedTreasure)
	INT16		n16TreasureID;
	DWORD		dwNameID;
CMD_END

//----------------------------------------------------------------------------
// �����ϻ��䱦
//----------------------------------------------------------------------------
CMD_START(NC_ActiveTreasure)
	UINT16		u16TreasureID;			// �䱦ID
CMD_END

CMD_START(NS_ActiveTreasure)
	DWORD		dwErrCode;				// ������
	UINT16		u16TreasureID;			// �䱦ID
CMD_END

#pragma pack(pop)