#include "StdAfx.h"
#include "NavCollider_MoveOnCarrier.h"
#include "NavColliderPrivateDef.h"
#include "NavCarrierNode.h"
#include "NavCollider.h"
#include "NavMovePath.h"


namespace WorldBase
{
	//--class NavCollider_MoveOnCarrier::Collider--------------------------------------------------
	class NavCollider_MoveOnCarrier::Collider
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

			struct tagLanded
			{
				D3DVECTOR	landPos;

				void Fill(const Vector3& _landPos)
				{
					landPos				= _landPos;
				}
			};

			union 
			{
				tagCarry	carry;
				tagLanded	landed;
			};
		};
	public:
		NavCollider_MoveOnCarrier*	m_p;
		tagCDResult					m_cdResult;
		Vector3						m_stepStart;

	public:
		Collider(NavCollider_MoveOnCarrier* pParent)
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
			m_stepStart=m_p->m_input.localStartPos;
		}

		bool Collide( NavCollider* pCD,NavCarrierNode* pCarrier,float startTime,float deltaTime )
		{
			const Vector3& roleSize		= m_p->m_input.roleSize;
			const Vector3 halfSize		= m_p->m_input.roleSize*0.5f;
			const Vector3& dir			= m_p->m_input.localMoveDir;
			const float speed			= m_p->m_input.moveSpeed;
			const int maxCDTimes		= m_p->m_input.maxCDTimes;

			m_cdResult.path.Clear();
			m_cdResult.path.SetStartPos(m_stepStart);

			float movedTime=0.0f;
			for(int i=0;;++i)
			{
				/////////////////////////////////////////////////////////////////////////////////////
				// ��xzƽ���ƶ�һ������
				Vector3 xzStepEnd=m_stepStart+dir*g_collideStep;

				/////////////////////////////////////////////////////////////////////////////////////
				// ������xzƽ�����굱ǰ����Ҫ��ʱ��
				float xzStepTime=g_collideStep/speed;

				/////////////////////////////////////////////////////////////////////////////////////
				// ���ɽ�ɫAABB
				AABBox box;
				box.min.x=xzStepEnd.x-halfSize.x;
				box.max.x=xzStepEnd.x+halfSize.x;
				box.min.z=xzStepEnd.z-halfSize.z;
				box.max.z=xzStepEnd.z+halfSize.z;

				/////////////////////////////////////////////////////////////////////////////////////
				// ���ؾ���ײ��ȡ����ŵ���Ϊ�����յ�
				box.max.y=xzStepEnd.y+g_moveFall;
				box.min.y=-g_floatMax;
				float intersectH=box.min.y;
				if(!pCarrier->AABBoxCollideBoxTopHighestInLocal(box,intersectH))
				{
					pCarrier->EnableCollide(false);
					CollideWorldOutOfCarrier(pCD,pCarrier,xzStepEnd,startTime+movedTime+xzStepTime);
					pCarrier->EnableCollide(true);

					return true;
				}

				Vector3 stepEnd=xzStepEnd;
				stepEnd.y=intersectH;

				/////////////////////////////////////////////////////////////////////////////////////
				// �������굱ǰ����Ҫ��ʱ��
				float stepTime=Vec3Dist(m_stepStart,stepEnd)/speed;

				/////////////////////////////////////////////////////////////////////////////////////
				// �ж����
				if(fabs(m_stepStart.y-stepEnd.y)>g_moveFall)
				{
					if(stepEnd.y>m_stepStart.y)
					{
						m_cdResult.result=ENR_Blocking;
						return true;
					}
					else
					{
						m_cdResult.path.AddPoint(xzStepEnd);
						m_cdResult.result=ENR_WillDrop;
						return true;
					}
				}

				/////////////////////////////////////////////////////////////////////////////////////
				// �����ŵ��Ƿ��ͨ��
				box.max.y=stepEnd.y+roleSize.y-10.0f;
				box.min.y=stepEnd.y+10.0f;
				if(pCarrier->AABBoxCollideBoxInLocal(box))
				{
					m_cdResult.result=ENR_Blocking;
					return true;
				}

				/////////////////////////////////////////////////////////////////////////////////////
				// ���ؾ����볡����ײ
				{
					pCarrier->EnableCollide(false);
					bool bResult=CollideWorldOnCarrier(pCD,pCarrier,stepEnd,startTime+movedTime+stepTime);
					pCarrier->EnableCollide(true);

					if(bResult)
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

		void CollideWorldOutOfCarrier( NavCollider* pCD,NavCarrierNode* pCarrier,const Vector3& stepEnd,float curTime )
		{
			const Vector3 halfSize	= m_p->m_input.roleSize*0.5f;
			const Vector3& roleSize	= m_p->m_input.roleSize;

			Vector3 wpt;
			pCarrier->PosToWorld(stepEnd,curTime,wpt);

			//////////////////////////////////////////////////////////////////////////////////////////
			// ���ɽ�ɫAABB
			AABBox box;
			box.min.x=wpt.x-halfSize.x;
			box.max.x=wpt.x+halfSize.x;
			box.min.z=wpt.z-halfSize.z;
			box.max.z=wpt.z+halfSize.z;

			//////////////////////////////////////////////////////////////////////////////////////////
			// ��ײ�õ���ŵ�
			box.max.y=wpt.y+g_moveFall;
			box.min.y=-g_floatMax;

			float intersectH=0.0f;
			DWORD dwCarrierObjID=-1;
			if(!pCD->AABBoxCollideBoxTopAndTrn(box,curTime,intersectH,NULL,0,&dwCarrierObjID))
			{
				m_cdResult.result=ENR_Blocking;
				return;
			}

			//////////////////////////////////////////////////////////////////////////////////////////
			// �ж����
			if(fabs(wpt.y-intersectH)>g_moveFall)
			{
				if(intersectH>wpt.y)
				{
					m_cdResult.result=ENR_Blocking;
					return;
				}
				else
				{
					/////////////////////////////////////////////////////////////////////////////////
					// ����Ƿ񱻵�ס
					box.max.y=wpt.y+roleSize.y;
					box.min.y=wpt.y;
					if(pCD->AABBoxCollideBox(box,curTime))
					{
						m_cdResult.result=ENR_Blocking;
						return;
					}
					else
					{
						m_cdResult.path.AddPoint(stepEnd);
						m_cdResult.result=ENR_WillDrop;
						return;
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////////////////////
			// �����ŵ��Ƿ��ͨ��
			box.max.y=intersectH+roleSize.y-10.0f;
			box.min.y=intersectH+10.0f;
			if(pCD->AABBoxCollideBox(box,curTime))
			{
				m_cdResult.result=ENR_Blocking;
				return;
			}

			//////////////////////////////////////////////////////////////////////////////////////////
			// ��½�������ؾ߻�̬����
			wpt.y=intersectH;
			if(dwCarrierObjID==-1)
			{
				m_cdResult.path.AddPoint(stepEnd);
				m_cdResult.result=ENR_CarryLanded;
				m_cdResult.landed.Fill(wpt);
				return;
			}
			else
			{
				m_cdResult.path.AddPoint(stepEnd);
				m_cdResult.result=ENR_WillCarry;
				m_cdResult.carry.Fill(dwCarrierObjID,wpt);
				return;
			}
		}

		bool CollideWorldOnCarrier( NavCollider* pCD,NavCarrierNode* pCarrier,const Vector3& stepEnd,float curTime )
		{
			const Vector3 halfSize	= m_p->m_input.roleSize*0.5f;
			const Vector3& roleSize	= m_p->m_input.roleSize;

			Vector3 wpt;
			pCarrier->PosToWorld(stepEnd,curTime,wpt);

			//////////////////////////////////////////////////////////////////////////////////////////
			// ���ɽ�ɫAABB
			AABBox box;
			box.min.x=wpt.x-halfSize.x;
			box.max.x=wpt.x+halfSize.x;
			box.min.z=wpt.z-halfSize.z;
			box.max.z=wpt.z+halfSize.z;

			//////////////////////////////////////////////////////////////////////////////////////////
			// ��ײ�õ���ŵ�
			box.max.y=wpt.y+g_moveFall;
			box.min.y=wpt.y;

			float intersectH=0.0f;
			DWORD dwCarrierObjID=-1;
			if(!pCD->AABBoxCollideBoxTop(box,curTime,intersectH,0,&dwCarrierObjID))
			{
				m_cdResult.path.AddPoint(stepEnd);
				m_cdResult.result=ENR_ToBeContinued;
				return false;
			}

			//////////////////////////////////////////////////////////////////////////////////////////
			// �����ŵ��Ƿ��ͨ��
			box.max.y=intersectH+roleSize.y-10.0f;
			box.min.y=intersectH+10.0f;
			if(pCD->AABBoxCollideBox(box,curTime))
			{
				m_cdResult.result=ENR_Blocking;
				return true;
			}

			//////////////////////////////////////////////////////////////////////////////////////////
			// ��½�������ؾ߻�̬����
			wpt.y=intersectH;
			if(dwCarrierObjID==-1)
			{
				m_cdResult.path.AddPoint(stepEnd);
				m_cdResult.result=ENR_CarryLanded;
				m_cdResult.landed.Fill(wpt);
				return true;
			}
			else
			{
				m_cdResult.path.AddPoint(stepEnd);
				m_cdResult.result=ENR_WillCarry;
				m_cdResult.carry.Fill(dwCarrierObjID,wpt);
				return true;
			}
		}
	};
	

	//--class NavCollider_MoveOnCarrier::MoveCtrl--------------------------------------------------
	class NavCollider_MoveOnCarrier::MoveCtrl
	{
	public:
		NavCollider_MoveOnCarrier*	m_p;

		float						m_totalTime;		//���ƶ�ʱ��
		float						m_movedTime;		//���ƶ�ʱ��
		float						m_endTime;			//�ƶ�����ʱ��
		Vector3						m_endPos;			//�ƶ�����λ��
		bool						m_bFinished;		//�ƶ���ɱ�־

	public:
		MoveCtrl(NavCollider_MoveOnCarrier* pParent)
			:m_p(pParent)
		{}
		~MoveCtrl()
		{}

		bool MoveStart(const Collider::tagCDResult& cdResult,NavCarrierNode* pCarrier,float startTime,float deltaTime,float& moveTime,float& endTime,Vector3& out)
		{
			//--�������ƶ�ʱ��
			m_totalTime=cdResult.path.GetDist()/m_p->m_input.moveSpeed;

			//--��ʼ�����ƶ�ʱ��
			m_movedTime=0.0f;

			//--����ƶ���ɱ�־
			m_bFinished=false;

			//--�����ƶ�Update
			return MoveUpdate(cdResult,pCarrier,startTime,deltaTime,moveTime,endTime,out);
		}

		bool MoveUpdate(const Collider::tagCDResult& cdResult,NavCarrierNode* pCarrier,float startTime,float deltaTime,float& moveTime,float& endTime,Vector3& out)
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

				if(cdResult.result==ENR_WillCarry)
					out=cdResult.carry.carryPos;
				else if(cdResult.result==ENR_CarryLanded)
					out=cdResult.landed.landPos;
				else
					pCarrier->PosToWorld(cdResult.path.Back(),endTime,out);

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
				Vector3 local;
				cdResult.path.SlerpPos(factor,local);

				pCarrier->PosToWorld(local,startTime+deltaTime,out);

				return false;
			}
		}
	};



	//--class NavCollider_MoveOnCarrier------------------------------------------------------------
	NavCollider_MoveOnCarrier::NavCollider_MoveOnCarrier(void):m_state(EState_None)
	{
		m_pCollider=new Collider(this);
		m_pMoveCtrl=new MoveCtrl(this);
	}

	NavCollider_MoveOnCarrier::~NavCollider_MoveOnCarrier(void)
	{
		SAFE_DELETE(m_pCollider);
		SAFE_DELETE(m_pMoveCtrl);
	}

	void NavCollider_MoveOnCarrier::Init( DWORD dwCarrierObjID,const Vector3& localStartPos,const Vector3& localMoveDir,const Vector3& roleSize,float moveSpeed,int maxCDTimes )
	{
		//--����������
		ASSERT(moveSpeed>0.0f);
		ASSERT(maxCDTimes>0);

		//--�����������
		m_input.dwCarrierObjID	= dwCarrierObjID;
		m_input.localStartPos	= localStartPos;
		m_input.localMoveDir	= localMoveDir;
		m_input.roleSize		= roleSize;
		m_input.moveSpeed		= moveSpeed;
		m_input.maxCDTimes		= maxCDTimes;

		//--�ƶ�����ת����xzƽ�淽��
		m_input.localMoveDir.y	= 0;
		Vec3Normalize(m_input.localMoveDir);

		//--��ʼ��m_output
		ZeroMemory(&m_output,sizeof(m_output));
		m_output.dwCarrierObjID=-1;

		//--����״̬
		m_state					= EState_Initialized;
	}

	ENavResult NavCollider_MoveOnCarrier::Update( NavCollider* pCollider,float startTime,float deltaTime,Vector3& outPos,float& endTime,DWORD& dwCarrierObjID )
	{
		const Collider::tagCDResult& cdResult=m_pCollider->GetResult();

		//--�����ؾ�
		NavCarrierNode* pCarrier=pCollider->FindCarrier(m_input.dwCarrierObjID);
		ASSERT(pCarrier!=NULL&&pCarrier->IsRunning());

		switch(m_state)
		{
		case EState_Initialized:
			{
				//--��ʼ����ײ�����
				m_pCollider->Init();

				//--��ײ
				bool bCDOk=m_pCollider->Collide(pCollider,pCarrier,startTime,deltaTime);
				if(bCDOk)
				{
					m_output.result=cdResult.result;
					if(cdResult.result==ENR_WillCarry)
						m_output.dwCarrierObjID=cdResult.carry.dwCarrierObjID;
				}

				//--��ʼ�ƶ�
				float moveTime=0.0f;
				bool bMoveOk=m_pMoveCtrl->MoveStart(cdResult,pCarrier,startTime,deltaTime,moveTime,m_output.endTime,m_output.outPos);

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
				bool bMoveOk=m_pMoveCtrl->MoveUpdate(cdResult,pCarrier,startTime,deltaTime,moveTime,m_output.endTime,m_output.outPos);
				if(!bMoveOk)
				{
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}

				startTime+=moveTime;
				deltaTime-=moveTime;

				//--��ײ
				bool bCDOk=m_pCollider->Collide(pCollider,pCarrier,startTime,deltaTime);
				if(bCDOk)
				{
					m_output.result=cdResult.result;
					if(cdResult.result==ENR_WillCarry)
						m_output.dwCarrierObjID=cdResult.carry.dwCarrierObjID;
				}

				//--��ʼ�ƶ�
				bMoveOk=m_pMoveCtrl->MoveStart(cdResult,pCarrier,startTime,deltaTime,moveTime,m_output.endTime,m_output.outPos);

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
				bool bMoveOk=m_pMoveCtrl->MoveUpdate(cdResult,pCarrier,startTime,deltaTime,moveTime,m_output.endTime,m_output.outPos);
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
}//namespace WorldBase