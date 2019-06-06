#pragma once
#include "..\Cool3D.h"

class IFS;
namespace Cool3D
{
	/**	\class BitMap
		\biref ÿ������Ϊ1bit��map, ��Ҫ��Ϊ�˴���Height map��visibility
	*/
	class Cool3D_API BitMap
	{
	public:
		BitMap(void);
		virtual ~BitMap(void);

		void Create(int w,int h,bool initVal);
		void Destroy();
		inline bool GetValue(int x,int y) const;
		void SetValue(int x,int y,bool val);
		bool SafeGetValue(int x,int y) const;
		void SafeSetValue(int x,int y,bool val);

		inline UINT Width() const	{ return m_width;};
		inline UINT Height() const	{ return m_height; };
		inline bool IfIndex(int x,int y) const { return x>=0 && x<m_width 
													&& y>=0 && y<m_height;}

		void WriteFile(FILE *fp);
		void ReadFile(IFS *pFS,DWORD hFile);

		void Clone(BitMap *pTarget);
		void ExportImage(TCHAR* szFileName);

		/** ���Լ�������copy��ָ���Ķ�����
		*/
		void CopyTo(BitMap& out,int destx,int desty) const;
		void CopyTo(BitMap& out,int srcx,int srcy,int w,int h,int destx,int desty) const;

	protected:
		int		m_width,
				m_height;
		DWORD	*m_pData;//ʹ��4�ֽ�ָ��,�������Ѱַ�ٶ�
		int		m_numBytes;//buffer size
	};
}//namespace Cool3D