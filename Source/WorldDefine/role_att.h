//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_att.h
// author: 
// actor:
// data: 2008-07-21
// last:
// brief: �������Ի�ȡ���仯����Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "RoleDefine.h"
#include "talent_define.h"
#include "skill_define.h"
#include "buff_define.h"
#include "QuestDef.h"
#include "suit_define.h"
#include "SocialDef.h"
#include "pet_define.h"
#include "guild_define.h"
#pragma pack(push, 1)

enum
{
	E_BidAttPoint_PointInvalid			=	1,		// Ͷ�㲻�Ϸ�
	E_BidAttPoint_NoEnoughAttPoint		=	2,		// û���㹻�����Ե�

	E_ClearAttPoint_ItemNotValid		=	10,		// ʹ�õ���Ʒ���Ϸ�
};

enum ERoleInitType
{
	ERIT_Null = -1,
	
	ERIT_Att				=	0,			// �����������
	ERIT_Skill				=	1,			// ���＼��
	ERIT_Item				=	2,			// ������Ʒ
	ERIT_ClientConfig		=	3,			// �ͻ�������
	ERIT_FrindAndEnemy		=	4,			// �����к��ѵ�ͬ����,�Լ�����Щ��У�������
	ERIT_CompleteQuest		=	5,			// �Ѿ���ɵ�����
	ERIT_IncompleteQuest	=	6,			// û����ɵ�����
	ERIT_Money				=	7,			// ��Ǯ
	ERIT_Reputation			=	8,			// ����
	ERIT_Guild				=	9,			// ����
	ERIT_End,
};

enum EClassErrorCode
{
    ECEC_Success            =   E_Success,

    ECEC_NotUpToLevel       =   1,          // ��ҵȼ�����
    ECEC_IsNotBaseClass     =   2,          // ֻ���������ܽ���ְҵר��
    ECEC_LackOfNewclassItem =   3,          // ȱ��ְҵר��ƾ֤����
    ECEC_LackOfReclassItem  =   4,          // ȱ�ٸ���ר��ְҵim����
	ECEC_IsNotBaseClassEx	=	5,			// �Ѿ�ְ��Ӣ��ְҵ
	ECEC_ClassNotMatch		=	6,			// Ӣ��ְҵ��ר��ְҵ��ƥ��
	ECEC_ClassNotSet		=	7,			// δ��ְר��ְҵ�����ɽ���ְҵת��
	ECEC_NPCError			=	8,			// NPC��Ч

    ECEC_End,
};

//-----------------------------------------------------------------------------
// ״̬�ı�
//-----------------------------------------------------------------------------
CMD_START(NS_SetState)
	DWORD		dwRoleID;
	EState		eState;
CMD_END

CMD_START(NS_UnSetState)
	DWORD		dwRoleID;
	EState		eState;
CMD_END

//-----------------------------------------------------------------------------
// ���״̬�ı�
//-----------------------------------------------------------------------------
CMD_START(NS_SetRoleState)
	DWORD		dwRoleID;						// ��ɫID
	ERoleState	eState;							// ״̬��־
CMD_END

CMD_START(NS_UnSetRoleState)
	DWORD		dwRoleID;						// ��ɫID
	ERoleState	eState;							// ״̬��־
CMD_END

CMD_START(NS_SetRoleStateEx)
	ERoleStateEx	eState;							// ״̬��־
CMD_END

CMD_START(NS_UnSetRoleStateEx)
	ERoleStateEx	eState;							// ״̬��־
CMD_END

//-----------------------------------------------------------------------------
// �����ʾ���øı�
//-----------------------------------------------------------------------------
CMD_START(NC_Fashion)		// ����ʱװģʽ
	bool			bFashion;	// �Ƿ�ʹ��ʹװģʽ
CMD_END

CMD_START(NS_AvatarEquip)	// ��ǰ���
	DWORD			dwRoleID;
	tagAvatarEquip	sAvatarEquip;
	bool			bFashion;	// �Ƿ�ʹ��ʹװģʽ
CMD_END

CMD_START(NC_RoleSetDisplay)	// ���ò�λ��ʾ����
	tagDisplaySet	sDisplaySet;
CMD_END

CMD_START(NS_RoleSetDisplay)
	DWORD			dwRoleID;
	tagDisplaySet	sDisplaySet;
CMD_END

