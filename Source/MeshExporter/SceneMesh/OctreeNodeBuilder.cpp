#include "OctreeNodeBuilder.h"
using namespace Cool3D;

const int MAX_SIZE=120000;
list<AABBox> boxList[MAX_SIZE];
int	OctreeNodeBuilder::m_idHold=0;

OctreeNodeBuilder::OctreeNodeBuilder()
: m_boxSize(50.0f)
, m_xzEpsilon(20.0f)
, m_yEpsilon(5.0f)
{
}

OctreeNodeBuilder::~OctreeNodeBuilder()
{
	//
}

const AABBox& OctreeNodeBuilder::MergeNode(const OctreeBuilder::tagNode& node)
{
	if(node.box.max.x > m_box.max.x)
		m_box.max.x=node.box.max.x;
	if(node.box.max.y > m_box.max.y)
		m_box.max.y=node.box.max.y;
	if(node.box.max.z > m_box.max.z)
		m_box.max.z=node.box.max.z;

	if(node.box.min.x < m_box.min.x)
		m_box.min.x=node.box.min.x;
	if(node.box.min.y < m_box.min.y)
		m_box.min.y=node.box.min.y;
	if(node.box.min.z < m_box.min.z)
		m_box.min.z=node.box.min.z;

	return m_box;
}

bool OctreeNodeBuilder::BoxIfNeedSplit(const AABBox& box)
{
	return (box.max.x-box.min.x>m_boxSize
		&& box.max.y-box.min.y>m_boxSize
		&& box.max.z-box.min.z>m_boxSize);
}

bool OctreeNodeBuilder::NodeIfNeedSplit(UINT numNodes)
{
	//return numNodes>10 && m_lv<4;
	return numNodes>10 && m_lv<10;
}

