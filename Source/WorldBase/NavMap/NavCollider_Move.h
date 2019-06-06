#pragma once
#include "NavColliderPublicDef.h"

namespace WorldBase
{
	class NavCollider;
	/** \class NavCollider_Move
		\brief �ƶ���ײ�����
	*/
	class WORLD_BASE_API NavCollider_Move
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
			Vector3		startPos;		//�ƶ����
			Vector3		endPos;			//�ƶ��յ�
			Vector3		moveDir;		//xzƽ���ƶ�����
			Vector3		roleSize;		//��ɫ�ߴ�
			float		moveSpeed;		//�ƶ��ٶ�
			float		xzDist;			//xzƽ�����
			int			maxCDTimes;		//�����ײ����
			bool		bPassCheck;		//�Ƿ����ͨ���Լ��
			DWORD		dwCDBoxTopFlags;//��ײ���Ӷ�����־λ
			DWORD		dwCDBoxFlags;	//��ײ���ӱ�־λ
		};

		struct tagOutput				//��������ͷ���ֵ
		{
			ENavResult	result;			//����ֵ
			Vector3		outPos;			//��ɫλ��
			float		endTime;		//����ʱ��
			DWORD		dwCarrierObjID;	//�ؾ�ObjID
		};

	public:
		NavCollider_Move(void);
		~NavCollider_Move(void);

		/** ��ʼ��
		*/
		void Init(const Vector3& startPos,const Vector3& endPos,const Vector3& roleSize,float moveSpeed,bool bPassCheck=true,bool bIgnoreCarrier=false,int maxCDTimes=100);
		/** �ƶ�
		*/
		ENavResult Update(NavCollider* pCollider,float startTime,float deltaTime,Vector3& outPos,float& endTime,DWORD& dwCarrierObjID,bool bOnWaterCheck=false);
		/** �ƶ����ԣ������ؾ�
		*/
		ENavResult IfCanGo(NavCollider* pCollider,const Vector3& startPos,const Vector3& endPos,const Vector3& roleSize,Vector3* pOutPos,int maxCDTimes=100,bool bIgnoreCarrier=true,bool bMustSupportPathFind=false);

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