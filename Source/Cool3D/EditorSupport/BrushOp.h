#pragma once
#include "..\Math\math.h"
#include "..\Math\Noise.h"
#include ".\TerrainEditor.h"
#include "..\Terrain\Heightmap.h"
#include "..\Terrain\BitMap.h"
#include ".\BrushOPRecorder.h"

namespace Cool3D
{
#pragma warning( disable : 4244 )
	namespace Detail
	{
		/**	���x==0�򷵻�a
		*/
		template<class T>
			T Lerp(T a,T b,float x,ELerpType type=ELerp_Cosine)
		{
			const float e=19.0f/7;
			switch(type)
			{
			case ELerp_None:
				return b;
			case ELerp_Cosine:
				x=(1-cos(x*3.1415927f))*0.5f;
				break;
			case ELerp_Quadratic:
				x=x*x;
				break;
			default://case ELerp_Linear:
				break;
			}

			return (T)(a*(1-x)+b*x);
		}

		template<typename BrushOp>
			void DoBrush(Heightmap *pMap,const tagTEditBrush& brush,BrushOp& op)
		{
			if(pMap==NULL)
				return;
			switch(brush.shape)
			{
			case EShape_Quad:
				{
					int strip=brush.outer-brush.inner;
					int x,z;
					for(z=brush.centerZ-brush.outer;z<brush.centerZ+brush.outer;z++)
					{
						if(z<0 || z>=(int)pMap->Height())
							continue;
						for(x=brush.centerX-brush.outer;x<brush.centerX+brush.outer;x++)
						{
							if(x<0 || x>=(int)pMap->Width())
								continue;
							if(z>=brush.centerZ-brush.inner
								&& z<brush.centerZ+brush.inner
								&& x>=brush.centerX-brush.inner
								&& x<brush.centerX+brush.inner)
								op.OnInner(pMap,x,z,brush.strength);
							else//����Ȧ����Ȧ֮��
							{
								if(strip>0)
								{
									//����Ȧ�ľ���
									float zdist=abs(z-brush.centerZ)-brush.inner;
									float xdist=abs(x-brush.centerX)-brush.inner;
									float r=max((zdist/strip),(xdist/strip));

									float a=(float)brush.strength;
									float v=Lerp(a,0.0f,r,brush.lerp);
									op.OnOuter(pMap,x,z,(int)v,r,brush.lerp);
								}
							}
						}
					}//endof for(z)
				}
				break;
			case EShape_Circle:
				{
					//Բ��
					POINT	tCenter={brush.centerX,brush.centerZ};
					//Բ�ھ�
					
					int x,z;
					for(z = tCenter.y-brush.outer;
						z <= tCenter.y+brush.outer;
						z++)
					{
						if(z<0 || z>=(int)pMap->Height())
							continue;
						for(x = tCenter.x-brush.outer;
							x <= tCenter.x+brush.outer;
							x++)
						{
							if(x<0 || x>=(int)pMap->Width())
								continue;

							int cx=x-tCenter.x;
							int cz=z-tCenter.y;//????cx,cz�����float�ͻ�������
							float dist=sqrtf(cx*cx+cz*cz);
							if(dist > brush.outer)
								continue;
							if(dist<=brush.inner)//���ھ���Χ��,��ֱ������brush��ֵ
								op.OnInner(pMap,x,z,brush.strength);
							else//���ھ����⾶֮��,���brush��ֵ���в�ֵ��������
							{
								float r=(dist-brush.inner)/(brush.outer-brush.inner);//�뿪�ھ�ԽԶrֵԽ��,���ھ��Ϸ���0
								float a=(float)brush.strength;
								float v=Lerp(a,0.0f,r,brush.lerp);
								op.OnOuter(pMap,x,z,(int)v,r,brush.lerp);
							}
						}
					}//endof for(z)
				}//endof else
				break;
			case EShape_Line:		//������ʵ�����͵�ˢ�ӱ༭���߸߶�ֻ���������������
				{	// ����ˢ�����ĵ���Ϊ�߶ε���ʼ��
					POINT	tCenter={brush.centerX,brush.centerZ};

					int x,z;
					x = tCenter.x; z=tCenter.y;
					// ��ˢ���ھ���Ϊ�߶εĳ��ȣ���ˢ���⾶��Ϊ�߶���X��ĽǶ�
					for(int i=1;i<brush.inner;i++)
					{
						if(z<0 || z>=(int)pMap->Height())
							continue;
						if(x<0 || x>=(int)pMap->Width())
							continue;
						
						x = tCenter.x + cosf(_DegToRad(brush.outer))*i;
						z = tCenter.y + sinf(_DegToRad(brush.outer))*i;
						op.OnInner(pMap,x,z,brush.strength);
					}
				}
				break;
			case EShape_Plane:
				{	// רΪ���ڱ༭������б�µ����߸߶����
					int x,z;
					for(z=brush.centerZ-brush.inner+1;z<brush.centerZ+brush.inner;z++)
					{
						if(z<0 || z>=(int)pMap->Height())
							continue;
						for(x=brush.centerX-brush.inner+1;x<brush.centerX+brush.inner;x++)
						{
							if(x<0 || x>=(int)pMap->Width())
								continue;														
							op.OnInner(pMap,x,z,brush.strength);
						}
					}//end of for(z)
				}
				break;
			}// end of switch			
		}

