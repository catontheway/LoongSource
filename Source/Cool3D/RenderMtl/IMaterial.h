#pragma once
#include "..\util\FArchive.h"
#include "..\util\rttiobj.h"
#include "..\Cool3D.h"

namespace Cool3D
{
	class MtlRenderCache;
	class IMtlModifier;
	/**	\class IMaterial 
		\brief �������ʵĽӿ���
		\remarks Material����Texture��pixel stage�ļ��㷽ʽ(����Texture�Ļ���Լ�VertexColor�Ĵ����),
		����ʹ��FixedFunc pipeline��texture stage����ʵ��,Ҳ����ʹ��pixel shaderʵ��
		Ŀǰֻ֧�־�̬��Material,��CubeMap,Sphere����ͼ�����������ɺõ�
	*/
	class Cool3D_API IMaterial : public RTTIObj,public ISerializable
	{
	public:
		IMaterial(void);
		virtual ~IMaterial(void);

		/**	����RenderCache,��this����pCache��������		*/
		void AttachRenderCache(MtlRenderCache *pCache);
		MtlRenderCache* GetRenderCache()	{	return m_pRenderCache;}
		
		/**	����MtlModifier,��this����pMod��������		*/
		void AttachModifier(IMtlModifier *pMod);
		bool RemoveModifier(const TCHAR* szName);
		UINT GetNumMods() const;
		const IMtlModifier *GetModifier(UINT i) const;
		IMtlModifier *GetModifier(const TCHAR* szClassName);
		virtual void UpdateModifiers(float deltaTime);
		void ResetModAni();

		/** �Ƿ���ʹ�õ�resource(��Ҫָtexture)���Ѿ�load ok
			\remark ��Ҫ��Ϊ�˴���resource �첽IO,���ȫ��res load ok,����Դ���mtl render cache
			\see class ResMgr, class ResBase, class ResTexture
		*/
		virtual bool IsResourceReady() const;
		/**����ֵ��������Ⱦ������,Ŀ���Ǽ���shader�л�����*/
		virtual DWORD GetRenderTrait1() const;
		/**����ֵ��������Ⱦ������,Ŀ���Ǽ�����ͼ�л�����*/
		virtual DWORD GetRenderTrait2() const;

		//--ISerializable
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
		static bool SaveToFile(IMaterial *pMtl,const TCHAR* szFileName);
		static IMaterial* LoadFromFile(const TCHAR* szFileName);

		void InvalidateRenderCache();
		void UpdateRenderCache();
	protected:
		IMaterial				*m_pFallbackMtl;//�����ǰϵͳ��֧��this Mtl,��ʹ��Fallback Mtl����
		MtlRenderCache			*m_pRenderCache;//��Ⱦϵͳ��Cache����,��this����������

	private:
		class Member;
		Member	*m_p;

		DECL_RTTI(IMaterial);
	};
}//namespace Cool3D