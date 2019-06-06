//-----------------------------------------------------------------------------
//!\file msg_motion.h
//!
//!\date 2009-09-24
//!
//!\brief �ͻ����������֮����ڶ�������Ϣ
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push, 1)

enum 
{
	E_Motion_Success			= 0,
	E_Motion_NotValid			= 1,	// ������Ч
	E_Motion_CanotActive		= 2,	// �޷����н���
	E_Motion_FriendValNotEnough	= 3,	// �ѺöȲ�����
	E_Motion_DistanceNotEnough	= 4,	// ���벻��
	E_Motion_RoleNotFround		= 5,	// ����û�ҵ�
	E_Motion_DstRoleRefuse		= 6,	// Ŀ����Ҿܾ�
	E_Motion_WaitRemotePesponses= 7,	// ���ڵȴ�Ŀ����һ�Ӧ
	E_Motion_CannotStartWhenWait= 8,	// ���ڵȴ�Ŀ����һ�Ӧʱ�����ܿ�ʼ�µĽ���
	E_Motion_Wimming			= 9,	// ��Ӿʱ���ܿ�ʼ�µĽ���
};

/*******************************************************************************
�������̣�
	��֤�׶Σ�
		1.�ͻ��˷�NC_DuetMotionInvite���������
		2.�����������жϣ���ͨ������NS_DuetMotionOnInvite�����������
		3.���������ѡ���Ƿ���ܷ���NC_DuetMotionOnInvite��������
		4.����������NS_DuetMotionInvite��������ҽ��
	��ʼ������
		5.��������յ�NS_DuetMotionInvite���ܵ���������Ҵ�
		6.������������������NC_DuetMotionStart��ʼ˫�˶���
		7.�������㲥��˫�˶�����ʼ	
*******************************************************************************/

//------------------------------------------------------------------------------
// ������
//------------------------------------------------------------------------------
CONST INT MAX_MOTION_DIST	= 50 * 10;

//----------------------------------------------------------------------------
// �ͻ��˱��ݸ��Զ���
//----------------------------------------------------------------------------
CMD_START(NC_RoleStyleAction)
	DWORD			dwActionID;
CMD_END

CMD_START(NS_RoleStyleAction)
	DWORD			dwRoleID;
	DWORD			dwDstRoleID;
	DWORD			dwActionID;
CMD_END

//------------------------------------------------------------------------------
// ˫�˶��� �����������Ϣ
//------------------------------------------------------------------------------
CMD_START(NC_DuetMotionInvite)	
	DWORD	dwDstRoleID;	// Ŀ�����id
	DWORD	dwActionID;		// ����id
CMD_END

CMD_START(NS_DuetMotionInvite)	
	DWORD	dwErrCode;		// ������
CMD_END

//------------------------------------------------------------------------------
// ˫�˶��� ���������Ϣ
//------------------------------------------------------------------------------
CMD_START(NS_DuetMotionOnInvite)	
	DWORD	dwSrcRoleID;	// �������id
	DWORD	dwActionID;		// ����id
CMD_END

CMD_START(NC_DuetMotionOnInvite)
	DWORD	dwSrcRoleID;	// �������id
	DWORD	dwActionID;		// ����id
	BOOL	bAccpet;		// �Ƿ�ͬ��
CMD_END

//------------------------------------------------------------------------------
// ˫�˶��� ��ʼ����
//------------------------------------------------------------------------------
CMD_START(NC_DuetMotionStart)	
	DWORD	dwDstRoleID;	// Ŀ�����id
	DWORD	dwActionID;		// ����id
CMD_END


#pragma pack(pop)