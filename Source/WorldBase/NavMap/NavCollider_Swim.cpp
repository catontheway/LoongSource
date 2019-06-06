#include "StdAfx.h"
#include "NavCollider_Swim.h"
#include "NavColliderPrivateDef.h"
#include "NavCollider.h"
#include "NavMovePath.h"
#include "NavSceneNode.h"


namespace WorldBase
{
	//--class NavCollider_Swim::Collider------------------------------------------------------------
	class NavCollider_Swim::Collider
	{
	public:
		struct tagCDResult						//��ײ���
		{
			ENavResult		result;				//��ײ����ֵ
			NavMovePath		path;				//��Ӿ·��
		};
		
	public:
		NavCollider_Swim*	m_p;
		tagCDResult			m_cdResult;
		float				m_xzSwimdDist;
		Vector3				m_stepStart;

	public:
		Collider(NavCollider_Swim* pParent)
			:m_p(pParent)
		{}
		~Collider()
		{}

		const tagCDResult& GetResult()
		{
			return m_cdResult;
		}

		void Init()
		{
			m_xzSwimdDist=0.0f;
			m_stepStart=m_p->m_input.startPos;
		}

		bool Collide( NavCollider* pCD,float startTime,float deltaTime,bool bWaterWalk )
		{
			const Vector3& roleSize		= m_p->m_input.roleSize;
			const Vector3 halfSize		= m_p->m_input.roleSize*0.5f;
			const Vector3& dir			= m_p->m_input.swimDir;
			const Vector3& end			= m_p->m_input.endPos;
			const float speed			= m_p->m_input.swimSpeed;
			const float xzDist			= m_p->m_input.xzDist;
			const int maxCDTimes		= m_p->m_input.maxCDTimes;
			const bool bPassCheck		= m_p->m_input.bPassCheck;
			const DWORD dwCDBoxTopFlags	= m_p->m_input.dwCDBoxTopFlags;
			const DWORD dwCDBoxFlags	= m_p->m_input.dwCDBoxFlags;
	
			m_cdResult.path.Clear();
			m_cdResult.path.SetStartPos(m_stepStart);

			float swimdTime=0.0f;
			bool bArrive=false;
			for(int i=0;;++i)
			{
				/////////////////////////////////////////////////////////////////////////////////////
				// ��xzƽ����Ӿһ������
				Vector3 stepEnd=m_stepStart+dir*g_collideStep;

				/////////////////////////////////////////////////////////////////////////////////////
				// ������Ӿ����ˮ�����ߣ�ʱ�ŵ׵ĸ߶�
				if( !bWaterWalk )
					stepEnd.y=pCD->CalcSwimFootHeight(stepEnd,roleSize.y,stepEnd.y);
				else
					stepEnd.y=pCD->GetWaterHeightEx(stepEnd,stepEnd.y);

				/////////////////////////////////////////////////////////////////////////////////////
				// �ж��Ƿ񵽴��յ�
				m_xzSwimdDist+=g_collideStep;
				if(m_xzSwimdDist>=xzDist)
				{
					stepEnd.x=end.x;
					stepEnd.z=end.z;
					bArrive=true;
				}

				/////////////////////////////////////////////////////////////////////////////////////
				// ���������굱ǰ����Ҫ��ʱ��
				float stepTime=Vec3Dist(m_stepStart,stepEnd)/speed;

				/////////////////////////////////////////////////////////////////////////////////////
				// ������������
				if(!pCD->IfCanGo(stepEnd.x,stepEnd.z))
				{
					m_cdResult.result=ENR_Blocking;
					return true;
				}

				/////////////////////////////////////////////////////////////////////////////////////
				// ���ɽ�ɫAABB
				AABBox box;
				box.min.x=stepEnd.x-halfSize.x;
				box.max.x=stepEnd.x+halfSize.x;
				box.min.z=stepEnd.z-halfSize.z;
				box.max.z=stepEnd.z+halfSize.z;

				/////////////////////////////////////////////////////////////////////////////////////
				// �����жϽ�ɫ���������Ƿ���ģ��
				bool bHaveModel=pCD->HaveModel(box.min.x,box.min.z,box.max.x,box.max.z);

				/////////////////////////////////////////////////////////////////////////////////////
				// ȡ�õ���߶�
				float groundH=pCD->SafeGetGroundHeightSlerp(stepEnd.x,stepEnd.z);

				if(bPassCheck)
				{
					/////////////////////////////////////////////////////////////////////////////////
					// ����Ƿ������ϰ���
					box.min.y=stepEnd.y;
					box.max.y=box.min.y+roleSize.y;
					if(pCD->AABBoxCollideBoxAndTrn(box,0.0f,groundH,dwCDBoxFlags))
					{
						/////////////////////////////////////////////////////////////////////////////
						// ����Ƿ������������Ӷ���
						box.max.y=stepEnd.y+g_moveFall;
						box.min.y=stepEnd.y;
						bool bOnGround=false;
						if(pCD->AABBoxCollideBoxTopAndTrn(box,0.0f,groundH,stepEnd.y,&bOnGround,dwCDBoxTopFlags))
						{
							/////////////////////////////////////////////////////////////////////////
							// �ж����
							if(stepEnd.y-m_stepStart.y>g_moveFall)
							{
								m_cdResult.result=ENR_Blocking;
								return true;
							}

							/////////////////////////////////////////////////////////////////////////
							// �����ŵ��Ƿ��ͨ��
							box.min.y=stepEnd.y+10.0f;
							box.max.y=box.min.y+roleSize.y-10.0f;
							if(!pCD->AABBoxCollideBoxAndTrn(box,0.0f,groundH,dwCDBoxFlags))
							{
								m_cdResult.path.AddPoint(stepEnd);
								m_cdResult.result=ENR_SwimLanded;
								return true;
							}
						}

						m_cdResult.result=ENR_Blocking;
						return true;
					}
				}

				if(bArrive)
				{
					m_cdResult.path.AddPoint(stepEnd);
					m_cdResult.result=ENR_Arrive;
					return true;
				}

				/////////////////////////////////////////////////////////////////////////////////////
				// �ж�ʱ���Ƿ��ˣ�
				if(swimdTime+stepTime>=deltaTime
					||i>=maxCDTimes)
				{
					m_cdResult.path.AddPoint(stepEnd);
					m_cdResult.result=ENR_ToBeContinued;
					
					m_stepStart=stepEnd;
					return false;
				}

				////////////////////////////////////////////////////////////////////////////////////
				// ����һ��·����
				m_cdResult.path.AddPoint(stepEnd);

				swimdTime+=stepTime;
				m_stepStart=stepEnd;
			}

			//������ִ�е����
			ASSERT(0);
			return false;
		}	
	};


