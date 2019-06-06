#include "StdAfx.h"
#include "NavCollider_NPCMove.h"
#include "NavColliderPrivateDef.h"
#include "NavCollider.h"
#include "NavMovePath.h"
#include "NavSceneNode.h"


namespace WorldBase
{
	//--class NavCollider_NPCMove::Collider------------------------------------------------------------
	class NavCollider_NPCMove::Collider
	{
	public:
		struct tagCDResult						//��ײ���
		{
			ENavResult		result;				//��ײ����ֵ
			NavMovePath		path;				//�ƶ�·��

			struct tagCarry
			{
				DWORD		dwCarrierObjID;
				D3DVECTOR	carryPos;

				void Fill(DWORD _dwCarrierObjID,const Vector3& _carryPos)
				{
					dwCarrierObjID		= _dwCarrierObjID;
					carryPos			= _carryPos;
				}
			};

			union 
			{
				tagCarry	carry;
			};
		};
		
	public:
		NavCollider_NPCMove*	m_p;
		tagCDResult			m_cdResult;
		float				m_xzMovedDist;
		Vector3				m_stepStart;

	public:
		Collider(NavCollider_NPCMove* pParent)
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
			m_xzMovedDist=0.0f;
			m_stepStart=m_p->m_input.startPos;
		}

		bool Collide( NavCollider* pCD,float startTime,float deltaTime )
		{
			const Vector3& roleSize		= m_p->m_input.roleSize;
			const Vector3 halfSize		= m_p->m_input.roleSize*0.5f;
			const Vector3& dir			= m_p->m_input.moveDir;
			const Vector3& end			= m_p->m_input.endPos;
			const float speed			= m_p->m_input.moveSpeed;
			const float xzDist			= m_p->m_input.xzDist;
			const int maxCDTimes		= m_p->m_input.maxCDTimes;
			const bool bPassCheck		= m_p->m_input.bPassCheck;
			const float moveFall		= m_p->m_input.moveFall;
			const DWORD dwCDBoxTopFlags	= m_p->m_input.dwCDBoxTopFlags;
			const DWORD dwCDBoxFlags	= m_p->m_input.dwCDBoxFlags;
	
			m_cdResult.path.Clear();
			m_cdResult.path.SetStartPos(m_stepStart);

			float movedTime=0.0f;
			bool bArrive=false;
			for(int i=0;;++i)
			{
				/////////////////////////////////////////////////////////////////////////////////////
				// ��xzƽ���ƶ�һ������
				Vector3 xzStepEnd=m_stepStart+dir*g_collideStep;

				/////////////////////////////////////////////////////////////////////////////////////
				// �ж��Ƿ񵽴��յ�
				m_xzMovedDist+=g_collideStep;
				if(m_xzMovedDist>=xzDist)
				{
					xzStepEnd.x=end.x;
					xzStepEnd.z=end.z;
					bArrive=true;
				}

				/////////////////////////////////////////////////////////////////////////////////////
				// ������xzƽ�����굱ǰ����Ҫ��ʱ��
				float xzStepTime;
				if(bArrive)
					xzStepTime=Vec3Dist(m_stepStart,xzStepEnd)/speed;
				else
					xzStepTime=g_collideStep/speed;

				/////////////////////////////////////////////////////////////////////////////////////
				// ������������
				if(!pCD->IfCanGo(xzStepEnd.x,xzStepEnd.z))
				{
					m_cdResult.result=ENR_Blocking;
					return true;
				}

				/////////////////////////////////////////////////////////////////////////////////////
				// ���ɽ�ɫAABB
				AABBox box;
				box.min.x=xzStepEnd.x-halfSize.x;
				box.max.x=xzStepEnd.x+halfSize.x;
				box.min.z=xzStepEnd.z-halfSize.z;
				box.max.z=xzStepEnd.z+halfSize.z;

				/////////////////////////////////////////////////////////////////////////////////////
				// �����жϽ�ɫ���������Ƿ���ģ��
				bool bHaveModel=pCD->HaveModel(box.min.x,box.min.z,box.max.x,box.max.z);

				/////////////////////////////////////////////////////////////////////////////////////
				// ȡ�õ���߶�
				float groundH=pCD->SafeGetGroundHeightSlerp(xzStepEnd.x,xzStepEnd.z);

				/////////////////////////////////////////////////////////////////////////////////////
				// �ҳ���ŵ�
				Vector3 stepEnd=xzStepEnd;
				bool bOnGround=false;
				DWORD dwCarrierObjID=-1;
				if(bHaveModel)
				{
					box.max.y=xzStepEnd.y+moveFall;
					box.min.y=-g_floatMax;
					if(!pCD->AABBoxCollideBoxTopAndTrn(box,startTime+movedTime+xzStepTime,groundH,stepEnd.y,&bOnGround,dwCDBoxTopFlags,&dwCarrierObjID))
					{
						m_cdResult.result=ENR_Blocking;
						return true;
					}
				}
				else
				{
					bOnGround=true;
					stepEnd.y=groundH;
				}

				/////////////////////////////////////////////////////////////////////////////////////
				// �������굱ǰ����Ҫ��ʱ��
				float stepTime=Vec3Dist(m_stepStart,stepEnd)/speed;

				if(bPassCheck)
				{
					////////////////////////////////////////////////////////////////////////////////
					// �ж����
					if(fabs(m_stepStart.y-stepEnd.y)>moveFall)
					{
						m_cdResult.result=ENR_Blocking;
						return true;
					}

					////////////////////////////////////////////////////////////////////////////////
					// �����ŵ��Ƿ���ͨ��
					if(bHaveModel)
					{
						box.min.y=stepEnd.y+10.0f;
						box.max.y=box.min.y+roleSize.y-10.0f;
						if(pCD->AABBoxCollideBoxAndTrn(box,startTime+movedTime+stepTime,groundH,dwCDBoxFlags))
						{
							m_cdResult.result=ENR_Blocking;
							return true;
						}
					}

					////////////////////////////////////////////////////////////////////////////////
					// �ж��Ƿ����Ӿ
					if(pCD->IfWillSwim(stepEnd,roleSize.y))
					{
						m_cdResult.result=ENR_Blocking;
						return true;
					}
				}

				if(bArrive)
				{
					////////////////////////////////////////////////////////////////////////////////
					// �ж����
					if(fabs(stepEnd.y-end.y)>moveFall)
					{
						m_cdResult.result=ENR_Blocking;
						return true;
					}

					m_cdResult.path.AddPoint(stepEnd);
					m_cdResult.result=ENR_Arrive;
					return true;
				}

				/////////////////////////////////////////////////////////////////////////////////////
				// �ж�ʱ���Ƿ��ˣ�
				if(movedTime+stepTime>=deltaTime
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

				movedTime+=stepTime;
				m_stepStart=stepEnd;
			}

			//������ִ�е����
			ASSERT(0);
			return false;
		}	
	};


