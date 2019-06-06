#include "StdAfx.h"
#include ".\mtlscriptvm.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "..\Util\Exception.h"
#include "..\Util\RTTInfo.h"
#include "..\ResSys\ResMgr.h"
#include "..\RenderMtl\IMaterial.h"
#include "..\RenderMtl\IRenderMtl.h"
#include "..\RenderMtl\MBitmap.h"
#include "..\RenderMtl\MtlCombiner.h"
#include "..\terrain\TerrainMtl.h"
#include "..\ResSys\ResTexture.h"

namespace F3D
{
	IMaterial *g_curMtl=NULL;

	//�����ƶ���class name����IMaterial����
	//����1:�ַ���,class name
	int mlNewMtl(lua_State *L)
	{
		const char* szClassName=lua_tostring(L,1);
		g_curMtl=(IMaterial*)RTTFactory::Inst()->CreateObj(szClassName);
		ASSERT(g_curMtl!=NULL && "Unknow mtl class");
		return 0;
	}

	//����TextureBasePath
	//����1:����,path����
	int mlSetTexBasePath(lua_State *L)
	{
		const char* szPath=lua_tostring(L,1);
		ResMgr::Inst()->SetTextureBasePath(szPath);
		return 0;
	}
	//���õ�ǰBitmap Material����ͼ
	//����1:����,texture index
	//����2:�ַ���,textture name
	int mlMBTexture(lua_State *L)
	{
		UINT index=lua_tonumber(L,1);
		const char* string=lua_tostring(L,2);
		ASSERT(g_curMtl->IS_STATIC_CLASS(MBitmap));
		MBitmap* pMtl=(MBitmap*)g_curMtl;
		pMtl->SetTexture(index,string);
		return 0;
	}
	//����bitmap Mtl��mip map�Ǿ�
	//����1:����,enum ETextrueFilter,see class IDevice
	int mlMBMipFilter(lua_State *L)
	{
		UINT num=lua_tonumber(L,1);
		ASSERT(g_curMtl->IS_STATIC_CLASS(MBitmap));
		MBitmap* pMtl=(MBitmap*)g_curMtl;
		pMtl->SetMipFilter((ETextrueFilter)num);
		return 0;
	}


	class MtlScriptVM::Member
	{
	public:
		lua_State	*m_pLua;

		Member()
		{
			m_pLua = lua_open();
			lua_baselibopen(m_pLua);
			lua_iolibopen(m_pLua);
			lua_strlibopen(m_pLua);
			lua_mathlibopen(m_pLua);

			lua_register(m_pLua, "NewMtl", mlNewMtl);
			lua_register(m_pLua, "SetTexBasePath", mlSetTexBasePath);
			lua_register(m_pLua, "MBTexture", mlMBTexture);
			lua_register(m_pLua, "MBMipFilter", mlMBMipFilter);
			
		}
		~Member()
		{
			if(m_pLua!=NULL)
			{
				lua_close(m_pLua);
				m_pLua=NULL;
			}
		}


	};

	MtlScriptVM::MtlScriptVM(void)
	{
		m_p=new Member;
	}

	MtlScriptVM::~MtlScriptVM(void)
	{
		delete m_p;
	}

	MtlScriptVM* MtlScriptVM::Inst()
	{
		static MtlScriptVM g_mtlScriptVM;//��������ʱ������ʼ��lua�Ķ���
		return &g_mtlScriptVM;
	}

	void MtlScriptVM::Init()
	{
		//������ʲô
	}

	IMaterial* MtlScriptVM::RunString(const char* szScript)
	{
		ASSERT(szScript!=NULL);

		g_curMtl=NULL;
		ASSERT(m_p!=NULL);
		ASSERT(m_p->m_pLua!=NULL);
		
		lua_dostring(m_p->m_pLua,szScript);

		return g_curMtl;
	}

	
}//namespace F3D