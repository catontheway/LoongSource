#pragma once
#include "SkillItem.h"
struct tagUpdateLPSkillEvent;

const INT MAX_SKILL_BOOK_NUM = 16;


class SkillBookFrame :
	public GameFrame
{
public:
	SkillBookFrame();
	~SkillBookFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	
	// �����ܽ����
	VOID OnSkillBookOpen();

private:
	// ����GUI��Ϣ
	DWORD EventHandler(tagGUIEvent* pEvent);

	// ������Ϸ�¼�
	DWORD OnUpdateLPSkillEvent(tagUpdateLPSkillEvent* pGameEvent);
	DWORD OnWeaponSwap(tagGameEvent* pGameEvent);
	

	// �Ƿ��ǵ�ǰҳ
	BOOL IsCurPage(INT nIndex){return nIndex >= m_nCurPage*MAX_SKILL_BOOK_NUM 
		&& nIndex < (m_nCurPage+1)*MAX_SKILL_BOOK_NUM;}
	// ���õ�ǰҳ��
	VOID SetCurPage(INT nPage);
	// ����ҳ�¼�
	VOID OnPageUp(BOOL bUpPage);

	// ˢ�µ�ǰҳ
	VOID ReflashCurPage();
private:

	TSFPTrunk<SkillBookFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	GUIWnd*						m_pWnd;
	GUIStatic*					m_pStcPage;
	GUIButton*					m_pBtnPageUp;
	GUIButton*					m_pBtnPageDown;

	SkillItem					m_SkillItem[MAX_SKILL_BOOK_NUM];

	TList<DWORD>				m_skillList;//--�����б���
	TList<DWORD>::TListIterator m_skillIter;//--�����б������

	INT							m_nCurPage;

};
