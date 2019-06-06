#include "..\SceneWriter.h"
#include "..\SceneMesh\OctreeBuilder.h"
#include <vector>

#pragma once

namespace Cool3D
{
	/**	\class StaticMeshWriter 
		\brief û���κ�Ԥ����ļ򵥼�����������
	*/
	class StaticMeshWriter : public SceneWriter  
	{
	public:
		StaticMeshWriter();
		virtual ~StaticMeshWriter();

		virtual bool Write(const TCHAR* szPath, INode *pNode=NULL);
		
	protected:
		virtual void Prepare();

	protected:
		void ComputeAABBox();
		Vector3	min,max;

		//���ǵ������ʽ�ı仯�������Ը�����Ϊ��λ���д洢
		std::vector<BYTE>		m_vertBuf;
		std::vector<LodSubMesh> m_subMeshs;
		TagNodeFrame			m_tagNodeFrm;
		OctreeBuilder			m_octree;
		INode					*m_pOriginNode;
		
		void GetVertices(ExpMesh *pMesh);
		void GetSubMeshs(WORD baseIndex,WORD numVert,ExpMesh* pMesh);
	};
}//namespace Cool3D