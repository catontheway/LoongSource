
#pragma once

namespace Cool3D
{
	/**	\class FreePtrVector
		\brief deleteһ��vector�е�����ָ��, ������մ�vector
	*/
	template<class T>
		class FreePtrVector
	{
	public:
		void operator()(vector<T*>& array)
		{
			for(size_t i=0;i<array.size();i++)
			{
				T* pObj=array[i];
				delete pObj;
			}
			array.clear();
		}
	};
}//namespace Cool3D