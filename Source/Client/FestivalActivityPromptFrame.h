#pragma once
#include "gameframe.h"
#include "ActivityMgr.h"
#include "QuestMgr.h"

/**���ջ������ʾ
*/
class FestivalActivityPromptFrame : public GameFrame
{

public:
	FestivalActivityPromptFrame( void );
	~FestivalActivityPromptFrame( void );

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

	BOOL CanDestroy();

private:
	BOOL EventHandler( tagGUIEvent* pEvent );
	VOID Navigate( tagGUIEvent* pEvent );
	VOID ShowWnd( BOOL bShow );
	VOID Prompt();

	/**��Ҫ������ʾ�Ľ��ջ �Ĺ�����
	*/
	bool NeedPrompt4FestivalActivity( const tagActivityProtoData &activity );

private:
	TSFPTrunk<FestivalActivityPromptFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pMgr;

	std::list<tagActivityProtoData>		m_needPromptActivities;	//��Ҫ��ʾ�ļ��ջ
	tagActivityProtoData			m_curActivityProtoData;	//��ǰ������Ľ��ջ��Ϣ
	vector<tagIDLink>				m_vecIdLinks;			//NPC����������
	vector<tagPosLink>				m_vecPosLinks;			//λ�õ���������

	GUIWnd*							m_pWnd;					//������
	GUIButton*						m_pOk;					//ȷ��
	GUIStaticEx*					m_pStcContent;			//��ʾ���ݵ����
	GUIScrollBar*					m_pScrollBar;			//������

	BOOL							m_bCanDestroy;			//�Ƿ�����ٴ�ģ��

};
