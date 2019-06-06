#pragma once

class IFS;
namespace WorldBase
{
	/**	\class NavResBase 
		\brief ����Resource�Ļ���
	*/
	class NavResBase : public RefObj
	{
	public:
		explicit NavResBase(const TCHAR* szName);
		virtual ~NavResBase(void);

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

		DECL_RTTI(NavResBase);
	};
}//namespace WorldBase