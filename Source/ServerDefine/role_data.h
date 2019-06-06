//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_data.h
// author: 
// actor:
// data: 2008-06-04
// last:
// brief: �������ݵĶ�ȡ�ͱ��������Ϣ
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push)
#pragma pack(1)

const INT32 MAX_ITEM_NEEDLOG_NUM		= 100000;


#include "../WorldDefine/msg_common_errorcode.h"
#include "../WorldDefine/base_define.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/container_define.h"
#include "../WorldDefine/ItemDefine.h"
#include "role_data_define.h"

enum
{
	E_DBInsert_RoleNameExist		= 1,		// ������ɫ�������Ѵ���(��������ɫ������������ͬ)
	E_DBInsert_RoleFailed			= 2,		// ���ݿ��д�����ɫʧ��
	E_DBInsert_FriendFailed			= 3,		// ���ݿ�����Ӻ���ʧ��
	E_DBInsert_EnemyFailed			= 4,		// ���ݿ�����ӳ��ʧ��
	E_DBInsert_FriendshipFailed		= 5,		// ���ݿ�������Ѻö�ʧ��
	E_DBInsert_BlackFailed			= 6,		// ��Ӻ�����ʧ��

	E_DBLoad_SimRoleFailed			= 51,		// ���ʺ����޽�ɫ�����ȡ�ʺŽ�ɫ������Ϣʧ��	
	E_DBLoad_RoleDataFailed			= 52,		// �����ݿ��ж�ȡĳ����ɫʧ��	
	E_DBLoad_SkillFailed			= 53,		// �����ݿ��ж�ȡ�����б�ʧ��	
	E_DBLoad_RoleBuffFailed			= 54,		// �����ݿ��ж�ȡ��ɫBuffʧ��	
	//E_DBLoad_TalentFailed			= 55,		// �����ݿ��ж�ȡ�����б�ʧ��	
	E_DBLoad_TitleFailed			= 56,		// �����ݿ��ж�ȡ�ƺ��б�ʧ��	
	E_DBLoad_ItemFailed				= 57,		// �����ݿ��ж�ȡ��Ʒ�б�ʧ��
	E_DBLoad_EquipFailed			= 58,		// �����ݿ��ж�ȡװ���б�ʧ��	
	E_DBLoad_FriendFailed			= 59,		// �����ݿ��ж�ȡ�����б�ʧ��	
	E_DBLoad_EnemyFailed			= 60,		// �����ݿ��ж�ȡ����б�ʧ��	
	E_DBLoad_QuestFailed			= 61,		// �����ݿ��ж�ȡ�����б�ʧ��	
	E_DBLoad_QuestDoneFailed		= 62,		// �����ݿ��ж�ȡ����������б�ʧ��		
	E_DBLoad_SimQuestFailed			= 63,		// �����ݿ��ж�ȡ���������б�ʧ��	
	E_DBLoad_SimQuestDoneFailed		= 64,		// �����ݿ��ж�ȡ����ɼ��������б�ʧ��		
	E_DBLoad_FriendshipFailed		= 65,		// �����ݿ��ж�ȡ���Ѷ��б�ʧ��	

//	E_DBLoad_BaiBao_NoNewItem		= 65,		// û���µ���Ҫ���ŵ��ٱ����е���Ʒ(װ��)
	E_DBLoad_BaiBao_Failed			= 66,
	E_DBLoad_ItemNeedLog_Failed		= 67,
	E_DBSave_RoleFailed				= 70,		// ���ݿⱣ��ʧ��

	E_DBLoad_BaiBaoLog_Failed		= 71,		// �ٱ�����ʷ��¼��ȡʧ��

