//-----------------------------------------------------------------------------
// File: sfp_trunk
// Auth: Lyp
// Date: 2004/2/1
// Last: 2004/7/19
//
// Desc: ģ����: ����̬���ɵľ�̬�����ӿ�ָ����ͨ��ĳ�Ա����
// Copyright (c) 1985-2007 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

CONST INT MAX_SFP_CODE_SIZE	= 40;
//-----------------------------------------------------------------------------
// template TSFPTrunk
// ע�⣺��ģ���֧��IA32 CPU�Լ�VC++ compiler
//-----------------------------------------------------------------------------
template<typename your_class>
class TSFPTrunk
{
// 1DWORD���������������̺߳���
// 4DWORD�������������ڴ�����Ϣ������
typedef DWORD (your_class::* sfp_FP0)();
typedef DWORD (your_class::* sfp_FP1)(DWORD);
typedef DWORD (your_class::* sfp_FP2)(DWORD, DWORD);
typedef DWORD (your_class::* sfp_FP3)(DWORD, DWORD, DWORD);
typedef DWORD (your_class::* sfp_FP4)(DWORD, DWORD, DWORD, DWORD);

public:

	template<typename T> 
	DWORD (WINAPI* sfp0(T f))()	// �������޲����ĺ���
	{
		DWORD dwID = *(DWORD*)(VOID*)&f;
		std::map<DWORD, DWORD>::iterator it = m_mapFP.find(dwID);
		if( it != m_mapFP.end() )	// �ȴӱ�������
			return (DWORD(WINAPI*)())it->second;

		LPBYTE pByte = new BYTE[MAX_SFP_CODE_SIZE];
		if( pByte == NULL )
			return NULL;

		m_RealCodeList.push_back(pByte);
		m_fp0List.push_back((sfp_FP0)f);
		m_mapFP.insert(std::pair<DWORD, DWORD>(dwID, (DWORD)pByte));

		memcpy( pByte, m_Code0, sizeof(m_Code0));
		*(DWORD*)&pByte[2] = (DWORD)&m_pObj;
		*(DWORD*)&pByte[10] = (DWORD)&m_fp0List.back();
		return (DWORD(WINAPI*)())pByte;
	}

	template<typename T>
	DWORD (WINAPI* sfp1(T f))(DWORD)	// ������1�����ĺ���
	{
		DWORD dwID = *(DWORD*)(VOID*)&f;
		std::map<DWORD, DWORD>::iterator it = m_mapFP.find(dwID);
		if( it != m_mapFP.end() )	// �ȴӱ�������
			return (DWORD(WINAPI*)(DWORD))it->second;

		LPBYTE pByte = new BYTE[MAX_SFP_CODE_SIZE];
		if( pByte == NULL )
			return NULL;

		m_RealCodeList.push_back(pByte);
		m_fp1List.push_back((sfp_FP1)f);
		m_mapFP.insert(std::pair<DWORD, DWORD>(dwID, (DWORD)pByte));

		memcpy( pByte, m_Code1, sizeof(m_Code1));
		*(DWORD*)&pByte[2] = (DWORD)&m_pObj;
		*(DWORD*)&pByte[14] = (DWORD)&m_fp1List.back();
		
		return (DWORD(WINAPI*)(DWORD))pByte;
	}

	// ������1�����ĺ���,����cdecl�ͺ���,��ҪΪLua�ű������ṩ
	template<typename T>
	DWORD (__cdecl* sfp1_cdecl(T f))(DWORD)	
	{
		DWORD dwID = *(DWORD*)(VOID*)&f;
		std::map<DWORD, DWORD>::iterator it = m_mapFP.find(dwID);
		if( it != m_mapFP.end() )	// �ȴӱ�������
			return (DWORD(__cdecl*)(DWORD))it->second;

		LPBYTE pByte = new BYTE[MAX_SFP_CODE_SIZE];
		if( pByte == NULL )
			return NULL;

		m_RealCodeList.push_back(pByte);
		m_fp1List.push_back((sfp_FP1)f);
		m_mapFP.insert(std::pair<DWORD, DWORD>(dwID, (DWORD)pByte));

		memcpy( pByte, m_Code1, sizeof(m_Code1));
		*(DWORD*)&pByte[2] = (DWORD)&m_pObj;
		*(DWORD*)&pByte[14] = (DWORD)&m_fp1List.back();
		pByte[19] = 0;
		
		return (DWORD(__cdecl*)(DWORD))pByte;
	}