	//--class NavCollider_Swim::SwimCtrl------------------------------------------------------------
	class NavCollider_Swim::SwimCtrl
	{
	public:
		NavCollider_Swim*		m_p;

		float					m_totalTime;		//����Ӿʱ��
		float					m_swimdTime;		//����Ӿʱ��
		float					m_endTime;			//��Ӿ����ʱ��
		Vector3					m_endPos;			//��Ӿ����λ��
		bool					m_bFinished;		//��Ӿ��ɱ�־

	public:
		SwimCtrl(NavCollider_Swim* pParent)
			:m_p(pParent)
		{}
		~SwimCtrl()
		{}

		bool SwimStart(const Collider::tagCDResult& cdResult,float startTime,float deltaTime,float& swimTime,float& endTime,Vector3& out)
		{
			//--��������Ӿʱ��
			m_totalTime=cdResult.path.GetDist()/m_p->m_input.swimSpeed;
	
			//--��ʼ������Ӿʱ��
			m_swimdTime=0.0f;

			//--�����Ӿ��ɱ�־
			m_bFinished=false;

			//--������ӾUpdate
			return SwimUpdate(cdResult,startTime,deltaTime,swimTime,endTime,out);
		}

		bool SwimUpdate(const Collider::tagCDResult& cdResult,float startTime,float deltaTime,float& swimTime,float& endTime,Vector3& out)
		{
			if(m_bFinished)
			{
				swimTime=0.0f;
				endTime=m_endTime;
				out=m_endPos;
				return true;
			}

			float leaveTime=m_totalTime-m_swimdTime;
			if(deltaTime>=leaveTime)
			{
				swimTime=leaveTime;

				endTime=startTime+swimTime;

				out=cdResult.path.Back();

				m_bFinished=true;
				m_endTime=endTime;
				m_endPos=out;
				return true;
			}
			else
			{
				m_swimdTime+=deltaTime;

				swimTime=deltaTime;

				float factor;
				if(m_totalTime>0)
					factor=m_swimdTime/m_totalTime;
				else
					factor=0.0f;

				cdResult.path.SlerpPos(factor,out);
				return false;
			}
		}
	};


