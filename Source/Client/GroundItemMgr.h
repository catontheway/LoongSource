//-----------------------------------------------------------------------------
//!\file GroundItemMgr.h
//!\author Lyp
//!
//!\date 2008-10-27
//! last 2008-10-27
//!
//!\brief ������Ʒ����
//!
//!	Copyright (c) 2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

struct tagGroundItem
{
	INT64			n64Serial;		// ��Ʒ64λID
	DWORD			dwRoleID;		// ˭�����
	DWORD			dwTypeID;		// ��ƷtypeID 
	INT				nNum;			// ��Ŀ
	DWORD			dwOwnerID;		// ������ɫ
	DWORD			dwGroupID;		// �����Ŷ�ID
	DWORD			dwTeamID;		// ��������ID
	DWORD           dwEffectID;     // ��Ʒ����ЧID��������Ҿ���ʰȡȨ���Ქ��һ����Ч��
	
	BOOL			bChecked;		// �Ƿ�����ʰȡ����û�յ���Ϣ
	ObjectSpace		space;			// ��Ʒ3Dλ�ã���ǰλ�ã�
	EntityNode*		pNode;

	tagGroundItem() { n64Serial = 0; dwTypeID = GT_INVALID; nNum = 0; pNode = 0; dwEffectID = GT_INVALID; bChecked = FALSE; }
};

struct tagParabolaItem //��Ʒ�׶�����
{
	INT64			n64Serail;
	DWORD			dwStartTime;	// ���俪ʼʱ��
	FLOAT			fTotalTime;		// �������ʱ�� 
	FLOAT			fXZSpeed;		// XZ�����ϵ��ٶ�
	Vector3			vAxis;			// ת����
	Vector3			vStartPos;		// ��ʼ�׳���λ��
	Vector3			vEndPos;		// ���յ�λ��
};

class Item;
struct tagNS_SyncGroundItem;
struct tagNS_RoleGroundItemDisappear;
struct tagNS_PutDownItem;
struct tagNS_RolePickUpItem;
//-----------------------------------------------------------------------------
// GroundItemMgr
//-----------------------------------------------------------------------------
class GroundItemMgr
{
public:

	VOID Init();
	VOID Destroy();

	VOID Update();

	VOID ClearAll();

	tagGroundItem* PickItem(const Ray& rayMouse);
	VOID PutDownItem(Item* pItem, FLOAT fX, FLOAT fY, FLOAT fZ);
	BOOL PickUpItem(INT64 n64Serial, DWORD dwNum);

	tagGroundItem* FindItem(INT64 n64Serial);
	TMap<INT64, tagGroundItem*>& GetItemMap() { return m_mapGroundItem; }

	// ս��ϵͳ���ô˺������Ա㴦����Ϣ����
	DWORD RoleSyncGroundItem(tagNS_SyncGroundItem* pRecv, DWORD dwParam);

	// ��ȡԲ��֮�ڵ�����ı�����ҿ�ʰȡ����Ʒ��û�з���NULL
	tagGroundItem* GetNearestCanPickUpItem(const Vector3& vCenter, const float fRange);

    // �����ͻ�ȡԲ��֮�ڵ�����ı�����ҿ�Яʰȡ����Ʒ��û�з���NULL
    tagGroundItem* GetNearestCanPickUpItem(const Vector3& vCenter, const float fRange,
        bool bMoney, bool bEquip,
        bool bMaterial, bool bTrophy, bool bMedicine, bool bOthers);

	// ������������
	VOID SetPutDownAction(tagGroundItem* pItem);
	
	// ��������Ƿ����ʰȡȨ��
	bool CanPickUp(const tagGroundItem* pItem);

	// ��ʾ������Ϣ
	VOID ShowErrMsg(DWORD dwErrCode);

	GroundItemMgr();
	~GroundItemMgr();
	static GroundItemMgr* Inst();

private:
	TSFPTrunk<GroundItemMgr>		m_Trunk;
	TObjRef<Util>					m_pUtil;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;


	TMap<INT64, tagGroundItem*>		m_mapGroundItem;
	TList<tagParabolaItem*>			m_listParabolaItem;


	DWORD RoleGroundItemDisappear(tagNS_RoleGroundItemDisappear* pRecv, DWORD dwParam);
	DWORD RolePickUpItem(tagNS_RolePickUpItem* pRecv, DWORD dwParam);


	BOOL ShowItem(tagGroundItem* pItem);
	VOID HideItem(tagGroundItem* pItem);
	// play drop sound
	VOID PlayDropSound(DWORD dwTypeID);
};