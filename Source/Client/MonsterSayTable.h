#pragma once

/***************************
\!brief ����˵���ı�
***************************/

class MonsterSayTable
{
public:
	static MonsterSayTable *Inst();
	~MonsterSayTable(){}
	BOOL Init();
	tstring GetSay(DWORD dwID);

private:
	std::map<DWORD, tstring> m_mapSays;
	MonsterSayTable(){}
};