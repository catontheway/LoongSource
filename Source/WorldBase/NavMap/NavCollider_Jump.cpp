#include "StdAfx.h"
#include "NavCollider_Jump.h"
#include "NavColliderPrivateDef.h"
#include "NavCollider.h"
#include "NavCarrierNode.h"


namespace WorldBase
{

	/** ���㵽��������ָ���߶ȵ�ʱ��
	*/
	inline bool __CalcJumpCollideTime(float ySpeed,float startH,float intersectH,float minTime,float g,float& time)
	{
		float a=0.5f*g;
		float b=-ySpeed;
		float c=intersectH-startH;
		float d=b*b-4.0f*a*c;
		if(d>0)
		{
			time=(-b+sqrtf(d))/(2.0f*a);
			if(time>=minTime)
			{
				return true;
			}
		}
		return false;
	}

	/** ����ʱ�����������λ��
	*/
	inline void __CalcJumpPos( const Vector3& start,const Vector3& xzDir,float xzSpeed,float ySpeed,float time,float g,Vector3& out )
	{
		out=start+xzDir*(xzSpeed*time);
		out.y=start.y+(ySpeed*time-0.5f*g*time*time);//v0t-1/2gt^2
	}


	//--class NavCollider_Jump::Collider------------------------------------------------------------
	class NavCollider_Jump::Collider
	{
	public:
		struct tagCDResult						//��ײ���
		{
			ENavResult	result;					//��ײ����ֵ

			struct tagLanded
			{
				float		landTime;			//��½ʱ�䣨���������ʱ�䣩

				void Fill(float _landTime)
				{
					landTime			= _landTime;
				}
			};

			struct tagSwim
			{
				float		swimTime;			//��Ӿʱ�䣨���������ʱ�䣩

				void Fill(float _swimTime)
				{
					swimTime			= _swimTime;
				}
			};

			struct tagOnWater
			{
				float		onWaterTime;		//��ˮʱ�䣨���������ʱ�䣩

				void Fill(float _onWaterTime)
				{
					onWaterTime			= _onWaterTime;
				}
			};

			struct tagSlide
			{
				float		slideTime;			//����ʱ�䣨���������ʱ�䣩

				void Fill(float _slideTime)
				{
					slideTime			= _slideTime;
				}
			};

			struct tagToBeContinue
			{
				float		lastCDTime;			//���һ����ײʱ�䣨���������ʱ�䣩

				void Fill(float _lastCDTime)
				{
					lastCDTime			= _lastCDTime;
				}
			};

			struct tagCarry
			{
				float		cdTime;				//��ײʱ�䣨���������ʱ�䣩
				D3DVECTOR	localLandPos;		//�ֲ��ռ���½λ��
				DWORD		dwCarrierObjID;		//�ؾ�ObjID

				void Fill(float _cdTime,const Vector3& _localLandPos,DWORD _dwCarrierObjID)
				{
					cdTime				= _cdTime;
					localLandPos		= _localLandPos;
					dwCarrierObjID		= _dwCarrierObjID;
				}
			};

			struct tagVDrop
			{
				float		vdropTime;			//��ֱ����ʱ�䣨���������ʱ�䣩
				float		cdCarrierTime;		//��ײ���ؾߵ�ʱ�䣨����ʱ�䣩
				DWORD		dwCarrierObjID;		//��ײ�����ؾ�ObjID��-1��ʾδ��ײ���ؾ�

				void Fill(float _vdropTime,const float _cdCarrierTime,DWORD _dwCarrierObjID)
				{
					vdropTime			= _vdropTime;
					cdCarrierTime		= _cdCarrierTime;
					dwCarrierObjID		= _dwCarrierObjID;
				}			
			};

			union 
			{
				tagLanded		landed;
				tagSwim			swim;
				tagOnWater		onWater;
				tagSlide		slide;
				tagToBeContinue	toBeContinue;
				tagCarry		carry;
				tagVDrop		vdrop;
			};
		};

	public:
		NavCollider_Jump*	m_p;
		Vector3				m_lastPos;			//�ϴ���ײλ��
		float				m_lastTime;			//�ϴ���ײʱ�䣨���������ʱ�䣩
		float				m_cdInterval;		//��ײʱ����


