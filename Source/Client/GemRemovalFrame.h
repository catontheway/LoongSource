#pragma once
#include "gameframe.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/msg_compose.h"

struct tagNS_GetGemRemovalInfo;

class Item;
class ComposeStatic;
class IconStatic;
class IconStaticHL;

class GemRemovalFrame : public GameFrame
{

public:
	GemRemovalFrame(void);
	~GemRemovalFrame(void);

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID OnEvent( tagGameEvent* pEvent );

	void SetNPCID( DWORD dwNPCID ){ m_dwNPCId = dwNPCID; }
	void ShowWnd( BOOL bShow )
	{
		if( P_VALID( m_pWnd ) )
		{
			m_pWnd->SetInvisible( !bShow );
			m_pWnd->SetRefresh( TRUE );
		}
	}

private:
	BOOL EventHandler( tagGUIEvent* pEvent );
	
	/** ��ʾȷ�Ͻ���
	*/
	VOID ShowSureWnd();
	
	/** �õ�������Ҫ�ı�ʯ���Ͳ��������Ϣ
	*/
	INT16 GetWantGemNum( tagNC_GemRemoval& evt );

	/** ���ͱ�ʯ�������֮ǰ-���
	*/
	bool SubmitCheck();

	/** ������пؼ���������
	*/
	void ClearAll();

	void DisplayGoldCost( INT nGoldCost,INT	nSilver );

	struct tagGemRmSymbolInfo
	{
		INT64	n64ItemID;		//��ʯ�������64λID
		INT		nLevel;			//��ʯ������ļ���
		INT		nQuantity;		//��ʯ�����������

		tagGemRmSymbolInfo()
		{
			n64ItemID = GT_INVALID;
			nLevel = GT_INVALID;
			nQuantity = 0;
		}
	};

	/** \���ݱ����еı�ʯ�������Ϣ��ͳ�Ƴ�������Ҫ�ĸ�ʽ
	*/
	VOID CensusGemRmSignal( std::list<Item*>& lst );

	//-------�����¼�������-------------------
	DWORD OnNS_GetRemovalInfo( tagNS_GetGemRemovalInfo* pMsg, LPVOID pPrama );
	DWORD OnNS_GetRemoval( tagNS_GemRemoval* pMsg, LPVOID pParam );

private:
	TSFPTrunk<GemRemovalFrame>			m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	GUIWnd*				m_pWnd;
	GUIButton*			m_pBtnClose;							//�رհ�ť
	GUIButton*			m_pBtnOk;								//ȷ����ť
	GUIButton*			m_pBtnCancel;							//ȡ����ť
	ComposeStatic*			m_pIconEquip;						//��Ҫ�������ʯ��װ��
	IconStaticHL*		m_pIconGems[MAX_EQUIPHOLE_NUM];			//��Ҫ�õ��ı�ʯ��ͼƬ
	IconStatic*			m_pIconSymbols[MAX_EQUIPHOLE_NUM];		//��Ҫ���ĵı�ʯ�����
	GUIStatic*			m_pStcGold;								//��
	GUIStatic*			m_pStcSilver;							//��

	GUIWnd*				m_pWndSure;								//����ȷ�Ͻ���
	GUIButton*			m_pBtnSureClose;						//ȷ�Ͻ���Ĺرհ�ť
	GUIButton*			m_pBtnSureOk;							//ȷ�Ͻ����OK
	GUIButton*			m_pBtnSureCancel;						//ȷ�Ͻ����ȡ����ť
	GUIListBox*			m_plbGems;								//ȷ�Ͻ�����ı�ʯ�б��
	GUIListBox*			m_plbSymbols;							//ȷ�Ͻ�����������б��
	GUIStatic*			m_pStcSureGold;							//ȷ�Ͻ���-��
	GUIStatic*			m_pStcSureSilver;						//ȷ�Ͻ���-��


	DWORD				m_Gems[MAX_EQUIPHOLE_NUM];				//����ı�ʯ��typeid
	std::list<tagGemRmSymbolInfo>		m_lstGmRmSymbolInfo;

	INT64				m_Symbols[MAX_EQUIPHOLE_NUM];			//���ĵı�ʯ�������64λID
	DWORD				m_n64GoldCost;							//��Ҫ���ĵĽ�
	INT					m_nSilverCost;							//��Ҫ���ĵ���


	enum GemRemovalState
	{
		EGRS_Operate,
		EGRS_SendMsg,
		EGRS_ReceiveMsg
	};

	GemRemovalState		m_CurState;								//��ǰ�Ĳ���״̬
	INT64				m_n64Src;								//Ҫ�������Ʒ
	DWORD				m_dwNPCId;								//npc id
	DWORD				m_dwElapseTime;							//��ʱ������鵽NPC�ľ���
};
