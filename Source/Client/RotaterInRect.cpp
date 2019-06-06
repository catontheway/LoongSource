#include "stdafx.h"
#include "RotaterInRect.h"

RotaterInRect::RotaterInRect()
{
	m_nCount = 0;
}

RotaterInRect::~RotaterInRect()
{
}

void RotaterInRect::InitSquare( )
{
	m_nLength = 500;
	SetRect( short( m_nLength/2 ), short( m_nLength/2) );
}

void RotaterInRect::SetRect( short nW, short nH )
{
	ASSERT( nW * nH > 0 );
	

	// ��360����
	m_nCount = 360; 

	//  ����
	int nIndex1		 = 0;
	m_pPt[nIndex1].x =  0;
	m_pPt[nIndex1].y =  -nH;

	// �����һ�����ڵ�ֵ
	// �Ҷ���Ϊ�ڽ��
	int nIndex2 = m_nCount/8; 
	for( int i=nIndex1+1; i<nIndex2; i++ )
	{
		float fRadian = FLOAT_PI * 2 * i / m_nCount;
		m_pPt[i].x = short( nH * tan( fRadian ) );
		m_pPt[i].y = short( -nH );
	}

	// �Ҷ��������
	m_pPt[nIndex2].x = short( nW );
	m_pPt[nIndex2].y = short( -nH );

	nIndex1 = nIndex2;
	nIndex2 = m_nCount/4; 
	for( int i=nIndex1+1; i<nIndex2; i++ )
	{
		float fRadian = FLOAT_PI * 2 * i / m_nCount;
		m_pPt[i].x = nW;
		m_pPt[i].y = short( float(-nW)/tan( fRadian ) );
	}

	m_pPt[nIndex2].x = nW;
	m_pPt[nIndex2].y = 0;
	// end �����һ�����ڵ�ֵ

	// ������������ֵ���öԳƷ�ʽȡ��
	// ���������ڵ�ֵ
	nIndex1 = nIndex2;
	nIndex2 = m_nCount / 2;
	for( int i=nIndex1+1; i<=nIndex2; i++ )
	{
		m_pPt[i].x = m_pPt[nIndex2-i].x;
		m_pPt[i].y = -m_pPt[nIndex2-i].y;
	}

	// �ڶ��������ڵ�ֵ
	nIndex1 = nIndex2;
	nIndex2 = m_nCount;
	for( int i=nIndex1+1; i<m_nCount; i++ )
	{
		m_pPt[i].x = -m_pPt[nIndex2-i].x;
		m_pPt[i].y = m_pPt[nIndex2-i].y;
	}

}

int RotaterInRect::GetPoint( float fRatio )
{
	ASSERT( fRatio < 1.0f );

	int nIndex = int( fRatio * m_nCount );

	if( nIndex >= m_nCount )
		nIndex = m_nCount-1;
	if( nIndex <= 0 )
		nIndex = 1;

	return nIndex;
}


int RotaterInRect::GetPointList( float fRatio, list<POINT>& list, int nLength )
{
	list.clear();

	int nIndex = GetPoint( fRatio );

	list.push_back( CalcPOINT( m_pPt[0], nLength ) );	// ���ȵ�

	// �۵�
	int nCornerIndex[4] = { m_nCount*7/8, m_nCount*5/8, m_nCount*3/8, m_nCount/8 };
	for( int i=0; i<4; i++)
	{
		if( nIndex < nCornerIndex[i] )
		{
			PNT& pnt = m_pPt[ nCornerIndex[i] ];
			list.push_back( CalcPOINT( pnt, nLength )  );
		}
	}

	list.push_back( CalcPOINT( m_pPt[nIndex], nLength ) );	// ĩ��

	return 1;
}