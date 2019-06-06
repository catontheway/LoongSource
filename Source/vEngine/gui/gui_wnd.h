//-----------------------------------------------------------------------------
//!\file gui_wnd.h
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2004-12-08
//!
//!\brief ����ϵͳ���
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳ���
//-----------------------------------------------------------------------------
class VENGINE_API GUIWnd
{
public:
	//-----------------------------------------------------------------------------
	// �ɼ̳еĻ����ؼ���Ϊ
	//-----------------------------------------------------------------------------
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Update();
	virtual VOID Render();

	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg); // ������Ϣ
	virtual GUIWnd* GetWnd(tagPoint& pt);	 //���ݸ��������������˿ؼ�(���ǲ��ɼ�)

	virtual VOID SendEvent(tagGUIEvent* pEvent);// ���ⷢ���¼�
	virtual VOID OnEvent(tagGUIEvent* pEvent); // �����¼�
	virtual GUIWnd* OnActive();	// ������֪ͨ
	virtual VOID OnDeactive(GUIWnd* pWnd);		// ȡ������֪ͨ,pWnd����Ҫ����Ŀؼ�

	virtual GUIWnd* FlipToTop();		// �����ؼ���������ǰ��
	virtual BOOL PointInWnd(tagPoint& pt, bool bCheckChild=true); // ָ�����Ƿ��ڱ�������

	virtual VOID AddChild(GUIWnd* pChild) {m_listChild.push_back(pChild);}	// ����Ӵ���
	virtual VOID DelChild(GUIWnd* pChild) {m_listChild.remove(pChild);} // ɾ���Ӵ���
	
	virtual BOOL Move(tagPoint& ptClient); // �ƶ�ָ��ƫ����
	virtual BOOL Resize(tagPoint& ptSize); // �ı�ؼ���С,�����´�С

	virtual VOID ClientToView(tagPoint& ptClient); // ���ؼ�client����ת������Ϸ���ڵ��߼�����(view)
	virtual VOID RefreshViewCoord(); // ˢ���߼�����(view)
	virtual BOOL Align();	// �Զ�����,��������ؼ����ƶ�,�򷵻�TRUE

	virtual VOID SetInvisible(BOOL b) { m_bInvisible = b; SetRefresh(b); }
	//-----------------------------------------------------------------------------
	// �������Բ���
	//-----------------------------------------------------------------------------
	DWORD GetID() { return m_dwID; }
	std::list<GUIWnd*>& GetAllChild() { return m_listChild; }	// �õ�������һ���ؼ���ָ��,����Z order
	const tstring& GetFullName() { return m_strFullName; }	// �õ�ȫ��
	const tstring& GetName() { return m_strName; }			// �õ�����
	const tstring& GetClassName() { return m_strClassName; }	// �õ���������
	VarMgr* GetVar() { return m_pVarMgr; }				// �õ�����������
	GUIWnd* GetFather() { return m_pFather; }			// �õ����ؼ�ָ��
	GUIWnd* GetChild(LPCTSTR szChildName);				// �õ��ӿؼ�ָ�룬����Ҫ��ȫ��
	const tagPoint& GetSize() { return m_ptSize; }		// �õ��ؼ���С
	const tagPoint& GetClient() { return m_ptClient; }	// �õ�client����
	const tagPoint& GetView() { return m_ptView; }		// �õ�view����
	tagGUIFont* GetFont() { return m_pFont; }
	BOOL IsInvisible() { return m_bInvisible; }
	BOOL IsMovable() { return m_bMovable; }
	BOOL IsResizable() { return m_bResizable; }
	BOOL IsTopMost() { return m_bTopMost; }
	BOOL IsBottomMost() { return m_bBottomMost; }
	VOID SetMovable(BOOL b) { m_bMovable = b; }
	VOID SetResizable(BOOL b) { m_bResizable = b; }
	VOID SetTopMost(BOOL b) { m_bTopMost = b; }
	BOOL IsEditable() { return m_bEditable; }
	VOID SetUnClickable(BOOL b) { m_bUnClickable = b; }

	VOID SetMsgHandle(VUIMSGHANDLE fpHandle){ m_fpMsgHandle = fpHandle;}	// ����ΪNULL�������

	//-----------------------------------------------------------------------------
	// �ṩ���ⲿ�ݴ���ʱ����,�ؼ����ٺ�,����Ҳ��������
	//-----------------------------------------------------------------------------
	std::vector<DWORD>& GetUserData() {return m_vecUserData;}
	VOID SetUserData(INT nIndex, DWORD dwValue);
	DWORD GetUserData(INT nIndex);

	//-----------------------------------------------------------------------------
	// ��������
	//-----------------------------------------------------------------------------
	tagGUIImage* GetCanvas() { return m_pCanvas; }
	VOID SetCanvas(tagGUIImage* pImage) { m_pCanvas = pImage; }
	// ������Ҫ�ػ棬bCanvas:�ػ���������
	VOID SetRefresh(BOOL bCanvas=FALSE) {m_bRefresh = TRUE; m_pCanvas->bRefresh = bCanvas?TRUE:m_pCanvas->bRefresh;}
	BOOL IsRefresh() { return (!m_bInvisible && (m_bRefresh || m_pCanvas->bRefresh));}	// �Ƿ���Ҫ�ػ�
	
	GUIWnd();
	virtual ~GUIWnd();