	public:
		Collider(NavCollider_Jump* pParent)
			:m_p(pParent)
		{}
		~Collider()
		{}

		void Init()
		{
			//--
			m_lastTime=0.0f;

			//--
			m_lastPos=m_p->m_input.startPos;

			//--
			if(m_p->m_input.xzSpeed>0.0f
				||m_p->m_input.ySpeed>0.0f)
			{
				m_cdInterval=g_collideStep/max(m_p->m_input.xzSpeed,m_p->m_input.ySpeed);
				if(m_cdInterval>0.1f)
					m_cdInterval=0.1f;
			}
			else
				m_cdInterval=0.1f;
		}

		bool Collide( NavCollider* pCD,float startTime,float deltaTime,BitMap* pSpecCangoMap,bool bOnWaterCheck,tagCDResult& out )
		{
			const Vector3& startPos	= m_p->m_input.startPos;
			const float xzSpeed		= m_p->m_input.xzSpeed;
			const float ySpeed		= m_p->m_input.ySpeed;
			const Vector3& dir		= m_p->m_input.xzDir;
			const float g			= m_p->m_input.g;
			const float timeOut		= m_p->m_input.cdTimeOut;

			for(float t=m_cdInterval;;t+=m_cdInterval)
			{
				//--
				float curTime=m_lastTime+m_cdInterval;

				//--
				Vector3 curPos=startPos+dir*(xzSpeed*curTime);
				curPos.y=startPos.y+(ySpeed*curTime-0.5f*g*curTime*curTime);//v0*t-0.5*g*t^2;

				//--������������
				if(!pCD->IfCanGo(curPos.x,curPos.z,pSpecCangoMap))
				{
					out.result=ENR_WillVDrop;
					out.vdrop.Fill(m_lastTime,0.0f,-1);
					return true;
				}

				//--��ײ
				if(ySpeed>g*curTime)//����
				{
					if(RiseCollide(pCD,curPos,curTime,m_lastTime,startTime+t,out))
						return true;
				}
				else//�½�
				{
					if(DropCollide(pCD,curPos,curTime,m_lastTime,startTime+t,bOnWaterCheck,out))
						return true;
				}

				//--����cdInterval
				float jumpSpeed=Vec3Dist(m_lastPos,curPos)/m_cdInterval;
				m_cdInterval=g_collideStep/jumpSpeed;
				if(m_cdInterval>0.1f)
					m_cdInterval=0.1f;

				//--�ж�ʱ���Ƿ��ˣ�
				if(t>=deltaTime
					||t>=timeOut)
				{
					out.result=ENR_ToBeContinued;
					out.toBeContinue.Fill(curTime);

					//--
					m_lastPos=curPos;
					m_lastTime=curTime;
					return false;
				}

				//--
				m_lastPos=curPos;
				m_lastTime=curTime;
			}

			//������ִ�е����
			ASSERT(0);
			return false;
		}	

		bool RiseCollide( NavCollider* pCD,Vector3 curPos,float curTime,float lastTime,float absTime,tagCDResult& out )
		{
			const Vector3 halfSize		= m_p->m_input.roleSize*0.5f;
			const Vector3& roleSize		= m_p->m_input.roleSize;
			const DWORD cdBoxFlags		= m_p->m_input.cdBoxFlag;

			//--���ɽ�ɫAABB
			AABBox box;
			box.min.x=curPos.x-halfSize.x;
			box.max.x=curPos.x+halfSize.x;
			box.min.z=curPos.z-halfSize.z;
			box.max.z=curPos.z+halfSize.z;
			box.min.y=curPos.y;
			box.max.y=curPos.y+roleSize.y;

			//--�����Ƿ�����ײ
			DWORD dwCarrierObjID=-1;
			if(pCD->AABBoxCollideBoxAndTrn(box,absTime,cdBoxFlags,&dwCarrierObjID))
			{
				out.result=ENR_WillVDrop;
				out.vdrop.Fill(m_lastTime,absTime,dwCarrierObjID);
				return true;
			}

			return false;
		}

