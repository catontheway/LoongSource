#include "SceneMeshFile.h"
#include "..\..\MaxPublic\staticMeshFile.h"
#include "..\MsgBox.h"
#include "..\..\MaxPublic\BintreeNode.h"
#include "..\ExpMtl.h"

namespace Cool3D
{
	SceneMeshFile::SceneMeshFile()
	{
		
	}
	
	SceneMeshFile::~SceneMeshFile()
	{
		
	}
	
	bool SceneMeshFile::Write(const TCHAR* szPath, INode *pNode)
	{
		Prepare();

		ComputeAABBox();
		
		FILE* fp=_tfopen(szPath,_T("wb"));
		if(fp == NULL)
			return false;
		size_t i;
		SceneMeshHeader header;
		memset(&header,0,sizeof(header));
		wcscpy(header.magic,SceneMesh_Magic);

		header.ver=2;
		header.vertType=m_vertType;
		header.numVert=m_vertBuf.size()/GetVertSize(m_vertType);
		header.numMesh=m_subMeshs.size();
		header.numTagNode=m_tagNodeFrm.nodes.size();
		header.min=min;
		header.max=max;
		header.lightmapSize=F3DExpMtl::s_iLightMapSize;

		if( F3DExpMtl::s_bExportLMUV
			&& header.lightmapSize!=0
			&& header.lightmapSize!=32
			&& header.lightmapSize!=64
			&& header.lightmapSize!=128
			&& header.lightmapSize!=256
			&& header.lightmapSize!=512
			&& header.lightmapSize!=1024)
		{
			g_msgBox.ShowMsg(_T("lightmap�Ĵ�С������0��32��64��128��256��512��1024�е�һ��ֵ��\r\n"));
			return false;
		}

		//--pre write header
		fwrite(&header,sizeof(header),1,fp);
		
		//--context
		header.offsetVert=ftell(fp);
		fwrite(&(*m_vertBuf.begin()),GetVertSize(header.vertType),header.numVert,fp);

		header.offsetMesh=ftell(fp);
		for(i=0;i<header.numMesh;i++)
			m_subMeshs[i].WriteFile(fp);

		header.offsetMtl=ftell(fp);
		header.numMtl=WriteMtlFromSceneContext(fp,szPath);

		header.offsetTagNode=ftell(fp);
		m_tagNodeFrm.WriteFile(fp);

		header.boxTreeOffset=ftell(fp);
		m_boxTree.WriteFile(fp);
				
		//--header
		fseek(fp,0,SEEK_SET);
		fwrite(&header,sizeof(header),1,fp);

		fclose(fp);
		fp=NULL;

		//--����Nav������ײ���
		if( F3DExpMtl::s_bExportBox )
		{
			if( m_octree.GetNumBox() == 0 )//û����ײ��
			{
				g_msgBox.ShowMsg(_T("��ģ��û����ײ�У�����ȫ�����οղ��ʣ���Ҫ��ѡѡ������µ�����\r\n"));
				return false;
			}
			else
			{
				TCHAR szDrive[512];
				TCHAR szDir[512];
				TCHAR szFilePath[512];
				TCHAR szNavPath[512];
				_tsplitpath(szPath,szDrive,szDir,szFilePath,NULL);
				_stprintf(szNavPath,_T("%s%s%s.Nav"),szDrive,szDir,szFilePath);
				fp=_tfopen(szNavPath,_T("wb"));
				if(fp == NULL)
					return false;

				SceneMeshNavHeader navHeader;
				memset(&navHeader,0,sizeof(navHeader));
				wcscpy(navHeader.magic,SceneMeshNav_Magic);

				//navHeader.ver=1;
				navHeader.ver=2;
				fwrite(&navHeader,sizeof(navHeader),1,fp);

				navHeader.offsetOctree=ftell(fp);
				OctreeNode::maxLevel=0;
				OctreeNode::noLeafCount=0;
				OctreeNode::boxCount=0;
				OctreeNode::maxContent=0;
				OctreeNode::boxTotal=0;
				m_octree.WriteFile(fp);
				//g_msgBox.ShowMsg(_T("�˲���ͳ����Ϣ---------------------------------------\r\n"));
				//g_msgBox.ShowMsg(_T("�˲���--��ײ��������%d\r\n"),m_octree.GetNumBox());
				//g_msgBox.ShowMsg(_T("�˲���--��������%d\r\n"),OctreeNode::maxLevel);
				//g_msgBox.ShowMsg(_T("�˲���--��Ҷ�ӽڵ�������%d\r\n"),OctreeNode::noLeafCount);
				//g_msgBox.ShowMsg(_T("�˲���--Ҷ�ӽڵ�������%d\r\n"),OctreeNode::boxCount);
				//g_msgBox.ShowMsg(_T("�˲���--Ҷ�ӽڵ�ӵ�е����AABBox������%d\r\n"),OctreeNode::maxContent);
				//g_msgBox.ShowMsg(_T("�˲���--��������������%d\r\n"),OctreeNode::boxTotal);
				//g_msgBox.ShowMsg(_T("�˲���ͳ����Ϣ---------------------------------------\r\n"));
				g_msgBox.ShowMsg(_T("������ͳ����Ϣ---------------------------------------\r\n"));
				g_msgBox.ShowMsg(_T("������--��ײ��������%d\r\n"),m_octree.GetNumBox());
				g_msgBox.ShowMsg(_T("������--��������%d\r\n"),OctreeNode::maxLevel);
				g_msgBox.ShowMsg(_T("������--��Ҷ�ӽڵ�������%d\r\n"),OctreeNode::noLeafCount);
				g_msgBox.ShowMsg(_T("������--Ҷ�ӽڵ�������%d\r\n"),OctreeNode::boxCount);
				g_msgBox.ShowMsg(_T("������--Ҷ�ӽڵ�ӵ�е����AABBox������%d\r\n"),OctreeNode::maxContent);
				g_msgBox.ShowMsg(_T("������--��������������%d\r\n"),OctreeNode::boxTotal);
				g_msgBox.ShowMsg(_T("������ͳ����Ϣ---------------------------------------\r\n"));

				//--header
				fseek(fp,0,SEEK_SET);
				fwrite(&navHeader,sizeof(navHeader),1,fp);

				fclose(fp);
			}
		}
		
		return true;
	}