protected:
	TObjRef<Util>		m_pUtil;
	GUISystem*			m_pSystem;
	VarMgr*				m_pVarMgr;
	IGUIRender*			m_pRender;
	
	DWORD				m_dwID;					// Fullname CRC32 id
	tstring				m_strFullName;			// �ؼ�ȫ��
	GUIWnd*				m_pFather;				// Ψһ���ؼ�
	VUIMSGHANDLE		m_fpMsgHandle;			// ��Ϣ����ص�����
	std::list<GUIWnd*>	m_listChild;			// ����Z�����е�����һ���ӿؼ�

	tagPoint			m_ptView;				// ��Ϸ��������߼�����
	tagPoint			m_ptLastMouseClickView;	// ���һ���������λ��
	BOOL				m_bDrag;				// �Ƿ����϶�״̬
	BOOL				m_bResize;				// �Ƿ��ڸı��С״̬
	
	tagGUIImage*		m_pCanvas;				// ����
	BOOL				m_bRefresh;				// �Ƿ���Ҫ�ػ�
	BOOL				m_bEditable;			// �Ƿ��ǿɱ༭�ؼ�
	tagGUIFont*			m_pFont;				// ����
	
	BOOL				m_bUnClickable;			// �Ƿ��޷�����
	std::vector<DWORD>	m_vecUserData;			// �û�����




	//-----------------------------------------------------------------------------
	// �ɱ༭����
	//-----------------------------------------------------------------------------
	tstring				m_strName;			// �ؼ���
	tstring				m_strClassName;		// �ؼ�����
	
	BOOL				m_bInvisible;		// �Ƿ񲻿ɼ�
	BOOL				m_bResizable;		// �Ƿ�ɸı��С
	BOOL				m_bMovable;			// �Ƿ���ƶ�
	BOOL				m_bMoveFather;		// �Ƿ��������ؼ��ƶ�
	BOOL				m_bCanMoveOut;		// �Ƿ����Ƶ����ؼ���
	BOOL				m_bTopMost;			// �Ƿ������ǰ
	BOOL				m_bBottomMost;		// �Ƿ�������
	
	tagPoint			m_ptClient;			// �߼�����
	tagPoint			m_ptSize;			// ��С

	tstring				m_strAlignWnd;		// ����ؼ���
	EGUIAlign			m_eAlign;			// ���뷽ʽ
	tagPoint			m_ptAlignOffset;	// ����ƫ��

};



}	// namespace vEngine {