		bool DropCollide( NavCollider* pCD,Vector3 curPos,float curTime,float lastTime,float absTime,bool bOnWaterCheck,tagCDResult& out )
		{
			const Vector3 halfSize		= m_p->m_input.roleSize*0.5f;
			const Vector3& roleSize		= m_p->m_input.roleSize;
			const Vector3& startPos		= m_p->m_input.startPos;
			const Vector3& dir			= m_p->m_input.xzDir;
			const float xzSpeed			= m_p->m_input.xzSpeed;
			const float ySpeed			= m_p->m_input.ySpeed;
			const float g				= m_p->m_input.g;
			const DWORD cdBoxTopFlags	= m_p->m_input.cdBoxTopFlags;
			const DWORD cdBoxFlags		= m_p->m_input.cdBoxFlag;

			////////////////////////////////////////////////////////////////////////////////////
			// ���ɽ�ɫAABB
			AABBox box;
			box.min.x=curPos.x-halfSize.x;
			box.max.x=curPos.x+halfSize.x;
			box.min.z=curPos.z-halfSize.z;
			box.max.z=curPos.z+halfSize.z;
			box.min.y=curPos.y;
			box.max.y=curPos.y+roleSize.y;

			////////////////////////////////////////////////////////////////////////////////////
			// �����Ƿ�����ײ
			DWORD cdCarrierObjID=-1;
			box.min.y=curPos.y;
			box.max.y=curPos.y+roleSize.y;
			if(pCD->AABBoxCollideBoxAndTrn(box,absTime,cdBoxTopFlags,&cdCarrierObjID))
			{
				////////////////////////////////////////////////////////////////////////////////////
				// �ж��Ƿ���ײ����½��
				float intersectH=0;
				bool bOnGround=false;
				DWORD landCarrierObjID=-1;
				if(!pCD->AABBoxCollideBoxTopAndTrn(box,absTime,intersectH,&bOnGround,cdBoxFlags,&landCarrierObjID))
				{
					out.result=ENR_WillVDrop;
					out.vdrop.Fill(lastTime,absTime,cdCarrierObjID);
					return true;
				}

				////////////////////////////////////////////////////////////////////////////////////
				// ���㵽������������ײ�߶ȵ�ʱ��
				float intersectTime=0.0f;
				if(!__CalcJumpCollideTime(ySpeed,startPos.y,intersectH,lastTime,g,intersectTime))
				{
					out.result=ENR_WillVDrop;
					out.vdrop.Fill(lastTime,absTime,cdCarrierObjID);
					return true;
				}

				////////////////////////////////////////////////////////////////////////////////////
				// ���㵽��������������ײ�߶ȵ�����
 				__CalcJumpPos(startPos,dir,xzSpeed,ySpeed,intersectTime,g,curPos);

				////////////////////////////////////////////////////////////////////////////////////
				// �ж���½���Ƿ����ͨ��
				box.min.y=curPos.y+10.0f;
				box.max.y=box.min.y+roleSize.y-10.0f;
				if(pCD->AABBoxCollideBoxAndTrn(box,absTime,cdBoxTopFlags,&cdCarrierObjID))
				{
					out.result=ENR_WillVDrop;
					out.vdrop.Fill(lastTime,absTime,cdCarrierObjID);
					return true;
				}

				////////////////////////////////////////////////////////////////////////////////////
				// �ж���½����
				if(landCarrierObjID!=-1)//�ؾ�
				{
					NavCarrierNode* pCarrier=pCD->FindCarrier(landCarrierObjID);
					ASSERT(pCarrier!=NULL&&pCarrier->IsRunning());

					Vector3 landPos;
					pCarrier->PosToLocal(curPos,absTime,landPos);

					out.result=ENR_WillCarry;
					out.carry.Fill(curTime,landPos,landCarrierObjID);
					return true;
				}
				else//��̬����
				{
					////////////////////////////////////////////////////////////////////////////////////
					// �ж��Ƿ����ˮ
					float waterH=0;
					if(bOnWaterCheck && pCD->IfWillOnWater(curPos,waterH))
					{
						curPos.y=waterH;
						float onWaterTime=0;
						__CalcJumpCollideTime(ySpeed,startPos.y,curPos.y,lastTime,g,onWaterTime);
						out.result=ENR_WillOnWater;
						out.onWater.Fill(onWaterTime);
						return true;
					}

					////////////////////////////////////////////////////////////////////////////////////
					// �ж��Ƿ����Ӿ
					if(pCD->IfWillSwim(curPos,roleSize.y,curPos.y))
					{
						float swimTime=0;
						__CalcJumpCollideTime(ySpeed,startPos.y,curPos.y,lastTime,g,swimTime);
						out.result=ENR_WillSwim;
						out.swim.Fill(swimTime);
						return true;
					}

					////////////////////////////////////////////////////////////////////////////////////
					// �жϻ���
					if(bOnGround
						&&pCD->IfWillSlide(curPos.x,curPos.z))
					{
						out.result=ENR_WillSlide;
						out.slide.Fill(intersectTime);
						return true;
					}

					////////////////////////////////////////////////////////////////////////////////////
					// ��½����̬����
					out.result=ENR_Landed;
					out.landed.Fill(intersectTime);
					return true;
				}
			}
			else
			{
				////////////////////////////////////////////////////////////////////////////////////
				// �ж��Ƿ����ˮ
				float waterH=0;
				if(bOnWaterCheck && pCD->IfWillOnWater(curPos,waterH))
				{
					curPos.y=waterH;
					float onWaterTime=0;
					__CalcJumpCollideTime(ySpeed,startPos.y,curPos.y,lastTime,g,onWaterTime);
					out.result=ENR_WillOnWater;
					out.onWater.Fill(onWaterTime);
					return true;
				}

				////////////////////////////////////////////////////////////////////////////////////
				// �ж��Ƿ����Ӿ
				if(pCD->IfWillSwim(curPos,roleSize.y,curPos.y))
				{
					float swimTime=0;
					__CalcJumpCollideTime(ySpeed,startPos.y,curPos.y,lastTime,g,swimTime);
					out.result=ENR_WillSwim;
					out.swim.Fill(swimTime);
					return true;
				}

				return false;
			}
		}
	};

