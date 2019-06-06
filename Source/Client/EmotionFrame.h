#pragma once

#include ".\ChatSysDefine.h"

/**
	���飬��emotion.xml�ж�ȡ����ת����Լ����Ľ���
*/
class GameFrame;
//���촰�ڵ���Ϸ�¼�
struct tagGameEventForChat:public tagGameEvent
{
	tstring		strEmotion;
	tagGameEventForChat(LPCTSTR szEventName, GameFrame* pSenderFrame, LPCTSTR szEmotion)
		:tagGameEvent(szEventName, pSenderFrame)
	{
		strEmotion = szEmotion;	
	}
};

class EmotionFrame:
	public GameFrame
{
public:
	EmotionFrame(void);
	~EmotionFrame(void);
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	//�������ļ��ж�ȡ�����ת���ַ��Լ�����ڹ��
	VOID LoadEmotionFile(const tstring& szFile);
	
	//��ʾ\���ش���
	VOID SetInVisibleFrame(bool bVal); 
	BOOL IsInvisibleFrame() { return m_pWnd->IsInvisible(); }

	//�õ����������ڹ��
	const TCHAR* GetEmtionTran(int index);

	//
	const TCHAR* GetEmotionMean(int index);

private:

	//GUI�¼�������
	BOOL EventHandler(tagGUIEvent* pEvent);

private:
	TSFPTrunk<EmotionFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;
	GUIButton*					m_pBtnEmotion[MAX_EMOTION];		//���鰴ť(60��)

	vector<tstring>				m_vecEmotionTran;		//���������ڹ��
	vector<tstring>				m_vecEmotionMean;		//�����ת���
};