void OctreeNodeBuilder::BuildChildBoxes(const AABBox& parentBox,AABBox *pBoxes,int& numBoxes)
{
	float xSize=parentBox.max.x-parentBox.min.x;
	float ySize=parentBox.max.y-parentBox.min.y;
	float zSize=parentBox.max.z-parentBox.min.z;
	xSize*=0.5f;
	ySize*=0.5f;
	zSize*=0.5f;

	float scalex2y = 1.0f;
	float scalex2z = 1.0f;
	float scaley2z = 1.0f;
	int index = 0;

	float lowLimit = 0.6f;
	float highLimit = 1.67f;

	scalex2y = xSize / ySize;
	if( scalex2y <= lowLimit )//����y
	{
		AABBox *pBox=pBoxes+0;
		pBox->min.x=parentBox.min.x;
		pBox->max.x=parentBox.max.x;
		pBox->min.y=parentBox.min.y;
		pBox->max.y=parentBox.max.y-ySize;
		pBox->min.z=parentBox.min.z;
		pBox->max.z=parentBox.max.z;

		pBox=pBoxes+1;
		pBox->min.x=parentBox.min.x;
		pBox->max.x=parentBox.max.x;
		pBox->min.y=parentBox.min.y+ySize;
		pBox->max.y=parentBox.max.y;
		pBox->min.z=parentBox.min.z;
		pBox->max.z=parentBox.max.z;

		numBoxes = 2;

		return;
	}
	else if( scalex2y >= highLimit )//����x
	{
		AABBox *pBox=pBoxes+0;
		pBox->min.x=parentBox.min.x;
		pBox->max.x=parentBox.max.x-xSize;
		pBox->min.y=parentBox.min.y;
		pBox->max.y=parentBox.max.y;
		pBox->min.z=parentBox.min.z;
		pBox->max.z=parentBox.max.z;

		pBox=pBoxes+1;
		pBox->min.x=parentBox.min.x+xSize;
		pBox->max.x=parentBox.max.x;
		pBox->min.y=parentBox.min.y;
		pBox->max.y=parentBox.max.y;
		pBox->min.z=parentBox.min.z;
		pBox->max.z=parentBox.max.z;

		numBoxes = 2;

		return;
	}

	scalex2z = xSize / zSize;
	if( scalex2z <= lowLimit )//����z
	{
		AABBox *pBox=pBoxes+0;
		pBox->min.x=parentBox.min.x;
		pBox->max.x=parentBox.max.x;
		pBox->min.y=parentBox.min.y;
		pBox->max.y=parentBox.max.y;
		pBox->min.z=parentBox.min.z;
		pBox->max.z=parentBox.max.z-zSize;

		pBox=pBoxes+1;
		pBox->min.x=parentBox.min.x;
		pBox->max.x=parentBox.max.x;
		pBox->min.y=parentBox.min.y;
		pBox->max.y=parentBox.max.y;
		pBox->min.z=parentBox.min.z+zSize;
		pBox->max.z=parentBox.max.z;

		numBoxes = 2;

		return;
	}
	else if( scalex2z >= highLimit )//����x
	{
		AABBox *pBox=pBoxes+0;
		pBox->min.x=parentBox.min.x;
		pBox->max.x=parentBox.max.x-xSize;
		pBox->min.y=parentBox.min.y;
		pBox->max.y=parentBox.max.y;
		pBox->min.z=parentBox.min.z;
		pBox->max.z=parentBox.max.z;

		pBox=pBoxes+1;
		pBox->min.x=parentBox.min.x+xSize;
		pBox->max.x=parentBox.max.x;
		pBox->min.y=parentBox.min.y;
		pBox->max.y=parentBox.max.y;
		pBox->min.z=parentBox.min.z;
		pBox->max.z=parentBox.max.z;

		numBoxes = 2;

		return;
	}

	if( scaley2z <= lowLimit )//����z
	{
		AABBox *pBox=pBoxes+0;
		pBox->min.x=parentBox.min.x;
		pBox->max.x=parentBox.max.x;
		pBox->min.y=parentBox.min.y;
		pBox->max.y=parentBox.max.y;
		pBox->min.z=parentBox.min.z;
		pBox->max.z=parentBox.max.z-zSize;

		pBox=pBoxes+1;
		pBox->min.x=parentBox.min.x;
		pBox->max.x=parentBox.max.x;
		pBox->min.y=parentBox.min.y;
		pBox->max.y=parentBox.max.y;
		pBox->min.z=parentBox.min.z+zSize;
		pBox->max.z=parentBox.max.z;

		numBoxes = 2;

		return;
	}
	else if( scaley2z >= highLimit )//����y
	{
		AABBox *pBox=pBoxes+0;
		pBox->min.x=parentBox.min.x;
		pBox->max.x=parentBox.max.x;
		pBox->min.y=parentBox.min.y;
		pBox->max.y=parentBox.max.y-ySize;
		pBox->min.z=parentBox.min.z;
		pBox->max.z=parentBox.max.z;

		pBox=pBoxes+1;
		pBox->min.x=parentBox.min.x;
		pBox->max.x=parentBox.max.x;
		pBox->min.y=parentBox.min.y+ySize;
		pBox->max.y=parentBox.max.y;
		pBox->min.z=parentBox.min.z;
		pBox->max.z=parentBox.max.z;

		numBoxes = 2;

		return;
	}

	AABBox *pBox=pBoxes+0;
	pBox->min.x=parentBox.min.x;
	pBox->max.x=parentBox.max.x-xSize;
	pBox->min.y=parentBox.min.y;
	pBox->max.y=parentBox.max.y-ySize;
	pBox->min.z=parentBox.min.z+zSize;
	pBox->max.z=parentBox.max.z;

	pBox=pBoxes+1;
	pBox->min.x=parentBox.min.x+xSize;
	pBox->max.x=parentBox.max.x;
	pBox->min.y=parentBox.min.y;
	pBox->max.y=parentBox.max.y-ySize;
	pBox->min.z=parentBox.min.z+zSize;
	pBox->max.z=parentBox.max.z;

	pBox=pBoxes+2;
	pBox->min.x=parentBox.min.x+xSize;
	pBox->max.x=parentBox.max.x;
	pBox->min.y=parentBox.min.y;
	pBox->max.y=parentBox.max.y-ySize;
	pBox->min.z=parentBox.min.z;
	pBox->max.z=parentBox.max.z-zSize;

	pBox=pBoxes+3;
	pBox->min.x=parentBox.min.x;
	pBox->max.x=parentBox.max.x-xSize;
	pBox->min.y=parentBox.min.y;
	pBox->max.y=parentBox.max.y-ySize;
	pBox->min.z=parentBox.min.z;
	pBox->max.z=parentBox.max.z-zSize;

	pBox=pBoxes+4;
	pBox->min.x=parentBox.min.x;
	pBox->max.x=parentBox.max.x-xSize;
	pBox->min.y=parentBox.min.y+ySize;
	pBox->max.y=parentBox.max.y;
	pBox->min.z=parentBox.min.z+zSize;
	pBox->max.z=parentBox.max.z;

	pBox=pBoxes+5;
	pBox->min.x=parentBox.min.x+xSize;
	pBox->max.x=parentBox.max.x;
	pBox->min.y=parentBox.min.y+ySize;
	pBox->max.y=parentBox.max.y;
	pBox->min.z=parentBox.min.z+zSize;
	pBox->max.z=parentBox.max.z;

	pBox=pBoxes+6;
	pBox->min.x=parentBox.min.x+xSize;
	pBox->max.x=parentBox.max.x;
	pBox->min.y=parentBox.min.y+ySize;
	pBox->max.y=parentBox.max.y;
	pBox->min.z=parentBox.min.z;
	pBox->max.z=parentBox.max.z-zSize;

	pBox=pBoxes+7;
	pBox->min.x=parentBox.min.x;
	pBox->max.x=parentBox.max.x-xSize;
	pBox->min.y=parentBox.min.y+ySize;
	pBox->max.y=parentBox.max.y;
	pBox->min.z=parentBox.min.z;
	pBox->max.z=parentBox.max.z-zSize;

	numBoxes = 8;
}

