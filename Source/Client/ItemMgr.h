#pragma once
#include "..\WorldDefine\ItemDefine.h"
#include "..\WorldDefine\role_att.h"
#include "..\WorldDefine\msg_item.h"
#include "..\WorldDefine\container_define.h"
#include "..\WorldDefine\msg_guild.h"
class ItemContainer;
class Item;
class Equipment;
class NetSession;
struct tagNS_BagExtend;
struct tagNS_WareExtend;
struct tagNS_SuitNum;
struct tagNS_Abrase;
struct tagItemContainerExtend;

/** \struct tagBeGoldStoneItemTable
	\breif ���ʯ��Ʒ��
*/
struct tagBeGoldStoneItemTable
{
	DWORD dwSrcItemID;
	DWORD dwDestItemID;
	DWORD dwGoldStoneType;
};

/** \struct tagBaiBaoRecordC
	\breif	�ٱ���������¼�ͻ��˴洢�ṹ
*/
struct tagBaiBaoRecordC
{
	EBaiBaoRecordType	eType;
	DWORD				dwTypeID;
	DWORD				dwRoleID;
	tagDWORDTime		sTime;
	tstring				strWords;
};

/**	\class ItemMgr
	\brief ��Ʒ������

*/
class ItemMgr
{
public:
	enum EPocketEncryptState// ��������״̬
	{
		EPES_Null			=	-1,		
		EPES_EnterPassword	=	0,		// ������������
		EPES_SetupPassword	=	1,		// ������������
		EPES_ModifyPassword	=	2,		// �޸���������
		EPES_CancelPassword	=	3		// ȡ����������
	};
public:
	ItemMgr();
	~ItemMgr();

	static ItemMgr* Inst();


	VOID Init();
	VOID Update();
	VOID Destroy();
		
	// ��ȡ��������ָ��
	ItemContainer* GetPocket(){return m_pPocket;}
	ItemContainer* GetThrone(){return m_pThrone;}
	ItemContainer* GetEquipment(){return m_pEquipment;}
	ItemContainer* GetStorge(){return m_pStorge;}
	ItemContainer* GetQuest(){return m_pQuest;}
    ItemContainer* GetGuildStore(){return m_pGuildStore;}
	
	// ����ģ��ID��ȡ��Ʒ��ͼ��
	tstring GetItemIconPath(DWORD dwTypeID);

	// ��ȡ�����е���Ʒ
	Item* GetPocketItem(INT16 n16Pos);
	// ��ñ����е���Ʒ
	Item* GetPocketItemByID(INT64 n64ID);
	// ��ȡװ�����е�װ��
	Equipment* GetCurEquip(EEquipPos ePos);
	// ��ȡװ�����е�װ��
	Equipment* GetCurEquipByID(INT64 n64ID);
	// ��ȡ��Ӧ�����е���Ʒ
	Item* GetConItem(EItemConType ePos, INT16 n16Pos);
	// ��ȡ��Ӧ�����е���Ʒ
	Item* GetConItemByID(EItemConType ePos, INT64 n64ID);
	// �ж�װ�����Ƿ�Ϊ��
	BOOL IsNotEquip(EEquipPos ePos);
	// ��ȡ��Ӧ������Ӧҳ��һ����λ
	INT16 GetConBlank(EItemConType ePos, INT nIndex = 0);

	// ��ȡĳ��װ��ǰ����
	INT8 GetSuitNum(DWORD dwSuitID){return m_mapSuitNum.Peek(dwSuitID);}
	// ��ȡĳ��װ�����������Ƿ񼤻�
	bool GetLonghun(EEquipPos ePos, INT64 id);

	// ��ȡ��ȴʱ��
	VOID GetItemCurCDTime(DWORD dwTypeID, FLOAT& fCDCount, FLOAT& fRestoreRatio);

	// ��ȡ��Ӧ���⹦�����͵���ƷID
	INT64 GetSpecfuncItemID(EItemSpecFunc eType);
	// ��ȡ��ǰ������������
	EItemTypeEx GetHandWeaponType(EEquipPos ePos=EEP_RightHand);
	
	// ��ʾ��Ʒ��ش�����
	VOID ShowItemErrorMsg(DWORD dwErrorCode);

