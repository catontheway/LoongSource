#pragma once

//! \class	HttpPicEx
//! \brief	�ܹ�ʵʱ���Ŀؼ�ͼƬURL
//! \author	hyu
//! \date	2009-5-25
//! \base	GUIHttpPic
//! \sa		GUIHttpPic

class HttpPicEx :
    public GUIHttpPic
{
public:
    virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
    virtual VOID Destroy();
    virtual VOID Render();
    virtual VOID Update();

    // ����url
    VOID SetPicURL(const tstring& strFileURL);

    // ���õ�url�޷�����ʱĬ�ϵ�ͼƬ
    VOID SetDefaultPic(const tstring& strFilePath);

    HttpPicEx(void);
    virtual ~HttpPicEx(void);

protected:
    tagGUIImage*        m_pImageDefault;        //< Ĭ��ͼƬ
    tagGUIImage*        m_pUrlPic;              //< URLͼƬ
};
