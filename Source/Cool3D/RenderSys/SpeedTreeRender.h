#pragma once
#include "RenderSys.h"

namespace F3D
{
	/**	\class SpeedTreeRender
		\brief ���speed treeϵͳ����Ⱦ�ڵ���������Ż�
	*/
	class SpeedTreeRender
	{
	public:
		SpeedTreeRender(void);
		virtual ~SpeedTreeRender(void);

		void Clear();
		void AttachTree(const RenderItem& item);									 
		void RenderAllTrees(SceneGraph *pSG);

		void SetWindStrength(float str)	{	m_fWindStrength=str;}
		
		static void Init();
		static bool s_bWindEnable;
	protected:
		typedef vector<RenderItem> TreeType;
		vector<TreeType>	m_trees;
		float				m_fWindStrength;

	protected:
		void UpdateWind();
		void RenderTreeType(TreeType& treeType);
	};
}//namespace F3D