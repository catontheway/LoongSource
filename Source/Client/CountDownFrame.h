#pragma once

class CountDownFrame : public GameFrame 
{
public:
	CountDownFrame();
	~CountDownFrame();
	/*!
	\brief ��ʼ��
	*/
	virtual BOOL Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam);

	/*!
	\brief ����
	*/
	virtual BOOL Destroy();
	
	/*!
	\brief ����UI
	*/
	virtual BOOL ReloadUI();

	/*!
	\brief ����
	*/
	virtual VOID Update();
	
	/*!
	\brief ���ؽ���
	*/
	VOID Hide();

	VOID Show();

	/*!
	\brief ���ø���ʣ��ʱ��
	*/
	VOID SetCDTime(DWORD dwCDTime);
private:
	TObjRef<GUISystem>		m_pGUI;
	TObjRef<GameFrameMgr>	m_pFrameMgr;
	
	GUIWnd					*m_pWnd;
	GUIStatic				*m_pCDTime;
	GUIStatic				*m_pLeftTime;
	int						m_nCDTime;		//����ʣ��ʱ��
};