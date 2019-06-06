#pragma once
#include "stdafx.h"
#include  "..\Worlddefine\ItemDefine.h"

#define CTRL_QUANTITY 7						//���Ͽؼ�����
#define FORMULA_QUANTITY 5					//���䷽�ؼ�����
#define STUFF_QUANTITY 6					//����������ʾ����
#define MAX_ASSIST_STUFF_QUANTITY 6			//���ϵ�����


//�ϳɡ�ǿ������(��Ʒ��װ��)
struct tagMainStuff
{
	INT64			n64ID;					//64ID
	DWORD			dwTypeID;				//typeid
	EEquipPos		euqipPos;				//װ����λ��
	tagMainStuff()
	{
		memset(this, 0x0, sizeof(*this));
	}
};

//�����ϳɡ�ǿ������(�����䷽��Ҫ�Ĳ��Ϻ���ҷ���Ĳ���)
struct tagAssistStuff
{
	INT64			n64ID;					//64ID���ϵ�ȫ��ID
	DWORD			dwTypeID;				//����TypeID(�ӱ�ϳ�ǿ�����ж�ȡ)
	EStuffType		stuffType;				//��������(�ӱ�ϳ�ǿ�����ж�ȡ)
	INT				nQuantity;				//������Ҫ������
	INT				nBagQuan;				//�����иò��ϵ�����
	tagAssistStuff()
	{
		memset(this, 0x0, sizeof(*this));
	}
};

//IM����
struct tagIMStuff
{
	INT64			n64ID;					//IMid
	tagIMStuff()
	{
		n64ID = GT_INVALID;
	}
};

/**class CompConsoContainer
   brief:	�����ϳɡ�ǿ������
   purpose:	�������� �����ϵ����ݡ�
*/
class CompConsoContainer					
{
public:
	CompConsoContainer(void){ ClearAllStuff(); }
	~CompConsoContainer(void){ ClearAllStuff(); }

	void Add(const tagMainStuff& value)
	{
		m_mainStuff = value;
	}
	void Add(const tagAssistStuff& value)
	{
		m_assStuff.push_back(value);
	}
	void Add(const tagIMStuff& value)
	{
		m_imStuff = value;
	}

	void ClearAllStuff()
	{
		memset(&m_mainStuff, 0x0, sizeof(m_mainStuff));
		memset(&m_imStuff, 0x0, sizeof(m_imStuff));
		m_assStuff.clear();
	}
	
	void ClearAssistStuff()
	{
		m_assStuff.clear();
		memset(&m_imStuff, 0x0, sizeof(m_imStuff));
	}
	
	EStuffType GetAssistStuffType(DWORD dwPos)
	{
		if(dwPos>=0 && dwPos<(DWORD)m_assStuff.size())
			return EST_Null;
		return m_assStuff[dwPos].stuffType;
	}

	EEquipPos GetMainStuffEquipPos()
	{
		return m_mainStuff.euqipPos;
	}

	void GetAssistStuffMap(vector<tagAssistStuff>& assMap)
	{
		assMap = m_assStuff;
	}

	void GetMainStuff(tagMainStuff& value)
	{
		value = m_mainStuff;
	}

	void GetIMStuff(tagIMStuff& value)
	{
		value = m_imStuff;
	}

	tagAssistStuff* GetAssistStuff(int nPos)
	{
		if(nPos<0 && nPos>=(int)m_assStuff.size())
			return NULL;
		return& m_assStuff[nPos];
	}

	tagAssistStuff* GetAssistStuff(INT64 n64ID)
	{
		for(int i=0; i<(int)m_assStuff.size(); i++)
		{
			if(m_assStuff[i].n64ID==n64ID)
				return &m_assStuff[i];
		}
		return NULL;
	}

	int GetAssStuffSize() { return (int)m_assStuff.size(); }

private:
	tagMainStuff				m_mainStuff;		//����
	vector<tagAssistStuff>		m_assStuff;			//����
	tagIMStuff					m_imStuff;			//IM����s
};
