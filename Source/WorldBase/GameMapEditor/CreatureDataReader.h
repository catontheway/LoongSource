#pragma once
#include "..\..\WorldDefine\creature_define.h"

/** class	CreatureData
	brief	���NPC���������ԭʼ���Խṹ�洢
*/

namespace WorldBase
{
	class WORLD_BASE_API CreatureDataReader
	{
	public:
		CreatureDataReader(void);
		~CreatureDataReader(void);
			
		/** ���������ļ�
		*/
		void LoadFromFile();
		/** ��ȡNPCģ���ļ�·��
		*/
		static tstring GetCreatureMdlPath(const tagCreatureProto *pNpcAtt);
		/** ��ȡNPC��Ƥ�ļ�·��
		*/
		static tstring GetCreatureSkinPath(const tagCreatureProto *pNpcAtt,int index);
		/** ����NPC��ԭʼ����
		*/
		const tagCreatureProto* FindCreatureAtt(DWORD attID)
		{
			return FindInMap(m_creatureMap,attID);
		}
		/** ��ȡ���е�NPCԭʼ����
		*/
		void GetCreatureMap(map<DWORD,tagCreatureProto>& npcMap)
		{
			npcMap = m_creatureMap;
		}
		const map<DWORD,tagCreatureProto>* GetCreatureMap()
		{
			return &m_creatureMap;
		}

		tagCreatureProto* FindInMap(map<DWORD,tagCreatureProto>& dataMap,DWORD attID)
		{
			map<DWORD, tagCreatureProto> ::iterator pIter = dataMap.find(attID);
			if(pIter != dataMap.end())
				return &pIter->second;
			return NULL;
		}

	private:
		map<DWORD,tagCreatureProto>		m_creatureMap;
	};

} // namespace WorldBase