	// ��ȡ�ٱ�����¼�б�
	VOID GetBaiBaoList(TList<tagBaiBaoRecordC> &listRecord);
	
	// ���õ�ǰ���Ҽ��ܵ�״̬������ͻ��˴�����������͵���ϢNS_BagPsd
	void SetPocketEncryptState(EPocketEncryptState eState){m_eEncryptState = eState;}

    // ��ȡ���ɲֿ�
    void OnGetGuildWare(tagNS_GetGuildWare* pMsg);
    // ��հ��ɲֿ�
    void ClearGuildWare();

	//brief �ӱ����еõ���һ�����⹦�ܵ���Ʒ
	Item* GetFirstSpecFuncItemFromPocket( EItemSpecFunc eItemSpecFunc );

	// �����Ƿ��ڵ��ʯ��Ʒ����
	bool IsInGoldStoneTable( DWORD dwItemId, DWORD dwGoldStoneType );

	/** \brief �ӱ����еõ���һ�����⹦�ܵ���Ʒ
		\param DWORD dwItemTypeID ���ܲ�ͬ����Ʒ�� ͬ�������⹦�ܣ�������ͨ����͸߼����
		\param bEnableType ���������ڶ���������Ĭ���ǲ�������
	*/
	Item* GetFirstSpecFuncItemFromPocket( EItemSpecFunc eItemSpecFunc, DWORD dwItemTypeID, BOOL bEnableType = FALSE );

protected:
	//--������Ϣ������
	// ��ʼ����Ʒ
	DWORD NetInitLocalPlayerItem(tagNS_GetRoleInitState_Item* pNetCmd, DWORD);
	// ��ʼ����Ʒ��ȴ
	DWORD NetInitLocalPlayerItemCD(tagNS_GetRoleInitState_ItemCDTime* pNetCmd, DWORD);
	// ��ʼ����װ
	DWORD NetInitLocalPlayerSuit(tagNS_GetRoleInitState_Suit* pNetCmd, DWORD);
	// ��ʼ����������
	DWORD NetInitLocalPlayerLonghun(tagNS_GetRoleInitState_Longhun* pNetCmd, DWORD);
	// ��װ������
	DWORD NetEquipment(tagNS_Equip* pNetCmd, DWORD);
	// ��װ������
	DWORD NetUnEquipment(tagNS_Unequip* pNetCmd, DWORD);
	// ����������
	DWORD NetSwapWeapon(tagNS_SwapWeapon* pNetCmd, DWORD);
	// ��Ʒλ�øı䣬ͬһ������
	BOOL NetItemPosChange(tagNS_ItemPosChange* pNetCmd, DWORD);
	// ��Ʒλ�øı䣬��ͬ����֮��
	BOOL NetItemPosChangeEx(tagNS_ItemPosChangeEx* pNetCmd, DWORD);
	// ����һ����Ʒ
	DWORD NetNewItemAdd(tagNS_NewItemAdd* pNetCmd, DWORD);
	// ����һ��װ��
	DWORD NetNewEquipAdd(tagNS_NewEquipAdd* pNetCmd, DWORD);
	// ����һ����֪��Ʒ
	DWORD NetItemAdd(tagNS_ItemAdd* pNetCmd, DWORD);
	// ɾ��һ����Ʒ
	DWORD NetItemRemove(tagNS_ItemRemove* pNetCmd, DWORD);
	// ����Ʒ
	DWORD NetItemBind(tagNS_ItemBind* pNetCmd, DWORD);
	// װ�����Ըı�
	DWORD NetEquipChange(tagNS_EquipChange* pNetCmd, DWORD);
	// ��ƷCD�ı�
	DWORD NetItemCDUpdate(tagNS_ItemCDUpdate* pNetCmd, DWORD);
	// ��������
	DWORD NetBagExtend(tagNS_BagExtend* pNetCmd, DWORD);
	// �ֿ�����
	DWORD NetWareExtend(tagNS_WareExtend* pNetCmd, DWORD);
	// ��װ����
	DWORD NetSuitNumChange(tagNS_SuitNum* pNetCmd, DWORD);
	// ����װ��
	DWORD NetIdentifyEquip(tagNS_IdentifyEquip* pNetCmd, DWORD);
	// ĥʯ��ĥ
	DWORD NetAbraseWeapon(tagNS_Abrase* pNetCmd, DWORD);
    // ʱװȾɫ
    DWORD NetDyeFashion(tagNS_DyeFashion* pNetCmd, DWORD);
	// ���Ҽ��ܷ���
	DWORD NetBagPassword(tagNS_BagPsd* pNetCmd, DWORD);
	// ����ո�¶ȱ仯
	DWORD NetWeaponNewnessChange(tagNS_RWNewessChange* pNetCmd, DWORD);
	// �����Զ�����
	DWORD NetContainerReorder(tagNS_ItemReorder* pNetCmd, DWORD);
	DWORD NetContainerReorderEx(tagNS_ItemReorderEx* pNetCmd, DWORD);
	// ����װ������������������
	DWORD NetEquipLonghunOn(tagNS_LongHunOn* pNetCmd, DWORD);
	DWORD NetEquipLonghunOff(tagNS_LongHunOff* pNetCmd, DWORD);
	// ����ٱ�����¼����
	DWORD NetInitBaiBaoRecord(tagNS_InitBaiBaoRecord* pNetCmd, DWORD);
	DWORD NetSingleBaiBaoRecord(tagNS_SingleBaiBaoRecord* pNetCmd, DWORD);

