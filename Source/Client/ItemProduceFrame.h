#pragma once
#include "ComposeStatic.h"
#include "CompClidateDefine.h"
#include "AppliedTree.h"
#include "..\WorldDefine\compose_define.h"
#include "ComColidateEvent.h"
#include "..\WorldDefine\msg_item.h"
#include "..\WorldDefine\msg_compose.h"
#include "ItemEvent.h"
/*
	class ItemSmithComposeFrame
	brief ��Ʒ�����ϳ�
*/

class NetSession;
struct tagUpdateLPSkillEvent;
class ItemProduceFrame :
	public GameFrame
{
private:
	enum EItemProduceMsg
	{
		EEItemProduce_NULL = 0,
		EEItemProduce_SendMsg,		
		EEItemProduce_Waiting,
		EEItemProduce_RecvMsg,
	};

public:
	ItemProduceFrame(void);
	~ItemProduceFrame(void);

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
	BOOL EventHandler(tagGUIEvent* pEvent);

	//--�����䷽
	void SetFormula(EProduceType eType);
	void AddFormula2Tree(EProduceType eProType, EComposeType eComType);
	void DeleteFormula();
	//�㻯�䷽���⴦��
	void SetFormulaForPointUp();

	// �����䷽���ò���
	void SetStuffOnFormula(EProduceType eType, DWORD dwID);

	// �����������
	void ClearStuffSelectFormula();

	// ��տؼ������������
	void ClearStuff();

	// ��ʼ����,�жϲ����Ƿ������䷽
	void JudgeStuffIsContentForm(EProduceType eType, DWORD dwID);

	// ��κϳ�ʱ,���ϳ���һ��ʱ,�����жϲ��ϵĸ����Ƿ������䷽Ҫ��
	bool ReCountStuff();

	// ���ͺϳ���Ϣ
	void SendProduceMsg();
	void SendDeComposeMsg();
	DWORD NetRecvItemUpdate(tagNS_ItemRemove* pMsg, DWORD pPrama);
	DWORD NetRecvNS_Produce(tagNS_Produce* pMsg, DWORD pPrama);
	DWORD NetRecvNS_Decomposition(tagNS_Decomposition* pMsg, DWORD pPrama);

	BOOL HandleDragStuff(tagGUIEvent* pEvent);

	//�ж�������Ʒ�Ƿ���������
	BOOL JudgeSpecItem(ESpecFuncComposeAdvance eSpec);

	void SetStcCtrlState(EProduceType eType);

	void ResetStartState();

	//��������,ˢ���䷽
	DWORD OnUpdateLPSkillEvent(tagUpdateLPSkillEvent* pEvent);
	
	//�����һ�װ������Ʒ��ӵ�����ϵͳ
	DWORD OnEventProduceAddItem(tagItemPosChangeExEvent* pGameEvent);

	void PushButtonState(bool bEnable);

	// 
	void UtilCalculateSuccessRatio();

	void HandleItem( bool bOperable );


private:
	TSFPTrunk<ItemProduceFrame>			m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	GUIWnd*						m_pWndMain;
	GUIWnd*						m_pWndCaption;
	GUIStatic*					m_pWndName;				//�ϳɴ��ڱ���
	GUIButton*					m_pBtnExit;				//�˳�

	GUIStatic*					m_pStcComBack;
	GUIButton*					m_pBtnOk;				//ȷ��
	GUIButton*					m_pBtnCancel;			//ȡ��
	GUIStatic*					m_pStcNotice;			//����

	GUIPatch*					m_pStcTreeBack;
	AppliedTree*				m_pTreeList;			//�䷽�б�
	GUIStatic*					m_pStcListTitle;		//�䷽����

	GUIStatic*					m_pStcSuccTitle;		//�ɹ��ʱ���
	ComposeStatic*				m_pStcSuccCtrl;			//������߳ɹ��ʵ���Ʒ��ť
	GUIStatic*					m_pStcSucc;				//�ɹ�����ʾ
	GUIStatic*					m_pStcSuccOdds;			//��ʾ�ɹ���
	GUIStatic*					m_pStcModi;				//�޸�����ʾ
	GUIStatic*					m_pStcModiOdds;			//������

	GUIStaticEx*				m_pStcNeedShow;			//��ʾ

	GUIStatic*					m_pStcGolden;			//�����ʾ
	GUIStatic*					m_pStcSiller;			//������ʾ

	GUIEditBox*					m_pEditComNum;			//����ϳɵ���Ŀ
	GUIStatic*					m_pStcConNum;			//ǿ����Ŀ��ʾ
	GUIStatic*					m_pStcMasterHint;		//��������ʾ
	GUIStatic*					m_pStcSuccMaster;		//������
	GUIStatic*					m_pStcPotValHint;		//������ʾ
	GUIStatic*					m_pStcPotVal;			//��Ҫ���Ļ���

	GUIStatic*					m_pStcSuccHint;			//�ɹ���ʾ
	GUIStatic*					m_pStcLoseHint;			//ʧ����ʾ
	GUIStatic*					m_pStcPerfectHint;		//������ʾ

	GUIProgress*				m_pProTime;				//������
	GUIPushButton*				m_pPBtnFormular[FORMULA_QUANTITY];			//�䷽pushbutton
	ComposeStatic*				m_pStcCtrl[CTRL_QUANTITY];					//�ϳɿؼ�
	GUIStatic*					m_pStcMainStuffHint;						//������ʾ
	GUIStatic*					m_pStcStuffHint[STUFF_QUANTITY];			//���ϵ���ʾ
	GUIStatic*					m_pStcStuffQuan[STUFF_QUANTITY];			//��������/����

	DWORD						m_dwItemSuccID;						//��߳ɹ��ʵ���ƷID
	DWORD						m_dwStartTime;						//�ɹ�\ʧ��\����ʱ��ʾ��ʱ��
	DWORD						m_dwRecvMsgTime;					//�ɹ�\ʧ��\����ʱ��ʾ��ʱ��
	INT							m_nProduceNum;						//�ϳɴ���

	typedef std::map<DWORD, DWORD> TreeFormularMap;				
	TreeFormularMap				m_tree2formula;						//��ID��Ӧ���䷽ID
	DWORD						m_dwFormulaID;						//��ǰѡ����䷽ID

	EProduceType				m_eProType;							//��������
	EComposeType				m_eComType;							//�ϳ�����(�������͵�����)
	CompConsoContainer			m_Container;						//��������
	
	EItemProduceMsg				m_eCurState;						//��ǰ��Ϣ״̬

	EStartCompose				m_eComposeType;						//��ʲô��ʽ�򿪵�UI
	ComposeID					m_uComposeID;
	DWORD						m_escapeTime;
};
