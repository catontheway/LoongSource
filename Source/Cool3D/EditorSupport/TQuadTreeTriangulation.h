#pragma once

namespace Cool3D
{
	class Heightmap;
	/**	\class TQuadTreeTriangulation
		\brief ʹ���Ĳ���ΪHeightMap����������list
	*/
	class TQuadTreeTriangulation
	{
	public:
		TQuadTreeTriangulation(void);
		virtual ~TQuadTreeTriangulation(void);

		/**	��������Դ
		*/
		void SetDataSrc(Heightmap *pMap,RECT rect,bool forceTri=false)	{	m_pMap=pMap; m_rect=rect; m_bForceTri=forceTri;}
		/**	����Triangle List
		*/
		void GenerateIndices(float desireResolution);
		/**	����Triangle List
			\param	Ԥ�ڵķֱ���,���ڵĸ��߷ֱ���
			\remarks �����ڵĽڵ㽫ʹ�ø��ߵķֱ���,�Ա�֤�޷�����
		*/
		void GenerateIndices(float desireResolution,float higherRes);
		/**	�õ�Index����
		*/
		UINT GetNumIndices();
		/**	�õ�Index Buffer
		*/
		WORD* GetIndices();
	protected:
		/**	�ݹ�ļ���Quad matrix
		*/
		void BuildMatrixR(RECT myRect);
		/**	�ݹ�Ĵ���triangle list,����ӵ�index buffer
		*/
		void BuildTriListR(RECT myRect);
		/**	�ݹ���޲�myRectָ��Ľڵ���������Ͽ��ܲ������ѷ�
		*/
		void FixCrackTopR(RECT myRect,vector<WORD>& fan,bool first=true);
		void FixCrackLeftR(RECT myRect,vector<WORD>& fan,bool first=true);
		void FixCrackRightR(RECT myRect,vector<WORD>& fan,bool first=true);
		void FixCrackBottomR(RECT myRect,vector<WORD>& fan,bool first=true);

		/**	��һ��Rect�зֳ�4����Rect
		*/
		void GetChildrenRect(RECT myRect,RECT *pRect);
		/**	��һ��Triangle Fan��ӵ�triangle list��index buffer��
		*/
		void AppendTriFan(vector<WORD>& fan);
		/**	����Quad matrix��ֵ
			\param x,y��Height map����ϵ
		*/
		void SetMatrix(UINT x,UINT y,BYTE val);
		/**	�õ�Quad Matrix��ֵ
			\param x,y��Height map����ϵ
		*/
		BYTE GetMatrix(UINT x,UINT y);
		/**	�õ�ָ���������Vertex index
			\param x,y��Height map����ϵ
			\remarks vertex buffer�ǽ�����m_rectָ���������ϵ�
		*/
		WORD GetVertIndex(UINT x,UINT y);

	protected:
		/**	rect��ָ��Ľڵ��Ƿ�Ҫ����ϸ��
		*/
		bool IfNeedSplit(Heightmap *pMap,RECT rect);
		/**	�õ�height map��rect��������߶ȱ仯ֵ
		*/
		float GetMaxDeltaH(Heightmap *pMap,RECT rect);
	protected:
		Heightmap		*m_pMap;
		BYTE			*m_quadMatrix;//quad tree matrix
		UINT			m_matW,
						m_matH;
		RECT			m_rect;
		vector<WORD>	m_indices;

		float			m_desireResolution;
		float			m_higherRes;
		bool			m_bForceTri;//ǿ�������λ�

	protected:
		static float m_minResolution;
	};
}//namespace Cool3D