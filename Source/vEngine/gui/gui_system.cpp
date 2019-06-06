//-----------------------------------------------------------------------------
//!\file gui_system.cpp
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2007-04-13
//!
//!\brief ����ϵͳ
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "gui_define.h"
#include "gui_system.h"
#include "..\console\console_gui.h"

#include "gui_static.h"
#include "gui_staticex.h"
#include "gui_button.h"
#include "gui_pushbutton.h"
#include "gui_editbox.h"
#include "gui_editboxex.h"
#include "gui_listbox.h"
#include "gui_scrollbar.h"
#include "gui_progress.h"
#include "gui_combobox.h"
#include "gui_patch.h"
#include "gui_richeditbox.h"
#include "gui_framemutex.h"
#include "gui_frame.h"
#include "gui_virtual.h"
#include "gui_tab.h"
#include "gui_tree.h"
#include "gui_window.h"
#include "gui_httppic.h"

#include "..\vfs\vfs.h"
namespace vEngine {
//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL GUISystem::Init(INT nDesktopWidth, INT nDesktopHeight, IGUIRender* pRender,
					 LPCSTR szVFS)
{
	m_pRender = pRender;

	// �����������
	m_pPoint = m_pActive = m_pDesktop = new GUIWnd;
	if( NULL == m_pDesktop )
		return FALSE;

	XmlElement ele;
	ele.SetAttribute("Name", "desktop");
	ele.SetAttribute("ClassName", "GUIWnd");
	ele.SetAttribute("Movable", FALSE);
	ele.SetAttribute("Size_x", nDesktopWidth);
	ele.SetAttribute("Size_y", nDesktopHeight);
	if( FALSE == m_pDesktop->Init(this, NULL, &ele) )	// pFather = NULL
	{
		SAFE_DEL(m_pDesktop);
		return FALSE;
	}

		// ��ʼ������
	tagGUIImage* pNewTarget = new tagGUIImage;
	pNewTarget->dwHandle = 0;
	pNewTarget->ptSize = m_pDesktop->GetSize();
	m_pDesktop->SetCanvas(pNewTarget);

	// ��ӵ�ȫ�ֱ���
	m_mapAllWnd.Add(m_pUtil->Crc32(_T("\\desktop")), m_pDesktop);	

	m_listWndType.clear();
	m_listWndDesc.clear();
	m_listDestroy.Clear();

	// ע�������ڲ��ؼ�
	this->RegisterControls();

	if( szVFS )
		m_pVFS = szVFS;

	return TRUE;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID GUISystem::Destroy()
{
	ASSERT(0 != m_listWndType.size()); // �ⲿ�����ȵ��ô˺�������ע�����пؼ�����
	if( m_pDesktop )
	{
		m_pDesktop->Destroy();
		tagGUIImage* pTarget = m_pDesktop->GetCanvas();
		SAFE_DEL(pTarget);
	}
	SAFE_DEL(m_pDesktop);

	m_mapHandle.ResetIterator();
	tagEventHandle* pEventHandle = NULL;
	while( m_mapHandle.PeekNext(pEventHandle) )
		SAFE_DEL(pEventHandle);
	m_mapHandle.Clear();
}


//-----------------------------------------------------------------------------
// �����ؼ�
//-----------------------------------------------------------------------------
VOID GUISystem::Update()
{
	// ����destroy�б�
	if( m_listDestroy.Size() > 0 )
	{
		GUIWnd* pWnd;
		m_listDestroy.ResetIterator();
		while( m_listDestroy.PeekNext(pWnd) )
			this->DestroyWnd(pWnd);	// ����ɾ���ؼ�

		m_listDestroy.Clear();
	}

	// �¼�����
	DWORD dwSize= 0;
	tagGUIEvent* pEvent = NULL;
	while( pEvent = (tagGUIEvent*)m_ListEvent.GetMsg(dwSize) )
	{
		tagEventHandle* pHandle = m_mapHandle.Peek(pEvent->dwSendWndID);
		if( !P_VALID(pHandle) )
		{
			m_ListEvent.FreeMsg(pEvent);
			continue;
		}

		std::list<VUIEVENTHANDLE>::iterator it;
		for(it=pHandle->listFP.begin(); it!=pHandle->listFP.end(); ++it)
		{
			if( (*it)((DWORD)pEvent) )	// ��һ��������������Ͳ��ٵ���������������
				break;
		}

		m_ListEvent.FreeMsg(pEvent);
	}

	// ���пؼ�Update
	m_pDesktop->Update();
}


//-----------------------------------------------------------------------------
// �����ؼ�
//-----------------------------------------------------------------------------
VOID GUISystem::Render()
{ 
	m_pDesktop->Render(); 
}


//-----------------------------------------------------------------------------
// ���ļ�ֱ�Ӵ����ؼ���, szNewClass����ת�����ؼ�����
//-----------------------------------------------------------------------------
GUIWnd* GUISystem::CreateFromFile(LPCSTR szVFS, LPCTSTR szFileName, LPCSTR szNewClass)
{
	GUIWnd* pWndRoot = (GUIWnd*)GT_INVALID;

	XmlDocument doc;
	if( !doc.LoadFile(szVFS, szFileName) )
		return pWndRoot;

	XmlHandle docHandle( &doc );
	XmlHandle eleHandle = docHandle.FirstChildElement("desktop").FirstChildElement();
	XmlElement* pElement = eleHandle.Element();
	if( !pElement )	// �Ƿ����CGUI���̸�ʽ
		return pWndRoot;

	while( pElement )
	{
		if( !P_VALID(pWndRoot) )
		{
			if( P_VALID(szNewClass) )
				pElement->SetAttribute("ClassName", szNewClass);
			pWndRoot = CreateWnd(_T("\\desktop"), pElement);
		}
		else
		{
			CreateWnd(_T("\\desktop"), pElement);
		}

		pElement = pElement->NextSiblingElement();
	}

	return pWndRoot;
}


//-----------------------------------------------------------------------------
// ���ļ���ȡ�ؼ���
//-----------------------------------------------------------------------------
BOOL GUISystem::LoadXml(XmlElement* pElement, LPCSTR szVFS, LPCTSTR szFileName)
{
	XmlDocument doc;
	if( !doc.LoadFile(szVFS, szFileName) )
		return FALSE;

	XmlHandle docHandle( &doc );
	XmlHandle eleHandle = docHandle.FirstChildElement("desktop").FirstChildElement();
	if( !eleHandle.Element() )	// �Ƿ����CGUI���̸�ʽ
		return FALSE;

	*pElement = *eleHandle.Element();
	return TRUE;
}


//-----------------------------------------------------------------------------
// �޸�XML�����еĿؼ�����
//-----------------------------------------------------------------------------
BOOL GUISystem::ChangeXml(XmlElement* pXmlElement, LPCSTR szControl, LPCSTR szKey, LPCSTR szValue)
{
	std::string strName, strFind(szControl);
	BOOL bFinally = (strFind.find_first_of('\\') == std::string::npos);
	strFind.assign(strFind, 0, strFind.find_first_of('\\'));

	while( pXmlElement )
	{
		strName = pXmlElement->Attribute("Name");
		if( strName == strFind )
		{
			if( bFinally )
			{
				pXmlElement->SetAttribute(szKey, szValue);
				return TRUE;
			}
			else
			{
				pXmlElement = pXmlElement->FirstChildElement();
				szControl = strstr(szControl, "\\") + 1;	// ����"\"
				return ChangeXml(pXmlElement, szControl, szKey, szValue);
			}
		}
		pXmlElement = pXmlElement->NextSiblingElement();
	}
	return FALSE;
}


//-----------------------------------------------------------------------------
// �����ؼ�
//-----------------------------------------------------------------------------
GUIWnd* GUISystem::CreateWnd(LPCTSTR szFatherFullName, XmlElement* pXmlElement)
{
	// Ѱ�Ҹ��ؼ�
	DWORD dwFatherID = m_pUtil->Crc32(szFatherFullName);
	GUIWnd* pFather = m_mapAllWnd.Peek(dwFatherID);
	if( !P_VALID(pFather) )
	{
		IMSG(_T("could not found father control:%s\r\n"), szFatherFullName);
		pFather = m_pDesktop;	// �Ҳ������ؼ�,������Ϊ���ؼ�
	}

	// ��������
	tstring strClassName(m_pUtil->Unicode8ToUnicode(pXmlElement->Attribute("ClassName")));
	DWORD dwClassID = m_pUtil->Crc32(strClassName.c_str());
	GUIWnd* pWnd = m_AF.Create(dwClassID);
	if( !P_VALID(pWnd) )
	{
		IMSG(_T("could not find control class:%s\r\n"), strClassName.c_str());
		DWORD dwClassID = m_pUtil->Crc32(_T("CGUIStatic"));	// Ĭ�ϴ���Static�ؼ�
		pWnd = m_AF.Create(dwClassID);
	}

	// �ؼ���ʼ��
	if( FALSE == pWnd->Init(this, pFather, pXmlElement) )	
		IMSG(_T("could not init control:%s\r\n"), pWnd->GetFullName());
	
	m_mapAllWnd.Add(m_pUtil->Crc32(pWnd->GetFullName().c_str()), pWnd);	// ���ID
	pFather->AddChild(pWnd); // ֪ͨ����

	XmlElement* pChild = pXmlElement->FirstChildElement();
	while( pChild )
	{
		this->CreateWnd(pWnd->GetFullName().c_str(), pChild);
		pChild = pChild->NextSiblingElement();
	}

	// �Զ�����,ֻ������FALSE,˵�����пؼ����Ƶ�����λ��
	while( pWnd->Align() );

	pWnd->FlipToTop();
	return pWnd;
}


//-----------------------------------------------------------------------------
// ����ɾ���ؼ�
//-----------------------------------------------------------------------------
VOID GUISystem::DestroyWnd(GUIWnd* pWnd)
{
	// ��ȫ�ֱ���ɾ��
	DWORD dwID = m_pUtil->Crc32(pWnd->GetFullName().c_str());
	m_mapAllWnd.Erase(dwID);

	// �����active�ؼ���������active�ؼ� 
	if( m_pActive == pWnd )	
		m_pActive = m_pDesktop;
	if( m_pPoint == pWnd )
		m_pPoint = m_pDesktop;

	// ������ɾ������
	pWnd->Destroy();

	// ���ڴ���ɾ��
	DWORD dwClassID = m_pUtil->Crc32(pWnd->GetClassName().c_str());
	m_AF.Destroy(pWnd, dwClassID);

	// ɾ��ע����¼�������
	tagEventHandle* pHandle = m_mapHandle.Peek(dwID);
	if( P_VALID(pHandle) )
	{
		m_mapHandle.Erase(dwID);
		SAFE_DEL(pHandle);
	}
}



//-----------------------------------------------------------------------------
// ɾ���ؼ�
//-----------------------------------------------------------------------------
BOOL GUISystem::AddToDestroyList(GUIWnd* pWnd)	
{
	DWORD dwID = m_pUtil->Crc32(pWnd->GetFullName().c_str());
	if( !GT_VALID(m_mapAllWnd.Peek(dwID)) )
		return FALSE;	// ������ָ���ؼ�

	m_listDestroy.PushBack(pWnd);
	return TRUE;
}


//-----------------------------------------------------------------------------
// ��¼ָ���ؼ������ӿؼ�
//-----------------------------------------------------------------------------
VOID GUISystem::Record(XmlElement* pXmlElement, GUIWnd* pWnd)
{
	if( pWnd == m_pDesktop )
	{
		pXmlElement->SetValue("desktop");
	}
	else
	{
		pXmlElement->SetValue("UIControl");
		pWnd->GetVar()->Save(pXmlElement, TRUE);
	}

	std::list<GUIWnd*>& listWnd = pWnd->GetAllChild();
	std::list<GUIWnd*>::iterator it;
	for(it=listWnd.begin(); it!=listWnd.end(); ++it)
	{
		XmlElement* pNewXmlElement = new XmlElement;
		if( P_VALID(pNewXmlElement) )
		{
			Record(pNewXmlElement, (*it));
			pXmlElement->LinkEndChild(pNewXmlElement);
		}
		else
		{
			delete(pNewXmlElement);
		}
	}
}


//-----------------------------------------------------------------------------
// �ػ����пؼ�
//-----------------------------------------------------------------------------
VOID GUISystem::RedrawAllWnd()
{
	GUIWnd* pWnd = NULL;
	m_mapAllWnd.ResetIterator();
	while(m_mapAllWnd.PeekNext(pWnd))
		pWnd->SetRefresh();
}


//-----------------------------------------------------------------------------
// ����windows��Ϣ
//-----------------------------------------------------------------------------
BOOL GUISystem::OnWindowsMsg(DWORD dwMsg, DWORD dw1, DWORD dw2)
{
	BOOL bHandle = FALSE;
	m_Msg.Set(dwMsg, dw1, dw2);

	if( dwMsg >= WM_MOUSEFIRST && dwMsg <= WM_MOUSELAST )
		m_pRender->ClientToView(m_Msg.pt);

	// �Ƚ���Ϣֱ�Ӵ�������ؼ�
	bHandle = m_pActive->OnInputMessage(&m_Msg);
	
	if( dwMsg == WM_MOUSEMOVE )	// ����ƶ���Ϣͬʱ��������������Ŀؼ�
	{
		GUIWnd* pLastPointWindow = m_pPoint;
		GUIWnd* pWnd = m_pDesktop->GetWnd(m_Msg.pt);
		if( P_VALID(pWnd) && pWnd != m_pPoint )
			m_pPoint = pWnd;	// �ı����ָ��ؼ�

		if( pLastPointWindow != m_pActive )
			pLastPointWindow->OnInputMessage(&m_Msg);
	}
		
	return bHandle;
}


//-----------------------------------------------------------------------------
//! ���ý���ؼ�
//-----------------------------------------------------------------------------
BOOL GUISystem::SetActive(GUIWnd* pControl) 
{ 
	if( m_pActive == pControl )	// ��ǰ�Ŀؼ����ǽ���
		return TRUE;

	if( P_VALID(m_pModal) && pControl != m_pModal && !IsChildOf(m_pModal, pControl) )
		return FALSE;

	DWORD dwID = pControl->GetID();
	GUIWnd* pLostActive = m_pActive;
	pLostActive->OnDeactive(pControl);	// ���������ɿؼ��ع�

	pControl = GetWnd(dwID);
	if( P_VALID(pControl) )
	{
		m_pActive = pControl->OnActive();
		return TRUE;
	}
	else
	{
		m_pActive = m_pDesktop->OnActive();
		return FALSE;
	}
}


//-----------------------------------------------------------------------------
// ����ǿ�ƽ���ؼ�������NULL��ȡ��
//-----------------------------------------------------------------------------
VOID GUISystem::SetModal(GUIWnd* pControl)
{
	m_pModal = pControl;
}


//-----------------------------------------------------------------------------
// ע���¼�������
//-----------------------------------------------------------------------------
BOOL GUISystem::RegisterEventHandler( tstring strWndName, VUIEVENTHANDLE fp )
{
	DWORD dwID = m_pUtil->Crc32(strWndName.c_str());
	tagEventHandle* pHandle = m_mapHandle.Peek(dwID);
	if( !P_VALID(pHandle) )
	{
		pHandle = new tagEventHandle;
		pHandle->strWnd = strWndName;
		m_mapHandle.Add(dwID, pHandle);
	}

	pHandle->listFP.push_back(fp);
	return TRUE;
}


//-----------------------------------------------------------------------------
// ע���¼�������
//-----------------------------------------------------------------------------
BOOL GUISystem::UnRegisterEventHandler( tstring strWndName, VUIEVENTHANDLE fp )
{
	DWORD dwID = m_pUtil->Crc32(strWndName.c_str());
	tagEventHandle* pHandle = m_mapHandle.Peek(dwID);
	if( !P_VALID(pHandle) )
		return FALSE;
	
	pHandle->listFP.remove(fp);
	if( pHandle->listFP.empty() )
	{
		m_mapHandle.Erase(dwID);
		SAFE_DEL(pHandle);
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// ��ȡ����
//-----------------------------------------------------------------------------
BOOL GUISystem::LoadFont(LPCTSTR szFontFile)
{
	tstring strName = g_strLocalPath + _T("\\") +  szFontFile;
	DWORD dwFileSize = m_pVFS->GetSize(strName.c_str());
	DWORD dwFonts = 1;
	if( !P_VALID(dwFileSize) )
		return FALSE;

	LPBYTE pMem = new BYTE[dwFileSize];
	if( !P_VALID(pMem) )
		return FALSE;

	m_pVFS->LoadFile(pMem, strName.c_str());
	::AddFontMemResourceEx(pMem, dwFileSize, 0, &dwFonts);
	SAFE_DEL_ARRAY(pMem);
	return TRUE;
}



//-----------------------------------------------------------------------------
// ����Ƿ���ָ���ؼ�������ؼ�
//-----------------------------------------------------------------------------
BOOL GUISystem::IsChildOf(GUIWnd* pFather, GUIWnd* pChild)
{
	if( pFather == pChild->GetFather() )
		return TRUE;

	pChild = pChild->GetFather();
	if( P_VALID(pChild) )
		return IsChildOf(pFather, pChild);

	return FALSE;
}


//-----------------------------------------------------------------------------
// ע�������ڲ��ؼ�
//-----------------------------------------------------------------------------
VOID GUISystem::RegisterControls()
{
	Register(TWrap<GUIWnd>(), _T("CGUIWnd"), _T("BaseClass"));
	Register(TWrap<GUIStatic>(), _T("CGUIStatic"), _T("Static"));
	Register(TWrap<GUIStaticEx>(), _T("CGUIStaticEx"), _T("StaticEx"));
	Register(TWrap<GUIButton>(), _T("CGUIButton"), _T("Button"));
	Register(TWrap<GUIPushButton>(), _T("CGUIPushButton"), _T("PushButton"));
	Register(TWrap<GUIEditBox>(), _T("CGUIEditBox"), _T("EditBox"));
	Register(TWrap<GUIEditBoxEx>(), _T("CGUIEditBoxEx"), _T("EditBoxEx"));
	Register(TWrap<GUIRichEditBox>(), _T("CGUIRichEditBox"), _T("RichEditBox"));
	Register(TWrap<GUIListBox>(), _T("CGUIListBox"), _T("ListBox"));
	Register(TWrap<GUIScrollBar>(), _T("CGUIScrollBar"), _T("ScrollBar"));
	Register(TWrap<GUIProgress>(), _T("CGUIProgress"), _T("ProgressBar"));
	Register(TWrap<GUIComboBox>(), _T("CGUIComboBox"), _T("ComboBox"));
	Register(TWrap<GUIPatch>(), _T("CGUIPatch"), _T("PatchWindow"));
	Register(TWrap<GUIFrame>(), _T("CGUIFrame"), _T("FrameBox"));
	Register(TWrap<GUIFrameMutex>(), _T("CGUIFrameMutex"), _T("FrameMutex"));
	Register(TWrap<GUIVirtual>(), _T("CGUIVirtual"), _T("VirtualFrame"));
	Register(TWrap<GUITab>(), _T("CGUITab"), _T("Tab"));
	Register(TWrap<GUITree>(), _T("CGUITree"), _T("Tree"));
	Register(TWrap<GUIWindow>(), _T("CGUIWindow"), _T("WindowWrap"));
	Register(TWrap<GUIHttpPic>(), _T("CGUIHttpPic"), _T("HttpPicture"));
}



}	// namespace vEngine {

