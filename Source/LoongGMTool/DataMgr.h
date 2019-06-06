#pragma once
#include "Singleton.h"

#include "..\ServerDefine\login_define.h"

//��Ϸ����
struct tagGameWorldInfo
{
	//��Ϸ��������,world name
	tstring				world_name;
	//world id
	DWORD				id;
	//world ip
	std::string			world_ip;
	//db ip
	std::string			db_ip;
	//world ״̬
	EWorldStatus		world_status;
	//DB ״̬
	EWorldStatus		db_status;
	// ��������
	INT					nPlayerOnline;
	// ����������
	INT					nMaxPlayerNum;	
};

//������Ϣ
struct tagSectionInfo
{
	//��������
	tstring				section_name;
	//����Crc
	DWORD				id;
	//Login ip
	std::string			login_ip;
	//��������Ϸ�������
	INT					num;
	//Login ״̬
	EWorldStatus		login_status;

	std::map<DWORD, tagGameWorldInfo> game_world_infos;
};

struct tagWorld
{
	//��������
	tstring				section_name;
	DWORD				section_id;

	//��Ϸ��������
	tstring				world_name;
	DWORD				world_id;

	//������ip
	std::string			ip;
	//������״̬
	EWorldStatus		status;

	tagWorld()
	{
		section_id = 0;
		world_id = 0;
		section_name = _T("");
		world_name = _T("");
		ip = "";
		world_name = _T("");
		status = EWS_InitNotDone;
	}
};

struct tagSWId
{
	DWORD				section_id;
	DWORD				world_id;
};

struct tagSWIdEx
{
	DWORD				section_id;
	std::list<DWORD>	list_world_id;
};

class DataMgr : public Singleton<DataMgr>
{
public:
	DataMgr(void);
	~DataMgr(void);

	void Destroy();

	//--
	void AddWorldDataList(tagWorld* world);
	std::list<tagWorld*>* GetWorldDataList() { return &m_listWorld; }
	void UpdateWorldDataList(tagSectionInfo* section_info);
	void GetSectionAndWorldID(const tstring& name, tagSWId* id);
	bool IsSectionWorldValid(const tstring& szSectionName, const tstring szWorldName);
	bool IsSectionValid(const tstring& szSectionName);
	//--
	void AddSectionDataList(tagSectionInfo& info);	
	bool JudgeSingleSection(const DWORD& section_id, const std::list<DWORD>& list_world_id);
	EWorldStatus JudgeWorldListState(const DWORD& section_id, const DWORD& world_id);
	tstring GetWorldNameByWorldID(DWORD world_id);

private:
	std::list<tagWorld*>					m_listWorld;				
	std::list<tagSectionInfo>				m_listSection;
};

#define sDataMgrPtr DataMgr::GetSingletonPtr()