	void SceneMeshFile::Prepare()
	{
		StaticMeshWriter::Prepare();

		m_boxTree.BeginBuild(&(*m_vertBuf.begin()), m_vertType);
		int oldMtlID=-1;

		for(size_t i=0;i<m_subMeshs.size();i++)
		{
			LodSubMesh& subMesh=m_subMeshs[i];
			//--�οղ��ʲ�����octree��aabbtree
			if(subMesh.mtlID>=0)
			{
				tagMtlDesc mtl;
				SceneContext::Inst()->GetMtl(subMesh.mtlID)->GetMtlDesc(mtl);
				if(mtl.m_bOpcityMap
					&& !F3DExpMtl::s_bExportOpcity)
					continue;
			}

			if( subMesh.mtlID <= oldMtlID )//��ͬ����ʹ�ò�ͬ�Ķ��㼯��
			{
				m_boxTree.AddSubMesh(&(*subMesh.baseFaces.begin()),subMesh.baseFaces.size(),subMesh.numVert);
			}
			else
			{
				oldMtlID = subMesh.mtlID;
				m_boxTree.AddSubMesh(&(*subMesh.baseFaces.begin()),subMesh.baseFaces.size(),0);
			}

			//m_boxTree.AddSubMesh(&(*subMesh.baseFaces.begin()),subMesh.baseFaces.size());
		}
		int numNodes=m_boxTree.EndBuild();
		g_msgBox.ShowMsg(_T("AABBTree number nodes=%d.\r\n"),numNodes);
	}
}//namespace Cool3D