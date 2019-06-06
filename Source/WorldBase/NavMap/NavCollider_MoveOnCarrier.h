#pragma once
#include "NavColliderPublicDef.h"

namespace WorldBase
{
	class NavCollider;
	class NavCarrierNode;
	/** \class NavCollider_MoveOnCarrier
		\brief �ؾ����ƶ���ײ�����
	*/
	class WORLD_BASE_API NavCollider_MoveOnCarrier
	{
		enum EState						//״̬
		{
			EState_None,				//δ�����κ��ƶ�����
			EState_Initialized,			//�ѳ�ʼ��
			EState_Moving,				//�ƶ���
			EState_WaitForFinish,		//�ȴ��ƶ������һ��
			EState_Finished,			//�ƶ����
		};

		struct tagInput					//�������
		{
			DWORD		dwCarrierObjID;	//�ؾ�ObjID
			Vector3		localStartPos;	//�ƶ����
			Vector3		localMoveDir;	//xzƽ���ƶ�����
			Vector3		roleSize;		//��ɫ�ߴ�
			float		moveSpeed;		//�ƶ��ٶ�
			int			maxCDTimes;		//�����ײ����
		};

		struct tagOutput				//��������ͷ���ֵ
		{
			ENavResult	result;			//����ֵ
			Vector3		outPos;			//��ɫλ��
			float		endTime;		//����ʱ��
			DWORD		dwCarrierObjID;	//�ؾ�ObjID
		};
	
	public:
		NavCollider_MoveOnCarrier(void);
		~NavCollider_MoveOnCarrier(void);

		/** \��ʼ��
		*/
		void Init(DWORD dwCarrierObjID,const Vector3& localStartPos,const Vector3& localMoveDir,const Vector3& roleSize,float moveSpeed,int maxCDTimes=100);
		/** \�ƶ�
		*/
		ENavResult Update(NavCollider* pCollider,float startTime,float deltaTime,Vector3& outPos,float& endTime,DWORD& dwCarrierObjID);

	private:
		tagInput	m_input;			//�������
		tagOutput	m_output;			//��������ͷ���ֵ

		EState		m_state;			//״̬

		class Collider;
		class MoveCtrl;

		friend class Collider;
		friend class MoveCtrl;

		Collider* m_pCollider;
		MoveCtrl* m_pMoveCtrl;
	};
}//namespace WorldBase