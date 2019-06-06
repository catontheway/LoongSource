#pragma once

const INT X_STRENGTH_RANKINGS_LEN = 500;

struct tagStrength
{
	DWORD			dwValue;			// ʵ��ֵ
	DWORD			m_dwHeroism;		// Ӣ��ֵ
	DWORD			m_dwWit;			// ı��ֵ
	DWORD			m_dwErrantry;		// ����ֵ
	DWORD			m_dwValor;			// ��սֵ

	tagStrength()
	{
		dwValue = m_dwErrantry + m_dwHeroism + m_dwValor + m_dwWit;
	}
};

struct tagStrengthRoleNode
{
	INT		nNum;						// �ý�ɫ����	
	INT		nPreNum;					// �ý�ɫ�ϴ�����	
	TCHAR	szRoleName[X_SHORT_NAME];	// ��ɫ����
	DWORD	dwRoleID;					// ��ɫID
	DWORD	dwStrength;					// �ý�ɫ��ʵ��ֵ
};
