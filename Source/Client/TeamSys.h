#pragma once

#include "..\WorldDefine\group_define.h"

struct tagNS_InviteToLeader;
struct tagNS_InviteJoinTeam;
struct tagNS_ApplyToJoinTeam;
struct tagNS_ApplyReplyRefuse;
struct tagNS_InviteReply;
struct tagNS_KickMember;
struct tagNS_LeaveTeam;
struct tagNS_SetPickMode;
struct tagNS_ChangeLeader;
struct tagRoleGetNameEvent;
struct tagNS_RoleStateToTeam;
struct tagNS_RolePosToTeam;
struct tagNS_RoleLevelChange;
struct tagNS_RemoteAttChangeSingle;
struct tagNS_RemoteAttChangeMutiple;
struct tagNS_GetTeamID;
struct tagInviteJoinTeamEvent;
struct tagApplyJoinTeamEvent;
struct tagTeammateData
{
	DWORD			dwRoleID;
	tstring			strName;
	DWORD			dwMapID;
	EClassTypeEx	eClassEx;
	INT				nLevel;
	INT				nMaxHP;
	INT				nHP;
	INT				nMaxMP;
	INT				nMP;
	FLOAT			fX;
	FLOAT			fZ;
	DWORD			dwEquipTypeID[TEAM_DISPLAY_NUM];
	tagAvatarAtt	AvatarAtt;
	tagTeammateData() : dwRoleID( GT_INVALID ) {}
};

/** \class TeamSys
	\brief ���ϵͳ������ģʽ�������ⲿ���ò�ѯ�ӿڣ�
*/
class TeamSys
{
public:
	TeamSys(void);
	~TeamSys(void);
	static TeamSys* Inst();

	BOOL Init(void);
	VOID Destroy(void);
	VOID Updata();

	/** \��������Ƿ��ڶ�����
	*/
	bool IsInTeam() const { return m_vecTeammates.size() > 0; }
	/** \��ö�������
	*/
	int GetTeammateNum() const { return m_vecTeammates.size(); }
	/** \�����Ƿ�����
	*/
	bool IsTeamFull() { return m_vecTeammates.size() == MAX_TEAM_NUM - 1; }
	/** \��ö����б�
	*/
	const vector<tagTeammateData>& GetTeammateList() { return m_vecTeammates; }
	/** \�Ƿ�ӳ�
	*/
	bool IsLeader( const DWORD dwRoleID ) const { ASSERT(P_VALID(dwRoleID)); return dwRoleID == m_dwLeaderRoleID; }
	/** \�Ƿ���ѣ������ӳ���
	*/
	bool IsTeammate( const DWORD dwRoleID );
	/** \��õ�ǰʰȡģʽ
	*/
	EPickMode GetPickMode() const { return m_ePickMode; }
	/** \����ʰȡģʽ
	*/
	void SetPickMode( const EPickMode ePickMode );

	/**����������ģʽ
	*/
	ERefuseApplyMode GetRefuseApplyMode(){ return m_eRefuseApplyMode; }
	/**�����������ģʽ
	*/
	void SetRefuseApplyMode( ERefuseApplyMode mode ){ m_eRefuseApplyMode = mode; }

	/** \�Ƴ���Ա
	*/
	void KickTeammate( const DWORD dwTeammateID );
	/** \�ƽ��ӳ�
	*/
	void HandOutLeader( const DWORD dwNewLeaderID );
	/** \��ȡ������Ϣ
	*/
	tstring GetTeammateNameByID( const DWORD dwRoleID ) const;
	DWORD GetTeammateIDByName( const TCHAR* szName ) const;
	/** \�뿪����
	*/
	void LeaveTeam();
	/** \�������
	*/
	void InviteJoinTeam( const DWORD dwRoleID );
	
	/**\����������
	   \param dwRoleID Ŀ�����ID, �������Ҫ����Ķ�����
	*/
	void ApplyJoinTeam( const DWORD dwRoleID );
	
