#pragma once
#include ".\BaseData.h"
#include "..\WorldDefine\filter.h"
class FilterData
	: public BaseData
{
public:
	FilterData(void);
	~FilterData(void);

	static FilterData* Inst();

	/** ������˴��ļ�
	*/
	virtual void LoadFromFile();

	/** �����Ƿ�ȫ��
	*/
	DWORD IsValidName(LPCTSTR name, INT nMax=7, INT nMin=1);

	/** �������
	*/
	DWORD WordsFilteror(tstring& strText, INT nMax=200);

private:
	vector<tstring>					m_vecNameFilter;
	vector<tstring>					m_vecChatFilter;
};