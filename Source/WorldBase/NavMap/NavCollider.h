#pragma once
#include "NavSurface.h"

namespace WorldBase
{
	class NavQuadTreeSG;
	class NavResHeightmap;
	class NavColliderCache;
	class HalfByteMap;
	class NavSceneNode;
	class NavCarrierNode;
	class NavResBitMap;

	/** \class NavCollider
		\brief ��ײ�����
	*/
	class WORLD_BASE_API NavCollider
	{
	public:
		enum EReturn			//��������ֵ����
		{
			ERet_None	= -1,	//
			ERet_Arrive,		//�����յ�
			ERet_Blocking,		//����ס
			ERet_WillDrop,		//������
			ERet_WillVDrop,		//����ֱ����
			ERet_WillSlide,		//������
			ERet_WillSwim,		//����Ӿ
			ERet_WillInWater,	//����ˮ
			ERet_SwimGround,	//��Ӿ��ǳ
			ERet_Standup,		//�����վ����λ��
			ERet_Infinite,		//����������ײ���״̬�������ϲ��߼�������
			ERet_TooLong,		//����̫Զ
			ERet_CannotJump,	//�޷�����
			ERet_ToBeContinued,	//���Լ����ƶ�
		};

		struct tagMovePoint		//�ƶ���
		{
			Vector3 pos;		//����
			float   dist;		//��������
		};

		struct tagStepMoveData	//�ֶ�move����
		{
			Vector3 xzDir;		
			float	xzTotalDist;
			float   xzCurDist;	
			float	curDist;	
			Vector3 curPos;		
		};

		struct tagStepSwimData	//�ֶ�swim����
		{
			Vector3 xzDir;		
			float	xzTotalDist;
			float   xzCurDist;	
			float	curDist;	
			Vector3 curPos;		
		};

	public:
		NavCollider(void);
		~NavCollider(void);

		//--��������-----------------------------------------------------------------------

		/** ��������
		*/
		void LoadFromFile(const TCHAR* szMapName,IFS* pFS,bool bLoadGroundType=false);
		/** �ͷ�����
		*/
		void Destroy();
		/** ���ÿɼ������С
		*/
		void SetViewZoneSize(const Vector3& size);
		/** ���¿ɼ�����
		*/
		void InvalidViewZone(const Vector3& zoneCenter);
		/** ��¡����
		*/
		void Clone(NavCollider* pTarget);

		//--ģ����ײ���-------------------------------------------------------------------

		/** AABB��ģ��AABB��ײ���
		*/
		bool AABBoxCollideBox(const AABBox&  box,float curTime,DWORD dwFlags=0,DWORD* pdwCarrierObjID=NULL);
		bool AABBoxCollideBoxAndTrn(const AABBox& box,float curTime,float groundH,DWORD dwFlags=0,DWORD* pdwCarrierObjID=NULL);
		bool AABBoxCollideBoxAndTrn(const AABBox& box,float curTime,DWORD dwFlags=0,DWORD* pdwCarrierObjID=NULL);
		/** AABB��ģ��AABB��ײ��⣬������ײλ��
		*/
		bool AABBoxCollideBoxTop(const AABBox& box,float curTime,float& intersectH,DWORD dwFlags=0,DWORD* pdwCarrierObjID=NULL);
		bool AABBoxCollideBoxTopAndTrn(const AABBox& box,float curTime,float groundH,float& intersectH,bool* pbOnGround=NULL,DWORD dwFlags=0,DWORD* pdwCarrierObjID=NULL);
		bool AABBoxCollideBoxTopAndTrn(const AABBox& box,float curTime,float& intersectH,bool* pbOnGround=NULL,DWORD dwFlags=0,DWORD* pdwCarrierObjID=NULL);
		/** ������ģ��AABB��ײ���
		*/
		bool RayCollideBox(const Ray& ray,Vector3& intersectPos);
		bool RayCollideBoxAndTrn(const Ray& ray,Vector3& intersectPos);
		bool RayCollideBox(const Ray& ray);
		bool RayCollideBoxAndTrn(const Ray& ray,DWORD dwFlags=0);

