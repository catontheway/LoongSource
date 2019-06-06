#include "StdAfx.h"
#include ".\quadtreedatabuilder.h"
#include "SGQuadTreeNode.h"
#include "..\Device\DrawX.h"
#include "..\Kernel.h"
#include "..\Util\FArchiveAdv.h"
#include "..\Public\FileUtil.h"

namespace Cool3D
{
	class QuadTreeNodeBuilder: public SGQuadTreeNode
	{
	public:
		QuadTreeNodeBuilder()
		{}
		virtual ~QuadTreeNodeBuilder()
		{
			m_pContent.clear();//�Լ���������Щscene node��������
		}

		static int	m_idHold;
		static int NexID()	{	return ++m_idHold;}

		static int m_depth;//���
		static int m_numTreeNodes;//�Ĳ��������
		static int m_maxSceneNodes;//�����Ĳ���������scenenode��

	protected:
		static void ComputeBoxForNodes(const vector<SceneNode *>& sceneNodes,AABBox& box)
		{
			box.max=Vector3(-FLOAT_MAX,-FLOAT_MAX,-FLOAT_MAX);
			box.min=Vector3(FLOAT_MAX,FLOAT_MAX,FLOAT_MAX);

			AABBox nodeBox;
			for(size_t i=0;i<sceneNodes.size();i++)
			{
				const SceneNode* pNode=sceneNodes[i];
				pNode->GetBox(nodeBox);
				box.Merge(nodeBox);
			}
		}

		void BuildChildBoxes(AABBox& sub0,AABBox& sub1,AABBox& sub2,AABBox& sub3)
		{
			/*��m_box��XZƽ���зֳ�4��Сbox

						--------|--------m_box.max
						|       |		|
						|   0   |	1	|
						|-------|-------|
						|       |		|	   /\Z
						|   3   |	2	|		|
			   m_box.min|_______|_______|		|-->x
			*/

			float xSize=m_box.max.x-m_box.min.x;
			float zSize=m_box.max.z-m_box.min.z;
			xSize*=0.5f;
			zSize*=0.5f;

			sub0.max.y=m_box.max.y;
			sub0.min.y=m_box.min.y;
			sub0.max.x=m_box.min.x+xSize;
			sub0.max.z=m_box.max.z;
			sub0.min.x=m_box.min.x;
			sub0.min.z=m_box.max.z-zSize;

			sub1.max.y=m_box.max.y;
			sub1.min.y=m_box.min.y;
			sub1.max=m_box.max;
			sub1.min.x=sub1.max.x-xSize;
			sub1.min.z=sub1.max.z-zSize;

			sub2.max.y=m_box.max.y;
			sub2.min.y=m_box.min.y;
			sub2.max.x=m_box.max.x;
			sub2.max.z=m_box.max.z-zSize;
			sub2.min.x=sub2.max.x-xSize;
			sub2.min.z=sub2.max.z-zSize;


			sub3.max.y=m_box.max.y;
			sub3.min.y=m_box.min.y;
			sub3.min=m_box.min;
			sub3.max.x=sub3.min.x+xSize;
			sub3.max.z=sub3.min.z+zSize;
		}

		bool IfNeedSplit(int numSceneNodes) const
		{
			return numSceneNodes>4 && m_lv<10;
		}

	public:
		void Build(vector<SceneNode *> sceneNodes,int myID,int myLV)
		{
			ASSERT(sceneNodes.size()>0);
			//--
			SGQuadTreeNode::m_id=myID;
			SGQuadTreeNode::m_lv=myLV;

			if(myLV>m_depth)
				m_depth=myLV;

			m_numTreeNodes++;
			
			//--�����������Щscene nodes�ĺ���
			ComputeBoxForNodes(sceneNodes,SGQuadTreeNode::m_box);

			//--
			if(IfNeedSplit((int)sceneNodes.size()))
			{
				//--�����е�scene nodes���տռ�ֳ��ķ�,�����children,ʣ��Ĵ���this
				AABBox subBox[4];
				BuildChildBoxes(subBox[0],subBox[1],subBox[2],subBox[3]);
				for(int k=0;k<4;k++)//ʹ�ú��ӱ���һЩ
				{
					Vector3 vSize=subBox[k].max-subBox[k].min;
					vSize*=0.25f;

					subBox[k].max+=vSize;
					subBox[k].min-=vSize;
				}

				vector<SceneNode*> subNodes[4];
				AABBox nodeBox;
				size_t i;
				for(i=0;i<sceneNodes.size();i++)
				{
					sceneNodes[i]->GetBox(nodeBox);
					for(int j=0;j<4;j++)
					{
						if(subBox[j].Contain(nodeBox))
						{
							subNodes[j].push_back(sceneNodes[i]);
							sceneNodes[i]=NULL;//��־��node�Ѿ���childȡ��
							break;
						}
					}//endof for(j)
				}//end of for each node
				
				//--�ݹ�Ĵ����ӽڵ�
				for(i=0;i<4;i++)
				{
					if(subNodes[i].size()>0)
					{
						QuadTreeNodeBuilder *pNewNode=new QuadTreeNodeBuilder;
						pNewNode->Build(subNodes[i],NexID(),m_lv+1);
						m_pChildren[i]=pNewNode;

						m_childrenID[i]=pNewNode->GetID();
						m_childrenBox[i]=pNewNode->GetBox();
					}
				}

				//--��ʣ��Ľڵ㿽����this
				for(i=0;i<sceneNodes.size();i++)
				{
					if(sceneNodes[i]!=NULL)
					{
						//ASSERT(0 && "not leaf tree ");////��ΪҪ����leaf tree,���Բ�Ӧ��ʣ��
						m_pContent.push_back(sceneNodes[i]);
					}
				}
			}
			else
			{
				m_pContent=sceneNodes;
			}

			if((int)m_pContent.size()>m_maxSceneNodes)
				m_maxSceneNodes=(int)m_pContent.size();

		}//endof Build()

