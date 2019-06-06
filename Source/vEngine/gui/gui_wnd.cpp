//-----------------------------------------------------------------------------
//!\file gui_wnd.cpp
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2005-03-31
//!
//!\brief ����ϵͳ���
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "gui_wnd.h"
#include "gui_event_define.h"


namespace vEngine {
//-----------------------------------------------------------------------------
// constructor / destructor
//-----------------------------------------------------------------------------
GUIWnd::GUIWnd()
{
	m_pVarMgr = new VarMgr;
	m_fpMsgHandle = NULL;
	m_bUnClickable = FALSE;

	// ע�����пɱ༭����
	m_pVarMgr->Register(_T("Name"), &m_strName);
	m_pVarMgr->Register(_T("ClassName"), &m_strClassName);
	m_pVarMgr->Register(_T("Invisible"), &m_bInvisible, _T("no yes"));
	m_pVarMgr->Register(_T("Resizable"), &m_bResizable, _T("no yes"));
	m_pVarMgr->Register(_T("Movable"), &m_bMovable, _T("no yes"));
	m_pVarMgr->Register(_T("MoveFather"), &m_bMoveFather, _T("no yes"));
	m_pVarMgr->Register(_T("CanMoveOutOfFather"), &m_bCanMoveOut, _T("no yes"));
	m_pVarMgr->Register(_T("TopMost"), &m_bTopMost, _T("no yes"));
	m_pVarMgr->Register(_T("BottomMost"), &m_bBottomMost, _T("no yes"));
	m_pVarMgr->Register(_T("LogicPos"), &m_ptClient);
	m_pVarMgr->Register(_T("Size"), &m_ptSize);
	m_pVarMgr->Register(_T("AlignControl"), &m_strAlignWnd);
	m_pVarMgr->Register(_T("AlignType"), (INT*)&m_eAlign, 
		_T("NoAlign Center InnerLeftTop InnerLeft InnerLeftBottom InnerRightTop InnerRight ")
		_T("InnerRightBottom InnerTop InnerBottom OuterLeftTop OuterLeft OuterLeftBottom ")
		_T("OuterRightTop OuterRight OuterRightBottom OuterTopLeft OuterTop OuterTopRight ")
		_T("OuterBottomLeft OuterBottom OuterBottomRight"));
	m_pVarMgr->Register(_T("AlignOffset"), &m_ptAlignOffset);

	// ���ó�ʼֵ
	m_ptSize.Set(128, 128);
	m_bRefresh = TRUE;
	m_bEditable = FALSE;
	m_pFont = NULL;
}


GUIWnd::~GUIWnd()
{
	SAFE_DEL(m_pVarMgr);
}

//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL GUIWnd::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	m_pSystem = pSystem;
	m_pFather = pFather;
	m_pRender = pSystem->GetRender();

	// ��������
	m_pVarMgr->Load(pXmlElement);

	// �õ�ȫ��
	m_strFullName = P_VALID(pFather) ? pFather->GetFullName() : _T("");
	m_strFullName.append(_T("\\"));
	m_strFullName.append(m_strName);

	// ȷ��������
	while( P_VALID(m_pSystem->GetWnd(m_strFullName.c_str())) )
	{
		m_strName += _T("0");
		m_strFullName += _T("0");
	}

	m_dwID = m_pUtil->Crc32(m_strFullName.c_str());

	// ���ʵ������
	m_ptView = m_ptClient;
	ClientToView(m_ptView);
	m_bDrag = FALSE;
	m_bResize = FALSE;	// �Ƿ��ڸı��С״̬

	// �Ƿ���Ҫ�̳�һЩ����
	if( pFather )
	{
		m_pCanvas = pFather->GetCanvas();	// ����
		m_pCanvas->bRefresh = TRUE;
	}

	return TRUE;
}



//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID GUIWnd::Destroy()
{
	// ɾ�������Ӵ���
	std::list<GUIWnd*> temp = m_listChild;
	std::list<GUIWnd*>::iterator it;
	for(it=temp.begin(); it!=temp.end(); ++it)
		m_pSystem->DestroyWnd(*it);

	// ֪ͨ������ɾ���Լ�
	if( m_pFather )
		m_pFather->DelChild(this);

	SetRefresh(TRUE);
}


//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID GUIWnd::Update()
{
	std::list<GUIWnd*>::iterator it;
	for(it=m_listChild.begin(); it!=m_listChild.end(); ++it)
		(*it)->Update();
}


