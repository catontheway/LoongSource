#pragma once
#include "..\Math\Math.h"

namespace F3D
{
	class VertStream;

	/**	\class EdgeCollapse
		\brief ʹ�ñ����ݵķ�������Mesh
	*/
	class EdgeCollapse
	{
		struct tagEdge
		{
			WORD		index[2];	//��������ߵ��������������
			ste<WORD>	faces;	//�����������ߵ�����face
			float		ccost[2];	//����������������ֱ�ļ򻯴���,d[0]Ϊvert0�ƶ���vert1
		};
		struct tagTriFace
		{
			WORD	index[3];	//�����������ε��������������
			Vector3	normal;		//��λ������
			bool	bRemoved;	//�Ƿ��Ѿ����򻯵���
		};
		struct tagVertShare
		{
			set<WORD>		faces;	//������������������
			Vector3			normal;	//��������ƽ��������
		};
	public:
		EdgeCollapse(void);
		virtual ~EdgeCollapse(void);

		void BuildTriListMesh(VertStream *pVertStream,WORD *pIndices,UINT numTri);

		/**	С��ĳ���򻯴��۵ı߱�ѹ��,�Լ�Mesh
			\param ���Ŀɼ򻯴���
		*/
		void Simplify(float maxCost);
		void GetIndices(vector<WORD>& indices);

	protected:
		/**	ѡ��򻯴�����С��һ����,���м�
			\remarks ɾ��һ����,һ������,���face
		*/
		void CollaspeOne();
		/**	����ߵĹ���face��vert�Ĺ���face
		*/
		void CalMeshTopo();
		/**	����һ��������������ļ򻯴���
		*/
		void CalCost(tagEdge& edge);

	protected:
		VertStream				*m_pVertStream;
		vector<tagEdge>			m_edgeArray;	//ԭʼMesh�ı�����
		vector<tagTriFace>		m_faceArray;	//ԭʼMesh��������
		vector<tagVertShare>	m_vertArray;	//���㹲����Ϣ����
	};
}//namespace F3D