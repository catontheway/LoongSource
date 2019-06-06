#include "..\Cool3D.h"
#pragma warning(disable:4251)//Warning of D3DXVECTOR3 etc need dll export
#pragma once
#include "..\Public\MathBaseDefine.h"

namespace Cool3D
{
	//��floatֵ����int����ʱ������������������ڲ�����_ftol()���ή������
	//ʹ����Ƕ���ָ���FPUָ��
	inline int ftol_ambient(float f) 
	{
		int i;

		__asm 
		{
			fld f;
			fistp i;
		}
		return i;
	}

	inline void LinearLerp_SSE(float *pOut, const float *pV1, const float *pV2, float s, int count)
	{
		//pOut=pV1 + s * (pV2-pV1);
		__declspec(align(16)) float factor[4]={s,s,s,s}, test1;
		float *pFactor=factor;
		int subCount=count%12;
		int mainCount=count-subCount;
		__asm
		{
			mov		ecx,	pFactor
			movaps	xmm0,	xmmword ptr [ecx]
			mov		ecx,	mainCount
			mov		edx,	pV1
			lea		edx,	[edx+ecx*4]
			mov		esi,	pV2
			lea		esi,	[esi+ecx*4]
			mov		edi,	pOut
			lea		edi,	[edi+ecx*4]
			neg		ecx
StartLoop:	//һ�δ���12�������ȸ���Ĳ�ֵ
			movups	xmm1,	xmmword ptr [edx+ecx*4]
			movups	xmm2,	xmmword ptr [edx+ecx*4+16]
			movups	xmm3,	xmmword ptr [edx+ecx*4+16*2]
			movups	xmm4,	xmmword ptr [esi+ecx*4]
			movups	xmm5,	xmmword ptr [esi+ecx*4+16]
			movups	xmm6,	xmmword ptr [esi+ecx*4+16*2]
			subps	xmm4,	xmm1
			subps	xmm5,	xmm2
			subps	xmm6,	xmm3
			mulps	xmm4,	xmm0
			mulps	xmm5,	xmm0
			mulps	xmm6,	xmm0
			addps	xmm1,	xmm4
			addps	xmm2,	xmm5
			addps	xmm3,	xmm6
			movaps	test1,	xmm1
			movups	xmmword ptr [edi+ecx*4],		xmm1
			movups	xmmword ptr [edi+ecx*4+16],		xmm2
			movups	xmmword ptr [edi+ecx*4+16*2],	xmm3
			add     ecx,	12

			jnz     StartLoop
		}

		//����ʣ�µ�����0~11
		for (int i=0; i<subCount; i++)
			pOut[mainCount+i]=pV1[mainCount+i]+s*(pV2[mainCount+i]-pV1[mainCount+i]);
	}

	inline float LinearLerp(float a,float b,float x)
	{
		return (a*(1-x)+b*x);
	}

	inline void LinearLerp(const Vector3& a,const Vector3& b,float x,Vector3& out)
	{
		 out.x=(a.x*(1-x)+b.x*x);
		 out.y=(a.y*(1-x)+b.y*x);
		 out.z=(a.z*(1-x)+b.z*x);
	}

	inline float CosLerp(float a,float b,float x)
	{
		x=(1-cos(x*3.1415927f))*0.5f;
		return (a*(1-x)+b*x);
	}

	/** ret=v*M,��D3D�Զ���ı任�Ĺ�ʽ */
	inline Vector3 operator * (const Vector3& v,const Matrix4& m)
	{
		return Vector3(v.x*m._11+v.y*m._21+v.z*m._31+m._41,
			v.x*m._12+v.y*m._22+v.z*m._32+m._42,
			v.x*m._13+v.y*m._23+v.z*m._33+m._43);
	}

	inline float Vec3Dist(const Vector3& p1,const Vector3& p2)
	{
		Vector3 t=p1-p2;
		return D3DXVec3Length(&t);
	}
	inline float Vec3DistSq(const Vector3& p1,const Vector3& p2)
	{
		Vector3 t=p1-p2;
		return D3DXVec3LengthSq(&t);
	}

	#define  Vec3Len D3DXVec3Length
	inline void Vec3Normalize(Vector3& v)
	{
		D3DXVec3Normalize(&v,&v);
	}

	//!	����һ������ĵ�λ����
	inline Vector3 RandUnitVec3()
	{
		Vector3 tmp((float)rand()/(RAND_MAX/2.0f)-1.0f,
			(float)rand()/(RAND_MAX/2.0f)-1.0f,
			(float)rand()/(RAND_MAX/2.0f)-1.0f);
		Vec3Normalize(tmp);
		return tmp;
	}
	//!	����һ�������0~1.0֮��ĸ�����
	inline float RandUnit()
	{
		return (float)rand()/RAND_MAX;
	}
	inline int GetRandom(int nMin, int nMax)
	{
		int nDiff = nMax - nMin + 1;

		return nMin + (rand( ) % nDiff);
	}

	inline float GetRandom(float fMin, float fMax)
	{
		float fUnit = float(rand( )) / RAND_MAX;
		float fDiff = fMax - fMin;

		return fMin + fUnit * fDiff;
	}

