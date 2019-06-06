#pragma once

class IFS;
namespace WorldBase
{
	class NavSGQuadTreeNode;
	class NavQuadTreeSG;
	/**	\class NavQuadTreeData
		\brief ����QuadTree Scene Graph�Ĵ�������,������ʱloadĳ��node
	*/
	class NavQuadTreeData
	{
	protected:
		struct tagItem
		{
			int		id;	//QuadTreeNode��id
			DWORD	offset;//��Node���ļ��е�offset
		};
		struct tagHeader
		{
			BYTE	magic[4];
			BYTE	ver;
			DWORD	numItem;
			DWORD	itemOffset;
		};
	public:
		NavQuadTreeData(void);
		virtual ~NavQuadTreeData(void);

		void OpenFromFile(IFS* pFS,const TCHAR* szFileName);
		NavSGQuadTreeNode*	LoadNode(int id,NavQuadTreeSG* pSG);
		void Close();
	protected:
		FArchiveAdv			m_ar;
		map<int,DWORD>		m_itemMap;//id,offset
	};
}//namespace WorldBase