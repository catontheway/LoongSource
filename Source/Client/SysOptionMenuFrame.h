#pragma once

/** \class SysOptionMenuFrame
    \brief ϵͳѡ������
*/
class SysOptionMenuFrame : public GameFrame
{
public:
	SysOptionMenuFrame();
	~SysOptionMenuFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
private:
	BOOL GUIEventHandler(tagGUIEvent* pEvent);
	VOID SafeDestroyUI();
	VOID OnOpenGameSettings();
	VOID OnOpenInputSettings();
	VOID OnLogOut( const int nLogType );
private:
	TSFPTrunk<SysOptionMenuFrame>	m_Trunk;
	TObjRef<GUISystem>				m_pGUI;

	GUIWnd*							m_pWnd;						  // ϵͳѡ������
	GUIWnd*							m_pPanel;					  // �װ�
	GUIButton*						m_pBn_ResumeGame;             // ������Ϸ
	GUIButton*						m_pBn_GameSettings;           // ��Ϸ����
	GUIButton*						m_pBn_InputSettings;	      // �Զ����λ
	GUIButton*						m_pBn_ResumeRoleSelect;       // ���ؽ�ɫѡ��
	GUIButton*						m_pBn_ResumeLogin;            // �����ʺŵ�½
	GUIButton*						m_pBn_QuitGame;			      // �뿪��Ϸ
};