		void GetChildrenR(vector<SGQuadTreeNode*>& out)
		{
			int i=0;
			for(i=0;i<4;i++)
			{
				if(m_pChildren[i])
					out.push_back(m_pChildren[i]);
			}

			for(i=0;i<4;i++)
			{
				QuadTreeNodeBuilder *pChild=(QuadTreeNodeBuilder*)m_pChildren[i];
				if(pChild)
					pChild->GetChildrenR(out);
			}
		}
	};
	int	QuadTreeNodeBuilder::m_idHold=5000;
	int QuadTreeNodeBuilder::m_depth=0;
	int QuadTreeNodeBuilder::m_numTreeNodes=0;
	int QuadTreeNodeBuilder::m_maxSceneNodes=0;

	QuadTreeDataBuilder::QuadTreeDataBuilder(void)
	{
		m_pRoot=NULL;
	}

	QuadTreeDataBuilder::~QuadTreeDataBuilder(void)
	{
	}

	void QuadTreeDataBuilder::Begin()
	{
		SAFE_DELETE(m_pRoot);
		m_allNode.clear();
	}

	void QuadTreeDataBuilder::AddNode(SceneNode *pNode)
	{
		m_allNode.push_back(pNode);
	}

	bool QuadTreeDataBuilder::End(const TCHAR* szSaveFile,int& baseNodeID)
	{
		if(m_allNode.size()<=0)
		{
			Kernel::Inst()->SetLastError(_T("������û���Ѿ�scene node,�޷�����QuadTree"));
			return false;
		}

		//--����ID
		for(int i=0;i<(int)m_allNode.size();i++)
		{
			SceneNode* pNode=m_allNode[i];
			pNode->SetStaticNodeID(baseNodeID+i+1);
		}
		baseNodeID+=(int)m_allNode.size();

		QuadTreeNodeBuilder::m_depth=0;
		QuadTreeNodeBuilder::m_maxSceneNodes=0;
		QuadTreeNodeBuilder::m_numTreeNodes=0;

		m_pRoot=new QuadTreeNodeBuilder;
		m_pRoot->Build(m_allNode,0,0);

		LogTreeInfo();

		bool ret=true;
		if(szSaveFile!=NULL)
			ret=SaveToFile(szSaveFile);
		//--
		delete m_pRoot;

		return ret;
	}

	
	bool QuadTreeDataBuilder::SaveToFile(const TCHAR* szFileName)
	{
		if(m_pRoot==NULL)
		{
			Kernel::Inst()->SetLastError(_T("QuadTreeDataBuilder::m_pRootΪNULL"));
			return false;
		}

		FArchiveAdv ar;
		try
		{
			ar.OpenForWrite(szFileName);
		}
		catch (...)
		{
			Kernel::Inst()->SetLastError(_T("QuadTreeDataBuilder::���ļ�ʧ��,�ļ���=%s."),szFileName);
			return false;
		}

		//--�õ�һ���ڵ�����
		vector<SGQuadTreeNode*>	allQuadTreeNodes;
		allQuadTreeNodes.push_back(m_pRoot);
		m_pRoot->GetChildrenR(allQuadTreeNodes);

		//--д���ļ�ͷ
		tagHeader header;
		memset(&header,0,sizeof(header));
		_tcscpy((TCHAR*)header.magic,_T("QTD"));
		header.ver=1;
		header.numNode=(int)m_allNode.size();
		header.numItem=allQuadTreeNodes.size();

		FILE *fp=ar.GetWriteFile();
		ASSERT(fp!=NULL);
		FWriteValue(fp,header);

		//--д��Item����
		header.numItem=allQuadTreeNodes.size();
		header.itemOffset=ftell(fp);
		vector<tagItem>	items;
		items.resize(header.numItem);
		size_t i;
		for(i=0;i<items.size();i++)
		{
			memset(&items[i],0,sizeof(tagItem));
			FWriteValue(fp,items[i]);
		}

		//--д��ڵ�����,���Ҽ�¼ÿ���ڵ��offset��item����
		for(i=0;i<allQuadTreeNodes.size();i++)
		{
			SGQuadTreeNode *pQTN=allQuadTreeNodes[i];
			tagItem& item=items[i];
			item.id=pQTN->GetID();
			item.offset=ftell(fp);
			pQTN->Serialize(ar);
		}

		//--��дItem����
		fseek(fp,header.itemOffset,SEEK_SET);
		for(i=0;i<items.size();i++)
			FWriteValue(fp,items[i]);
		
		//--��дheader
		fseek(fp,0,SEEK_SET);
		FWriteValue(fp,header);

		//--
		ar.Close();
		return true;
	}

	void QuadTreeDataBuilder::LogTreeInfo()
	{
		LOG(_T("--QuadTree Info-----------------------------------\n"));
		LOG(_T("depth:%d\n"),QuadTreeNodeBuilder::m_depth);
		LOG(_T("num tree nodes:%d\n"),QuadTreeNodeBuilder::m_numTreeNodes);
		LOG(_T("num scene nodes:%d\n"),m_allNode.size());
		LOG(_T("max scene nodes per tree node:%d\n"),QuadTreeNodeBuilder::m_maxSceneNodes);
		LOG(_T("avage scene nodes per tree node:%d\n"),m_allNode.size()/QuadTreeNodeBuilder::m_numTreeNodes);
		LOG(_T("--QuadTree Info-----------------------------------\n"));

	}
}//namespace Cool3D