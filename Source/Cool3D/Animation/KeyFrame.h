#pragma once
#include "..\Util\RTTIObj.h"
#include "..\Public\SkinBoneDefine.h"

namespace Cool3D
{
	class VertStream;

	/**	\class KeyFrame
		\brief �ؼ�֡���࣬һ��Track����һ��ؼ�֡��ɵ�
	*/
	class KeyFrame : public RTTIObj
	{
	public:
		KeyFrame(void);
		KeyFrame(float time);
		virtual ~KeyFrame(void) {}

	public:
		float m_time;


		DECL_RTTI(KeyFrame);
	};

	/**	\class SkeletonKeyFrame
		\brief ���������ؼ�֡
	*/
	class SkeletonKeyFrame : public KeyFrame
	{
	public:
		SkeletonKeyFrame(void);
		SkeletonKeyFrame(const SkeletonKeyFrame& rhs);
		virtual ~SkeletonKeyFrame(void) {}

		const SkeletonKeyFrame& operator=(const SkeletonKeyFrame& rhs);

		UINT GetNumBone(void) const	{ return m_boneArray.size(); }

	private:
		void Copy(const SkeletonKeyFrame& rhs);

	public:
		DWORD			m_name;
		vector<Bone>	m_boneArray;
		TagNodeFrame	m_tagNodes;


		DECL_RTTI(SkeletonKeyFrame);
	};

	/**	\class VertexKeyFrame
		\brief ���㶯���ؼ�֡������ֻ֧��Morph
	*/
	class VertexKeyFrame : public KeyFrame
	{
	public:
		VertexKeyFrame(void);
		virtual ~VertexKeyFrame(void);

	public:
		DWORD			m_name;
		int				m_index;
		VertStream		*m_pStream;


		DECL_RTTI(VertexKeyFrame);
	};
}//namespace Cool3D