#pragma once
#include "..\WorldDefine\msg_combat.h"
#include "..\WorldDefine\action.h"
#include "..\WorldDefine\msg_loot.h"
#include "..\WorldDefine\role_att.h"
#include "CombatEvent.h"

class FSM_LP;
class ItemFrame;

/** \class NetCmdHandler_LP
	\brief �������������Ϣ������
*/
class NetCmdHandler_LP
{
public:
	NetCmdHandler_LP(void);
	virtual ~NetCmdHandler_LP(void);
	void SetFSM(FSM_LP* pFSM){m_pFSM=pFSM;}

	virtual void OnNetCmd(tagNetCmd* pNetCmd) =0;
	virtual void OnGameEvent(tagGameEvent* pEvent) =0;
	virtual void Update() =0;
protected:
	FSM_LP*					m_pFSM;
	TObjRef<Util>			m_pUtil;
	TObjRef<GameFrameMgr>	m_pGameFrameMgr;
};



/** \class NS_MoveFailedHandler_LP
	\brief �ƶ�ʧ���������
*/
class NS_MoveFailedHandler_LP : public NetCmdHandler_LP
{
public:
	NS_MoveFailedHandler_LP();
	virtual ~NS_MoveFailedHandler_LP();

	//--NetCmdHandler_LP
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();
};



/** \class NS_StopActionHandler_LP
	\brief ֹͣ������Ϊ�������
*/
class NS_StopActionHandler_LP : public NetCmdHandler_LP
{
public:
	NS_StopActionHandler_LP();
	virtual ~NS_StopActionHandler_LP();

	//--NetCmdHandler_LP
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();
};



/** \class NS_HPChangeHandler_LP
	\brief ��ɫHP�仯�������
*/
class NS_HPChangeHandler_LP : public NetCmdHandler_LP
{
	struct tagData
	{
		DWORD recvTime;
		tagNS_RoleHPChange cmd;
	};

public:
	NS_HPChangeHandler_LP();
	virtual ~NS_HPChangeHandler_LP();

	//--NetCmdHandler_LP
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();

private:
	void SendShowHPChangeEvent(tagNS_RoleHPChange* pCmd);
	void SendBeAttackEvent(tagNS_RoleHPChange* pCmd);
	void SendSelectTargetEvent(tagNS_RoleHPChange* pCmd);
	void PlayBlockSound(tagNS_RoleHPChange* pCmd);
	void PlayQuake();
	bool IfNeedCacheCmd(tagNS_RoleHPChange* pCmd);
	bool IfNeedClearCmd(tagNS_RoleHPChange* pCacheCmd,tagHitTargetEvent* pEvent);
private:
	list<tagData> m_cache;
};

/** \class NS_RoleDeadHandler_LP
	\brief ��ɫ�����������
*/
class NS_RoleDeadHandler_LP : public NetCmdHandler_LP
{
	struct tagData
	{
		DWORD recvTime;
		tagNS_RoleDead cmd;
	};
public:
	NS_RoleDeadHandler_LP();
	virtual ~NS_RoleDeadHandler_LP();

	//--NetCmdHandler_LP
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();

private:
	void SendRewardEvent(tagNS_RoleDead* pCmd);
	void SendCostEvent(tagNS_RoleDead* pCmd);
	bool IfNeedCacheCmd(tagNS_RoleDead* pCmd);
	bool IfNeedClearCmd(tagNS_RoleDead* pCacheCmd,tagHitTargetEvent* pEvent);

private:
	list<tagData> m_cache;
};

/** \class NS_HitFlyHandler_LP
	\brief �����������
*/
class NS_HitFlyHandler_LP : public NetCmdHandler_LP
{
public:
	NS_HitFlyHandler_LP();
	virtual ~NS_HitFlyHandler_LP();

	//--NetCmdHandler_LP
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();

private:
	bool			m_bRecvedCmd;
	tagNS_HitFly	m_cmd;
	DWORD			m_recvTime;
};