int OctreeNodeBuilder::CalSplitPlaneScore( vector<tagNode>& nodes,Vector4 plane )
{
	int front=0,back=0;

	for(size_t i=0;i<nodes.size();i++)
	{
		AABBox& box=nodes[i].box;
		Vector3 p=box.GetCenter();
		float d=plane.x*p.x+plane.y*p.y+plane.z*p.z+plane.w;
		if(d>0)
			front++;
		else
			back++;
	}

	return abs(front-back);
}

void OctreeNodeBuilder::SplitBalanceBox( vector<tagNode>& nodes,AABBox splitBox[8] )
{
	/*
		z
		/\
		|
		|
	����
		6-----7-----8 max
		|	  |		|
		|	  |		|
		3-----4-----5
		|	  |		|
		|	  |		|
		0-----1-----2

	�м�
		15----16----17
		|	  |		|
		|	  |		|
		12----13----14
		|	  |		|
		|	  |		|
		9-----10----11

	�ײ�
		24----25----26
		|	  |		|
		|	  |		|
		21----22----23
		|	  |		|
		|	  |		|
	min 18----19----20	----->x
	*/

	/*
		box0	min->max	18->13
	*/

	/*
		box1	min->max	19->14
	*/

	/*
		box2	min->max	21->16
	*/

	/*
		box3	min->max	22->17
	*/

	/*
		box4	min->max	9->4
	*/

	/*
		box5	min->max	10->5
	*/

	/*
		box6	min->max	12->7
	*/

	/*
		box7	min->max	13->8
	*/

	Vector3 size=m_box.max-m_box.min;
	const int numTest=64;
	int s;

	//yz plane
	int miniScore=nodes.size();
	int xblanceS=1;
	for(s=1;s<numTest;s++)
	{
		Vector4 plane(1,0,0,m_box.min.x+size.x/numTest*s);
		plane.w*=-1.0f;
		int ss=CalSplitPlaneScore(nodes,plane);//blance
		if(ss<miniScore)
		{
			miniScore=ss;
			xblanceS=s;
		}
	}


	//xz plane
	miniScore=nodes.size();
	int yblanceS=1;
	for(s=1;s<numTest;s++)
	{
		Vector4 plane(0,1,0,m_box.min.y+size.y/numTest*s);
		plane.w*=-1.0f;
		int ss=CalSplitPlaneScore(nodes,plane);//blance
		if(ss<miniScore)
		{
			miniScore=ss;
			yblanceS=s;
		}
	}


	//xy plane
	miniScore=nodes.size();
	int zblanceS=1;
	for(s=1;s<numTest;s++)
	{
		Vector4 plane(0,0,1,m_box.min.z+size.z/numTest*s);
		plane.w*=-1.0f;
		int ss=CalSplitPlaneScore(nodes,plane);//blance
		if(ss<miniScore)
		{
			miniScore=ss;
			zblanceS=s;
		}
	}

	splitBox[0].min=m_box.min;
	splitBox[0].max=Vector3(m_box.min.x+size.x/numTest*xblanceS,
		m_box.min.y+size.y/numTest*yblanceS,
		m_box.min.z+size.z/numTest*zblanceS);

	splitBox[1].min=Vector3(m_box.min.x+size.x/numTest*xblanceS,
		m_box.min.y,
		m_box.min.z);
	splitBox[1].max=Vector3(m_box.max.x,
		m_box.min.y+size.y/numTest*yblanceS,
		m_box.min.z+size.z/numTest*zblanceS);

	splitBox[2].min=Vector3(m_box.min.x,m_box.min.y,m_box.min.z+size.z/numTest*zblanceS);
	splitBox[2].max=Vector3(m_box.min.x+size.x/numTest*xblanceS,
		m_box.min.y+size.y/numTest*yblanceS,
		m_box.max.z);

	splitBox[3].min=Vector3(m_box.min.x+size.x/numTest*xblanceS,
		m_box.min.y,m_box.min.z+size.z/numTest*zblanceS);
	splitBox[3].max=Vector3(m_box.max.x,m_box.min.y+size.y/numTest*yblanceS,m_box.max.z);

	splitBox[4].min=Vector3(m_box.min.x,m_box.min.y+size.y/numTest*yblanceS,m_box.min.z);
	splitBox[4].max=Vector3(m_box.min.x+size.x/numTest*xblanceS,
		m_box.max.y,m_box.min.z+size.z/numTest*zblanceS);

	splitBox[5].min=Vector3(m_box.min.x+size.x/numTest*xblanceS,
		m_box.min.y+size.y/numTest*yblanceS,m_box.min.z);
	splitBox[5].max=Vector3(m_box.max.x,m_box.max.y,m_box.min.z+size.z/numTest*zblanceS);

	splitBox[6].min=Vector3(m_box.min.x,m_box.min.y+size.y/numTest*yblanceS,m_box.min.z+size.z/numTest*zblanceS);
	splitBox[6].max=Vector3(m_box.min.x+size.x/numTest*xblanceS,m_box.max.y,m_box.max.z);

	splitBox[7].min=Vector3(m_box.min.x+size.x/numTest*xblanceS,
		m_box.min.y+size.y/numTest*yblanceS,
		m_box.min.z+size.z/numTest*zblanceS);
	splitBox[7].max=m_box.max;
}

