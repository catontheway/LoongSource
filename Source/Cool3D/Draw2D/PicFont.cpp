#include "StdAfx.h"
#include ".\picfont.h"

#include ".\draw2dImp.h"
#include "..\Util\Exception.h"
#include "..\Device\IDevice.h"
#include "..\Device\IDeviceTex.h"
#include "..\Kernel.h"
#include "..\ResSys\ResTexture.h"


namespace Cool3D
{
	class PicFont::Member
	{
	public:
		ResTexture*		m_pASCIITex;		// ����ASCII����ŵ�ͼƬ����
		int				m_nTextHeight;		// ��Ҫ�������ֵĸ߶�
		int				m_nTextWidth;		// ��������ֵĿ��
		int				m_nSpaceWidth;		// �ַ�֮��ļ��
		int				m_nSpaceHeight;		// ������֮��ļ��
		bool			m_bCreated;			// ����ͼƬ�����Ƿ񴴽��ɹ�

		Member()	
		{
			m_bCreated = false;
			m_nSpaceHeight = 8;
			m_nSpaceWidth = 2;
			m_nTextWidth = 16;
			m_nTextHeight = 16;
			m_pASCIITex = NULL;
		}

		ResTexture* GetASCIITex()
		{
			if(m_pASCIITex)
				return m_pASCIITex;
			return NULL;
		}

		~Member()	
		{
		}
	};

	PicFont::PicFont(void)
	{
		m_p=new Member;
	}

	PicFont::~PicFont(void)
	{
		delete m_p;
	}

	void PicFont::Destroy()
	{
		if (m_p->m_pASCIITex!=NULL)	// �ͷ�������Դ
		{
			IDraw2D::Inst()->ReleaseTexture(m_p->m_pASCIITex);
		}	
		m_p->m_pASCIITex=NULL;
	}

	bool PicFont::Create(const TCHAR* szPicFileName,IFS *pFS)
	{
		if (m_p->m_bCreated) return true;
		//����һ��ͼƬ���������
		m_p->m_pASCIITex = IDraw2D::Inst()->NewTexture(szPicFileName,pFS);
		
		if (m_p->m_pASCIITex)
		{
			m_p->m_bCreated = true;		// �����ɹ�
			return true;
		}
		else 
		{
			m_p->m_bCreated = false;	// ����ʧ��
			return false;
		}
	}

	void PicFont::SetDrawCharSize(const int w/* =16 */,const int h/* =16 */)
	{	// ���ֵĿ�Ⱥ͸߶ȶ�����0��ʱ��Ϊ�Ϸ������ã����򱣳�Ĭ��ֵ16����
		if(w>=0)		m_p->m_nTextWidth  = w;
		
		if(h>=0)		m_p->m_nTextHeight = h;		
	}

	void PicFont::SetSpaceSize(const int w /* =2 */, const int h /* =8 */)
	{
		if(w>=0)		m_p->m_nSpaceWidth  = w;

		if(h>=0)		m_p->m_nSpaceHeight = h;
	}
	
	void PicFont::SetDrawCharHeight(const int nH)
	{
		if (nH>=0) m_p->m_nTextHeight = nH;			
	}

	void PicFont::SetDrawCharWidth(const int nW)
	{
		if(nW>=0) m_p->m_nTextWidth  = nW;
	}

	void PicFont::SetSpaceWidth(const int w)
	{
		if(w>=0) m_p->m_nSpaceWidth  = w;
	}

	void PicFont::SetSpaceHeight(const int h)
	{
		if (h>=0) m_p->m_nSpaceHeight = h;			
	}

	
	int PicFont::GetDrawCharWidth()
	{
		return m_p->m_nTextWidth;
	}

	int PicFont::GetDrawCharHeight()
	{
		return m_p->m_nTextHeight;
	}

	int PicFont::GetSpaceWidth()
	{
		return m_p->m_nSpaceWidth;
	}

	int PicFont::GetSpaceHeight()
	{
		return m_p->m_nSpaceHeight;
	}

	const TCHAR* PicFont::GetASCIITexPath() const
	{
		if(!m_p->m_bCreated || m_p->m_pASCIITex == NULL)
			return NULL;

		return (m_p->m_pASCIITex->GetName());
	}


