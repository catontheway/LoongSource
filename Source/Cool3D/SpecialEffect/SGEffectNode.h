#pragma once
#include "..\scenegraph\scenenode.h"
#include "EffectMsg.h"

namespace Cool3D
{
	class EffectBase;
	class EffectPathCtrl;
	/**	\class SGEffectNode 
		\brief ��Ч��SceneGraph�ڵ�
		\remarks ����и��ڵ�,����游�ڵ����������ƶ�
	*/
	class Cool3D_API SGEffectNode : public SceneNode
	{
	public:
		enum { MAX_LOD = 4 };		// ���LOD
		static int sUserLod;        // �ⲿ���õ�LOD����SceneGraph�������LODȡ�ϴ��һ����
		struct tagEffectRenderNode
		{
			int nLOD;
			RenderNode* pRenderNode;
			tagEffectRenderNode( const int n, RenderNode* rendernode ) : 
			nLOD( n ), pRenderNode( rendernode ){}
		};

		SGEffectNode(void);
		virtual ~SGEffectNode(void);

		
		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
		//--ScenneNode
		virtual bool IsInFrustum(const Frustum *pFrumstum) const;
		virtual void LoadRes(const TCHAR* szName);
		virtual UINT GetNumRenderNode() const;
		virtual RenderNode *GetRenderNode(UINT i) const;
		virtual const Matrix4& GetWorldMat() const;
		virtual void SetWorldMat(const Matrix4 *pMat);
		virtual void GetBox(AABBox& out) const;
		virtual IMaterial*	GetMtl(int mtlID);
		virtual void Update(float deltaTime,SceneGraph *pSG);
		virtual void ForceUpdate(float deltaTime,SceneGraph *pSG);
		
		//--
		void SetUserUpdate(bool bUserUpdate);//���û��ֶ�����Update,����scene graph���Զ�Update
		void UserUpdate(float deltaTime,SceneGraph *pSG);
		void SetBoxCenter(const Vector3& center);
		void SetUserWorldMat(bool bUserWorldMat);
		void SetUserPlay(bool bUserPlay);//���û��ֶ����ò���/�رսӿڣ�������ɺ󲻻���������
		bool ReBuildLocalBox();
		int  GetCurLOD(); //��ȡ��ǰ��LOD

		void Play();//!��ʼ����
		void Close(); //!��������(�е���Ч��)
		bool IsClosed();//!�Ƿ����е���ЧԪ�ض���ȫ�ر���?
		void Pause();//!��ͣ����
		void Resume();//!��������
		void SetTimeScale(float timeScale);//!���ò����ٶ�
		void Show(bool bShow)	{	m_bShow=bShow;}
		float GetPlayTime();//!����ʱ�������޳�����-1
		
		//!	�õ���֡��������Ϣ
		const vector<EffectMsg::tagState>& GetFrmMsgArray() const;
		//!	���һ��֡��Ϣ,�ṩ��EffectBase���������
		void PushFrmMsg(EffectMsg::tagState msg);
		
		
		//--editor
		void AddEffect(EffectBase *pEffect);
		void AddEffectPath(EffectPathCtrl *pPath);
		void RemoveEffect(const TCHAR* szName,bool bAutoDel);
		void RemovePath(const TCHAR* szName,bool bAutoDel);
		EffectBase *FindEffect(const TCHAR* szName);
		EffectPathCtrl *FindPath(const TCHAR* szName);

		void RebuildEffectRenderNodes();
		float GetRunTime() const;//�õ������˶��

		void GetEffectArray(vector<EffectBase*>& out);
		void GetEffectPathArray(vector<EffectPathCtrl*>& out);

		bool IsBuild();

		//--
		EffectPathCtrl* GetEffectPath(const TCHAR* szName);

		// ר��Ϊtarget·���ṩ�Ľӿڣ������ⲿ���á�����·��
		void ResetTargetPath( const Vector3& vStart, const Vector3& vTarget );
		void ChangeTargetPathTarget( const Vector3& vTarget );
		bool IsTargetPathArrive();
	protected:
		virtual void MainUpdate(float deltaTime,SceneGraph *pSG);
		void UpdataWorldBox();
	private:
		bool m_bShow;
		class Member;
		Member	*m_p;

		DECL_RTTI_DYNC(SGEffectNode);
	};
}//namespace Cool3D