//-----------------------------------------------------------------------------
// ��Ҹ������øı�
//-----------------------------------------------------------------------------
CMD_START(NC_SetPersonalSet)				// �޸ı�����ҵĸ�������
	tagDWPlayerPersonalSet	PersonalSet;
CMD_END;

CMD_START(NS_SetPersonalSet)				// ��������
	tagDWPlayerPersonalSet	PersonalSet;
CMD_END;

//-----------------------------------------------------------------------------
// ��ȡ������ҳ�ʼ�ṹ
//-----------------------------------------------------------------------------
CMD_START(NC_GetRoleInitState)
	ERoleInitType eType;					// �ͻ�����Ҫ�õ���������
CMD_END

//-----------------------------------------------------------------------------
// ���������ʼ����
//-----------------------------------------------------------------------------
CMD_START(NS_GetRoleInitState_Att)
	tagAvatarAtt	Avatar;							// ���
	tagAvatarEquip	AvatarEquip;					// װ�����
	tagDisplaySet	DisplaySet;						// �����ʾ����
	INT				nAtt[ERA_End];					// ��ǰ��������
	INT				nAttPointAdd[X_ERA_ATTA_NUM];	// ���Ͷ�ŵ�����һ�������е�ֵ
	INT				nLevel;							// �ȼ�
	INT				nCurLevelExp;					// ��ǰ��������
	INT				nCredit;						// ���ö�
	INT				nIdentity;						// ���
	INT				nVIPPoint;						// ��Ա����
	DWORD			dwState;						// ״̬��־λ
	DWORD			dwRoleState;					// ��Ҷ���״̬��־λ -- ��㲥����Χ���
	DWORD			dwRoleStateEx;					// ��Ҷ���״̬��־λ -- ֻ���Լ�֪��
	ERolePKState	ePKState;						// PK״̬
	DWORD			dwRebornMapID;					// �����ͼID
	Vector3			vRebornPoint;					// �����
	DWORD           dwTransportID;                  // ��ǰ��¼���ͷ��󶨵�ͼID
	Vector3         vTransportPoint;                // ��ǰ��¼���ͷ��󶨵�ͼ����
	DWORD			dwGuildID;						// ����id
	UINT16			u16ActTitleID;					// ��ǰ����ƺ�ID
	DWORD			dwLoverID;						// ���˶�Ӧ��roleid û�����ΪGT_INVALID
	BOOL			bHaveWedding;					// �Ƿ���й�����
	INT				nBuffNum;						// ״̬����
	BOOL			bHasLeftMsg;					// ������
    EClassType      eClassType;                     // ר��ְҵ
    EClassTypeEx    eClassTypeEx;                   // Ӣ��ְҵ(δ����)
	BYTE			byStallLevel;					// ̯λ�ȼ�
	tagBuffMsgInfo	Buff[1];						// ״̬�б�
CMD_END

//-----------------------------------------------------------------------------
// �������＼���б�
//-----------------------------------------------------------------------------
CMD_START(NS_GetRoleInitState_Skill)
	tagRoleTalent		Talent[X_MAX_TALENT_PER_ROLE];	// ����
	INT					nNum;							// ���ܸ���
	tagSkillMsgInfo		Skill[1];						// �����б�
CMD_END

//-----------------------------------------------------------------------------
// �Ѿ���ɵ����� ������
//-----------------------------------------------------------------------------
CMD_START(NS_GetRoleInitState_CompleteQuest)
	INT					nNum;						// �������
	tagCompleteQuestMsgInfo completeQuest[1];
CMD_END

//-----------------------------------------------------------------------------
// ��ǰ����
//-----------------------------------------------------------------------------
CMD_START(NS_GetRoleInitState_IncompleteQuest)
	INT					nNum;						// �������
	tagIncompleteQuestMsgInfo incompleteQuest[1];
CMD_END

//-----------------------------------------------------------------------------
// ������Ʒװ��
//-----------------------------------------------------------------------------
CMD_START(NS_GetRoleInitState_Item)
	INT16	n16SzBag;					// ������С
	INT16	n16SzRoleWare;				// �ֿ��С
	INT		nNum;						// ��Ʒװ������
	BYTE	byData[1];					// �����б�
CMD_END

//-----------------------------------------------------------------------------
// ��װ
//-----------------------------------------------------------------------------
CMD_START(NS_GetRoleInitState_Suit)
	INT		nSuitNum;
	BYTE	byData[1];					// tagSuitInit
