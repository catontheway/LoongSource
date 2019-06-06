#pragma once
#include "gameframe.h"

/**
	\brief �����͸� �����ֵ�¼��Ϸ����10����Ϊ��λ�������������
		   ��Ҫ��Ϊ�˷�ֹ���ֵ���ʧ
    \auth  dyb
*/
class IconStatic;
class ServerTime;
struct tagNS_GetLoongBenediction;
struct tagNS_SetNextBenediction;

class LoongBenedictionFrame : public GameFrame
{
public:
	LoongBenedictionFrame(void);
	~LoongBenedictionFrame(void);

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual VOID Update();
	virtual BOOL ReloadUI();

private:
	BOOL EventHandler( tagGUIEvent* pEvent );
	VOID SafeDestroyUI();

	/** ��ý���
	*/
	VOID GetBenediction();
	
	/** ��������Ի�ȡ�����ķ����¼�
	*/
	DWORD OnNS_GetBenediction( tagNS_GetLoongBenediction* pMsg, DWORD dwParam );

	/** ������������Ϣ������һ�δ͸�ʱ�䣬�Լ���Ʒ��Ϣ
	*/
	DWORD OnNS_SetNextBenediction( tagNS_SetNextBenediction* pMsg, DWORD dwParam );

	/** �жϵ�ǰ�����Ƿ���չ����
	*/
	BOOL IsWindowExpand();

	VOID ShowWnd( GUIWnd* pWnd, BOOL bShow );

	VOID EnableWnd( GUIButton* pBtn, BOOL bEnable );

private:
	TSFPTrunk<LoongBenedictionFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;

	GUIWnd*							m_pWndExpand;
	GUIStatic*						m_pStcWaitTime;			//������һ�������͸���ʱ�䣬����ʱ
	GUIButton*						m_pBtnGetBenediction;	//��ȡ
	GUIButton*						m_pBtnArrow;			//�۵�
	GUIStatic*						m_pStcExprence;			//��õľ���

	IconStatic*						m_pStcItem;				//��õĵ���

	GUIWnd*							m_pWndFold;					//�۵�����֮��Ľ���
	GUIStatic*						m_pStcFoldWaitTime;			//������һ�������͸���ʱ�䣬����ʱ
	GUIButton*						m_pBtnFoldGetBenediction;	//��ȡ
	GUIButton*						m_pBtnFoldArrow;			//�۵�
	GUIWnd*							m_pWndCurDisplay;			//�õ���ǰ������ʾ�Ŀؼ�

	DWORD							m_nItemTypeId;				//��Ҫ��õĵ��ߵ�ID
	INT								m_nItemCount;				//������Ʒ�ĸ���
	//DWORD							m_dwExprence;				//��Ҫ��õľ���ֵ

	DWORD							m_dwNeedTime;				//����һ�ν�����Ҫ������
	float							m_dwStepTime;				//����ʱ
	DWORD							m_dwStartTimer;				//��ʱ��

	BOOL							m_bCanUpdate;				//�ܷ���£���û���յ������������� ������һ�δ͸�ʱ�� ֮ǰ�ǲ��ܸ��µ�
};
