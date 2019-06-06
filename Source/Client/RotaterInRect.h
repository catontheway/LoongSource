#pragma once

#include <list>
using namespace std;

/** �ھ����е���ת��
*/
class RotaterInRect 
{
	struct PNT{
		short x;
		short y;		
	};
public:
	RotaterInRect();
	~RotaterInRect();

	int GetPointList( float fRatio, list<POINT>& list, int nLength );	// ���ݻ��ȣ�ȡ������б� ͨ��nLength/m_nLength�������
	void InitSquare();													// �˷���Ŀǰ��������������
private:
	int	GetPoint( float fRadian );										// ���ݻ��ȣ�ȡ��Ӧ������㡣
	void SetRect( short nWidth, short nHeight );						// ���þ��Σ�������Ӧ����㡣
																		// ����ͨ��С�����Σ������ʼ���������εĽǶ�ֵ

	POINT CalcPOINT( const PNT& pnt, int nLength ){
		POINT p;
		p.x = pnt.x * nLength / m_nLength;
		p.y = pnt.y * nLength / m_nLength;
		return p;
	}
	
private:
	int			m_nCount;		// �����С
	PNT			m_pPt[360];		// ��ȴ״̬ʱ��
	int			m_nLength;		// �����α߳�
};