CMD_END

//-----------------------------------------------------------------------------
// ������Ʒ��ȴʱ��
//-----------------------------------------------------------------------------
CMD_START(NS_GetRoleInitState_ItemCDTime)
	INT		nNum;						// ��ȴʱ����Ʒ(TypeID)����
	BYTE	byData[1];					// tagCDTime
CMD_END

//-----------------------------------------------------------------------------
// ���н�Ǯ
//-----------------------------------------------------------------------------
CMD_START(NS_GetRoleInitState_Money)
	INT32	nBagYuanBao;
	INT64	n64BagSilver;
	INT64	n64WareSilver;
	INT32	nBaiBaoYuanBao;
	INT32	nExchangeVolume;
	INT32	nCurClanCon[ECLT_NUM];
CMD_END

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
CMD_START(NS_GetRoleInitState_Reputation)
	INT		nReputation[ECLT_NUM];
    BYTE    nActiveCount[ECLT_NUM];	
    BYTE    bisFame[ECLT_NUM];
CMD_END


//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
CMD_START(NS_GetRoleInitState_Longhun)
	INT8	n8Num;
	INT16	n16EquipPos[1];				// ���������������װ��λ
CMD_END

//-----------------------------------------------------------------------------
// ���ڰ�����Ϣ��������Ϣ
//-----------------------------------------------------------------------------
CMD_START(NS_GetRoleInitState_Guild)
	tagGuildBase	sGuildBase;			// ���ɻ�������
	tagGuildMember	sGuildMember;		// ������Ϣ
CMD_END

//------------------------------------------------------------------------------
// ��ȡԶ����ҵ�ǰ����
//------------------------------------------------------------------------------
CMD_START(NC_GetRemoteRoleState)
	INT		nRoleNum;
	DWORD	dwRoleID[1];	// ������Զ�����Ҳ������Զ�̹��һ�η������50��
CMD_END

//-------------------------------------------------------------------------------------------
// �ͻ���Զ�����ͬ��������Ϣ�ṹ
//-------------------------------------------------------------------------------------------
struct tagRemoteRoleData
{
	DWORD				dwID;						// ���ID
	INT					nLevel;						// �ȼ�
	FLOAT				fPos[3];					// λ��
	FLOAT				fFaceTo[3];					// ����
	DWORD				dwState;					// ״̬��־λ
	DWORD				dwRoleState;				// ��ɫ����״̬��־λ
	ERolePKState		ePKState;					// PK״̬
	DWORD				dwGuildID;					// ����id(GT_INVALID��ʾû�м������)
	INT8				n8GuildPos;					// ����ְλ
	UINT16				u16CurActTitleID;			// ����ƺ�(GT_INVALID��ʾû�м���ƺ�)
	BYTE				byStallLevel;				// ̯λģ�͵ȼ�
	DWORD				dwMountPetID;				// ��˳���id
	DWORD				dwMountPetTypeID;			// ��˳���typeid
	tagDisplaySet		sDisplaySet;				// װ�������ʾ����
	tagAvatarAtt		Avatar;						// ���
	tagAvatarEquip		AvatarEquip;				// װ�����
	INT					nAtt[ERRA_End];				// Զ����һ�������
	tagRemoteOpenSet	sRemoteOpenSet;				// ��Զ����ҹ�����Ϣ����
    EClassType          eClassType;                 // ר��ְҵ
    EClassTypeEx        eClassTypeEx;               // Ӣ��ְҵ
    DWORD			    dwLoverID;					// ���˶�Ӧ��roleid û�����ΪGT_INVALID
    BOOL			    bHaveWedding;				// �Ƿ���й�����
	INT					nBuffNum;					// ״̬����
	tagBuffMsgInfo		Buff[1];					// ״̬�б�
};

CMD_START(NS_GetRemoteRoleState)
	tagRemoteRoleData		RoleData;		// Զ����ҽṹ�����ܵ�����Ҳ���ɲ������ṹ
CMD_END

