#pragma once

namespace WorldBase
{
	/** \class OutlinePathFinder
		\brief ��������Ѱ·�㷨
	*/
	class OutlinePathFinder
	{
	private:
		enum EWay
		{
			EW_None,
			EW_Top,
			EW_Left,
			EW_Right,
			EW_Bottom,
			EW_LeftTop,
			EW_RightTop,
			EW_LeftBottom,
			EW_RightBottom,
		};
		struct tagBlock		//�ϰ���
		{
			POINT inPt;		//�����
			POINT outPt;	//�ӳ���
		};
	public:
		OutlinePathFinder(void);
		~OutlinePathFinder(void);

		/** ���ÿ���������ͼ
		*/
		void SetCangoMap(BitMap* pCangoMap);
		/** ���������ϰ���ʱ�������������ٸ�����
		*/
		void SetMaxRoundTiles(int numTiles);
		/** Ѱ·
		*/
		bool FindPath(int x1,int z1,int x2,int z2,list<POINT>& path);

	private:
		bool FindBlocks(int x1,int z1,int x2,int z2,tagBlock* pBlocks,int& numBlocks);
		void AddPathPoint(int x1,int z1,list<POINT>& path,int pointX,int pointZ);
		void AddPathPoints(int x1,int z1,const POINT* pPoints,int numPoints,list<POINT>& path);
		int RoundBlock_CW(const tagBlock* pBlock,POINT* pTiles,int& numTiles);
		int RoundBlock_CCW(const tagBlock* pBlock,POINT* pTiles,int& numTiles);

	private:
		BitMap* m_pCangoMap;
		int m_maxRoundTiles;
	};
}//namespace WorldBase