//-----------------------------------------------------------------------------
// render
//-----------------------------------------------------------------------------
VOID GUIWnd::Render()
{
	if( m_bInvisible )
		return;

	if( P_VALID(m_pCanvas->dwHandle) )	// �������������
		m_bRefresh = FALSE;	// ������Ⱦ��Ͳ���Ҫ�ٴ���Ⱦ

	std::list<GUIWnd*>::reverse_iterator it;
	for(it=m_listChild.rbegin(); it!=m_listChild.rend(); ++it)
		(*it)->Render();
}


//-----------------------------------------------------------------------------
// �ƶ�ָ��ƫ����
//-----------------------------------------------------------------------------
BOOL GUIWnd::Move(tagPoint& ptClient)
{
	if( FALSE == P_VALID(m_pFather) )
		return FALSE;

	if( ptClient.IsEmpty() ) 
		return FALSE;

	if( !m_bMovable )
	{
		if( m_bMoveFather )
			return m_pFather->Move(ptClient);
		else
			return FALSE;
	}

	// ������ƫ����������Ҫ�����ʵ��ֵ
	tagPoint ptNewClient = m_ptClient + ptClient;

	// ���ؼ��Ƿ��ܹ��ƶ���������֮���λ��
	if( !m_bCanMoveOut )
	{
		// �ѿؼ������ڸ�������
		const tagPoint& ptFatherSize = m_pFather->GetSize();
		if( ptNewClient.x + m_ptSize.x > ptFatherSize.x )
			ptNewClient.x = ptFatherSize.x - m_ptSize.x;
		if( ptNewClient.x < 0 )
			ptNewClient.x = 0;
		if( ptNewClient.y + m_ptSize.y > ptFatherSize.y )
			ptNewClient.y = ptFatherSize.y - m_ptSize.y;
		if( ptNewClient.y < 0 )
			ptNewClient.y = 0;
	}

	SetRefresh(TRUE);
	
	m_ptClient = ptNewClient;
	this->RefreshViewCoord();

	tagGUIEvent event(this, EGUIE_Drag, NULL);
	SendEvent(&event);
	return TRUE;
}

//-----------------------------------------------------------------------------
// �ı�ؼ���С
//-----------------------------------------------------------------------------
BOOL GUIWnd::Resize(tagPoint& ptSize)
{
	if( FALSE == P_VALID(m_pFather) )	// ���治�ܸĶ���С
		return FALSE;

	if( !m_bResizable )
		return FALSE;

	m_ptSize = ptSize;

	// ���ؼ��Ƿ��ܹ��ƶ���������֮���λ��
	if( !m_bCanMoveOut )
	{
		// �ѿؼ������ڸ�������
		const tagPoint& ptFatherSize = m_pFather->GetSize();
		if( m_ptSize.x + m_ptClient.x > ptFatherSize.x )
			m_ptSize.x = ptFatherSize.x - m_ptClient.x;
		if( m_ptSize.y + m_ptClient.y > ptFatherSize.y )
			m_ptSize.y = ptFatherSize.y - m_ptClient.y;
	}

	SetRefresh(TRUE);

	tagGUIEvent event(this, EGUIE_Resize, NULL);
	SendEvent(&event);
	return TRUE;
}


//-----------------------------------------------------------------------------
// ���ؼ�client����ת������Ϸ���ڵ��߼�����(view)
//-----------------------------------------------------------------------------
VOID GUIWnd::RefreshViewCoord()
{
	m_ptView = m_ptClient;
	ClientToView(m_ptView);

	// ���������ӿؼ�������
	std::list<GUIWnd*>::iterator it;
	for(it=m_listChild.begin(); it!=m_listChild.end(); ++it)
		(*it)->RefreshViewCoord();
}



//-----------------------------------------------------------------------------
// ��windows��client����ת����������Ϸ���ڵ��߼�����(view)
//-----------------------------------------------------------------------------
VOID GUIWnd::ClientToView(tagPoint& ptClient)
{
	if( FALSE == P_VALID(m_pFather) )
		return;

	ptClient = ptClient + m_pFather->GetClient();
	m_pFather->ClientToView(ptClient);
}



