/*******************************

�������ͻ��˹���ʵ������Ϣ

*******************************/

#pragma once

#include "msg_common_errorcode.h"
#include "role_info.h"

#pragma pack(push, 1)

const INT MAX_RANKINGS_LEN = 500;
const INT RANKINGS_PAGE_LEN = 20;

enum ERankingType
{
	ERT_Strength,	// ʵ��ֵ
	ERT_Level,		// ��ɫ�ȼ�
	//ERT_Money,		// ��Ǯ
	ERT_Fame,		// ����
	ERT_Pet,		// ����
	ERT_Equip,		// ����
	ERT_All
};

struct tagRankingInfo
{	
	INT				nCurRanking;		// ��ǰ����
	INT				nRankingChange;		// �����仯,�������ϴ�������500�����⣬�����Ž���ǰ500����ô�޷��õ�����ı仯ֵ
										// ���磺��ұ�������Ϊ450���ϴ�����Ϊ500���⣬��ô��ֵΪ 500 + ��500 - 450��= 550
										// �ͻ��˵Ĵ���Ϊ�� 550 - 500 = 50����ʾ�����仯Ϊ������ >50
	INT64			n64Value;			// ֵ
	tagRoleIDName	sIDName;			// ��ɫID������
};

struct tagPetRankingInfo
{
	INT				nCurRanking;		// ��ǰ����
	INT				nRankingChange;		// �����仯
	TCHAR			szPetName[X_SHORT_NAME];		// ���������
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
	INT				nPage;								// ����������ҳ��
	INT				nLocalPlayerRank;					// �����������
	ERankingType	eType;								// ��������
	tagRankingInfo	sData[RANKINGS_PAGE_LEN];			// ��ȡ��������Ϣ
CMD_END;

CMD_START(NS_GetPetRankingInfo)
	INT				nPage;								// ����������ҳ��
	INT				nLocalPlayerRank;					// �����������
	//ERankingType	eType;								// ��������
	tagPetRankingInfo	sData[RANKINGS_PAGE_LEN];			// ��ȡ��������Ϣ
CMD_END;

#pragma pack(pop)
