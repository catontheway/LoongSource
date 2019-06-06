#pragma once
#include "EntityNode.h"
#include "..\Cool3D.h"

namespace Cool3D
{
	/**	\class SGAvatarAttachment 
		\brief ��һ��ģ�͹��ص�ĳ��bone��
	*/
	class Cool3D_API SGAvatarAttachment :	public EntityNode
	{
	public:
		SGAvatarAttachment(void);
		virtual ~SGAvatarAttachment(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
		//--SceneNode
		virtual void Update(float deltaTime,SceneGraph *pSG);
		
		//--
		/**	�ѱ������������Tag node(szTagName)��
		    ��������������szTagNameͬ����Tag node���򸽼Ӵ�Tag node��ƫ��
		*/
		void BindToTagNode(const TCHAR* szTagName);

		bool GetModelSpace(const TCHAR* szTagName,Matrix4& out);
		bool HaveTagNode(const TCHAR* szTagName) const;
		// ��ù��ص�����
		const tstring GetTagNodeName() const;
		// �󶨵Ĺ����Ƿ���
		bool IsBindMirroredBone() const;
	private:
		bool GetSrcTagNodeOffset( Vector3* pOut );
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(SGAvatarAttachment);
	};
}//namespace Cool3D