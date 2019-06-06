/////////////////////////////////////////////////////////////////////////////
// Program:     loong����༭��
// Name:        QuestMgr.h
// Created:     2008-11-12
/////////////////////////////////////////////////////////////////////////////

#include  <windows.h>

#ifndef QUEST_MGR_H_
#define QUEST_MGR_H_
#include <string>
#include <map>
#include <list>
#include <vector>
using namespace std;

#include "Singleton.h"
#include "tinyxml/tinyxml.h"

struct tagQuestProto;

struct tagQuest 
{
	tagQuestProto *pQuestProto;
	wstring file_path;
	wstring local_path;
	wstring file_name;
	wstring file_dir;
	tagQuest()
	{
		pQuestProto = NULL;
		file_path = _T("");
		local_path = _T("");
		file_name = _T("");
		file_dir = _T("");
	}
};

typedef map<string,string> XmlNodeMap;
typedef vector<pair<string,string>> XmlNodeArray;
typedef map<UINT16, tagQuest*> QuestMap;
typedef std::vector<pair<INT16,wstring>> QuestTypeArray;

class QuestMgr : public Singleton<QuestMgr>
{
public:
	QuestMgr();
	~QuestMgr();

	//��xml�����ļ��������������
	BOOL LoadQuest(LPCTSTR pPath);

	//���������ݱ��浽xml�ļ���
	BOOL SaveQuest(UINT16 id);

	//ɾ�������ļ�
	void DeleteQuest(UINT16 id);

	//
	void SaveAcceptQuestRelation(LPCTSTR pPath);

	void SaveCompleteQuestRelation(LPCTSTR pPath);
	
	void SaveQuestNPCRelation(LPCTSTR pPath);

	QuestMap& GetQuestMap() { return m_mapQuest; }

	QuestTypeArray& GetQuestType() { return m_questType; }
	
	list<wstring>& GetQuestDir() { return m_listQuestDir; }

	//����ID�Ƿ����
	BOOL IsExist(INT16 questID)
	{
		return m_mapQuest.end() != m_mapQuest.find(questID);
	}

	//
	void AddQuestToMap();

	//wstring-->string
	static string ToAnsi(const wstring& widestring);

	//utf8
	static string ToUtf8(const wstring& widestring);

	//���ڱ�����ʱ�½���Quest��
	tagQuest *m_pQuestTmp;

private:
	
	//�����ƶ�Ŀ¼����ָ�������׺�����ļ����뼯��
	void FindFile(LPCTSTR pFolder, list<wstring>& xmlMap, list<wstring>& localMap);
	//���������ļ�
	void LoadQuestFile(LPCTSTR pPath);
	//��xml�����ļ����������Local����
	void LoadQuestLocal(LPCTSTR pPath);
	//��ȡ������������
	BOOL LoadQuestType(LPCTSTR szPath);

	//������������
	void LoadXmlNode(TiXmlElement* pNode, XmlNodeMap& xmlNodeMap);
	void SetQuestProto(tagQuestProto* pQuestProto, XmlNodeMap& xmlNodeMap);

	void LoadXmlNode(TiXmlElement* pNode, XmlNodeArray& xmlNodeArray);
	
	void SetQuestDword(string str, DWORD& dwQuest,XmlNodeMap& xmlNodeMap);
	void SetQuestDwordFlags(string str, DWORD& dwQuest, INT32 nMask, XmlNodeMap& xmlNodeMap);
	void SetQuestUINT16(string str, UINT16& unQuest,XmlNodeMap& xmlNodeMap);
	void SetQuestBYTE(string str, BYTE& byQuest,XmlNodeMap& xmlNodeMap);
	void SetQuestINT16(string str, INT16& n16Quest,XmlNodeMap& xmlNodeMap);
	void SetQuestINT32(string str, INT32& nQuest,XmlNodeMap& xmlNodeMap);
	void SetQuestBool(string str, BOOL& bQuest,XmlNodeMap& xmlNodeMap);
	void SetQuestFloat(string str, FLOAT& fQuest,XmlNodeMap& xmlNodeMap);
	void SetQuestString(string str, TCHAR* szQuest, INT32 nLen, XmlNodeMap& xmlNodeMap);

	//��������
	void SaveEntry( TiXmlElement* pEle, LPCSTR szName, LPCTSTR szValue );
	void SaveEntry( TiXmlElement* pEle, LPCSTR szName, DWORD dwValue );
	void SaveEntry( TiXmlElement* pEle, LPCSTR szName, DWORD dwValue1, DWORD dwValue2 );
	void SaveItem( TiXmlElement* pEle, LPCSTR szName, DWORD dwValue1, DWORD dwValue2, INT nValue3 );
	void SaveMaxEntry( TiXmlElement* pEle, LPCSTR szName, DWORD dwValue, DWORD dwMax, DWORD dwMin );
	void SaveCreatureEntry( TiXmlElement* pEle, LPCSTR szName, DWORD dwValue, DWORD dwID2, DWORD dwID3, DWORD dwNum );

	//string-->wstring
	wstring ToGBKW(const char* utf8);

	//����ļ���չ���Ƿ�ƥ��
	bool IsExtensionOK(LPCTSTR pFileName, LPCTSTR pExt);

private:

	//������������,������IDΪ����
	QuestMap m_mapQuest;

	QuestTypeArray	m_questType;
	
	//���������ļ�����
	list<wstring> m_listQuestFile;
	//��������Local�ļ�����
	list<wstring> m_listQuestLocalFile;
	//����Ŀ¼����
	list<wstring> m_listQuestDir;
};

#define sQuestMgr QuestMgr::getSingleton()

#endif /* QUEST_MGR_H_ */


