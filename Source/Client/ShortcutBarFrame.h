#pragma once
#include ".\WorldFrame.h"
class ButtonEx;
class ShortcutButton;
class DragBtn;

struct tagUpdateLPItemEvent;
struct tagUpdateLPSkillEvent;
struct tagRoleGetNameEvent;
struct tagShortcutCacheEvent;
struct tagRolePickEvent;
struct tagNS_VirginOnline;
//-----------------------------------------------------------------------------
// !class ShortcutBarFrame.h
// !brief ��������
//-----------------------------------------------------------------------------
class ShortcutBarFrame :
	public GameFrame
{
public:
	ShortcutBarFrame(void);
	~ShortcutBarFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual VOID Update();

	//�ؼ��¼�������
	DWORD EventHandler(tagGUIEvent* pEvent);

	//���ؿ�ݰ�ť
	BOOL LinkShortcutBtn(TCHAR* szName, int nBar);

	//������չ��ݰ�ť
	void CreateShortcutBtnEx(char* szName);

	//������ˢ��
	void RefreshRoleExp();

	//���ָ���Ŀ�ݰ�ť
	ShortcutButton* GetShortcutButtion(int group, int num);

	void ReflashCDTime();
	void SetBtnEnable();

	// ������������	
	void SaveBtnData();

private:
    // ��ɫ���ε�¼���ÿ�ݰ�ť
    DWORD OnNS_VirginOnline(tagNS_VirginOnline* pMsg, DWORD);

	//ˢ����ͨ����ͼƬ
	DWORD OnWeaponSwap(tagGameEvent* pEvent);

	//��ʾ/���ؿ�ݰ�ť
	void ShowBar(int bar);
	void HideBar(int bar);

	DWORD OnKey(DWORD dwID, BOOL bDown);
	DWORD OnKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnKeyEx(DWORD dwID, BOOL bDown);
	DWORD OnKeyExChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnKey2Move(DWORD dwID, BOOL bDown);
	DWORD OnKey2MoveChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnKey2Move1(DWORD dwID, BOOL bDown);
	DWORD OnKey2MoveChange1(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnRolePickEvent(tagRolePickEvent* pEvent);
	DWORD OnInitLPAttEvent(tagGameEvent* pEvent);
	DWORD OnKeyUpDown(DWORD dwID, BOOL bDown);
	DWORD OnKey2MoveOpenOrClose(DWORD dwID, BOOL bDown);

	// ��DIK�õ������ַ�����
	LPCTSTR GetKeyName(DIK key);

	//ע�����
	void RegisterKeyBoard();

	//���°�ť�Ŀ�������
	void UpdateBtnEnable();		

	// �뱳��ͬ������
	DWORD OnUpdateLPItemEvent(tagUpdateLPItemEvent* pEvent);
	DWORD OnUpdateLPSkillEvent(tagUpdateLPSkillEvent* pEvent);
	DWORD OnInitedSkillEvent(tagGameEvent* pEvent);

	// 
	DWORD OnShortcutCacheEvent(tagShortcutCacheEvent* pGameEvent);

	// ��ȡ���������	
	void LoadBtnData(DWORD dwLocalID);
	void ReflashShortcutBtn();

	DWORD OnRecvNetRoleGetName( tagRoleGetNameEvent* pMsg);

	void ShowMoveShortcutBar(BOOL bShow);
	void ShowMove1ShortcutBar(BOOL bShow);
private:

	TSFPTrunk<ShortcutBarFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameInputMap>		m_pKeyMap;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	

	static const int			BAR_COUNT = 10;					//�����������
	static const int			BAR_BUTTON_COUNT = 8;			//ÿ���ݼ�����
	static const int			BAR_TITLE_COUNT = 12;
	static const int			BAR_COUNT_TIPS = 4;
	static TCHAR*				m_szAccArray[BAR_TITLE_COUNT+BAR_COUNT_TIPS];
	static int					m_nAccArray[ BAR_TITLE_COUNT+BAR_COUNT_TIPS ];
	static TCHAR*				m_szMoveArray[BAR_BUTTON_COUNT];
	static byte					m_byMoveArray[BAR_BUTTON_COUNT];
	static TCHAR*				m_szMoveArray1[BAR_BUTTON_COUNT];
	static int					m_nMoveArray1[BAR_BUTTON_COUNT];
	static TCHAR*				m_szBarGroup[BAR_COUNT_TIPS];
	static byte					m_byBarGroup[BAR_COUNT_TIPS];
	static TCHAR*				m_szKeyUpDown[2];
	static int					m_nKeyUpDown[2];
	static TCHAR*				m_szKeyMoveOC[2];
	static int					m_nKeyMoveOC[2];
	int							m_nBarGroupCount;				//��¼��ǰ��������
	FLOAT						m_fCurrentExp;					//��ǰ����
	DWORD						m_LastSuccSkill;

	GUIWnd*						m_pWnd;
	GUIStatic*					m_pPthBack;						//���������
	GUIStatic*					m_pPthShortcutBack;				//�������ť����
	GUIStatic*					m_pPthExpBack;					//����������
	GUIProgress*				m_pProExp;						//������
	GUIStatic*					m_pStcChoose;					//���������ʾF1-F4
	GUIButton*					m_pBtnMoveDn;					//�ƶ���������л������л���ť
	GUIButton*					m_pBtnMoveUp;					//�ƶ���������л������л���ť
	GUIButton*					m_pBtnDn;						//��������л������л���ť
	GUIButton*					m_pBtnUp;						//��������л������л���ť

	ShortcutButton*				m_pShortcutBtn[BAR_COUNT][BAR_BUTTON_COUNT];		//4���ݰ�ť

	GUIWnd*						m_pWnd_Move1;
	GUIWnd*						m_pWnd_Move2;

	INT							m_nCurExp;						//��ǰ����
	INT							m_nCurLev;						//��ǰ�ȼ�
	INT							m_nCurTotalExp;					//��һ����Ҫ�ľ���
	INT							m_nPctNExp;						//N%������
	BOOL						m_bInitShortcut;				//�Ƿ��ȡ������

	bool						m_bLPAttIsOK;

public:
	ShortcutButton*				m_pLastPress;					//�ϴΰ��Ŀ�ݼ�

};
