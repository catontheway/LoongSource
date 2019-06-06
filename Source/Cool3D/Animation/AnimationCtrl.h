#pragma once
#include "..\Util\RTTIObj.h"
#include "..\Cool3D.h"
#include "..\ResSys\AniMsgTable.h"
#include "..\Public\MathBaseDefine.h"
#include "..\Math\Transform.h"

namespace Cool3D
{
	class ResBase;
	class ResSkeleton;
	class ResKeyFrame;
	class AnimationState;
	class NodeAnimationTrack;
	class VertexAnimationTrack;
	class VertStream;
	class NodeAnimationMixer;
	class VertexAnimationMixer;

	enum BodyPartType
	{
		BPT_All		= 0,	// ȫ��
		BPT_Upper,			// ����
		BPT_Lower,			// ����
		BPT_Head,			// ͷ��
		BPT_UpperOrigin,	// ������ڵ�
		BPT_HeadOrigin		// ͷ�����ڵ�
	};

	struct Cool3D_API tagTagNodeSpace
	{
		DWORD		name;
		Transform	trans;

		void LerpTrans(const Vector3& pos1,const Quaternion& rotat1,
			const Vector3& pos2,const Quaternion& rotat2,float r,bool bMirrored)
		{
			Vector3 pos;
			Quaternion rotat;

			D3DXVec3Lerp(&pos,&pos1,&pos2,r);
			D3DXQuaternionSlerp(&rotat,&rotat1,&rotat2,r);

			trans.LoadIdentity();
			trans.RotateQuat(rotat);
			trans.Translate(pos);

			if(bMirrored)
				trans.Scale(-1,-1,-1);
		}
	};	

	/**	\class AnimationCtrl
		\brief ��������������
		\remarks
			�ṩһЩ�����������Ĺ���
	*/
	class Cool3D_API AnimationCtrl : public RTTIObj
	{
	public:
		AnimationCtrl(void);
		virtual ~AnimationCtrl(void);

		virtual void		BindAnimationRes(ResBase *pRes) = 0;
		virtual const ResBase*	GetRes(void) = 0;
		/**	��һ������
			\param timePos��������ʼ���ŵ�ʱ��㣬-1Ϊ�����һ֡��ʼ����
		*/
		virtual bool		OpenTrack(const TCHAR *szName,bool loop,float timePos=0.0f,float fadeinTime=0.0f,BodyPartType partType=BPT_All,bool isBlending=false,float weight=1.0f) = 0;
		virtual void		CloseTrack(const TCHAR *szName) = 0;
		virtual void		ClearTrack(void) = 0;
		virtual const AniMsgTable* GetAniMsgTable(void) const = 0;
		virtual void		UpdateRes(void) = 0;
		virtual void		Update(void) = 0;
		virtual void		ApplyTime(float offset) = 0;
		virtual void		GetTrackSet(vector<tstring>& trackNames) = 0;
		virtual DWORD		PeekFrameMsg(void) = 0;

		bool				IsTrackEnd(const TCHAR *szName) const;
		/** ���ĳ�������ĵ�ǰ����ʱ��
			\ret �������ֵ<0.0f����ʾ����������
		*/
		float				GetTrackPosition(const TCHAR *szName) const;
		void				SetTimeFactor(float factor) { m_timeFactor=factor; }
		void				SetEnable(const TCHAR *szName,bool enabled);
		bool				IsFadeinTime(const TCHAR *szName) const;

		typedef std::map<DWORD,AnimationState*> AnimationStateList;

	protected:
		struct tagCmd 
		{
			DWORD			name;
			bool			loop;
			float			timePos;
			float			fadeinTime;
			BodyPartType	partType;
			bool			isBlending;
			float			weight;
		};
		virtual void		ExecuteCmd(const tagCmd& cmd) = 0;
		virtual bool		TrackIsReady(DWORD dwTrackName) = 0;

	protected:
		float				m_timeFactor;		//ʱ������
		AnimationStateList	m_stateList;
		float				m_lastUpdateTime;

		typedef std::list<tagCmd> CacheCmdList;
		CacheCmdList		m_cacheCmd;


		DECL_RTTI(AnimationCtrl);
	};

	/**	\class NodeAnimationCtrl
		\brief �ڵ㶯��������
		\remarks
			���ƹ�����������AvatarNode�е��ô�����ж�������
	*/
	class Cool3D_API NodeAnimationCtrl : public AnimationCtrl
	{
		friend class NodeAnimationMixer;

	public:
		NodeAnimationCtrl(void);
		virtual ~NodeAnimationCtrl(void);

		//--AnimationCtrl
		virtual void	BindAnimationRes(ResBase *pRes);
		virtual const ResBase*	GetRes(void) { return (ResBase*)m_pRes; }
		virtual bool	OpenTrack(const TCHAR *szName,bool loop,float timePos=0.0f,float fadeinTime=0.0f,BodyPartType partType=BPT_All,bool isBlending=false,float weight=1.0f);
		virtual void	CloseTrack(const TCHAR *szName);
		virtual void	ClearTrack(void);
		virtual const AniMsgTable* GetAniMsgTable(void) const;
		virtual void	UpdateRes(void);
		virtual void	Update(void);
		virtual void	ApplyTime(float offset);
		virtual void	GetTrackSet(vector<tstring>& trackNames);
		virtual DWORD	PeekFrameMsg(void);

