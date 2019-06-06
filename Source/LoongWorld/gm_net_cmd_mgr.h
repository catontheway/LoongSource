//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: gm_net_cmd_mgr.h
// author: Sxg
// actor:
// data: 2008-11-05
// last:
// brief: GM �����
//-----------------------------------------------------------------------------
#pragma once

class Role;
//-----------------------------------------------------------------------------
class GMCommandMgr
{
	typedef DWORD (GMCommandMgr::*GMCMDHANDLE)(const std::vector<DWORD>&, Role*);

public:
	GMCommandMgr();
	~GMCommandMgr();

public:
	DWORD Excute(LPCTSTR szCommand, Role *pRole);
	VOID RegisterAll();
	VOID UnRegisterAll();

public:
	VOID LogGMCmd(DWORD dwRoleID, LPCTSTR szCmd, DWORD dwErrorCode);
//public:
//	VOID SetGMCmdID(DWORD dwCmdID) { if(GT_INVALID == m_dwGMCmdID) { m_dwGMCmdID = dwCmdID;} }

private:
	VOID Register(LPCTSTR szName, GMCMDHANDLE pFun, BYTE byPrivilege, LPCTSTR szDesc=NULL, BYTE byParamNum=1);

private: // GM �������
	// GM����
	DWORD HandleCreateItem(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleGetSilver(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleGetYuanBao(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleGetExVolume(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleClearBag(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleGoto(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleGotoRole(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleFillExp(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleFillLevel(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleEquipPotInc(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleAddQuest(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleClearSkillCD(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleCoolOff(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleCoolOn(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleChangeRoleAtt(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleSetReputation(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleRoleQuest(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleClearRoleQuest(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleAddBuff(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleSetVocation(const std::vector<DWORD>& vectParam, Role* pGM);

	// �����
	DWORD HandleMoveRole(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleKickRole(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleSelectRole(const std::vector<DWORD>& vectParam, Role* pGM);

	// �ű�
	DWORD HandleReloadScripts(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleChangeScriptData(const std::vector<DWORD>& vectParam, Role* pGM);

	// ��Ϸ����
	DWORD HandleDouble(const std::vector<DWORD>& vectParam, Role* pGM);

	// �����ɵ�½�������
	DWORD HandleResizeOnlineNum(const std::vector<DWORD>& vectParam, Role* pGM);

	// ��������������
	DWORD HandleClanData(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleVipNetBar(const std::vector<DWORD>& vectParam, Role* pGM);

	// ���Գ���
	DWORD HandlePet(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleAddSkill(const std::vector<DWORD>& vectParam, Role* pGM);

	// ���ɲ���
	DWORD HandleGetFund(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleGetMaterial(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleGetContribute(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleResetAffairTimes(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleGetTael(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleGuildStatus(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleGuildFacility(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleRoleGuild(const std::vector<DWORD>& vectParam, Role* pGM);

	// �����Ź�������
	DWORD HandleLaunchGP(const std::vector<DWORD>& vectParam, Role* pGM);	
	DWORD HandleRespondGP(const std::vector<DWORD>& vectParam, Role* pGM);	

	// ��������
	DWORD HandleVCard(const std::vector<DWORD>& vectParam, Role* pGM);

	// ���Գƺ�
	DWORD HandleTitle( const std::vector<DWORD>& vectParam, Role* pGM );

	// ����ҵ�ǰ����ˢ�����
	DWORD HandleCreateMonster( const std::vector<DWORD>& vectParam, Role* pGM);

	// �Ƿ���㼼�ܵĴ�����
	DWORD HandleTriggerOff(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleTriggerOn(const std::vector<DWORD>& vectParam, Role* pGM);

	// װ������
	DWORD HandleEngrave(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandlePosy(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleLoongSoul(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleAddAtt2Weapon(const std::vector<DWORD>& vectParam, Role* pGM);
	
	// ������������������
	DWORD HandleProficiency(const std::vector<DWORD>& vectParam, Role* pGM);

	// ����װ����װ
	DWORD HandleAddSuit(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleAddEquip(const std::vector<DWORD>& vectParam, Role* pGM);

	DWORD HandleLeftMsg(const std::vector<DWORD>& vectParam, Role* pGM);

	// ���Ա���ڵ�
	DWORD HandleChangeRoleChestSum(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleChangeServerChestSum(const std::vector<DWORD>& vectParam, Role* pGM);

	// ������
	DWORD HandleOpenCloseDoor(const std::vector<DWORD>& vectParam, Role* pGM);
	
	DWORD HandleLurk(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleInvincible(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleRolePosition(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleNoSpeak(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleCancelNoSpeak(const std::vector<DWORD>& vectParam, Role* pGM);
	DWORD HandleKillMonster(const std::vector<DWORD>& vectParam, Role* pGM);

	// ̯λ����
	DWORD HandleStallExp(const std::vector<DWORD>& vectParam, Role* pGM);

private:
	struct tagGMCommand
	{
		tstring		strCmd;		// ������
		tstring		strDesc;	// ����
		GMCMDHANDLE	handler;	// ����ָ��
		BYTE		byParamNum;	// ��������
		BYTE		byPrivilege;// GM����Ȩ��
		INT16		n16ExeTimes;// ִ�д���
	};

	TObjRef<Util>				m_pUtil;
	//DWORD						m_dwGMCmdID;

	TMap<DWORD, tagGMCommand*>	m_mapGMCommand;
};