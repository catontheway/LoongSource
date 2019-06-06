#pragma once
#include "..\Cool3D.h"
#include "..\Math\Math.h"
#include "..\Math\GeomDefine.h"
#include "..\Math\Ray.h"
#include "..\Util\RTTIObj.h"
#include "..\Util\FArchive.h"

namespace Cool3D
{
	class SceneGraph;
	class IMaterial;
	class RenderNode;
	class Frustum;
	class Occluder;

	enum ESceneNodeType
	{
		ESNT_StaticMesh,
	};
	/**	\class SceneNode
		\brief SceneGraph�е�һ���ڵ�
		\todo ��һ��"QueryResInterface()"�Ľӿ�,�����õ��ڲ�����Դָ��,��Ϊ�ڲ�����Դ�п���������ȸ��ӽӿ�,��Ҫ�ͻ��˶�����в���
		\todo ��SceneNode����IMaterial����,��Ϊ������ʱ���ʿ��ܸı�,��Ҫָ��װ,����һ��Res�п�
		�ܰ����������.
		\todo ��node��world matrix??
		\todo ����һ��Enable()�Ľӿ�,��Щ������Ҫ��ʱ��������
	*/
	class Cool3D_API SceneNode : public RTTIObj,public ISerializable
	{
	public:
		SceneNode(void);
		virtual ~SceneNode(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
		//--
		/**	�����Ƿ�����׶��
			\remarks ��Node��������Ϊ,Node�����ͬ��Bounding Volume���ܲ�ͬ,����LightNode����ΪSphere����Cone
		*/
		virtual bool IsInFrustum(const Frustum *pFrumstum) const =0;
		virtual void LoadRes(const TCHAR* szName) =0;
		virtual UINT GetNumRenderNode() const =0;
		virtual RenderNode *GetRenderNode(UINT i) const =0;
		virtual const Matrix4& GetWorldMat() const =0;
		virtual void SetWorldMat(const Matrix4 *pMat) =0;
		virtual void GetBox(AABBox& out) const =0;//��Ϊ�е�SceneNode�Ǽ�ʱ�����,���Բ�ֱ�ӷ���һ��ָ���������
		virtual UINT GetNumMtl() const{ return 0;}
		virtual IMaterial*	GetMtl(int mtlID) =0;
		/**	update this node and all children recursive*/
		virtual void Update(float deltaTime,SceneGraph *pSG);
		/**	ǿ�Ƹ���
			\remarks Update()����ֻ����Node�ɼ�ʱ�Żᱻ����,��Ҫ�����������¶�����;
				�� FroceUpdate()��������Node�Ƿ�ɼ����ᱻ����,��Ҫ�������������ڲ��߼�,
				�Լ�Is Still Loading�ȼ��.
		*/
		virtual void ForceUpdate(float deltaTime,SceneGraph *pSG) =0;
		virtual TResult RayCollide(const Ray& ray);
		
		UINT GetNumOccluder();
		Occluder* GetOccluder(UINT i);
		void AttachOccluder(Occluder* pOccluder)
		{
			m_occluders.push_back(pOccluder);
		}
		void DetachOccluder(Occluder* pOccluder)
		{
			for(vector<Occluder*>::iterator iter=m_occluders.begin();
				iter!=m_occluders.end();++iter)
			{
				if((*iter)==pOccluder)
				{
					m_occluders.erase(iter);
					break;
				}
			}
		}
		/**	�򱾽ڵ������һ���ӽڵ�,��this������������
			\remarks ���͵�Ӧ�������ProjectShadow,decal,�Լ�Avatar
		*/
		void AttachChild(SceneNode *pChild)
		{
			pChild->m_parent=this;
			m_children.push_back(pChild);
		}
		
		/** If you detach the nod from parent node,you need DELETE it yourself*/
		void DetachChild(const SceneNode *pNode);
		void DetachChild(const TCHAR* szClassName);
		int GetNumChildren() const					{	return m_children.size();	}
		const SceneNode *GetChild(int i) const		{	return m_children[i];	}
		SceneNode *GetChildVar(int i)		{	return m_children[i];	}

		//userdata
		void SetUserData(const TCHAR* szUserData)	{	m_szUserData=szUserData;}
		const TCHAR* GetUserData() const				{	return m_szUserData.c_str();}
		const SceneNode*	GetParent() const		{	return m_parent; }

		// For scenegraph visitor
		int GetStaticNodeID()			{ return m_staticNodeID;}
		void SetStaticNodeID(int id)	{ m_staticNodeID=id;	}

		void SetLightmap(const TCHAR* szLightmap)	{	m_szLightMap=szLightmap;}
		const TCHAR* GetLightmap()					{	return m_szLightMap.c_str();}

		virtual bool IsLoaded(){ return true;}

	private:
		vector<Occluder*>	m_occluders;
		vector<SceneNode*>	m_children;
		SceneNode*			m_parent;
		tstring				m_szUserData;
		tstring				m_szLightMap;
		int					m_staticNodeID;

		DECL_RTTI(SceneNode);
	};
}//namespace Cool3D