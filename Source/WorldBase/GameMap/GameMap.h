#pragma once
#include "..\WorldBase.h"
#include "..\..\WorldDefine\MapAttDefine.h"

namespace WorldBase
{
	/**	\class GameMap
	\brief ��Ϸ�е�һ����ͼ
	\remarks ���class����Ҫ�Ǳ����ͼ����Ϣ,�����Ǳ�������ʱ�������(�����)��ʵ��
	*/
	class WORLD_BASE_API GameMap
	{
	public:
		enum ELoadOption
		{
			ELO_Npc=1,
			ELO_Static=2,
			ELO_WayPoint=4,
			ELO_Trigger=8,
			ELO_Sound=16,
			ELO_PointLight=32,
			ELO_MapRect=64,
			ELO_PathPoint=128,
			ELO_SpawnPoint=256,
			ELO_DynamicBlock=512,
			ELO_EventTrigger=1024,
			ELO_MapDoor = 2048,
			ELO_MapCarrier = 4096,
			ELO_MapTriggerEffect = 8192,
			ELO_All=0xFFFFFFFF,
		};
	public:
		GameMap(void);
		~GameMap(void);

		void LoadFromFile(IFS* pFS,const TCHAR* szFileName,DWORD options=ELO_All);
		virtual void Destroy();

		//const tagDynamicMapObj* FindDynamicMapObj(DWORD mapID);
		const tagMapNPC* FindMapNpc(DWORD mapID);
		const tagStaticMapObj* FindStaticMapObj(DWORD mapID);
		const tagMapAreaEx* FindMapAreaEx(DWORD areaID);

		void  GetData(tagLight& SunLight, tagDistFog &Fog)
		{
			SunLight = m_SunLight;
			Fog       = m_Fog;
		}
		const vector<tagStaticMapObj>& GetStaticMapObjs() const
		{
			return m_staticMapObjs;
		}
		const map<DWORD,tagMapNPC>& GetNpc() const
		{
			return m_npcs;
		}
		const vector<tagMapWayPoint>& GetWayPoints() const
		{
			return m_waypoints;
		}
		const vector<tagMapTrigger>& GetTriggers() const
		{
			return m_triggers;
		}
		const vector<tagMapSound>& GetSounds() const
		{
			return m_sounds;
		}

		const vector<tagMapPointLight>& GetPointLight() const
		{
			return m_PointLight;
		}

		const vector<tagMapArea>& GetMapRect() const
		{
			return m_MapRect;
		}

		const map<DWORD,tagMapAreaEx>& GetMapAreaEx() const
		{
			return m_MapAreaEx;
		}

		const vector<tagMapPathPoint>& GetPathPoint()const
		{
			return m_PathPoint;
		}

		const vector<tagMapSpawnPoint>& GetSpawnPoint() const
		{
			return m_SpawnPoint;
		}
		const vector<tagMapDynamicBlock>& GetDynamicBlock() const
		{
			return m_DynamicBlock;
		}

		const vector<tagMapEventTrigger>& GetEventTrigger()const
		{
			return m_EventTriggers;
		}
		const vector<tagMapDoor>& GetMapDoor()const
		{
			return m_MapDoor;
		}
		const vector<tagMapTriggerEffect>& GetMapTriggerEffect()const
		{
			return m_MapTriggerEffect;
		}
		const tagMapTriggerEffect* FindTriggerEffect(DWORD dwObjID)const
		{
			for(size_t i=0;i<m_MapTriggerEffect.size();i++)
			{
				if(m_MapTriggerEffect[i].dwObjID==dwObjID)
					return &m_MapTriggerEffect[i];
			}
			return NULL;
		}
		const tagMapDoor* FindMapDoor(DWORD dwObjID) const
		{
			for(size_t i=0;i<m_MapDoor.size();i++)
			{
				if(m_MapDoor[i].dwObjID==dwObjID)
					return &m_MapDoor[i];
			}
			return NULL;
		}
		const vector<tagMapCarrier>& GetMapCarrier()const
		{
			return m_MapCarrier;
		}
		const tagMapCarrier* FinMapCarrier(DWORD dwObjID)const
		{
			for( size_t i=0; i<m_MapCarrier.size(); i++ )
			{
				if( m_MapCarrier[i].dwObjID == dwObjID )
					return &m_MapCarrier[i];
			}
			return NULL;
		}

