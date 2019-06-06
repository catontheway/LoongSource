#pragma once
#include ".\BaseData.h"

struct tagItem2GM
{
	DWORD				dwTypeID;					//��ƷTypeID
	TCHAR				szName[X_SHORT_STRING];		// ��Ʒ����
};

/** \class	ItemProtoData
	\brief	��Ʒ��װ��ԭʼ���Խṹ�洢
*/
class ItemProtoData
	: public BaseData
{
public:
	ItemProtoData(void);
	~ItemProtoData(void);

	static ItemProtoData* Inst();

	/** ���������ļ�
	*/
	virtual void LoadFromFile();

	int GetItemNum() { return (int)m_mapItemProto.size(); }
	std::map<DWORD, tagItem2GM>& GetItem() { return m_mapItemProto; }

	const tagItem2GM* FindItemProto(const tstring& strName) 
	{
		std::map<DWORD, tagItem2GM>::iterator it=m_mapItemProto.begin();
		for( ; it!=m_mapItemProto.end(); ++it )
		{
			if( it->second.szName == strName )
			{
				return &(it->second);
			}
		}
		return NULL;
	}

private:
	std::map<DWORD, tagItem2GM>				m_mapItemProto;		// ��Ʒԭʼ�ṹ
};