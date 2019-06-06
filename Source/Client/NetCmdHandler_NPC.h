#pragma once
#include "..\WorldDefine\msg_combat.h"
#include "..\WorldDefine\action.h"
#include "..\WorldDefine\msg_loot.h"
#include "CombatEvent.h"



class FSM_NPC;
/** \class NetCmdHandler_NPC
	\brief NPC������Ϣ������
*/
class NetCmdHandler_NPC
{
public:
	NetCmdHandler_NPC(void);
	virtual ~NetCmdHandler_NPC(void);
	void SetFSM(FSM_NPC* pFSM){m_pFSM=pFSM;}

	virtual void OnNetCmd(tagNetCmd* pNetCmd) =0;
	virtual void OnGameEvent(tagGameEvent* pEvent) =0;
	virtual void Update() =0;
protected:
	FSM_NPC*				m_pFSM;
	TObjRef<Util>			m_pUtil;
	TObjRef<GameFrameMgr>	m_pGameFrameMgr;
};




/** \class NS_SyncStandHandler_NPC
	\brief վ��״̬ͬ���������
*/
class NS_SyncStandHandler_NPC : public NetCmdHandler_NPC
{
public:
	NS_SyncStandHandler_NPC();
	virtual ~NS_SyncStandHandler_NPC();

	//--NetCmdHandler_NPC
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();
};


/** \class NS_StopActionHandler_NPC
	\brief ֹͣ������Ϊ�������
*/
class NS_StopActionHandler_NPC : public NetCmdHandler_NPC
{
public:
	NS_StopActionHandler_NPC();
	virtual ~NS_StopActionHandler_NPC();

	//--NetCmdHandler_NPC
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();
};



/** \class NS_HPChangeHandler_NPC
	\brief ��ɫHP�仯�������
*/
class NS_HPChangeHandler_NPC : public NetCmdHandler_NPC
{
	struct tagData
	{
		DWORD recvTime;
		tagNS_RoleHPChange cmd;
	};
public:
	NS_HPChangeHandler_NPC();
	virtual ~NS_HPChangeHandler_NPC();

	//--NetCmdHandler_NPC
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();

private:
	void SendShowHPChangeEvent(tagNS_RoleHPChange* pCmd);
	void SendBeAttackEvent();
	void PlayCritSound(tagNS_RoleHPChange* pCmd);
	void PlayQuake();
	bool IfNeedCacheCmd(tagNS_RoleHPChange* pCmd);
	bool IfNeedClearCmd(tagNS_RoleHPChange* pCacheCmd,tagHitTargetEvent* pEvent);

private:
	list<tagData> m_cache;
};



/** \class NS_RoleDeadHandler_NPC
	\brief ��ɫ�����������
*/
class NS_RoleDeadHandler_NPC : public NetCmdHandler_NPC
{
	struct tagData
	{
		DWORD recvTime;
		tagNS_RoleDead cmd;
	};
public:
	NS_RoleDeadHandler_NPC();
	virtual ~NS_RoleDeadHandler_NPC();

	//--NetCmdHandler_NPC
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



/** \class NS_HitFlyHandler_NPC
	\brief �����������
*/
class NS_HitFlyHandler_NPC : public NetCmdHandler_NPC
{
public:
	NS_HitFlyHandler_NPC();
	virtual ~NS_HitFlyHandler_NPC();

	//--NetCmdHandler_NPC
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();

private:
	bool			m_bRecvedCmd;
	tagNS_HitFly	m_cmd;
	DWORD			m_recvTime;
};



/** \class NS_SpecialMove_NPC
	\brief �����ƶ��������
*/
class NS_SpecialMove_NPC : public NetCmdHandler_NPC
{
public:
	NS_SpecialMove_NPC();
	virtual ~NS_SpecialMove_NPC();

	//--NetCmdHandler_LP
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();
};



/** \class NS_AddRoleBuffHandler_NPC
	\brief ���Buff�������
*/
class NS_AddRoleBuffHandler_NPC : public NetCmdHandler_NPC
{
public:
	NS_AddRoleBuffHandler_NPC();
	virtual ~NS_AddRoleBuffHandler_NPC();

	//--NetCmdHandler_NPC
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();

private:
};







/** \class NS_SetStateHandler_NPC
	\brief ����״̬�������
*/
class NS_SetStateHandler_NPC : public NetCmdHandler_NPC
{
public:
	NS_SetStateHandler_NPC();
	virtual ~NS_SetStateHandler_NPC();

	//--NetCmdHandler_NPC
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();

private:
};




/** \class NS_HitTargetHandler_NPC
	\brief ����Ŀ���������
*/
class NS_HitTargetHandler_NPC : public NetCmdHandler_NPC
{
	struct tagData
	{
		DWORD recvTime;
		DWORD maxChannelIndex;
		tagNS_HitTarget cmd;
	};
public:
	NS_HitTargetHandler_NPC();
	virtual ~NS_HitTargetHandler_NPC();

	//--NetCmdHandler_NPC
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











/** \class NS_SyncGroundItemHandler_NPC
	\brief ��ɫ�����������
*/
class NS_SyncGroundItemHandler_NPC : public NetCmdHandler_NPC
{
	struct tagData
	{
		DWORD recvTime;
		tagNS_SyncGroundItem cmd;
	};
public:
	NS_SyncGroundItemHandler_NPC();
	virtual ~NS_SyncGroundItemHandler_NPC();

	//--NetCmdHandler_NPC
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();

private:
	list<tagData> m_cache;
};









/** \class NS_MonsterEnterCombatHandler_NPC
	\brief �������ս���������
*/
class NS_MonsterEnterCombatHandler_NPC : public NetCmdHandler_NPC
{
public:
	NS_MonsterEnterCombatHandler_NPC();
	virtual ~NS_MonsterEnterCombatHandler_NPC();

	//--NetCmdHandler_NPC
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();
};

/** \class NS_MonsterPlayActionHandler_NPC
\brief ���ﲥ��һ������������
*/
class NS_MonsterPlayActionHandler_NPC : public NetCmdHandler_NPC
{
public:
	NS_MonsterPlayActionHandler_NPC(){};
	virtual ~NS_MonsterPlayActionHandler_NPC(){};

	virtual void OnNetCmd(tagNetCmd *pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();
};

/** \class NS_MonsterSaHandler_NPC
\brief ����˵��
*/

class MonsterSayTable;

class NS_MonsterSaHandler_NPC : public NetCmdHandler_NPC
{
public:
	NS_MonsterSaHandler_NPC();
	virtual ~NS_MonsterSaHandler_NPC();
	
	virtual void OnNetCmd(tagNetCmd *pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void Update();
};

