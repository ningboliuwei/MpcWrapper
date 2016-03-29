
#include "DecoderDisplayWidget.hpp"
#include <QtGui/QPainter>

CDeocderDisplayWidget::CDeocderDisplayWidget( QWidget *parent )
                        : QWidget( parent )
                            , m_pYV12Paint( 0 )
                            , m_uYV12PaintLen( 0 )
                            , m_iYV12PaintWidth( 0 )
                            , m_iYV12PaintHeight( 0 )
                            , m_pYV12( 0 )
                            , m_uYV12Len( 0 )
                            , m_iYV12Width( 0 )
                            , m_iYV12Height( 0 )
                            , m_bNeedRePaint( false )
                            , m_pYV12Write( 0 )
                            , m_uYV12WriteLen( 0 )
                            , m_iYV12WriteWidth( 0 )
                            , m_iYV12WriteHeight( 0 )
                            , m_pRGB24( 0 )
                            , m_iRGB24Size( 0 )
                            , m_pConvertVRGB24( 0 )
                            , m_iConvertVRGB24Size( 0 )
                            , m_mutex()
{
}

CDeocderDisplayWidget::~CDeocderDisplayWidget()
{
    if ( m_pYV12Paint )
    {
        delete [] m_pYV12Paint;
        m_pYV12Paint = 0;
        m_uYV12PaintLen = 0;
        m_iYV12PaintWidth = 0;
        m_iYV12PaintHeight = 0;
    }

    if ( m_pYV12 )
    {
        delete [] m_pYV12;
        m_pYV12 = 0;
        m_uYV12Len = 0;
        m_iYV12Width = 0;
        m_iYV12Height = 0;
        m_bNeedRePaint = false;
    }

    if ( m_pYV12Write )
    {
        delete [] m_pYV12Write;
        m_pYV12Write = 0;
        m_uYV12WriteLen = 0;
        m_iYV12WriteWidth = 0;
        m_iYV12WriteHeight = 0;
    }

    if ( m_pRGB24 )
    {
        delete [] m_pRGB24;
        m_pRGB24 = 0;
        m_iRGB24Size = 0;
    }

    if ( m_pConvertVRGB24 )
    {
        delete [] m_pConvertVRGB24;
        m_pConvertVRGB24 = 0;
        m_iConvertVRGB24Size = 0;
    }

}

void CDeocderDisplayWidget::paintEvent(QPaintEvent *event)
{
    {
        // 如果数据有更新，交换 显示缓冲区 和 最新数据缓冲区
        lock_type l( m_mutex );

        if ( m_bNeedRePaint )
        {
            unsigned char   *pYV12Temp = m_pYV12;
            uint32_t        uYV12LenTemp = m_uYV12Len;
            int32_t         iYV12WidthTemp = m_iYV12Width;
            int32_t         iYV12HeightTemp = m_iYV12Height;

            m_pYV12 = m_pYV12Paint;
            m_uYV12Len = m_uYV12PaintLen;
            m_iYV12Width = m_iYV12PaintWidth;
            m_iYV12Height = m_iYV12PaintHeight;
            m_bNeedRePaint = false;

            m_pYV12Paint = pYV12Temp;
            m_uYV12PaintLen = uYV12LenTemp;
            m_iYV12PaintWidth = iYV12WidthTemp;
            m_iYV12PaintHeight = iYV12HeightTemp;
        }
    }

    if ( !m_pYV12Paint )
    {
        // 没有图像，使用父类的paintEvent
        QWidget::paintEvent(event);
        return;
    }

    // 将数据转换成 QImage
    unsigned char   *buf = m_pYV12Paint;
    const int32_t   width = m_iYV12PaintWidth;
    const int32_t   height = m_iYV12PaintHeight;

    // 准备 RGB24 缓存
    const int32_t   newRGB24Size = width*height*3;
    if ( newRGB24Size > m_iRGB24Size )
    {
        if ( m_pRGB24 )
        {
            delete [] m_pRGB24;
            m_pRGB24 = 0;
            m_iRGB24Size = 0;
        }

        m_pRGB24 = new unsigned char[ newRGB24Size ];
        if( !m_pRGB24 )
        {
            return;
        }
        m_iRGB24Size = newRGB24Size;
    }

    memset(m_pRGB24, unsigned char(0), width*height*3);

    // 从YV12映射到RGB24
    if (!YV12ToRGB24(buf, m_pRGB24, width, height))
    {
        return;
    }

    // 准备 convertVRGB24 缓存
    const int32_t   newConvertVRGB24Size = width*height*3;
    if ( newConvertVRGB24Size > m_iConvertVRGB24Size )
    {
        if ( m_pConvertVRGB24 )
        {
            delete [] m_pConvertVRGB24;
            m_pConvertVRGB24 = 0;
            m_iConvertVRGB24Size = 0;
        }

        m_pConvertVRGB24 = new unsigned char[newConvertVRGB24Size];
        if ( !m_pConvertVRGB24 )
        {
            return;
        }
        m_iConvertVRGB24Size = newConvertVRGB24Size;
    }

    memset(m_pConvertVRGB24, unsigned char(0), width*height*3);

    // 翻转RGB24数据
    if( !ConvertRGBV(m_pRGB24, width, height, m_pConvertVRGB24) )
    {
        return;
    }

    // 生成图像
    QImage  tempImage(m_pConvertVRGB24, width, height, QImage::Format_RGB888);

    // 显示数据
    QPainter    painter( this );
    QRect       rect = contentsRect();

    painter.drawImage( rect, tempImage );
}

