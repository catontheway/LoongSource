//--------------------------------------------------------------------------------------------------
// File: StreamPool.h
// Desc: Mysql SQL statement pool
// Time: 2008-05-05
// Auth: Aslan
//
// Copyright (c) 2008 Tengreng All rights reserved.
//---------------------------------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "Config.h"
#include "MysqlStream.h"
#include "SafeStreamQueue.h"

namespace Beton {

class MyStream;
class SafeStreamQueue;
//----------------------------------------------------------------------------------------------------
/**	StreamPool�� - Ϊ�ϲ��ṩMysqlStream�أ����ϲ���ȡʹ��
1. Ԥ�ȷ���һ����С����Դ���粻��ɶ�̬���䣬�������࣬���ڲ���Ҫʱ�ͷŶ����ڴ�
2. �̰߳�ȫ
*/
//-----------------------------------------------------------------------------------------------------
class BETON_API StreamPool
{
public:
	StreamPool(INT nDefaultSize=1000);
	virtual ~StreamPool();

private:
	StreamPool(const StreamPool&);
	StreamPool& operator = (const StreamPool&);

public:
	BETON_INLINE MyStream* AllocStream();
	BETON_INLINE VOID FreeStream(MyStream* pStream);

	BETON_INLINE LONG GetAvailableStreamNum() { return m_FreeQueue.Size(); }
	BETON_INLINE INT GetAllStreamNum() { return m_nTotalNum; }

private:
	BETON_INLINE VOID RealAlloc(INT nNum);
	BETON_INLINE VOID RealFree(INT nNum);
	BETON_INLINE VOID RealFreeAll();

private:
	Mutex				m_Mutex;
	SafeStreamQueue		m_FreeQueue;
	VOLATILE INT		m_nDefaultSize;		// ������ʱ��Ĭ�ϴ�С
	VOLATILE INT		m_nTotalNum;		// �ó�һ�������˶��ٸ�stream
};

} // namespace Beton