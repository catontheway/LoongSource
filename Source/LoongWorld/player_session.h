//-----------------------------------------------------------------------------
//!\file player_session.h
//!\author Aslan
//!
//!\date 2008-06-10
//! last 2008-06-10
//!
//!\brief ������Ӵ�����
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "player_net_cmd_mgr.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/role_data_define.h"
#include "Mutex.h"
#include "gm_net_cmd_mgr.h"
#include "fatigue_guarder.h"

class Creature;

const INT CON_LOST	=	-1000000;		// ���ӶϿ���־
const INT RET_TRANS	=	-2000000;		// ���ϲ㴦��

class PlayerSession
{
public:
	friend class DBSession;
	friend class WorldNetCmdMgr;

	PlayerSession(DWORD dwSessionID, DWORD dwInternalIndex, DWORD dwIP, BYTE byPrivilege,
				BOOL bGuard, DWORD dwAccOLSec, LPCSTR tszAccount,tagDWORDTime dwPreLoginTime,
				DWORD dwPreLoginIP);
	~PlayerSession();

	//-----------------------------------------------------------------------------
	// ����Get
	//-----------------------------------------------------------------------------

	tagDWORDTime GetPreLoginTime()	const	{ return m_dwPreLoginTime;}
	DWORD		 GetPreLoginIP()	const	{ return m_dwPreLoginIP;}
	DWORD		 GetCurLoginIP()	const	{ return m_dwIP;}

	LPCSTR		GetAccount()		const	{ return m_szAccount;	}
	DWORD		GetGetIP()			const	{ return m_dwIP; }
	DWORD		GetSessionID()		const	{ return m_dwAccountID; }
	INT			GetMsgWaitNum()		const	{ return m_nMsgNum; }
	DWORD		GetInternalIndex()	const	{ return m_dwInternalIndex; }
	Role*		GetRole()			const	{ return m_pRole; }
	Role*		GetOtherInMap( DWORD dwRoleID ) const ;
	const FatigueGuarder&	GetFatigueGuarder()	const { return m_FatigueGarder; }
	INT			GetVNBExpRate()		const;
	INT			GetVNBLootRate()		const;
	LPCTSTR		GetVNBName()		const;
	BOOL		IsKicked() const			{ return m_bKicked; }

	// �ʺ��½�ɫͨ��������ز���
	const INT	GetBaiBaoYB()		const	{ return m_sAccountCommon.nBaiBaoYuanBao; }
	const INT64	GetWareSilver()		const	{ return m_sAccountCommon.n64WareSilver; }
	const INT16	GetWareSize()		const	{ return m_sAccountCommon.n16WareSize; }

	//----------------------------------------------------------------------------
	// ����Set
	//----------------------------------------------------------------------------
	VOID		SetRole(Role* pRole)		{ m_pRole = pRole; }
	VOID		SetConnectionLost()			{ InterlockedExchange((LPLONG)(&m_bConnectionLost), TRUE); }
	VOID		SetKicked()					{ InterlockedExchange((LPLONG)(&m_bKicked), TRUE); }

	VOID		SetBaiBaoYB(INT nYuanBao)		{ m_sAccountCommon.nBaiBaoYuanBao	= nYuanBao; }
	VOID		SetWareSilver(INT64 n64Silver)	{ m_sAccountCommon.n64WareSilver	= n64Silver; }
	VOID		SetAccOLMin(DWORD dwState, DWORD dwAccOLMin)	{ m_FatigueGarder.SetAccOLTimeMin(dwState, dwAccOLMin);				}
	VOID		SetRoleLoading()				{ m_bRoleLoading = TRUE;	}
	VOID		SessionLogout();

	//----------------------------------------------------------------------------
	// ��Ϣ�������
	//----------------------------------------------------------------------------
	INT			HandleMessage();
	VOID		SendMessage(LPVOID pMsg, DWORD dwSize);
	VOID		BroadcastCurrChannel(LPCTSTR szMsg);

	//-----------------------------------------------------------------------------
	// ѡ�˽�����ж�
	//-----------------------------------------------------------------------------
	bool		IsRoleLoading()		const	{ return m_bRoleLoading; }
	bool		IsRoleEnuming()		const	{ return m_bRoleEnuming; }
	bool		IsRoleCreating()	const	{ return m_bRoleCreating; }
	bool		IsRoleDeleting()	const	{ return m_bRoleDeleting; }
	bool		IsInWorld()			const	{ return m_bRoleInWorld; }