//---------------------------------------------------------------------
// Զ���������Խṹ
//---------------------------------------------------------------------
struct tagRemoteCreatureData
{
	DWORD			dwID;						// ����ID
	DWORD			dwTypeID;					// ����ID
	INT				nLevel;						// �ȼ�
	DWORD			dwState;					// ״̬��־λ
	FLOAT			fPos[3];					// λ��
	FLOAT			fFaceTo[3];					// ����
	INT				nAtt[ERRA_End];				// Զ����һ�������
	DWORD			dwMapObjID;					// ��ͼ����ID	
	BOOL			bOpen;						// �ų�ʼ״̬
	DWORD			dwTaggedOwner;				// 
	INT				nBuffNum;					// ״̬����
	tagBuffMsgInfo	Buff[1];					// ״̬�б�
};

CMD_START(NS_GetRemoteCreatureState)
	tagRemoteCreatureData	CreatureData;		// Զ������ṹ
CMD_END

//---------------------------------------------------------------------
// Զ�̳������Խṹ
//---------------------------------------------------------------------
struct tagRemotePetData
{
    DWORD           dwRoleID;                   // ���������Ľ�ɫid��Ϊ-1ʱ����������
	DWORD			dwID;						// ����ID
	DWORD			dwProtoID;					// ԭ��ID
	UPetState		uState;						// ��̬
};

CMD_START(NS_GetRemotePetState)
	tagRemotePetData	PetData;
CMD_END

//------------------------------------------------------------------------------
// ����������Ըı�
//------------------------------------------------------------------------------
CMD_START(NS_RoleAttChangeSingle)
	ERoleAttribute	eType;
	INT				nValue;
CMD_END

struct tagRoleAttValue
{
	ERoleAttribute	eType;
	INT				nValue;
};

CMD_START(NS_RoleAttChangeMutiple)
	INT				nNum;
	tagRoleAttValue	value[1];
CMD_END

//-------------------------------------------------------------------------------
// Զ������Լ�Զ���������Ըı�
//------------------------------------------------------------------------------
CMD_START(NS_RemoteAttChangeSingle)
	DWORD			dwRoleID;
	ERemoteRoleAtt	eType;
	INT				nValue;
CMD_END

struct tagRemoteAttValue
{
	ERemoteRoleAtt	eType;
	INT				nValue;
};

CMD_START(NS_RemoteAttChangeMutiple)
	DWORD				dwRoleID;
	INT					nNum;
	tagRemoteAttValue	value[1];
CMD_END

//--------------------------------------------------------------------------------
// ��Ҿ���ı�
//--------------------------------------------------------------------------------
CMD_START(NS_RoleExpChange)
	INT		nExp;				// ��ǰ
	INT		nChange;			// �仯����
	BOOL	bKill;				// �Ƿ�ͨ����ɱ������
CMD_END

//--------------------------------------------------------------------------------
// �������
//--------------------------------------------------------------------------------
CMD_START(NS_RoleReputeChange)
	BYTE	byClanType;			// ����
	INT		nRepute;			// ��ǰ
	INT		nChange;			// �仯����
CMD_END

//-----------------------------------------------------------------------------
// ����״̬�ı�
//-----------------------------------------------------------------------------
CMD_START(NS_BecomeFame)
    BYTE    eClanType;
CMD_END

//-----------------------------------------------------------------------------
// Զ����Ұ�����Ϣ�仯
//-----------------------------------------------------------------------------
CMD_START(NS_RemoteRoleGuildInfoChange)
	DWORD	dwRoleID;
	DWORD	dwGuildID;					// ����id(GT_INVALID��ʾû�м������)
	INT8	n8GuildPos;					// ����ְλ
CMD_END

//-----------------------------------------------------------------------------
// �����䱦��������ı�
//-----------------------------------------------------------------------------
CMD_START(NS_TreasureActCountChange)
    BYTE    eClanType;
    BYTE    nActCount;
CMD_END

//-----------------------------------------------------------------------------
// ���幱�״ﵽ����
//-----------------------------------------------------------------------------
CMD_START(NS_ClanConUpperLimit)
	BYTE    eClanType;
CMD_END

//--------------------------------------------------------------------------------
// ��ҵȼ��ı�
//--------------------------------------------------------------------------------
CMD_START(NS_RoleLevelChange)
	DWORD	dwRoleID;
	INT		nLevel;
	BOOL	bKill;				// �Ƿ���Ϊ��ɱ��������仯
CMD_END

//--------------------------------------------------------------------------------
// �������Ͷ��
//--------------------------------------------------------------------------------
CMD_START(NC_RoleBidAttPoint)
	INT	nAttPointAdd[X_ERA_ATTA_NUM];
