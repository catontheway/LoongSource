#pragma once
#include "NavSurface.h"

namespace WorldBase
{
	/**	\class NavSceneNode
		\brief ����ͼ�������	
	*/
	class WORLD_BASE_API NavSceneNode : public RTTIObj,public ISerializable
	{
	public:
		enum EFlags
		{
			EFlag_SupportPlayerPathFind=1,	//����Զ�Ѱ·��ͨ����
			EFlag_SupportNPCNavMap=2,		//���ӵ���NPC��ͨ����
			EFlag_Door=4,					//��
			EFlag_NotDoor=8,				//����
			EFlag_Carrier=16,				//�ؾ�
			EFlag_NotCarrier=32,			//���ؾ�
			EFlag_Terrain=64,				//����
		};
	public:
		NavSceneNode();
		virtual ~NavSceneNode();

		//--ISerializable
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);

		//--
		virtual void LoadRes(const TCHAR *szName) =0;
		virtual void GetBox(AABBox& box) =0;
		virtual void SetWorldMat(const Matrix4& worldMat) =0;

		virtual bool AABBoxCollideBox(const AABBox& box,float curTime) =0;
		virtual bool AABBoxCollideBoxTopHighest(const AABBox& box,float curTime,float& intersectY) =0;
		virtual TResult RayCollideBox(const Ray& ray) =0;
		virtual bool RayCollideBoxOnce(const Ray& ray) =0;
		virtual bool QuerySurfaceType(const AABBox& box,float curTime,float& intersectY,ESurfaceType& out) =0;

		virtual NavSceneNode* Clone() =0;

		DWORD GetFlags(){ return m_dwFlags;}
		void SetFlags(DWORD dwFlags){ m_dwFlags=dwFlags;}

		void EnableCollide(bool bEnable){ m_bEnableCollide=bEnable;}
		bool IsEnableCollide(){ return  m_bEnableCollide;}

		void SetObjID(DWORD dwObjID){ m_dwObjID=dwObjID;}
		DWORD GetObjID(){ return m_dwObjID;}

	protected:
		DWORD m_dwFlags;		//��־λ
		DWORD m_dwObjID;		//�༭����ָ�������ID
		bool m_bEnableCollide;	//��ײ��⿪��

		DECL_RTTI(NavSceneNode);
	};
}//namespace WorldBase