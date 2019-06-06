#pragma once

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
	template<typename T>
	/** ��ָ��map�в���
	*/
	T* FindInMap(std::map<DWORD,T>& dataMap,DWORD attID)
	{
		std::map<DWORD, T> ::iterator pIter = dataMap.find(attID);
		if(pIter != dataMap.end())
			return &pIter->second;

		return NULL;
	}

	template<typename T>
	T* FindInMap(std::map<tstring,T>& dataMap, const tstring& strName)
	{
		std::map<tstring, T> ::iterator pIter = dataMap.find(strName);
		if(pIter != dataMap.end())
			return &pIter->second;

		return NULL;
	}

};
