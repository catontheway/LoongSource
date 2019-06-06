//-----------------------------------------------------------------------------
// File: Util.h
// Desc: Game System Util 2.0
// Auth: Lyp
// Date: 2003/11/13
//
// Copyright (c) 1985-2007 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once


#define UTIL_PROBABILITY_MOD	8192	// ����ȡģ��
//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
class Util
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
	
	// Win98����ϵͳ��IntersectionRect��BUG, ����������дһ��
	BOOL	RectIntersection(RECT& rcOut, RECT rc1, RECT rc2);

	// ����32λCRC,�㷨����
	DWORD	Crc32(LPCSTR lpData);	// ר���������ַ���
	DWORD	Crc32(LPCTSTR pData);	// ר���������ַ���
	DWORD	Crc32(LPBYTE pData, INT nByteCount);

	// unicode��unicode8ת��
	// ʹ��ʱע��ʹ�õ��ǹ���buff,�����ε��ú����ݸ���
	LPCSTR	UnicodeToUnicode8(LPCTSTR szSrc);
	LPCSTR	UnicodeToAnsi(LPCTSTR szSrc);
	LPCTSTR	Unicode8ToUnicode(LPCSTR szSrc);
	LPCTSTR	AnsiToUnicode(LPCSTR szSrc);

	// ȥ��tstring��β�Ŀո�
	VOID	CutSpaceOfStringHeadAndTail(tstring& string);
	// �������ַ�תΪtoken;
	VOID	StringToToken(std::vector<tstring>& token, LPCWSTR szString, WCHAR chSeparator=_T(' '));

	// ���ע�����ָ����ֵ�Ƿ����
	BOOL CheckReg(HKEY key, LPCTSTR subKey);
	// ��ָ����ֵд��ע�����ָ��·��,���ָ��·��û��,�򴴽���д��
	BOOL WriteReg(LPCTSTR KEY_ROOT/*ע����е�·��*/, LPCTSTR KEY_NAME, LPCTSTR sz);
	// ��ȡע�����ָ��·���ļ�ֵ
	BOOL ReadReg(LPCTSTR KEY_ROOT, LPCTSTR KEY_NAME, DWORD& key_value);
	BOOL ReadReg(LPCTSTR KEY_ROOT, LPCTSTR KEY_NAME, LPTSTR outstr);

	// �õ����̵�ʣ��ռ�
	INT64 GetDriveFreeSpace(LPCTSTR szDriver);

	Util();
	~Util();

private:

	CRITICAL_SECTION	m_Lock;

	DWORD	m_dwIDHolder;		// ��¼IDʹ��״̬
	INT		m_nRandHolder;
	INT		m_nRandCallCount;
	CHAR*	m_pCharPool;
	TCHAR*	m_pTCharPool;
};



