/*******************************

�������ͻ��˹���ʵ������Ϣ

*******************************/

#pragma once

#include "msg_common_errorcode.h"
#include "role_info.h"

#pragma pack(push, 1)

enum ERankingType
{
	ERT_Strength,	// ʵ��ֵ
	ERT_Level,		// ��ɫ�ȼ�
	ERT_Money,		// ��Ǯ
	ERT_Fame		// ����
};
struct tagRankingInfo
{	
	INT				nCurRanking;		// ��ǰ����
	INT				nRankingChange;		// �����仯
	INT64			n64Value;			// ֵ
	tagRoleIDName	sIDName;			// ��ɫID������
};

//-----------------------------------------------------------
// ��ȡ������ʵ������
//-----------------------------------------------------------
CMD_START(NC_GetRankingInfo)
	INT				nPage;				// ����������ҳ�����ɷ������Լ����㣬nPage��1��ʼ
	ERankingType	eType;				// Ҫ�������е�����
CMD_END;

//-----------------------------------------------------------
// ������������������Ϣ
//-----------------------------------------------------------
CMD_START(NS_GetRankingInfo)
	INT				nPage;				// ����������ҳ��
	INT				nLocalPlayerRank;	// �����������
	ERankingType	eType;				// ��������
	tagRankingInfo	sData[20];			// ��ȡ��������Ϣ
CMD_END;

//-----------------------------------------------------------
// �����ȡ�Լ���ʵ��ֵ
//-----------------------------------------------------------
CMD_START(NC_GetSelfStrength)
CMD_END;

//-----------------------------------------------------------
// ��ȡ�Լ���ʵ��ֵ
//-----------------------------------------------------------
CMD_START(NS_GetSelfStrength)
	DWORD		dwSelfStrength;			// ��ǰ���ʵ��ֵ
	DWORD		dwHero;					// Ӣ��ֵ
	DWORD		dwSmart;				// ���ֵ
	DWORD		dwChivalrous;			// ����ֵ
	DWORD		dwWarfare;				// ��սֵ
CMD_END

#pragma pack(pop)