		void GetSunLight(tagLight& sunLight)const	{	sunLight = m_SunLight; }
		void GetFog(tagDistFog& distFog)const		{	distFog = m_Fog;	   }
		void GetSkyCol(Color4f & skyCol)const		{	skyCol = m_SkyCol;	}
		const tagLight& GetSunLight()const			{   return m_SunLight;	   }
		const tagDistFog& GetFog()const				{   return m_Fog;		   }

		TCHAR* GetSkyBoxTop()		{	return m_szSkyBoxTopTex;}
		TCHAR* GetSkyBoxFrontRight()	{	return m_szSkyBoxFrontRightTex;}
		TCHAR* GetSkyBoxBackLeft()	{	return m_szSkyBoxBackLeftTex;}
		void GetSkyBoxSize(Vector3& drawSize)const
		{
			drawSize = Vector3(m_fSkyBoxXsize, m_fSkyBoxYsize, m_fSkyBoxZsize);
		}
		void GetSkyBoxOff(Vector3& offSize)const
		{
			offSize = Vector3(m_fSkyBoxOffX, m_fSkyBoxOffY, m_fSkyBoxOffZ);
		}
		FLOAT GetSunModulus()const { return m_fSunModulus; }
		FLOAT GetSkyYaw()const	{ return m_fSkyYaw; }
		bool  IsRenderSkyShade()const { return m_bRenderSkyShade; }
		DWORD GetSkyShadeCol()const {	return m_dwSkyShadeCol; }
		DWORD GetDynamicDiffCol()const { return m_dwDynamicDiffCol; }
		DWORD GetDynamicAmbCol()const { return m_dwDynamicAmbCol; }
		DWORD GetDynamicSpecCol()const { return m_dwDynamicSpecCol; }
		bool  GetMapAreaFog( const int nAreaIndex, tagDistFog& fog );
	protected:
		vector<tagStaticMapObj>		m_staticMapObjs;
		//map<DWORD,tagDynamicMapObj>	m_dynamicMapObjs;
		map<DWORD,tagMapNPC>		m_npcs;
		vector<tagMapWayPoint>      m_waypoints;
		vector<tagMapTrigger>		m_triggers;
		vector<tagMapSound>         m_sounds;
		vector<tagMapPointLight>	m_PointLight;	//by add xtian 2008-2-28
		vector<tagMapArea>			m_MapRect;		//by add xtian 2008-5-12
		vector<tagMapPathPoint>		m_PathPoint;	//by add xtian 2008-8-6
		vector<tagMapSpawnPoint>	m_SpawnPoint;	//by add xtian 2008-8-11
		vector<tagMapDynamicBlock>	m_DynamicBlock;	//by add xtian 2008-8-12
		vector<tagMapEventTrigger>  m_EventTriggers;//by add xtian 2008-9-10
		map<DWORD,tagMapAreaEx>		m_MapAreaEx;
		vector<tagMapDoor>			m_MapDoor;
		vector<tagMapCarrier>		m_MapCarrier;
		vector<tagMapTriggerEffect> m_MapTriggerEffect;//by add xtian 2009-7-21

		tagDistFog					m_Fog;
		tagLight					m_SunLight;
		Color4f						m_SkyCol;

		TCHAR						m_szSkyBoxTopTex[X_LONG_NAME];			//��պ���������
		TCHAR						m_szSkyBoxFrontRightTex[X_LONG_NAME];	//��պ�ǰ������
		TCHAR						m_szSkyBoxBackLeftTex[X_LONG_NAME];		//��պк�������
		FLOAT						m_fSkyBoxXsize;		//��պ�X��С
		FLOAT						m_fSkyBoxYsize;		//��պ�Y��С
		FLOAT						m_fSkyBoxZsize;		//��պ�Z��С
		FLOAT						m_fSkyBoxOffX;		//��պ�Xƫ��
		FLOAT						m_fSkyBoxOffY;		//��պ�Yƫ��
		FLOAT						m_fSkyBoxOffZ;		//��պ�Zƫ��
		FLOAT						m_fSunModulus;		//̫������ǿϵ��	by add xtian 2008-6-18
		FLOAT						m_fSkyYaw;			//��պ���ת�Ƕ�	by add xtian 2008-6-26
		bool                        m_bRenderSkyShade;  //�Ƿ���Ⱦ�������
		DWORD						m_dwSkyShadeCol;
		DWORD						m_dwDynamicDiffCol; // ��̬ƽ�й�diffuse
		DWORD						m_dwDynamicAmbCol;	// ��̬ƽ�й�Ambient
		DWORD						m_dwDynamicSpecCol; // ��̬ƽ�й�Specular
	};
}//namespace WorldBase