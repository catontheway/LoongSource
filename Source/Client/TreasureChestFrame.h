#pragma once
#include "..\WorldDefine\msg_digbao.h"

class IconStatic;
class TreasureChestFrame : 
	public GameFrame
{	
public:
	TreasureChestFrame( void );
	virtual ~TreasureChestFrame( void );
	virtual BOOL Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

public:
	static const int					sci_ItemNum = 16;	

private:
	BOOL EventHandler(tagGUIEvent* pEvent);

	// ��ʼ���������
	void InitTreasureChestUI();
	void RefreshUI();
	void SetTreasureChestUI();

	//--��Ϸ�¼�
	DWORD OnEOSAaginEvent(tagMsgBoxEvent* pGameEvent);

	//--������Ϣ
	DWORD OnNS_TreasureChest(tagNS_TreasureChest* pMsg, DWORD dwParam);
	DWORD OnNS_StopTreasureChest(tagNS_StopTreasureChest* pMsg, DWORD dwParam);
	DWORD OnNS_AgainTreasureChest(tagNS_AgainTreasureChest* pMsg, DWORD dwParam);
	DWORD OnNS_ChestGetItem(tagNS_ChestGetItem* pMsg, DWORD dwParam);

	//--����״̬
	enum EOperateState
	{
		EOperateState_NULL		= 0,
		EOperateState_Begin,
		EOperateState_Stop,
		EOperateState_Again,
	};
	void SetOperateState( EOperateState eState );
	void SetCaption(DWORD dwChestTypeID);
	void SendStopTreasureMsg();
	void SendGetTreasureMsg();

private:
	
	TSFPTrunk<TreasureChestFrame>		m_Trunk;
	TObjRef<Util>						m_pUtil;
	TObjRef<NetCmdMgr>					m_pCmdMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<GameFrameMgr>				m_pFrameMgr;

	GUIWnd*								m_pWnd;
	GUIPatch*							m_pPatWaption;
	GUIButton*							m_pBtnMFunction;
	GUIButton*							m_pBtnAgain;
	IconStatic*							m_pStcAuto[sci_ItemNum];
	IconStatic*							m_pStcItem[sci_ItemNum];
	IconStatic*							m_pLastAuto;

	EOperateState						m_eOperateState;				//����״̬
	DWORD								m_dwRunTime;
	DWORD								m_dwStartTime;					//��ʼʱ��(1���Ӻ�ֹͣ)
	DWORD								m_dwStopTime;					//ֹͣ��ʱ��
	DWORD								m_dwItemTypeID;					//��õ���Ʒ
	INT									m_dwStopCount;					//ֹͣ����
	vector<pair<DWORD,INT>>				m_vecItem;						//���������Ʒ
	
};