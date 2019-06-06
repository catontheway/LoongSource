#pragma once
#include "..\Public\VertexDefine.h"
#include "..\Math\GeomDefine.h"
#include "..\Cool3D.h"
#include "IDevice.h"	

namespace Cool3D
{
	class VertStream;
	class IStreamDefine;
	class IMaterial;
	class Cool3D_API DrawX
	{
	public:
		DrawX(void);
		~DrawX(void);

		/**	��һ��������ԭ��Ϊ���İ뾶Ϊ1����		*/
		static void DrawSphere();
		/**	�򵥵Ļ���һ���ַ���		*/
		static void DrawDebugString(const TCHAR* szString,int x,int y,DWORD color=0xFFFFFFFF);
		/**	ʹ��3D������3D�ռ��л���һ������		*/
		static void Draw3DQuad(const Vert_PD* pVert);
		/**	����һ��AABBox		*/
		static void DrawAABBox(const AABBox& box,DWORD color=0xFFFF0000);
		/**	ʹ��3D������3D�ռ��л���һ���߶�		*/
		static void Draw3DLine(const Vector3& p1,const Vector3& p2,DWORD color);
		/**	ʹ����Ļ���������ڻ�һ��2D��Quad		*/
		static void Draw2DQuad(int x,int y,int width,int height,bool bLine=true,DWORD color=0xFFFFFFFF);
		/**	ʹ����Ļ���������ڻ�һ������		*/
		static void Draw2DLine(int sx,int sy,int ex,int ey,DWORD color=0xFFFFFFFF);
		/** ʹ�ö����Ļ�������껭����ͼԪ			*/
		static void Draw2DPrimitiveUP(EDrawType eType, LPPOINT lpPoints, int nVerCount, DWORD color=0xFFFFFFFF);

		static void BeginMtl();
		static void EndMtl();
	private:
		static void CreateDefaultMtl();
	private:
		static void SetStreamDefine(int type=0/*0:3D,1:2D*/);
		static IStreamDefine *m_pVertDef;
		static IStreamDefine *m_pVertDef2D;
		static VertStream	*m_pSphereVerts;
		static IMaterial	*m_pMtl3D;
	};
}//namespace Cool3D