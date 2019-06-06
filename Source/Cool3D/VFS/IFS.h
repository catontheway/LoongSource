#pragma once
#include "..\Cool3D.h"
/**	\class IFS
	\brief �����ļ�ϵͳ�ӿ�
	\remarks һ�������ļ�ϵͳ��Ӧһ���ļ�������һ������Ŀ¼
*/
class Cool3D_API IFS
{
public:
	// ���õ�ǰ�������ļ���
	virtual BOOL SetWorkingPackage(LPCTSTR szPackage)=0;
	// ��ǰ�Ƿ��а���
	virtual BOOL HasWorkingPackage()=0;

	virtual DWORD	Open(LPCTSTR szFileName)=0;
	virtual BOOL	Close(DWORD dwHandle)=0;
	virtual DWORD	Read(PVOID lpMemOut, DWORD dwSize, DWORD dwHandle)=0;
	virtual DWORD	Seek(DWORD dwHandle, INT nOffset, DWORD dwOrigin)=0;
	virtual DWORD	Tell(DWORD dwHandle)=0;

	// �Ż�����
	virtual DWORD	GetSize(LPCTSTR szFileName)=0;	// ����ļ������ڣ��򷵻�GT_INVALID
	virtual DWORD	LoadFile(LPVOID lpMemOut, LPCTSTR szFileName)=0;

	IFS(){}
	virtual ~IFS(){}
};
