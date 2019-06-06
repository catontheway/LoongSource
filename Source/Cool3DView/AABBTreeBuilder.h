#pragma once

#include "..\Cool3D\Cool3DEngine.h"
#include "..\Cool3D\Public\VertexDefine.h"
#include "..\Cool3D\Public\AABBTree.h"
using namespace Cool3D;

class AABBTreeBuilder :	public AABBTree
{
public:
	AABBTreeBuilder(void);
	virtual ~AABBTreeBuilder(void);

	void BeginBuild(BYTE *pVB, EVertType vertType);
	void AddFace(AABBTreeNode* pNode);
	//!����һ���ж��ٸ�node
	int EndBuild();

private:
	//! �����Node���еݹ���з�
	void SplitNode(AABBTreeNode* pNode);
	//! ���Node�Ƿ���Ҫ�����з�
	bool ContinueSplit(AABBTreeNode* pNode);


	//!Ϊָ����Node�е�faces����aabbox
	void BuildAABBox(AABBTreeNode* pNode);
	//!����Node��AABBoxѡ����Nodeһ��Ϊ����Plane
	Vector4 SelectSplitPlane(AABBTreeNode* pNode);
	int CalSplitPlaneScore(AABBTreeNode* pNode,Vector4 plane);
	//!����ָ��Face�����ĵ�
	Vector3 FaceCenter(TriFace *pFace);
	//!��tree�м���һ��Node,����id
	int AddNode(AABBTreeNode* pNode)	
	{	
		int ret=m_nodes.size();
		m_nodes.push_back(pNode);
		return ret;
	}
	void GetPos(UINT i,Vector3& out);

private:
	BYTE			*m_pVB;
	EVertType		m_vertType;
	FaceBuffer		m_buildBuffer;
};
