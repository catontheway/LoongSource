#include "StdAfx.h"
#include "NavCollider_Slide.h"
#include "NavColliderPrivateDef.h"
#include "NavCollider.h"
#include "NavMovePath.h"
#include "NavSceneNode.h"


namespace WorldBase
{
	/** ���㻬��ָ��������Ҫ��ʱ��
	*/
	inline float __CalcSlideTime( float slideDist )
	{
		return sqrtf(slideDist/g_halfGravitation);
	}

	//--class NavCollider_Slide::Collider---------------------------------------------------------------
	class NavCollider_Slide::Collider
	{
	public:
		struct tagCDResult				//��ײ���
		{
			ENavResult		result;		//��ײ����ֵ
			NavMovePath		path;		//����·��
		};

	public:
		NavCollider_Slide*	m_p;
		tagCDResult			m_cdResult;
		float				m_slideDist;
		Vector3				m_stepStart;
		POINT				m_stepStartTile;
		Vector3				m_slideDir;

	public:
		Collider(NavCollider_Slide* pParent)
			:m_p(pParent)
		{}
		~Collider()
		{}

		const tagCDResult& GetResult()
		{
			return m_cdResult;
		}

		bool Init(NavCollider* pCD)
		{
			m_slideDist=0.0f;
			m_stepStart=m_p->m_input.startPos;
			m_stepStartTile=_World2Tile(m_stepStart.x,m_stepStart.z);

			if(!pCD->SafeCalcSlideDir(m_stepStart.x,m_stepStart.z,m_slideDir))
				return false;

			m_cdResult.path.Clear();
			m_cdResult.path.SetStartPos(m_stepStart);

			return true;
		}