	//--class NavCollider_NPCMove::MoveCtrl------------------------------------------------------------
	class NavCollider_NPCMove::MoveCtrl
	{
	public:
		NavCollider_NPCMove*		m_p;

		float					m_totalTime;		//���ƶ�ʱ��
		float					m_movedTime;		//���ƶ�ʱ��
		float					m_endTime;			//�ƶ�����ʱ��
		Vector3					m_endPos;			//�ƶ�����λ��
		bool					m_bFinished;		//�ƶ���ɱ�־

	public:
		MoveCtrl(NavCollider_NPCMove* pParent)
			:m_p(pParent)
		{}
		~MoveCtrl()
		{}

		bool MoveStart(const Collider::tagCDResult& cdResult,float startTime,float deltaTime,float& moveTime,float& endTime,Vector3& out)
		{
			//--�������ƶ�ʱ��
			m_totalTime=cdResult.path.GetDist()/m_p->m_input.moveSpeed;
	
			//--��ʼ�����ƶ�ʱ��
			m_movedTime=0.0f;

			//--����ƶ���ɱ�־
			m_bFinished=false;

			//--�����ƶ�Update
			return MoveUpdate(cdResult,startTime,deltaTime,moveTime,endTime,out);
		}

		bool MoveUpdate(const Collider::tagCDResult& cdResult,float startTime,float deltaTime,float& moveTime,float& endTime,Vector3& out)
		{
			if(m_bFinished)
			{
				moveTime=0.0f;
				endTime=m_endTime;
				out=m_endPos;
				return true;
			}

			float leaveTime=m_totalTime-m_movedTime;
			if(deltaTime>=leaveTime)
			{
				moveTime=leaveTime;

				endTime=startTime+moveTime;

				out=cdResult.path.Back();
				if(cdResult.result==ENR_WillCarry)
					out=cdResult.carry.carryPos;

				m_bFinished=true;
				m_endTime=endTime;
				m_endPos=out;
				return true;
			}
			else
			{
				m_movedTime+=deltaTime;

				moveTime=deltaTime;

				float factor;
				if(m_totalTime>0)
					factor=m_movedTime/m_totalTime;
				else
					factor=0.0f;
				cdResult.path.SlerpPos(factor,out);
				return false;
			}
		}
	};


