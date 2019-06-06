/********************************************************************
	created:	2009/08/07
	created:	7:8:2009   18:12
	filename: 	d:\loong_client\Client\QuizFrame.h
	file path:	d:\loong_client\Client
	file base:	QuizFrame
	file ext:	h
	author:		leili
	
	purpose:	�����������
*********************************************************************/
#pragma once

class QuizFrame : public GameFrame
{
public:
	QuizFrame(void);
	~QuizFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	VOID		 Show(void);
	VOID		 Hide(void);

	VOID		 SetContent(LPCTSTR szContent);
	VOID		 SetOption(LPCTSTR szOption, DWORD param1, DWORD param2);
	VOID		 SetCD(DWORD cd);
	VOID		 SetNum(DWORD num);
	VOID		 Clear(void);

	VOID		 SetTarget(INT targetType, DWORD targetID) { m_targetType=targetType;m_targetID=targetID; }

private:
	BOOL		 EventHandler(tagGUIEvent* pEvent);

private:
	TSFPTrunk<QuizFrame>	m_Trunk;
	TObjRef<GUISystem>		m_pGUI;

	GUIWnd					*m_pWnd;				// ������
	GUIStatic				*m_pStcDesc;			// ��Ŀ����
	GUIStatic				*m_pStcOptA;			// ѡ��A
	GUIStatic				*m_pStcOptB;			// ѡ��B
	GUIStatic				*m_pStcOptC;			// ѡ��C
	GUIStatic				*m_pStcOptD;			// ѡ��D
	GUIPushButton			*m_pPshA;				// ѡ���A
	GUIPushButton			*m_pPshB;				// ѡ���B
	GUIPushButton			*m_pPshC;				// ѡ���C
	GUIPushButton			*m_pPshD;				// ѡ���D
	GUIStatic				*m_pStcCD;				// ����ʱ
	GUIStatic				*m_pStcNum;				// ������Ŀ
	GUIButton				*m_pBtnSubmit;			// �ύ
	GUIButton				*m_pBtnCancel;			// ����
	GUIButton				*m_pBtnCloseWnd;		// �رս��水ť����

	DWORD					m_curOpt;				// ��ǰѡ������
	DWORD					m_questTime;			// ����ʱ��
	DWORD					m_escapeTime;			// ������ȥʱ��

	struct tagOptAttr 
	{
		DWORD		id;
		DWORD		step;
	};
	vector<tagOptAttr>		m_optAttrs;

	INT						m_targetType;
	DWORD					m_targetID;
};