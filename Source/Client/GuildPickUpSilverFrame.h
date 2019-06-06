#pragma once

/*************************
\!brief��ȡ��������
*************************/

struct tagRefreshPickUpSilverEvent;
struct tagCommerceInfo;

class GuildPickUpSilverFrame : public GameFrame
{
public:
	GuildPickUpSilverFrame();
	~GuildPickUpSilverFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent *pEvent);
	DWORD OnEventPickUpSilverRefresh(tagRefreshPickUpSilverEvent *pEvent);

private:
	TSFPTrunk<GuildPickUpSilverFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;
	
	GUIWnd *m_pWnd;
	GUIStatic *m_pStcGrade;				// �ȼ�
	GUIStatic *m_pStcGold;				// ��
	GUIStatic *m_pStcSilver;			// ��
	GUIStatic *m_pStcPickUpSilver;		// ��ȡ����
	GUIStatic *m_pStcAimSilver;			// Ŀ��
	
	GUIButton *m_pBtnAccept;			// ����
	GUIButton *m_pBtnCancel;			// ȡ��

	const tagCommerceInfo *m_pCurrentCommerceInfo;
};