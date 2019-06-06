#pragma once;

#include "..\WorldDefine\time.h"
#include "..\WorldDefine\QuestDef.h"

struct tagNS_SynchronismTime;

class ServerTime
{
public:
	static ServerTime *Inst();
	void Init();
	void Destroy();
	
	// ȡ�õ�ǰ�����ڼ�
	EWeek GetCurrentWeekDay() const;

	// ���ָ�����������ڼ�
	EWeek GetWeekDay(tagDWORDTime& time) const;
	
	// ȡ�õ�ǰ����һ��
	WORD GetCurrentYear() const;

	// ȡ�õ�ǰ����һ��
	BYTE GetCurrentMonth() const;

	// ȡ�õ�ǰ����һ��
	BYTE GetCurrentDay() const;

	// ȡ�õ�ǰ����һ��Сʱ
	BYTE GetCurrentHour() const;

	// ȡ�õ�ǰ����һ����
	BYTE GetCurrentMinute() const;

	// ȡ�õ�ǰ����һ��
	BYTE GetCurrentSecond() const;
	
	// ȡ�õ�ǰ������tagDWORDTime��ʽʱ��
	tagDWORDTime CalCurrentServerDwordTime() const;
	
private:
	ServerTime();
	
	DWORD OnNS_SynchronismTime(tagNS_SynchronismTime *pCmd, DWORD);
	

private:
	TSFPTrunk<ServerTime> m_Trunk;
	TObjRef<NetCmdMgr> m_NetCmdMgr;

	tagDWORDTime m_ServerTime;	// ͬ������ʱ��������ʱ��
	tagDWORDTime m_ClientTime;	// ͬ������ʱ�ͻ��˵�ʱ��
	bool m_bSynchronized;		// �Ƿ��Ѿ�ͬ��		
};

