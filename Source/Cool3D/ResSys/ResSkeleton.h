#pragma once
#include "..\Cool3D.h"
#include "Resrenderable.h"
#include "..\Util\FArchive.h"
#include "..\Math\Math.h"
#include "..\VFS\IFS.h"
#define VFS_READ
#include "..\Public\SkinBoneDefine.h"
#include "AniMsgTable.h"
#include "ActTimeTable.h"
#include "..\Util\FileName.h"
#include "..\Animation\KeyFrame.h"

namespace Cool3D
{
	class ResSkeletonAction;

	const DWORD SKELETONUNKNOWN		= 0x00000000;
	const DWORD SKELETONUPPERPART	= 0x00000001;
	const DWORD SKELETONLOWERPART	= 0x00000010;
	const DWORD SKELETONHEADPART	= 0x00000100;
	const DWORD SKELETONCOMMONROOT	= 0x00001000;
	const DWORD SKELETONHEADORIGIN	= 0x00010000;
	const DWORD SKELETONUPPERORIGIN = 0x00100000;
	/**	\class ResSkeleton 
		\brief ����������Դ�ӿ�
		\remarks
			\par ֻ�ṩ�ӿڣ������������ݱ�����ResSkeletonAction��
			\par ����Track�ļ��غ��ͷ�
			\par ����Track���뵥�����ļ���
			\par �ṩ������Ϣ���Trackʱ�䳤�ȱ�
		\see ResSkeletonAction
	*/
	class Cool3D_API ResSkeleton :	public ResRenderable
	{
		struct TrackInfo{
			int nBegin;
			int nEnd;
			int nBaseFrm;//����֡��ת��
			ResSkeletonAction* pRes;//���ܱ��ͷ�
			float lastOpenTime;//�����ͷ�Res
		};
		
	public:
		ResSkeleton(const TCHAR* szName);
		virtual ~ResSkeleton(void);

		//--ResBase
		virtual UINT CreateContent(DWORD param);

		//--ResRenderable
		virtual UINT GetNumRenderNode() const			{	return 0;}
		virtual RenderNode *GetRenderNode(UINT i) const	{	return NULL;}
		virtual const AABBox	*GetAABBox() const		{	return NULL;}
		virtual int GetNumMtl() const					{	return 0;}
		virtual void CreateMtlArray(vector<IMaterial*>&	mtls,const TCHAR* szLightMap,float mtlDiffuseFactor=1.0f) const	{}

		//--
		UINT GetNumFrame() const { return m_numFrame;}
		SkeletonKeyFrame* GetFirstFrame(void);
		SkeletonKeyFrame* GetFrame(UINT index);
		UINT GetNumBone() const;
		DWORD GetPartInfoOfBone(const tstring& boneName) const;
		/**	����Bone��name����bone��id(�������е�index)
			\return -1Ϊû�ҵ�
		*/
		int GetBoneID(const TCHAR* szBoneName);
		const SkeletonStruct& GetStruct() const;
		const AniMsgTable* GetMsgTable(){ return &m_msgTable;}

		float GetTrackTime( DWORD dwTrackName );
		float GetTrackTimeFromRes( DWORD dwTrackName );
		bool  GetTrackRange( DWORD dwTrackName, int& nBegin, int& nEnd ); 
		float GetFrmTime( int nFrame );

		int GetNumTrack() {	return m_mapTrackInfo.size(); }
		int GetTrackName( vector<tstring>& trackNames ) const;

		//!	�����Ƿ����µ�track��load������
		bool PrepareTrack(DWORD dwTrackName);
		//! �����Դ�������
		void UpdateTrackRes();
		//! ����ָ��Track�Ƿ�������
		bool IsTrackLoaded(DWORD dwTrackName);
		//! ����ָ��Track�Ƿ��ͷ�
		bool IsTrackRelease(DWORD dwTrackName);

		//! ����Track��lastOpenTime
		void UpdateTrackLastOpenTime(DWORD dwTrackName);

		//! Editor support
		void ReloadMsgTable();

	private:
		typedef struct _BonePropties
		{
			DWORD	dwBodyPart;
		} BonePropties;

		void AppendTracks(ResSkeletonAction* pRes);
		void ReleaseTracks();
		Filename MakeTrackFileName(DWORD dwTrackName);
		/** \���ļ��м��ع��������ڵ�����
		*/
		void LoadBonesPropties(IFS *pFS, const TCHAR* szFileName, map<tstring, BonePropties>& bonesPropties);

		map<DWORD,TrackInfo*>	m_mapTrackInfo;	//Track��Ϣ
		AniMsgTable				m_msgTable;
		ActTimeTable			m_actTimeTable;
		UINT					m_numFrame;

		map<tstring, BonePropties>	m_bonesPropties;

		ResSkeletonAction*		m_pFirstRes;//�����˹������ݵ�Res
		map<DWORD,ResSkeletonAction*> m_asyncList;//�첽Res

		DECL_RTTI(ResSkeleton);
	};
}//namespace Cool3D