//-----------------------------------------------------------------------------
// ��ָ���Ӵ����ƶ�����ǰ��
//-----------------------------------------------------------------------------
GUIWnd* GUIWnd::FlipToTop()
{
	if( FALSE == P_VALID(m_pFather) )
		return this;

	std::list<GUIWnd*>& list = m_pFather->GetAllChild();
	list.remove(this);

	// ���������top most�ؼ��ͷŵ�����
	if( m_bTopMost )
	{
		list.push_front(this);
	}
	else if( m_bBottomMost )
	{
		// ����ŵ�����bottom most���͵Ŀؼ�������
		std::list<GUIWnd*>::iterator it = list.begin();
		while( it!=list.end() && !(*it)->IsBottomMost() )
			++it;

		list.insert(it, this);
	}
	else
	{
		// ����ŵ�����top most���͵Ŀؼ�������
		std::list<GUIWnd*>::iterator it = list.begin();
		while( it!=list.end() && (*it)->IsTopMost() )
			++it;

		list.insert(it, this);
	}

	SetRefresh();
	m_pFather->FlipToTop();
	return this;
}



//-----------------------------------------------------------------------------
// ָ�����Ƿ������Ǵ�����
//-----------------------------------------------------------------------------
BOOL GUIWnd::PointInWnd(tagPoint& pt, bool bCheckChild)
{
	if( m_bInvisible )
		return FALSE;

	tagRect rc;
	rc.SetLTWH(m_ptView, m_ptSize);
	if( !m_bInvisible && rc.IsContain(pt) )
		return TRUE;

	// �ټ����Ƿ������ǵ������Ӵ�����
	if( bCheckChild )
	{
		std::list<GUIWnd*>::iterator it;
		for(it=m_listChild.begin(); it!=m_listChild.end(); ++it)
		{
			if( TRUE == (*it)->PointInWnd(pt, bCheckChild) )
				return TRUE;
		}
	}

	return FALSE;
}



//-----------------------------------------------------------------------------
// ���ݸ��������������˿ؼ�(���ǲ��ɼ�)
//-----------------------------------------------------------------------------
GUIWnd* GUIWnd::GetWnd(tagPoint& pt) 
{
	if( m_bInvisible )
		return (GUIWnd*)GT_INVALID;

	if( !PointInWnd(pt) )
		return (GUIWnd*)GT_INVALID;

	if( m_bUnClickable )
		return (GUIWnd*)GT_INVALID;

	GUIWnd *pWnd = this;   //�����������,��δ���Ƕ��˴���

	// �Ӷ��˴��忪ʼ����
	std::list<GUIWnd*>::iterator it;
	for(it=m_listChild.begin(); it!=m_listChild.end(); ++it)
	{
		GUIWnd *pTmp = (*it)->GetWnd(pt);
		if( GT_VALID(pTmp) )
		{
			pWnd = pTmp;
			break;
		}
	}
	
	return pWnd;
}


//-----------------------------------------------------------------------------
// handle input message
//-----------------------------------------------------------------------------
BOOL GUIWnd::OnInputMessage(tagGUIInputMsg* pMsg)
{
	switch(pMsg->dwType)
	{
	case WM_LBUTTONDOWN:	// ������������
		{
			GUIWnd* pWnd = m_pSystem->GetDesktop()->GetWnd(pMsg->pt);
			if( P_VALID(pWnd) && pWnd != this )
			{
				if( m_pSystem->SetActive(pWnd) )
					return pWnd->OnInputMessage(pMsg);
			}
			
			m_ptLastMouseClickView = pMsg->pt;
			if( !m_bDrag )
			{
				m_bDrag = TRUE;
				tagGUIEvent event(this, EGUIE_DragStart, pMsg);
				SendEvent(&event);
			}
		}
		break;

	case WM_RBUTTONDOWN:	// �Ҽ����������
		{
			GUIWnd* pWnd = m_pSystem->GetDesktop()->GetWnd(pMsg->pt);
			if( P_VALID(pWnd) && pWnd != this )
				return pWnd->OnInputMessage(pMsg);

			m_pSystem->SetActive(this);
		}
		break;

	case WM_MOUSEMOVE:	// �������϶��͸ı��С
		if( m_bDrag )
		{
			if( m_bResize && m_bResizable )	// �Ƿ�ɸı��С
			{
				tagPoint pt;
				pt = m_ptSize + pMsg->pt - m_ptLastMouseClickView;
				if( pt.x >= 0 && pt.y >= 0 )
				Resize(pt);
			}
			else
				Move(pMsg->pt - m_ptLastMouseClickView);

			m_ptLastMouseClickView = pMsg->pt;
		}
		else
		{
			// �Ƿ���Resize����
			if( m_pSystem->GetMousePoint() == this 
				&& (pMsg->pt.x >= m_ptView.x + m_ptSize.x - 5 
				&& pMsg->pt.x <= m_ptView.x + m_ptSize.x
				&& pMsg->pt.y >= m_ptView.y + m_ptSize.y - 5 
				&& pMsg->pt.y <= m_ptView.y + m_ptSize.y) )
				m_bResize = TRUE;
			else
				m_bResize = FALSE;
		}
		break;

	case WM_LBUTTONUP:	// �����϶��¼����
		if( m_bDrag )
		{
			m_bDrag = FALSE;
			tagGUIEvent event(this, EGUIE_DragEnd, pMsg, 1);
			SendEvent(&event);
		}
		break;
	case WM_KEYDOWN:	// ����������¼�
		{
			tagGUIEvent event(this, EGUIE_KeyDown, pMsg);
			SendEvent(&event);
		}
		break;
	}

	if( m_fpMsgHandle )
		return (*m_fpMsgHandle)(pMsg);
	else
		return FALSE;
}



