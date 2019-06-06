#pragma once
#include "HalfByteMap.h"

namespace WorldBase
{
	/** \class AreaMap
		\brief ����Զ�Ѱ·����ͼ
	*/
	class AreaMap
	{
	public:
		AreaMap(void);
		~AreaMap(void);

		void ReadFile(IFS* pFS,DWORD hFile);
		int GetValue(int x,int z);
		int SafeGetValue(int x,int z);

		void Create(HalfByteMap* pSource);
		void WriteFile(FILE* fp);

	private:
		BYTE		m_type;			//����:0һ������,1��������,2�������
		BYTE		m_oneAreaIndex;	//ֻ��һ������ʱ��������
		BitMap		m_bitmap;		//����������ʱ����ΪBitmap
		HalfByteMap m_halfByteMap;	//�ж������ʱ����ΪHalfByteMap
	};
}//namespace WorldBase