		//--������ײ���--------------------------------------------------------------------

		/** �����ж�xzƽ���ָ�������Ƿ���ģ��
		*/
		bool HaveModel(float x,float z);
		bool HaveModel(float x1,float z1,float x2,float z2);
		/** ȡ�õر�߶ȣ�xz����������Զ����нض�
		*/
		float SafeGetGroundHeight(float x,float z);
		float SafeGetGroundHeight(float x1,float z1,float x2,float z2);
		/** ȡ�õر�߶ȣ������ڸ��ӵĸ߶Ƚ��в�ֵ��xz����������Զ����нض�
		*/
		float SafeGetGroundHeightSlerp(float x,float z);
		float SafeGetGroundHeightSlerp(float x1,float z1,float x2,float z2);
		/** �����������ײ���
		*/
		bool RayCollideTrn(const Ray& ray,Vector3& intersectPos);
		/** �ж��Ƿ�Ử��
		*/
		bool IfWillSlide(float x,float z);
		bool IfWillSlide(const Vector3& pos,const Vector3& roleSize);
		/** ���㻬�䷽��xz����������Զ����нض�
		*/
		bool SafeCalcSlideDir(float x,float z,Vector3& out);

		//--ˮ����ײ���--------------------------------------------------------------------

		/** �ж�ָ��λ���Ƿ�������Ӿ״̬
		*/
		bool IfWillSwim(const Vector3& curPos,float roleHeight);
		bool IfWillSwim(const Vector3& curPos,float roleHeight,float& footH);
		/** �ж�ָ��λ���Ƿ���ˮ��
		*/
		bool IfWillOnWater(const Vector3& curPos);
		bool IfWillOnWater(const Vector3& curPos,float& waterH);
		/** ������Ӿʱ���ŵ׵ĸ߶�
		*/
		float CalcSwimFootHeight(const Vector3& curPos,float roleHeight,float defaultHeight);
		/** ���ˮ��߶�
		*/
		bool  GetWaterHeight(const Vector3& curPos,float& out);
		float GetWaterHeightEx(const Vector3& curPos,float defaultHeight);
		/** ������ˮ����ײ���
		*/
		bool RayCollideWater(const Ray& ray,Vector3& intersectPos);

		//--����������--------------------------------------------------------------------
		inline bool IfCanGo(float x,float z,BitMap* pSpecCangoMap=NULL);

		//--�߼�����------------------------------------------------------------------------

		/** �ж��Ƿ�ᴹֱ����
		*/
		bool IfWillVDrop(const Vector3& curPos,const Vector3& roleSize);

