#pragma once

namespace WorldBase
{
	class NavMapEditorUtil
	{
	public:
		NavMapEditorUtil(void){}
		~NavMapEditorUtil(void){}

		/** ��src�Ĳ�����������Ӧ�õ�target
		*/
		static void MergeCangoMap(BitMap* pSrc,BitMap* pTarget);

	};

}//namespace WorldBase