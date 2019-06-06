#pragma once
#include "..\util\refobj.h"
#include "..\Cool3D.h"
#pragma warning(disable:4251)

	class IFS;
namespace Cool3D
{
	/**	\class ResBase 
		\brief ����Resource�Ļ���
	*/
	class Cool3D_API ResBase : public RefObj
	{
	public:
		explicit ResBase(const TCHAR* szName);
		virtual ~ResBase(void);

		/**	���ӵ�һ���ļ�ϵͳ
		*/
		void AttachFS(IFS *pFS)
		{	m_pFS=pFS; }
		IFS	*GetSafeFS();
		/**	��������ʽ����ʵ������,ֻ�ڵ�һ�β�������ʱ����
			\return ������Դ�Ĵ�С,��λ�����������,ResPool�������С�������Ƿ�ﵽ�趨������
			\remarks Ĭ��ʵ��Ϊ������Դ��ָ��Ĵ����ļ��Ĵ�С
		*/
		virtual UINT CreateContent(DWORD param) =0;
		/**	���첽��ʽ��������,�����Ƿ񴴽����ͨ������IsCreated()���ж�
			\see CreateContent(),IsCreate()
		*/
		virtual void CreateContentAsync(DWORD param);

		UINT GetSize() const
		{	return m_size; }
		const TCHAR *GetName() const
		{	return m_szName.c_str(); }
		bool IsCreated() const
		{	return m_bCreated!=0; }
	protected:
		IFS		*m_pFS;
		volatile LONG	m_bCreated;//4BYTE
		UINT	m_size;
	
		tstring	m_szName;

		DECL_RTTI(ResBase);
	};
}//namespace Cool3D