	//-----------------------------------------------------------------------------
	// ��ɫͨ���������
	//-----------------------------------------------------------------------------
	bool		IsHaveBagPsd()		const	{ return GetBagPsd() != GT_INVALID; }

	//-----------------------------------------------------------------------------
	// ��ɫ���
	//-----------------------------------------------------------------------------
	BOOL		FullLogin(Role* pRole, BOOL bFirst);
	VOID		LogoutPlayer();
	VOID		Refresh();

	//-----------------------------------------------------------------------------
	// ����������غ�GM����
	//----------------------------------------------------------------------------
	static VOID RegisterAllPlayerCmd();
	static VOID RegisterALLSendCmd();
	static VOID UnRegisterALL();

	
	//-----------------------------------------------------------------------------
	// GM�������
	//-----------------------------------------------------------------------------
	BOOL		IsPrivilegeEnough(BYTE byPrivilege) const { return byPrivilege <= m_byPrivilege; }

	//-----------------------------------------------------------------------
	// ������ϢMsg(�����������ã��������ͷ�)
	//-----------------------------------------------------------------------
	VOID		RecycleMsg(LPBYTE pMsg);

	//-----------------------------------------------------------------------------
	// ����
	//----------------------------------------------------------------------------
	INT			Update();

private:
	//-----------------------------------------------------------------------
	// ��Ҫ�����е�ͼ�߳��ϲ㴦�����Ϣע��
	//-----------------------------------------------------------------------
	static VOID	RegisterWorldMsg(LPCSTR szCmd, NETMSGHANDLER fp, LPCTSTR szDesc, DWORD dwSize);

	//-----------------------------------------------------------------------
	// ��Ϣ�������
	//-----------------------------------------------------------------------
	VOID			SendSmallMessage(LPVOID pMsg, DWORD dwSize);
	VOID			SendLargeMessage(LPVOID pMsg, DWORD dwSize);

	//-----------------------------------------------------------------------
	// �ײ�����
	//-----------------------------------------------------------------------
	LPBYTE			RecvMsg(DWORD& dwSize);
	VOID			ReturnMsg(LPBYTE pMsg);
	VOID			SendMsg(LPBYTE pMsg, DWORD dwSize);

	//-----------------------------------------------------------------------
	// ѡ�˽������
	//-----------------------------------------------------------------------
	BOOL			IsRoleExist(const DWORD dwRoleID) const;
	BOOL			AddRole(const DWORD dwRoleID);
	BOOL			RemoveRole(const DWORD dwRoleID);
	BOOL			CanSetSafeCode();
	BOOL			CanResetSafeCode() const;
	BOOL			CanCancelSafeCodeReset() const;

	//-----------------------------------------------------------------------
	// �ʺ��½�ɫͨ��������ز���
	//-----------------------------------------------------------------------
	const DWORD		GetBagPsd()		const { return m_sAccountCommon.dwBagPsdCrc; }
	const DWORD		GetSafeCode()	const { return m_sAccountCommon.sSafeCode.dwSafeCodeCrc; }

	VOID			SetBagPsd(DWORD dwNewPsdCrc);

	/************************************************************************
	** Handlers -- map thread
	*************************************************************************/

	//-----------------------------------------------------------------------
	// ������Ϸ
	//-----------------------------------------------------------------------
	DWORD	HandleJoinGame(tagNetCmd* pCmd);