//-----------------------------------------------------------------------------
// ���¼��������ص������������ϴ���,����TRUE��ʾ��Ϣ���ⲿ�ص���������
//-----------------------------------------------------------------------------
VOID GUIWnd::SendEvent(tagGUIEvent* pEvent)
{
	pEvent->dwSendWndID = m_dwID;
	m_pSystem->PushEvent(pEvent);
	if( m_pFather )
		m_pFather->SendEvent(pEvent);
}



//-----------------------------------------------------------------------------
// �����¼�
//-----------------------------------------------------------------------------
VOID GUIWnd::OnEvent(tagGUIEvent* pEvent)
{
	std::list<GUIWnd*>::iterator it;
	for(it=m_listChild.begin(); it!=m_listChild.end(); ++it)
		(*it)->OnEvent(pEvent);
}


//-----------------------------------------------------------------------------
// ȡ������֪ͨ
//-----------------------------------------------------------------------------
VOID GUIWnd::OnDeactive(GUIWnd* pWnd)
{
	m_bDrag = FALSE;
	tagGUIEvent event(this, EGUIE_Deactive, NULL, (DWORD)pWnd);
	SendEvent(&event);
}		


//-----------------------------------------------------------------------------
// ������֪ͨ
//-----------------------------------------------------------------------------
GUIWnd* GUIWnd::OnActive()
{ 
	tagGUIEvent event(this, EGUIE_Active, NULL);
	SendEvent(&event);
	return FlipToTop();
}


