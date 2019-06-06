#pragma once
#include "..\Cool3D.h"

#pragma warning(disable:4251)

class IFS;

namespace Cool3D
{
	struct tagAniMsg
	{
		//DWORD	trackName;//�����Ϣ�󶨵��Ǹ�Track
		float	time;
		DWORD	msgCode;//four cc
	};

	typedef multimap<DWORD,tagAniMsg>		MultiMsgMap;

	class Cool3D_API AniMsgTable
	{
	public:
		AniMsgTable(void);
		~AniMsgTable(void);

		//! ����,���̸�ʽΪCSV,����"atck,2.1,atck"
		bool LoadFromFile(IFS *pFS,const TCHAR* szFileName);
		//! ����
		bool SaveToFile(const TCHAR* szFileName); 
		//! �õ�һ��Track������msg�б�
		void CheckMsgForTime( DWORD trackName, float lastTime, float curTime, list<DWORD>& msgCodes ) const;

		MultiMsgMap&  GetMsgMap() { return m_msgMap; }

		void Clear() { m_msgMap.clear();}
	private:
		MultiMsgMap		m_msgMap;		/// ����map���Խ�ʡ���ҵ�ʱ��
	};
}//namespace Cool3D
