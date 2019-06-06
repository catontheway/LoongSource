#pragma once

namespace Cool3D
{
	/**	\class ISGEnumProc
		\brief ö��SceneGraph�����нڵ�Ľӿ�
		\see class SceneGraph;
	*/
	class ISGEnumProc
	{
	public:
		enum ERet
		{
			ERet_Continue	=0,		//����
			ERet_IgnoreChildren,	//���Ե�ǰ�ڵ�������ӽڵ�,Ȼ�����
			ERet_Abord,				//ֹͣ
		};
	public:
		ISGEnumProc(void)	{	}
		virtual ~ISGEnumProc(void)	{	}

		virtual ERet CallBack(SceneNode *pNode) =0;
	};
}//namespace Cool3D