void OctreeNodeBuilder::Prepare( vector<OctreeBuilder::tagNode>& nodes,vector<AABBox>& mergedBoxes,AABBox& parentBox )
{
	//--�����Ӻ���
	AABBox subBox[8];
	int boxCount=0;
	BuildChildBoxes(parentBox,subBox,boxCount);

	//--���������θ��Ӻ���
	vector<OctreeBuilder::tagNode> subNodes[8];
	int i;
	for(i=0;i<(int)nodes.size();i++)
	{
		for(int j=0;j<boxCount;j++)
		{
			if(subBox[j].IntersectsTriangle(nodes[i].a,nodes[i].b,nodes[i].c))
			{
				subNodes[j].push_back(nodes[i]);
			}
		}
	}

	//--�ж��Ӻ��Ӵ�С�������Ƿ��������
	if(!BoxIfNeedSplit(subBox[0]))
	{
		for(i=0;i<boxCount;i++)
		{
			if(subNodes[i].size()>0)
			{
				AABBox& sub=subBox[i];
				AABBox tempFace;
				tempFace.max=Vector3(-FLOAT_MAX,-FLOAT_MAX,-FLOAT_MAX);
				tempFace.min=Vector3(FLOAT_MAX,FLOAT_MAX,FLOAT_MAX);
				for(UINT faceIndex=0;faceIndex<subNodes[i].size();faceIndex++)
				{
					tempFace.Merge(subNodes[i][faceIndex].box);
				}

				AABBox tempBox;
				sub.Intersects(tempFace,tempBox);

				mergedBoxes.push_back(tempBox);
			}
		}
		return;
	}

	//--�ݹ�Ĵ����ӽڵ�
	for(i=0;i<boxCount;i++)
	{
		if(subNodes[i].size()>0)
		{
			Prepare(subNodes[i],mergedBoxes,subBox[i]);
		}
	}
}

