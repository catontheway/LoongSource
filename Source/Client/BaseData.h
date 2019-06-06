#pragma once

//----------------------------------------------------------------
// �ж��Ƿ�Ϊ�ٷ���
//----------------------------------------------------------------
#define MIsValuePct(nNum)			((nNum) > 100000 || (nNum) < -100000)

//----------------------------------------------------------------
// ���ٷ���ת��Ϊ��ֵ
//----------------------------------------------------------------
#define MValuePctTrans(nNum)		((nNum) > 100000 ? (nNum) - 100000 : (nNum) + 100000)

/** class	BaseData;
	brief	��Ϸ�������ݻ���
*/
class BaseData
{
public:
	BaseData(){}
	~BaseData(){}

	virtual void LoadFromFile() = 0;

protected:
	template<typename K,typename T>
	/** ��ָ��map�в���
	*/
	T* FindInMap(map<K,T>& dataMap,K key)
	{
		map<K,T> ::iterator pIter = dataMap.find(key);
		if(pIter != dataMap.end())
			return &pIter->second;

		// ������ʾ���Է�Release�������ļ����������޷��鵽
		//ERR(_T("could not find attID:%d"), attID);
		return NULL;
	}

};
