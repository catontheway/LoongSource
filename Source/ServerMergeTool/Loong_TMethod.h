#pragma once

#include "Loong_LoginDBCtrl.h"
#include "Loong_LoongDBCtrl.h"
#include "Loong_MergeLogicCtrl.h"
/*
/loong db ͳһ�ϲ�
/1���ٶȿ죬���ڴ�
*/
template<class T>
BOOL TMethod_Loong_ServerMerge(T ,CLoongMergeLogicCtrl &clsMergeLogicCtrl);

/*
/loong db �ֿ��ϲ� 
/1��Դ1��Դ2���߼���ϵ
/2���ٶȿ죬���ڴ�
/2����ǧ��ı�Ҫ�÷�ҳ�ϲ������⴦��
*/
template<class T>
BOOL TMethod_Loong_SeparateServerMerge(T ,CLoongMergeLogicCtrl &clsMergeLogicCtrl);

/*
/loong db ����accountid���û��ϲ� 
/1���ٶ��� ����ʡ�ϲ������ڴ�
*/
template<class T>
BOOL TMethod_Loong_AccountServerMerge(T ,CLoongMergeLogicCtrl &clsMergeLogicCtrl);

/*
/loog db ��ҳ��ѯ�ϲ�
/1��Ҫ��Դ1��Դ2���߼���ϵ
/2���ٶ���
/3����Լ�ڴ�
*/
template<class T>
BOOL TMethod_Loong_PagingServerMerge(T , CLoongMergeLogicCtrl &clsMergeLogicCtrl);

/*
/login db ͳһ�ϲ�
*/
template<class T>
BOOL TMethod_Login_ServerMerge(T ,CLoongMergeLogicCtrl &clsMergeLogicCtrl);



#include "Loong_TMethod.cpp"