void OctreeNodeBuilder::ComputeBoxForNodes( vector<tagNode>& nodes,AABBox& box )
{
	box.max=Vector3(-FLOAT_MAX,-FLOAT_MAX,-FLOAT_MAX);
	box.min=Vector3(FLOAT_MAX,FLOAT_MAX,FLOAT_MAX);

	for(UINT i=0;i<nodes.size();i++)
	{
		box.Merge(nodes[i].box);
	}
}

void OctreeNodeBuilder::BuildOctree( vector<tagNode>& nodes,int id,int level,const AABBox& box )
{
	if(nodes.size()==0)
		return;

	OctreeNode::m_id=id;
	OctreeNode::m_lv=level;
	m_box=box;

	//--�жϽڵ��Ƿ��������
	if(NodeIfNeedSplit(nodes.size()))
	{
		////--�����Ӻ���
		//AABBox subBox[8];
		//SplitBalanceBox(nodes,subBox);
		////for(int k=0;k<8;k++)//ʹ�ú��ӱ���һЩ
		////{
		////	Vector3 vSize=subBox[k].max-subBox[k].min;
		////	vSize*=0.25f;

		////	subBox[k].max+=vSize;
		////	subBox[k].min-=vSize;
		////}

		////--����ڵ���Ӻ���
		//vector<tagNode> subNodes[8];
		//int i=0;
		//for(i=0;i<(int)nodes.size();i++)
		//{
		//	tagNode& tag = nodes[i];
		//	for(int j=0;j<8;j++)
		//	{
		//		if( subBox[j].Intersects(nodes[i].box) )
		//		{
		//			subNodes[j].push_back(tag);
		//			//tag.id = 0xFFFF;
		//			//break;
		//		}
		//	}
		//}

		////--�ݹ�Ĵ����ӽڵ�
		//for(i=0;i<8;i++)
		//{
		//	if(subNodes[i].size()>0)
		//	{
		//		OctreeNodeBuilder *pNewNode=new OctreeNodeBuilder;
		//		pNewNode->BuildOctree(subNodes[i],NexID(),m_lv+1,subBox[i]);
		//		m_pChildren[i]=pNewNode;
		//		m_childrenID[i]=true;
		//	}
		//}

		//////--ʣ��ڵ�
		////for( i=0;i<nodes.size();i++ )
		////{
		////	if( nodes[i].id != 0xFFFF )
		////	{
		////		m_content.push_back(nodes[i].id);
		////	}
		////}

		//����ָ���
		AABBox splitBox[2];
		Vector4 plane=SelectSplitPlane(nodes,splitBox);

		//���ֽڵ�
		UINT i=0;
		vector<tagNode> subNodes[2];
		for(i=0;i<nodes.size();i++)
		{
			if(splitBox[0].Intersects(nodes[i].box))
			{
				subNodes[0].push_back(nodes[i]);
			}
			if(splitBox[1].Intersects(nodes[i].box))
			{
				subNodes[1].push_back(nodes[i]);
			}
		}

		//�ݹ�
		for(i=0;i<2;i++)
		{
			if(subNodes[i].size()>0)
			{
				OctreeNodeBuilder *pNode=new OctreeNodeBuilder;
				pNode->BuildOctree(subNodes[i],NexID(),m_lv+1,splitBox[i]);
				m_pChildren[i]=pNode;
				m_childrenID[i]=true;
			}
		}
	}
	else
	{
		for(UINT i=0;i<nodes.size();i++)
			m_content.push_back(nodes[i].id);
	}
}

void OctreeNodeBuilder::AddMesh( vector<OctreeBuilder::tagNode>& nodes,vector<AABBox>& boxes )
{
	if(nodes.size()==0)
		return;

	//Ԥ�����Ӻ��Ӽ������кϲ���Ȼ����а˲�������
	//--���ɰ�����
	m_box.max=Vector3(-FLOAT_MAX,-FLOAT_MAX,-FLOAT_MAX);
	m_box.min=Vector3(FLOAT_MAX,FLOAT_MAX,FLOAT_MAX);
	for(UINT index=0;index<nodes.size();index++)
		MergeNode(nodes[index]);
	m_box.min-=Vector3(100.0f,100.0f,100.0f);
	m_box.max+=Vector3(100.0f,100.0f,100.0f);

	Prepare(nodes,boxes,m_box);
	MergeOctreeBoxes(boxes);
}

