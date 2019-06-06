#pragma once

namespace Cool3D
{
	/**	\class IDeviceVizQuery
		\brief Ӳ���Ŀɼ��Բ�ѯ
		\remarks ����begin(),end()֮�������õ�Draw����ͨ��Z testing�����ظ���
	*/
	class IDeviceVizQuery
	{
	public:
		IDeviceVizQuery(void);
		virtual ~IDeviceVizQuery(void);

		virtual void Create() =0;
		virtual bool Begin() =0;
		virtual bool End() =0;
		virtual DWORD GetResultSync() =0;
		virtual bool GetResultAsync(DWORD& out) =0;
	};
}//namespace Cool3D