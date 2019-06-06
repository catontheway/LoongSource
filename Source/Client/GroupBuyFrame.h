#pragma once
#include "GameClientDefine.h"
#include "..\WorldDefine\mall_define.h"
#include "..\WorldDefine\msg_mall.h"
#define Max_GroupBuy_Size 3

#define Max_GBInfo_PerPage 6
#define Max_Item_Pro 5

class MallButton;
class IconStatic;

/*
	class GroupBuyFrame
	brief �Ź�
*/
class GroupBuyFrame : public GameFrame
{
public:
	enum EGroupBuy
	{
		EGroupBuy_NULL			= 0,
		EGroupBuy_Purchase,
		EGroupBuy_Info,
	};
public:
	GroupBuyFrame(void);
	~GroupBuyFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	// ��ʾ�Ź�����
	void ShowWnd(EGroupBuy eType=EGroupBuy_NULL);

	// ��ʾ��ǰҳ���Ź���Ϣ
	void ShowCurPageGroupBuyInfo(INT nPage);

	// ˢ���Ź���Ϣ
	void RefreshGroupBuyInfo();

private:
	DWORD EventHandler2GroupBuy(tagGUIEvent* pEvent);
	DWORD EventHandler2GroupInfo(tagGUIEvent* pEvent);

	//--������Ϣ
	DWORD OnNS_LaunchGroupPurchase(tagNS_LaunchGroupPurchase* pMsg, DWORD dwParam); 
	DWORD OnNS_Getparticipators(tagNS_Getparticipators* pMsg, DWORD dwParam);
	DWORD OnNS_RespondGroupPurchase(tagNS_RespondGroupPurchase* pMsg, DWORD dwParam);
	DWORD OnNS_RespondBroadCast(tagNS_RespondBroadCast* pMsg, DWORD dwParam);

	//--���ؿؼ�
	void HideTotalGroupBuyCtrl();
	void HideTotalGroupBuyInfoCtrl();

	// �����Ź�
	void HandlerGroupLauncher();

	//--�������ж�
	bool JudgePromotePro(const tagMallItemProtoBase* pItem);
	bool JudgeGroupBuyPro(const tagMallItemProtoBase* pItem);
	bool JudgePresentPro(const tagMallItemProtoBase* pItem);
	bool JudgeNumPro(const tagMallItemProtoBase* pItem);
	bool JudgeExchangePro(const tagMallItemProtoBase* pItem);

	//--��ҳ
	void PageDown();
	void PageUp();

	//ˢ�������Ź���Ʒ��ѡ���
	void RefreshGroupBuyItemUI();

	// ��ʾ��Ʒ������tips
	void ShowMallItemTips(int nPos, int nProPos);
	void ShowPromoteProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos);
	void ShowGroupBuyProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos);
	void ShowPresentProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos);
	void ShowNumProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos);
	void ShowExchangeProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos);

	//��Ӧ�Ź��ж�
	bool JudgeResponseGroupBuy();

	//�г��Ź���Ʒ������Ӧ��
	void ListResponse();

private:
	TSFPTrunk<GroupBuyFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;

	//--�����Ź�
	GUIWnd*							m_pWndGBuy;
	GUIButton*						m_pBtnLauncher;
	GUIButton*						m_pBtnCancel;
	MallButton*						m_pStcItem;											//�����Ź�����Ʒ
	GUIStatic*						m_pStcHoldTime;										//�Ź�����ʱ��
	GUIStatic*						m_pStcGBBack[Max_GroupBuy_Size];					//��ģ����
	GUIStatic*						m_pStcResponse[Max_GroupBuy_Size];					//��Ӧ����
	GUIStatic*						m_pStcDiscount[Max_GroupBuy_Size];					//�ۿ�ˤ
	GUIStatic*						m_pStcPrice[Max_GroupBuy_Size];						//�۸�
	GUIPushButton*					m_pPBtnScale[Max_GroupBuy_Size];					//�Ź���ģ
	GUIStatic*						m_pStcScaleBack[Max_GroupBuy_Size];					//�Ź���ģ����
	
	//--�Ź���Ϣ
	GUIWnd*							m_pWndGInfo;
	GUIButton*						m_pBtnPageUp;
	GUIButton*						m_pBtnPageDown;
	GUIStatic*						m_pStcPage;											//��ǰҳ
	GUIButton*						m_pBtnResponse;										//��Ӧ
	GUIStatic*						m_pBtnGroupBuyExp;									//�����Ź�ָ��
	GUIStatic*						m_pStcInfoBack[Max_GBInfo_PerPage];					//��Ʒ�ı���
	IconStatic*						m_pStcInfoSele[Max_GBInfo_PerPage];					//�Ź���Ʒ��ѡ���
	MallButton*						m_pBtnBGItemInfo[Max_GBInfo_PerPage];				//ÿҳ��ʾ���Ź���Ϣ����
	GUIStatic*						m_pStcItemName[Max_GBInfo_PerPage];					//��Ʒ����
	GUIStatic*						m_pStcItemPrice[Max_GBInfo_PerPage];				//�۸�
	IconStatic*						m_pStcGBItemState[Max_GBInfo_PerPage][Max_Item_Pro];//ÿ����Ʒ������
	GUIListBox*						m_pLBoxResponse;									//������Ӧ�б�
	GUIStatic*						m_pStcResBack[Max_GBInfo_PerPage];					//��Ӧ����
	GUIStatic*						m_pStcLauncherName[Max_GBInfo_PerPage];				//������
	GUIStatic*						m_pStcResCount[Max_GBInfo_PerPage];					//��Ӧ����
	GUIStatic*						m_pStcResTime[Max_GBInfo_PerPage];					//��Ӧʣ��ʱ��


	INT								m_nInfoCurPage;										//�Ź���Ϣ��ǰҳ
	INT								m_nInfoMaxPage;										//�Ź���Ϣ��ҳ��
	EGPSCOPE						m_eScope;											//�Ź�����
	DWORD							m_dwGLauncherItemID;								//�����Ź���ƷID
		
	DWORD							m_dwUpdateTime;										//�����������ʱ����
	INT								m_nCurPage;											//��Ӧ�Ź���ǰҳ
	INT								m_nMaxPage;											//��Ӧ�Ź���ҳ��

	BtnData							m_seleDate;											//��¼ѡ����Ź���Ʒ

	TList<DWORD>					m_listRespons;										//��Ӧ���б�
};