	//--class NavCollider_Jump::JumpCtrl------------------------------------------------------------
	class NavCollider_Jump::JumpCtrl
	{
	public:
		NavCollider_Jump*		m_p;
		Collider::tagCDResult	m_cdResult;			//��ײ���
		float					m_startJumpTime;	//����ʱ��
		float					m_totalTime;		//����Ծʱ��
		float					m_jumpedTime;		//����Ծʱ��
		bool					m_bFinished;		//����Ծ���
		float					m_endTime;
		Vector3					m_endPos;


	public:
		JumpCtrl(NavCollider_Jump* pParent)
			:m_p(pParent)
		{}
		~JumpCtrl()
		{}

		void Init(float startJumpTime)
		{
			//--
			m_startJumpTime=startJumpTime;

			//--
			m_jumpedTime=0.0f;
		}

	
		bool JumpStart(NavCollider* pCD,const Collider::tagCDResult& cdResult,float startTime,float deltaTime,float& jumpTime,float& endTime,Vector3& out)
		{
			//--������ײ���
			m_cdResult=cdResult;

			//--��������Ծʱ��
			m_totalTime=0.0f;
			switch(cdResult.result)
			{
			case ENR_ToBeContinued:
				{
					m_totalTime=cdResult.toBeContinue.lastCDTime;
				}
				break;
			case ENR_Landed:
				{
					m_totalTime=cdResult.landed.landTime;
				}
				break;
			case ENR_WillCarry:
				{
					m_totalTime=cdResult.carry.cdTime;
				}
				break;
			case ENR_WillSlide:
				{
					m_totalTime=cdResult.slide.slideTime;
				}
				break;
			case ENR_WillSwim:
				{
					m_totalTime=cdResult.swim.swimTime;
				}
			case ENR_WillOnWater:
				{
					m_totalTime=cdResult.onWater.onWaterTime;
				}
				break;
			case ENR_WillVDrop:
				{
					m_totalTime=cdResult.vdrop.vdropTime;
				}
				break;
			default:
				{
					ASSERT(0);
				}
			}

			//--�����ɱ�־
			m_bFinished=false;

			//--������ԾUpdate
			return JumpUpdate(pCD,startTime,deltaTime,jumpTime,endTime,out);
		}

