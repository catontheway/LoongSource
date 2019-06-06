//-----------------------------------------------------------------------------
//!\file gui_msgbox.h
//!\author Lyp
//!
//!\date 2008-11-11
//! last 2008-11-11
//!
//!\brief ��׼��Ϣ��
//-----------------------------------------------------------------------------
#pragma once

enum MsgBoxFlag
{
	MBF_Null = 0,	// δȷ��
	MBF_OK = 1,		// ȷ��
	MBF_Cancel = 2,	// ȡ��
};


enum MsgBoxInputType
{
	MBIT_Number		=	0,	// ��ֵ
	MBIT_Password	=	1,	// ����
	MBIT_Name		=	2,	// ����
	MBIT_Text		=	3,	// ���֣����ı���
};

enum MsgBoxMoneyType
{
	MBMT_Null		=	0,
	MBMT_Silver		=	1,	// ����
	MBMT_Yuanbao	=	2,	// Ԫ��
};

// ����3ѡ�Ի���ѡ����
enum MsgOptionBoxFlag
{
	EMOBF_Option1 = 0,	
	EMOBF_Option2 = 1,
	EMOBF_Option3 = 2,
	EMOBF_Null
};

struct tagMsgBoxEvent : public tagGameEvent
{
	tagMsgBoxEvent(LPCTSTR szEventName, GameFrame* pSenderFrame):tagGameEvent(szEventName, pSenderFrame){}
	MsgBoxFlag eResult;
};

struct tagMsgInputBoxEvent : public tagGameEvent
{
	tagMsgInputBoxEvent(LPCTSTR szEventName, GameFrame* pSenderFrame):tagGameEvent(szEventName, pSenderFrame){}
	MsgBoxFlag		eResult;
	MsgBoxInputType eInputType;
	INT				nInputNum;
	tstring			strInputName;
};

struct tagMsgMoneyBoxEvent : public tagGameEvent
{
	tagMsgMoneyBoxEvent(LPCTSTR szEventName, GameFrame* pSenderFrame):tagGameEvent(szEventName, pSenderFrame){}
	MsgBoxFlag		eResult;
	MsgBoxMoneyType	eMonetType;
	INT				nYuanbao;
	INT64			n64Silver;
};

struct tagMsgOptionBoxEvent :public tagGameEvent
{
	tagMsgOptionBoxEvent(LPCTSTR szEventName, GameFrame *pSendFrame) : 
	tagGameEvent(szEventName, pSendFrame) {}
	MsgOptionBoxFlag eResult;	
};

/* 
	ʹ�÷���

	CreateObj("Box1", "MsgBox");
	TObjRef<MsgBox>("Box1")->Init(_T("Test"), _T("Hello World"), _T("event1"), MBF_OK, TRUE);

	MsgBoxFlag eResult = TObjRef<MsgBox>("Box1")->GetResult();
	KillObj("Box1");
*/



//-----------------------------------------------------------------------------
// MsgBox
//-----------------------------------------------------------------------------
class MsgBox
{
public:
	virtual BOOL Init(LPCTSTR szCaption, LPCTSTR szText, LPCTSTR szEvent=NULL, 
		MsgBoxFlag eFlag=(MsgBoxFlag)(MBF_OK|MBF_Cancel), BOOL bModal=FALSE);

	MsgBoxFlag GetResult() { return m_eResult; } // ����MBF_Null�����û�δȷ��

	MsgBox();
	virtual ~MsgBox();

private:
	TSFPTrunk<MsgBox>		m_Trunk;
	TObjRef<GUISystem>		m_pGUI;
	TObjRef<Util>			m_pUtil;
	TObjRef<GameFrameMgr>	m_pGameFrameMgr;
	
	GUIWnd*					m_pWnd;
	GUIPatch*				m_pWndBack;
	GUIStaticEx*			m_pWndText;
	GUIButton*				m_pWndOK;
	GUIButton*				m_pWndCancel;

	tstring					m_strCaption;
	tstring					m_strText;
	tstring					m_strEvent;