	template<typename T>
	DWORD (WINAPI* sfp2(T f))(DWORD, DWORD)	// ������2�����ĺ���
	{
		DWORD dwID = *(DWORD*)(VOID*)&f;
		std::map<DWORD, DWORD>::iterator it = m_mapFP.find(dwID);
		if( it != m_mapFP.end() )	// �ȴӱ�������
			return (DWORD(WINAPI*)(DWORD,DWORD))it->second;

		LPBYTE pByte = new BYTE[MAX_SFP_CODE_SIZE];
		if( pByte == NULL )
			return NULL;

		m_RealCodeList.push_back(pByte);
		m_fp2List.push_back((sfp_FP2)f);
		m_mapFP.insert(std::pair<DWORD, DWORD>(dwID, (DWORD)pByte));

		memcpy( pByte, m_Code2, sizeof(m_Code2));
		*(DWORD*)&pByte[10] = (DWORD)&m_pObj;
		*(DWORD*)&pByte[16] = (DWORD)&m_fp2List.back();
		return (DWORD(WINAPI*)(DWORD,DWORD))pByte;
	}

	template<typename T>
	DWORD (WINAPI* sfp3(T f))(DWORD, DWORD, DWORD)	// ������3�����ĺ���
	{
		DWORD dwID = *(DWORD*)(VOID*)&f;
		std::map<DWORD, DWORD>::iterator it = m_mapFP.find(dwID);
		if( it != m_mapFP.end() )	// �ȴӱ�������
			return (DWORD(WINAPI*)(DWORD,DWORD,DWORD))it->second;

		LPBYTE pByte = new BYTE[MAX_SFP_CODE_SIZE];
		if( pByte == NULL )
			return NULL;

		m_RealCodeList.push_back(pByte);
		m_fp3List.push_back((sfp_FP3)f);
		m_mapFP.insert(std::pair<DWORD, DWORD>(dwID, (DWORD)pByte));

		memcpy( pByte, m_Code3, sizeof(m_Code3));
		*(DWORD*)&pByte[18] = (DWORD)&m_pObj;
		*(DWORD*)&pByte[24] = (DWORD)&m_fp3List.back();
		return (DWORD(WINAPI*)(DWORD,DWORD,DWORD))pByte;
	}

	template<typename T>
	DWORD (WINAPI* sfp4(T f))(DWORD, DWORD, DWORD, DWORD)	// ������4�����ĺ���
	{
		DWORD dwID = *(DWORD*)(VOID*)&f;
		std::map<DWORD, DWORD>::iterator it = m_mapFP.find(dwID);
		if( it != m_mapFP.end() )	// �ȴӱ�������
			return (DWORD(WINAPI*)(DWORD,DWORD,DWORD,DWORD))it->second;

		LPBYTE pByte = new BYTE[MAX_SFP_CODE_SIZE];
		if( pByte == NULL )
			return NULL;

		m_RealCodeList.push_back(pByte);
		m_fp4List.push_back((sfp_FP4)f);
		m_mapFP.insert(std::pair<DWORD, DWORD>(dwID, (DWORD)pByte));

		memcpy( pByte, m_Code4, sizeof(m_Code4));
		*(DWORD*)&pByte[20] = (DWORD)&m_pObj;
		*(DWORD*)&pByte[28] = (DWORD)&m_fp4List.back();

		return (DWORD(WINAPI*)(DWORD,DWORD,DWORD,DWORD))pByte;
	}

	VOID sfp_clear()	// ������м�¼�ĺ���
	{
		m_fp0List.clear();
		m_fp1List.clear(); 
		m_fp2List.clear();
		m_fp3List.clear();
		m_fp4List.clear();

		std::list<LPBYTE>::iterator iter;
		for( iter=m_RealCodeList.begin(); iter!=m_RealCodeList.end(); iter++ )
			delete[](*iter);

		m_RealCodeList.clear();
		m_mapFP.clear();
	}	

