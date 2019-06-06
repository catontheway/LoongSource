#pragma once
#include "farchive.h"

namespace Cool3D
{
	/**	\class FArchiveAdv 
		\brief ������ȡ���ڲ��ļ�ָ��Ľӿ�
		\remark ��Ҫ����Ϊ�ⲿ���������Ҫ���ļ�����Seek����
	*/
	class FArchiveAdv :	public FArchive
	{
	public:
		FArchiveAdv(void)			{}
		virtual ~FArchiveAdv(void)	{}

		FILE* GetWriteFile() const	{	return FArchive::m_pWriteFile;}
		DWORD GetReadFile() const	{	return FArchive::m_hReadFile;}
		IFS* GetReadFS() const		{	return FArchive::m_pFS;}
	};
}//namespace Cool3D