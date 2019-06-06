#pragma once
#include "..\WorldDefine\func_npc_define.h"
#include ".\BaseData.h"

struct tagDakSiteEx :
	public tagDakSite
{
	tstring strName;							//��ͼ��
	tstring strDesc;							//����
};

struct tagDakProtoEx
{
	DWORD			dwID;						// ��վ���
	tagDakSiteEx	dakSite[MAX_DAK_SITE_NUM];
};

class DakData :
	public BaseData
{
public:
	DakData(void);
	~DakData(void);

	static DakData* Inst();
	/** ������վ�����ļ�
	*/
	virtual void LoadFromFile();
	/**��ȡ��վ�����ṹ
	*/
	tagDakProtoEx* FindDakData(DWORD dwID)
	{
		return FindInMap(m_pDakProto, dwID);
	}

private:
	map<DWORD, tagDakProtoEx>			m_pDakProto;
};