		bool IsLandedToCarrier( NavCarrierNode* pCarrier,const Vector3& localLandPos,float curTime,float jumpTime,float minTime,float& landTime )
		{
			//���㵱ǰ�ؾ���½λ��
			Vector3 worldLandPos;
			pCarrier->PosToWorld(localLandPos,curTime,worldLandPos);

			//���㵱ǰ��ɫ�ŵ׸߶�
			float roleH=m_p->m_input.startPos.y+(m_p->m_input.ySpeed*jumpTime-0.5f*m_p->m_input.g*jumpTime*jumpTime);//v0t-1/2gt^2

			//�жϽ�ɫ�ŵ׸߶��Ƿ������½�߶�
			if(roleH<=worldLandPos.y)
			{
				return __CalcJumpCollideTime(m_p->m_input.ySpeed,m_p->m_input.startPos.y,worldLandPos.y,minTime,m_p->m_input.g,landTime);
			}

			return false;
		}

		bool JumpUpdate(NavCollider* pCD,float startTime,float deltaTime,float& jumpTime,float& endTime,Vector3& out)
		{
			if(m_bFinished)
			{
				jumpTime=0.0f;
				endTime=m_endTime;
				out=m_endPos;
				return true;
			}

			switch(m_cdResult.result)
			{
			case ENR_ToBeContinued:
			case ENR_Landed:
			case ENR_WillSlide:
			case ENR_WillSwim:
			case ENR_WillVDrop:
			case ENR_WillOnWater:
				{
					float leaveTime=m_totalTime-m_jumpedTime;
					if(deltaTime>=leaveTime)//finish
					{
						//output jumptime
						jumpTime=leaveTime;

						//add jumped time
						m_jumpedTime+=jumpTime;

						//output endtime
						endTime=m_startJumpTime+m_totalTime;

						//output pos
						__CalcJumpPos(m_p->m_input.startPos,
							m_p->m_input.xzDir,
							m_p->m_input.xzSpeed,
							m_p->m_input.ySpeed,
							m_totalTime,
							m_p->m_input.g,
							out);

						//save pos and endtime
						m_bFinished=true;
						m_endPos=out;
						m_endTime=endTime;

						return true;
					}
					else//jumpping
					{
						//output jumptime
						jumpTime=deltaTime;

						//add jumped time
						m_jumpedTime+=jumpTime;

						//output pos
						__CalcJumpPos(m_p->m_input.startPos,
							m_p->m_input.xzDir,
							m_p->m_input.xzSpeed,
							m_p->m_input.ySpeed,
							m_jumpedTime,
							m_p->m_input.g,
							out);

						return false;
					}
				}
				break;
			case ENR_WillCarry:
				{
					//--�����ؾ�
					NavCarrierNode* pCarrier=pCD->FindCarrier(m_cdResult.carry.dwCarrierObjID);
					ASSERT(pCarrier!=NULL&&pCarrier->IsRunning());

					//--�ж��Ƿ�����½���ؾ�
					float landTime=0.0f;
					if(IsLandedToCarrier(pCarrier,m_cdResult.carry.localLandPos,startTime+deltaTime,m_jumpedTime+deltaTime,m_jumpedTime,landTime))
					{//finish

						//output jumptime
						jumpTime=landTime-m_jumpedTime;

						//output endtime
						endTime=m_startJumpTime+landTime;

						//output pos
						__CalcJumpPos(m_p->m_input.startPos,m_p->m_input.xzDir,m_p->m_input.xzSpeed,m_p->m_input.ySpeed,landTime,m_p->m_input.g,out);

						//save pos and endtime
						m_bFinished=true;
						m_endPos=out;
						m_endTime=endTime;

						return true;
					}

					//--�ж��Ƿ񳬹���Ծ��ʱ��
					float leaveTime=m_totalTime-m_jumpedTime;
					if(deltaTime>=leaveTime)//finish
					{
						//output jumptime
						jumpTime=leaveTime;

						//output endtime
						endTime=m_startJumpTime+m_totalTime;

						//output pos
						__CalcJumpPos(m_p->m_input.startPos,m_p->m_input.xzDir,m_p->m_input.xzSpeed,m_p->m_input.ySpeed,m_totalTime,m_p->m_input.g,out);

						//save pos and endtime
						m_bFinished=true;
						m_endPos=out;
						m_endTime=endTime;

						return true;
					}
					else//jumpping
					{
						//output jumptime
						jumpTime=deltaTime;

						//output pos
						__CalcJumpPos(m_p->m_input.startPos,
							m_p->m_input.xzDir,
							m_p->m_input.xzSpeed,
							m_p->m_input.ySpeed,
							m_jumpedTime,
							m_p->m_input.g,
							out);

						//add jumped time
						m_jumpedTime+=deltaTime;
						return false;
					}
				}
				break;
			default:
				{
					ASSERT(0);
				}
			}

			return true;
		}

	};


