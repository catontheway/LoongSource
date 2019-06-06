#pragma once
#include <vector>
#include <map>
using namespace std;
#include <windows.h>

namespace Cool3D
{

	class TPatchTriangulationStatic;
	/** ����������һ��LOD������������
	*/
	class TPatchLodStatic
	{
		enum 
		{ 
			MAX_GRIDS=256
		};
	public:
		TPatchLodStatic();
		~TPatchLodStatic();

		//! ��������������
		void Create(int lod,TPatchTriangulationStatic* pParent);

		//! ��ȡ����������
		void GetIndices(const vector<int>& grids, vector<WORD>& indices);
		void GetIndices(const vector<int>& grids, vector<WORD>& indices,WORD baseIndex);
		void GetIndices(vector<WORD>& indices);

	private:
		
		//! heightmap����ת��������
		inline WORD MakeIndex(int x, int y);
	private:
		vector<WORD> m_indices[MAX_GRIDS];

		int m_mapW, m_mapH;
		int m_lod;
		int m_numGrids;

		TPatchTriangulationStatic* m_pParent;
	};


	/**	\class TPatchTriangulationStatic
		\brief Ϊ�ؿ�����������б�
		\remark 
			\par ֧��LOD
			\par ��ͬLOD�ĵؿ��ڲ����������ܶ���ͬ
			\par �ؿ黮�ֳ�NxN��GRID,GRID���ֳ�4��������
	*/
	class TPatchTriangulationStatic
	{
		friend class TPatchLodStatic;
	public:
		enum 
		{ 
			NUM_LOD=1
		};
	public:

		//! ��������LOD������������
		void Create();

		//! ��ȡָ��LOD������������
		void GetIndices(int lod,const vector<int>& grids,vector<WORD>& indices,int& numFace);
		void GetIndices(int lod,vector<WORD>& indices);

		static TPatchTriangulationStatic* Inst();
	private:
		TPatchTriangulationStatic(void);
		~TPatchTriangulationStatic(void);

		TPatchLodStatic m_lods[NUM_LOD];
		static int m_sideGrids[NUM_LOD];//ÿ��grid����
		static int m_sideVerts[NUM_LOD];//ÿ�߶�������
	};

}//end namespace Cool3D