		bool Collide( NavCollider* pCD,float startTime,float deltaTime,bool bOnWaterCheck )
		{
			const Vector3 halfSize		= m_p->m_input.roleSize*0.5f;
			const Vector3 roleSize		= m_p->m_input.roleSize;
			const int maxCDTimes		= m_p->m_input.maxCDTimes;

			float lastTime=__CalcSlideTime(m_slideDist);

			for(int i=0;;++i)
			{
				//////////////////////////////////////////////////////////////////////////////////////////
				// ���㲽���յ�
				Vector3 stepEnd=m_stepStart+m_slideDir*g_collideStep;

				//////////////////////////////////////////////////////////////////////////////////////////
				// ������������
				if(!pCD->IfCanGo(stepEnd.x,stepEnd.z))
				{
					m_cdResult.result=ENR_Blocking;
					return true;
				}

				/////////////////////////////////////////////////////////////////////////////////////////
				// �����ɫAABB
				AABBox box;
				box.min.x=stepEnd.x-halfSize.x;
				box.max.x=stepEnd.x+halfSize.x;
				box.min.z=stepEnd.z-halfSize.z;
				box.max.z=stepEnd.z+halfSize.z;
				box.max.y=stepEnd.y+roleSize.y;
				box.min.y=-g_floatMax;

				/////////////////////////////////////////////////////////////////////////////////////////
				// �����жϵ�ǰλ���Ƿ���ģ��
				bool bHaveModel=pCD->HaveModel(box.min.x,box.min.z,box.max.x,box.max.z);

				/////////////////////////////////////////////////////////////////////////////////////////
				// ȡ�õ�ǰλ�õ���߶�
				//float groundH=pCD->SafeGetGroundHeightSlerp(box.min.x,box.min.z,box.max.x,box.max.z);
				float groundH=pCD->SafeGetGroundHeightSlerp(stepEnd.x,stepEnd.z);

				bool bOnGround=false;
				POINT stepEndTile=_World2Tile(stepEnd);
				if(bHaveModel)
				{
					//////////////////////////////////////////////////////////////////////////////////////
					// �ҳ���ŵ�
					if(!pCD->AABBoxCollideBoxTopAndTrn(box,0.0f,groundH,stepEnd.y,
						&bOnGround,NavSceneNode::EFlag_NotCarrier,NULL))
					{
						m_cdResult.result=ENR_Blocking;
						return true;
					}

					/////////////////////////////////////////////////////////////////////////////////////
					// ����ģ����?
					if(!bOnGround)
					{
						m_cdResult.result=ENR_Blocking;
						return true;
					}

					/////////////////////////////////////////////////////////////////////////////////////
					// �����ŵ��Ƿ���ͨ��
					box.min.y=stepEnd.y+10.0f;
					box.max.y=box.min.y+roleSize.y-10.0f;
					if(pCD->AABBoxCollideBoxAndTrn(
						box,0.0f,groundH,NavSceneNode::EFlag_NotCarrier))//����ͨ��?
					{
						m_cdResult.result=ENR_Blocking;
						return true;
					}

					//////////////////////////////////////////////////////////////////////////////////////
					// �ж����
					if(stepEnd.y>m_stepStart.y)
					{
						m_cdResult.result=ENR_Blocking;
						return true;
					}

					if(m_stepStartTile.x!=stepEndTile.x
						||m_stepStartTile.y!=stepEndTile.y)
					{
						if(m_stepStart.y-stepEnd.y<g_slideFall)
						{
							///////////////////////////////////////////////////////////////////////////////
							// �ж��Ƿ��ܳ���������
							if(pCD->IfWillSlide(stepEnd.x,stepEnd.z))
							{
								if(!pCD->SafeCalcSlideDir(stepEnd.x,stepEnd.z,m_slideDir))
								{
									m_cdResult.result=ENR_Blocking;
									return true;
								}
							}
							else
							{
								m_cdResult.result=ENR_Blocking;
								return true;
							}
						}
					}
				}
				else
				{
					stepEnd.y=groundH;
					bOnGround=true;

					///////////////////////////////////////////////////////////////////////////////////////
					// �ж����
					if(stepEnd.y>m_stepStart.y)
					{
						m_cdResult.result=ENR_Blocking;
						return true;
					}

					if(m_stepStartTile.x!=stepEndTile.x
						||m_stepStartTile.y!=stepEndTile.y)
					{
						if(m_stepStart.y-stepEnd.y<g_slideFall)
						{
							///////////////////////////////////////////////////////////////////////////////
							// �ж��Ƿ��ܳ���������
							if(pCD->IfWillSlide(stepEnd.x,stepEnd.z))
							{
								if(!pCD->SafeCalcSlideDir(stepEnd.x,stepEnd.z,m_slideDir))
								{
									m_cdResult.result=ENR_Blocking;
									return true;
								}
							}
							else
							{
								m_cdResult.result=ENR_Blocking;
								return true;
							}
						}
					}
				}

				//////////////////////////////////////////////////////////////////////////////////////////
				// �ж��Ƿ����ˮ
				if(bOnWaterCheck)
				{
					float waterH=0;
					if(pCD->IfWillOnWater(stepEnd,waterH))
					{
						stepEnd.y=waterH;
						m_cdResult.path.AddPoint(stepEnd);

						m_cdResult.result=ENR_WillOnWater;
						return true;
					}
				}

				//////////////////////////////////////////////////////////////////////////////////////////
				// �ж��Ƿ����Ӿ
				float swimH=0;
				if(pCD->IfWillSwim(stepEnd,roleSize.y,swimH))
				{
					stepEnd.y=swimH;
					m_cdResult.path.AddPoint(stepEnd);

					m_cdResult.result=ENR_WillSwim;
					return true;
				}

				//////////////////////////////////////////////////////////////////////////////////////////
				// �ۼӻ�������
				m_slideDist+=Vec3Dist(m_stepStart,stepEnd);

				/////////////////////////////////////////////////////////////////////////////////////////
				// ���㵱ǰʱ��
				float curTime=__CalcSlideTime(m_slideDist);

				/////////////////////////////////////////////////////////////////////////////////////////
				// �ж�ʱ���Ƿ��㹻
				if(curTime-lastTime>=deltaTime
					||i>=maxCDTimes)
				{
					m_cdResult.path.AddPoint(stepEnd);
					m_cdResult.result=ENR_ToBeContinued;

					//--
					m_stepStart=stepEnd;
					m_stepStartTile=stepEndTile;
					return false;
				}

				////////////////////////////////////////////////////////////////////////////////////////
				// ���һ��·����
				m_cdResult.path.AddPoint(stepEnd);

				//--
				m_stepStart=stepEnd;
				m_stepStartTile=stepEndTile;
			}

			//������ִ�е����
			ASSERT(0);
			return false;
		}
	};


	//--class NavCollider_Slide::SlideCtrl--------------------------------------------------------------
	class NavCollider_Slide::SlideCtrl
	{
	public:
		NavCollider_Slide*		m_p;

		float					m_totalTime;		//�ܻ���ʱ��
		float					m_slideTime;		//�ѻ���ʱ��
		float					m_endTime;			//��������ʱ��
		Vector3					m_endPos;			//����λ��
		bool					m_bFinished;		//������ɱ�־

	public:
		SlideCtrl(NavCollider_Slide* pParent)
			:m_p(pParent)
		{}
		~SlideCtrl()
		{}

		void Init()
		{
			//--��ʼ���ѻ���ʱ��
			m_slideTime=0.0f;
		}

		bool SlideStart(const Collider::tagCDResult& cdResult,float startTime,float deltaTime,float& slideTime,Vector3& out,float& endTime)
		{
			//--�����ܻ���ʱ��
			m_totalTime=__CalcSlideTime(cdResult.path.GetDist());

			//--�����ɱ�־
			m_bFinished=false;

			//--���û���Update
			return SlideUpdate(cdResult,startTime,deltaTime,slideTime,out,endTime);
		}

