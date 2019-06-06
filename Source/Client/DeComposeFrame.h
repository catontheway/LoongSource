#pragma once
#include "ComposeStatic.h"
#include "CompClidateDefine.h"
#include "AppliedTree.h"
#include "..\WorldDefine\compose_define.h"
#include "ComColidateEvent.h"
#include "..\WorldDefine\msg_item.h"
#include "..\WorldDefine\msg_compose.h"
#include "ItemEvent.h"

struct tagUpdateLPSkillEvent;
class DeComposeFrame : public GameFrame
{
private:
	enum EDeComposeMsg
	{
		EDeCompose_NULL = 0,
		EDeCompose_SendMsg,		
		EDeCompose_Waiting,
		EDeCompose_RecvMsg,
	};
public:
	DeComposeFrame(void);
	~DeComposeFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual BOOL EscCancel();

	// ����������ʽ
	void SetProduceMode(EProduceType eType);

	// ��ʾ/���������ϳɴ���
	void ShowWnd(BOOL bVale) { if( P_VALID(m_pWndMain) ) m_pWndMain->SetInvisible(!bVale); }

	// �򿪽��淽ʽ��ID
	void SetOpenUIMode(EStartCompose eType, ComposeID uID);

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	DWORD NetRecvNS_Decomposition(tagNS_Decomposition* pMsg, LPVOID pPrama);

	//�����һ�װ������Ʒ��ӵ�����ϵͳ
	DWORD OnEventProduceAddItem(tagItemPosChangeExEvent* pGameEvent);

	//�϶��ͷ�
	DWORD HandleDragStuff(tagGUIEvent* pEvent);

	//�ж�������Ʒ�Ƿ���������
	BOOL JudgeSpecItem(ESpecFuncComposeAdvance eSpec);

	//����������Ϣ
	void SendDecomposeMsg();

	//�������ý���
	void ResetStartUIState();

	//������漰��������
	void ClearUIandStuff();

	//�ж��Ƿ��ܵ㻯��ֽ�
	void JudgeDecompositonFormula(DWORD dwItemID=GT_INVALID, INT64 n64ID=GT_INVALID, EEquipPos ePos=EEP_Null);

	//�ж��Ƿ���Է��͵㻯��Ϣ
	void JudgeSendDecompose();

	void PushButtonState(bool bEnable);

	//��������,ˢ�¼���
	DWORD OnUpdateLPSkillEvent(tagUpdateLPSkillEvent* pEvent);


private:
	TSFPTrunk<DeComposeFrame>			m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	GUIWnd*								m_pWndMain;
	GUIWnd*								m_pWndCaption;
	GUIWnd*								m_pWndBack;
	GUIButton*							m_pBtnExit;
	GUIButton*							m_pBtnCancel;
	GUIButton*							m_pBtnOk;
	GUIListBox*							m_pDeStuffList;			//�ɷֳ�����
	GUIStatic*							m_pStcTitle;			//����
	ComposeStatic*						m_pDeUnit;				//�ֽ���Ʒ
	ComposeStatic*						m_pDeSucc;				//��߳ɹ���
	GUIStatic*							m_pStcPotVal;			//����
	GUIStatic*							m_pStcGold;				//��
	GUIStatic*							m_pStcSilver;			//��
	GUIProgress*						m_pProTime;				//������
	GUIPushButton*						m_pPBtnType[PointUpSubTypeQuan];	//�㻯��

	DWORD								m_dwFormulaID;						//��ǰѡ����䷽ID
	DWORD								m_dwRecvMsgTime;					//�ɹ�\ʧ��\����ʱ��ʾ��ʱ��
	EProduceType						m_eProType;							//��������
	EComposeType						m_eComType;							//�ϳ�����(�������͵�����)
	CompConsoContainer					m_Container;						//��������
	EDeComposeMsg						m_eCurState;						//��ǰ��Ϣ״̬
	EStartCompose						m_eComposeType;						//��ʲô��ʽ�򿪵�UI
	ComposeID							m_uComposeID;
	DWORD								m_escapeTime;
};