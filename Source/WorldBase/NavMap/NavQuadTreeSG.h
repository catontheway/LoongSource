#pragma once
#include "navscenegraph.h"
#include "NavQuadTreeData.h"

class IFS;
namespace WorldBase
{
	class NavSGQuadTreeNode;
	class NavColliderCache;
	class NavEntityNode;
	class NavSceneNode;
	class NavCarrierNode;
	/**	\class NavQuadTreeSG 
		\brief ���͵��Ĳ�����������,�ṩ�����������֧��
	*/
	class NavQuadTreeSG :	public NavSceneGraph
	{
	public:
		NavQuadTreeSG(void);
		virtual ~NavQuadTreeSG(void);

		//--NavSceneGraph
		virtual void LoadFromFile(IFS *pFS,const TCHAR* szMapName);
		virtual void InvalidViewZone(const Vector3& zoneCenter);
		virtual bool AABBoxCollideBox(const AABBox& box,float curTime,DWORD dwFlags=0,DWORD* pdwCarrierObjID=NULL);
		virtual bool AABBoxCollideBoxTop(const AABBox& box,float curTime,float& intersectY,DWORD dwFlags=0,DWORD* pdwCarrierObjID=NULL);
		virtual bool RayCollideBox(const Ray& ray,Vector3& intersectPos);
		virtual bool RayCollideBox(const Ray& ray,DWORD dwFlags=0);
		virtual bool QuerySurfaceType(const AABBox& box,float curTime,float& intersectY,ESurfaceType& out);

		//--
		void Clone(NavQuadTreeSG* pTarget);
		void AddSceneNodeToCache(NavColliderCache* pCache);
		/** ע����
		*/
		void RegisterDoor(DWORD dwObjID,NavSceneNode* pDoor);
		/** ע����
		*/
		void UnregisterDoor(DWORD dwObjID);
		/** ��\�ر����е��ŵ���ײ
		*/
		void EnableAllDoorsCollide(bool bEnable);
		/** ��\�ر��ŵ���ײ
		*/
		void EnableDoorCollide(DWORD dwObjID,bool bEnable);
		/** ����ŵ�SceneNode,δ�ҵ�����NULL
		*/
		NavSceneNode* FindDoor(DWORD dwObjID);
		/** ע���ؾ�
		*/
		void RegisterCarrier(DWORD dwObjID,NavCarrierNode* pCarrier);
		/** ע���ؾ�
		*/
		void UnregisterCarrier(DWORD dwObjID);
		/** �����ؾ�
		*/
		NavCarrierNode* FindCarrier(DWORD dwObjID);


	private:
		NavQuadTreeData				m_data;
		NavSGQuadTreeNode			*m_pRoot;
		map<DWORD,NavSceneNode*>	m_doorMap;//��
		map<DWORD,NavCarrierNode*>  m_carrierMap;//�ؾ�

		DECL_RTTI_DYNC(NavQuadTreeSG);
	};
}//namespace WorldBase