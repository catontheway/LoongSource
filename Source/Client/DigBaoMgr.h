#pragma once
#include "..\WorldDefine\msg_digbao.h"
#include "..\WorldDefine\chat.h"
#include "ItemEvent.h"

struct tagDigBaoEvent : public tagGameEvent
{
	tagDigBaoEvent(LPCTSTR szEventName, GameFrame* pSenderFrame):tagGameEvent(szEventName, pSenderFrame){}
	DWORD			dwTargetID;
	DWORD			dwTargetTypeID;
};

class NetSession;

/** class DigBaoMgr
	brief �ڱ�,����������
*/
class DigBaoMgr
{
public:
	DigBaoMgr(void);
	~DigBaoMgr(void);
	static DigBaoMgr* Inst();
	
	void Init();
	void Destroy();
	void Update();

private:

	//���ڹ㲥����
	enum EDigBaoCastType
	{
		EDBCT_Null = 0,
		EDBCT_ChatBillike,
		EDBCT_ChatBadOldSeal,
		EDBCT_Plutus,
		EDBCT_AwakenGods,
		EDBCT_GodsOfPractice,
		EDBCT_MonsterAttackCity,
		EDBCT_RoleGetChestItem,
	};

	struct tagDigBaoCast
	{
		EDigBaoCastType			eType;			//�㲥����
		DWORD					dwRoleID;		//���ID
		DWORD					dwMapCrc;		//��ͼ����CRCֵ
		DWORD					dwNpcTypeID;	//NPC type id
		DWORD					dwItemTypeID;	//��Ʒ type id

		tagDigBaoCast():eType(EDBCT_Null),dwRoleID(GT_INVALID),dwMapCrc(GT_INVALID),
			dwNpcTypeID(GT_INVALID),dwItemTypeID(GT_INVALID)
		{
		}
	};

private:
	//--��Ϸ�¼�
	DWORD OnDigBaoEvent(tagDigBaoEvent* pGameEvent);
	DWORD OnDungeonEnterEvent(tagMsgBoxEvent* pGameEvent);
	DWORD OnGodsOfPracticeEvent(tagMsgBoxEvent* pGameEvent);
	DWORD OnTheRoleOfItemEvent(tagOnTheRoleOfItemEvent* pGameEvent);

	//--����������Ϣ
	DWORD OnNS_DiaBao(tagNS_DiaBao* pMsg, DWORD dwParam);
	DWORD OnNS_NormalTreasureMap(tagNS_NormalTreasureMap* pMsg, DWORD dwParam);
	DWORD OnNS_OldTreasureMap(tagNS_OldTreasureMap* pMsg, DWORD dwParam);
	DWORD OnNS_RareTreasureMap(tagNS_RareTreasureMap* pMsg, DWORD dwParam);

	//--�㲥
	DWORD OnNS_ChatBillike(tagNS_ChatBillike* pMsg, DWORD dwParam);
	DWORD OnNS_ChatBadOldSeal(tagNS_ChatBadOldSeal* pMsg, DWORD dwParam);
	DWORD OnNS_Plutus(tagNS_Plutus* pMsg, DWORD dwParam);
	DWORD OnNS_AwakenGods(tagNS_AwakenGods* pMsg, DWORD dwParam);
	DWORD OnNS_GodsOfPractice(tagNS_GodsOfPractice* pMsg, DWORD dwParam);
	DWORD OnNS_MonsterAttackCity(tagNS_MonsterAttackCity* pMsg, DWORD dwParam);
	DWORD OnNS_RoleGetChestItem(tagNS_RoleGetChestItem* pMsg, DWORD dwParam);

	// ������Ĺ㲥
	void OnDigBaoCast( tagDigBaoCast cast );

private:
	TSFPTrunk<DigBaoMgr>		m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	
	vector<tagDigBaoCast>		m_vecDigBaoCast;
	
};