		template<typename BrushOp>
		void DoBrush(BitMap *pMap,const tagTEditBrush& brush,BrushOp& op)
		{
			if(pMap==NULL)
				return;
			switch(brush.shape)
			{
			case EShape_Quad:
				{
					int strip=brush.outer-brush.inner;
					int x,z;
					for(z=brush.centerZ-brush.outer;z<brush.centerZ+brush.outer;z++)
					{
						if(z<0 || z>=(int)pMap->Height())
							continue;
						for(x=brush.centerX-brush.outer;x<brush.centerX+brush.outer;x++)
						{
							if(x<0 || x>=(int)pMap->Width())
								continue;
							if(z>=brush.centerZ-brush.inner
								&& z<brush.centerZ+brush.inner
								&& x>=brush.centerX-brush.inner
								&& x<brush.centerX+brush.inner)
								op.OnInner(pMap,x,z,brush.strength);
						}
					}//end of for(z)
				}
				break;
			case EShape_Circle:
				{
					//Բ��
					POINT	tCenter={brush.centerX,brush.centerZ};
					//Բ�ھ�

					int x,z;
					for(z = tCenter.y-brush.outer;
						z <= tCenter.y+brush.outer;
						z++)
					{
						if(z<0 || z>=(int)pMap->Height())
							continue;
						for(x = tCenter.x-brush.outer;
							x <= tCenter.x+brush.outer;
							x++)
						{
							if(x<0 || x>=(int)pMap->Width())
								continue;

							int cx=x-tCenter.x;
							int cz=z-tCenter.y;//????cx,cz�����float�ͻ�������
							float dist=sqrtf(cx*cx+cz*cz);
							if(dist > brush.outer)
								continue;
							if(dist<=brush.inner)//���ھ���Χ��,��ֱ������brush��ֵ
								op.OnInner(pMap,x,z,brush.strength);
						}
					}//endof for(z)
				}//endof else
				break;
			case EShape_Line:		//������ʵ�����͵�ˢ�ӱ༭���߸߶�ֻ���������������
				{	// ����ˢ�����ĵ���Ϊ�߶ε���ʼ��
					POINT	tCenter={brush.centerX,brush.centerZ};

					int x,z;
					x = tCenter.x; z=tCenter.y;
					// ��ˢ���ھ���Ϊ�߶εĳ��ȣ���ˢ���⾶��Ϊ�߶���X��ĽǶ�
					for(int i=1;i<brush.inner;i++)
					{
						if(z<0 || z>=(int)pMap->Height())
							continue;
						if(x<0 || x>=(int)pMap->Width())
							continue;

						x = tCenter.x + cosf(_DegToRad(brush.outer))*i;
						z = tCenter.y + sinf(_DegToRad(brush.outer))*i;
						op.OnInner(pMap,x,z,brush.strength);
					}
				}
				break;
			case EShape_Plane:
				{	// רΪ���ڱ༭������б�µ����߸߶����
					int x,z;
					for(z=brush.centerZ-brush.inner+1;z<brush.centerZ+brush.inner;z++)
					{
						if(z<0 || z>=(int)pMap->Height())
							continue;
						for(x=brush.centerX-brush.inner+1;x<brush.centerX+brush.inner;x++)
						{
							if(x<0 || x>=(int)pMap->Width())
								continue;														
							op.OnInner(pMap,x,z,brush.strength);
						}
					}//end of for(z)
				}
				break;
			}// end of switch
		}