	//--class NavCollider_Swim----------------------------------------------------------------------
	NavCollider_Swim::NavCollider_Swim(void):m_state(EState_None)
	{
		m_pCollider=new Collider(this);
		m_pSwimCtrl=new SwimCtrl(this);
	}

	NavCollider_Swim::~NavCollider_Swim(void)
	{
		SAFE_DELETE(m_pCollider);
		SAFE_DELETE(m_pSwimCtrl);
	}

	void NavCollider_Swim::Init( const Vector3& startPos,const Vector3& endPos,const Vector3& roleSize,float swimSpeed,bool bPassCheck,int maxCDTimes )
	{
		//--����������
		ASSERT(swimSpeed>0.0f);
		ASSERT(maxCDTimes>0);

		//--�����������
		m_input.startPos		= startPos;
		m_input.endPos			= endPos;
		m_input.roleSize		= roleSize;
		m_input.swimSpeed		= swimSpeed;
		m_input.bPassCheck		= bPassCheck;
		m_input.maxCDTimes		= maxCDTimes;

		m_input.dwCDBoxTopFlags=0;
		m_input.dwCDBoxTopFlags|=NavSceneNode::EFlag_NotCarrier;

		m_input.dwCDBoxFlags=0;
		m_input.dwCDBoxFlags|=NavSceneNode::EFlag_NotCarrier;

		//--����xzƽ�淽��
		m_input.swimDir			= endPos-startPos;
		m_input.swimDir.y		= 0.0f;

		//--����xzƽ�����
		m_input.xzDist			= Vec3Len(&m_input.swimDir);

		//--��һ��xzƽ�淽��
		if(m_input.xzDist>0.0f)
			m_input.swimDir/=m_input.xzDist;
		else
			m_input.swimDir=MathConst::Zero3;

		//--��ʼ��m_output
		ZeroMemory(&m_output,sizeof(m_output));

		//--
		m_pCollider->Init();

		//--����״̬
		m_state					= EState_Initialized;
	}

