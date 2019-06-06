#include "StdAfx.h"
#include ".\stdfs.h"
#include "..\Util\Exception.h"
#include "..\Kernel.h"

#pragma warning(disable:4311)	//�ӡ�FILE *������DWORD����ָ��ض�
#pragma warning(disable:4312)	//�ӡ�DWORD��ת��������ġ�FILE *��
#pragma warning(disable:4267)	//�ӡ�size_t��ת������DWORD�������ܶ�ʧ����


namespace Cool3D
{
	StdFS::StdFS(void)
	{
	}

	StdFS::~StdFS(void)
	{
	}

	BOOL StdFS::SetWorkingPackage(LPCTSTR szPackage)
	{
		return TRUE;
	}

	BOOL StdFS::HasWorkingPackage()
	{
		return TRUE;
	}

	DWORD	StdFS::Open(LPCTSTR szFileName)
	{
		//--��ֹ�ⲿ����SetCurrentDirector�ı乤��·��,ÿ�δ��ļ�������һ��
		//ע��:MFC��CFileDialog���Զ��ı�ϵͳ�ĵ�ǰ·��.
		BOOL workDirOK=::SetCurrentDirectory(Kernel::Inst()->GetWorkPath());
		ASSERT(workDirOK);

		//--
		tstring szFlag=_T("rb");
		FILE *file=_tfopen(szFileName,szFlag.c_str());
		return (DWORD)(file);
	}

	BOOL	StdFS::Close(DWORD dwHandle)
	{
		FILE *file=(FILE*)(dwHandle);
		int ret=fclose(file);
		return EOF!=ret;
	}

	DWORD	StdFS::Read(PVOID lpMemOut, DWORD dwSize, DWORD dwHandle)
	{
		FILE *file=(FILE*)(dwHandle);
		size_t readsize=fread(lpMemOut,dwSize,1,file);
		return readsize*dwSize;
	}

	DWORD	StdFS::Seek(DWORD dwHandle, INT nOffset, DWORD dwOrigin)
	{
		int orgn=0;
		switch(dwOrigin)
		{
		case FILE_CURRENT:
			orgn=SEEK_CUR;
			break;
		case FILE_END:
			orgn=SEEK_END;
			break;
		case FILE_BEGIN:
			orgn=SEEK_SET;
			break;
		default:
			ASSERT(0);
		}
		FILE *file=(FILE*)(dwHandle);
		DWORD ret=fseek(file,nOffset,orgn);
		return ret;
	}

	DWORD	StdFS::Tell(DWORD dwHandle)
	{
		FILE *file=(FILE*)(dwHandle);
		return ftell(file);
	}

	DWORD	StdFS::GetSize(LPCTSTR szFileName)
	{
		BOOL workDirOK=::SetCurrentDirectory(Kernel::Inst()->GetWorkPath());

		FILE *file=_tfopen(szFileName,_T("rb"));
		if(file == NULL)
			return -1;
		fseek(file,0,SEEK_END);
		DWORD size=ftell(file);
		fclose(file);
		return size;
	}

	DWORD	StdFS::LoadFile(LPVOID lpMemOut, LPCTSTR szFileName)
	{
		BOOL workDirOK=::SetCurrentDirectory(Kernel::Inst()->GetWorkPath());

		FILE *file=_tfopen(szFileName,_T("rb"));
		if(file == NULL)
			return 0;
		fseek(file,0,SEEK_END);
		DWORD size=ftell(file);
		fseek(file,0,SEEK_SET);
		DWORD read=fread(lpMemOut,size,1,file);
		fclose(file);
		return read;
	}

}//namespace Cool3D