/********************************************************************
	created:	2008/08/15
	created:	15:8:2008   17:16
	filename: 	d:\loong_client\Client\QuestTalkFrame.h
	file path:	d:\loong_client\Client
	file base:	QuestTalkFrame
	file ext:	h
	author:		leili
	
	purpose:	����Ի���������
*********************************************************************/
#pragma once

#include "IconStatic.h"

class QuestTalkFrame : public GameFrame
{
public:
	QuestTalkFrame(void);
	~QuestTalkFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	VOID		 Show(void);
	VOID		 Hide(void);
	BOOL		 IsVisible(void) const;

	VOID		 SetCaption(LPCTSTR szCaption);
	VOID		 SetContent(UINT16 questID, LPCTSTR szContent, bool accept);

	VOID		 SetCanSelect(bool bCanSel) { m_canSelect = bCanSel; }
	VOID		 SetTrigger(bool bIsTrigger){ m_isTrigger = bIsTrigger; }

private:
	BOOL		 EventHandler(tagGUIEvent* pEvent);
	bool		 AlreadySelRewItem(int& index);

	VOID		 SetSpeRew(const tagQuestProto* pQuestProto);

private:
	TSFPTrunk<QuestTalkFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;

	GUIWnd						*m_pWnd;				//������
	GUIPatch					*m_pCaption;			//����
	GUIStaticEx					*m_pStcCondition;		//�Ի����ݴ���

	GUIStatic					*m_pStcBonusExp;		//���齱��
	GUIStatic					*m_pStcBonusMoney1;		//��Ǯ����1
	GUIStatic					*m_pStcBonusMoney2;		//��Ǯ����2
	vector<GUIStatic*>			m_pStcBonusSpecT;
	vector<GUIStatic*>			m_pStcBonusSpecV;

	typedef std::vector<IconStatic*> GUIStaticList;
	GUIStaticList				m_stcBonusGoods;		//��Ʒ����
	GUIStaticList				m_stcBonusSels;			//��ѡ����
	GUIStaticList				m_stcBonusGoodOuters;	//��Ʒ����ѡ���
	GUIStaticList				m_stcBonusSelOuters;	//��ѡ����ѡ���
	DWORD						m_BonusGoodsID[QUEST_REW_ITEM];
	DWORD						m_BonusSelsID[QUEST_REW_ITEM];

	GUIButton					*m_pBtnCloseWnd;		//�ر����������水ť
	GUIButton					*m_pBtnOK;				//ȷ����ť
	GUIButton					*m_pBtnCancel;			//ȡ����ť

	UINT16						m_questID;
	bool						m_isAccept;

	bool						m_selIndex[QUEST_REW_ITEM];
	bool						m_canSelect;			//�Ƿ����ѡ���ѡ����
	bool						m_isDynamic;			//�Ƿ�̬����
	bool						m_hasRewChoicesItem;
	bool						m_isTrigger;			//�Ƿ��ɴ���������

	DWORD						m_escapeTime;			//����
};