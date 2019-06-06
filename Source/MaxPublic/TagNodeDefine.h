#pragma once
#include ".\MathBaseDefine.h"
#include ".\FileUtil.h"

namespace Cool3D
{
	/**	��־�ڵ�,�����洢һ��Node�ı任��Ϣ,�൱��û�и��ӹ�ϵ��Bone�ڵ�*/
	struct tagTagNode
	{
		DWORD			name;		//four cc name
		Vector3			trans;		//ƽ��
		Quaternion		rotat;		//��ת
		bool			bMirrored;	
	};

	class TagNodeFrame
	{
	public:
		vector<tagTagNode>	nodes;
	
	public:
		TagNodeFrame()	{}
		~TagNodeFrame()	{}

		int GetNumNodes() const	{	return nodes.size();}

		void WriteFile(FILE *fp)
		{
			FWriteValVector(fp,nodes);
		}
#ifdef VFS_READ
		void ReadFile(IFS *pFS,DWORD hFile)
		{
			FReadValVector(pFS,hFile,nodes);
		}
#else
		void ReadFile(FILE *fp)
		{
			FReadValVector(fp,nodes);
		}
#endif

		TagNodeFrame(const TagNodeFrame& other)
		{
			nodes=other.nodes;
		}
		const TagNodeFrame& operator = (const TagNodeFrame& other)
		{
			if(this!=&other)
			{
				nodes=other.nodes;
			}
			return *this;
		}
	};
}//endof namespace Cool3D