		class BOP_Base
		{
		public:
			BOP_Base(BrushOpRecorder* pBrushRecorder=NULL)
			{
				m_brushOpRecorder=pBrushRecorder;
			}
			~BOP_Base()
			{
				//
			}
			void RecordOp(BrushOpRecorder::EBrushOpMode mode, int x, int z, int undoVal, int redoVal)
			{
				if (m_brushOpRecorder!=NULL)
					m_brushOpRecorder->AddOp(mode,x,z,undoVal,redoVal);
			}
		private:
			BrushOpRecorder	*m_brushOpRecorder;
		};

		/**	��brush��ֵ�ӵ�height map��
		*/
		class BOP_AddBrush : public BOP_Base
		{
		public:
			BOP_AddBrush(BrushOpRecorder* pBrushRecorder=NULL):BOP_Base(pBrushRecorder)
			{
				//
			}
			void OnInner(Heightmap *pMap,int x,int z,int brushVal)
			{
				pMap->SafeAddValue(x,z,brushVal);
				RecordOp(BrushOpRecorder::EBOM_AddVal, x, z, -brushVal, brushVal);
			}
			void OnOuter(Heightmap *pMap,int x,int z,int brushVal,float ratio,ELerpType type)
			{
				pMap->SafeAddValue(x,z,brushVal);
				RecordOp(BrushOpRecorder::EBOM_AddVal, x, z, -brushVal, brushVal);
			}
		};


		/**����Bursh�ھ���Χ�ڸ߶ȵ�ƽ��ֵ
		*/
		class BOP_CalAverage
		{
		public:
			BOP_CalAverage()
			{	count=0;sum=0;		}
			void OnInner(Heightmap *pMap,int x,int z,int burshVal)
			{
				count++;
				sum+=pMap->GetSafeVal(x,z);
			}
			void OnOuter(Heightmap *pMap,int x,int z,int burshVal,float ratio,ELerpType type)
			{}
			int GetResult()
			{
				return sum/count;
			}
			int GetCount()	{return count;}
		private:
			int count;
			int sum;
		};
		/**���ھ���Χ�ڵ�height map����Ϊָ��ֵ,���ھ����⾶֮��ʹ��ָ��ֵ��ԭ��ֵ��ֵ
		*/
		class BOP_ApplyVal : public BOP_Base
		{
		public:
			BOP_ApplyVal(int val,BrushOpRecorder* pBrushRecorder=NULL):BOP_Base(pBrushRecorder)
			{
				m_val=val;
			}
			void OnInner(Heightmap *pMap,int x,int z,int brushVal)
			{
				int oldVal=pMap->GetSafeVal(x,z);
				pMap->SafeSetVal(x,z,m_val);
				RecordOp(BrushOpRecorder::EBOM_SetVal, x, z, oldVal, m_val);
			}
			void OnInner(BitMap *pMap,int x,int z,int brushVal)
			{
				if( brushVal>0 )
					pMap->SafeSetValue(x,z,true);
				else
					pMap->SafeSetValue(x,z,false);
			}
			void OnOuter(Heightmap *pMap,int x,int z,int burshVal,float ratio,ELerpType type)
			{
				int oldVal=pMap->GetSafeVal(x,z);
				int newVal=Lerp(m_val,oldVal,ratio,type);
				pMap->SafeSetVal(x,z,newVal);
				RecordOp(BrushOpRecorder::EBOM_SetVal, x, z, oldVal, newVal);
			}
		private:
			int m_val;
		};

