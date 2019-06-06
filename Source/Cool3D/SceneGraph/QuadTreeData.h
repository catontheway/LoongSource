#pragma once
#include "..\Util\FArchiveAdv.h"

class IFS;
namespace Cool3D
{
	class SGQuadTreeNode;
	/**	\class QuadTreeData
		\brief ����QuadTree Scene Graph�Ĵ�������,������ʱloadĳ��node
	*/
	class QuadTreeData
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
			int		numNode;
			DWORD	itemOffset;
		};
	public:
		QuadTreeData(void);
		virtual ~QuadTreeData(void);

		void OpenFromFile(IFS* pFS,const TCHAR* szFileName);
		SGQuadTreeNode*	LoadNode(int id);
		void Close();

		int GetNumNode(){ return m_numNode;}
	protected:
		FArchiveAdv			m_ar;
		map<int,DWORD>		m_itemMap;//id,offset
		int					m_numNode;
	};
}//namespace Cool3D