	BOOL					m_bModal;
	MsgBoxFlag				m_eFlag;
	MsgBoxFlag				m_eResult;

	DWORD					m_dwID;

	BOOL EventHandler(tagGUIEvent* pEvent);

	virtual BOOL LoadGUI();
};


/* 
ʹ�÷���

CreateObj("Box1", "MsgInputBox");
TObjRef<MsgInputBox>("Box1")->Init(_T("Test"), _T("Hello World"), _T("event1"), GT_INVALID, MBIT_Number, TRUE);

MsgBoxFlag eResult = TObjRef<MsgInputBox>("Box1")->GetResult();
KillObj("Box1");
*/

//--------------------------------------------------------------------------------
// MsgInputBox(�����)
//--------------------------------------------------------------------------------
class MsgInputBox
{
public:
	virtual BOOL Init(LPCTSTR szCaption, LPCTSTR szText, LPCTSTR szEvent=NULL,
		INT nMaxValue = GT_INVALID, MsgBoxInputType eInput=MBIT_Number, LPCTSTR szDefault=NULL, BOOL bModal=FALSE);

	VOID SetContent(LPCTSTR szContent, BOOL bSelectAll);

	MsgBoxFlag GetResult() { return m_eResult; } // ����MBF_Null�����û�δȷ��

	MsgInputBox();
	virtual ~MsgInputBox();

private:
	TSFPTrunk<MsgInputBox>	m_Trunk;
	TObjRef<GUISystem>		m_pGUI;
	TObjRef<Util>			m_pUtil;
	TObjRef<GameFrameMgr>	m_pGameFrameMgr;

	GUIWnd*					m_pWnd;
	GUIPatch*				m_pWndBack;
	GUIStaticEx*			m_pWndText;
	GUIButton*				m_pWndOK;
	GUIButton*				m_pWndCancel;
	GUIEditBox*				m_pWndInput;
	GUIEditBoxEx*			m_pWndInputEx;

	tstring					m_strCaption;
	tstring					m_strText;
	tstring					m_strEvent;

	BOOL					m_bModal;
	MsgBoxFlag				m_eResult;
	MsgBoxInputType			m_eInput;
	
	BOOL EventHandler(tagGUIEvent* pEvent);

	virtual BOOL LoadGUI();
};


//--------------------------------------------------------------------------
// MsgMoneyInputBox(��Ǯ�����)
//--------------------------------------------------------------------------
class MsgMoneyInputBox
{
public:
	virtual BOOL Init(LPCTSTR szCaption, LPCTSTR szText, LPCTSTR szEvent=NULL,
		INT64 n64MaxGold = 0, INT nMaxYuanBao = 0, INT64 n64Default = 0, INT nDefault = 0,
		MsgBoxMoneyType eInput=(MsgBoxMoneyType)(MBMT_Silver|MBMT_Yuanbao),
		BOOL bModal=FALSE);

	MsgBoxFlag GetResult() { return m_eResult; } // ����MBF_Null�����û�δȷ��

	MsgMoneyInputBox();
	virtual ~MsgMoneyInputBox();

private:
	TSFPTrunk<MsgMoneyInputBox>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;

	GUIWnd*						m_pWnd;
	GUIPatch*					m_pWndBack;
	GUIStaticEx*				m_pWndText;
	GUIButton*					m_pWndOK;
	GUIButton*					m_pWndCancel;
	GUIEditBox*					m_pWndYuanBao;
	GUIEditBox*					m_pWndGold;
	GUIEditBox*					m_pWndSilver;

	tstring						m_strCaption;
	tstring						m_strText;
	tstring						m_strEvent;

	BOOL						m_bModal;
	MsgBoxFlag					m_eResult;
	MsgBoxMoneyType				m_eInput;
	INT64						m_n64MaxGold;

	BOOL EventHandler(tagGUIEvent* pEvent);

	virtual BOOL LoadGUI();
};


