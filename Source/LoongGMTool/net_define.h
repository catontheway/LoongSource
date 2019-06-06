#pragma once

CONST INT GT_NET_BLOCK_TIME = 1000*150;	// 150 ms

//-----------------------------------------------------------------------------
// ����32λCRC,��һ���㷨���ٺܶ�
//-----------------------------------------------------------------------------
static DWORD Crc32(LPCSTR lpData)
{
	//��ASC����127���ַ��� ������unsigned char, ��������
	LPBYTE pData = (LPBYTE)lpData;
	if( 0 == pData[0] ) 
		return (DWORD)GT_INVALID;

	UINT unResult = *pData++ << 24;
	if( *pData )
	{
		unResult |= *pData++ << 16;
		if( *pData )
		{
			unResult |= *pData++ << 8;
			if( *pData ) unResult |= *pData++;
		}
	}
	unResult = ~ unResult;
	while( *pData )
	{
		unResult = (unResult << 8 | *pData) ^ globe_Crc32Table[unResult >> 24];
		pData++;
	}
	return ~unResult;
}

static DWORD Crc32(LPCTSTR lpData)
{
	LPBYTE pData = (LPBYTE)lpData;
	if( !(*pData || *(pData+1)) ) 
		return (DWORD)GT_INVALID;

	UINT unResult = *pData++ << 24;
	unResult |= *pData++ << 16;
	if( *pData || *(pData+1) )
	{
		unResult |= *pData++ << 8;
		unResult |= *pData++;
	}
	unResult = ~ unResult;
	INT nCount = 0;
	while( (nCount&1) ? true : (*pData || *(pData+1)) )
	{
		unResult = (unResult << 8 | *pData) ^ globe_Crc32Table[unResult >> 24];
		++pData;
		++nCount;
	}

	return ~unResult;
}


//-----------------------------------------------------------------------------
// ������Ϣ�����ṹ
//-----------------------------------------------------------------------------
struct tagNetCmd
{
	DWORD	dwID;
	DWORD	dwSize;

	DWORD Crc32(LPCSTR lpData)
	{
		UINT unResult;
		LPBYTE pData = (LPBYTE)lpData;
		if( 0 == pData[0] ) 
			return (DWORD)GT_INVALID;
		unResult = *pData++ << 24;
		if( *pData )
		{
			unResult |= *pData++ << 16;
			if( *pData )
			{
				unResult |= *pData++ << 8;
				if( *pData ) unResult |= *pData++;
			}
		}
		unResult = ~ unResult;
		while( *pData )
		{
			unResult = (unResult << 8 | *pData) ^ globe_Crc32Table[unResult >> 24];
			pData++;
		}
		return ~unResult;
	}
};

//-----------------------------------------------------------------------------
// ����������Ϣ�Ķ���
//-----------------------------------------------------------------------------

#ifndef CMD_START
#define CMD_START(name)	\
struct tag##name : public tagNetCmd{	\
	tag##name(){ ZeroMemory(this,sizeof(*this));  \
	dwID = Crc32(#name); dwSize = sizeof(*this); }
#define CMD_END		};
#endif

