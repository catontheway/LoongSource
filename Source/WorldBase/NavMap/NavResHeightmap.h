#pragma once
#include "NavResBase.h"

namespace WorldBase
{
	class NavHeightmap;
	/** \class NavResHeightmap
		\brief �߶�ͼ��Դ
	*/
	class NavResHeightmap :
		public NavResBase
	{
	public:
		NavResHeightmap(const TCHAR* szName);
		virtual ~NavResHeightmap(void);

		//--NavResBase
		virtual UINT CreateContent(DWORD param);

		//--
		NavHeightmap* GetHMap(){ return m_pHMap;}

	private:
		void LoadFromFile(IFS *pFS,const TCHAR* szName);

		NavHeightmap* m_pHMap;

		DECL_RTTI(NavResHeightmap);
	};
}//namespace WorldBase