/** \class NS_SpecialMove_LP
	\brief �����ƶ��������
*/
class NS_SpecialMove_LP : public NetCmdHandler_LP
{
public:
	NS_SpecialMove_LP();
	virtual ~NS_SpecialMove_LP();

	//--NetCmdHandler_LP
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();
};


/** \class NS_AddRoleBuffHandler_LP
	\brief ���Buff�������
*/
class NS_AddRoleBuffHandler_LP : public NetCmdHandler_LP
{
public:
	NS_AddRoleBuffHandler_LP();
	virtual ~NS_AddRoleBuffHandler_LP();

	//--NetCmdHandler_LP
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();

private:
};



/** \class NS_SetStateHandler_LP
	\brief ����״̬�������
*/
class NS_SetStateHandler_LP : public NetCmdHandler_LP
{
public:
	NS_SetStateHandler_LP();
	virtual ~NS_SetStateHandler_LP();

	//--NetCmdHandler_LP
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();

private:
};



/** \class NS_HitTargetHandler_LP
	\brief ����Ŀ���������
*/
class NS_HitTargetHandler_LP : public NetCmdHandler_LP
{
	struct tagData
	{
		DWORD recvTime;
		tagNS_HitTarget cmd;
	};
public:
	NS_HitTargetHandler_LP();
	virtual ~NS_HitTargetHandler_LP();

	//--NetCmdHandler_LP
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();

private:
	void PlayBeAttackEffect(tagNS_HitTarget* pCmd);
	void CacheCmd(tagNS_HitTarget* pCmd);
	bool IfNeedClearCmd(tagNS_HitTarget* pCacheCmd,tagHitTargetEvent* pEvent);
	bool IfNeedHandleCmd(tagNS_HitTarget* pCacheCmd,tagHitTargetEvent* pEvent);

private:
	list<tagData> m_cache;
};




/** \class NS_SyncGroundItemHandler_LP
	\brief ��ɫ�����������
*/
class NS_SyncGroundItemHandler_LP : public NetCmdHandler_LP
{
	struct tagData
	{
		DWORD recvTime;
		tagNS_SyncGroundItem cmd;
	};
public:
	NS_SyncGroundItemHandler_LP();
	virtual ~NS_SyncGroundItemHandler_LP();

	//--NetCmdHandler_LP
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();

private:
	list<tagData> m_cache;
};




/** \class NS_RoleExpChangeHandler_LP
	\brief ��ɫ����ֵ�仯��Ϣ������
*/
class NS_RoleExpChangeHandler_LP : public NetCmdHandler_LP
{
	struct tagData
	{
		DWORD recvTime;
		tagNS_RoleExpChange cmd;
	};
public:
	NS_RoleExpChangeHandler_LP();
	virtual ~NS_RoleExpChangeHandler_LP();

	//--NetCmdHandler_LP
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();

private:
	void SendShowExpChangeEvent(tagNS_RoleExpChange* pCmd);

private:
	list<tagData> m_cache;
};



/** \class NS_RoleLevelChangeHandler_LP
	\brief ��ɫ�ȼ��仯��Ϣ������
*/
class NS_RoleLevelChangeHandler_LP : public NetCmdHandler_LP
{
	struct tagData
	{
		DWORD recvTime;
		tagNS_RoleLevelChange cmd;
	};
public:
	NS_RoleLevelChangeHandler_LP();
	virtual ~NS_RoleLevelChangeHandler_LP();

	//--NetCmdHandler_LP
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();

private:
	void SendShowLevelChangeEvent(tagNS_RoleLevelChange* pCmd);

private:
	list<tagData> m_cache;
};

/** \class NS_CompleteFishHandler_LP
	\brief �һ�������Ϣ������
*/
class NS_CompleteFishHandler_LP : public NetCmdHandler_LP
{
public:
	NS_CompleteFishHandler_LP();
	virtual ~NS_CompleteFishHandler_LP();

	//--NetCmdHandler_LP
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();

private:
	ItemFrame*					m_pItemFrame;
};