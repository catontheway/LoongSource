/********************************************************************
	created:	2008/08/26
	created:	26:8:2008   17:59
	filename: 	d:\loong_client\Client\QuestScript.h
	file path:	d:\loong_client\Client
	file base:	QuestScript
	file ext:	h
	author:		leili
	
	purpose:	ִ������ϵͳ�Ľű�
*********************************************************************/
#pragma once
class GameScriptMgr;

class QuestScript : public GameScript
{
public:
	QuestScript(void);
	~QuestScript(void);

	/**	��ʼ�Ի�
	*/
	VOID OnTalk(DWORD npcID);

	/**	������Ի�
	*/
	VOID OnAcceptQuestTalk(DWORD questID, DWORD step);

	/**	������Ի�
	*/
	VOID OnCompleteQuestTalk(DWORD questID, DWORD step);

	/**	�龰�Ի�
	*/
	VOID OnScenarioTalk(DWORD id, DWORD step);

	/**	�������ܶԻ�
	*/
	VOID OnProduceSkillTalk(DWORD id, DWORD step);

	/**��������ѡ��
	*/
	VOID OnGuildCommerceTalk(DWORD id, DWORD step);

	/**	�ű���������������
	*/
	VOID OnParse(void);

	VOID OnTrigger(LPCTSTR scriptFile, DWORD questSerial);
	VOID OnUse(LPCTSTR scriptFile, DWORD questID);

private:
	VOID GetTable(LPCTSTR szPath, string& strTable);

private:
	TObjRef<Util>			m_pUtil;
	TObjRef<GameScriptMgr>	m_pGSMgr;
};