void CDeocderDisplayWidget::paintYV12Data(const uint8_t *ptrY      ///< Y分量数据
										  , const uint32_t ptrYSize   ///< Y分量数据长度
										  , const uint8_t *ptrV      ///< V分量数据数据
										  , const uint32_t ptrVSize   ///< V分量数据长度
										  , const uint8_t	*ptrU      ///< U分量数据数据
										  , const uint32_t ptrUSize   ///< U分量数据长度
                                           , const int32_t width
                                           , const int32_t height)
{
	uint32_t	uYV12Len = ptrYSize+ptrVSize+ptrUSize;
    if ( uYV12Len > m_uYV12WriteLen )
    {
        if ( m_pYV12Write )
        {
            delete [] m_pYV12Write;
            m_pYV12Write = 0;
            m_uYV12WriteLen = 0;
        }

        m_pYV12Write = new unsigned char[uYV12Len];
        if (!m_pYV12Write)
        {
            return;
        }
        m_uYV12WriteLen = uYV12Len;
        m_iYV12WriteWidth = width;
        m_iYV12WriteHeight = height;
    }

	memcpy(m_pYV12Write, ptrY, ptrYSize);
	memcpy(m_pYV12Write + ptrYSize, ptrV, ptrVSize);
    memcpy(m_pYV12Write + ptrYSize + ptrVSize, ptrU, ptrUSize);

    {
        // 交换写缓冲区 和 最新数据缓存区
        lock_type l( m_mutex );

        unsigned char   *pYV12Temp = m_pYV12;
        uint32_t        uYV12LenTemp = m_uYV12Len;
        int32_t         iYV12WidthTemp = m_iYV12Width;
        int32_t         iYV12HeightTemp = m_iYV12Height;

        m_pYV12 = m_pYV12Write;
        m_uYV12Len = m_uYV12WriteLen;
        m_iYV12Width = m_iYV12WriteWidth;
        m_iYV12Height = m_iYV12WriteHeight;
        m_bNeedRePaint = true;

        m_pYV12Write = pYV12Temp;
        m_uYV12WriteLen = uYV12LenTemp;
        m_iYV12WriteWidth = iYV12WidthTemp;
        m_iYV12WriteHeight = iYV12HeightTemp;
    }

    update();
}


bool CDeocderDisplayWidget::YV12ToRGB24(unsigned char* const pYV12, 
                              unsigned char* const pRGB24, 
                              const int32_t width, 
                              const int32_t height)
{
    if(!pYV12 || !pRGB24)
        return false;

    const long nYLen = long(width * height);
    const int32_t nHfWidth = (width>>1);

    if(nYLen<1 || nHfWidth<1) 
        return false;

    // yv12数据格式，其中Y分量长度为width * height, U和V分量长度都为width * height / 4
    // |WIDTH |
    // y......y--------
    // y......y   HEIGHT
    // y......y
    // y......y--------
    // v..v
    // v..v
    // u..u
    // u..u
    const unsigned char* yData = pYV12;
    const unsigned char* vData = &yData[nYLen];
    const unsigned char* uData = &vData[nYLen>>2];

    if(!uData || !vData)
        return false;

    // Convert YV12 to RGB24
    // 
    // formula
    //                                       [1          1             1       ]
    // [r g b] = [y u-128 v-128]             [0          0.34375       0       ]
    //                                       [1.375      0.703125      1.734375]
    // 
    // another formula
    //                                       [1          1             1       ]
    // [r g b] = [y u-128 v-128]             [0          0.698001      0       ]
    //                                       [1.370705   0.703125      1.732446]
    int rgb[3];
    int i, j, m, n, x, y;
    m = -width;
    n = -nHfWidth;
    for(y=0; y < height; y++)
    {
        m += width;
        if(!(y % 2))
            n += nHfWidth;
        for(x=0; x < width; x++)
        {
            i = m + x;
            j = n + (x>>1);
            //             rgb[2] = int(yData[i] + 1.370705 * (uData[j] - 128)); // r分量值
            //             rgb[1] = int(yData[i] - 0.698001 * (uData[j] - 128)  - 0.703125 * (vData[j] - 128)); // g分量值
            //             rgb[0] = int(yData[i] + 1.732446 * (vData[j] - 128)); // b分量值

            rgb[2] = int(1.164*(yData[i]-16) + 1.596 * (uData[j] - 128)); // r分量值
            rgb[1] = int(1.164*(yData[i]-16) - 0.813 * (uData[j] - 128)  - 0.391 * (vData[j] - 128)); // g分量值
            rgb[0] = int(1.164*(yData[i]-16) + 2.018 * (vData[j] - 128)); // b分量值

            j = nYLen - width - m + x;
            i = (j<<1) + j;
            for(j=0; j<3; j++)
            {
                if(rgb[j]>=0 && rgb[j]<=255)
                {
                    pRGB24[i + j] = rgb[j];
                }
                else
                {
                    pRGB24[i + j] = (rgb[j] < 0) ? 0 : 255;
                }
            }
        }
    }
    return true;
}

bool CDeocderDisplayWidget::ConvertRGBV(unsigned char *const srcData, 
                              const int32_t width, 
                              const int32_t height, 
                              unsigned char *const desData)
{
    unsigned int mLine=((width*3+31) & ~31);

    unsigned char *pbDes = desData;
    unsigned char *pbSrc = srcData;
    pbSrc=pbSrc+(height-1)*mLine;
    for (int i=0; i< height; i++ )
    {
        memcpy(pbDes, pbSrc, mLine);
        pbDes+=mLine;
        pbSrc-=mLine;
    }

    return true;
}