	E_BackupDelRoleDataFailed		= 80,		// ���ݱ�ɾ����ɫ�������ʧ��
	E_BackupDelItemFailed			= 81,		// ���ݱ�ɾ����ɫ��Ʒ����ʧ��
	E_DelRoleDataFailed				= 82,		// ɾ����ɫ�����������ʧ��
	E_RecordDelRoleDataFailed		= 83,		// ��ɫ����ɾ���ɹ�����дrole_del��ʧ��

//	E_DelItemFailed					= 90,		// ɾ����ɾ����ɫ��Ʒ����ʧ��
//	E_DBDelete_RoleBuffFailed		= 91,		// ɾ����ɫBuff����ʧ��
	E_DBDelete_FriendFailed			= 92,		// ɾ������ʧ��
	E_DBDelete_EnemyFailed			= 93,		// ɾ�����ʧ��
	E_DBDelete_RoleFailed			= 94,		// ɾ����ɫʧ��
	E_DBDelete_FriendshipFailed		= 95,		// ɾ���Ѻö�ʧ��
	E_DBDelete_BlackFailed			= 96,		// ɾ��������ʧ��


	E_ChangeRoleNameFailed			= 100,		// �޸Ľ�ɫ����ʧ��

	E_DBLoad_RepRankData_Failed		= 120,		// ��ȡ������������

	E_DBUpdate_StrengthRankingsFailed	=	130,// ����ʵ�����а�ʧ��
	E_DBUpdate_MoneyRankingsFailed		=	131,// �����ʽ����а�ʧ��
	E_DBUpdate_FameRankingsFailed		=	132,// �����������а�ʧ��
	E_DBUpdate_LevelRankingsFailed		=	133,// ���µȼ����а�ʧ��

};

CMD_START(Heartbeat)			// ����
	DWORD dwGoldenCode;		// ��������ɫ����
	DWORD dwHeartbeat;		// ��������������
CMD_END;


CMD_START(N_Certification)
	DWORD	dwGoldenCode;
	TCHAR	szWorldName[X_SHORT_NAME];
CMD_END;

CMD_START(NDBC_WorldShutDown)
CMD_END;

CMD_START(NDBS_WorldShutDown)
CMD_END;


CMD_START(NDBC_LoadAllRoleInfo)
CMD_END;

CMD_START(NDBS_LoadAllRoleInfo)
	DWORD		dwErrorCode;
	INT32		nNum;
	tagRoleInfo	RoleInfo[1];
CMD_END;

CMD_START(NDBC_LoadSimRole)
	DWORD	dwAccountID;
	CHAR	szAccount[X_SHORT_NAME];
CMD_END;

CMD_START(NDBS_LoadSimRole)
	DWORD				dwErrorCode;
	DWORD				dwAccountID;
	INT32				nNum;
	tagAccountCommon	sAccountCommon;
	tagSimRole			SimRole[MAX_ROLENUM_ONEACCOUNT];
CMD_END;

CMD_START(NDBC_CreateRole)	
	DWORD				dwAccountID;
	tagCreateRoleInfo	RoleInfo;
CMD_END;

CMD_START(NDBS_CreateRole)
	DWORD				dwErrorCode;
	DWORD				dwAccountID;
	tagSimRole			SimRole;
	tagRoleInfo			RoleInfo;
CMD_END;

CMD_START(NDBC_DelRole)
	DWORD		dwAccountID;
	DWORD		dwRoleID;
	DWORD		dwTime;
	TCHAR		szRoleName[X_SHORT_NAME];
	CHAR		szIP[X_IP_LEN];
CMD_END;

CMD_START(NDBS_DelRole)
	DWORD		dwErrorCode;
	DWORD		dwRoleID;
	DWORD		dwAccountID;
CMD_END;

CMD_START(NDBC_LoadRole)
	DWORD		dwAccountID;
	DWORD		dwRoleID;
CMD_END;

CMD_START(NDBS_LoadRole) // ע�����Ϣ�ṹ��С�����¼���
	DWORD			dwErrorCode;
	DWORD			dwAccountID;
	DWORD			dwRoleID;
	tagRoleDataLoad	RoleData;
CMD_END;