	//------------------------------------------------
	// ������Ҫ��ASCII���ַ���
	//------------------------------------------------
	void PicFont::DrawString(const TCHAR* szText,int x,int y,float z,DWORD dwColor,DWORD dwTextEffectColor,int nCount)
	{
		if (m_p->m_bCreated==false || m_p->m_pASCIITex==NULL)		//����δ�����ɹ�
			return;
		if(szText ==NULL)	return;			//��Ҫ���Ƶ��ַ���Ϊ��
		
		IDevice *pDev = Device();

		if(x<0 || x>= (int)pDev->GetClientWidth())	return;		// ������Ƶ����㲻����Ļ��Χ���˳�
		if(y<0 || y>= (int)pDev->GetClientHeight())	return;

		RECT destRect,srcRect;				//��Ļ���������ͼƬ��������
		const TCHAR *p = szText;			//�����ַ�ָ��
		int nTotal = nCount;				//��������ַ����ĳ���
		int nLength = _tcslen( szText );	
		if( nTotal < 0 || nTotal > nLength )//nCountΪ-1���ߴ����ַ����ĳ���,���������ַ���
		{
			nTotal = nLength;
		}
		const TCHAR* pEnd = p + nTotal;		//�ַ���������ָ��

		int xOrg = x;							//���Ƶ���ʼ��x
		int yOrg = y;							//���Ƶ���ʼ��y
		int nTexCharWidth  = (m_p->m_pASCIITex->GetDeviceTex()->Width())/16;		//��Ӧ������ÿ���ַ����
		int nTexCharHeight = (m_p->m_pASCIITex->GetDeviceTex()->Height())/8;		//��Ӧ������ÿ���ַ��߶�
		int nLineCharCount = 0;					//һ�����ַ�����λ��

		for( ; p<pEnd; )						// �����ַ����е�ÿһ���ַ�
		{
			if( TCHAR(*p) > 0 && TCHAR(*p) < 128)//ASCII(0~127)
			{
				if( *p == _T('\n') )	//�س�����
				{
					xOrg = x;
					yOrg += m_p->m_nTextHeight + m_p->m_nSpaceHeight;
					p++;
					nLineCharCount = 0;
					continue;
				}
				if( *p == _T('\t') )	//Tabռ�ĸ��ַ�
				{
					p++;
					nLineCharCount +=4;	
					xOrg += ((m_p->m_nSpaceWidth + m_p->m_nTextWidth)<<2);				
					continue;
				}
				//todo:��Բ�ͬ��ͼƬ����ͬ������
				//������õ���16*8�ַ�������ͼƬ
				int cx=((*p)%16)*nTexCharWidth;		// X Position Of Current Character in Texture
				int cy=((int)((*p)/16))*nTexCharHeight;		// Y Position Of Current Character in Texture
				
				if(cy<0 || cy>=8*nTexCharHeight) continue;		//���Ϸ�������ͼƬλ��

				//��Ļ�ϻ��Ƶ�λ��
				destRect.left	= xOrg;
				destRect.top	= yOrg;
				destRect.right	= xOrg + m_p->m_nSpaceWidth + m_p->m_nTextWidth;
				destRect.bottom	= yOrg + m_p->m_nSpaceHeight + m_p->m_nTextHeight;
				//��Ӧ�����������ָ���������������
				srcRect.left	= cx;
				srcRect.top		= cy;
				srcRect.bottom	= (cy + nTexCharWidth);
				srcRect.right	= (cx + nTexCharHeight);

				//��ߴ���
				if((dwTextEffectColor&0xFF000000)!=0x00000000)
				{
					//IDraw2D::Inst()->DrawWithZ(destRect,srcRect,m_p->m_pASCIITex,z,dwTextEffectColor);
					RECT rcEffect;
					
					//rcEffect.left	= destRect.left-1;
					//rcEffect.top	= destRect.top-1;
					//rcEffect.right	= destRect.right-1;
					//rcEffect.bottom	= destRect.bottom-1;	
					//IDraw2D::Inst()->DrawWithZ(rcEffect,srcRect,m_p->m_pASCIITex,z,dwTextEffectColor);
					
					rcEffect.left	= destRect.left+1;
					rcEffect.top	= destRect.top+1;
					rcEffect.right	= destRect.right+1;
					rcEffect.bottom	= destRect.bottom+1;	
					IDraw2D::Inst()->Draw(&rcEffect,&srcRect,m_p->m_pASCIITex,dwTextEffectColor,ETFilter_LINEAR,z);
					
					rcEffect.left	= destRect.left+1;
					rcEffect.top	= destRect.top-1;
					rcEffect.right	= destRect.right+1;
					rcEffect.bottom	= destRect.bottom-1;	
					IDraw2D::Inst()->Draw(&rcEffect,&srcRect,m_p->m_pASCIITex,dwTextEffectColor,ETFilter_LINEAR,z);
					
					rcEffect.left	= destRect.left-1;
					rcEffect.top	= destRect.top+1;
					rcEffect.right	= destRect.right-1;
					rcEffect.bottom	= destRect.bottom+1;	
					IDraw2D::Inst()->Draw(&rcEffect,&srcRect,m_p->m_pASCIITex,dwTextEffectColor,ETFilter_LINEAR,z);
				}

				IDraw2D::Inst()->Draw(&destRect,&srcRect,m_p->m_pASCIITex,dwColor,ETFilter_LINEAR,z);

				p++;

				nLineCharCount++;
				// �������һ����ʾ���µ����
				if( nLineCharCount >= 
					((int)pDev->GetClientWidth()-x)/(m_p->m_nTextWidth + m_p->m_nSpaceWidth) )
				{
					nLineCharCount = 0;
					xOrg = x;
					yOrg += m_p->m_nTextHeight + m_p->m_nSpaceHeight;
				}
				else
				{
					xOrg += m_p->m_nTextWidth + m_p->m_nSpaceWidth;
				}
			}
			else		// ��ASCII(0~127)���ַ�,���账��
				continue;
		}
	}




	
	class PicFontEx::Member
	{
	public:
		map<TCHAR,ResTexture*> m_mapTex;     //�ַ���Ӧ������
		int				m_nSpaceWidth;		// �ַ�֮��ļ��
		int				m_nSpaceHeight;		// ������֮��ļ��

