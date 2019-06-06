#pragma once


#include "..\WorldDefine\role_att.h"
#include "..\WorldDefine\remote_role.h"
#include "..\WorldDefine\msg_item.h"
#include "..\WorldDefine\msg_combat.h"
#include "..\WorldDefine\msg_role_title.h"
#include "PetEvent.h"

class Role;
class LocalPlayer;
class NetSession;
class ShadowPool;
struct tagNS_SetTransportMap;
struct tagLPRoleStateChangeEvent;

/** \class RoleMgr
	\brief ��ɫ������
*/
class RoleMgr
{
public:
	enum{MAX_VIEW_DIST=4};
public:
	RoleMgr(void);
	~RoleMgr(void);

	/** ������Ϸ����ʱ��ʼ��
	*/
	void Init(DWORD localPlayerID);
	/** �뿪��Ϸ����ʱ�ͷ���������
	*/
	void Destroy();
	/** ������Ϸ��ͼ
	*/
	void OnOpenMap(SceneGraph* pSG, const Vector3& localPlayerPos, const Vector3& localPlayerFaceto);
	/** �رյ�ͼʱ�ͷų����������������н�ɫ����
	*/
	void OnCloseMap(SceneGraph* pSG);
	/** ��Ӧ����/��ʾ��������ȼ�
	*/
	DWORD OnHideOtherPlayers( DWORD dwID, BOOL bDown );
	/** ������Ұ���뼶��
	*/
	void SetViewDistLevel( const int viewDistLevel );
	int  GetViewDistLevel() const { return m_viewDistLevel; }
	/** ���ݽ�ɫID���ҽ�ɫ
		\param bAddReqIfNotFind:�Ƿ�������������ɫ����
	*/
	Role* FindRole(DWORD roleID,bool bAddReqIfNotFind=false,bool bFindDeadCreatureList=false);
	/** ��ñ�����ҽ�ɫ
	*/
	LocalPlayer* GetLocalPlayer(){ return m_pLocalPlayer; }
	/** ��ȡ��ɫ����
	*/
	const map<DWORD,Role*>& GetRoleMap() { return m_roleMap; }
	/** ��ȡ�������ID
	*/
	DWORD GetLocalPlayerID();
	/** ÿ֡����
	*/
	void Update(SceneGraph* pSG);
	
	/** ��ɫʰȡ
	    \bIgnorePlayer �Ƿ�������
	*/
	DWORD PickRole(const Ray& mouseRay, const BOOL bIgnorePlayer);
	/** ��ʾ/������Ӱ
	*/
	void EnalbeShadow( const BOOL bEnalbe ) { m_bEnalbeShadow = bEnalbe; }	
	/** ��ɫ�Ƿ��ڽ�ɫ��Ұ��
	*/
	bool IsInViewDist( Role* pRole );