CMD_START(NDBC_ResumeRole)
CMD_END;

CMD_START(NDBC_SaveRole) // ע�����Ϣ�ṹ��С�����¼���
	DWORD			dwRoleID;
	tagRoleDataSave	RoleData;
CMD_END;

//CMD_START(NDBC_RecordDel) // ɾ����ҵ�buff������item/equip��. -- ����ϢҪ������NDBC_SaveRole
//	DWORD			dwRoleID;
//	tagRecordDel	Record;
//CMD_END;

CMD_START(NDBC_ChangeRoleName)
	DWORD		dwAccountID;
	DWORD		dwRoleID;
	TCHAR		szNewRoleName[X_SHORT_NAME];
CMD_END;

CMD_START(NDBS_ChangeRoleName)
	DWORD		dwErrorCode;
	DWORD		dwAccountID;
	DWORD		dwRoleID;
	TCHAR		szNewRoleName[X_SHORT_NAME];
CMD_END;

CMD_START(NDBC_ChangeRoleGuild)
	DWORD		dwRoleID;
	DWORD		dwGuildID;
CMD_END;

CMD_START(NDBC_InsertFriend)
	DWORD			dwRoleID;
	tagFriendSave	FriendSave;
CMD_END;

//CMD_START(NDBS_InsertFriend)
//	DWORD		dwErrorCode;
//	DWORD		dwRoleID;
//	DWORD		dwFriendID;
//CMD_END;

CMD_START(NDBC_DeleteFriend)
	DWORD		dwRoleID;
	DWORD		dwFriendID;
CMD_END;

CMD_START(NDBC_UpdateFriend)
	DWORD			dwRoleID;
	tagFriendSave	FriendSave;
CMD_END;

CMD_START(NDBC_DeleteFriVal) // ע�⣺��LoongDB�����Ѻö������Ϣʱ��ӦdwRoleID < dwFriendID
	DWORD		dwRoleID;
	DWORD		dwFriendID;
CMD_END;

CMD_START(NDBC_DelAllFriRel)	// ɾ����ɫʱ��ɾ�����ݿ���������غ�����Ϣ
	DWORD		dwRoleID;
CMD_END;

CMD_START(NDBC_DelAllFriValRel) // ɾ����ɫʱ��ɾ�����ݿ���������غ��Ѷ���Ϣ
	DWORD		dwRoleID;
CMD_END;

CMD_START(NDBC_InsertFriVal)
	DWORD				dwRoleID;
	tagFriendshipSave	FriendshipSave;
CMD_END;

CMD_START(NDBC_UpdateFriVal)
	DWORD				dwRoleID;
	tagFriendshipSave	FriendshipSave;
CMD_END;

//CMD_START(NDBS_DeleteFriend)
//	DWORD		dwErrorCode;
//	DWORD		dwRoleID;
//	DWORD		dwFriendID;
//CMD_END;

CMD_START(NDBC_InsertBlack)	// ������
	DWORD		dwRoleID;
	DWORD		dwBlackID;
CMD_END;

CMD_START(NDBC_DeleteBlack)
	DWORD		dwRoleID;
	DWORD		dwBlackID;
CMD_END;

CMD_START(NDBC_InsertEnemy)
	DWORD		dwRoleID;
	DWORD		dwEnemyID;
CMD_END;

CMD_START(NDBS_InsertEnemy)
	DWORD		dwErrorCode;
	DWORD		dwRoleID;
	DWORD		dwEnemyID;
CMD_END;

CMD_START(NDBC_DeleteEnemy)
	DWORD		dwRoleID;
	DWORD		dwEnemyID;
CMD_END;

CMD_START(NDBS_DeleteEnemy)
	DWORD		dwErrorCode;
	DWORD		dwRoleID;
	DWORD		dwEnemyID;
CMD_END;

