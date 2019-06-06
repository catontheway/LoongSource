/********************************************************************
	created:	2008/11/04
	created:	4:11:2008   13:55
	filename: 	d:\loong_client\Client\QuestTrackFrame.h
	file path:	d:\loong_client\Client
	file base:	QuestTrackFrame
	file ext:	h
	author:		leili
	
	purpose:	����׷�ٽ�������
*********************************************************************/
#pragma once

class QuestTrackFrame : public GameFrame
{
public:
	QuestTrackFrame(void);
	~QuestTrackFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();

	VOID		 Show(void);
	VOID		 Hide(void);
	BOOL		 IsVisible(void) const;

	VOID		 UpdateQuestTrack(void);
	BOOL		 CanTrackQuest(UINT16 questID);

private:
	BOOL		 EventHandler(tagGUIEvent* pEvent);
	BOOL		 AddQuestTrack(UINT16 questID);

private:
	TSFPTrunk<QuestTrackFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;

	GUIWnd						*m_pWnd;				//������
	GUIStaticEx					*m_pQuest;				//��������
	GUIButton					*m_pBtnCloseWnd;		//�ر�������ٽ��水ť

	int							m_trackCount;			//���ٵ���������
};