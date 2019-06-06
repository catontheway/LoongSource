#pragma once
#include "gameframe.h"

/** \class PocketEncryptFrame
	\brief ���Ҽ��ܽ���
*/
class PocketEncryptFrame :
	public GameFrame
{
public:
	PocketEncryptFrame(void);
	~PocketEncryptFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();

	virtual BOOL ReloadUI();

	virtual BOOL EscCancel();

private:

	DWORD EventHandler(tagGUIEvent* pEvent);

private:
	TSFPTrunk<PocketEncryptFrame>	m_Trunk;
	TObjRef<NetCmdMgr>				m_pNetCmd;
	TObjRef<NetSession>				m_pSession;
	TObjRef<GUISystem>				m_pGUI;
	
	GUIWnd*							m_pWnd;
	GUIWnd*							m_pWndCaption;
	GUIButton*						m_pBtnSetup;		// ����
	GUIButton*						m_pBtnModify;		// �޸�
	GUIButton*						m_pBtnCancel;		// ɾ��
	GUIButton*						m_pBtnExit;			// �뿪

};