	/** ��������Ƿ�Զ��NPC/���
	*/
	bool IsOutValidDist(DWORD dwRoleID);
	bool IsOutFaceDist(DWORD dwRoleID);
	/** �ж�AABBox���Ƿ��н�ɫ
	*/
	bool IsAABBoxCollideRole(const AABBox &box, bool bOnlyNPC = true);
	/** ���¿ɽ����������������Ч
	*/
	void UpdataGameObjectQuestEffect();
	static RoleMgr* Inst();

private:
	/** ��ӵ���ɫ�����б�
	*/
	void AddReqRole(DWORD dwRoleID);
	/** ���ͽ�ɫ����
	*/
	void SendReqRoleList();
	/** �ͷ�һ����ɫ���������ͷ��������
	*/
	void FreeRole(DWORD roleID,SceneGraph* pSG);
	//---------------------------------------------------------------------
	//  ������Ϣ����                                                              
	//---------------------------------------------------------------------
	/** ɾ��Զ�����
	*/
	DWORD NetRemoveRemote(tagNS_RemoveRemote* pNetCmd, DWORD);
	/** ��ʼ�������������
	*/
	DWORD NetInitLocalPlayerAtt(tagNS_GetRoleInitState_Att* pNetCmd, DWORD);
	/** ��ȡԶ�������Ϣ
	*/
	DWORD NetGetRemoteRoleState(tagNS_GetRemoteRoleState* pNetCmd, DWORD);
	/** ��ȡ��Χ������Ϣ
	*/
	DWORD NetGetRemoteCreatureState(tagNS_GetRemoteCreatureState* pNetCmd, DWORD);
    /** ��ȡ��Χ������Ϣ
    */
    DWORD NetGetRemotePetState(tagNS_GetRemotePetState* pNetCmd, DWORD);
    /** ��ȡ��Χ������Ϣ
    */
    DWORD OnGetPetMount(tagEnterMount* pGameEvent);
    /** ɾ������
    */
    DWORD OnLeavePetMount(tagLeaveMount* pGameEvent);
	/** ������ҵ������Ա仯
	*/
	DWORD NetRoleAttChangeSingle(tagNS_RoleAttChangeSingle* pNetCmd, DWORD);
	/** ������Ҷ������Ա仯 
	*/
	DWORD NetRoleAttChangeMutiple(tagNS_RoleAttChangeMutiple* pNetCmd, DWORD);
	/** Զ�̽�ɫ�������Ա仯
	*/
	DWORD NetRemoteAttChangeSingle(tagNS_RemoteAttChangeSingle* pNetCmd, DWORD);
	/** Զ�̽�ɫ�������Ա仯
	*/
	DWORD NetRemoteAttChangeMutiple(tagNS_RemoteAttChangeMutiple* pNetCmd, DWORD);
	/** Զ�̽�ɫ������Ϣ�仯
	*/
	DWORD NetRemoteGuildInfoChange(tagNS_RemoteRoleGuildInfoChange *pNetCmd, DWORD);
	/** ���Avatar�仯
	*/
	DWORD NetAvatarEquipChange(tagNS_AvatarEquipChange* pNetCmd, DWORD);
    //! ��ҳƺű仯
    DWORD NetRoleTitleChange(tagNS_RoleTitleChangeBroadcast* pNetCmd, DWORD);
	/** ���Avatarȫ�����£�ʱװ/װ���л���
	*/
	DWORD NetAvatarEquip(tagNS_AvatarEquip* pNetCmd, DWORD);
	/** ����Ч���ı�
	*/
	DWORD NetEquipEffectChange(tagNS_EquipEffectChange* pNetCmd, DWORD);
	/** ��װ��Ч�ı�
	*/
	DWORD NetSuitEffectChange(tagNS_SuitEffect* pNetCmd, DWORD);
	/** ���ӽ�ɫBuff
	*/
	DWORD NetAddRoleBuff(tagNS_AddRoleBuff* pNetCmd, DWORD);
	/** �Ƴ���ɫBuff
	*/
	DWORD NetRemoveRoleBuff(tagNS_RemoveRoleBuff* pNetCmd, DWORD);
	/** ���½�ɫBuff
	*/
	DWORD NetUpdateRoleBuff(tagNS_UpdateRoleBuff* pNetCmd, DWORD);
	/** ���±����������Ͷ��
	*/
	DWORD NetRoleAttPointAddChange(tagNS_RoleAttPointAddChange* pNetCmd, DWORD);
	/** Ͷ�㷵��
	*/
	DWORD NetRoleBidAttPoint(tagNS_RoleBidAttPoint* pNetCmd, DWORD);
	/** ϴ�㷵��
	*/
	DWORD NetRoleClearAttPoint(tagNS_RoleClearAttPoint* pNetCmd, DWORD);
	/** ��ӽ�ɫ����״̬
	*/
	DWORD NetSetRoleStateEx(tagNS_SetRoleStateEx* pNetCmd, DWORD);
	/** ȡ����ɫ����״̬
	*/
	DWORD NetUnsetRoleStateEx(tagNS_UnSetRoleStateEx* pNetCmd, DWORD);
	/** ������ҵ���ʾ����
	*/
	DWORD NetRoleSetDisplay(tagNS_RoleSetDisplay* pNetCmd, DWORD);
	/** ���ñ�����ҵĸ�������
	*/
	DWORD NetSetPersonalSet(tagNS_SetPersonalSet* pNetCmd, DWORD);
	/** ���������ı�
	*/
	DWORD NetCreatureTaggedOwnerChange(tagNS_CreatureTaggedOwnerChange* pNetCmd, DWORD);
	/** ��ɫ��Ӱ����
	*/
    /** ��ɫ�ɼ�¼���ͷ��ı�
    */
    DWORD OnNS_SetTransportMap(tagNS_SetTransportMap* pNetCmd, DWORD);

	//���������ʾ����Ҫ��Ϊ������ָ��
	DWORD OnNS_RoleLevelChange( tagNS_RoleLevelChange* pNetCmd, DWORD );

	// ������ָı�
	DWORD OnNetRename( tagNS_LPRename *pNetCmd, DWORD );
	// ������ָı�,֪ͨ��Χ���
	DWORD NetRoleRenameBroadcast( tagNS_RoleRename *pNetCmd, DWORD );

    void ShowSetTransportErrorMsg(DWORD dwErrCode);

	VOID UpdataRoleShadow();
	/** ��ɫ�Ƿ���Ҫ����
	*/
	bool GetRoleHide( Role* pRole );
	/** ���¸�����ʾ
	*/
	void UpdataHightLight( Role* pRole );
	/** ������ȫ����ʾ
	*/
	DWORD OnLpChangeStateEvent( tagLPRoleStateChangeEvent *pEvent );
private:
	TSFPTrunk<RoleMgr>			m_trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
    TObjRef<GameFrameMgr>       m_pFrameMgr;
	ShadowPool*                 m_pShadowPool;
	BOOL                        m_bEnalbeShadow;

	LocalPlayer*				m_pLocalPlayer;		 // �������
	map<DWORD,Role*>			m_roleMap;			 // ��ɫmap
	list<Role*>					m_listDeadCreature;	 // ���������б�
	pair<DWORD, list<DWORD> >	m_paReqRole;		 // ��ɫ�����б�

	bool                        m_bHideOtherPlayers; // �Ƿ������������
	int							m_viewDistLevel;     // ��ɫ��Ұ����
	float                       m_fViewDist;         // ��ɫ��Ұ����
};
