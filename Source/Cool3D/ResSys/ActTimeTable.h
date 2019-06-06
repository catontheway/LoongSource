#pragma once
#include "..\Cool3D.h"

#pragma warning(disable:4251)

class IFS;

namespace Cool3D
{
	class ActTimeTable
	{
	public:
		ActTimeTable(void);
		~ActTimeTable(void);

		//! ����,���̸�ʽΪCSV,����"atck,2000"
		bool LoadFromFile(IFS *pFS,const TCHAR* szFileName);
		float operator[](DWORD dwTrackName);
	private:
		map<DWORD,float>	m_actTimeMap;// ����map���Խ�ʡ���ҵ�ʱ��
	};
}//namespace Cool3D
