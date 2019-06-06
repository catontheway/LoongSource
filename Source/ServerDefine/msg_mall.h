//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_mall.h
// author: 
// actor:
// data: 2008-05-25
// last:
// brief: �̳����
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push)
#pragma pack(1)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ö��
//-----------------------------------------------------------------------------
enum
{
	E_DBLoad_GroupPurchase_Faild		= 1,		// ��ȡ�����Ź���Ϣʧ��
};

//-----------------------------------------------------------------------------
// �����ýṹ
//-----------------------------------------------------------------------------
struct tagGPInfoKey
{
	DWORD			dwGuildID;
	DWORD			dwRoleID;
	DWORD			dwMallID;
};

struct tagGPInfo : public tagGPInfoKey 
{
	INT32			nPrice;				//�Ź��۸�
	INT16			nParticipatorNum;	//��Ӧ����
	INT16			nRequireNum;		//�Ź���������
	INT32			nRemainTime;		//ʣ��ʱ��
	DWORD			dwParticipators[1];	//��Ӧ���б�

	INT32 Size()
	{
		return sizeof(tagGPInfo) + (nParticipatorNum-1) * sizeof(DWORD);
	}
};

//-----------------------------------------------------------------------------
// ��Ϣ�ṹ
//-----------------------------------------------------------------------------
CMD_START(NDBC_GetAllGPInfo)
CMD_END

CMD_START(NDBS_GetAllGPInfo)
	DWORD			dwErrorCode;
	INT				nGPInfoNum;
	tagGPInfo		GPInfoData[1];
CMD_END

CMD_START(NDBC_AddGPInfo)
	tagGPInfo		GPInfo;
CMD_END

CMD_START(NDBC_RemoveGPInfo)
	tagGPInfoKey	GpInfoKey;
CMD_END

CMD_START(NDBC_RemoveGuildGPInfo)
	DWORD			dwGuildID;
CMD_END

CMD_START(NDBC_UpdateGPInfo)
	tagGPInfoKey	GpInfoKey;
	DWORD			dwNewParticipator;	//�µ���Ӧ��ID
CMD_END

CMD_START(NDBC_UpdateGPTime)
CMD_END

//-----------------------------------------------------------------------------
#pragma pack(pop)
