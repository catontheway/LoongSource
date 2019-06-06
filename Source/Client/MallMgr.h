#pragma once
#include "..\WorldDefine\mall_define.h"
#include "..\WorldDefine\msg_mall.h"
/** class MallMgr
	brief �̳ǹ�����
*/

class NetSession;
class MallFrame;
class YuanbaoFrame;
class GroupBuyFrame;
class MallMgr
{
public:
	MallMgr(void);
	~MallMgr(void);
	static MallMgr* Inst();

	VOID Init();
	VOID Update();
	VOID Destroy();

	//--����������Ϣ
	DWORD OnNS_OpenMall(tagNS_OpenMall* pMsg, DWORD dwParam);
	DWORD OnNS_MallItem(tagNS_MallItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallFreeItem(tagNS_MallFreeItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallPack(tagNS_MallPack* pMsg, DWORD dwParam);
	DWORD OnNS_MallUpdateItem(tagNS_MallUpdateItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallUpdatePack(tagNS_MallUpdatePack* pMsg, DWORD dwParam);
	DWORD OnNS_MallUpdate(tagNS_MallUpdate* pMsg, DWORD dwParam);
	DWORD OnNS_GetSimGPInfo(tagNS_GetSimGPInfo* pMsg, DWORD dwParam);

	//--������Ϸ�¼�
	DWORD OnOpen_Mall(tagGameEvent* pEvent);
	DWORD OnClose_Mall(tagGameEvent* pEvent);

	//--
	TMap<DWORD, tagMallItemProto*>& GetMallProto()	{ return m_mapMallItem; }
	TMap<DWORD, tagMallPackProto*>& GetPackItem()	{ return m_mapPackItem; }
	tagMallFreeItem* GetMallFreeItem()				{ return &m_MallFreeItem; }
	tagMallItemProto* GetMallPromoteItem(int nIndex);
	map<INT64, tagGroupPurchase>& GetGroupBuyInfo() { return m_mapGgroupPur; }

	//--
	INT GetMallProtoSize() { return m_mapMallItem.Size(); }
	INT GetPackProtoSize() { return m_mapPackItem.Size(); } 
	INT GetGroupBuyInfoSize() { return m_mapGgroupPur.size(); }

	//--
	tagMallItemProto* GetMallItemByID(DWORD dwID);
	tagMallPackProto* GetMallPackByID(DWORD dwID);
	tagGroupPurchase* GetGroupBuyInfoByID(INT64 dwID);
	void DeleGroupBuyInfoByID(INT64 dwID);
	void RefreshGroupBuyInfoByID(INT64 dwID, INT nNum);


	//--�����ȡ��ʱ��
	DWORD GetFreeGetTime()const { return m_dwFreeGetTime; }
	void SetFreeGetTime(DWORD dwTime) { m_dwFreeGetTime = dwTime; } 

	//--
	void ClearMallItem();
	void ClearMallPack();
	void ClearGroupBuyInfo();

	//�̳�frame
	MallFrame* GetMallFrame() { return m_pMallFrame; }

	//--�Ź�
	void CreateGroupBuyFrame(LPCTSTR szFatherName);
	GroupBuyFrame* GetGroupBuyFrame() { return m_pGroupBuyFrame; }
	void SendGetGroupBuyInfo();
	INT GetGroupPurExp()const { return m_nGroupPurExp; }
	void SetGroupPurExp(INT32 exp)  { m_nGroupPurExp = exp; }

	//--���ʹ�Ʒ
	void CreateMallGiftFrame();
	void SetMallGiftFrameMaxBuyNum(INT nMaxBuyNum);

    //! Ԫ������
    void CreateYuanBaoFrame(LPCTSTR szFatherName);

	// װ�����ͻ�װ���Ի���ӿ�
	INT16 GetEquipDisplayPos(INT16 n16EquipPos);

	//--��ʾtips�õ���ʱ�����
	DWORD GetDay(DWORD& dwTimeDiff)
	{
		return dwTimeDiff / dwOneDay;
	}
	DWORD GetHour(DWORD& dwTimeDiff, DWORD& dwDay)
	{
		return (dwTimeDiff - (dwDay*dwOneDay)) / dwOneHour;
	}
	DWORD GetMin(DWORD& dwTimeDiff, DWORD& dwDay, DWORD& dwHour)
	{
		return (dwTimeDiff - (dwHour*dwOneHour) - (dwDay*dwOneDay)) / 60;
	}

private:
	TSFPTrunk<MallMgr>					m_Trunk;
	TObjRef<Util>						m_pUtil;
	TObjRef<NetCmdMgr>					m_pCmdMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
		
	MallFrame*							m_pMallFrame;
    YuanbaoFrame*                       m_pYuanbaoFrame;
	GroupBuyFrame*						m_pGroupBuyFrame;

	map<DWORD, tagMallItemProto>		m_mapMallItemEx;			//ȫ���Ǵ����Ʒ
	map<DWORD, tagMallPackProto>		m_mapPackItemEx;			//ȫ�������Ʒ

	TMap<DWORD, tagMallItemProto*>		m_mapMallItem;			
	TMap<DWORD, tagMallPackProto*>		m_mapPackItem;	
	tagMallItemProto*					m_pProItem[MAX_PromoteItem];//�Ƽ���Ʒ
	tagMallFreeItem						m_MallFreeItem;				//�����ȡ����Ʒ
	DWORD								m_dwFreeGetTime;			//�����ȡ��ʱ��
	DWORD								m_dwMallTime;				//�̳�ʱ��	
	DWORD								m_dwGroupBuyTime;			//�Ź�ʱ��
	BOOL								m_bOpen;

	//--����Ϊ�Ź�
	map<INT64, tagGroupPurchase>		m_mapGgroupPur;				//�Ź�������(���̳���ƷTypdIDΪKey)
	INT									m_nGroupPurExp;				//�����Ź�ָ��


	static const DWORD					dwOneDay  = 24 * 60 * 60;
	static const DWORD					dwOneHour = 60 * 60;
};