CMD_END

CMD_START(NS_RoleBidAttPoint)
	DWORD dwErrorCode;	// ������
CMD_END

//---------------------------------------------------------------------------------
// �������ϴ��
//---------------------------------------------------------------------------------
CMD_START(NC_RoleClearAttPoint)
	INT64 n64ItemID;				// ϴ�����
CMD_END

CMD_START(NS_RoleClearAttPoint)
	DWORD dwErrorCode;				// ������
CMD_END

//---------------------------------------------------------------------------------
// ��Ҹ���һ������Ͷ��ֵ�ı�
//---------------------------------------------------------------------------------
CMD_START(NS_RoleAttPointAddChange)
	INT	nAttPointAdd[X_ERA_ATTA_NUM];
CMD_END

//---------------------------------------------------------------------------------
// ��Һ��Ѻͺ�����
//---------------------------------------------------------------------------------
CMD_START(NS_SendFriendBlackList)
	DWORD			dwRolesID[MAX_BLACKLIST];
	INT				nNum;		  // ��������
	tagFriendInfo	FriendInfo[1];
CMD_END;

//---------------------------------------------------------------------------------
// ��/����
//---------------------------------------------------------------------------------
CMD_START(NS_OpenDoor)
	DWORD			dwRoleID;
CMD_END

CMD_START(NS_CloseDoor)
	DWORD			dwRoleID;
CMD_END

//---------------------------------------------------------------------------------
// ���������ı�
//---------------------------------------------------------------------------------
CMD_START(NS_CreatureTaggedOwnerChange)
	DWORD			dwCreatureID;
	DWORD			dwTaggedOwner;
CMD_END

//---------------------------------------------------------------------------------
// NPC�Ի�
//---------------------------------------------------------------------------------
CMD_START(NC_NPCTalk)
	DWORD			dwNPCId;
CMD_END

//----------------------------------------------------------------------------------
// �����������Է���װ��
//----------------------------------------------------------------------------------
CMD_START(NC_GetRemoteRoleEquipInfo)
	DWORD			dwRoleID;
CMD_END

CMD_START(NS_GetRemoteRoleEquipInfo)
	DWORD			dwRoleID;					
	INT				nEquipNum;					//װ������
	BYTE			byEquip[1];					//װ��
CMD_END

//----------------------------------------------------------------------------------
// �����������ְҵר��
//----------------------------------------------------------------------------------
CMD_START(NC_SetClass)
    DWORD           dwNpcId;    // npc id
    EClassType      eClass;
CMD_END

CMD_START(NS_SetClass)
    EClassErrorCode dwErrCode;  // E_SUCCESS when Okey
CMD_END

//----------------------------------------------------------------------------------
// �����������ר��ְҵ
//----------------------------------------------------------------------------------
CMD_START(NC_ChangeClass)
    DWORD           dwNpcId;    // npc id
    EClassType      eClass;
CMD_END

CMD_START(NS_ChangeClass)
    EClassErrorCode dwErrCode;  // E_SUCCESS when Okey
CMD_END

//-----------------------------------------------------------------------------
// ��ɫר��ְҵ�����ı䣬����Χ��ҹ㲥
//-----------------------------------------------------------------------------
CMD_START(NS_RoleClassChangeBroadcast)
    DWORD			dwRoleID;
    EClassType	    eClass;
CMD_END;

//------------------------------------------------------------------------------
// ��ɫ����
//------------------------------------------------------------------------------
CMD_START(NC_LPRename)
	INT64 n64RenameItemID;		// ������
	TCHAR szNewName[X_SHORT_NAME];			// ���������
CMD_END

CMD_START(NS_LPRename)
	DWORD dwRoleID;				// ��ɫID
	DWORD dwErrorCode;
	TCHAR szNewName[X_SHORT_NAME];			// ���������
	TCHAR szOldName[X_SHORT_NAME];			// ��Ҿ�����
CMD_END

//-----------------------------------------------------------------------------
// ������ָı䣬����Χ��ҹ㲥
//-----------------------------------------------------------------------------
CMD_START(NS_RoleRename)
	DWORD dwRoleID;				// ���ID
	TCHAR szNewName[1];			// ���������
CMD_END
	
#pragma pack(pop)
