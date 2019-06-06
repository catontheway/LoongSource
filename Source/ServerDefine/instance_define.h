#include "../WorldDefine/RoleDefine.h"

#pragma once

#pragma pack(push, 1)

#define		RAND_CTEATUTE_NUM	3					// ���������������صĹ�������
#define		RAND_INSTANCE_NUM	3					// ��������Ѷ����͵�����

struct tagRandSpawnPointInfo						// �������ˢ�ֵ�
{
	DWORD		dwSpawnPointID;						// ˢ�ֵ�ID(��ID��
	INT			nLevel;
	DWORD		dwNormalID[RAND_CTEATUTE_NUM];		// ��ͨ�Ѷȸ�������ID		
	DWORD		dwEliteID[RAND_CTEATUTE_NUM];		// ��Ӣ�Ѷȸ�������ID
	DWORD		dwDevilID[RAND_CTEATUTE_NUM];		// ħ���Ѷȸ�������ID
};

struct tagInstanceItem
{
	DWORD					dwMapID;
	TMap<DWORD, DWORD>		mapInstanceItem;
};

struct tagInstanceSkill
{
	DWORD					dwMapID;
	TMap<DWORD, DWORD>		mapInstanceSkill;
};

struct tagLevelMapping
{
	INT						nLevel;
	INT						nTransmitLevel;
};

#pragma pack(pop)