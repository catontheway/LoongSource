#pragma once
#include "NavColliderPublicDef.h"

namespace WorldBase
{
	class NavCollider;
	/** \class NavCollider_Swim
		\brief ��Ӿ��ײ�����
	*/
	class WORLD_BASE_API NavCollider_Swim
	{
		enum EState						//״̬
		{
			EState_None,				//δ�����κ���Ӿ����
			EState_Initialized,			//�ѳ�ʼ��
			EState_Swimming,			//��Ӿ��
			EState_WaitForFinish,		//�ȴ���Ӿ�����һ��
			EState_Finished,			//��Ӿ���
		};

		struct tagInput					//�������
		{
			Vector3		startPos;		//��Ӿ���
			Vector3		endPos;			//��Ӿ�յ�
			Vector3		swimDir;		//xzƽ����Ӿ����
			Vector3		roleSize;		//��ɫ�ߴ�
			float		swimSpeed;		//��Ӿ�ٶ�
			float		xzDist;			//xzƽ�����
			int			maxCDTimes;		//�����ײ����
			bool		bPassCheck;		//�Ƿ����ͨ���Լ��
			DWORD		dwCDBoxTopFlags;//��ײ��־λ
			DWORD		dwCDBoxFlags;	//��ײ��־λ
		};

		struct tagOutput				//��������ͷ���ֵ
		{
			ENavResult	result;			//����ֵ
			Vector3		outPos;			//��ɫλ��
			float		endTime;		//����ʱ��
		};

	public:
		NavCollider_Swim(void);
		~NavCollider_Swim(void);

		/** \��ʼ��
		*/
		void Init(const Vector3& startPos,const Vector3& endPos,const Vector3& roleSize,float swimSpeed,bool bPassCheck=true,int maxCDTimes=100);
		/** \��Ӿ
		*/
		ENavResult Update(NavCollider* pCollider,float startTime,float deltaTime,Vector3& outPos,float& endTime,bool bWaterWalk=false);
		/** �ƶ����ԣ������ؾ�
		*/
		ENavResult IfCanGo(NavCollider* pCollider,const Vector3& startPos,const Vector3& endPos,const Vector3& roleSize,Vector3* pOutPos,int maxCDTimes=100);

	private:
		tagInput	m_input;			//�������
		tagOutput	m_output;			//��������ͷ���ֵ

		EState		m_state;			//״̬

		class Collider;
		class SwimCtrl;

		friend class Collider;
		friend class SwimCtrl;

		Collider* m_pCollider;
		SwimCtrl* m_pSwimCtrl;
	};
}//namespace WorldBase