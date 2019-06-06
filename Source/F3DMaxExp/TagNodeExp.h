#pragma once

#pragma warning(disable:4251)

#include <string>
#include <vector>
using namespace std;
#include "F3DMaxExp.h"
#include "..\MaxPublic\TagNodeDefine.h"
using namespace Cool3D;


class F3D_MAX_API TagNodeExp
{
public:
	TagNodeExp();
	~TagNodeExp();
	
	void AddMaxNode(INode *pNode);
	void Clear();
	//ֻ����ÿ��MaxNode�Ķ�����Ϣ,������frmtime��frmname
	void ExportFrameAni(TagNodeFrame& out,INode *pParent,TimeValue time,bool expColor=false);
private:
	vector<INode*>	m_maxNodes;
};