	VOID operator=(your_class* pThis) { m_pObj=pThis; }

	// �ȿ��ɴ�ģ��̳У�Ҳ�ɽ�����Ϊ��Ա
	TSFPTrunk()
	{ m_pObj = (your_class*)this; }

	TSFPTrunk(your_class* pThis)
	{ m_pObj = pThis; }

	~TSFPTrunk()
	{ sfp_clear();	}

private:
	your_class*		m_pObj;

	std::list<sfp_FP0>	m_fp0List;
	std::list<sfp_FP1>	m_fp1List;
	std::list<sfp_FP2>	m_fp2List;
	std::list<sfp_FP3>	m_fp3List;
	std::list<sfp_FP4>	m_fp4List;
	std::list<LPBYTE>	m_RealCodeList;

	std::map<DWORD, DWORD>	m_mapFP;

	static CONST BYTE m_Code0[MAX_SFP_CODE_SIZE];
	static CONST BYTE m_Code1[MAX_SFP_CODE_SIZE];
	static CONST BYTE m_Code2[MAX_SFP_CODE_SIZE];
	static CONST BYTE m_Code3[MAX_SFP_CODE_SIZE];
	static CONST BYTE m_Code4[MAX_SFP_CODE_SIZE];
};



template<typename your_class>
CONST BYTE TSFPTrunk<your_class>::m_Code0[MAX_SFP_CODE_SIZE] = 
{	0x8b, 0x0d, 0x80, 0x53, 0x41, 0x00, 0x90, 0x90,
	0xff, 0x15, 0xe0, 0x67, 0x41, 0x00, 0xc2, 0x00, 
	0x00, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
}; 

template<typename your_class>
CONST BYTE TSFPTrunk<your_class>::m_Code1[MAX_SFP_CODE_SIZE] = 
{	0x8b, 0x0d, 0x80, 0x53, 0x41, 0x00, 0x58, 0x5a, 
	0x52, 0x50, 0x52, 0x90, 0xff, 0x15, 0xe0, 0x67, 
	0x41, 0x00, 0xc2, 0x04, 0x00, 0x90, 0x90, 0x90, 
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
}; 

template<typename your_class>
CONST BYTE TSFPTrunk<your_class>::m_Code2[MAX_SFP_CODE_SIZE] = 
{	0x59, 0x58, 0x5a, 0x52, 0x50, 0x51, 0x52, 0x50, 
	0x8b, 0x0d, 0x00, 0x00, 0x00, 0x00, 0xff, 0x15, 
	0x00, 0x00, 0x00, 0x00, 0xc2, 0x08, 0x00, 0x90, 
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
};

template<typename your_class>
CONST BYTE TSFPTrunk<your_class>::m_Code3[MAX_SFP_CODE_SIZE] = 
{	0x8b, 0x44, 0x24, 0x0c, 0x8b, 0x4c, 0x24, 0x08, 
	0x8b, 0x54, 0x24, 0x04, 0x50, 0x51, 0x52, 0x90, 
	0x8b, 0x0d, 0x00, 0x00, 0x00, 0x00, 0xff, 0x15, 
	0x00, 0x00, 0x00, 0x00, 0xc2, 0x0c, 0x00, 0x90
};

template<typename your_class>
CONST BYTE TSFPTrunk<your_class>::m_Code4[MAX_SFP_CODE_SIZE] = 
{	0x8b, 0x44, 0x24, 0x10, 0x8b, 0x4c, 0x24, 0x0c,
	0x8b, 0x54, 0x24, 0x08,	0x50, 0x8b, 0x44, 0x24, 
	0x08, 0x51,	0x8b, 0x0d, 0x94, 0x64, 0x41, 0x00,
	0x52, 0x50,	0xff, 0x15, 0x00, 0x00, 0x00, 0x00, 
	0xc2, 0x10, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90, 
}; 


