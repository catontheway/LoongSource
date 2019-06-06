//-----------------------------------------------------------------------------
//!\file gui_define.h
//!\author Lyp
//!
//!\date 2004-12-09
//! last 2005-03-29
//!
//!\brief ����ϵͳ������ݽṹ����
//-----------------------------------------------------------------------------
#pragma once

namespace vEngine {

// �������Ԥ����
class IGUIRender;
class GUISystem;
class GUIWnd;
class GUIStatic;
class GUIStaticEx;
class GUIButton;
class GUIPushButton;
class GUIEditBox;
class GUIEditBoxEx;
class GUIListBox;
class GUIScrollBar;
class GUIProgress;
class GUIComboBox;
class GUIPatch;
class GUIFrameMutex;
class GUIVirtual;
class GUITab;
class GUIWindow;
class GUIHttpPic;
class GUIEditor;
class GUIEventMgr;

struct tagGUIEvent;
typedef DWORD (WINAPI* VUIEVENTHANDLE)(DWORD);


enum EGUIButtonState	// ��ť״̬
{
	EGUIBS_PopUp=0,			// ��������״̬
	EGUIBS_PushDown,		// ����״̬
	EGUIBS_HighLight,		// ����״̬��һ���ʾ�����ָ��
};

enum EGUIAlign	// ���뷽ʽ
{
	EGUIA_Null=0,			// ���Զ�����
	EGUIA_Center,			// �ڲ�����
	EGUIA_InnerLeftTop,		// �ڲ�����
	EGUIA_InnerLeft,		// �ڲ�����
	EGUIA_InnerLeftBottom,	// �ڲ�����
	EGUIA_InnerRightTop,	// �ڲ�����
	EGUIA_InnerRight,		// �ڲ�����
	EGUIA_InnerRightBottom,	// �ڲ�����
	EGUIA_InnerTop,			// �ڲ�����
	EGUIA_InnerBottom,		// �ڲ�����
	EGUIA_OuterLeftTop,		// �ⲿ����
	EGUIA_OuterLeft,		// �ⲿ����
	EGUIA_OuterLeftBottom,	// �ⲿ����
	EGUIA_OuterRightTop,	// �ⲿ����
	EGUIA_OuterRight,		// �ⲿ����
	EGUIA_OuterRightBottom,	// �ⲿ����
	EGUIA_OuterTopLeft,		// �ⲿ����
	EGUIA_OuterTop,			// �ⲿ����
	EGUIA_OuterTopRight,	// �ⲿ����
	EGUIA_OuterBottomLeft,	// �ⲿ����
	EGUIA_OuterBottom,		// �ⲿ����
	EGUIA_OuterBottomRight,	// �ⲿ����
};


//-----------------------------------------------------------------------------
// �ڲ�ʹ�õ�������Ϣ
//-----------------------------------------------------------------------------
struct tagGUIInputMsg	
{
	DWORD			dwType;		// ��Ϣ���
	DWORD			dwParam1;	// win32 msg����������
	DWORD			dwParam2;	// win32 msg����������
	tagPoint		pt;			// �¼����������λ��

	VOID Set(DWORD dw, DWORD dw1, DWORD dw2) 
	{ dwType = dw; dwParam1 = dw1; dwParam2 = dw2; pt.Set(LOWORD(dw2), HIWORD(dw2)); }
};


typedef BOOL (WINAPI* VUIMSGHANDLE)(tagGUIInputMsg*);

}	// namespace vEngine {


#include "gui_render.h"
#include "gui_wnd.h"
#include "gui_event_define.h"
#include "gui_system.h"

