#pragma once

//--��ײ����ֵ����---------------------------------------------------------
enum ENavResult
{
	ENR_Failed=0,				//����ʧ�ܣ��������ϲ��߼�������
	ENR_Arrive,					//�����յ�
	ENR_Blocking,				//����ס
	ENR_WillDrop,				//������
	ENR_WillVDrop,				//����ֱ����
	ENR_WillSlide,				//������
	ENR_WillSwim,				//����Ӿ
	ENR_WillCarry,				//����½���ؾ���
	ENR_WillOnWater,			//��ˮ��վ��
	ENR_Landed,					//��Ծ��½
	ENR_SwimLanded,				//��Ӿ��ǳ
	ENR_CarryLanded,			//������½
	ENR_ToBeContinued,			//���Լ�����ײ
};
