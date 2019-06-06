#pragma once
#include "..\vEngine\gui\gui_staticex.h"

const INT MIN_TIPS_WIDTH = 16;
const INT MAX_TIPS_WIDTH = 216;
/** class	ToolTipStatic
	brief	Tips��ʾ��
	remarks ��ʾTips������Tips�Ĵ�С�����ؼ���С
*/
class ToolTipStatic :
	public GUIStaticEx
{
public:
	ToolTipStatic();

	virtual BOOL Resize(tagPoint& ptSize);

	virtual VOID SetText(LPCTSTR szText);

protected:
	/** ���ַ���������items�������ǿؼ��Ĵ�С
	*/
	VOID PreParseText();
	
	/** ���¼���ؼ��Ĵ�С
	*/
	BOOL RecalculateSize(tagPoint& ptSize);

};
