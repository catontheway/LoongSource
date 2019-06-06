#pragma once
#include "..\F3D.h"

class IFS;
namespace F3D
{
	struct tagAniMsg
	{
		DWORD	trackName;//�����Ϣ�󶨵��Ǹ�Track
		float	time;
		DWORD	msgCode;//four cc
	};

	class F3D_API AniMsgTable
	{
	public:
		AniMsgTable(void);
		~AniMsgTable(void);

		//! ����,���̸�ʽΪCSV,����"atck,2.1,atck"
		bool LoadFromFile(IFS *pFS,const char* szFileName);
		//! ����
		bool SaveToFile(const char* szFileName); 
		//! �õ�һ��Track������msg�б�
		vector<tagAniMsg> GetTrackMsgArray(DWORD trackName);

		vector<tagAniMsg>& GetMsgArray() { return m_msgArray; }

		void Clear() { m_msgArray.clear(); }
	private:
		vector<tagAniMsg>	m_msgArray;
	};
}//namespace F3D