//-----------------------------------------------------------------------------
//!\file input_map.h
//!\author Lyp
//!
//!\date 2008-02-27
//! last 2008-02-27
//!
//!\brief ��������ע��
//-----------------------------------------------------------------------------
#pragma once

namespace vEngine {
typedef DWORD (WINAPI* INPUTMAPEVENTHANDLE)(DWORD dwID, BOOL bDown);
typedef DWORD (WINAPI* INPUTMAPCHANGEEVENTHANDLE)(DWORD dwID, DWORD key1, DWORD key2);
//-----------------------------------------------------------------------------
//!\brief GUI��������ע��
//! ʹ������ INPUTMAPEVENTHANDLE �ĺ�����ע�ᰴ���ص��¼�
//! ʹ������ INPUTMAPCHANGEEVENTHANDLE �ĺ�����ע���ȼ��仯�ص��¼�
//-----------------------------------------------------------------------------
class VENGINE_API InputMap
{
public:
	InputMap();
	~InputMap();

	
	virtual BOOL LoadGUI(LPCSTR szVFS, LPCTSTR szWindowName, tagPoint& ptPos);
	virtual VOID UnloadGUI();
	virtual VOID Update();

	// ע������
	virtual BOOL Register(DWORD dwID, LPCTSTR szDesc, INPUTMAPEVENTHANDLE fpFunction, 
		DWORD key, BOOL bShow=TRUE, BOOL bEnableInEdit=FALSE, INT nUserData=0, 
		INPUTMAPCHANGEEVENTHANDLE fpChangeFunction=0);
	virtual BOOL Unregister(LPCTSTR szDesc);

	// ��ʱ���ð����Ƿ���Ч
	virtual BOOL SetEnable(LPCTSTR szDesc, BOOL bEnable);


	// ��DIK�õ������ַ�����
	LPCTSTR GetKeyName(DIK key);

protected:
	TSFPTrunk<InputMap>			m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<InputDX8>			m_pInput;
	TObjRef<Util>				m_pUtil;
	
	GUIWindow*					m_pWnd;
	GUIStatic*					m_pWndCaption;
	GUIListBox*					m_pWndList;
	GUIButton*					m_pWndOK;

	struct tagKeyMapInfo
	{
		DWORD		dwID;
		DWORD		dwKey;
		DWORD		dwOriginKey;
		DWORD		dwDefaultKey;
		tstring		strDesc;		// ����
		BOOL		bEnable;		// �Ƿ���Ч
		BOOL		bEnableInEdit;	// ��������Editʱ���Ƿ�����Ч
		BOOL		bShow;
		INT			nUserData;

		INPUTMAPEVENTHANDLE	pfn;
		INPUTMAPCHANGEEVENTHANDLE pfnChange;
	};


	TMap<DWORD, tagKeyMapInfo*>	m_mapKeyFunction;	// DescCRC -> info	ע����������ID
	TList<DWORD>				m_listKeyDesc;		// ������¼ע��˳��
	DWORD						m_dwCurrentSet;		// ��ǰ�������õļ�
	DWORD						m_dwTempKey;		// �����ݴ�

	BOOL EventHandler(tagGUIEvent* pEvent);
	
	VOID UpdateSetup();	// ����״̬ʱ�ĸ���
	virtual VOID ShowKeys();	// ������ʾ���а�����Ϣ
	
};






} // namespace vEngine {
