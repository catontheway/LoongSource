#pragma once
#include "..\WorldBase.h"

namespace WorldBase
{
	class NavResHeightmap;
	class OutlinePathFinder;
	class NPCNavMapDoors;
	/** \class NPCNavMap
		\brief NPC���ӵ���ͼ
	*/
	class WORLD_BASE_API NPCNavMap
	{
	public:
		NPCNavMap(void);
		~NPCNavMap(void);

		/** ��������
		*/
		void LoadFromFile(const TCHAR* szMapName,IFS* pFS,bool bLoadDoors=false);
		/** �ͷ�����
		*/
		void Destroy();
		/** ��¡����
		*/
		void Clone(NPCNavMap* pTarget);

		/** ��ȡָ�����ӵĸ߶ȣ�xz������Զ��ض�
		*/
		float SafeGetHeight(float x,float z);
		/** ��ȡָ�����ӵĸ߶ȣ��������ڸ��Ӹ߶Ȳ�ֵ��xz������Զ��ض�
		*/
		float SafeGetHeightSlerp(float x,float z);

		/** �ж�ָ�������Ƿ��ͨ��
		*/
		bool IfCanGo(float x,float z);
		bool IfCanGo(int x,int z);

		/** ʹ�û����㷨�ж��Ƿ��ֱ�ߵ���
			\return ����һ���������յ�᷵��true
		*/
		bool IfCanDirectGo(float x1,float z1,float x2,float z2,POINT* pNearPos=NULL);
		bool IfCanDirectGo(int x1,int z1,int x2,int z2,POINT* pNearPos=NULL);

		/** ʹ�á���Ե�����㷨�����ڸ���Ѱ·
		*/
		bool FindPath(float x1,float z1,float x2,float z2,int maxRoundTiles,list<POINT>& path);
		bool FindPath(int x1,int z1,int x2,int z2,int maxRoundTiles,list<POINT>& path);

		/** ƽ��·��
		*/
		void SmoothPath(float curx,float curz,list<POINT>& path);
		
		inline void Tile2World(const POINT& tile,float& x,float& z);
		inline POINT World2Tile(float x,float z);
		
		BitMap* GetCanGoMap()	{ return m_pCangoMap;}

		/** ��/�ر�ָ������
		*/
		void OpenCloseDoor(DWORD dwObjID,bool bOpen);
		/** ��/�ر�������
		*/
		void OpenCloseAllDoors(bool bOpen);

	private:
		BitMap*				m_pCangoMap;
		NavResHeightmap*	m_pHMap;	
		OutlinePathFinder*	m_pPathFinder;
		NPCNavMapDoors*		m_pDoors;
	};

}//namespace WorldBase
