//-----------------------------------------------------------------------------
// File: Util.h
// Desc: Game System Util 2.0
// Auth: Lyp
// Date: 2003/11/13
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {


#define UTIL_PROBABILITY_MOD	8192	// ����ȡģ��
//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
class VENGINE_API Util
{
public:
	DWORD	GetFreeID();
	
	// �����
	VOID	Randomize(DWORD dwSeed);
	DWORD	GetRandHolder() { return m_nRandHolder; }
	INT		Rand();

	// �������ʰٷ��������Ƿ�����
	BOOL	Probability(INT nProbCent);
	// ����������ֵ
	FLOAT	Fluctuate(FLOAT fNum, FLOAT fIncPercent, FLOAT fDecPercent);

	// ������ȡ�м�ֵ
	INT		RandomInRange(INT nMin, INT nMax);
	
	// Win98����ϵͳ��IntersectionRect��BUG, ����������дһ��
	BOOL	RectIntersection(RECT& rcOut, RECT rc1, RECT rc2);

	// ����32λCRC,�㷨����
	__forceinline DWORD	Crc32(LPCSTR lpData);	// ר���������ַ���
	__forceinline DWORD	Crc32(LPCTSTR pData);	// ר���������ַ���
	__forceinline DWORD	Crc32(LPBYTE pData, INT nByteCount);
	// ����16λCRC from quake2, �㷨û�������Crc32��
	WORD	Crc16(PBYTE pData, INT nByteCount);

	// unicode��unicode8ת��
	// ʹ��ʱע��ʹ�õ��ǹ���buff,�����ε��ú����ݸ���
	__forceinline LPCSTR	UnicodeToUnicode8(LPCTSTR szSrc, LPSTR szOut=NULL);
	__forceinline LPCTSTR	Unicode8ToUnicode(LPCSTR szSrc, LPTSTR szOut=NULL);
	__forceinline LPCSTR	UnicodeToAnsi(LPCTSTR szSrc);
	__forceinline LPCTSTR	AnsiToUnicode(LPCSTR szSrc);

	// ȥ��tstring��β�Ŀո�
	VOID CutSpaceOfStringHeadAndTail(tstring& string);
	// �������ַ�תΪtoken;
	VOID StringToToken(std::vector<tstring>& token, LPCWSTR szString, WCHAR chSeparator=_T(' '));

	// ���ע�����ָ����ֵ�Ƿ����
	BOOL CheckReg(HKEY key, LPCTSTR subKey);
	// ��ָ����ֵд��ע�����ָ��·��,���ָ��·��û��,�򴴽���д��
	BOOL WriteReg(LPCTSTR KEY_ROOT/*ע����е�·��*/, LPCTSTR KEY_NAME, LPCTSTR sz);
	// ��ȡע�����ָ��·���ļ�ֵ
	BOOL ReadReg(LPCTSTR KEY_ROOT, LPCTSTR KEY_NAME, DWORD& key_value);
	BOOL ReadReg(LPCTSTR KEY_ROOT, LPCTSTR KEY_NAME, LPTSTR outstr);


	// ��㵽ֱ�ߵľ��� (fX,fY)�ǵ㣬(fX1,fY1)-(fX2,fY2)��ֱ��
	FLOAT Point2LineDist(FLOAT fX, FLOAT fY, FLOAT fX1, FLOAT fY1, FLOAT fX2, FLOAT fY2);

	// IsDebuggerPresent��ҪWinNT4.0����ϵͳ���У��������������Լ�ʵ��һ��
	bool IsDebuggerPresent();

public:
	// ��ȡ�����ļ�����·��(���ܻ���δ����)
	BOOL GetIniPath( LPTSTR szPath, LPTSTR pFilePathPart );

	// ���ṩ����Կ��ĳ���ڴ�������м��ܣ�����ڴ�Ҫ����8�ֽڵ���������
	BOOL Encrypt(VOID* pIn, VOID* pOut, const INT nLen, UCHAR* key);
	// ���ṩ����Կ��ĳ���ڴ�������н��ܣ�����ڴ�Ҫ����8�ֽڵ���������
	BOOL Decrypt(VOID* pIn, VOID* pOut, const INT nLen, UCHAR* key);
	// ��ĳ��������ļ��ڴ���н��ܣ���Կ�������Կ�����е�һ������
	BOOL Encrypt(VOID* pIn, VOID* pOut, const INT nLen, const INT nIndex, const INT nMethod = 0);
	BOOL Decrypt(VOID* pIn, VOID* pOut, const INT nLen, const INT nIndex, const INT nMethod = 0);

private:
	// 3des�ӽ���ͨ�ú�����bEncypt�����Ƿ���ܣ�
	BOOL Crypt(UCHAR* pIn, UCHAR* pOut, const INT nLen, UCHAR* key, bool bEncypt=0);

public:
	VOID	MD5ForBuffer(BYTE* pBuffer, UINT uBufferLen, BYTE digest[16]);
	VOID	MD5ForString(CHAR* szString, BYTE digest[16]);
	BOOL	MD5ForFile(LPCTSTR szFileName, BYTE digest[16]);
	VOID	MD5ToString(std::string& str, BYTE digest[16]);

	Util();
	~Util();

private:

	CRITICAL_SECTION	m_Lock;

	DWORD	m_dwIDHolder;		// ��¼IDʹ��״̬
	INT		m_nRandHolder;
	INT		m_nRandCallCount;
	CHAR*	m_pCharPool;
	TCHAR*	m_pTCharPool;


	typedef struct 
	{
		DWORD	state[4];		/* state (ABCD) */
		DWORD	count[2];		/* number of bits, modulo 2^64 (lsb first) */
		BYTE	buffer[64];		/* input buffer */
	} MD5_CTX;
	MD5_CTX	m_context;


	void	MD5Init();
	void	MD5Update(BYTE *pInput, UINT uInputLen);
	void	MD5Final(BYTE digest[16]);

	void	MD5Transform (DWORD[4], unsigned char[64]);
	//��UINT4���ֽ�ת��ΪBYTE
	void	Encode(BYTE* pOutput, DWORD* pInput, unsigned int);
	//��BYTE���ֽ�ת��ΪUINT4
	void	Decode(DWORD* pOutput, BYTE* pInput, unsigned int);

};


#define CHAR_POOL_SIZE	1024*64
#define MASKBITS 0x3F
#define MASKBYTE 0x80
#define MASK2BYTES 0xC0
#define MASK3BYTES 0xE0
//-----------------------------------------------------------------------------
// ��unicodeת��unicode8
//-----------------------------------------------------------------------------
LPCSTR Util::UnicodeToUnicode8(LPCTSTR szSrc, LPSTR szOut)
{
	if( !szOut )
		szOut = m_pCharPool;
	LPSTR szDest = szOut;

	while( *szSrc )
	{
		if(*szSrc < 0x80)	// 0xxxxxxx
		{
			*szDest++ = (BYTE)*szSrc;
		}
		else if( *szSrc < 0x800 )	// 110xxxxx 10xxxxxx
		{
			*szDest++ = ((byte)(MASK2BYTES | *szSrc >> 6));
			*szDest++ = ((byte)(MASKBYTE | *szSrc & MASKBITS));
		}
		else	// 1110xxxx 10xxxxxx 10xxxxxx
		{
			*szDest++ = ((byte)(MASK3BYTES | *szSrc >> 12));
			*szDest++ = ((byte)(MASKBYTE | *szSrc >> 6 & MASKBITS));
			*szDest++ = ((byte)(MASKBYTE | *szSrc & MASKBITS));
		}
		++szSrc;
	}

	*szDest = 0; // ����0������
	return szOut;
}


//-----------------------------------------------------------------------------
// ��unicode8ת��unicode
//-----------------------------------------------------------------------------
LPCTSTR	Util::Unicode8ToUnicode(LPCSTR szSrc, LPTSTR szOut)
{
	if( !szOut )
		szOut = m_pTCharPool;

	LPTSTR szDest = szOut;

	while( *szSrc )
	{
		if( !(*szSrc & MASKBYTE) )	// 0xxxxxxx
		{
			*szDest++ = *szSrc++;
		}
		else if( (*szSrc & MASK3BYTES) == MASK3BYTES )	// 1110xxxx 10xxxxxx 10xxxxxx
		{
			*szDest++ = ((*szSrc & 0x0F) << 12) | ((*(szSrc+1) & MASKBITS) << 6) | (*(szSrc+2) & MASKBITS);
			++szSrc;
			++szSrc;
			++szSrc;
		}
		else if( (*szSrc & MASK2BYTES) == MASK2BYTES )	// 110xxxxx 10xxxxxx
		{
			*szDest++ = ((*szSrc & 0x1F) << 6) | (*(szSrc+1) & MASKBITS);
			++szSrc;
			++szSrc;
		}
	}

	*szDest = 0; // ����0������
	return szOut;
}


//-----------------------------------------------------------------------------
// ��unicodeת��ANSI
//-----------------------------------------------------------------------------
LPCSTR Util::UnicodeToAnsi(LPCTSTR szSrc)
{
	m_pCharPool[0] = 0;
	WideCharToMultiByte(CP_ACP, 0, szSrc, -1, m_pCharPool, CHAR_POOL_SIZE, NULL, NULL);
	return m_pCharPool;
}

//-----------------------------------------------------------------------------
// ��ANSIת��unicode
//-----------------------------------------------------------------------------
LPCTSTR Util::AnsiToUnicode(LPCSTR szSrc)
{
	m_pCharPool[0] = 0;
	MultiByteToWideChar(CP_ACP, 0, szSrc, -1, m_pTCharPool, CHAR_POOL_SIZE);
	return m_pTCharPool;
}

//-----------------------------------------------------------------------------
// ����32λCRC,��һ���㷨���ٺܶ�
//-----------------------------------------------------------------------------
DWORD Util::Crc32(LPCSTR lpData)
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

//-----------------------------------------------------------------------------
// ����32λCRC,��һ���㷨���ٺܶ�
//-----------------------------------------------------------------------------
DWORD Util::Crc32(LPCTSTR lpData)
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
// ����32λCRC,��һ���㷨���ٺܶ�
//-----------------------------------------------------------------------------
DWORD Util::Crc32(LPBYTE pData, INT nByteCount)
{
	if( nByteCount <= 0 )
		return (DWORD)GT_INVALID;

	UINT unResult = *pData++ << 24;
	if( --nByteCount > 0 )
	{
		unResult |= *pData++ << 16;
		if( --nByteCount > 0 )
		{
			unResult |= *pData++ << 8;
			if( --nByteCount > 0 ) unResult |= *pData++;
		}
	}
	unResult = ~ unResult;
	while( --nByteCount > 0 )
	{
		unResult = (unResult << 8 | *pData) ^ globe_Crc32Table[unResult >> 24];
		pData++;
	}

	return ~unResult;
}


} // namespace vEngine {