		Member()
		{
			m_nSpaceWidth=0;
			m_nSpaceHeight=0;
		}
		~Member()
		{
		}
   	};

	PicFontEx::PicFontEx()
	{
		m_p=new Member;
	}
	PicFontEx::~PicFontEx(void)
	{
		delete m_p;
	}
	void PicFontEx::Destroy()
	{
		// �ͷ�������Դ
		map<TCHAR,ResTexture*>::iterator it=m_p->m_mapTex.begin();
		for (;it!=m_p->m_mapTex.end();it++)
		{
			IDraw2D::Inst()->ReleaseTexture((*it).second);
			(*it).second=NULL;
		}
		m_p->m_mapTex.clear();
	}

	bool PicFontEx::LoadPic(TCHAR ch,const TCHAR* szPicFileName,IFS *pFS)
	{
		if (!m_p->m_mapTex.empty()&& m_p->m_mapTex.find(ch)!=m_p->m_mapTex.end())
		{
			//�Ѿ�ʹ�ù����ַ�������ͼƬ
			return false;
		}

		//���ض�Ӧ��ͼƬ,�����浽map��
		m_p->m_mapTex[ch]=IDraw2D::Inst()->NewTexture(szPicFileName,pFS);

		return true;
	}

	void PicFontEx::DrawString(const TCHAR* szText,int x,int y,float z,
		DWORD dwColor/* =0xFFFFFFFF */,int nCount/* =-1 */,int nStyle,float fZoom)
	{
		if(szText==NULL)return;

		int length=_tcslen(szText);
		if(nCount!=-1)
		{
			length=length>nCount?nCount:length;
		}
		//�������ֶ��뷽ʽ 
		CalStringPos(szText,nStyle,x,y);

		RECT destRect,srcRect;					//��Ļ���������ͼƬ��������
		int xOrg = x;							//���Ƶ���ʼ��x
		int yOrg = y;	
		TCHAR c;
		map<TCHAR,ResTexture*>::iterator it;
		int texW=0,texH=0;
		for (int i=0;i<length;i++)
		{
			c=szText[i];

			//���������ַ�
			if(c==_T('\n'))//����
			{
				yOrg += (int)(texH*fZoom) + m_p->m_nSpaceHeight;
				xOrg = x;
				texH=0;
				continue;
			}
			if(c==_T('\t'))	//Ĭ�Ͽ�һ���������ⲿ����
			{
				xOrg += 1*((int)(texW*fZoom) + m_p->m_nSpaceHeight);
				continue;
			}

			//�����Ƿ���map��
			it=m_p->m_mapTex.find(c);
			if(it==m_p->m_mapTex.end())
			{
				continue;	//û�б���
			}

			texW=((*it).second)->GetDeviceTex()->Width();
			int h=((*it).second)->GetDeviceTex()->Height();
			texH=h>texH?h:texH;

			//����Ļ��Χ�ڲ�����
			if(xOrg>=(int)Device()->GetClientWidth())
			{
				continue;
			}
			if(yOrg>=(int)Device()->GetClientHeight())
			{
				continue;
			}
			//��Ļ�ϻ��Ƶ�λ��
			destRect.left	= xOrg;
			destRect.top	= yOrg;
			destRect.right	= xOrg + (LONG)(texW*fZoom);
			destRect.bottom	= yOrg + (LONG)(texH*fZoom);
			//��Ӧ�����������ָ���������������
			srcRect.left	= 0;
			srcRect.top		= 0;
			srcRect.bottom	= texH;
			srcRect.right	= texW;
			IDraw2D::Inst()->Draw(&destRect, &srcRect, (*it).second,dwColor, ETFilter_POINT, 0, IDraw2D::EAWT_Write, z);

			//������ʼλ��
			xOrg += (int)(texW*fZoom) +m_p->m_nSpaceWidth;
		}
	}

