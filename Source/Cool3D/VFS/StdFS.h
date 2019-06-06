#pragma once
#include "ifs.h"

namespace Cool3D
{
	/**	\class StdFS 
		\brief ʹ��C runtimeʵ�ֵ�IFS�ӿ�,����Ĭ�ϵ��ļ�ϵͳ
	*/
	class Cool3D_API StdFS : public IFS
	{
	public:
		StdFS(void);
		virtual ~StdFS(void);

		virtual BOOL SetWorkingPackage(LPCTSTR szPackage);
		virtual BOOL HasWorkingPackage();
		
		virtual DWORD	Open(LPCTSTR szFileName);
		virtual BOOL	Close(DWORD dwHandle);
		virtual DWORD	Read(PVOID lpMemOut, DWORD dwSize, DWORD dwHandle);
		/**
			\param dwOriginΪFILE_BEGIN,FILE_CURRENT,FILE_END,���ǵĶ���μ�Win32 API��SetFilePointer()
		*/
		virtual DWORD	Seek(DWORD dwHandle, INT nOffset, DWORD dwOrigin);
		virtual DWORD	Tell(DWORD dwHandle);

		virtual DWORD	GetSize(LPCTSTR szFileName);
		virtual DWORD	LoadFile(LPVOID lpMemOut, LPCTSTR szFileName);
	};
}//namespace Cool3D