	//--class NavCollider_NPCMove----------------------------------------------------------------------
	NavCollider_NPCMove::NavCollider_NPCMove(void):m_state(EState_None)
	{
		m_pCollider=new Collider(this);
		m_pMoveCtrl=new MoveCtrl(this);
	}

	NavCollider_NPCMove::~NavCollider_NPCMove(void)
	{
		SAFE_DELETE(m_pCollider);
		SAFE_DELETE(m_pMoveCtrl);
	}

	void NavCollider_NPCMove::Init( const Vector3& startPos,const Vector3& endPos,const Vector3& roleSize,float moveSpeed,bool bPassCheck,int maxCDTimes,float moveFall )
	{
		//--����������
		ASSERT(moveSpeed>0.0f);
		ASSERT(maxCDTimes>0);

		//--�����������
		m_input.startPos		= startPos;
		m_input.endPos			= endPos;
		m_input.roleSize		= roleSize;
		m_input.moveSpeed		= moveSpeed;
		m_input.bPassCheck		= bPassCheck;
		m_input.maxCDTimes		= maxCDTimes;
		m_input.moveFall		= moveFall;

		//--������ײ��־λ
		m_input.dwCDBoxTopFlags=0;
		m_input.dwCDBoxTopFlags|=NavSceneNode::EFlag_NotCarrier;

		m_input.dwCDBoxFlags=0;
		m_input.dwCDBoxFlags|=NavSceneNode::EFlag_NotCarrier;

		//--����xzƽ�淽��
		m_input.moveDir			= endPos-startPos;
		m_input.moveDir.y		= 0.0f;

		//--����xzƽ�����
		m_input.xzDist			= Vec3Len(&m_input.moveDir);

		//--��һ��xzƽ�淽��
		if(m_input.xzDist>0.0f)
			m_input.moveDir/=m_input.xzDist;
		else
			m_input.moveDir=MathConst::Zero3;

		//--��ʼ��m_output
		ZeroMemory(&m_output,sizeof(m_output));
		m_output.dwCarrierObjID	= -1;

		//--����״̬
		m_state					= EState_Initialized;
	}

	ENavResult NavCollider_NPCMove::Update( NavCollider* pCollider,float startTime,float deltaTime,Vector3& outPos,float& endTime,DWORD& dwCarrierObjID )
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
					m_output.outPos=m_input.endPos;
					m_output.endTime=startTime;

					outPos=m_output.outPos;
					endTime=m_output.endTime;
					dwCarrierObjID=m_output.dwCarrierObjID;