void OctreeNodeBuilder::Build(vector<AABBox>& boxes,int id,int lv)
{
	vector<tagNode> mergedNodes;
	mergedNodes.resize(boxes.size());
	for(UINT i=0;i<mergedNodes.size();i++)
	{
		mergedNodes[i].box=boxes[i];
		mergedNodes[i].id=i;
	}

	//--�����������Щscene nodes�ĺ���
	ComputeBoxForNodes(mergedNodes,m_box);

	BuildOctree(mergedNodes,id,lv,m_box);
}

void OctreeNodeBuilder::GetChildrenR(vector<OctreeNode*>& out)
{
	int i=0;
	for(i=0;i<8;i++)
	{
		if(m_pChildren[i])
			out.push_back(m_pChildren[i]);
	}

	for(i=0;i<8;i++)
	{
		OctreeNodeBuilder *pChild=(OctreeNodeBuilder*)m_pChildren[i];
		if(pChild)
			pChild->GetChildrenR(out);
	}
}

void OctreeNodeBuilder::MergeOctreeBoxes( vector<AABBox>& octreeBoxes )
{
	UINT oldBoxNum;
	do 
	{
		oldBoxNum = octreeBoxes.size();
		MergeOctreeBoxesByXAxis(octreeBoxes);
		MergeOctreeBoxesByZAxis(octreeBoxes);
		MergeOctreeBoxesByYAxis(octreeBoxes);
	} while( oldBoxNum != octreeBoxes.size() );
}

void OctreeNodeBuilder::MergeOctreeBoxesByXAxis( vector<AABBox>& octreeBoxes )
{
	int numList=0;

	//--��x������кϲ�
	//--y�ᡢz����Сֵ��ȵ�box����һ��list����x�᷽�򳤶����
	for(UINT i=0;i<octreeBoxes.size();i++)
	{
		AABBox& box=octreeBoxes[i];
		bool bInsert=false;
		for(int j=0;j<numList;j++)
		{
			if(boxList[j].size()==0)
				break;
			boxListIter iter=boxList[j].begin();
			if(fabsf((*iter).min.y-box.min.y)<m_yEpsilon
				&& fabsf((*iter).min.z-box.min.z)<m_xzEpsilon
				&& fabsf((*iter).max.y-box.max.y)<m_yEpsilon
				&& fabsf((*iter).max.z-box.max.z)<m_xzEpsilon)
			{
				boxList[j].push_back(box);
				bInsert=true;
				break;
			}
		}
		//--�½�һ��List
		if(!bInsert)
		{
			boxList[numList++].push_back(box);
		}
	}

	for(int i=0;i<MAX_SIZE;i++)
	{
		if(boxList[i].size()==0)
			break;

		int numBoxes=0;
		do 
		{
			numBoxes=boxList[i].size();

			//ÿ��ǰ�ýڵ�����ýڵ���кϲ���ֱ�����һ���ڵ�
			boxListIter iter=boxList[i].begin();
			for(;iter!=boxList[i].end();++iter)
			{
				boxListIter behind=iter;
				for(++behind;behind!=boxList[i].end();)
				{
					AABBox& preNode=(*iter);
					AABBox& nexNode=(*behind);
					//��������ڵ�ĺ���������ϲ�������ɾ�����ýڵ�
					if(fabsf(preNode.min.x-nexNode.max.x)<m_xzEpsilon
						|| fabsf(preNode.max.x-nexNode.min.x)<m_xzEpsilon)
					{
						preNode.Merge(nexNode);
						behind=boxList[i].erase(behind);
						continue;
					}
					++behind;
				}
			}
		} while (numBoxes!=boxList[i].size());
	}

	//�Ѻϲ����box���·���octreeBoxes
	octreeBoxes.clear();
	for(int i=0;i<MAX_SIZE;i++)
	{
		if(boxList[i].size()==0)
			break;

		for(boxListIter iter=boxList[i].begin();iter!=boxList[i].end();++iter)
		{
			octreeBoxes.push_back(*iter);
		}
	}
	
	for(int i=0;i<MAX_SIZE;i++)
	{
		if(boxList[i].size()==0)
			break;

		boxList[i].clear();
	}
}