	//-----------------------------------------------------------------------
	// ��ɫ������ɾ������ȡ��ѡ��
	//-----------------------------------------------------------------------
	DWORD	HandleRoleCreate(tagNetCmd* pCmd);
	DWORD	HandleRoleEnum(tagNetCmd* pCmd);
	DWORD	HandleRoleDelete(tagNetCmd* pCmd);
	DWORD	HandleRoleSelect(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��ȫ�봦��
	//------------------------------------------------------------------------
	DWORD	HandleRoleSetSafeCode(tagNetCmd* pCmd);
	DWORD	HandleRoleResetSafeCode(tagNetCmd* pCmd);
	DWORD	HandleRoleCancelSafeCodeReset(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �������Ի�ȡ
	//------------------------------------------------------------------------
	DWORD	HandleGetRoleInitAtt(tagNetCmd* pCmd);
	DWORD	HandleGetRemoteUnitAtt(tagNetCmd* pCmd);
	DWORD	HandleChangeRoleName(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------
	DWORD	HandleRoleWalk(tagNetCmd* pCmd);
	DWORD	HandleRoleStopWalk(tagNetCmd* pCmd);
	DWORD	HandleRoleJump(tagNetCmd* pCmd);
	DWORD	HandleRoleDrop(tagNetCmd* pCmd);
	DWORD	HandleRoleVDrop(tagNetCmd* pCmd);
	DWORD	HandleRoleSlide(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// װ�����
	//------------------------------------------------------------------------
	DWORD	HandleRoleEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleUnequip(tagNetCmd* pCmd);
	DWORD	HandleRoleSwapWeapon(tagNetCmd* pCmd);
	DWORD	HandleRoleIdentifyEquip(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��ɫ�����ʾ����
	//------------------------------------------------------------------------
	DWORD	HandleRoleSetFashion(tagNetCmd* pCmd);
	DWORD	HandleRoleSetDisplay(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��Ʒ�������
	//------------------------------------------------------------------------
	DWORD	HandleRoleChangeItemPos(tagNetCmd* pCmd);
	DWORD	HandleRoleChangeItemPosEx(tagNetCmd* pCmd);
	DWORD	HandleRoleReorderItem(tagNetCmd* pCmd);
	DWORD	HandleRoleReorderItemEx(tagNetCmd* pCmd);
	DWORD	HandleRoleSetTransportMap(tagNetCmd* pCmd); //ʹ�ü�¼ʽ���ͷ�
	DWORD	HandleRoleUseTransportMap(tagNetCmd* pCmd); //ʹ�ü�¼ʽ���ͷ�
	//------------------------------------------------------------------------
	// ��Ҽ佻�����
	//------------------------------------------------------------------------
	DWORD	HandleRoleExchangeReq(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeReqRes(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeAdd(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeDec(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeMoney(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeLock(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeCancel(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeVerify(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �̵����
	//------------------------------------------------------------------------
	DWORD	HandleRoleGetShopItems(tagNetCmd* pCmd);
	DWORD	HandleRoleGetShopEquips(tagNetCmd* pCmd);
	DWORD	HandleRoleBuyShopItem(tagNetCmd* pCmd);
	DWORD	HandleRoleBuyShopEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleSellToShop(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��̯���
	//------------------------------------------------------------------------
	DWORD	HandleRoleStallStart(tagNetCmd* pCmd);
	DWORD	HandleRoleStallSetGoods(tagNetCmd* pCmd);
	DWORD	HandleRoleStallUnsetGoods(tagNetCmd* pCmd);
	DWORD	HandleRoleStallSetTitle(tagNetCmd* pCmd);
	DWORD	HandleRoleStallSetAdText(tagNetCmd* pCmd);
	DWORD	HandleRoleStallSetAdFlag(tagNetCmd* pCmd);
	DWORD	HandleRoleStallSetFinish(tagNetCmd* pCmd);
	DWORD	HandleRoleStallClose(tagNetCmd* pCmd);
	DWORD	HandleRoleStallGet(tagNetCmd* pCmd);
	DWORD	HandleRoleStallGetTitle(tagNetCmd* pCmd);
	DWORD	HandleRoleStallBuy(tagNetCmd* pCmd);
	DWORD	HandleRoleStallGetSpec(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��վ&Ǭ��ʯ
	//------------------------------------------------------------------------
	DWORD	HandleRoleDak(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ʹ��ĥʯ
	//------------------------------------------------------------------------
	DWORD	HandleRoleAbrase(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��ɫ�ֿ�
	//------------------------------------------------------------------------
	DWORD	HandleRoleSideWareOpen(tagNetCmd* pCmd);
	DWORD	HandleRoleWareExtend(tagNetCmd* pCmd);
	DWORD	HandleRoleBagExtand(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��ɫ�ֿ��д�ȡ��Ǯ&Ԫ��
	//------------------------------------------------------------------------
	DWORD	HandleRoleSaveSilver(tagNetCmd* pCmd);
	DWORD	HandleRoleGetSilver(tagNetCmd* pCmd);
	//DWORD	HandleRoleSaveYuanBao(tagNetCmd* pCmd);
	DWORD	HandleRoleGetYuanBao(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ���Ҽ������
	//------------------------------------------------------------------------
	DWORD	HandleRoleSetBagPsd(tagNetCmd* pCmd);
	DWORD	HandleRoleUnsetBagPsd(tagNetCmd* pCmd);
	DWORD	HandleRoleCheckBagPsd(tagNetCmd* pCmd);
	DWORD	HandleRoleResetBagPsd(tagNetCmd* pCmd);
	DWORD	HandleRoleOpenBagPsd(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------
	DWORD	HandleRoleEnterWorld(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------
	DWORD   HandleRoleChat(tagNetCmd* pCmd);
	DWORD   HandleRoleGetID(tagNetCmd* pCmd);
	DWORD	HandleRoleGetNameByNameID(tagNetCmd* pCmd);
	DWORD	HandleRoleGetSomeName(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// װ��չʾ
	//------------------------------------------------------------------------
	DWORD   HandleRoleShowEquip(tagNetCmd* pCmd);
	//------------------------------------------------------------------------
	// ��Ʒչʾ
	//------------------------------------------------------------------------
	DWORD   HandleRoleShowItem(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��ȡ����
	//------------------------------------------------------------------------
	DWORD   HandleRoleLoadLeftMsg(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��ͼ�¼�
	//------------------------------------------------------------------------
	DWORD	HandleRoleMapTrigger(tagNetCmd* pCmd);
	DWORD	HandleRoleInstanceNotify(tagNetCmd* pCmd);
	DWORD	HandleRoleInstanceAgree(tagNetCmd* pCmd);
	DWORD	HandleRoleLeaveInstance(tagNetCmd* pCmd);

	//-------------------------------------------------------------------------
	// ���Ե����
	//-------------------------------------------------------------------------
	DWORD	HandleRoleBidAttPoint(tagNetCmd* pCmd);
	DWORD	HandleRoleClearAttPoint(tagNetCmd* pCmd);

	//-------------------------------------------------------------------------
	// ���ʼ������
	//------------------------------------------------------------------------
	DWORD	HandleRoleLearnSkill(tagNetCmd* pCmd);
	DWORD	HandleRoleLevelUpSkill(tagNetCmd* pCmd);
	DWORD	HandleRoleForgetSkill(tagNetCmd* pCmd);
	DWORD	HandleRoleClearTalent(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ս��ϵͳ
	//------------------------------------------------------------------------
	DWORD	HandleRoleEnterCombat(tagNetCmd* pCmd);
	DWORD	HandleRoleLeaveCombat(tagNetCmd* pCmd);
	DWORD	HandleRoleSkill(tagNetCmd* pCmd);
	DWORD	HandleRoleInterruptSkill(tagNetCmd* pCmd);

	DWORD	HandleRoleCancelBuff(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// PKϵͳ
	//------------------------------------------------------------------------
	DWORD	HandleRoleSafeGuard(tagNetCmd* pCmd);
	DWORD	HandleRoleSetPK(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------
	DWORD	HandleRoleBindRebornMap(tagNetCmd* pCmd);
	DWORD	HandleRoleRevive(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ͨ�ú��� -- ���ڵ�ͼ�߳��ϲ㴦�����Ϣ
	//------------------------------------------------------------------------
	DWORD   HandleRoleMsg2World(tagNetCmd* pCmd) { return RET_TRANS; }

	//------------------------------------------------------------------------
	// ͨ������ -- GM
	//------------------------------------------------------------------------
	DWORD   HandleGMCommand(tagNetCmd* pCmd);
	
	//------------------------------------------------------------------------
	// �������
	//------------------------------------------------------------------------
	DWORD	HandleRoleNPCAcceptQuest(tagNetCmd* pCmd);
	DWORD	HandleRoleTriggerAcceptQuest(tagNetCmd* pCmd);
	DWORD	HandleRoleCompleteQuest(tagNetCmd* pCmd);
	DWORD   HandleRoleDeleteQuest(tagNetCmd* pCmd);
	DWORD	HandleRoleUpdateQuestNPCTalk(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��Ʒǿ��
	//------------------------------------------------------------------------
	DWORD	HandleRolePosyEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleEngraveEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleInlayEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleBrandEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleLoongSoul(tagNetCmd* pCmd);
	DWORD	HandleRoleQuench(tagNetCmd* pCmd);
	DWORD	HandleRoleChisel(tagNetCmd* pCmd);
	DWORD	HandleRoleDyeFashion(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �����ϳ���Ʒ
	//------------------------------------------------------------------------
	DWORD	HandleRoleProduceItem(tagNetCmd* pCmd);

	// Jason 2009-12-6
	DWORD	HandleRoleAddEquipPotVal(tagNetCmd* pCmd);
	// Jason 2009-12-7
	DWORD	HandleLessingLoong(tagNetCmd * pCmd);

	// Jason 2009-12-8 ���ʯ
	DWORD HandleGoldStone(tagNetCmd * pCmd);

	// Jason v1.3.1 2009-12-21
	DWORD HandleGemRemoval(tagNetCmd * pCmd);
	DWORD HandleGetGemRemovalInfo(tagNetCmd * pCmd);

	//------------------------------------------------------------------------
	// �㻯, װ���ֽ�
	//------------------------------------------------------------------------
	DWORD	HandleRoleDeCompose(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ʹ����Ʒ
	//------------------------------------------------------------------------
	DWORD	HandleRoleUseItem(tagNetCmd* pCmd);
	DWORD	HandleRoleInterruptUseItem(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ʰȡ��Ʒ
	//------------------------------------------------------------------------
	DWORD	HandleRolePickUpItem(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �ӵ���Ʒ
	//------------------------------------------------------------------------
	DWORD	HandleRolePutDownItem(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �������
	//------------------------------------------------------------------------
	DWORD   HandleRoleMakeFriend(tagNetCmd* pCmd);
	DWORD	HandleRoleCancelFriend(tagNetCmd* pCmd);
	DWORD	HandleUpdateFriendGroup(tagNetCmd* pCmd);
	DWORD	HandleMoveBlackList(tagNetCmd* pCmd);
	DWORD	HandleDeleteBlackList(tagNetCmd* pCmd);
	DWORD	HandleRoleSendGift(tagNetCmd* pCmd);
	DWORD	HandleRoleSendGiftReply(tagNetCmd* pCmd);
	DWORD   HandleUpdateFriOnline(tagNetCmd* pCmd);
	//Jason 2009-11-25 ����������Э��
	DWORD	HandleApplyJoinTeam(tagNetCmd *pCmd);
	DWORD	HandleReplyForApplingToJoinTeam(tagNetCmd * pCmd);

	//------------------------------------------------------------------------
	// С�����
	//------------------------------------------------------------------------
	DWORD   HandleRoleJoinTeam(tagNetCmd* pCmd);
	DWORD	HandleRoleJoinTeamReply(tagNetCmd* pCmd);
	DWORD	HandleRoleKickMember(tagNetCmd* pCmd);
	DWORD	HandleRoleLeaveTeam(tagNetCmd* pCmd);
	DWORD	HandleRoleSetPickMol(tagNetCmd* pCmd);
	DWORD	HandleRoleChangeLeader(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �ƺ����
	//------------------------------------------------------------------------
	DWORD HandleRoleActiveTitle(tagNetCmd* pCmd);
	DWORD HandleRoleGetTitles(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �������
	//------------------------------------------------------------------------
	DWORD HandleRoleGetVCard(tagNetCmd* pCmd);
	DWORD HandleRoleSetVCard(tagNetCmd* pCmd);
	DWORD HandleGetHeadPicUrl(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ���������
	//------------------------------------------------------------------------
	DWORD HandleGetFatigueInfo(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ���������
	//------------------------------------------------------------------------
	DWORD HandleGetFameHallRoles(tagNetCmd* pCmd);
	DWORD HandleGetReputeTop(tagNetCmd* pCmd);
	DWORD HandleGetActClanTreasure(tagNetCmd* pCmd);
	DWORD HandleActiveTreasure(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �������
	//------------------------------------------------------------------------
	DWORD HandleGetRoleClanData(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��������
	//------------------------------------------------------------------------
	DWORD HandleGameGuarderMsg(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �������
	//------------------------------------------------------------------------
	DWORD HandleGetPetAttr(tagNetCmd* pCmd);
	DWORD HandlePetSkill(tagNetCmd* pCmd);
	DWORD HandlePetSkillStopWorking(tagNetCmd* pCmd);
	DWORD HandleUsePetEgg(tagNetCmd* pCmd);
	DWORD HandleDeletePet(tagNetCmd* pCmd);
	DWORD HandleGetPetDispInfo(tagNetCmd* pCmd);
	DWORD HandleSetPetState(tagNetCmd* pCmd);

	DWORD HandlePetEquip(tagNetCmd* pCmd);
	DWORD HandlePetUnEquip(tagNetCmd* pCmd);
	DWORD HandlePetSwapEquipPos(tagNetCmd* pCmd);
	DWORD HandleGetPetPourExpMoneyNeed(tagNetCmd* pCmd);
	DWORD HandlePetPourExp(tagNetCmd* pCmd);
	DWORD HandlePetUpStep(tagNetCmd* pCmd);
	DWORD HandlePetEnhance(tagNetCmd* pCmd);
	DWORD HandlePetLearnSkill(tagNetCmd* pCmd);
	DWORD HandlePetInvite(tagNetCmd* pCmd);
	DWORD HandlePetOnInvite(tagNetCmd* pCmd);
	DWORD HandlePetFood(tagNetCmd* pCmd);
	DWORD HandlePetSetLock(tagNetCmd* pCmd);
	

	DWORD	HandleRolePetExchangeReq(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeReqRes(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeAdd(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeDec(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeMoney(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeLock(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeCancel(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeVerify(tagNetCmd* pCmd);


	//------------------------------------------------------------------------
	// �ٱ������
	//------------------------------------------------------------------------
	DWORD HandleInitBaiBaoRecord(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// VIP̯λ���
	//------------------------------------------------------------------------
	DWORD HandleGetAllVIPStallInfo(tagNetCmd* pCmd);
	DWORD HandleUpdateVIPStallInfo(tagNetCmd* pCmd);
	DWORD HandleApplyVIPStall(tagNetCmd* pCmd);
	DWORD HandleSpecVIPStallGet(tagNetCmd* pCmd);
	DWORD HandleBuyVIPStallGoods(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// NPC���
	//------------------------------------------------------------------------
	DWORD HandleTalkToNPC(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �����������
	//------------------------------------------------------------------------
	DWORD HandleGetVipNetBarName(tagNetCmd* pCmd);


	//���
	DWORD HandleGetMarriageReq(tagNetCmd* pCmd);
	DWORD HandleGetMarriageReqRes(tagNetCmd* pCmd);
	//���
	DWORD HandleBreakMarriageReq(tagNetCmd* pCmd);
	DWORD HandleBreakMarriageReqRes(tagNetCmd* pCmd);
	//����
	DWORD HandleCelebrateWeddingReq(tagNetCmd* pCmd);
	DWORD HandleCelebrateWeddingReqRes(tagNetCmd* pCmd);
	//------------------------------------------------------------------------
	// ���Զ���
	//------------------------------------------------------------------------
	DWORD	HandleRoleStyleAction(tagNetCmd* pCmd);
	DWORD	HandleRoleDuetMotionInvite(tagNetCmd* pCmd);
	DWORD	HandleRoleDuetMotionOnInvite(tagNetCmd* pCmd);
	DWORD	HandleRoleDuetMotionStart(tagNetCmd* pCmd);


	/************************************************************************
	** Handlers -- upper all map thread
	*************************************************************************/

	//------------------------------------------------------------------------
	// �̳����
	//------------------------------------------------------------------------
	DWORD HandleRoleMallGet(tagNetCmd* pCmd);
	DWORD HandleRoleMallUpdate(tagNetCmd* pCmd);
	DWORD HandleRoleMallBuyItem(tagNetCmd* pCmd);
	DWORD HandleRoleMallBuyPack(tagNetCmd* pCmd);
	DWORD HandleRoleMallPresentItem(tagNetCmd* pCmd);
	DWORD HandleRoleMallPresentPack(tagNetCmd* pCmd);
	DWORD HandleRoleMallFreeGetItem(tagNetCmd* pCmd);
	DWORD HandleRoleMallLaunchGroupPurchase(tagNetCmd* pCmd);
	DWORD HandleRoleMallRespondGroupPurchase(tagNetCmd* pCmd);
	DWORD HandleRoleMallGetGroupPurchaseInfo(tagNetCmd* pCmd);
	DWORD HandleRoleMallGetParticipators(tagNetCmd* pCmd);
	DWORD HandleRoleMallItemExchange(tagNetCmd* pCmd);
	DWORD HandleRoleMallPackExchange(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// Ԫ���������
	//------------------------------------------------------------------------
	DWORD HandleRoleSaveYB2Account(tagNetCmd* pCmd);
	DWORD HandleRoleSaveSilver2Account(tagNetCmd* pCmd);
	DWORD HandleRoleDepositYBAccount(tagNetCmd* pCmd);
	DWORD HandleRoleDepositSilver(tagNetCmd* pCmd);
	DWORD HandleRoleGetYBTradeInfo(tagNetCmd* pCmd);
	DWORD HandleRoleSubmitSellOrder(tagNetCmd* pCmd);
	DWORD HandleRoleSubmitBuyOrder(tagNetCmd* pCmd);
	DWORD HandleRoleDeleteOrder(tagNetCmd* pCmd);
	DWORD HandleRoleGetYBOrder(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �������
	//------------------------------------------------------------------------
	DWORD HandleCreateGuild(tagNetCmd* pCmd);
	DWORD HandleDismissGuild(tagNetCmd* pCmd);
	DWORD HandleJoinGuildReq(tagNetCmd* pCmd);
	DWORD HandleJoinGuildReqRes(tagNetCmd* pCmd);
	DWORD HandleLeaveGuild(tagNetCmd* pCmd);
	DWORD HandleKickFromGuild(tagNetCmd* pCmd);
	DWORD HandleTurnoverGuild(tagNetCmd* pCmd);
	DWORD HandleDemissFromGuild(tagNetCmd* pCmd);
	DWORD HandleAppointForGuild(tagNetCmd* pCmd);
	DWORD HandleChangeGuildTenet(tagNetCmd* pCmd);

	DWORD HandleSyncGuildInfo(tagNetCmd* pCmd);

	DWORD HandleGetGuildMembers(tagNetCmd* pCmd);
	DWORD HandleGetGuildMemberEx(tagNetCmd* pCmd);
	DWORD HandleRefreshGuildMember(tagNetCmd* pCmd);
	DWORD HandleGetGuildName(tagNetCmd* pCmd);
	DWORD HandleGetGuildTenet(tagNetCmd* pCmd);

	DWORD HandleGetGuildWareItems(tagNetCmd* pCmd);
	DWORD HandleGetGuildWarePriList(tagNetCmd* pCmd);
	DWORD HandleGuildWarePrivilege(tagNetCmd* pCmd);

	DWORD HandleGetGuildFacilitiesInfo(tagNetCmd* pCmd);
	DWORD HandleHandInItems(tagNetCmd* pCmd);

	DWORD HandleSpreadGuildAffair(tagNetCmd* pCmd);

	DWORD HandleGetGuildSkillInfo(tagNetCmd* pCmd);
	DWORD HandleUpgradeGuildSkill(tagNetCmd* pCmd);
	DWORD HandleLearnGuildSkill(tagNetCmd* pCmd);
	DWORD HandleSetResearchSkill(tagNetCmd* pCmd);

	DWORD HandleGetCofCInfo(tagNetCmd* pCmd);
	DWORD HandleCloseCofC(tagNetCmd* pCmd);
	DWORD HandleBuyCofCGoods(tagNetCmd* pCmd);
	DWORD HandleSellCofCGoods(tagNetCmd* pCmd);
	DWORD HandleGetCommodityInfo(tagNetCmd* pCmd);
	DWORD HandleGetTaelInfo(tagNetCmd* pCmd);
	DWORD HandleGetCommerceRank(tagNetCmd* pCmd);

	DWORD HandleAcceptCommerce(tagNetCmd* pCmd);
	DWORD HandleCompleteCommerce(tagNetCmd* pCmd);
	DWORD HandleAbandonCommerce(tagNetCmd* pCmd);
	DWORD HandleSwitchCommendation(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �����������
	//------------------------------------------------------------------------
	DWORD HandleOpenTreasureChest(tagNetCmd* pCmd);
	DWORD HandleStopTreasureChest(tagNetCmd* pCmd);
	DWORD HandleAgainTreasureChest(tagNetCmd* pCmd);
	DWORD HandleChestGetItem(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ���ؽ�ɫѡ��
	//------------------------------------------------------------------------
	DWORD HandleReturnRoleSelect(tagNetCmd* pCmd);

	// �ͻ��˶Ի��򷢸������ȱʡ��Ϣ
	DWORD HandleDlgDefaultMsg(tagNetCmd* pCmd);
	// �ͻ��˴����������ű���ȱʡ��Ϣ
	DWORD HandleDefaultRequest(tagNetCmd* pCmd);

	// ���������ҵ�װ����Ϣ
	DWORD HandleGetSomeoneEquip(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ְҵ���
	//------------------------------------------------------------------------
	DWORD HandleSetClass(tagNetCmd* pCmd);
	DWORD HandleChangeClass(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ʵ��ϵͳ���
	//------------------------------------------------------------------------
	DWORD HandleGetRankings(tagNetCmd* pCmd);
	DWORD HandleGetSelfStrength(tagNetCmd* pCmd);
	DWORD HandleGetStrengthRankings(INT nPage);
	DWORD HandleGetFameRankings(INT nPage);
	DWORD HandleGetLevelRankings(INT nPage);
	//DWORD HandleGetMoneyRankings();

	//------------------------------------------------------------------------
	// ������ʾ
	//------------------------------------------------------------------------
	DWORD HandleRoleLeaveNotify(tagNetCmd* pCmd);

private:
	static PlayerNetCmdMgr	m_PlayerNetMgr;						// ��Ӧ�Ŀͻ�����Ϣ������
	static GMCommandMgr		m_GMCommandMgr;						// GM���������

	DWORD					m_dwAccountID;						// session id����Ӧ�����ʺ�ID
	DWORD					m_dwInternalIndex;					// �ײ������ID
	INT						m_nMsgNum;							// ����ײ�δ�������Ϣ����

	bool					m_bRoleLoading;						// ѡ������ʱ���ڵȴ����ݿⷵ��
	bool					m_bRoleEnuming;						// ������Ϸʱ�ȴ�������ѡ�˽���ɫ��Ϣ��ȡ����
	bool					m_bRoleEnumDone;					// ��ȡ��ɫ��Ϣ���
	bool					m_bRoleEnumSuccess;					// ��ȡ��ɫ��Ϣ�Ƿ�ɹ�
	bool					m_bRoleCreating;					// �ȴ�������ɫ
	bool					m_bRoleDeleting;					// ɾ������ʱ�ȴ����ݿⷵ��
	BYTE					m_byPrivilege;						// gmȨ��
	INT8					m_n8RoleNum;						// �Ѿ������Ľ�ɫ����

	bool					m_bRoleInWorld;						// ����Ϸ������

	DWORD					m_dwRoleID[MAX_ROLENUM_ONEACCOUNT];	// �ʺ������н�ɫID
	
	char					m_szAccount[X_SHORT_NAME];			// ����˺�

	volatile BOOL			m_bConnectionLost;					// �����Ƿ��Ѿ��Ͽ�
	volatile BOOL			m_bKicked;							// �Ƿ��Ѿ����ߵ�

	tagAccountCommon		m_sAccountCommon;					// �˺�ͨ����Ϣ

	Role*					m_pRole;							// ��Ӧ�Ľ�ɫ����

	DWORD					m_dwIP;								// �ͻ���IP

	FatigueGuarder			m_FatigueGarder;					// ������


	tagDWORDTime			m_dwPreLoginTime;					// �ϴε�¼ʱ��
	DWORD					m_dwPreLoginIP;						// �ϴε�¼ip

	FastMutex				m_PlayerNetLock;
};