		/**	����һ���߳�Ϊ�⾶*2������ͼ��,���ھ���Χ��ʹ��������ֵ��ԭֵ���,���⾶��Χ��ʹ������ֵ��0��ֵ,����ԭֵ���
		*/
		class BOP_ApplyNoise : public BOP_Base
		{
		public:
			BOP_ApplyNoise(UINT noiseSize,float freq,int startX,int startZ,BrushOpRecorder* pBrushRecorder=NULL)
				:BOP_Base(pBrushRecorder)
			{
				if(fabsf(m_noise.GetFreq()-freq) > 0.01f)//�Ż�:ֻ��Freq�仯ʱ����������,��������϶���ȽϿ�(�϶�brushʱfreq����ı�)
				{
					m_noise.SetFreq(freq);
					m_noise.Generate(noiseSize,noiseSize);
				}
				m_startX=startX;
				m_startZ=startZ;
			}
			void OnInner(Heightmap *pMap,int x,int z,int burshVal)
			{
				int nx=x-m_startX;
				int ny=z-m_startZ;
				if(nx<0
					|| nx>=(int)m_noise.Width()
					|| ny<0
					|| ny>=(int)m_noise.Height())
					return;
				float ns=m_noise.GetData(nx,ny)*255;
				ns-=128;
				ns=ns/128*burshVal;
				int i=ftol_ambient(ns);
				pMap->SafeAddValue(x,z,i);
				RecordOp(BrushOpRecorder::EBOM_AddVal, x, z, -i, i);
			}
			void OnOuter(Heightmap *pMap,int x,int z,int burshVal,float ratio,ELerpType type)
			{
				int nx=x-m_startX;
				int ny=z-m_startZ;
				if(nx<0
					|| nx>=(int)m_noise.Width()
					|| ny<0
					|| ny>=(int)m_noise.Height())
					return;
				float ns=m_noise.GetData(nx,ny)*255;
				ns-=128;
				ns=ns/128*burshVal;
				int i=ftol_ambient(ns);
				pMap->SafeAddValue(x,z,i);
				RecordOp(BrushOpRecorder::EBOM_AddVal, x, z, -i, i);
			}
		private:
			int		m_startX,
					m_startZ;//����ͼ���(0,0)��Ӧheight map�ϵ��ĸ�����
		public:
			static Noise	m_noise;
		};

		/** ���ھ���Χ�ڵ�height mapʹ���Ǿ�����ƽ��,�����ھ��⾶֮���heightmapʹ��ƽ��ֵ��ԭֵ���в�ֵ
			\todo �Ľ�
		*/
		class BOP_SmoothFilter : public BOP_Base
		{
		public:
			BOP_SmoothFilter(int filterSize,BrushOpRecorder* pBrushRecorder=NULL):BOP_Base(pBrushRecorder)
			{
				ASSERT(filterSize > 0);
				size=filterSize;
				pFilter=(float*)malloc(sizeof(float)*filterSize*filterSize);

				float hSize=filterSize/2.0f;
				float r=sqrtf(hSize*hSize*2);
				filterSum=0;
				for(int y=0;y<filterSize;y++)
				{
					int cy=y-filterSize/2;
					for(int x=0;x<filterSize;x++)
					{
						int cx=x-filterSize/2;
						float myR=sqrtf(cx*cx+cy*cy);//��ǰ���뿪���ĵľ���
						float val=Lerp(1.0f,0.0f,myR/r,ELerp_Cosine);
						pFilter[y*filterSize+x]=val;
						filterSum+=val;
					}
				}
			}
			~BOP_SmoothFilter()
			{
				SAFE_FREE(pFilter);
			}
			void OnInner(Heightmap *pMap,int x,int z,int burshVal)
			{
				tagVal tv;
				tv.x=x;
				tv.z=z;
				tv.val=CalSmoothVal(pMap,x,z);
				m_valArray.push_back(tv);
			}