	ENavResult NavCollider_Swim::Update( NavCollider* pCollider,float startTime,float deltaTime,Vector3& outPos,float& endTime,bool bWaterWalk/*=false*/ )
	{
		const Collider::tagCDResult& cdResult=m_pCollider->GetResult();

		switch(m_state)
		{
		case EState_Initialized:
			{
				//--���xz����
				if(m_input.xzDist==0.0f)
				{
					m_state=EState_Finished;
					m_output.result=ENR_Arrive;
					m_output.endTime=startTime;
					m_output.outPos=m_input.startPos;

					//--��Ҫ������ֵ��ֵ
					outPos=m_output.outPos;
					endTime=m_output.endTime;

					return ENR_Arrive;
				}

				//--��ײ
				bool bCDOk=m_pCollider->Collide(pCollider,startTime,deltaTime,bWaterWalk);
				if(bCDOk)
				{
					m_output.result=cdResult.result;
				}

				//--��ʼ��Ӿ
				float swimTime=0.0f;
				bool bSwimOk=m_pSwimCtrl->SwimStart(cdResult,startTime,deltaTime,swimTime,m_output.endTime,m_output.outPos);
			
				//--����״̬
				if(bCDOk&&bSwimOk)
				{
					m_state=EState_Finished;
					outPos=m_output.outPos;
					endTime=m_output.endTime;
					return m_output.result;
				}
				else if(bCDOk&&!bSwimOk)
				{
					m_state=EState_WaitForFinish;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
				else
				{
					m_state=EState_Swimming;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
			}
			break;
		case EState_Swimming:
			{
				//--��Ӿ����
				float swimTime=0.0f;
				bool bSwimOk=m_pSwimCtrl->SwimUpdate(cdResult,startTime,deltaTime,swimTime,m_output.endTime,m_output.outPos);
				if(!bSwimOk)
				{
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}

				startTime+=swimTime;
				deltaTime-=swimTime;

				//--��ײ
				bool bCDOk=m_pCollider->Collide(pCollider,startTime,deltaTime,bWaterWalk);
				if(bCDOk)
				{
					m_output.result=cdResult.result;
				}

				//--��ʼ��Ӿ
				bSwimOk=m_pSwimCtrl->SwimStart(cdResult,startTime,deltaTime,swimTime,m_output.endTime,m_output.outPos);

				//--����״̬
				if(bCDOk&&bSwimOk)
				{
					m_state=EState_Finished;
					outPos=m_output.outPos;
					endTime=m_output.endTime;
					return m_output.result;
				}
				else if(bCDOk&&!bSwimOk)
				{
					m_state=EState_WaitForFinish;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
				else
				{
					m_state=EState_Swimming;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
			}
			break;
		case EState_WaitForFinish:
			{
				//--��Ӿ����
				float swimTime=0.0f;
				bool bSwimOk=m_pSwimCtrl->SwimUpdate(cdResult,startTime,deltaTime,swimTime,m_output.endTime,m_output.outPos);
				if(!bSwimOk)
				{
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
				
				//--����״̬
				m_state=EState_Finished;
				outPos=m_output.outPos;
				endTime=m_output.endTime;
				return m_output.result;
			}
			break;
		case EState_Finished:
			{
				outPos=m_output.outPos;
				endTime=m_output.endTime;
				return m_output.result;
			}
			break;
		}

		ASSERT(0);
		return ENR_Failed;
	}

	ENavResult NavCollider_Swim::IfCanGo( NavCollider* pCollider,const Vector3& startPos,const Vector3& endPos,const Vector3& roleSize,Vector3* pOutPos,int maxCDTimes/*=100*/ )
	{
		const float speed=500.0f;
		const float timeOut=1000000.0f;

		m_input.startPos		= startPos;
		m_input.endPos			= endPos;
		m_input.roleSize		= roleSize;
		m_input.swimSpeed		= speed;
		m_input.bPassCheck		= true;
		m_input.maxCDTimes		= maxCDTimes;

		m_input.dwCDBoxTopFlags=0;
		m_input.dwCDBoxTopFlags|=NavSceneNode::EFlag_NotCarrier;

		m_input.dwCDBoxFlags=0;
		m_input.dwCDBoxFlags|=NavSceneNode::EFlag_NotCarrier;

		//--����xzƽ�淽��
		m_input.swimDir			= endPos-startPos;
		m_input.swimDir.y		= 0.0f;

		//--����xzƽ�����
		m_input.xzDist			= Vec3Len(&m_input.swimDir);

		//--��һ��xzƽ�淽��
		if(m_input.xzDist>0.0f)
			m_input.swimDir/=m_input.xzDist;
		else
			m_input.swimDir=MathConst::Zero3;

		//--��ʼ��m_output
		ZeroMemory(&m_output,sizeof(m_output));

		//--��ײ���s
		const Collider::tagCDResult& cdResult=m_pCollider->GetResult();

		m_pCollider->Init();
		if(!m_pCollider->Collide(pCollider,0.0f,timeOut,false))
		{
			if(pOutPos)*pOutPos=cdResult.path.Back();
			return ENR_ToBeContinued;
		}

		if(pOutPos)*pOutPos=cdResult.path.Back();
		return cdResult.result;
	}
}//namespace WorldBase