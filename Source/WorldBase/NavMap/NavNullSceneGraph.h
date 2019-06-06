#pragma once
#include "NavSceneGraph.h"


namespace WorldBase
{
	class NavEntityNode;
	/** \class NavNullSceneGraph
		\brief �򵥵ĳ���������
	*/
	class WORLD_BASE_API NavNullSceneGraph :
		public NavSceneGraph
	{
	public:
		NavNullSceneGraph(void);
		virtual ~NavNullSceneGraph(void);

		//--NavSceneGraph
		virtual void LoadFromFile(IFS *pFS,const TCHAR *szMapName) {}
		virtual void InvalidViewZone(const Vector3& zoneCenter){}
		virtual bool AABBoxCollideBoxTop(const AABBox& box,float curTime,float& intersectY,DWORD dwFlags=0,DWORD* pdwCarrierObjID=NULL);
		virtual bool AABBoxCollideBox(const AABBox& box,float curTime,DWORD dwFlags=0,DWORD* pdwCarrierObjID=NULL);
		virtual bool RayCollideBox(const Ray& ray,Vector3& intersectPos);
		virtual bool RayCollideBox(const Ray& ray,DWORD dwFlags=0){return false;}
		virtual bool QuerySurfaceType(const AABBox& box,float curTime,float& intersectY,ESurfaceType& out){return false;}

		//--
		/** ���һ��SceneNode����������this����
		*/
		void AttachNode(NavEntityNode* pNode);
		/** ��thisɾ��һ��SceneNode����SceneNode���ⲿ�ͷ�
		*/
		void DetachNode(NavEntityNode* pNode);

		bool AABBoxCollideBox(const AABBox& box,const Vector3& maxIgnoreMdlSize);
		
		/**���õ�ǰNavNode��������ײ���ʱ�������Լ�
		*/
		void SetCurNavEntityNode(NavEntityNode* pNode){m_pCurNode = pNode;}
	private:
		list<NavEntityNode*> m_sceneNodes;
		NavEntityNode*		 m_pCurNode;
	};
}//namespace WorldBase