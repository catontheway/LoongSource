#pragma warning(disable:4251)
#pragma once

#include "..\MaxPublic\UnicodeUtil.h"
#include "..\MaxPublic\MtlDesc.h"
#include "F3DMaxExp.h"
class MsgBox;
using namespace std;

namespace Cool3D
{
	class F3D_MAX_API F3DExpMtl  
	{
	public:
		F3DExpMtl(Mtl* pMtl,int subID,int myID,Mtl *pBakedMtl);
		virtual ~F3DExpMtl();
		
		void GetMtlDesc(tagMtlDesc& out);
		bool Equal2Max(Mtl *pMtl,int subID,Mtl *pBakedMtl)
		{
			return m_pMaxMtl==pMtl && m_nMaxSubID==subID 
				&& m_pBakedMtl==pBakedMtl;
		}
		
	public:
		//--options
		static tstring		s_szBasePath;
		static tstring		s_szSubPath;		//���·��
		static bool			s_bAutoCopy;	
		static bool			s_bExportLMUV;		//�Ƿ񵼳�LightMap��UVͨ��
		static bool			s_bExportBUUV;		//�Ƿ񵼳�NormalMap��UVͨ����ͬʱ���㶥��Tangent
		static bool			s_bMultiPass;		//�Ƿ�ʹ�ö����Ⱦ
		static bool			s_bExportOpcity;	//�Ƿ񵼳����οղ��ʵ�����
		static bool			s_bExportBox;		//�Ƿ񵼳���ײ��
		static tstring		s_szOriginNode;		//�Դ˽ڵ�Ϊ���ĵ���
		static int			s_iLightMapSize;	//LightMap�ĳߴ�
		static float		s_fCollideBoxSize;	//��ײ�еĳߴ�
		//--
		static MsgBox		*s_pMsgBox;
	private:
		tstring GetTexMapPath(Mtl* pMtl, DWORD mapid);
		//--max mtl identity
		Mtl *m_pMaxMtl;
		Mtl *m_pBakedMtl;
		int m_nMaxSubID;
		
		int	m_nID;//mtl id in SceneContext
		
		//--mtl data
		Vector4 m_diffuse;
		Vector4 m_ambient;
		Vector4 m_specular;
		Vector4 m_emissive;
		float m_power;
		
		bool m_bTwoSide;
		bool m_bOpacityMap;
		BYTE m_opacityVal;
		bool m_bMultiPass;
		
		tstring m_mapfile[EMap_Max];//see enum MapChannel	
	};
	
}//namespace Cool3D
#pragma warning(default:4251)