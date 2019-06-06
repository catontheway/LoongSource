//-----------------------------------------------------------------------------
//!\file gui_event_define.h
//!\author Lyp
//!
//!\date 2005-04-29
//! last 2005-04-29
//!
//!\brief ����ϵͳ����¼�����
//-----------------------------------------------------------------------------
#pragma once

namespace vEngine {
//-----------------------------------------------------------------------------
// �¼�ö��
//-----------------------------------------------------------------------------
enum EGUIEvent
{
	EGUIE_Null=0,
	EGUIE_Create,					// �ؼ��ձ�����
	EGUIE_Destroy,					// �ؼ�������
	EGUIE_Click,					// �ؼ���һ�������ĵ��
	EGUIE_PushButtonDown,			// PushButton����
	EGUIE_EditBoxEnter,				// �û���edit box�ڰ��»س�
	EGUIE_UserChangeListContext,	// �û��ı�List����
	EGUIE_ItemClick,				// ��Ŀ�����
	EGUIE_ItemDblClick,				// ��Ŀ��˫��
	EGUIE_Scroll,					// ������֮��Ĺ���, dwParam1=��ǰֵ, Param2=��ֵ
	EGUIE_Active,					// ������
	EGUIE_Deactive,					// ���ٱ�����
	EGUIE_FrameMutex,				// frame�ڲ��Ļ����¼�dwParam = 0���� 1.
	EGUIE_RichLinkClick,			// richeditbox�г����ӱ����(˫�����ߵ���������)
	EGUIE_DragStart,				// �϶���ʼ
	EGUIE_DragEnd,					// �϶�ֹͣ
	EGUIE_DragOver,					// �϶�����
	EGUIE_Drag,						// �ƶ��¼�
	EGUIE_Resize,					// �ı��С�¼�
	EGUIE_KeyDown,					// VIRTUAL_KEY_DOWN�¼�
	EGUIE_CursorMove,				// ����ƶ�(EditBoxEx)
	EGUIE_MoveIntoCtrl,				// �������ÿؼ�	
	EGUIE_MoveOutOfCtrl,			// ����Ƴ��ÿؼ�
	EGUIE_StaticExLinkClick,		// StaticEx�г����ӱ����(˫�����ߵ���������)

	EGUIE_User = 0x80000000,		// �ͻ��Զ����¼�
	EGUIE_End  = 0xffffffff,
};



//-----------------------------------------------------------------------------
// �¼����ݽṹ
//-----------------------------------------------------------------------------
struct tagGUIEvent
{
	DWORD			dwWndID;		// ʵ�ʷ����¼��Ŀؼ�
	EGUIEvent		eEvent;			// �¼�����
	DWORD			dwParam1;		// �¼�����
	DWORD			dwParam2;
	DWORD			dwParam3;
	DWORD			dwParam4;
	tagGUIInputMsg	msg;			// �����¼�����Ϣ
	DWORD			dwSendWndID;	// ���ʹ��¼��Ŀؼ�


	tagGUIEvent(GUIWnd* pGUIWnd, EGUIEvent event, tagGUIInputMsg* pM=NULL, 
		DWORD dw1=0, DWORD dw2=0, DWORD dw3=0, DWORD dw4=0)
		:dwWndID(GT_INVALID), eEvent(event), dwParam1(dw1), dwParam2(dw2), dwParam3(dw3), dwParam4(dw4)
	{ 
		if( P_VALID(pGUIWnd) )
			dwWndID = pGUIWnd->GetID();

		if(pM) 
			memcpy(&msg, pM, sizeof(msg)); 
	}
};

}	// namespace vEngine {
