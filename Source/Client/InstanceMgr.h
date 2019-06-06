#pragma once

#include "..\WorldDefine\msg_map.h"
#include "InstanceProtoData.h"


class CountDownFrame;
class QuestTargetWnd;
class InstanceProtoData;

struct tagLeaveTeamEvent;

class InstanceMgr
{
public:
	~InstanceMgr();
	/*!
	\brief ��ȡ��������ָ��
	*/
	static InstanceMgr *Inst();

	/*!
	\brief ��ʼ��
	*/
	BOOL Init();

	/*!
	\brief ����
	*/
	VOID Destroy();
	
	/*!
	\brief ���ظ������Ա�
	*/
	BOOL LoadInstanceProtoTable();

	
	VOID TryGotoInstance(DWORD dwTriggerID, DWORD dwMapID);
	VOID TryOutInstance(DWORD dwTriggerID, DWORD dwMapID);
	/*!
	\brief ���ʹ���������Ϣ
	*/
	VOID Send_TriggerInstance(DWORD dwTriggerID, DWORD dwLevel);

	/*!
	\brief �����Ƿ�֪ͨ����������Ϣ
	*/
	VOID Send_NotifyTeammate(bool bNofify);

	/*!
	\brief �����Ƿ�ͬ����븱��������Ϣ
	*/
	VOID Send_AgreeGotoInstance(bool bAgree);

	/*!
	\brief ���������뿪����������Ϣ
	*/
	VOID Send_RequestLeaveInstance();
	
	/*!
	\brief �ͻ��˼���Ƿ��ܽ��븱��������
	*/
	BOOL IsAbleToGotoInstance();

	/*!
	\brief ����ǰ�Ƿ�ѯ��
	*/
	BOOL IsAskBeforeEnter();
	/*!
	\brief �Ƿ��ܽ���ս��
	*/
	BOOL IsAbleToCombat();
	/*!
	\brief �Ƿ����PK
	*/
	BOOL IsAbleToPK();
	/*!
	\brief �Ƿ��ڱ���ģʽ����Ȼ����PK,������ģʽʧЧ
	*/
	BOOL IsloseSafeGuard();

	/*!
	\brief ��ɫ�Ƿ��ڸ�����
	*/
	BOOL IsInInstance();

	/*!
	\brief ����ѯ���Ƿ����Ի���
	*/		
	VOID CreateAskEnterMsgBox();	
	
	/*!
	\brief ����ѯ���Ƿ��뿪�����Ի���
	*/			
	VOID CreateAskOutMsgBox();
	
	/*!
	\brief �����Ѷ�ѡ��Ի���
	*/
	VOID CreateDifficultyChooseOptionBox();
	
	EInstanceHardMode GetHardMode() const;
	
	
	
private:
	InstanceMgr();
	TSFPTrunk<InstanceMgr>		m_trunk;
	TObjRef<VarContainer>		m_pVarContainer;
	TObjRef<NetCmdMgr>			m_pNetCmdMgr;
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;
	TObjRef<NetSession>			m_pSession;
	CountDownFrame				*m_pCountDownFrame;
	QuestTargetWnd				*m_pQuestTargetWnd;

	InstanceProtoData			m_instanceProtoData;	// �������Ա�����
	DWORD						m_dwTargetMapID;		// ׼������ĸ���ID
	DWORD						m_dwTriggerID;			// ����������ID
	DWORD						m_dwOutTargetMapID;		// ׼����ȥ�ĵ�ͼID
	DWORD						m_dwOutTriggerID;		// ��ȥ�Ĵ�����ID

	EInstanceHardMode			m_eHardMode;			// �����Ѷ�ģʽ
	BOOL						m_bIn;					// ��ǰ�Ƿ��ڸ�����
	
	/*!
	\brief ��Ӧ֪ͨ��������Ƿ��ٻ�����������Ϣ
	*/
	DWORD OnNS_InstanceNofity(tagNS_InstanceNofity *pCmd, DWORD);
	/*!
	\brief ��Ӧ֪ͨ��������Ƿ���븱��������Ϣ
	*/
	DWORD OnNS_InstanceAgree(tagNS_InstanceAgree *pCmd, DWORD);
	/*!
	\brief ��Ӧ�������������Ϣ
	*/
	DWORD OnNS_InstanceComplete(tagNS_InstanceComplete *pCmd, DWORD);
	
	DWORD OnNS_EnterInstance(tagNS_EnterInstance *pCmd, DWORD);
	
	DWORD OnInstanceNotifyBox(tagMsgBoxEvent *pEvent);

	DWORD OnInstanceAgreeBox(tagMsgBoxEvent *pEvent);

	DWORD OnInstanceCompleteBox(tagMsgBoxEvent *pEvent);

	DWORD OnActiveTransmitPoint(tagMsgBoxEvent *pEvent);

	DWORD OnAskEnterMsgBox(tagMsgBoxEvent *pEvent);

	DWORD OnAskOutMsgBox(tagMsgBoxEvent *pEvent);

	DWORD OnInstanceDifficultyBox(tagMsgOptionBoxEvent *pEvent);
	
	DWORD OnEnterInstanceEvent(tagGameEvent*);

	DWORD OnLeaveInstanceEvent(tagGameEvent*);

	DWORD OnLeaveTeamEvent(tagGameEvent*);
	
	VOID CreateQuestTargetWindow();

	VOID DestroyQuestTargetWindow();

	/*!
	\brief ������������
	*/
	VOID Reset();
};