#include "..\Cool3D.h"
#include "..\util\FArchive.h"
#pragma once
#include "..\util\rttiobj.h"

namespace Cool3D
{
	class IMaterial;
	/**	\class IMtlModifier 
		\brief �����޸Ĳ��ʵ���ɫ,UV Matrix��
	*/
	class Cool3D_API IMtlModifier : public RTTIObj,public ISerializable
	{
	public:
		IMtlModifier(void);
		virtual ~IMtlModifier(void);

		virtual void Update(float deltaTime,IMaterial *pOwner);
		virtual void ResetAni(IMaterial *pOwner);

		void SetName(const TCHAR* szName);
		const TCHAR* GetName() const;
		//--ISerializable
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
	private:
		class Member;
		Member *m_p;
		DECL_RTTI(IMtlModifier);
	};
}//namespace Cool3D