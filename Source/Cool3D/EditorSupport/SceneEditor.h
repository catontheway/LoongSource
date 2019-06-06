#pragma once
#include "..\Cool3D.h"

namespace Cool3D
{
	class SceneNode;
	/**	\class SceneEditor
		\brief �����༭֧��,��������SceneGraph
	*/
	class Cool3D_API SceneEditor
	{
	public:
		SceneEditor(void);
		virtual ~SceneEditor(void);

		void Update(float deltaTime);
		void Render();

		SceneNode* AddStaticNode(const TCHAR* szResName);
		void RemoveStaticNode(SceneNode *pNode);
		SceneNode* PickNode(POINT cursorPt);

	private:
		class Member;
		Member	*m_p;
	};
}//namespace Cool3D