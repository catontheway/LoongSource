#pragma once
#include "NavColliderPublicDef.h"

namespace WorldBase
{
	class NavCollider;
	class NavCarrierNode;
	/** \class NavCollider_Jump
		\brief ��Ծ��ײ�����
	*/
	class WORLD_BASE_API NavCollider_Jump
	{
		enum EState						//״̬
		{
			EState_None,				//δ�����κ��ƶ�����
			EState_Initialized,			//�ѳ�ʼ��
			EState_Jumpping,			//��Ծ��
			EState_WaitForFinish,		//�ȴ���Ծ������һ��
			EState_Finished,			//��Ծ���
		};

		struct tagInput					//�������
		{
			Vector3		startPos;		//�ƶ����
			Vector3		xzDir;			//xzƽ����Ծ����
			Vector3		roleSize;		//��ɫ�ߴ�
			float		xzSpeed;		//xzƽ���ٶ�
			float		ySpeed;			//y���ٶ�
			float		g;				//�������ٶ�
			float		cdTimeOut;		//��ײ��ʱ
			DWORD		cdBoxTopFlags;	//��ײ��־λ
			DWORD		cdBoxFlag;		//��ײ��־λ
		};

		struct tagOutput				//��������ͷ���ֵ
		{
			ENavResult	result;			//����ֵ
			Vector3		outPos;			//��ɫλ��
			float		endTime;		//����ʱ��
			float		cdCarrierTime;	//��ײ���ؾߵ�ʱ��
			DWORD		dwCarrierObjID;	//�ؾ�ObjID
		};

	public:
		NavCollider_Jump(void);
		~NavCollider_Jump(void);

		/** ��ʼ��
		*/
		void Init(const Vector3& startPos,const Vector3& xzDir,const Vector3& roleSize,float xzSpeed,float ySpeed,float g=1200.0f,float cdTimeOut=60.0f,bool bTrnOnly=false,bool bIgnoreCarrier=false);
		/** ��Ծ
		*/
		ENavResult Update(NavCollider* pCollider,float startTime,float deltaTime,BitMap* pSpecCangoMap,Vector3& outPos,float& endTime,DWORD& dwCarrierObjID,float& cdCarrierTime,bool bOnWaterCheck=false);

	private:
		tagInput	m_input;			//�������
		tagOutput	m_output;			//��������ͷ���ֵ

		EState		m_state;			//״̬

		class Collider;
		class JumpCtrl;

		friend class Collider;
		friend class JumpCtrl;

		Collider* m_pCollider;
		JumpCtrl* m_pJumpCtrl;
	};
}//namespace WorldBase