#pragma once

namespace Cool3D
{
	class SceneNode;
	/**	\class ISGNodeVisableCallBack
		\brief SceneGraph::Render()�����ҵ�һ��visable scene nodeʱ����callback
		\see SceneGraph,SceneNode
	*/
	class ISGNodeVisableCallBack
	{
	public:
		ISGNodeVisableCallBack(void)	{}
		virtual ~ISGNodeVisableCallBack(void)	{}

		virtual void Begin() =0;
		virtual void CallBack(SceneNode *pVisNode) =0;
		virtual void End() =0;
	};

	/**	\class SGNodeVisableSet
		\brief ��SceneGraph::Render()���������Ǽ�¼һ���ɼ���
	*/
	class SGNodeVisableSet : public ISGNodeVisableCallBack
	{
	public:
		SGNodeVisableSet();
		virtual ~SGNodeVisableSet();

		virtual void Begin();
		virtual void CallBack(SceneNode *pVisNode);
		virtual void End();

		/**	����һ��SceneNode�Ƿ��ڿɼ�����*/
		bool IsNodeVisable(SceneNode *pNode) const;

	private:
		class Member;
		Member *m_p;
	};
}//namespace Cool3D