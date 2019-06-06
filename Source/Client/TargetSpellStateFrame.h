#pragma once

class ActiveStatic;
class ActiveProgress;
struct tagRoleSpellStartEvent;
struct tagRoleSpellStopEvent;
struct tagRolePickEvent;

/** \class TargetSpellStateFrame
	\brief Ŀ������״̬����
*/
class TargetSpellStateFrame : public GameFrame
{
public:
	TargetSpellStateFrame();
	~TargetSpellStateFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
private:
	VOID SafeDestroyUI();
	VOID Close();

	/** \��Ϸ�¼���Ӧ
	*/
	DWORD OnRoleSpellStartEvent(tagRoleSpellStartEvent* pEvent);
	DWORD OnRoleSpellStopEvent(tagRoleSpellStopEvent* pEvent);
	DWORD OnRolePickEvent(tagRolePickEvent* pEvent);
private:
	TSFPTrunk<TargetSpellStateFrame>	m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	GUIWnd*                             m_pWnd;
	ActiveStatic*						m_pIcon;			// ͼ��
	GUIStatic*							m_pName;			// ����
	ActiveProgress*						m_pTimePro;			// ʱ�����
	DWORD                               m_dwInterruptTime;  // ����ϵ�ʱ��
	DWORD                               m_dwRoleID;         // ���浱ǰ��ʾ�Ľ�ɫID
};