void OctreeNodeBuilder::MergeOctreeBoxesByZAxis( vector<AABBox>& octreeBoxes )
{
	int numList=0;

	//��z������кϲ������Ǻ����Ѿ�������ͬ�ȴ�С�ˣ���Ҫ�жϺ��ӵ�size�����Ƿ���Ժϲ�
	for(UINT i=0;i<octreeBoxes.size();i++)
	{
		AABBox& box=octreeBoxes[i];
		bool bInsert=false;
		for(int j=0;j<numList;j++)
		{
			if(boxList[j].size()==0)
				break;
			boxListIter iter=boxList[j].begin();
			if(fabsf((*iter).min.y-box.min.y)<m_yEpsilon
				&& fabsf((*iter).min.x-box.min.x)<m_xzEpsilon
				&& fabsf((*iter).max.y-box.max.y)<m_yEpsilon
				&& fabsf((*iter).max.x-box.max.x)<m_xzEpsilon)
			{
				boxList[j].push_back(box);
				bInsert=true;
				break;
			}
		}
		//--�½�һ��List
		if(!bInsert)
		{
			boxList[numList++].push_back(box);
		}
	}

	for(int i=0;i<MAX_SIZE;i++)
	{
		if(boxList[i].size()==0)
			break;

		int numBoxes=0;
		do 
		{
			numBoxes=boxList[i].size();

			//ÿ��ǰ�ýڵ�����ýڵ���кϲ���ֱ�����һ���ڵ�
			boxListIter iter=boxList[i].begin();
			for(;iter!=boxList[i].end();++iter)
			{
				boxListIter behind=iter;
				for(++behind;behind!=boxList[i].end();)
				{
					//��������ڵ�ĺ���������ϲ�������ɾ�����ýڵ�
					AABBox& preNode=*iter;
					AABBox& nexNode=*behind;
					if(fabsf(preNode.min.z-nexNode.max.z)<m_xzEpsilon
						|| fabsf(preNode.max.z-nexNode.min.z)<m_xzEpsilon)
					{
						preNode.Merge(nexNode);
						behind=boxList[i].erase(behind);
						continue;
					}
					++behind;
				}
			}
		} while (numBoxes!=boxList[i].size());
	}

	//�Ѻϲ����box���·���octreeBoxes
	octreeBoxes.clear();
	for(int i=0;i<MAX_SIZE;i++)
	{
		if(boxList[i].size()==0)
			break;

		for(boxListIter iter=boxList[i].begin();iter!=boxList[i].end();++iter)
		{
			octreeBoxes.push_back(*iter);
		}
	}

	for(int i=0;i<MAX_SIZE;i++)
	{
		if(boxList[i].size()==0)
			break;

		boxList[i].clear();
	}
}

void OctreeNodeBuilder::MergeOctreeBoxesByYAxis( vector<AABBox>& octreeBoxes )
{
	int numList=0;

	//��y������кϲ�
	for(UINT i=0;i<octreeBoxes.size();i++)
	{
		AABBox& box=octreeBoxes[i];
		bool bInsert=false;
		for(int j=0;j<numList;j++)
		{
			if(boxList[j].size()==0)
				break;
			boxListIter iter=boxList[j].begin();
			if(fabsf((*iter).min.z-box.min.z)<m_xzEpsilon
				&& fabsf((*iter).min.x-box.min.x)<m_xzEpsilon
				&& fabsf((*iter).max.z-box.max.z)<m_xzEpsilon
				&& fabsf((*iter).max.x-box.max.x)<m_xzEpsilon)
			{
				boxList[j].push_back(box);
				bInsert=true;
				break;
			}
		}
		//--�½�һ��List
		if(!bInsert)
		{
			boxList[numList++].push_back(box);
		}
	}

	for(int i=0;i<MAX_SIZE;i++)
	{
		if(boxList[i].size()==0)
			break;

		int numBoxes=0;
		do 
		{
			numBoxes=boxList[i].size();

			//ÿ��ǰ�ýڵ�����ýڵ���кϲ���ֱ�����һ���ڵ�
			boxListIter iter=boxList[i].begin();
			for(;iter!=boxList[i].end();++iter)
			{
				boxListIter behind=iter;
				for(++behind;behind!=boxList[i].end();)
				{
					//��������ڵ�ĺ���������ϲ�������ɾ�����ýڵ�
					AABBox& preNode=*iter;
					AABBox& nexNode=*behind;
					if(fabsf(preNode.min.y-nexNode.max.y)<m_yEpsilon
						|| fabsf(preNode.max.y-nexNode.min.y)<m_yEpsilon)
					{
						preNode.Merge(nexNode);
						behind=boxList[i].erase(behind);
						continue;
					}
					++behind;
				}
			}
		} while (numBoxes!=boxList[i].size());
	}

	//�Ѻϲ����box���·���octreeBoxes
	octreeBoxes.clear();
	for(int i=0;i<MAX_SIZE;i++)
	{
		if(boxList[i].size()==0)
			break;

		for(boxListIter iter=boxList[i].begin();iter!=boxList[i].end();++iter)
		{
			octreeBoxes.push_back(*iter);
		}
	}

	for(int i=0;i<MAX_SIZE;i++)
	{
		if(boxList[i].size()==0)
			break;

		boxList[i].clear();
	}
}

