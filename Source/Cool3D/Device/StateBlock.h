#pragma once
#include "..\Cool3D.h"
#include "..\util\Noncopyable.h"
#include "IDevice.h"

namespace Cool3D
{
	/**	\class StateBlock 
		\brief һ��RenderState��ֵ������
	*/
	class Cool3D_API StateBlock : private Noncopyable
	{
		struct tagStateVal
		{
			ERStateType state;
			DWORD val;
		};
	public:
		StateBlock(void);
		virtual ~StateBlock(void);

		void Clear();
		/**	��Device��ָ����State�ĵ�ǰֵ����������
		*/
		void SaveState(ERStateType state);
		/**	���������State��ֵ������Ӧ�õ�Device��
		*/
		void Apply();

	protected:
		vector<tagStateVal>	m_stateArray;
	};
}//namespace Cool3D