		bool SlideUpdate(const Collider::tagCDResult& cdResult,float startTime,float deltaTime,float& slideTime,Vector3& out,float& endTime)
		{
			if(m_bFinished)
			{
				slideTime=0.0f;
				endTime=m_endTime;
				out=m_endPos;
				return true;
			}

			float leaveTime=m_totalTime-m_slideTime;
			if(deltaTime>=leaveTime)
			{
				m_slideTime=m_totalTime;

				slideTime=leaveTime;

				endTime=startTime+slideTime;

				out=cdResult.path.Back();

				m_bFinished=true;
				m_endTime=endTime;
				m_endPos=out;
				return true;
			}
			else
			{
				m_slideTime+=deltaTime;

				slideTime=deltaTime;

				float totalDist=cdResult.path.GetDist();
				float slideDist=g_halfGravitation*m_slideTime*m_slideTime;
				
				float factor;
				if(totalDist>0)
					factor=slideDist/totalDist;
				else
					factor=0.0f;
				cdResult.path.SlerpPos(factor,out);
				return false;
			}
		}
	};

	//--class NavCollider_Slide-------------------------------------------------------------------------
	NavCollider_Slide::NavCollider_Slide(void):m_state(EState_None)
	{
		m_pCollider=new Collider(this);
		m_pSlideCtrl=new SlideCtrl(this);
	}

	NavCollider_Slide::~NavCollider_Slide(void)
	{
		SAFE_DELETE(m_pCollider);
		SAFE_DELETE(m_pSlideCtrl);
	}

	void NavCollider_Slide::Init( const Vector3& startPos,const Vector3& roleSize,int maxCDTimes )
	{
		//--�����������
		m_input.startPos	= startPos;
		m_input.roleSize	= roleSize;
		m_input.maxCDTimes	= maxCDTimes;

		//--���������0
		ZeroMemory(&m_output,sizeof(m_output));

		//--����״̬
		m_state				= EState_Initialized;
	}

	ENavResult NavCollider_Slide::Update( NavCollider* pCollider,float startTime,float deltaTime,Vector3& outPos,float& endTime,bool bOnWaterCheck/*=false*/ )
	{
		const Collider::tagCDResult& cdResult=m_pCollider->GetResult();

		switch(m_state)
		{
		case EState_Initialized:
			{
				//--��ʼ����ײ�����
				if(!m_pCollider->Init(pCollider))
				{
					m_state=EState_Finished;

					m_output.result=ENR_Blocking;
					m_output.outPos=m_input.startPos;
					m_output.endTime=startTime;

					outPos=m_output.outPos;
					endTime=m_output.endTime;
					return m_output.result;
				}

				//--��ʼ������������
				m_pSlideCtrl->Init();

				//--��ײ
				bool bCDOk=m_pCollider->Collide(pCollider,startTime,deltaTime,bOnWaterCheck);
				if(bCDOk)
				{
					m_output.result=cdResult.result;
				}

				//--��ʼ����
				float slideTime=0.0f;
				bool bSlideOk=m_pSlideCtrl->SlideStart(cdResult,startTime,deltaTime,slideTime,m_output.outPos,m_output.endTime);

				//--����״̬
				if(bCDOk&&bSlideOk)
				{
					m_state=EState_Finished;
					outPos=m_output.outPos;
					endTime=m_output.endTime;
					return m_output.result;
				}
				else if(bCDOk&&!bSlideOk)
				{
					m_state=EState_WaitForFinish;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
				else
				{
					m_state=EState_Sliding;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
			}
			break;
		case EState_Sliding:
			{
				//--�ƶ�����
				float slideTime=0.0f;
				bool bSlideOk=m_pSlideCtrl->SlideUpdate(cdResult,startTime,deltaTime,slideTime,m_output.outPos,m_output.endTime);
				if(!bSlideOk)
				{
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}

				startTime+=slideTime;
				deltaTime-=slideTime;

				//--��ײ
				bool bCDOk=m_pCollider->Collide(pCollider,startTime,deltaTime,bOnWaterCheck);
				if(bCDOk)
				{
					m_output.result=cdResult.result;
				}

				//--��ʼ�ƶ�
				bSlideOk=m_pSlideCtrl->SlideStart(cdResult,startTime,deltaTime,slideTime,m_output.outPos,m_output.endTime);

				//--����״̬
				if(bCDOk&&bSlideOk)
				{
					m_state=EState_Finished;
					outPos=m_output.outPos;
					endTime=m_output.endTime;
					return m_output.result;
				}
				else if(bCDOk&&!bSlideOk)
				{
					m_state=EState_WaitForFinish;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
				else
				{
					m_state=EState_Sliding;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
			}
			break;
		case EState_WaitForFinish:
			{
				//--�ƶ�����
				float slideTime=0.0f;
				bool bSlideOk=m_pSlideCtrl->SlideUpdate(cdResult,startTime,deltaTime,slideTime,m_output.outPos,m_output.endTime);
				if(!bSlideOk)
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

}//namespace WorldBase