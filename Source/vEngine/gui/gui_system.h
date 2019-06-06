//-----------------------------------------------------------------------------
//!\file gui_system.h
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2007-04-12
//!
//!\brief ����ϵͳ
//-----------------------------------------------------------------------------
#pragma once

namespace vEngine {
//-----------------------------------------------------------------------------
// GUI����ϵͳ, (Ĭ�ϻᴴ��һ������ desktop �ĸ��ؼ�)
//-----------------------------------------------------------------------------
class VENGINE_API GUISystem
{
public:
	BOOL Init(INT nWidth, INT nHeight, IGUIRender*, LPCSTR szVFS=NULL);
	VOID Destroy();
	VOID Update();
	VOID Render();

	// �ⲿͨ���˺�����XUIϵͳ��������Ϣ�������Ƿ����˼�����Ϣ
	BOOL OnWindowsMsg(DWORD dwMsg, DWORD dwParam1, DWORD dwParam2);

	// ���ļ�ֱ�Ӵ����ؼ���, szNewClass����ת�����ؼ�����
	GUIWnd* CreateFromFile(LPCSTR szVFS, LPCTSTR szFileName, LPCSTR szNewClass=NULL);	
	// ���ļ���ȡ�ؼ���, �������ϴ���
	BOOL LoadXml(XmlElement* pElement, LPCSTR szVFS, LPCTSTR szFileName);
	// �ı�XML��ĳ���ؼ���ָ������
	BOOL ChangeXml(XmlElement* pXmlElement, LPCSTR szControl, LPCSTR szKey, LPCSTR szValue);
	GUIWnd* CreateWnd(LPCTSTR szFatherFullName, XmlElement* pXmlElement);// �����ؼ�

	// ���ٿؼ�
	BOOL AddToDestroyList(GUIWnd* pWnd); // �ⲿ���ã�����ɾ���ؼ�
	VOID DestroyWnd(GUIWnd* pWnd);

	// ���㴦��
	BOOL SetActive(GUIWnd* pControl);	// ���ý���ؼ�
	VOID SetModal(GUIWnd* pControl);	// ����ǿ�ƽ���ؼ�������NULL��ȡ��

	// �¼�����
	BOOL PushEvent(tagGUIEvent* pEvent) { return m_ListEvent.AddMsg(pEvent, sizeof(tagGUIEvent)); }
	BOOL RegisterEventHandler(tstring strWndName, VUIEVENTHANDLE fp); // ע���¼�������
	BOOL UnRegisterEventHandler(tstring strWndName, VUIEVENTHANDLE fp); // ע���¼�������

	// ��ȡ����
	BOOL LoadFont(LPCTSTR szFontFile);

	// ���ֹ��߽ӿ�
	GUIWnd* GetWnd(DWORD dwID) {return m_mapAllWnd.Peek(dwID);} // ��ÿؼ�ָ��,��������ڷ���GT_INVALID
	GUIWnd* GetWnd(LPCTSTR szFullName) {return m_mapAllWnd.Peek(m_pUtil->Crc32(szFullName));}
	GUIWnd* GetMousePoint() {return m_pPoint;}	// �õ���굱ǰָ��Ŀؼ�
	GUIWnd* GetActive() {return m_pActive;}	// �õ���ǰ����Ŀؼ�
	GUIWnd* GetDesktop() {return m_pDesktop;}	// �õ�����
	IGUIRender*	GetRender() {return m_pRender;}	// �����Ⱦ�ӿ�

	BOOL IsChildOf(GUIWnd* pFather, GUIWnd* pChild); // ����Ƿ���ָ���ؼ�������ؼ�
	VOID Record(XmlElement* pXmlElement, GUIWnd* pWnd);	// ��¼ָ���ؼ������ӿؼ���XmlElement
	VOID RedrawAllWnd(); // �ػ����пؼ�

	// ע��ؼ���
	template<typename T>VOID Register(TWrap<T> wrap, LPCTSTR szName, LPCTSTR szDesc);

	// ���Ժͱ༭�ӿ�
	std::list<tstring>& EnumControlType() {return m_listWndType;}	// �õ����пؼ�����
	std::list<tstring>& EnumControlDesc() {return m_listWndDesc;}	// �õ����пؼ���������
	INT GetControlNum() { return m_mapAllWnd.Size(); } // �õ��ؼ�����

	GUISystem():m_pDesktop(0),m_pActive(0),m_pModal(0),m_pPoint(0){}
	~GUISystem() {this->Destroy();}

private:
	TObjRef<Util>					m_pUtil;
	TObjRef<VirtualFileSys>			m_pVFS;
	TAutoFactory<GUIWnd>			m_AF;
	IGUIRender*						m_pRender;

	std::list<tstring>				m_listWndType;	// ���пؼ�����
	std::list<tstring>				m_listWndDesc;	// ���пؼ�����
	TMap<DWORD, GUIWnd*>			m_mapAllWnd;	// ָ�����пؼ�
	TList<GUIWnd*>					m_listDestroy;	// ɾ���б�
	GUIWnd*							m_pDesktop;		// ���ؼ�(����)
	GUIWnd*							m_pActive;		// ��ǰ����Ŀؼ�
	GUIWnd*							m_pModal;		// ��ǰǿ�Ƽ���Ŀؼ�
	GUIWnd*							m_pPoint;		// ��ǰָ��Ŀؼ�
	tagGUIInputMsg					m_Msg;


	struct tagEventHandle	// �¼�����ṹ����
	{
		tstring						strWnd;
		std::list<VUIEVENTHANDLE>	listFP;
	};
	TMap<DWORD, tagEventHandle*>	m_mapHandle;	// �¼�������
	MsgQueue						m_ListEvent;	// �¼��б�

	VOID RegisterControls();	// ע�������ڲ��ؼ�
};




//-----------------------------------------------------------------------------
// ע��ؼ�
//-----------------------------------------------------------------------------
template<typename T>VOID GUISystem::Register(TWrap<T> wrap, LPCTSTR szName, LPCTSTR szDesc)
{ 
	if( m_AF.Register(wrap, m_pUtil->Crc32(szName)) )
	{
		m_listWndType.push_back(tstring(szName));
		m_listWndDesc.push_back(tstring(szDesc));
	}
}

}	// namespace vEngine {
