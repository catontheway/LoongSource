#pragma once

#include "ActivityProtoData.h"

struct tagNS_RoleInitOk;
// ǰ������
struct tagNS_ActivityTaskState;
class FestivalActivityPromptFrame;

class ActivityMgr
{

public:
	enum EActivityState
	{
		EAS_Start,		// ��ʼ��
		EAS_End, 		// ������
		EAS_StartSoon,	// ������ʼ
		EAS_EndSoon		// ��������
	};
	
	enum ESortType
	{
		EST_None,
		EST_SortByTime,
		EST_SortByName,
		EST_SortByLevel,
		EST_SortByNpc,
		EST_SortByAcceptTime
	};
	
	typedef std::vector<tagActivityProtoData> ActivityVector;

public:
	~ActivityMgr();
	bool Init();
	VOID Update();
	void Destroy();
	
	static ActivityMgr* Inst();
	const ActivityProtoData& GetData() const { return m_data; }
	const ActivityVector& GetActivityVector();
	void SortActivities(ESortType type);
	UINT16 GetActivityID(DWORD index) const;

private:
	ActivityMgr();

	bool LoadActivityProto();
	
	// ��ʼ����� 
	void InitActivityTable();

	// ��Ӧ�������״̬������Ϣ 
	DWORD OnNS_ActivityTaskState(tagNS_ActivityTaskState *pCmd, DWORD);

	DWORD OnNS_RoleInitOk( tagNS_RoleInitOk* pNetCmd, DWORD dwParam );
	
private:
	TSFPTrunk<ActivityMgr> m_trunk;
	TObjRef<NetCmdMgr> m_pNetCmdMgr;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;
	TObjRef<NetSession> m_pSession;
	
	FestivalActivityPromptFrame*	m_pFsectivalActivityFrame;
	ActivityProtoData m_data;								// ��������Ա�
	ActivityVector m_activities;
};