		bool			CanGetBoneSpace(void)	const;
		void			GetAllBonePos(vector<Vector3>& bonesPos) const;
		void			GetAllBoneMat(vector<Matrix4>& bonesMat) const;
		const Matrix4*	GetBoneSpace(UINT boneID) const;
		const Matrix4*	GetBoneSpace(const TCHAR* szBoneName) const;
		bool			BoneIsMirrored(UINT boneID) const;
		void			DrawSkeleton(void) const;
		void			RotateUpper(float yaw,float pitch,float roll);
		void			RotateUpperYaw(float yaw)	{ m_upperYaw = yaw; }
		void			RotateHead(float yaw,float pitch,float roll);
		void			RotateHeadYaw(float yaw)	{ m_headYaw = yaw; }
		void			RotateHeadPitch(float pitch){ m_headPitch = pitch; }
		float			GetUpperYaw(void)	const	{ return m_upperYaw; }
		float			GetUpperPitch(void) const	{ return m_upperPitch; }
		float			GetUpperRoll(void)	const	{ return m_upperRoll; }
		float			GetHeadYaw(void)	const	{ return m_headYaw; }
		float			GetHeadPitch(void)	const	{ return m_headPitch; }
		float			GetHeadRoll(void)	const	{ return m_headRoll; }
		void			SyncTrack(const TCHAR *szName);
		bool			IsBoneType(UINT boneID,BodyPartType partType) const;

		typedef std::map<DWORD, NodeAnimationTrack*> NodeTrackList;
		typedef std::vector<DWORD> BoneTypeList;
		typedef std::set<DWORD> ActiveTrackList;

	protected:
		virtual void	ExecuteCmd(const tagCmd& cmd);
		virtual bool	TrackIsReady(DWORD dwTrackName);

	private:
		void			GetBoneChildren(UINT boneID,vector<UINT>& children) const;

	protected:
		ResSkeleton			*m_pRes;
		bool				m_resIsReady;
		NodeTrackList		m_nodeTrackList;
		BoneTypeList		m_boneTypeList;

	private:
		ActiveTrackList		m_activeTracks;
		float				m_upperYaw;
		float				m_upperPitch;
		float				m_upperRoll;
		float				m_headYaw;
		float				m_headPitch;
		float				m_headRoll;

		NodeAnimationMixer	*m_pMixer;


		DECL_RTTI(NodeAnimationCtrl);
	};

	/**	\class VertexAnimationCtrl
		\brief ���㶯��������
		\remarks
			���ƶ�����ζ�������EntityNode�е��ô�����ж�������
			�ؼ�֡����Ӧ�ò���Ҫͬʱ���ڶ���Track�����Ľ�
	*/
	class Cool3D_API VertexAnimationCtrl : public AnimationCtrl
	{
	public:
		VertexAnimationCtrl(void);
		virtual ~VertexAnimationCtrl(void);

		//-AnimationCtrl
		virtual void		BindAnimationRes(ResBase *pRes);
		virtual const ResBase*	GetRes(void) { return (ResBase*)m_pRes; }
		virtual bool		OpenTrack(const TCHAR *szName,bool loop,float timePos=0.0f,float fadeinTime=0.0f,BodyPartType partType=BPT_All,bool isBlending=false,float weight=1.0f);
		virtual void		CloseTrack(const TCHAR *szName);
		virtual void		ClearTrack(void);
		virtual const AniMsgTable* GetAniMsgTable(void) const;
		virtual void		UpdateRes(void);
		virtual void		Update(void);
		virtual void		ApplyTime(float offset);
		virtual void		GetTrackSet(vector<tstring>& trackNames);
		virtual DWORD		PeekFrameMsg(void);

		VertStream*			GetFrameVertStream(void) const;
		bool				GetTagSpace(const TCHAR* szTagName,Matrix4& out);
		const vector<tagTagNodeSpace>& GetTagNodes() const { return m_tagNodes; }

		typedef std::map<DWORD, VertexAnimationTrack*> VertexTrackList;

	protected:
		virtual void		ExecuteCmd(const tagCmd& cmd);
		virtual bool		TrackIsReady(DWORD dwTrackName);

	protected:
		ResKeyFrame				*m_pRes;
		bool					m_resIsReady;
		VertexTrackList			m_vertexTrackList;

	private:
		bool					m_hasActiveTrack;
		DWORD					m_activeTrack;
		vector<tagTagNodeSpace>	m_tagNodes;			//�洢��ǰ��ֵ������tag node


		DECL_RTTI(VertexAnimationCtrl);
	};
}//namespace Cool3D