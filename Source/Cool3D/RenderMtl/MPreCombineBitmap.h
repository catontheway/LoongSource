#pragma once
#include "..\Cool3D.h"
#include "MBitmap.h"

namespace Cool3D
{
	/**	\class MPreCombineBitmap 
		\brief �ڴ���ʱ�����bitmap��ͼ��ϵ�һ����ͼ
	*/
	class Cool3D_API MPreCombineBitmap : public MBitmap
	{
	public:
		MPreCombineBitmap(void);
		virtual ~MPreCombineBitmap(void);

		void SetFinalSize(int w,int h);//Ĭ��Ϊ��ʹ�õ�ͼ��Ĵ�С
		void SetColorOp(int op/*0add,1sub,2mul*/);
		int GetColorOp() const;
		void BeginCombine();
		void AddLayer(const TCHAR* szImgFile,Color4f colorOffset);
		void AddLayer(const TCHAR* szImgFile,const TCHAR* szAlphaMap);
		void ApplyAlpha(const TCHAR* szAlphaMap);
		void EndCombine();

		UINT GetNumLayers() const;
		tstring& GetLayer(UINT i,Color4f& color);
		tstring GetAlphaMap();

		//--ISerializable
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
	private:
		class Member;
		Member *m_p;
		DECL_RTTI_DYNC(MPreCombineBitmap);
	};
}//namespace Cool3D