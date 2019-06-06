/**	\mainpage 3DS Max8 Mesh Export Plug-in
	
	\section Design
	\par
		���Ŀ�������MAX��Mesh, Light����. ǰ��Ӧ֧��Polygon,Triangle����ͼԪ.
		Ӧ֧��������ͳ���ģ��(with LightingMap)�ͼ򵥵�Mesh����(����������).
		��Ӧ����Ϊ�ؼ�֡��������Ļ���(SceneContext����һ֡??).
	\par
		�������ʹ�õ����ģʽ��Strategy,Composition,Singleton. SceneContext��
	Composition��ʵ��, �����ò�ͬ��SceneWriter�ľ���Strategyд�벻ͬ�Ķ���
	�Ƹ�ʽ�ļ�.	
	\par
		Max�е�Mesh�޷���Ƴ�FlyWight. ��Ȼ���е�Max������MAX pipleline �Ŀ�ʼ��
		�����ǹ����, ������Ҫ���Ǵ�ObjectҪ�������ӵĶ�����modifier, ���ܴﵽ��
		���World space state, �����Ǽ򵥵�����ϵ�任.
	
	\section Usage
	\par
		���������ɵ�"MeshExporter.dle"������"Max\plugins"Ŀ¼, ��������MAX����.
	����Ҫ�����ѡ��MAX�˵�:File->Export, Ȼ��ѡ����Ҫ���ļ����ͼ���.
	\par
		���ʹ��Max����Lighting Map, ��Ҫѡ��ʹ��Map channelΪ3.
		���ʹ��Max����Normal Map����Ҫѡ��ʹ��Map channelΪ4.
	\par
		�����Դ����Ҫ��װMax8.

	\version 1.0.0
	\author	������ 
	\modify [3/23/2008 leili]
	\date	������2003��9��29��
*/
#include "Max.h"
#include <vector>
#include "Noncopyable.h"
#include <string>
#include "..\F3DMaxExp\TagNodeExp.h"
#pragma once


//!	\namespace Cool3D
namespace Cool3D
{
	class ExpMesh;
	class ExpLight;
	class ExpMtl;

	/** \class SceneContext
		\brief ����������ݵ�composion, Core class of this plug-in
		\par
		SceneContext���ΪSingleton, Ϊ��ǰ��Ӧ��MAX��������. ����Ҫ���������Ҫ��
		������MAX������. ��Щ����Ҫ��Ӧ��Ҫ��MAX��ʱ��״̬(ObjectState), ҪWriter
		friendly.
		\par
		�������Ӧ��Ϊ:1.Enumerate������Ҫ�����INode��Mtl, �����ౣ��; 2.����Ҫ�����
		world space state����ָ����ʱ�乹��ExpMesh etc; 3.���浽�ļ�
		\par
		��Ҫ����һ��Mesh����, һ��Light����.

		\todo �����һ���������Ҫ�����ĳ������Nodes(���ܰ�������), ����������Ƿֱ����
	*/
	class SceneContext  
	{
	public:
		SceneContext();
		~SceneContext();

		/**����һ���µĲ���
			\param the mtl,�Ӳ���ID
			\return	�½����ʵ�ID
			\remarks �������в���, ���û���ҵ�������
			\see ExpMesh::MergeFace()
		*/
		unsigned int MergeMtl(Mtl *pMaxMtl,int sub,Mtl *pBakedMtl);

		/**��������ڲ�����
		*/
		void ClearAll();

		/** �õ�������Ҫ�����MAX�ڵ�
			\see SceneEnumProc
		*/
		void BuildNodeList(IScene* pScene);

		/** ��Node List�е�MAX�ڵ�������Զ������ݽṹ
			\param ��Ҫ�����MAX������ʱ���, Ĭ��Ϊ������ʼ��
			\see BuildNodeList()
		*/
		void ExportData(TimeValue time=0);
	
		/** ������д���ļ�
			\param �ļ�·��. ʹ�ú�׺����ȷ�������ļ�����
			\return �Ƿ�ɹ�
			\remarks �����ļ�����, ������ͬ��Strategy, ��Ӧ����д���ļ�
			\see SceneWriter, POLWriter
		*/
		bool WriteToFile(const TCHAR* szPath, bool bExpLMUV=false, bool bExpBUUV=false, const TCHAR* szOrigin=NULL);

		INode *GetNode(const TCHAR *szOrigin) const;
		bool GetMat(INode *pNode, D3DXMATRIX& mat) const;
		
		void AddMeshNode(INode* pNode);
		void AddBoneNode(INode* pNode);

		//-------------------------------------
		//�ڲ����ݶ����ӿ�
		int GetNumMeshs()
		{ return m_pMeshs.size(); }
		ExpMesh* GetMesh(int i)
		{
			//ASSERT(i>=0 && i<m_pMeshs.size());
			return m_pMeshs[i];
		}
		int GetNumMtls()
		{ return m_mtlLib.size(); }
		ExpMtl *GetMtl(int i)
		{
			return m_mtlLib[i];
		}
		TagNodeExp *GetTagNodeExp()
		{
			return &m_tagNodeExp;
		}
		//-------------------------------------

		/** ����Singletonָ��
		*/
		static SceneContext* Inst();
		void ClearExportData();

	private:
		//--MAX����
		std::vector<INode*>	m_pMeshNodes;
		std::vector<INode*> m_pBoneNodes;
		TagNodeExp			m_tagNodeExp;

		//--��������
		std::vector<ExpMtl*>	m_mtlLib;
		std::vector<ExpMesh*>	m_pMeshs;
	};
	
}