Vector4 OctreeNodeBuilder::SelectSplitPlane( vector<tagNode>& nodes,AABBox splitBox[2] )
{
	Vector4 retPlane(0,1,0,0);
	Vector3 size=m_box.max-m_box.min;
	const int numTest=64;
	TCHAR axis;

	//�ҵ�����
	if(size.x>size.y)
	{
		if(size.x>size.z)
			axis=_T('x');
		else
			axis=_T('z');
	}
	else
	{
		if(size.y>size.z)
			axis=_T('y');
		else
			axis=_T('z');
	}

	switch(axis)
	{
	case _T('x')://yz plane
		{
			int miniScore=nodes.size();
			int blanceS=1;
			for(int s=1;s<numTest;s++)
			{
				Vector4 plane(1,0,0,m_box.min.x+size.x/numTest*s);
				plane.w*=-1.0f;
				int ss=CalSplitPlaneScore(nodes,plane);//blance
				if(ss<miniScore)
				{
					retPlane=plane;
					miniScore=ss;
					blanceS=s;
				}
			}
			splitBox[0].min=m_box.min;
			splitBox[0].max=Vector3(m_box.min.x+size.x/numTest*blanceS,m_box.max.y,m_box.max.z);
			splitBox[1].min=Vector3(m_box.min.x+size.x/numTest*blanceS,m_box.min.y,m_box.min.z);
			splitBox[1].max=m_box.max;
		}
		break;
	case _T('y'):
		{
			int miniScore=nodes.size();
			int blanceS=1;
			for(int s=1;s<numTest;s++)
			{
				Vector4 plane(0,1,0,m_box.min.y+size.y/numTest*s);
				plane.w*=-1.0f;
				int ss=CalSplitPlaneScore(nodes,plane);//blance
				if(ss<miniScore)
				{
					retPlane=plane;
					miniScore=ss;
					blanceS=s;
				}
			}
			splitBox[0].min=m_box.min;
			splitBox[0].max=Vector3(m_box.max.x,m_box.min.y+size.y/numTest*blanceS,m_box.max.z);
			splitBox[1].min=Vector3(m_box.min.x,m_box.min.y+size.y/numTest*blanceS,m_box.min.z);
			splitBox[1].max=m_box.max;
		}
		break;
	case _T('z'):
		{
			int miniScore=nodes.size();
			int blanceS=1;
			for(int s=1;s<numTest;s++)
			{
				Vector4 plane(0,0,1,m_box.min.z+size.z/numTest*s);
				plane.w*=-1.0f;
				int ss=CalSplitPlaneScore(nodes,plane);//blance
				if(ss<miniScore)
				{
					retPlane=plane;
					miniScore=ss;
					blanceS=s;
				}
			}
			splitBox[0].min=m_box.min;
			splitBox[0].max=Vector3(m_box.max.x,m_box.max.y,m_box.min.z+size.z/numTest*blanceS);
			splitBox[1].min=Vector3(m_box.min.x,m_box.min.y,m_box.min.z+size.z/numTest*blanceS);
			splitBox[1].max=m_box.max;
		}
		break;
	}

	return retPlane;
}