		/** �ƶ���ײ���
		*/
		EReturn Move(const Vector3& start,const Vector3& end,const Vector3& roleSize,int maxCollideTimes,list<tagMovePoint>& path,bool bPassCheck=true,DWORD dwFlags=0,float fMoveFall=100.0f,bool bInWaterCheck=false);
		/** �ֶ��ƶ���ʼ��
		*/
		void StepMoveInit(const Vector3& start,const Vector3& end,tagStepMoveData& data);
		/** �ֶ��ƶ�
		*/
		EReturn StepMove(const Vector3& start,const Vector3& end,const Vector3& roleSize,float maxMoveDist,tagStepMoveData& data,list<tagMovePoint>& path,bool bPassCheck=true,bool bInWaterCheck=false);
		/** ������ײ���
		*/
		EReturn Drop(const Vector3& start,const Vector3& dir,float xzSpeed,const Vector3& roleSize,float& totalTime,bool bInWaterCheck=false);
		/** ��ֱ������ײ���
		*/
		EReturn VDrop(const Vector3& start,const Vector3& roleSize,float& totalTime,bool bOnlyTrn=false,bool bInWaterCheck=false);
		/** ��Ծ��ײ���
		*/
		EReturn Jump(const Vector3& start,const Vector3& dir,float xzSpeed,float ySpeed,const Vector3& roleSize,float& totalTime,bool bOnlyTrn=false,BitMap* pSpecCangoMap=NULL,bool bInWaterCheck=false);
		/** ������ײ���
		*/
		EReturn Slide(const Vector3& start,const Vector3& roleSize,list<tagMovePoint>& path,bool bInWaterCheck=false);
		/** NPC�ƶ���ײ���
		*/
		bool NPCMove(const Vector3& start,const Vector3& end,const Vector3& roleSize,int maxCollideTimes,list<tagMovePoint>& path,bool bHeightCheck=false,bool bPassCheck=false);
		/** NPC�ֶ��ƶ���ʼ��
		*/
		void NPCStepMoveInit(const Vector3& start,const Vector3& end,tagStepMoveData& data);
		/** NPC�ֶ��ƶ�
		*/
		EReturn NPCStepMove(const Vector3& start,const Vector3& end,const Vector3& roleSize,float maxMoveDist,tagStepMoveData& data,list<tagMovePoint>& path,bool bHeightCheck=false,bool bPassCheck=false);
		/** ��Ӿǰ��
		*/
		EReturn Swim(const Vector3& start,const Vector3& end,const Vector3& roleSize,int maxCollideTimes,list<tagMovePoint>& path,bool bPassCheck=true,bool bWaterWalk=false);
		/** �ֶ���Ӿ��ʼ��
		*/
		void StepSwimInit(const Vector3& start,const Vector3& end,tagStepSwimData& data);
		/** �ֶ���Ӿ
		*/
		EReturn StepSwim(const Vector3& start,const Vector3& end,const Vector3& roleSize,float maxMoveDist,tagStepSwimData& data,list<tagMovePoint>& path,bool bPassCheck=true,bool bWaterWalk=false);

		//--��������---------------------------------------------------------------------------

		/** �����ƶ�·���Ͳ�ֵ���Ӽ��㵱ǰλ��
		*/
		void CalcMovePos(const Vector3& start,list<tagMovePoint>& path,float factor,Vector3& out);
		/** ����ʱ��������λ��
		*/
		void CalcDropPos(const Vector3& start,const Vector3& dir,float xzSpeed,float curTime,float totalTime,Vector3& out);
		/** ����ʱ����㴹ֱ����λ��
		*/
		void CalcVDropPos(const Vector3& start,float curTime,float totalTime,Vector3& out);
		/** ����ʱ�������Ծλ��
		*/
		void CalcJumpPos(const Vector3& start,const Vector3& dir,float xzSpeed,float ySpeed,float curTime,float totalTime,Vector3& out);
		/** ���㻬���ֵ����
		*/
		float CalcSlideFactor(float totalDist,float curTime);

		//--�����-----------------------------------------------------------------------------
		
		/** ����/�ر������ŵ���ײ
		*/
		void EnableAllDoorsCollide(bool bEnable);
		/** ����/�ر�ָ���ŵ���ײ
		*/
		void EnableDoorCollide(DWORD dwObjID,bool bEnable);
		/** ����ŵ�SceneNode,δ�ҵ�����NULL
		*/
		NavSceneNode* FindDoor(DWORD dwObjID);

		//--�ؾ����---------------------------------------------------------------------------
		
		/** �����ؾ�ObjID���SceneNode
		*/
		NavCarrierNode* FindCarrier(DWORD dwObjID);

		//--��������---------------------------------------------------------------------------
		
		/** ����ָ��λ���Ƿ�֧���Զ�Ѱ·
		*/
		bool IfCanFindPath(const Vector3& pos,const Vector3& roleSize);
		/** ��ָ��λ�ô�ֱ������ײ��ѯ��������
		*/
		ESurfaceType QuerySurfaceType(const Vector3& pos,float curTime);
		/** �Ƿ��Ѽ���
		*/
		bool IsLoaded() const { return m_bLoaded; }
	private:
		NavQuadTreeSG*		m_pSG;
		NavResHeightmap*	m_pHMap;
		NavResBitMap*		m_pModelMap;
		NavResBitMap*		m_pCangoMap;
		NavResBitMap*		m_pWaterArea;
		NavResHeightmap*	m_pWaterHeight;
		HalfByteMap*		m_pGroundType;
		bool                m_bLoaded;
	};

}//namespace WorldBase