	//! ����ˮƽ����
	inline float CalcYaw(Vector3 dir)
	{
		dir.y=0;
		Vec3Normalize(dir);

		const Vector3 negZ(0,0,-1);
		float yaw=D3DXVec3Dot(&negZ,&dir);

		if(yaw>1.0f)
			yaw=1.0f;
		if(yaw<-1.0f)
			yaw=-1.0f;

		yaw=acosf(yaw);
		if(dir.x>0.0f)
		{
			yaw=FLOAT_PI*2-yaw;
		}
		return yaw;
	}

	inline float CalcYaw(Vector2 dir)
	{
		D3DXVec2Normalize(&dir,&dir);

		const Vector2 negZ(0,-1);
		float yaw=D3DXVec2Dot(&negZ,&dir);

		if(yaw>1.0f)
			yaw=1.0f;
		if(yaw<-1.0f)
			yaw=-1.0f;

		yaw=acosf(yaw);
		if(dir.x>0.0f)
		{
			yaw=FLOAT_PI*2-yaw;
		}
		return yaw;
	}

	inline void Yaw2Dir(float yaw,Vector3& dir)
	{
		dir=Vector3(0,0,-1);

		Matrix4 matRotate;
		D3DXMatrixRotationY(&matRotate,yaw);

		D3DXVec3TransformCoord(&dir,&dir,&matRotate);
	}

	//! ����yawΪ0~FLOAT_PI*2
	inline void YawNormalize(float& yaw)
	{
		int n=int(yaw/(FLOAT_PI*2));
		if(yaw>0)
			yaw-=n*FLOAT_PI*2;
		else
			yaw+=(n+1)*FLOAT_PI*2;
	}

	//! ���㸩��
	inline float CalcPitch(Vector3 dir)
	{
		Vector3 level=dir;//ˮƽ����
		level.y=0;
		Vec3Normalize(level);

		Vec3Normalize(dir);

		float pitch=D3DXVec3Dot(&level,&dir);

		if(pitch>1.0f)
			pitch=1.0f;
		if(pitch<-1.0f)
			pitch=-1.0f;

		pitch=acosf(pitch);
		if(dir.y<0.0f)
		{
			pitch=FLOAT_PI*2-pitch;
		}

		float deg=_RadToDeg(pitch);

		return pitch;
	}

	void sinCos(float *returnSin, float *returnCos, float theta);

	//! ��4*4����õ�λ�ƺ���ת��Ԫ��
	void GetTranAndQuatFromMatrix4(const Matrix4& mat, Vector3& tran, Quaternion& quat);

	//! ����Ԫ���õ�ŷ����
	void GetEulerAngFromQuatO2I(const Quaternion&q, float* yaw, float* pitch, float* roll);
	void GetEulerAngFromQuatI2O(const Quaternion&q, float* yaw, float* pitch, float* roll);

	Quaternion RotateObjectToInertial(float yaw, float pitch, float roll);
	Quaternion RotateInertialToObject(float yaw, float pitch, float roll);

	//! ������������֮�����Ԫ��
	Quaternion GetRotationTo(const Vector3& from, const Vector3& to);

	/** Test to see if a number is an exact power of two (from Steve Baker's Cute Code Collection) */
    inline bool IsPowerOfTwo(unsigned int n)
	{ return ((n&(n-1))==0); }

	/** Returns the least power of 2 greater than or equal to "x".
		Note that for x=0 and for x>2147483648 this returns 0!	*/
	inline __declspec(naked) unsigned __fastcall CeilPowerOf2(unsigned x) 
	{
		__asm {
			xor eax,eax
				dec ecx
				bsr ecx,ecx
				cmovz ecx,eax
				setnz al
				inc eax
				shl eax,cl
				ret
		}
	}

	/** Returns the greatest power of 2 less than or equal to "x".
	 Note that for x=0 this returns 0!*/
	inline __declspec(naked) unsigned __fastcall FloorPowerOf2(unsigned x) 
	{
		__asm {
			xor eax,eax
				bsr ecx,ecx
				setnz al
				shl eax,cl
				ret
		}
	}


	
	//����0--���߶κ���,1--�����߶�,2-���߶���
	inline int ProjectPointToLineSegment(const Vector3& pt,const Vector3& line1,const Vector3& line2,float& prj)
	{
		Vector3 line=line2-line1;
		Vector3 vec=pt-line1;
		float len=D3DXVec3Length(&line);
		if(len<FLOAT_EPSILON)
			return 1;
		prj=D3DXVec3Dot(&line,&vec)/len;
		if(prj<0)
			return 0;
		else if(prj > len)
			return 1;

		return 2;
	}

	//--����һ��ZFarΪ����Զ��͸�Ӿ���
	void MatrixPerspectiveFovLH_INF(Matrix4 *pOut,float fov,float aspect, float znear);

	//--�޸�͸�Ӿ����Զ�zֵ�������ź�ƫ��
	void SetPrjZScaleOffset(Matrix4& matPrj,float zScale,float zOffset);


}//endof namespace Cool3D
