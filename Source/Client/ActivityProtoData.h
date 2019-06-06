#pragma once

enum EActivityMode
{
	EActivityMode_Day,
	EActivityMode_Week,
	EActivityMode_Month,
	EActivityMode_Year
};


struct tagActivityProtoData 
{
	UINT16 id;				// �id
	EActivityMode mode;		// �����
	tstring name;			// �����
	tstring desc;			// �˵��
	//EActivityType type;	// �����
	BOOL acceptTimeLimit;	// ��ȡʱ������
	
	INT  prompt;			// �Ƿ���ʾ
	INT  startmonth;		// ��ʼ�£����ջʹ��
	INT  endmonth;			// ������
	INT  startday;			// ��ʼ�գ����ջʹ��
	INT	 endday;			// ������

	EWeek week;				// ���ڼ�
	int startHour;			// ��ʼСʱ
	int startMinute;		// ��ʼ����
	int endHour;			// ����Сʱ
	int endMinute;			// ��������
	DWORD acceptNpcID;		// ��ȡNPC��ID
	INT32 minLevel;			// �ȼ���С����
	INT32 maxLevel;			// �ȼ��������
	INT acceptTimes;		// ��ȡ��������
};

class ActivityProtoData
{
public:
	typedef std::map<UINT16, tagActivityProtoData> ActivityProtoMap;
public:

	BOOL LoadFromFile();
	const ActivityProtoMap& GetData() const;
	const tagActivityProtoData *GetQuestProto(UINT16 id) const;
private:
	ActivityProtoMap m_mapActivityProtoData;	// �ID����Ӧ�����Ա�
};


