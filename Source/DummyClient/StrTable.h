#pragma once

/**	\class StrTable
	\brief �ַ�����
	\remarks Ϊ����Ӧ�����԰汾,�����в���ʹ�ú���,���г������ַ�������ͨ�����

*/
class StrTable
{
	typedef std::map<tstring,tstring> str2str;
public:
	StrTable(void);
	~StrTable(void);

	const TCHAR* operator [](const TCHAR *key);
	void Load(const TCHAR *filename);

private:
	str2str	m_data;
};