	//--class NavCollider_Jump----------------------------------------------------------------------
	NavCollider_Jump::NavCollider_Jump(void):m_state(EState_None)
	{
		m_pCollider=new Collider(this);
		m_pJumpCtrl=new JumpCtrl(this);
	}

	NavCollider_Jump::~NavCollider_Jump(void)
	{
		SAFE_DELETE(m_pCollider);
		SAFE_DELETE(m_pJumpCtrl);
	}

	void NavCollider_Jump::Init( const Vector3& startPos,const Vector3& xzDir,const Vector3& roleSize,float xzSpeed,float ySpeed,float g,float cdTimeOut,bool bTrnOnly,bool bIgnoreCarrier )
	{
		//--�����������
		m_input.startPos		= startPos;
		m_input.xzDir			= xzDir;
		m_input.roleSize		= roleSize;
		m_input.xzSpeed			= xzSpeed;
		m_input.ySpeed			= ySpeed;
		m_input.g				= g;
		m_input.cdTimeOut		= cdTimeOut;

		//--������ײ��־λ 
		m_input.cdBoxTopFlags	= 0;
		if(bTrnOnly)
			m_input.cdBoxTopFlags|=NavSceneNode::EFlag_Terrain;
		if(bIgnoreCarrier)
			m_input.cdBoxTopFlags|=NavSceneNode::EFlag_NotCarrier;

		m_input.cdBoxFlag		= m_input.cdBoxTopFlags;

		//--����xzƽ��jump����
		m_input.xzDir.y=0;
		Vec3Normalize(m_input.xzDir);

		//--��ʼ��m_output
		ZeroMemory(&m_output,sizeof(m_output));
		m_output.dwCarrierObjID=-1;

		//--
		m_pCollider->Init();

		//--����״̬
		m_state					= EState_Initialized;
	}