CMD_START(NDBC_LoadBaiBao)	// �ٱ������п�λʱ,�����ȡ����Ʒ��Ϣ
	DWORD		dwAccountID;
	INT32		nFreeSpaceSize;	// �ٱ����п�λ��Ŀ
	INT64		n64Serial;		// ��ȡָ��ID����Ʒ(��ָ����ΪGT_INVALID)
CMD_END;

CMD_START(NDBS_LoadBaiBao)
	DWORD		dwErrorCode;
	DWORD		dwAccountID;
	INT32		nRetNum;		// ��ȡ������Ʒ(װ��)����
	BYTE		byData[SPACE_BAIBAO_BAG * sizeof(tagEquip)];
CMD_END;

CMD_START(NDBC_LoadBaiBaoYuanBao)
	DWORD		dwAccountID;
CMD_END

CMD_START(NDBS_LoadBaiBaoYuanBao)
	DWORD		dwAccountID;
	INT32		nBaiBaoYuanBao;
CMD_END

CMD_START(NDBC_LoadBaiBaoLog)	// �����ȡ�ٱ�����ʷ��¼
	DWORD		dwAccountID;
CMD_END

CMD_START(NDBS_LoadBaiBaoLog)
	DWORD		dwErrorCode;
	DWORD		dwAccountID;
	INT32		nLogNum;		// ��ȡ���ļ�¼����
	BYTE		byData[1];		// Log��¼����
CMD_END

CMD_START(NDBC_AddBaiBaoLog)	// ����׷��һ����¼
	DWORD				dwAccountID;
	tagBaiBaoRecord		LogInfo;
CMD_END

CMD_START(NDBC_LoadItemNeedLog)
CMD_END;

CMD_START(NDBS_LoadItemNeedLog)
	DWORD		dwErrorCode;
	INT32		nItemNum;
	DWORD		dwTypeID[MAX_ITEM_NEEDLOG_NUM];
CMD_END;

CMD_START(NDBC_LoongWorldInitOK)		// loongworld��loongDB���͵ĳ�ʼ����ɵ�ȷ����Ϣ
CMD_END

CMD_START(NDBS_LoongWorldInitOK)
CMD_END

CMD_START(NDBC_ClearRoleDressMdID)
	DWORD		dwRoleID;
	WORD		wNewVal;
CMD_END

// ��ȡ��Ʒ&װ��(ѹ��������)
CMD_START(NDBC_TestLoadRoleItem)
	DWORD		dwAccountID;
	DWORD		dwRoleID;
CMD_END

CMD_START(NDBC_TestLoadRole)
	DWORD		dwAccountID;
	DWORD		dwRoleID;
CMD_END

//	��������������
CMD_START(NDBC_UpdateTreasureSum)
DWORD		dwRoleID;
INT			nSum;
CMD_END

CMD_START(NDBS_UpdateTreasureSum)
	DWORD		dwRoleID;
	INT			nSum;
CMD_END

// ���������Ϣ
CMD_START(NDBC_SaveRenameInfo)
	DWORD		dwAccountID;
	DWORD		dwRoleID;
	TCHAR		szName[X_SHORT_NAME];
CMD_END

// ������Ҿ���
CMD_START(NDBC_UpdateRoleAtt)
	DWORD		dwRoleID;
	INT			nType;
	INT			nValue;
CMD_END

// ����������Ե�
CMD_START(NDBC_UpdateRoleAttPoint)
	DWORD		dwRoleID;
	INT			nAttPointAdd[X_ERA_ATTA_NUM];
	INT			nAttPointLeft;
CMD_END

// ����������ʵ�
CMD_START(NDBC_UpdateRoleTalentPoint)
	DWORD		dwRoleID;
	tagRoleTalent	Talent;
	INT				nIndex;
	INT				nTalentPoint;
CMD_END

// ����������Ե�
CMD_START(NDBC_UpdateRoleItemTransport)
	DWORD		dwRoleID;
	DWORD		dwMapID;
	float		fX;
	float		fZ;
	float		fY;
CMD_END

#pragma pack(pop)
