#pragma once
#include "..\WorldDefine\compose_define.h"
#include ".\BaseData.h"

class ConsolidateData
	: public BaseData
{
public:
	ConsolidateData(void);
	~ConsolidateData(void);

	static ConsolidateData* Inst();

	/** ����ǿ�������ļ�
	*/
	virtual void LoadFromFile();

	/** ��ȡ�����䷽�����ṹ
	*/
	tagPosyProto* FindPosyProto(DWORD dwID)
	{
		return FindInMap(m_mapPosy, dwID);
	}
	/**��ȡ�Կ��䷽�����ṹ
	*/
	tagEngraveProto* FindEngraveProto(DWORD dwID)
	{
		return FindInMap(m_mapEngrave, dwID);
	}
	/**��ȡǿ�����߻����ṹ
	*/
	tagConsolidateItem* FindConsolidateItemProto(DWORD dwID)
	{
		return FindInMap(m_mapConsolidateItem, dwID);
	}
	/**��ȡ������������ṹ
	*/
	tagQuenchProto* FindQuenchProto(DWORD dwID)
	{
		return FindInMap(m_mapQuench, dwID);
	}

	/**��ȡ����map
	*/
	const map<DWORD, tagPosyProto>& GetPosyMap( )
	{
		return m_mapPosy;
	}

	/**��ȡ�Կ�map
	*/
	const map<DWORD, tagEngraveProto>& GetEngraveMap( )
	{
		return m_mapEngrave;
	}

	/**��ȡǿ������map
	*/
	const map<DWORD, tagConsolidateItem>& GetConsolidateItemMap( )
	{
		return  m_mapConsolidateItem;
	}
	/**��ȡ���map
	*/
	const map<DWORD, tagQuenchProto>& GetQuenchMap()
	{
		return m_mapQuench;
	}

private:
	map<DWORD, tagPosyProto>		m_mapPosy;			//����
	map<DWORD, tagEngraveProto>		m_mapEngrave;		//�Կ�
	map<DWORD, tagQuenchProto>		m_mapQuench;		//���
	map<DWORD, tagConsolidateItem>  m_mapConsolidateItem;	//ǿ������(��Ƕ,��ӡ,����)
};