//-----------------------------------------------------------------------------
// �Զ�����,��������ؼ����ƶ�,�򷵻�TRUE
//-----------------------------------------------------------------------------
BOOL GUIWnd::Align()
{ 
	BOOL bResult = FALSE;

	std::list<GUIWnd*>::iterator it;
	for(it=m_listChild.begin(); it!=m_listChild.end(); ++it)
	{
		if( (*it)->Align() )
			bResult = TRUE;
	}

	if( EGUIA_Null == m_eAlign || !P_VALID(m_pFather) )	// ���Զ�����
		return bResult;

	if(  m_strAlignWnd.empty() && P_VALID(m_pFather) )
		m_strAlignWnd = m_pFather->GetFullName();

	GUIWnd* pWnd = m_pSystem->GetWnd(m_strAlignWnd.c_str());
	if( !P_VALID(pWnd) )
	{
		if( m_strAlignWnd == m_pFather->GetName() )
			pWnd = m_pFather;
		else
			return bResult;
	}

	// ��ʼ������
	tagPoint ptClientOrigin = m_ptClient;
	tagPoint ptViewOrigin = m_ptView;

	m_ptView.Clear();
	m_ptClient.Clear();
	ClientToView(m_ptView);

	tagPoint ptSize = pWnd->GetSize();
	tagPoint ptView = pWnd->GetView();
	tagPoint ptOffset = ptView - m_ptView;

	switch(m_eAlign)
	{
	case EGUIA_Center:				// �ڲ�����
		m_ptClient.x = (ptSize.x - m_ptSize.x) / 2; m_ptClient.y = (ptSize.y - m_ptSize.y) / 2; break;
	case EGUIA_InnerLeftTop:		// �ڲ�����
		m_ptClient.x = 0; m_ptClient.y = 0;	break;
	case EGUIA_InnerLeft:			// �ڲ�����
		m_ptClient.x = 0; m_ptClient.y = (ptSize.y - m_ptSize.y) / 2; break;
	case EGUIA_InnerLeftBottom:		// �ڲ�����
		m_ptClient.x = 0; m_ptClient.y = ptSize.y - m_ptSize.y; break;
	case EGUIA_InnerRightTop:		// �ڲ�����
		m_ptClient.x = ptSize.x - m_ptSize.x; m_ptClient.y = 0; break;
	case EGUIA_InnerRight:			// �ڲ�����
		m_ptClient.x = ptSize.x - m_ptSize.x; m_ptClient.y = (ptSize.y - m_ptSize.y) / 2; break;
	case EGUIA_InnerRightBottom:	// �ڲ�����
		m_ptClient.x = ptSize.x - m_ptSize.x; m_ptClient.y = ptSize.y - m_ptSize.y; break;
	case EGUIA_InnerTop:			// �ڲ�����
		m_ptClient.x = (ptSize.x - m_ptSize.x) / 2; m_ptClient.y = 0; break;
	case EGUIA_InnerBottom:			// �ڲ�����
		m_ptClient.x = (ptSize.x - m_ptSize.x) / 2; m_ptClient.y = ptSize.y - m_ptSize.y; break;
	case EGUIA_OuterLeftTop:		// �ⲿ����
		m_ptClient.x = -m_ptSize.x; m_ptClient.y = 0; break;
	case EGUIA_OuterLeft:			// �ⲿ����
		m_ptClient.x = -m_ptSize.x; m_ptClient.y = (ptSize.y - m_ptSize.y) / 2; break;
	case EGUIA_OuterLeftBottom:		// �ⲿ����
		m_ptClient.x = -m_ptSize.x; m_ptClient.y = ptSize.y - m_ptSize.y; break;
	case EGUIA_OuterRightTop:		// �ⲿ����
		m_ptClient.x = ptSize.x; m_ptClient.y = 0; break;
	case EGUIA_OuterRight:			// �ⲿ����
		m_ptClient.x = ptSize.x; m_ptClient.y = (ptSize.y - m_ptSize.y) / 2; break;
	case EGUIA_OuterRightBottom:	// �ⲿ����
		m_ptClient.x = ptSize.x; m_ptClient.y = ptSize.y - m_ptSize.y; break;
	case EGUIA_OuterTopLeft:		// �ⲿ����
		m_ptClient.x = 0; m_ptClient.y = -m_ptSize.y; break;
	case EGUIA_OuterTop:			// �ⲿ����
		m_ptClient.x = (ptSize.x - m_ptSize.x) / 2; m_ptClient.y = -m_ptSize.y; break;
	case EGUIA_OuterTopRight:		// �ⲿ����
		m_ptClient.x = ptSize.x - m_ptSize.x; m_ptClient.y = -m_ptSize.y; break;
	case EGUIA_OuterBottomLeft:		// �ⲿ����
		m_ptClient.x = 0; m_ptClient.y = ptSize.y; break;
	case EGUIA_OuterBottom:			// �ⲿ����
		m_ptClient.x = (ptSize.x - m_ptSize.x) / 2; m_ptClient.y = ptSize.y; break;
	case EGUIA_OuterBottomRight:	// �ⲿ����
		m_ptClient.x = ptSize.x - m_ptSize.x; m_ptClient.y = ptSize.y; break;
	}

	m_ptClient = m_ptClient + ptOffset;
	m_ptClient = m_ptClient + m_ptAlignOffset;
	if( !(ptClientOrigin == m_ptClient) )
	{
		this->RefreshViewCoord();
		this->SetRefresh(TRUE);	// �ػ���������
		return TRUE;
	}
	else
	{
		m_ptView = ptViewOrigin;
		return bResult;
	}

	
}


//-----------------------------------------------------------------------------
// �õ��ӿؼ���������Ҫ��ȫ��
//-----------------------------------------------------------------------------
GUIWnd* GUIWnd::GetChild(LPCTSTR szChildName)
{
	tstring str = m_strFullName + _T("\\") + szChildName;
	return m_pSystem->GetWnd(str.c_str());
}


//-----------------------------------------------------------------------------
// �����û�����
//-----------------------------------------------------------------------------
VOID GUIWnd::SetUserData( INT nIndex, DWORD dwValue )
{
	if( (INT)m_vecUserData.size() < nIndex+1 )
		m_vecUserData.resize(nIndex+1);
	m_vecUserData[nIndex] = dwValue;
}


//-----------------------------------------------------------------------------
// �õ��û�����
//-----------------------------------------------------------------------------
DWORD GUIWnd::GetUserData( INT nIndex )
{
	if( (INT)m_vecUserData.size() < nIndex+1 )
		m_vecUserData.resize(nIndex+1, 0);
	return m_vecUserData[nIndex];
}

}	// namespace vEngine {