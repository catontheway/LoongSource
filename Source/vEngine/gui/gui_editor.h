//-----------------------------------------------------------------------------
//!\file gui_editor.h
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2007-12-31
//!
//!\brief ����ϵͳ �༭��
//-----------------------------------------------------------------------------
#pragma once

namespace vEngine {
//-----------------------------------------------------------------------------
// editor
//-----------------------------------------------------------------------------
class VENGINE_API GUIEditor
{
public:
	BOOL Init(GUISystem* pSystem, BOOL bInGame=FALSE);
	VOID Destroy();
	VOID Render();

	// �ⲿͨ���˺�����XUI Editorϵͳ��������Ϣ, ����TRUE�����Ѿ�����
	BOOL OnWindowsMsg(DWORD dwMsg, DWORD dwParam1, DWORD dwParam2);
	
	GUIEditor();
	~GUIEditor();

	// ��InputMap���õ���ʾ����
	DWORD Switch(DWORD dwID, BOOL bDown);
	TSFPTrunk<GUIEditor>& GetTrunk() { return m_Trunk; }

private:
	TSFPTrunk<GUIEditor>		m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<WindowWrap>			m_pWindow;
	TObjRef<DiscIOMgr>			m_pDiscIO;

	BOOL						m_bInGame;	// �Ƿ�����Ϸ��
	HWND						m_hWnd;

	GUISystem*					m_pSystem;
	DWORD						m_dwActiveWndID;
	GUIWnd*						m_pActiveWnd;
	GUIWnd*						m_pEditor;

	INT							m_nActiveX;
	INT							m_nActiveY;
	TCHAR						m_szFileName[MAX_PATH];
	XmlElement					m_CopyElement;	// ����Ctrl+C
	XmlElement					m_DragElement;	// ����Drag
	tagPoint					m_ptLastMouseClickView;	// ���һ���������λ��

	tagRect						m_rcActiveOut;
	tagRect						m_rcActiveIn;
	BOOL						m_bInResize;
	BOOL						m_bResize;
	BOOL						m_bChanges;		// �Ƿ���Ҫ����


	struct tagGUIEditorAction
	{
		XmlElement		OriginEle;	// ԭ�пؼ�������
		tstring	strFather;	// ԭ�пؼ��ĸ��ؼ�ȫ��
		tstring	strNew;		// �����ؼ���ȫ��
	};
	std::list<tagGUIEditorAction*>	m_listUndoAction;
	std::list<tagGUIEditorAction*>	m_listRedoAction;

	// ��ʼ����ʱ�򴴽����ֿؼ� 
	VOID CreateControls();
	// ���ܲ�����ť��Ϣ
	BOOL EventHandler(tagGUIEvent* pEvent);

	// ��FatherWnd�Լ��������пؼ����ּ���Tree�ؼ�
	VOID AddControlsToTree(GUITree* pTree, LPCTSTR szName, GUIWnd* pWnd);

	// �ع�ָ���ؼ������ӿؼ�
	VOID Rebuild(GUIWnd* pWnd, LPCTSTR szKey, LPCTSTR szValue);

	// ��¼ָ���ؼ������ӿؼ�
	BOOL Record(XmlElement* pXmlElement, GUIWnd* pWnd);

	// ���õ�ǰ����ؼ�
	VOID SetActiveWnd(GUIWnd* pWnd);

	// ˢ�¿ؼ���,ͬʱ���õ�ǰ����ؼ�
	VOID RefreshControlsTree(GUIWnd* pActiveWnd);

	// ���浽����
	BOOL Save(LPCTSTR szFileName);
	// �Ӵ��̶�ȡ
	BOOL Load(LPCTSTR szFileName, bool bClear);
	// ������пؼ�
	BOOL Clear();

	// ��¼������Ϊ
	VOID RecordAction(XmlElement* pOriginEle, LPCTSTR szFather, LPCTSTR szNew);
	
	VOID Undo();	
	VOID Redo();
	VOID Copy();
	VOID Paste();
	VOID Delete();

	VOID Open();
	VOID Add();
	VOID SaveAs();
};

}