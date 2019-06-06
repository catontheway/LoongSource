/********************************************************************
	created:	2008/08/16
	created:	16:8:2008   18:02
	filename: 	d:\loong_client\Client\QuestNPCFrame.h
	file path:	d:\loong_client\Client
	file base:	QuestNPCFrame
	file ext:	h
	author:		leili
	
	purpose:	�ɽ�����NPC��������
*********************************************************************/
#pragma once

class QuestNPCFrame : public GameFrame
{
public:
	QuestNPCFrame(void);
	~QuestNPCFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	VOID		 Show(void);
	VOID		 Hide(void);

	BOOL		 IsVisible(void) const;
	BOOL		 ReloadUI(LPCSTR alignCtrl, LPCSTR alignType, LPCSTR szNewName = NULL );
	VOID		 Align(void);

	VOID		 UpdateNPCList(void);
	VOID         UpdateUI(void);

private:
	BOOL		 EventHandler(tagGUIEvent* pEvent);
	DWORD		 OnEventQuestFilterConfigChange(tagGameEvent *pEvent);

private:
	TSFPTrunk<QuestNPCFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;

	GUIWnd						*m_pWnd;				//������
	GUIListBox					*m_pListNPCs;			//NPC�б�
	GUIButton					*m_pBtnCloseWnd;		//�رմ��ڰ�ť
	GUIPushButton				*m_pPbnFilterByLevel;	//���˵͵ȼ�����
	GUIPushButton				*m_pPbnFilterByType;	//�����͹�������
	GUIPushButton				*m_pPbnType1;			//����һ
	GUIPushButton				*m_pPbnType2;			//������
	GUIStatic					*m_pStcType1;			//����һ˵��
	GUIStatic					*m_pStcType2;			//������˵��
	GUIButton					*m_pBtnConfig;			//�ֶ������������
	

	
	DWORD						m_curRow;
	vector<DWORD>				m_optflags;
};