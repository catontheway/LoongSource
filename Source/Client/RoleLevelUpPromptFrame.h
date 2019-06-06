#pragma once
#include "gameframe.h"
#include "QuestMgr.h"

class RoleLevelUpPromptFrame : public GameFrame
{
public:
	RoleLevelUpPromptFrame( void );
	~RoleLevelUpPromptFrame( void );

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

	VOID LevelUpTo( DWORD dwLevel );

private:
	BOOL EventHandler( tagGUIEvent* pEvent );
	VOID Navigate( tagGUIEvent* pEvent );
	
	VOID ShowWnd( BOOL bShow );

private:
	TSFPTrunk<RoleLevelUpPromptFrame>		m_Trunk;
	TObjRef<GUISystem>						m_pGUI;
	TObjRef<Util>							m_pUtil;
	TObjRef<GameFrameMgr>					m_pMgr;

	vector<tagIDLink>						m_vecIdLinks;
	vector<tagPosLink>						m_vecPosLinks;

	GUIWnd*									m_pWnd;					//������
	GUIButton*								m_pOk;					//ȷ��
	GUIStaticEx*							m_pStcContent;			//��ʾ���ݵ����

	tstring									m_strDesc;				//����ʱ��Ҫ��ʾ����Ϣ
};