	void PicFontEx::SetSpaceSize(const int w /* =2 */, const int h /* =2 */)
	{
		m_p->m_nSpaceWidth=w;
		m_p->m_nSpaceHeight=h;
	}
	void PicFontEx::SetSpaceWidth(const int w)
	{
		m_p->m_nSpaceWidth=w;
	}
	void PicFontEx::SetSpaceHeight(const int h)
	{
		m_p->m_nSpaceHeight=h;
	}
	int PicFontEx::GetSpaceHeight()
	{
		return m_p->m_nSpaceHeight;
	}
	int PicFontEx::GetSpaceWidth()
	{
		return m_p->m_nSpaceWidth;
	}

	void PicFontEx::GetDrawStringSize(const TCHAR* szText, int& nWidth, int& nHeight)
	{
		ASSERT(szText!=NULL);
		int length=_tcslen(szText);
		int xWidth=0;//�п��
		int xOrg = 0;//���Ƶ���ʼ��x
		TCHAR c;
		map<TCHAR,ResTexture*>::iterator it;
		int texW=0, texH =0;
		for (int i=0;i<length;i++)
		{
			c=szText[i];

			//���������ַ�
			if(c==_T('\n'))//����
			{
				xOrg = 0;//����
				continue;
			}
			if(c==_T('\t'))
				continue;

			//�����Ƿ���map��
			it=m_p->m_mapTex.find(c);
			if(it==m_p->m_mapTex.end())
			{
				continue;	//û�б���
			}

			texW=((*it).second)->GetDeviceTex()->Width();
			INT nH=((*it).second)->GetDeviceTex()->Height();
			if( nH > texH )
				texH = nH;
			ASSERT(texW!=0);
			xOrg += texW;
			xWidth = xWidth>xOrg?xWidth:xOrg;
		}

		nWidth = xWidth;
		nHeight = texH;
	}

	void PicFontEx::CalStringPos(const TCHAR* szText,int nStyle ,int& x,int& y)
	{
		if(nStyle==0)//���������Բ���Ҫ����
			return;
	
		int nWidth, nHeight;
		this->GetDrawStringSize(szText, nWidth, nHeight);

		switch(nStyle)
		{
		case 1://���ж���
			x -= nWidth/2;
			break;
		case 2://���Ҷ���
			x -= nWidth;
			break;
		default:
			break;
		}
	}
	
}//namespace Cool3D