			void OnOuter(Heightmap *pMap,int x,int z,int burshVal,float ratio,ELerpType type)
			{
				float sv=CalSmoothVal(pMap,x,z);
				float ov=pMap->GetSafeVal(x,z);
				tagVal tv;
				tv.x=x;
				tv.z=z;
				tv.val=Lerp(sv,ov,ratio,type);
				m_valArray.push_back(tv);
			}
			void Apply(Heightmap *pMap)
			{
				int valArraySize=(int)m_valArray.size();
				for(int i=0;i<valArraySize;i++)
				{
					tagVal& tv=m_valArray[i];
					int oldVal=pMap->GetSafeVal(tv.x,tv.z);
					pMap->SafeSetVal(tv.x,tv.z,tv.val);
					RecordOp(BrushOpRecorder::EBOM_SetVal, tv.x, tv.z, oldVal, tv.val);
				}
			}
		private:
			struct tagVal
			{
				int x,z;
				int val;
			};
			vector<tagVal>	m_valArray;
			float CalSmoothVal(Heightmap *pMap,int x,int z)
			{
				float sum=0;
				int hSize=size/2;
				for(int fy=0;fy<size;fy++)
				{
					for(int fx=0;fx<size;fx++)
					{
						float f=pFilter[fy*size+fx];
						sum+=pMap->GetSafeVal(x+fx-hSize,z+fy-hSize)*f;
					}
				}
				return sum/filterSum+0.5f;//��������
			}
			int	size;
			float *pFilter;
			float filterSum;
		};

		/**ֻ�ǽ�BOP_SmoothFilter���͵�ELerp_Cosine�ĳ���ELerp_Linear
		*/
		class BOP_LineFilter : public BOP_Base
		{
		public:
			BOP_LineFilter(int filterSize,BrushOpRecorder* pBrushRecorder=NULL):BOP_Base(pBrushRecorder)
			{
				ASSERT(filterSize > 0);
				size=filterSize;
				pFilter=(float*)malloc(sizeof(float)*filterSize*filterSize);

				float hSize=filterSize/2.0f;
				float r=sqrtf(hSize*hSize*2);
				filterSum=0;
				for(int y=0;y<filterSize;y++)
				{
					int cy=y-filterSize/2;
					for(int x=0;x<filterSize;x++)
					{
						int cx=x-filterSize/2;
						float myR=sqrtf(cx*cx+cy*cy);//��ǰ���뿪���ĵľ���
						float val=Lerp(1.0f,0.0f,myR/r,ELerp_Linear);
						pFilter[y*filterSize+x]=val;
						filterSum+=val;
					}
				}
			}
			~BOP_LineFilter()
			{
				SAFE_FREE(pFilter);
			}
			void OnInner(Heightmap *pMap,int x,int z,int burshVal)
			{
				tagVal tv;
				tv.x=x;
				tv.z=z;
				tv.val=CalSmoothVal(pMap,x,z);
				m_valArray.push_back(tv);
			}

			void OnOuter(Heightmap *pMap,int x,int z,int burshVal,float ratio,ELerpType type)
			{
				float sv=CalSmoothVal(pMap,x,z);
				float ov=pMap->GetSafeVal(x,z);
				tagVal tv;
				tv.x=x;
				tv.z=z;
				tv.val=Lerp(sv,ov,ratio,type);
				m_valArray.push_back(tv);
			}
			void Apply(Heightmap *pMap)
			{
				int valArraySize=(int)m_valArray.size();
				for(int i=0;i<valArraySize;i++)
				{
					tagVal& tv=m_valArray[i];
					int oldVal=pMap->GetSafeVal(tv.x, tv.z);
					pMap->SafeSetVal(tv.x,tv.z,tv.val);
					RecordOp(BrushOpRecorder::EBOM_SetVal, tv.x, tv.z, oldVal, tv.val);
				}
			}
		private:
			struct tagVal
			{
				int x,z;
				int val;
			};
			vector<tagVal>	m_valArray;
			float CalSmoothVal(Heightmap *pMap,int x,int z)
			{
				float sum=0;
				int hSize=size/2;
				for(int fy=0;fy<size;fy++)
				{
					for(int fx=0;fx<size;fx++)
					{
						float f=pFilter[fy*size+fx];
						sum+=pMap->GetSafeVal(x+fx-hSize,z+fy-hSize)*f;
					}
				}
				return sum/filterSum+0.5f;//��������
			}
			int	size;
			float *pFilter;
			float filterSum;
		};
#pragma warning( default : 4244 )
	}//namespace Detail
}//namespace Cool3D