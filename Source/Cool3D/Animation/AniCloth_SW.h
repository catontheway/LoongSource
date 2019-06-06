#pragma once
#include "..\ResSys\VertStream.h"
#include <vector>
using namespace std;

namespace Cool3D
{
	class NodeAnimationCtrl;
	class ResCloth;
	/**	\class AniCloth_SW
		\brief ʹ���������cloth��vert buffer
	*/
	class AniCloth_SW
	{
		enum{NUM_LOD=2};

		struct tagMass 
		{
			Vector3	v;			// �ٶ�
			Vector3 f;			// ��
			DWORD	index;		// ��������
			float	oneOverM;	// 1/m

			tagMass()
			{
				memset(this,0,sizeof(tagMass));
				oneOverM = 1.0f;
			}
		};

	public:
		AniCloth_SW(void);
		virtual ~AniCloth_SW(void);

		void Create(const ResCloth *pCloth);

		/**	��������󶨶���
		*/
		void ComputeSkinVB(const NodeAnimationCtrl *pCtrl,int lod);
		/** ���㲼�϶���
			\remark	����ģ����Ҫÿ���̶�ʱ�����ģ����㣬����Ӧ����forceupdateʱ����
		*/
		void ComputeClothVB(const NodeAnimationCtrl *pCtrl,const Matrix4& worldMat,float deltaTime);

		const VertStream* GetVertStream() const { return &m_vertStream; }

	protected:
		void CreateVertStream(const ResCloth *pCloth);
		void CreateIndices(const ResCloth *pCloth);

		/** ����ģ��
		*/
		void ClothSimulate(const NodeAnimationCtrl *pCtrl,const Matrix4& worldMat,float deltaTime);

		/**	����Ӱ�첼�϶�������������������
		*/
		void ComputeForces(const NodeAnimationCtrl *pCtrl,const Matrix4& worldMat,float deltaTime);

		/**	ŷ��һ�λ���
		*/
		void EulerIntegrate(float deltaTime);

		/**	�е���λ���
		*/
		void MidPointIntegrate(float deltaTime);

		/**	RK�Ĵλ���
		*/
		void RK4Integrate(float deltaTime);

		void Integrate(float deltaTime);

	public:
		enum EIntegratorType
		{
			EIT_EULER		= 0,	// ŷ��һ�λ���
			EIT_MIDPOINT,			// �е���λ���
			EIT_RK4					// RK�Ĵλ���
		};

	private:
		enum ECollisionTypes
		{
			ECT_NOT_COLLIDING = 0,	// û����ײ
			ECT_COLLIDING			// ������ײ
		};

		struct tagContact 
		{
			int		index;
			Vector3 normal;
		};

	private:
		ECollisionTypes CheckForCollisions(const NodeAnimationCtrl *pCtrl);
		void ResolveCollisions(void);

	protected:
		ResCloth				*m_pResCloth;
		VertStream				m_vertStream;
		vector<int>				m_indices[NUM_LOD];
		int						m_numLod;

		vector<tagMass>			m_massArray;
		vector<tagContact>		m_contactArray;

		EIntegratorType			m_IntegratorType;
		int						m_timeIterations;	// ���ֱ��������ٲ��϶����ļ���
		float					m_maxTimeStep;		// �����㲽��
		Vector3					m_gravity;			// �������ٶ�
		float					m_kr;				// �ָ�ϵ��
		float					m_xKd;				// x�������������ϵ��
		float					m_zKd;				// z�������������ϵ��
		float					m_yUpKd;			// y�������Ͽ���������ϵ��
		float					m_yDownKd;			// y�������¿���������ϵ��
		float					m_dragForceKs;		// �����󶨵���ק���Ļָ�ϵ��
		bool					m_isFirst;			//
		Vector3					m_lastPos;
		Vector3					m_vel;
	};
}//namespace Cool3D