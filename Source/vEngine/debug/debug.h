//-----------------------------------------------------------------------------
// File: debug
// Desc: Game Tool debug 2.0
// Auth: Lyp
// Date: 2003/12/11
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {


//-----------------------------------------------------------------------------
// for debug
//-----------------------------------------------------------------------------
// ������Ϣ,��windows���ڸ�����ʾ,�������û������������˳�������log
#define ERR				TObjRef<Debug>()->ErrMsg

// �ڲ���Ϣ,��ʾ��console
#define IMSG			TObjRef<Console>()->Print

// ���¶���ASSERT
#undef ASSERT
#ifdef _DEBUG
	// ����windows������ʾ,�������û������������˳�������log
	#define ASSERT(f)	if( !(f) ) TObjRef<Debug>()->ErrMsg( \
		_T("Failure:%s\r\nFile:%s\r\nLine:%d"), _T(#f), _T(__FILE__), __LINE__)
#else
	#define ASSERT(f)	((void)0)
#endif


//-----------------------------------------------------------------------------
// class Debug ���������еĵ��Թ���
//-----------------------------------------------------------------------------
class VENGINE_API Debug
{
public:
	// ��ʾһ�������Ϣ������ȡϵͳ������Ϣ
	INT  ErrMsg(LPCTSTR lpFormat, ...);

private:
	TObjRef<Console>	m_pConsole;

};



} // vEngine



