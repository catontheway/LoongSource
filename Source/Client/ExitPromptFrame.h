#pragma once
#include "gameframe.h"
#include "QuestMgr.h"

struct tagNS_ExitGamePrompt;

class ExitPromptFrame : public GameFrame
{
public:
	ExitPromptFrame(void);
	~ExitPromptFrame(void);

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

private:
	BOOL GUIEventHandler(tagGUIEvent* pEvent);
	VOID SafeDestroyUI();
	VOID OnLogOut( const int nLogType );

	//���������¼���Ϣ���õ���Ҫ��ʾ��Щ����
	DWORD OnNS_ExitGamePrompt( tagNS_ExitGamePrompt* pNetCmd, DWORD dwParams );

	VOID Navigate( tagGUIEvent* pEvent,int nWndNum );
private:
	TSFPTrunk<ExitPromptFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;

	vector<tagIDLink>				m_vecIdLinks[2];
	vector<tagPosLink>				m_vecPosLinks[2];

	GUIWnd*							m_pWnd;
	GUIStaticEx*					m_stcPrompts[2];		// ɱ����ʾ
	GUIButton*						m_pBtnContinue;			// ������Ϸ
	GUIButton*						m_pBtnQuit;				// �˳���Ϸ
	GUIButton*						m_pBtnClose;			// �رհ�ť

	DWORD							m_dwTime;				// ���
};