	ENavResult NavCollider_Jump::Update( NavCollider* pCollider,float startTime,float deltaTime,BitMap* pSpecCangoMap,Vector3& outPos,float& endTime,DWORD& dwCarrierObjID,float& cdCarrierTime,bool bOnWaterCheck/*=false*/ )
	{
		switch(m_state)
		{
		case EState_Initialized:
			{
				//--��ʼ����Ծ������
				m_pJumpCtrl->Init(startTime);

				//--��ײ
				Collider::tagCDResult cdResult;
				bool bCDOk=m_pCollider->Collide(pCollider,startTime,deltaTime,pSpecCangoMap,bOnWaterCheck,cdResult);
				if(bCDOk)
				{
					m_output.result=cdResult.result;
					if(cdResult.result==ENR_WillCarry)
					{
						m_output.dwCarrierObjID=cdResult.carry.dwCarrierObjID;
					}
					if(cdResult.result==ENR_WillVDrop)
					{
						if(cdResult.vdrop.vdropTime>0.0f
							||m_input.xzSpeed>0.0f
							||m_input.ySpeed>0.0f)//xzspeed>0��yspeed>0��ʾ��vdrop����vdrop
						{
							m_output.dwCarrierObjID=cdResult.vdrop.dwCarrierObjID;
							m_output.cdCarrierTime=cdResult.vdrop.cdCarrierTime;
						}
						else
						{
							//vdropTimeΪ0��ʾ�޷��������������vdrop���ܻ����vdrop����ѭ��
							m_state=EState_Finished;

							if(cdResult.vdrop.dwCarrierObjID!=-1)
							{
								m_output.result=ENR_WillCarry;
								m_output.outPos=m_input.startPos;
								m_output.endTime=startTime;
								m_output.dwCarrierObjID=cdResult.vdrop.dwCarrierObjID;

								outPos=m_output.outPos;
								endTime=m_output.endTime;
								dwCarrierObjID=m_output.dwCarrierObjID;
								return m_output.result; 
							}
							else
							{
								m_output.result=ENR_Landed;
								m_output.outPos=m_input.startPos;
								m_output.endTime=startTime;

								outPos=m_output.outPos;
								endTime=m_output.endTime;
								return m_output.result; 
							}
						}
					}
				}

				//--��ʼ��Ծ
				float jumpTime=0.0f;
				bool bJumpOk=m_pJumpCtrl->JumpStart(pCollider,cdResult,startTime,deltaTime,jumpTime,m_output.endTime,m_output.outPos);

				//--����״̬
				if(bCDOk&&bJumpOk)
				{
					m_state=EState_Finished;
					outPos=m_output.outPos;
					endTime=m_output.endTime;
					dwCarrierObjID=m_output.dwCarrierObjID;
					cdCarrierTime=m_output.cdCarrierTime;
					return m_output.result;
				}
				else if(bCDOk&&!bJumpOk)
				{
					m_state=EState_WaitForFinish;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
				else
				{
					m_state=EState_Jumpping;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
			}
			break;
		case EState_Jumpping:
			{
				//--��Ծ����
				float jumpTime=0.0f;
				bool bJumpOk=m_pJumpCtrl->JumpUpdate(pCollider,startTime,deltaTime,jumpTime,m_output.endTime,m_output.outPos);
				if(!bJumpOk)
				{
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}

				startTime+=jumpTime;
				deltaTime-=jumpTime;

				//--��ײ
				Collider::tagCDResult cdResult;
				bool bCDOk=m_pCollider->Collide(pCollider,startTime,deltaTime,pSpecCangoMap,bOnWaterCheck,cdResult);
				if(bCDOk)
				{
					m_output.result=cdResult.result;
					if(cdResult.result==ENR_WillCarry)
					{
						m_output.dwCarrierObjID=cdResult.carry.dwCarrierObjID;
					}
					if(cdResult.result==ENR_WillVDrop)
					{
						m_output.dwCarrierObjID=cdResult.vdrop.dwCarrierObjID;
						m_output.cdCarrierTime=cdResult.vdrop.cdCarrierTime;
					}
				}

				//--��ʼ��Ծ
				bJumpOk=m_pJumpCtrl->JumpStart(pCollider,cdResult,startTime,deltaTime,jumpTime,m_output.endTime,m_output.outPos);

				//--����״̬
				if(bCDOk&&bJumpOk)
				{
					m_state=EState_Finished;
					outPos=m_output.outPos;
					endTime=m_output.endTime;
					dwCarrierObjID=m_output.dwCarrierObjID;
					cdCarrierTime=m_output.cdCarrierTime;
					return m_output.result;
				}
				else if(bCDOk&&!bJumpOk)
				{
					m_state=EState_WaitForFinish;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
				else
				{
					m_state=EState_Jumpping;
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}
			}
			break;
		case EState_WaitForFinish:
			{
				//--��Ծ����
				float jumpTime=0.0f;
				bool bJumpOk=m_pJumpCtrl->JumpUpdate(pCollider,startTime,deltaTime,jumpTime,m_output.endTime,m_output.outPos);
				if(!bJumpOk)
				{
					outPos=m_output.outPos;
					return ENR_ToBeContinued;
				}

				//--����״̬
				m_state=EState_Finished;
				outPos=m_output.outPos;
				endTime=m_output.endTime;
				dwCarrierObjID=m_output.dwCarrierObjID;
				cdCarrierTime=m_output.cdCarrierTime;
				return m_output.result;
			}
			break;
		case EState_Finished:
			{
				outPos=m_output.outPos;
				endTime=m_output.endTime;
				dwCarrierObjID=m_output.dwCarrierObjID;
				cdCarrierTime=m_output.cdCarrierTime;
				return m_output.result;
			}
			break;
		}

		ASSERT(0);
		return ENR_Failed;
	}

}//namespace WorldBase