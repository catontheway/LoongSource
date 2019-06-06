#pragma once
#include "ComposeStatic.h"
#include "CompClidateDefine.h"
#include "AppliedTree.h"
#include "..\WorldDefine\msg_compose.h"
#include "..\WorldDefine\compose_define.h"
#include "ComColidateEvent.h"
#include "ItemEvent.h"
/*
class EquipConsolidateFrame
brief װ��ǿ��(���ġ��Կ̡����)
*/
class NetSession;
class EquipConsolidateFrame :
	public GameFrame
{
private:
	enum EEquipConsolidate
	{
		EEConsolidate_NULL = 0,
		EEConsolidate_SendMsg,	
		EEConsolidate_Waiting,
		EEConsolidate_RecvMsg,
	};
public:
	EquipConsolidateFrame(void);
	~EquipConsolidateFrame(void);

	//--GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual BOOL EscCancel();

	// ����ǿ����ʽ
	void SetConsolidateMode(EConsolidateType eType);

	// ��ʾ/����ǿ������
	void ShowWnd(BOOL bVale) { if( P_VALID(m_pWndMain) ) m_pWndMain->SetInvisible(!bVale); m_eCurState=EEConsolidate_NULL;}

	// �򿪽��淽ʽ��ID
	void SetOpenUIMode(EStartCompose eType, ComposeID uID);

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	// ��ղ��ϡ����Ϻ���߳ɹ��ʰ�ť
	void ClearStuff();

	// ��ѡ����䷽��������ϼ����ϵ���ʾ
	void ClearStuffSelectFormula();

	//--�䷽
	void SetFormula(BYTE byTimes);
	void DeleteFormula();
	void AddFormula2Tree(EConsolidateType eType, DWORD dwFormulaID);

	// �ж��䷽ǿ������
	bool JudgeFormulaForCliDate(EConsolidateType eType, DWORD dwID);

	// ���䷽���ø���
	void SetStuffOnFormula(EConsolidateType eType,  DWORD dwID);

	//--����������Ϣ
	DWORD NetRecvConsolidatePosy(tagNS_ConsolidatePosy* pMsg, DWORD pPrama);
	DWORD NetRecvConsolidateEngrave(tagNS_ConsolidateEngrave* pMsg, DWORD pPrama);
	DWORD NetRecvConsolidateQuench(tagNS_ConsolidateQuench* pMsg, DWORD pPrama);
	void SendConsolidateMsg();

	// �յ�������Ϣ�������������ϣ���ˢ���䷽����ո���
	void ResetStuffFormular(); 

	DWORD HandleDragStuff(tagGUIEvent* pEvent);

	BOOL JudgeSpecItem(ESpecFuncComposeAdvance eSpec);

	//�����һ�װ������Ʒ��ӵ�ǿ��ϵͳ
	DWORD OnEventConsolidateAddItem(tagItemPosChangeExEvent* pGameEvent);

	//�������������UI,���������䷽
	void OperatedClear();

	//
	bool RefreshItemNum2UI();

	//�������ճɹ���
	void UtilCalculateSuccessRatio();

	float SetSuccessRatioAndGetStuffEffect(DWORD dwSrcSuccess);

	// ���ĳɹ������ӵ�����
	void ShowNewPosyAtt( const tagPosyProto *pPosy, float fStuffEffect );

	// �Կ̳ɹ������ӵ�����
	void ShowNewEngraveAtt( const tagEngraveProto *pEngrave, float fStuffEffect );

	void HandleItem( bool bOperable );

private:
	TSFPTrunk<EquipConsolidateFrame>	m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	GUIWnd*						m_pWndMain;
	GUIWnd*						m_pWndCaption;
	GUIStatic*					m_pWndName;				//ǿ����Ʒ����
	GUIButton*					m_pBtnExit;				//�˳�

	GUIStatic*					m_pStcClidateBack;
	GUIButton*					m_pBtnOk;				//ȷ��
	GUIButton*					m_pBtnCancel;			//ȡ��
	GUIStatic*					m_pStcNotice;			//����

	GUIPatch*					m_pStcTreeBack;
	AppliedTree*				m_pTreeList;			//�䷽�б�
	GUIStatic*					m_pStcListTitle;		//�䷽����

	GUIStatic*					m_pStcSuccTitle;		//�ɹ��ʱ���
	ComposeStatic*				m_pStcSuccCtrl;			//������߳ɹ��ʵ���Ʒ��ť
	GUIStatic*					m_pStcSuccOdds;			//��ʾ�ɹ���
	GUIStatic*					m_pStcModi;				//�޸�����ʾ
	GUIStatic*					m_pStcModiOdds;			//������

	GUIStatic*					m_pStcNeedShow;			//��ʾ

	GUIStatic*					m_pStcGolden;			//�����ʾ
	GUIStatic*					m_pStcSiller;			//������ʾ

	GUIEditBox*					m_pEditComNum;			//����ϳɵ���Ŀ
	GUIStatic*					m_pStcConNum;			//ǿ����Ŀ��ʾ
	GUIStatic*					m_pStcMasterHint;		//��������ʾ
	GUIStatic*					m_pStcSuccMaster;		//������
	GUIStatic*					m_pStcPotValHint;		//Ǳ����ʾ
	GUIStatic*					m_pStcPotVal;			//��Ҫ����Ǳ��

	GUIStatic*					m_pStcSuccHint;			//�ɹ���ʾ
	GUIStatic*					m_pStcLoseHint;			//ʧ����ʾ
	GUIStatic*					m_pStcPerfectHint;		//������ʾ

	GUIStatic*					m_pStcShowAddedValue;	// ǿ����ֵԤ��ʾ

	GUIProgress*				m_pProTime;				//������
	GUIPushButton*				m_pPBtnFormular[FORMULA_QUANTITY];			//�䷽pushbutton
	ComposeStatic*				m_pStcCtrl[CTRL_QUANTITY];					//�ϳɿؼ�
	GUIStatic*					m_pStcMainStuffHint;						//������ʾ
	GUIStatic*					m_pStcStuffHint[STUFF_QUANTITY];			//�����ϵ���ʾ
	GUIStatic*					m_pStcStuffQuan[STUFF_QUANTITY];			//����������/����

	DWORD						m_dwRecvMsgTime;					//�ɹ�\ʧ��\����ʱ��ʾ��ʱ��

	typedef std::map<DWORD, DWORD> TreeFormularMap;				
	TreeFormularMap				m_tree2formula;						//��ID��Ӧ���䷽ID
	DWORD						m_dwFormulaID;						//��ǰѡ����䷽ID

	CompConsoContainer			m_Container;						//�ؼ�����������
	EConsolidateType			m_eConType;							//ǿ������

	EEquipConsolidate			m_eCurState;						//��ǰ��Ϣ״̬

	DWORD						m_dwNPCID;							//NPC��ID
	DWORD						m_escapeTime;
	DWORD						m_dwErrorCode;						//ʧ��ʱ������������
	INT							m_nLoseTry;						//ʧ��ʱ�ظ�����
};