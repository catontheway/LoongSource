#pragma once
#include "Cool3D.h"

namespace Cool3D
{
	/** \class Cool3DOption
		\brief ���滭��ѡ��
		\remarks
			\par ����NormalMap���Զ���HWSkin
	*/
	class Cool3D_API Cool3DOption
	{
	public:
		enum EOption		
		{
			//--���洴��ǰָ���Ļ���ѡ��
			EO_LightMap,		//��ʵ��Ӱ
			EO_TerrainDetail,	//����ϸ��
			EO_Highlight,		//�������	
			EO_NormalMap,		//������ͼ
			EO_HWSkin,			//GPU����
			EO_GrassAni,		//�ݶ���
			EO_TexDetail,		//��ͼ����
			EO_TerrainSpecular, //���θ���
			EO_WaterSpecular,	//ˮ�����

			//--��������Ϸ����ʱ���ĵĻ���ѡ��
			EO_WaterReflect,	//ˮ�淴��Ч��
			EO_WaterRefract,	//ˮ������Ч��
			EO_Music,			//���ֿ���
			EO_Sound,			//��Ч����
			EO_DebugInfo,		//������Ϣ
			EO_DrawGrass,		//������ʾ
			EO_Num				
		};

		enum ELightMap			//����ͼ
		{
			ELM_None,			//��
			ELM_Open,			//��
			ELM_End
		};
			
		enum ETerrainDetail		//����ϸ��
		{
			ETD_Precombine,		//Ԥ�����ͼ(�ѵ���lightmap)
			ETD_Editor,			//Ԥ�����ͼ(δ����lightmap)+LightMap
			ETD_TwoLayers,		//Ԥ�����ͼ(�ѵ���lightmap)+ϸ������
			ETD_MultiLayers,	//��������
			ETD_End
		};

		enum ETerrainSpecular	//���θ߹�
		{
			ETS_None,			//��
			ETS_Open,			//��
			ETS_End
		};

		enum EHighlight			//�������
		{
			EHL_None,			//�� 
			EHL_Open,			//��
			EHL_End
		};

		enum ENormalMap			//������ͼ
		{
			ENM_None,			//��
			ENM_Open,			//��
			ENM_End
		};

		enum EHWSkin			//Ӳ��������Ƥ����
		{
			EHWS_None,			//��
			EHWS_Open,			//��
			EHWS_End
		};

		enum EGrassAni			//�ݶ���
		{
			EGA_None,			//��
			EGA_Open,			//��
			EGA_End
		};

		enum EWaterReflect		//ˮ�淴��Ч��
		{
			EWR_None,			//��
			EWR_Mid,			//��
			EWR_High,			//��
			EWR_End
		};

		enum EWaterRefract		//ˮ������Ч��
		{
			EWRR_None,			//��
			EWRR_Open,			//��
			EWRR_End
		};

		enum ETexDetail			//��ͼ����
		{
			ETexDetail_High,	//��
			ETexDetail_Mid,		//��
			ETexDetail_Low,		//��
			ETexDetail_End
		};

		enum EWaterSpecular		//ˮ��߹�
		{
			EWS_None,			//��
			EWS_Open,			//��
			EWS_End
		};

		enum EMusic				//����
		{
			EM_None,			//��
			EM_Open,			//��
			EM_End
		};

		enum ESound				//��Ч
		{
			ES_None,			//��
			ES_Open,			//��
			ES_End
		};

		enum EDebugInfo			//������Ϣ
		{
			EDI_None,			//��
			EDI_Open,			//��
			EDI_End
		};

		enum EDrawGrass			//������ʾ
		{
			EDG_None,			//��
			EDG_Open,			//��
			EDG_End
		};

	public:
		/** �п�����ΪӲ����֧�ֶ�ʧ��
		*/
		bool SetOption(EOption opt,int val);
		int  GetOption(EOption opt);
		bool IsSupport(EOption opt,int val);
		/** ��֤���洴��ǰָ����ѡ���Ƿ�Ϸ�
		*/
		void CheckCreateOptions();

		/** �༭��ģʽ�¿�����runtime�޸��κ�ѡ��
		*/
		void SetEditorMode(bool bEditorMode);

		static Cool3DOption* Inst();
	private:
		Cool3DOption();
		~Cool3DOption();

		bool			m_bEditorMode;
		int				m_options[EO_Num];
	};

}//namespace Cool3D