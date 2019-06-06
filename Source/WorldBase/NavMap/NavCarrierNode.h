#pragma once
#include "NavEntityNode.h"

namespace WorldBase
{
	/** \class NavCarrierNode
		\brief �ؾ߽��
	*/
	class WORLD_BASE_API NavCarrierNode : public NavEntityNode
	{
		struct tagWayPoint//������		
		{
			Vector3 pos;
			float stopTime;

			float stopEndTime;
			float moveEndTime;
		};

	public:
		NavCarrierNode(void);
		virtual ~NavCarrierNode(void);

		//--ISerializable
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);

		//--NavSceneNode
		virtual bool AABBoxCollideBox(const AABBox& box,float curTime);
		virtual bool AABBoxCollideBoxTopHighest(const AABBox& box,float curTime,float& intersectY);
		virtual TResult RayCollideBox(const Ray& ray){ return TResult(false,0.0f);}
		virtual bool RayCollideBoxOnce(const Ray& ray){ return false;}
		virtual bool QuerySurfaceType(const AABBox& box,float curTime,float& intersectY,ESurfaceType& out);
		virtual NavSceneNode* Clone();

		//--
		/** ����ƶ�·��
		*/
		void ClearPath();
		/** ���һ��������
		*/
		void AddWayPoint(const Vector3& pos,float stopTime);
		/** �����ƶ��ٶ�
		*/
		void SetSpeed(float speed);
		/** ����ƶ��ٶ�
		*/
		float GetSpeed(){ return m_speed;}

		/** ��ʼ����
		*/
		void Run(float curTime,float timePos);
		/** ��õ�ǰ����ʱ��
		*/
		float GetRunTime(float curTime);
		/** �����Ƿ�������
		*/
		bool IsRunning(){return m_bRunning;}


		/** �������Object�ռ�任��World�ռ�
		*/
		void PosToWorld(const Vector3& localPos,float curTime,Vector3& out);
		/** �������World�ռ�任��Object�ռ�
		*/
		void PosToLocal(const Vector3& worldPos,float curTime,Vector3& out);
		/** �������World�ռ�任��Object�ռ�
		*/
		void DirToLocal(const Vector3& worldDir,float curTime,Vector3& out);

		/** AABB��Object�ռ���this��ײ
		*/
		bool AABBoxCollideBoxInLocal(const AABBox& box);
		/** AABB��Object�ռ���this��ײ���Ӷ���
		*/
		bool AABBoxCollideBoxTopHighestInLocal(const AABBox& box,float& intersectY);

		/** ��������ռ�AABB
		*/
		void SetWorldBox(const AABBox& box);

		/** ����ƶ�������ٶ�
		*/
		void GetMoveDirAndSpeed(float curTime,Vector3& dir);
		/** �޸���Ծ���ٶȺͷ���
		*/
		void AdjustJumpDirAndSpeed(float curTime,Vector3& xzDir,float& xzSpeed,float& ySpeed);
		/** ���㵱ǰλλ��
		*/
		void GetPos(float curTime,Vector3& out);

	private:
		bool m_bRunning;			//�Ƿ�������
		float m_speed;				//�ƶ��ٶ�
		float m_startRunTime;		//����ʱ��
		float m_lapRunTime;			//����һȦ��Ҫ�೤ʱ��

		vector<tagWayPoint> m_path;	//�ƶ�·��
		
		DECL_RTTI_DYNC(NavCarrierNode);
	};
}//namespace WorldBase