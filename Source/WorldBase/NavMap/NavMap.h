#pragma once
#include "NavCollider.h"
#include "NPCNavMap.h"
#include "PlayerNavMap.h"
#include "NPCNavGraph.h"

namespace WorldBase
{
	/** \class NavMap
		\breif ����ͼ
	*/
	class WORLD_BASE_API NavMap
	{
	public:
		enum ELoadOptions			//����ѡ��
		{
			ELO_Collider=1,			//��ײ����
			ELO_NPCNavMap=2,		//NPC���ӵ���ͼ
			ELO_NPCNavGraph=4,		//NPC��������
			ELO_PlayerNavMap=8,		//����Զ�Ѱ·����ͼ
			ELO_GroundType=16,		//�ر�����
			ELO_NPCNavMapDoors=32,	//NPC���ӵ���ͼ������

			ELO_All=0xFFFFFFFF
		};
	public:
		NavMap(void);
		~NavMap(void);

		/** ���ص���ͼ����
		*/
		void LoadFromFile(const TCHAR* szMapName,IFS* pMapFS,IFS* pSysFS,DWORD loadOptions=ELO_Collider);
		/** �ͷ�����
		*/
		void Destroy();
		/** ���ÿɼ������С
		*/
		void SetViewZoneSize(const Vector3& size);
		/** ���¿ɼ�����
		*/
		void InvalidViewZone(const Vector3& zoneCenter);
		/** ��¡����
		*/
		void Clone(NavMap* pTarget);
		/** �����ײ�����
		*/
		NavCollider* GetCollider(){ return &m_collider;}
		/** ���NPC���ӵ���ͼ
		*/
		NPCNavMap*		GetNPCNavMap()	{ return &m_npcNavMap;}
		/** ��û���·���NPC��������(NavMesh)
		*/
		NPCNavGraph*	GetNPCNavGraph(){ return &m_npcNavGraph;}
		/** �������Զ�Ѱ·����ͼ
		*/
		PlayerNavMap* GetPlayerNavMap(){ return &m_playerNavMap;}

	private:
		NavCollider		m_collider;		//��ײ�����
		NPCNavMap		m_npcNavMap;	//NPC���ӵ���ͼ
		PlayerNavMap	m_playerNavMap;	//����Զ�Ѱ·����ͼ
		NPCNavGraph		m_npcNavGraph;	//NPC��������
		DWORD			m_loadOptions; 
	};

}//namespace WorldBase
