/********************************************************************
	created:	2008/08/16
	created:	16:8:2008   17:36
	filename: 	d:\loong_client\Client\QuestActiveFrame.h
	file path:	d:\loong_client\Client
	file base:	QuestActiveFrame
	file ext:	h
	author:		leili
	
	purpose:	�̶�������������
*********************************************************************/
#pragma once

#include "QuestQuery.h"
#include "ActivityMgr.h"

class QuestActiveFrame : public GameFrame
{
public:
	QuestActiveFrame(void);
	~QuestActiveFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	VOID		 Show(void);
	VOID		 Hide(void)		{ if( P_VALID(m_pWnd) ) m_pWnd->SetInvisible(TRUE); }

private:
	BOOL		 EventHandler(tagGUIEvent* pEvent);

	VOID		 ShowQuest(void);
	VOID		 ShowDesc(UINT16 questID);
	bool		 CompareTime(int SrcHour, int SrcMinute, int DestHour, int DestMinute)
	{
		if (((SrcHour * 60 + SrcMinute) - (DestHour * 60 + DestMinute)) > 0)
			return true;
		return false;
	}

private:
	TSFPTrunk<QuestActiveFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;

	GUIWnd						*m_pWnd;				//������
	GUIStaticEx					*m_pStcDesc;			//������������
	GUIListBox					*m_pListQuests;			//�����б�
	GUIButton					*m_pBtnTime;			//��ȡʱ�䰴ť
	GUIButton					*m_pBtnName;			//�������ư�ť
	GUIButton					*m_pBtnLimit;			//�ȼ����ư�ť
	GUIButton					*m_pBtnNPC;				//��ȡNPC��ť
	GUIButton					*m_pBtnCloseWnd;		//�رմ��ڰ�ť
	GUIPushButton				*m_pPshSwitchCan;		//���˿����

	ActivityMgr::ESortType		m_curSortType;
	DWORD						m_curRow;
	bool						m_showCan;
	ActivityMgr::ESortType		m_sortType;
	std::vector<int>			m_curShowQuestIDs; 
};