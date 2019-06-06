
#pragma once
#include "..\Math\GeomDefine.h"
#include "..\Math\Ray.h"

class IFS;
#define VFS_READ
#include "..\Public\AABBTree.h"
#include "VertStream.h"

namespace Cool3D
{
	class Cool3D_API AABBTreeCollider : public AABBTree
	{
	public:
		AABBTreeCollider();
		virtual ~AABBTreeCollider();

		void Draw();
		TResult RayCollide(const Ray& ray,const Matrix4 *pObj2World,VertStream *pVS);

		//�ж��߶��Ƿ�������Ҷ�ӽڵ��ཻ
		bool LineCollide(Vector3 vPoint1,Vector3 vPoint2,Matrix4 WorldMat);

	private:
		void RayCollideR(AABBTreeNode *pNode,const Ray& localRay,TResult& lastTR,VertStream *pVS);
		//�ж��߶��Ƿ�������Ҷ�ӽڵ��ཻ���ݹ飩
		bool LineCollideR(AABBTree* pTree,AABBTreeNode *pNode,Vector3 vPoint1,Vector3 vPoint2,Matrix4 WorldMat);

	};
}//namespace Cool3D