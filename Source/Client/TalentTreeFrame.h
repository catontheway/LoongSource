#pragma once
#include "TalentTree.h"
#include "..\WorldDefine\RoleDefine.h"

struct tagUpdateLPSkillEvent;
struct tagUpdateLPLevel;
struct tagUpdateLPAttEvent;
struct tagUpdateLPTalentPoint;

/** class	TalentTreeFrame
	brief	�츳������
*/
class TalentTreeFrame :
	public GameFrame
{
public:
	TalentTreeFrame();
	~TalentTreeFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();

	// �������ʴ�
	VOID OnTalentTreeOpen();

private:
	// ����GUI��Ϣ
	DWORD EventHandler(tagGUIEvent* pEvent);
	
	// ������Ϸ�¼�
	DWORD OnUpdateLPSkillEvent(tagUpdateLPSkillEvent* pGameEvent);
	DWORD OnUpdateLPLevel(tagUpdateLPLevel* pGameEvent);
	DWORD OnUpdateLPAttEvent(tagUpdateLPAttEvent* pGameEvent);
	DWORD OnUpdateLPTalentPoint(tagUpdateLPTalentPoint* pGameEvent);
	DWORD OnEventClearTalent(tagGameEvent* pGameEvent);

	// ���õ�ǰ��Ͷ��
	VOID SetBidden(INT nValue);
	// ���õ�ǰ��Ͷ��
	VOID SetAvail(INT nValue);
private:

	TSFPTrunk<TalentTreeFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>		m_pSession;

	GUIWnd*						m_pWnd;
	GUIPushButton*				m_pPbnTalent[ETT_End];
	GUIStatic*					m_pStcAvail;
	GUIStatic*					m_pStcBidden;

	TalentTree					m_TalentTree;

	INT							m_nCurBidden;
	ETalentType					m_eCurTalent;
	INT							m_nCurAvail;
	INT							m_nCurTalentNum;

};