	/** \��ȡ��������
	*/
	const tagTeammateData* GetTeammateData( const DWORD dwRoleID );
	/** \��ȡ����ID
	*/
	DWORD GetTeamID() const;
private:
	VOID  HandleNSErrorCode( const DWORD dwErrorCode );
	VOID  ShowInviteMsgBox( const TCHAR* szInviteName );
	VOID  ShowLeaveTeamMsgBox();
	VOID  ShowKickMemberMsgBox( const TCHAR* szDestName );
	VOID  AddChatMsg( const TCHAR* szMsg );
	VOID  ShowJoinTeamMsg( const TCHAR* szPlayerName );
	VOID  ShowLeaveTeamMsg( const TCHAR* szPlayerName );
	/** \�����ɢ
	*/
	VOID  OnBreakTeam( const bool bSendEvent = true );
	/** \������Ϣ��Ӧ
	*/
	DWORD OnNS_InviteToLeader( tagNS_InviteToLeader* pCmd, DWORD );
	DWORD OnNS_InviteJoinTeam( tagNS_InviteJoinTeam* pCmd, DWORD );
	DWORD OnNS_InviteReply( tagNS_InviteReply* pCmd, DWORD );
	DWORD OnNS_KickMember( tagNS_KickMember* pCmd, DWORD );
	DWORD OnNS_LeaveTeam( tagNS_LeaveTeam* pCmd, DWORD );
	DWORD OnNS_SetPickMode( tagNS_SetPickMode* pCmd, DWORD );
	DWORD OnNS_ChangeLeader( tagNS_ChangeLeader* pCmd, DWORD );
	DWORD OnNS_RoleStateToTeam( tagNS_RoleStateToTeam* pCmd, DWORD );
	DWORD OnNS_RolePosToTeam( tagNS_RolePosToTeam* pCmd, DWORD );
	DWORD OnNS_RoleLevelChange( tagNS_RoleLevelChange* pCmd, DWORD );
	DWORD OnNS_RemoteAttChangeSingle( tagNS_RemoteAttChangeSingle* pCmd, DWORD );
	DWORD OnNS_RemoteAttChangeMutiple( tagNS_RemoteAttChangeMutiple* pCmd, DWORD );
	DWORD OnNS_GetTeamID( tagNS_GetTeamID* pCmd, DWORD );

	/**����������ҵ��������
	*/
	DWORD OnNS_RPWantToJoinTeam( tagNS_ApplyToJoinTeam* pCmd, DWORD );
	/**�ӳ��ܾ�������������
	*/
	DWORD OnNS_ApplyReplyRefuse( tagNS_ApplyReplyRefuse* pCmd, DWORD );

	/** \��Ϸ�¼���Ӧ
	*/
	DWORD  OnRoleGetNameEvent(tagRoleGetNameEvent* pEvent);
	DWORD  OnInviteMsgBox( tagMsgBoxEvent* pEvent );
	DWORD  OnLeaveTeamMsgBox( tagMsgBoxEvent* pEvent );
	DWORD  OnKickMemberMsgBox( tagMsgBoxEvent* pEvent );
	DWORD  OnInviteJoinTeamEvent(tagInviteJoinTeamEvent* pEvent);

	/** \������������˵Ķ���
	*/
	DWORD  OnApplyJoinTeamEvent( tagApplyJoinTeamEvent* pEvent );
	/**Զ��������������飬�ӳ�����һ����ʾ��
	*/
	DWORD OnApplyToJoinTeamMsgBox( tagMsgBoxEvent* pEvent );

private:
	TSFPTrunk<TeamSys>			m_trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GameFrameMgr>       m_pGameFrameMgr;

	DWORD						m_dwTeamID;                 // ����ID
	vector<tagTeammateData>		m_vecTeammates;				// ����RoleID�б�
	DWORD                       m_dwLeaderRoleID;			// ���Ᵽ��һ���ӳ���RoleID
	EPickMode                   m_ePickMode;				// ��ǰ��ʰȡģʽ
	DWORD                       m_dwInviteRoleID;			// �����ߵ�ID�����ڻ�ȡ���������֣�
	DWORD                       m_dwKickingMemberID;        // ���������Ķ�Ա�����ڵȴ�ȷ�ϣ�
	BOOL                        m_bWaitForInviteRoleName;
	
	DWORD                       m_dwInviteMsgBoxShowTime;   // ������Ի����ʱ��
	DWORD						m_dwWantToJoinTeamTime;		// Զ������������Ի���ʱ�䵹��ʱ15��

	ERefuseApplyMode			m_eRefuseApplyMode;			// �������ģʽ
	DWORD						m_dwApplyTeamRPId;			// ��ǰ���ڴ���ģ���������������ID
	//TMap<DWORD,INT>				m_mapApplyRP;				// Զ��������������飬�ӳ�����ͬ����߾ܾ�
	TMap<DWORD,tagNS_ApplyToJoinTeam> m_mapApplyRP;
};