					return ENR_Arrive;
				}

				//--��ʼ����ײ�����
				m_pCollider->Init();

				//--��ײ
				bool bCDOk=m_pCollider->Collide(pCollider,startTime,deltaTime);
				if(bCDOk)
				{
					m_output.result=cdResult.result;
					if(cdResult.result==ENR_WillCarry)
						m_output.dwCarrierObjID=cdResult.carry.dwCarrierObjID;
				}

				//--��ʼ�ƶ�
				float moveTime=0.0f;
				bool bMoveOk=m_pMoveCtrl->MoveStart(cdResult,startTime,deltaTime,moveTime,m_output.endTime,m_output.outPos);
			
				//--����״̬
				if(bCDOk&&bMoveOk)
				{
					m_state=EState_Finished;
					outPos=m_output.outPos;
					endTime=m_output.endTime;
					dwCarrierObjID=m_output.dwCarrierObjID;
					return m_output.result;
				}
				else if(bCDOk&&!bMoveOk)
				{
					m_state=EState_WaitForFinish;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
				else
				{
					m_state=EState_Moving;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
			}
			break;
		case EState_Moving:
			{
				//--�ƶ�����
				float moveTime=0.0f;
				bool bMoveOk=m_pMoveCtrl->MoveUpdate(cdResult,startTime,deltaTime,moveTime,m_output.endTime,m_output.outPos);
				if(!bMoveOk)
				{
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}

				startTime+=moveTime;
				deltaTime-=moveTime;

				//--��ײ
				bool bCDOk=m_pCollider->Collide(pCollider,startTime,deltaTime);
				if(bCDOk)
				{
					m_output.result=cdResult.result;
					if(cdResult.result==ENR_WillCarry)
						m_output.dwCarrierObjID=cdResult.carry.dwCarrierObjID;
				}

				//--��ʼ�ƶ�
				bMoveOk=m_pMoveCtrl->MoveStart(cdResult,startTime,deltaTime,moveTime,m_output.endTime,m_output.outPos);

				//--����״̬
				if(bCDOk&&bMoveOk)
				{
					m_state=EState_Finished;
					outPos=m_output.outPos;
					endTime=m_output.endTime;
					dwCarrierObjID=m_output.dwCarrierObjID;
					return m_output.result;
				}
				else if(bCDOk&&!bMoveOk)
				{
					m_state=EState_WaitForFinish;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
				else
				{
					m_state=EState_Moving;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
			}
			break;
		case EState_WaitForFinish:
			{
				//--�ƶ�����
				float moveTime=0.0f;
				bool bMoveOk=m_pMoveCtrl->MoveUpdate(cdResult,startTime,deltaTime,moveTime,m_output.endTime,m_output.outPos);
				if(!bMoveOk)
				{
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
				
				//--����״̬
				m_state=EState_Finished;
				outPos=m_output.outPos;
				endTime=m_output.endTime;
				dwCarrierObjID=m_output.dwCarrierObjID;
				return m_output.result;
			}
			break;
		case EState_Finished:
			{
				outPos=m_output.outPos;
				endTime=m_output.endTime;
				dwCarrierObjID=m_output.dwCarrierObjID;
				return m_output.result;
			}
			break;
		}

		ASSERT(0);
		return ENR_Failed;
	}

	ENavResult NavCollider_NPCMove::IfCanGo( NavCollider* pCollider,const Vector3& startPos,const Vector3& endPos,const Vector3& roleSize,Vector3* pOutPos,float* pTotalDist,bool bPassCheck,int maxCDTimes/*=100*/,bool bMustSupportNPCNavMap,float moveFall/*=100.0f*/ )
	{
		const float speed=500.0f;
		const float timeOut=1000000.0f;

		//--
		m_input.startPos		= startPos;
		m_input.endPos			= endPos;
		m_input.roleSize		= roleSize;
		m_input.moveSpeed		= speed;
		m_input.bPassCheck		= bPassCheck;
		m_input.maxCDTimes		= maxCDTimes;
		m_input.moveFall		= moveFall;

		//--������ײ��־λ
		m_input.dwCDBoxTopFlags=0;
		m_input.dwCDBoxTopFlags|=NavSceneNode::EFlag_NotCarrier;
		if(bMustSupportNPCNavMap)
			m_input.dwCDBoxTopFlags|=NavSceneNode::EFlag_SupportNPCNavMap;

		m_input.dwCDBoxFlags=0;
		m_input.dwCDBoxFlags|=NavSceneNode::EFlag_NotCarrier;


		//--����xzƽ�淽��
		m_input.moveDir			= endPos-startPos;
		m_input.moveDir.y		= 0.0f;

		//--����xzƽ�����
		m_input.xzDist			= Vec3Len(&m_input.moveDir);

		//--��һ��xzƽ�淽��
		if(m_input.xzDist>0.0f)
			m_input.moveDir/=m_input.xzDist;
		else
			m_input.moveDir=MathConst::Zero3;

		//--��ײ���
		const Collider::tagCDResult& cdResult=m_pCollider->GetResult();

		m_pCollider->Init();
		if(!m_pCollider->Collide(pCollider,0.0f,timeOut))
		{
			if(pOutPos)*pOutPos=cdResult.path.Back();
			if(pTotalDist)*pTotalDist=cdResult.path.GetDist();
			return ENR_ToBeContinued;
		}

		if(pOutPos)*pOutPos=cdResult.path.Back();
		if(pTotalDist)*pTotalDist=cdResult.path.GetDist();
		return cdResult.result;
	}
}//namespace WorldBase