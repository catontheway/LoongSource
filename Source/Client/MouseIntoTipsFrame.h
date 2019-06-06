#pragma once

class ToolTipStatic;
class MouseIntoTipsFrame : public GameFrame
{
public:
	MouseIntoTipsFrame(void);
	~MouseIntoTipsFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update(); 
	virtual BOOL ReloadUI();

	//��ָ��λ������ʾ��Ϣ
	VOID ShowTips(tagPoint ptPos, LPCTSTR szTip);

private:
	tstring GetMouseIntoRoleNameFont();
	tstring GetMouseIntoRoleOtherFont();

	tstring GetMouseIntoIsAttackColor(DWORD dwType);
	tstring GetMouseIntoMonsterColor(INT nIn);

	VOID ResetWindowPoint(tagPoint ptSize, tagPoint ptPos);

private:
	TSFPTrunk<MouseIntoTipsFrame>		m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<GUIRender>					m_pRender;

	GUIWnd*								m_pWnd;						// Tips����
	ToolTipStatic*						m_pStcTips;					// Tips������ʾ�ؼ�
	DWORD								m_dwMouseIntoID;			//���ָ���id
	tagPoint							m_clientPoint;								
};