	//��Ӧ�򿪲ֿ�
	DWORD NetWareOpen( tagNS_WareOpen* pNetCmd, DWORD );

	DWORD NetBeGoldStone( tagNS_BeGoldStone *pNetCmd, DWORD );

	//--������Ϸ�¼�
	// ��Ӧ��������
	DWORD OnEventBagExtend(tagItemContainerExtend* pGameEvent);
	// ��Ӧ�ֿ�����
	DWORD OnEventWareExtend(tagItemContainerExtend* pGameEvent);
	// ȷ����������
	DWORD OnEventBagExtendCheck(tagMsgBoxEvent* pGameEvent);
	// ȷ�ϲֿ�����
	DWORD OnEventWareExtendCheck(tagMsgBoxEvent* pGameEvent);

	
	// ��ȡ��Ӧ�������͵�ָ��
	ItemContainer* GetContainer(EItemConType eType);
	
	// ����һ����Ʒ;
	VOID AddNewItem(Item* pItem);

	// ����һ���ٱ�����¼
	VOID AddNewBaiBaoRecord(tagBaiBaoRecord* pRecord);

private:
	void LoadBeGoldStoneItemTable();

private:
	TSFPTrunk<ItemMgr>		m_Trunk;
	TObjRef<Util>			m_pUtil;
	TObjRef<NetCmdMgr>		m_pCmdMgr;
	TObjRef<NetSession>		m_pSession;
	TObjRef<GameFrameMgr>	m_pFrameMgr;
	TObjRef<VarContainer>	m_pVarContainer;

	ItemContainer*			m_pPocket;		// ����
	ItemContainer*			m_pStorge;		// �ֿ�
	ItemContainer*			m_pEquipment;	// װ����
	ItemContainer*			m_pThrone;		// �ٱ���
	ItemContainer*			m_pQuest;		// ������Ʒ��
    ItemContainer*          m_pGuildStore;  // ���ɲֿ�

	TMap<DWORD, DWORD>		m_mapCurStartTime;		// ��ȴʱ�俪ʼ��
	TMap<DWORD, INT8>		m_mapSuitNum;			// ��װ����
	TMap<INT16, INT8>		m_mapLonghunOn;			// ���꼤�����
	bool					m_bUseSilverExtend;		// ʹ�ý�Ǯ�����־λ
	bool					m_bNotEnoughYuanbao;	// Ԫ�������־λ
	bool					m_bNotEnoughSilver;		// ��Ǯ�����־λ
	bool					m_bNotInitBaiBaoRecord;	// �ٱ������±�־λ
	TList<tagBaiBaoRecordC>	m_listBaiBaoRecord;		// �ٱ�����¼�б�
	vector<tagBeGoldStoneItemTable> m_vecBeGoldStoneItemTable;	// ���ʯ��Ʒ��

	EPocketEncryptState		m_eEncryptState;		// �������뵱ǰ����״̬
};
