//-----------------------------------------------------------------------------
//!\file unit_pool.h
//!\author Lyp
//!
//!\date 2004-07-01
//! last 2004-07-06
//!
//!\brief ��ɶ˿�����ͨѶ�ײ��ڴ��
//-----------------------------------------------------------------------------
#pragma once
#include "safe_unit_queue.h"

namespace vEngine {

#ifdef _DEBUG_UNIT
	extern TSafeMap<tagUnitData*, tagUnitData*>	mapUnitAddress;	// for DEBUG
#endif // _DEBUG_UNIT
//-----------------------------------------------------------------------------
//!\brief ��ɶ˿�����ͨѶ�ײ��ڴ��
//!	���ƶ��壺���ÿ��unit, ������1����������Ϊwindows�����wbuf
//-----------------------------------------------------------------------------
class VENGINE_API UnitPool
{
public:
	// nUnitNum = ��ʼ��ʱ��ʵ��alloc����unit
	BOOL Init(INT nUnitNum);
	BOOL Destroy();

	// ʧ�ܷ���-1,�ɹ�����unit index
	__forceinline tagUnitData*	AllocUnit();	
	__forceinline VOID			FreeUnit(tagUnitData*);

	// ���Խ���������
	__forceinline tagUnitData*	TryAllocUnit();
	__forceinline BOOL			TryFreeUnit(tagUnitData*);

	// �õ���ǰunit����
	INT	GetSize() { return m_FreeUnitQueue.GetNum(); }
	// �õ��ӱ���һ��ʵ��alloc��unit����,�����ڲ�alloc�ĵ��ⲿδ�����
	INT	GetTotalNum()	{ return m_nTotalNum; }


	UnitPool();
	~UnitPool();

private:
	SafeQueue<tagUnitData*>	m_FreeUnitQueue;	// ����
	VOLATILE INT			m_nDefaultNum;
	VOLATILE INT			m_nTotalNum;

	BOOL		RealAlloc(INT nUnitNum);
	BOOL		RealFree(INT nUnitNum);
};


//-----------------------------------------------------------------------------
// ʧ�ܷ���NULL
//-----------------------------------------------------------------------------
tagUnitData* UnitPool::AllocUnit()
{
	tagUnitData* pUnit = m_FreeUnitQueue.Get();
	while( NULL == pUnit )
	{
		RealAlloc(m_nDefaultNum);	// û���ˣ�����������һ��
		pUnit = m_FreeUnitQueue.Get();
	}

	pUnit->eState = EUS_Null;

#ifdef _DEBUG_UNIT
	mapUnitAddress.Add(pUnit, pUnit) ;	// FOR DEBUG
#endif

	return pUnit;
}


//-----------------------------------------------------------------------------
// free unit
//-----------------------------------------------------------------------------
VOID UnitPool::FreeUnit(tagUnitData* pUnit)
{
	m_FreeUnitQueue.Add(pUnit);

#ifdef _DEBUG_UNIT
	mapUnitAddress.Erase(pUnit);	// FOR DEBUG
#endif

	// ���е�̫��,�����ڴ�
	INT n = m_FreeUnitQueue.GetNum();
	if( n > m_nDefaultNum * 8 )
		RealFree(n/4);
}


//-----------------------------------------------------------------------------
// ���Խ���������,alloc unit
//-----------------------------------------------------------------------------
tagUnitData* UnitPool::TryAllocUnit()
{
	tagUnitData* pUnit = NULL;
	if( m_FreeUnitQueue.GetNum() <=0 )
	{
		// û���ˣ�����������һ��
		this->RealAlloc(m_nDefaultNum);
	}

	pUnit = m_FreeUnitQueue.TryGet();
	
	if( pUnit )
	{
		pUnit->eState = EUS_Null;

#ifdef _DEBUG_UNIT
		mapUnitAddress.Add(pUnit, pUnit) ;	// FOR DEBUG
#endif
	}
	
	return pUnit;
}


//-----------------------------------------------------------------------------
// ���Խ���������,free unit
//-----------------------------------------------------------------------------
BOOL UnitPool::TryFreeUnit(tagUnitData* pUnit)
{
	if( FALSE == m_FreeUnitQueue.TryAdd(pUnit) )
		return FALSE;

	// ���е�̫��,�����ڴ�
	INT n = m_FreeUnitQueue.GetNum();
	if( n > m_nDefaultNum * 8 )
		RealFree(n/4);


#ifdef _DEBUG_UNIT
	mapUnitAddress.Erase(pUnit) ;	// FOR DEBUG
#endif

	return TRUE;
}




} // namespace wEngine {
