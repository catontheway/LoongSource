#pragma once

enum MallGiftFlag
{
	MGF_NULL	= 0,
	MGF_OK		= 1,	// ȷ��
	MGF_Cancel	= 2,	// ȡ��
};

struct tagMsgMallGiftEvent : public tagGameEvent
{
	tagMsgMallGiftEvent(LPCTSTR szEventName, GameFrame* pSenderFrame):tagGameEvent(szEventName, pSenderFrame){}
	MallGiftFlag	eRet;
	tstring			strName;			//�������
	INT16			n16Num;				//������Ʒ����
	tstring			strWord;			//����
	DWORD			dwTime;
};

class MallGiftFrame : public GameFrame
{
public:
	MallGiftFrame(void);
	~MallGiftFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	void SetBuyNum();
	void SetMaxBuyNum(INT nMaxNum);
	void ShowGiftFrame();
private:
	// ����GUI��Ϣ
	VOID EventHandler(tagGUIEvent* pEvent);

private:

	TSFPTrunk<MallGiftFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;		
	GUIButton*					m_pBtnOK;
	GUIButton*					m_pBtnCancel;
	GUIEditBox*					m_pEdtName;					//�������
	GUIEditBox*					m_pEdtNum;					//����
	GUIEditBox*					m_pEditWord;				//����
};