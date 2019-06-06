/********************************************************************
	created:	2008/08/16
	created:	16:8:2008   17:18
	filename: 	d:\loong_client\Client\NPCTalkFrame.h
	file path:	d:\loong_client\Client
	file base:	NPCTalkFrame
	file ext:	h
	author:		leili
	
	purpose:	��ͨNPC�Ի���������
*********************************************************************/
#include "Item.h"

#pragma once

class NPCTalkFrame : public GameFrame
{
public:
	enum EOptionType
	{
		EOT_None			= -1,		//
		EOT_AcceptQuest		= 0,		// �ɽ�����ѡ��
		EOT_CompleteQuest,				// �ɽ�����ѡ��
		EOT_ScenarioTalk,				// �龰�Ի�ѡ��
		EOT_ProduceSkillTalk,			//
		EOT_LearnProduceSkill,			//
		EOT_LeaveProduceSkill,			//
		EOT_CancelProduceSkill,			//
		EOT_ServerCtrl,					//	����������ѡ��
		EOT_GuildCommerce,				// ��������
        EOT_SetTransportPoint,          // ���ÿɼ�¼���͵�(added by hyu on 2009-11-27)
	};

public:
	NPCTalkFrame(void);
	~NPCTalkFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	VOID		 Show(void);
	VOID		 Hide(void);

	VOID		 SetCaption(LPCTSTR szCaption);
	VOID		 SetContent(LPCTSTR szContent);
	VOID		 ClearOptions(void);
	VOID		 AddTalkOption(LPCTSTR szOption, DWORD param1, DWORD param2, EOptionType optionType);
	VOID		 AddAcceptOptions(DWORD npcID);
	VOID		 AddCompleteOptions(DWORD npcID);

	VOID		 SetTarget(INT targetType, DWORD targetID) { m_targetType=targetType;m_targetID=targetID; }

    void SetTransItem(Item* pItem) { m_pTransportItem = pItem; }

    void SetTransportItemEnable(bool bEnabled) { m_isUsingTransportItem = bEnabled; }

private:
	BOOL		 EventHandler(tagGUIEvent* pEvent);

private:
	TSFPTrunk<NPCTalkFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;

	GUIWnd						*m_pWnd;				//������
	GUIPatch					*m_pCaption;			//����
	GUIStaticEx					*m_pStcCondition;		//�Ի����ݴ���
	GUIListBox					*m_pListOptions;		//�Ի�ѡ��
	GUIButton					*m_pBtnCloseWnd;		//�رս��水ť����

	DWORD						m_curRow;
	DWORD						m_escapeTime;			//����

	struct tagOptAttr 
	{
		DWORD		id;
		DWORD		step;
		EOptionType	type;
	};
	vector<tagOptAttr>			m_optAttrs;

	INT							m_targetType;
	DWORD						m_targetID;

    Item*                       m_pTransportItem;       // �ɼ�¼���ͷ�item
    bool                        m_isUsingTransportItem; // �Ƿ�����ʹ��TransportItem
};