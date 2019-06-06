#include "SceneContext.h"
#include "ExpMesh.h"
#include "MeshExportBaseDef.h"
#pragma once

#include <string>
using namespace std;
#include "..\MaxPublic\subMesh.h"

namespace Cool3D
{
	/**	\class SceneWriter
		\brief abstract Strategy of writing, Ҳ���Builderģʽ, �ṩ����������д�빦��,������������.
	*/
	class SceneWriter  
	{
	public:
		virtual bool Write(const TCHAR* szPath, INode *pNode=NULL)=0;
		SceneWriter();
		virtual ~SceneWriter();
		EVertType GetVertType() const { return m_vertType; }
		void SetVertType(EVertType vertType) { m_vertType=vertType; }
		
	protected:
		EVertType	m_vertType;
		/**	��SceneContext�е�Mtlд���ļ�
			\return Mtl�ĸ���
		*/
		int WriteMtlFromSceneContext(FILE *fp, const TCHAR* szFilePath);
		/**	��һ��ExpMesh�����е�VertBuf������Out��
		*/
		void GetVert(ExpMesh *pMesh, std::vector<BYTE>& out);
		/**	��һ��ExpMesh�����е�����SubMesh������Out��
		*/
		void GetSubMeshs(WORD baseIndex,WORD numVert,ExpMesh* pMesh, std::vector<SubMesh>& out);
		void BuildLodSubMesh(const SubMesh& src,LodSubMesh& out);
		virtual void Prepare();
		void GetPos(BYTE *m_pBuf, UINT i,Vector3& out);
	};

	SceneWriter *GreateWriter(const TCHAR* szPath);	
}