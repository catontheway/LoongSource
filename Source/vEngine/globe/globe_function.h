//-----------------------------------------------------------------------------
// File: globe_function
// Auth: Lyp
// Date: 2004/3/14
//-----------------------------------------------------------------------------
#pragma once

namespace vEngine
{
//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
VENGINE_API BOOL InitEngine();
VENGINE_API VOID DestroyEngine();
VENGINE_API VOID SetDefaultDir();	// ����Ĭ��·��
VENGINE_API BOOL InitNetwork();		// ��ʼ�������
VENGINE_API VOID DestroyNetwork();	

//-----------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------
//! �õ���ָ�������ָ��
VENGINE_API LPVOID GetObj(LPCSTR szName);
//! ����ע����Ķ��󴴽�����
VENGINE_API LPCSTR CreateObj(LPCSTR szDestObjName, LPCSTR szSrcObjName);
//! ������������
VENGINE_API BOOL KillObj(LPCSTR szObjName);


//-----------------------------------------------------------------------------
// �쳣����
//-----------------------------------------------------------------------------
// �ṹ���쳣ת��
VENGINE_API VOID SEH_Translation(UINT unCode, EXCEPTION_POINTERS* pExp);
// ���::SetUnhandledExceptionFilterʹ�õ��쳣������
// ���޷�ʹ��try...catch��ʱ����ʹ��
VENGINE_API LONG WINAPI SEH_ExceptionFilter(EXCEPTION_POINTERS* pExp);
// �����ʼĴ��mini_dump
VENGINE_API VOID InitDump(LPCTSTR szIP, LPCTSTR szUser, LPCTSTR szPassword, LPCTSTR szVersion=NULL);

//-----------------------------------------------------------------------------
// ע���µĶ���
//-----------------------------------------------------------------------------
template<typename class_type>
VOID Register(vEngine::TWrap<class_type> wrap, LPCSTR szName)
{ vEngine::g_pInterfaceMgr->Register(wrap, szName); }


//-----------------------------------------------------------------------------
//	�ͻ���->���������ݰ�ͷ���봦��	by: Jampe
//-----------------------------------------------------------------------------
#define XCS_KEY						0x12345678

#define XCS_FLAG					0xC0000000

#define XCS_MAX_SIZE				0x00FFFFFF

#define XCS_SWAP_WORD(n)			((((n) & 0xFF00) >> 8) | (((n) & 0xFF) << 8))

#define XCS_SWAP_DWORD(n)			((XCS_SWAP_WORD(((n) & 0xFFFF0000) >> 16)) | (XCS_SWAP_WORD((n) & 0xFFFF) << 16))

//	���ܰ�ͷ������Ϣ
#define XCS_FIX_SIZE(n)				do{													\
										(n) ^= XCS_KEY;									\
										(n) = XCS_SWAP_DWORD(n);						\
										if(XCS_FLAG == ((n) & XCS_FLAG))				\
											(n) = XCS_MAX_SIZE - (XCS_MAX_SIZE & (n));	\
										else											\
											(n) = 0x7FFFFFFF;							\
									}while(0)

//	���ܰ�ͷ������Ϣ
#define XCS_SET_SIZE(n)				do{													\
										INT r = m_pUtil->RandomInRange(0, 0x0F);		\
										r <<= 24;										\
										(n) = XCS_FLAG | r | (XCS_MAX_SIZE - (n));		\
										(n) = XCS_SWAP_DWORD(n);						\
										(n) ^= XCS_KEY;									\
									}while(0)


} // namespace vEngine {



