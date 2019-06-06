#pragma once
#include "..\Cool3D.h"
#include "Math.h"

namespace Cool3D
{
	/**	\class Transform
		\brief �任����,���Լ���һϵ�б任
		\remarks
		����:Transform t;
		t.RotateAxis('x',3.1415926f);
		t.Translate(Vector3(0,10,0));
		t.TransVec3(v);
		�൱�ڽ�v��ƽ��,Ȼ������ת.
	*/
	class Cool3D_API Transform
	{
	public:
		enum ECombineMode
		{
			ECM_ToLeft,
			ECM_ToRight
		};
	public:
		Transform(void);
		Transform(ECombineMode mode);
		~Transform(void);

		/**	��Transform�óɵ�λ����,��û���κα任		*/
		void LoadIdentity();
		/**	����һ���ƶ�		*/
		void Translate(const Vector3& offset);
		/**	������ת		*/
		void Rotate(float yaw,float pitch,float roll);
		/**	������ĳ������ת
			\param axis����ĸx,y,z֮һ,rad����ת�Ƕ�,�Ƕ�Ϊ������Ϊ��ʱ�뷽����ת
		*/
		void RotateAxis(char axis,float rad);
		/**	��������������ת		*/
		void RotateAxis(const Vector3& axis,float rad);
		/** �����Ԫ����ת */
		void RotateQuat(const Quaternion& q);
		/**	��������		*/
		void Scale(float x,float y,float z);

		/** �ڵ�ǰ�ı任�м���һ���任		*/
		void CombineTrans(const Matrix4* pMatrix);
		void CombineTransRight(const Matrix4* pMatrix);
		/**	ʹ��һ�������滻��ǰ�ľ���		*/
		void LoadMatrix(const Matrix4* pMatrix);
		
		void TransVec3(Vector3& v);
		const Matrix4* GetMatrix() const	{	return &m_mat;}
	private:
		Matrix4	m_mat;
		ECombineMode m_mode;
	};
}//namespace Cool3D