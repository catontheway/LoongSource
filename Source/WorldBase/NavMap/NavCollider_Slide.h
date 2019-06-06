#pragma once
#include "NavColliderPublicDef.h"

namespace WorldBase
{
	class NavCollider;
	/** \class NavCollider_Slide
		\brief ������ײ�����
	*/
	class WORLD_BASE_API NavCollider_Slide
	{
		enum EState						//״̬
		{
			EState_None,				//δ�����κλ�������
			EState_Initialized,			//�ѳ�ʼ��
			EState_Sliding,				//������
			EState_WaitForFinish,		//�ȴ����������һ��
			EState_Finished,			//�������
		};

		struct tagInput					//�������
		{
			Vector3		startPos;		//�������
			Vector3		roleSize;		//��ɫ�ߴ�
			int			maxCDTimes;		//�����ײ����
		};

		struct tagOutput				//��������ͷ���ֵ
		{
			ENavResult	result;			//����ֵ
			Vector3		outPos;			//��ɫλ��
			float		endTime;		//����ʱ��
		};

	public:
		NavCollider_Slide(void);
		~NavCollider_Slide(void);

		/** \��ʼ��
		*/
		void Init(const Vector3& startPos,const Vector3& roleSize,int maxCDTimes=100);
		/** \����
		*/
		ENavResult Update(NavCollider* pCollider,float startTime,float deltaTime,Vector3& outPos,float& endTime,bool bOnWaterCheck=false);

	private:
		tagInput	m_input;			//�������
		tagOutput	m_output;			//��������ͷ���ֵ

		EState		m_state;			//��ǰ״̬

		class Collider;
		class SlideCtrl;

		friend class Collider;
		friend class SlideCtrl;

		Collider*	m_pCollider;
		SlideCtrl*	m_pSlideCtrl;
	};
}//namespace WorldBase