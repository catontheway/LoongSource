#pragma once
#include <vector>
#include <map>
#include <windows.h>
#include "..\Device\IIndexBuffer.h"
#include "..\RenderSys\HardwareBufferPool.h"
using namespace std;

namespace Cool3D
{
	class TRiverPatchTriangulationDynamic;
	/** \class TRiverPatchLODDynamic
		\brief ����������һ��LOD������������
	*/
	class TRiverPatchLODDynamic
	{
		struct tagIB
		{
			vector<WORD> indices;
			IIndexBuffer* pIB;
		};
	public:
		TRiverPatchLODDynamic();
		~TRiverPatchLODDynamic();

		void Create(TRiverPatchTriangulationDynamic* pParent,int lod);
		void Destroy();

		IIndexBuffer* GetIB_HW(int& numFaces);
		const vector<WORD>& GetIB();

	private:
		inline WORD MakeIndex(int x, int y);

		void BuildIndices(vector<WORD>& indices);
		const tagIB& _GetIB();

	private:
		int		m_lod;
		int		m_mapW,m_mapH;
		TRiverPatchTriangulationDynamic		*m_pParent;

		HardwareBufferPool<StaticIBAlloc>	m_ibPool;
		map<int,tagIB>	m_ibCache;
	};


	/**	\class TRiverPatchTriangulationDynamic
		\brief Ϊ����������������б�
		\remark 
		\par ֧��LOD
		\par �ڽ�Ӳ��IndexBuffer����أ������Ѵ��������������з���
	*/
	class TRiverPatchTriangulationDynamic
	{
		friend class TRiverPatchLODDynamic;
	public:
		enum 
		{ 
			NUM_LOD	= 3
		};
	public:
		void Create();
		void Destroy();

		IIndexBuffer* GetIB_HW(int lod,int& numVerts,int& numFaces);
		const vector<WORD>& GetIB(int lod);

		static TRiverPatchTriangulationDynamic* Inst();

	private:
		TRiverPatchTriangulationDynamic(void);
		~TRiverPatchTriangulationDynamic(void);

		TRiverPatchLODDynamic m_lods[NUM_LOD];
		static int m_sideGrids[NUM_LOD];		//ÿ��grid����
		static int m_sideVerts[NUM_LOD];		//ÿ�߶�������
	};

}//namespace Cool3D