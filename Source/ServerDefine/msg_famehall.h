//-----------------------------------------------------------------------------
// filename: msg_famehall.h
// author: 
// actor:
// data: 2009-02-25
// last: 
// brief: �����÷�������Ϣ��ʽ
//-----------------------------------------------------------------------------
#pragma once
#include "../ServerDefine/famehall_define.h"
#include "../WorldDefine/reputation.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// ������������ʱ���
//-----------------------------------------------------------------------------
CMD_START(NDBC_UpDateRepRstTime)
	DWORD	dwRepRstTimeStamp;
	BYTE	byClanType;		
CMD_END;


//-----------------------------------------------------------------------------
// �����������
//-----------------------------------------------------------------------------
CMD_START(NDBC_GetRepRankList)
	BYTE	byClanType;	
CMD_END;

CMD_START(NDBS_GetRepRankList)
	BYTE			byClanType;
	DWORD			dwErrorCode;
	INT16			nRecNum;
	tagRepRankData	repRank[MAX_REP_RANK_NUM];
CMD_END;

//-----------------------------------------------------------------------------
// �����ó�ʼ��
//-----------------------------------------------------------------------------
CMD_START(NDBC_FameHallInitStart)
CMD_END;

//-----------------------------------------------------------------------------
// ��������ʱ���
//-----------------------------------------------------------------------------
CMD_START(NDBS_GetRepResetTimestamp)
	DWORD	dwResetTime[ECLT_NUM];	// tagDWORDTime
CMD_END;

//-----------------------------------------------------------------------------
// ����Ѽ����䱦
//-----------------------------------------------------------------------------
CMD_START(NDBS_GetActTreasureList)
	INT16			nRecNum[ECLT_NUM];
	tagTreasureData	treasureData[MAX_ACT_TREASURE_NUM];
CMD_END;

//-----------------------------------------------------------------------------
// ��û�ý��������ÿ���
//-----------------------------------------------------------------------------
CMD_START(NDBS_GetFameHallEnterSnap)
	INT16					nRecNum[ECLT_NUM];
	tagFameHallEnterSnap	enterSnap[MAX_ENTER_FAMEHALL_SNAP_NUM * ECLT_NUM];		// tagFameHallEnterSnap
CMD_END;

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
CMD_START(NDBC_InsertFameHallSnap)
	BYTE	byClanType;				// ��������
	DWORD	dwRoleID;				// ��ɫid
	DWORD	dwEnterNameID;			// ����������������id
	DWORD	dwEnterTime;			// tagDWORDTime
CMD_END;


//-----------------------------------------------------------------------------
// �����䱦
//-----------------------------------------------------------------------------
CMD_START(NDBC_InsertActClanTreasure)
	BYTE	byClanType;				// ��������
	UINT16	u16TreasureID;			// �䱦��������Ʒid
	DWORD	dwRoleID;				// �����䱦������id
	DWORD	dwActNameID;			// �����䱦ʱ������id
	DWORD	dwActTime;				// ����ʱ��
CMD_END;


#pragma pack(pop)