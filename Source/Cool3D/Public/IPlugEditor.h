#pragma once

#include "..\Cool3D\Cool3DEngine.h"
using namespace Cool3D;
/**	\class IPlugEditor
	\brief �༭��PlugIn�Ľӿ�
*/
class IPlugEditor
{
public:
	enum EBuildItems
	{
		EBI_None=0,
		EBI_Terrain=1,//����
		EBI_River=2,//����
		EBI_MapObj=4,//��ͼ����
		EBI_Nav=8,//����ͼ
		EBI_LightMap=16,//��Ӱ��ͼ
		EBI_CompressTex=32,//��ͼѹ��
		EBI_RiverDataToTerrain=64,//��������д�����
		EBI_MiniMap=128,//����ͼ
	};
	struct tagBuildOptions
	{
		//--Lightmap
		bool bLightMapping;//������պ���Ӱ
		bool bRadiosity;//���ܴ���ģ��
		int	 radiosityTimes;//���ܴ��ݴ���
		bool bBlur;//ģ��
		int  blurTimes;//ģ������
		bool bApplyToCombine;//Ӧ�õ��ر�Ԥ�����ͼ
		bool bApplyToDeco;//Ӧ�õ��ر�װ�β�
		bool bAmbientOcclusion;//�������
		bool bSmallAO;//С�����Ƿ�Ӱ�����

		//-navmap
		bool bColliderData;//��ײ����
		bool bNPCNavMap;//NPC���ӵ���ͼ
		bool bNPCNavGraph;//NPC·�㵼��ͼ
		bool bPlayerNavMap;//����Զ�Ѱ·����ͼ
		float fNPCPassHeight;//NPC��ͨ���߶�
		float fNPCNavGraphPassHeight; //NPC·�㵼��ͼ��ͨ���߶�

		//--����ͼ
		int  nMiniMapSize;//����ͼ��С
	};
public:
	IPlugEditor(void) {}
	virtual ~IPlugEditor(void) {}

	/**	����Ҫ�༭�����ݵĽӿ�
	*/
	virtual void AttachEngineInterface(void *pInterface,void *pExInterface=NULL) =0;
	virtual void SetCamera(CameraBase *pCamera) =0;

	virtual void Update(bool bActive) {}
	virtual void Render(bool bActive) {}/**�ڳ�����Ⱦ��Begin(),end()֮�����*/
	virtual void RenderPreview()	{}/**�ڳ�����Ⱦ��Begin(),end()֮�����,��Ҫ�Լ���Begin,End*/

	/**	������ʾ���û����Ĳ������
	*/
	virtual const TCHAR* GetUserName() =0;
	/**	�����ڲ�Class����
	*/
	virtual const TCHAR* GetClassName() =0;
	/**	�ⲿ�������ʱ����һ��
	*/
	virtual void Create(DWORD pParentWnd) =0;
	/**	�ⲿ���ٲ��ʱ����һ��
	*/
	virtual void Destroy() =0;
	/**	��ʾ/���ش˱༭��
	*/
	virtual void ShowWindow(bool bShow) =0;
	/**	��������Ҫ��������Ⱦ���ض��Ĵ����򷵻ش��ھ��,���򷵻ؿ�
	*/
	virtual HWND GetRenderWnd() {	return NULL; }
	virtual bool Build(const TCHAR* szMapName,const TCHAR* szSaveBasePath,DWORD dwItems,tagBuildOptions& options,IUICallBack* pUI)	{	return true;}

	/**	������Ϣ,���������򴰿��յ�����ʱ������Ӧ�ĺ���
	*/
	virtual void OnLButtonDown(UINT nFlags, UINT x,UINT y) =0;
	virtual void OnLButtonUp(UINT nFlags, UINT x,UINT y) =0;
	virtual void OnMouseMove(UINT nFlags, UINT x,UINT y) =0;
	virtual void OnMouseWheel(UINT nFlags, short zDelta, UINT x,UINT y) =0;
	virtual void OnRButtonDown(UINT nFlags, UINT x,UINT y) =0;
	virtual void OnRButtonUp(UINT nFlags, UINT x,UINT y) =0;
};
