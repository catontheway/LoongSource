//-----------------------------------------------------------------------------
//!\file lua_manager.h
//!\author Lyp
//!
//!\date 2008-01-07
//! last 2008-01-07
//!
//!\brief Lua�ű�����
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {

enum ELuaDebugState
{
	ELDS_Normal = 0,
	ELDS_StepOver,	// F10
	ELDS_StepInto,	// F11
};

class LuaScript;
class GUIEditBoxEx;
class GUIListBox;
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class VENGINE_API LuaManager
{
public:
	BOOL	Init(LPCSTR szVFS, BOOL bDebug);
	VOID	Update(float fElapsedSec);

    LuaScript* CreateScript(LPCSTR szVFS, LPCTSTR szFileName);
	BOOL DestroyScript(LPCTSTR szFileName);


	LuaScript* GetScript(DWORD dwID) { return m_mapScript.Peek(dwID); }
	INT	GetNumScripts() { return m_mapScript.Size(); }


	// debug ���
	BOOL IsDebug() { return m_bDebug; }
	VOID DebugMsg(LPCWSTR lpFormat, ...);
	BOOL EventHandler(tagGUIEvent* pEvent);
	
	BOOL SetEditScript(LuaScript* pScript);	// ���õ�ǰ���ڱ༭�Ľű�
	BOOL SetDebugScript(LuaScript* pScript);	// ���õ�ǰ���ڵ��ԵĽű�

	VOID SetDebugState(ELuaDebugState eState);
	ELuaDebugState GetDebugState() { return m_eDebugState; }
	GUIListBox* GetStackWnd() { return m_pWndStack; }
	GUIListBox* GetWatchWnd() { return m_pWndWatch; }

	VOID OnScriptStateChange(LuaScript* pScript);	// �ű�״̬�ı�

	LuaManager(void);
	~LuaManager(void);

	lua_State*		m_masterState;

private:
	TSFPTrunk<LuaManager>		m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<GUISystem>			m_pGUI;
	TMap<DWORD, LuaScript*>	m_mapScript;
	LuaScript*					m_pDebugScript;	// ��ǰ���ڵ��ԵĽű�
	LuaScript*					m_pEditScript;	// ��ǰ���ڱ༭�Ľű�

	BOOL						m_bDebug;	// �Ƿ����ģʽ
	ELuaDebugState				m_eDebugState;

	GUIStatic*					m_pWndCaption;
	GUIEditBoxEx*				m_pWndDebug;
	GUIListBox*				m_pWndFileList;
	GUIEditBoxEx*				m_pWndCode;
	GUIStatic*					m_pWndState;
	GUIStatic*					m_pWndCodeState;
	GUIListBox*				m_pWndStack;
	GUIListBox*				m_pWndWatch;
	XmlElement					m_XmlElement;
	

};

} // namespace vEngine{