//--------------------------------------------------------------------------
// MsgPasswordCheckBox(����ȷ�Ͽ�)
//--------------------------------------------------------------------------
class MsgPasswordCheckBox
{
	
};




/*!
\brief ����3ѡ�Ի�����
*/
class MsgOptionBox
{
public:
	MsgOptionBox();

	virtual ~MsgOptionBox();


	/*!
	\brief ��ʼ��
	*/
	virtual BOOL Init(LPCTSTR szCaption, LPCTSTR szDescription, 
					  LPCTSTR szOptionText1, LPCTSTR szOptionText2, LPCTSTR szOptionText3, 
					  bool bOption1, bool bOption2, bool bOption3,
					  LPCTSTR szEvent = NULL, BOOL bModal = FALSE);

	/*!
	\brief ȡ��ѡ����
	*/
	MsgOptionBoxFlag GetResult() const { return m_eResult; }

private:
	TSFPTrunk<MsgOptionBox>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;

	GUIWnd*						m_pWnd;
	GUIPatch*					m_pWndBack;
	GUIButton*					m_pBtnCancel;	// ȡ����ť
	GUIButton*					m_pBtnSure;		// ȷ����ť
	GUIStaticEx*				m_pSeDescption;	// ��������
	GUIStaticEx*				m_pSeOption1;	// ѡ��1����
	GUIStaticEx*				m_pSeOption2;	// ѡ��2����
	GUIStaticEx*				m_pSeOption3;	// ѡ��3����
	GUIFrameMutex*				m_pFrameMutex;	// ������
	GUIPushButton*				m_pBtnOption1;	// ѡ��1��ť	
	GUIPushButton*				m_pBtnOption2;	// ѡ��2��ť
	GUIPushButton*				m_pBtnOption3;	// ѡ��3��ť

	tstring						m_strEvent;		// �¼���
	BOOL						m_bModal;		// �Ƿ�ģ̬
	MsgOptionBoxFlag			m_eResult;		// ѡ����
	MsgOptionBoxFlag			m_eTemp;		// ��ʱ���

	/*!
	\brief ����GUI
	*/
	virtual BOOL LoadGUI();

	/*!
	\brief �¼�������
	*/
	BOOL EventHandler(tagGUIEvent *pEvent);
};


//--------------------------------------------------------------------------------
// MsgBoxEx(ֻ��һ��ȷ����ť)
//--------------------------------------------------------------------------------

/* 
ʹ�÷���

CreateObj("Box1", "MsgBoxEx");
TObjRef<MsgBoxEx>("Box1")->Init(_T("Test"), _T("Hello World"), _T("event1"), );

MsgBoxFlag eResult = TObjRef<MsgBoxEx>("Box1")->GetResult();
KillObj("Box1");
*/

class MsgBoxEx
{
public:
	virtual BOOL Init(LPCTSTR szCaption, LPCTSTR szText, LPCTSTR szEvent=NULL, 
		MsgBoxFlag eFlag=(MsgBoxFlag)(MBF_OK), BOOL bModal=TRUE, BOOL bTop=FALSE);

	MsgBoxFlag GetResult() { return m_eResult; } // ����MBF_Null�����û�δȷ��

	MsgBoxEx();
	virtual ~MsgBoxEx();

private:
	TSFPTrunk<MsgBoxEx>		m_Trunk;
	TObjRef<GUISystem>		m_pGUI;
	TObjRef<Util>			m_pUtil;
	TObjRef<GameFrameMgr>	m_pGameFrameMgr;

	GUIWnd*					m_pWnd;
	GUIPatch*				m_pWndBack;
	GUIStaticEx*			m_pWndText;
	GUIButton*				m_pWndOK;

	tstring					m_strCaption;
	tstring					m_strText;
	tstring					m_strEvent;

	BOOL					m_bModal;
	MsgBoxFlag				m_eFlag;
	MsgBoxFlag				m_eResult;

	DWORD					m_dwID;

	BOOL EventHandler(tagGUIEvent* pEvent);

	virtual BOOL LoadGUI();
};