#include "MAX.h"
#pragma once

/**	\class SceneEnumProc
	\brief ö�����г����е�Node, ����Ҫ�����Node,��¼��SceneContext
	\see MAX class ITreeEnumProc
*/

class SceneEnumProc : public ITreeEnumProc  
{
public:
	/** �����Light node
	*/
	bool IsLightNode(INode *pNode);
	/** ����Ƿ�Ϊ��Ҫ�����Mesh node
		\param The node to check
		\return true for need export
		\remarks ��Node������ת����Triobj����Polyobj
	*/
	bool IsMeshNode(INode *pNode);
	/**	Max ָ���Ļص�����
		\param The node
		\return One of the following values may be returned to control how enumeration continues:<br>
			TREE_CONTINUE	Continue enumerating.<br>
			TREE_IGNORECHILDREN	Don't enumerate the children of this node, but continue enumerating.<br>
			TREE_ABORT Stop enumerating.<br>
	*/
	virtual int callback( INode *node );
	SceneEnumProc();
	virtual ~SceneEnumProc();

};
