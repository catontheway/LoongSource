//-----------------------------------------------------------------------------
//!\file msg_group.h
//!
//!\date 2008-11-20
//!
//!\brief �ͻ����������֮����ڶ������Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "group_define.h"

#pragma pack(push, 1)

// ������
enum
{
	E_Team_Not_Leader					=	1,		// ���뷽���Ƕӳ�
	E_Team_Target_Not_Exit				=	2,		// Ŀ�겻�Ϸ�
	E_Team_Target_Have_Team				=	3,		// ������������С����
	E_Team_Member_Full					=	4,		// С�ӳ�Ա����
	E_Team_Role_Not_In_Team				=	5,		// �߳�����Ҳ��ڱ�С��
	E_Team_Pick_Model_Not_Exit			=	6,		// �����õ�ʰȡģʽ������
	E_Team_Target_Busy					=	7,		// ����������æ
	E_Team_Target_Not_Online			=	8,		// ��������Ҳ�����
	E_Team_Map_CanNot_Invite			=	9,		// �õ�ͼ���޷�����������
	E_Team_Map_CanNot_Leave				=	10,		// �õ�ͼ���޷��뿪С��
	E_Team_Map_CanNor_ChangeLeader		=	11,		// �õ�ͼ���޷��ƽ��ӳ�
	E_Team_Map_CanNot_Kick				=	12,		// �õ�ͼ���޷�����
	E_Team_You_Have_Team				=   13,		// ���Ѿ��ڶ�����
	//Jason 2009-11-25
	E_Team_Apply_Invalid_Param			=	14,		// ������Ӳ�����Ч
	E_Team_Hack_Tools_Prohibited		=	15,		// ��ҽ�ֹ
	E_Team_Cannot_Find_Map				=	16,		// δ�ҵ���ͼ
	E_Team_Already_In_Team				=	17,		// �������Ѿ��ڸö���
	E_Team_Already_Have_Team			=	18,		// �������Ѿ���������
	E_Team_Target_No_Team				=	19,		// �������ˣ�û�ж�
	E_Team_Refuse						=	20,		// �ܾ�
	E_Team_Peer_Not_Online				=	21,		// �����뷽�����ߣ����ͻ����жϷ���Щ����ʹǰ���Ѿ���һ��E_Team_Target_Not_Online��
};


CMD_START(NC_InviteJoinTeam)		// �������
	DWORD		dwDestRoleID;		// ���������	
CMD_END;

CMD_START(NS_InviteToLeader)		// �������(���ظ�������)
	DWORD		dwErrorCode;
CMD_END;

CMD_START(NS_InviteJoinTeam)		// �������(���ظ���������)
	DWORD		dwLeaderID;
	DWORD		dwDestRoleID;
CMD_END;

CMD_START(NC_InviteReply)			// ����Ƿ�ͬ���������
	DWORD		dwLeaderID;
	BOOL		bAgree;				// �Ƿ�ͬ��
CMD_END;

CMD_START(NS_InviteReply)
	DWORD		dwTeamMemID[MAX_TEAM_NUM];
	BOOL		bAgree;
	DWORD		dwErrorCode;
CMD_END;

CMD_START(NC_KickMember)			// ����ҳ���
	DWORD		dwSrcRoleID;		// �ӳ�	
	DWORD		dwDestRoleID;		// �������
CMD_END;

CMD_START(NS_KickMember)
	DWORD		dwDestRoleID;
	DWORD		dwErrorCode;
CMD_END;

CMD_START(NC_LeaveTeam)				// �뿪����
CMD_END;
	
CMD_START(NS_LeaveTeam)				// ���������뿪�͵���
	DWORD		dwRoleID;
	DWORD		dwLeaderID;			// �ӳ�ID
	DWORD		dwErrorCode;
CMD_END;

CMD_START(NC_SetPickMode)			// ����С��ʰȡģʽ
	EPickMode	ePickMode;			// ʰȡģʽ
CMD_END;

CMD_START(NS_SetPickMode)
	EPickMode	ePickMode;
	DWORD		dwErrorCode;
CMD_END;

CMD_START(NC_ChangeLeader)
	DWORD		dwNewLeader;
CMD_END;

CMD_START(NS_ChangeLeader)
	DWORD		dwLeaderID;
	DWORD		dwNewLeaderID;
	DWORD		dwErrorCode;
CMD_END;

CMD_START(NS_RoleStateToTeam)
	DWORD			dwRoleID;		// ���ID
	DWORD			dwMapID;
	EClassTypeEx    eClassEx;		// ְҵ
	BYTE		    bySex;
	INT				nLevel;
	INT				nMaxHP;
	INT				nHP;
	INT				nMaxMP;
	INT				nMP;
	FLOAT			fX;
	FLOAT			fZ;
	DWORD			dwEquipTypeID[TEAM_DISPLAY_NUM];	
	tagAvatarAtt	AvatarAtt;
CMD_END;

CMD_START(NS_RolePosToTeam)		
	DWORD		dwRoleID;
	DWORD		dwMapID;
	FLOAT		fX;
	FLOAT		fZ;
CMD_END;

CMD_START(NS_GetTeamID)
	DWORD		dwTeamID;
	EPickMode	ePickMode;
CMD_END

// Jason 2009-11-25
// ����������Э��
CMD_START(NC_ApplyToJoinTeam)	// ������� �������߿ͻ��˷���world������
	//DWORD		dwApplicantID;	// ������id������ȥ����
	DWORD		dwTeamMemberID;	// �Ŷӳ�Աid
CMD_END
CMD_START(NS_ApplyToJoinTeam)	// ������ӣ���Э����worldת�����ӳ��ͻ���
	DWORD		dwApplicantID;	// ������id
	INT			nLevel;
	//����
	TCHAR	szName[X_SHORT_NAME];
CMD_END

CMD_START(NC_ApplyReply)			// �ӳ��Ƿ�ͬ��������룬�ӳ��ͻ��˷��͸�world������
	//DWORD		dwTeamLeaderID;		// �ӳ�id������ȥ��
	DWORD		dwApplicantID;		// ������id
	BOOL		bTeamLeaderAgree;	// �Ƿ�ͬ��
CMD_END

// Jason��ע�⣬���ӳ�ͬ��ʱ����������Ӧ��Э�飬�����ͻ��˾Ͳ����ٴ�������ɹ���Ϣ�ˡ�
// Ҳ����˵�����ӳ�ͬ��ʱ������world���������ص�Э����tagNS_InviteReply��
// �ӳ���ͬ��ʱ�����������Э��
CMD_START(NS_ApplyReplyRefuse)			// �ӳ��Ƿ�ͬ��������롣world�������������������߿ͻ���
	DWORD		dwTeamLeaderID;		// �ӳ�id
	//DWORD		dwApplicantID;		// ������id
	//BOOL		bTeamLeaderAgree;	// �Ƿ�ͬ��
	DWORD		dwErrCode;
	//����
	TCHAR	szName[X_SHORT_NAME];
CMD_END



#pragma pack(pop)