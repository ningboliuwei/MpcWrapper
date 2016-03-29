#ifndef DECODERDISPLAYWIDGET_HPP_
#define DECODERDISPLAYWIDGET_HPP_

#include <QtGui/QWidget>
#include <QtGui/QImage>
#include <boost/thread.hpp>

class CDeocderDisplayWidget: public QWidget
{
    Q_OBJECT

public:
    CDeocderDisplayWidget( QWidget *parent = 0 );
    ~CDeocderDisplayWidget();

protected:

    /** @brief      显示函数
     */
    virtual void paintEvent(QPaintEvent *event);

public:
    void paintYV12Data( const uint8_t *ptrY      ///< Y分量数据
						, const uint32_t ptrYSize   ///< Y分量数据长度
						, const uint8_t *ptrV      ///< V分量数据数据
						, const uint32_t ptrVSize   ///< V分量数据长度
						, const uint8_t	*ptrU      ///< U分量数据数据
						, const uint32_t ptrUSize   ///< U分量数据长度
                        , const int32_t width
                        , const int32_t height);

private:

    bool CDeocderDisplayWidget::YV12ToRGB24(unsigned char* const pYV12, 
        unsigned char* const pRGB24, 
        const int32_t width, 
        const int32_t height);

    bool CDeocderDisplayWidget::ConvertRGBV(unsigned char *const srcData, 
        const int32_t width, 
        const int32_t height, 
        unsigned char *const desData);

private:

    unsigned char   *m_pYV12Paint;
    uint32_t        m_uYV12PaintLen;
    int32_t         m_iYV12PaintWidth;
    int32_t         m_iYV12PaintHeight;

    unsigned char   *m_pYV12;
    uint32_t        m_uYV12Len;
    int32_t         m_iYV12Width;
    int32_t         m_iYV12Height;
    bool            m_bNeedRePaint;     // 用来标记数据是否有更新

    unsigned char   *m_pYV12Write;
    uint32_t        m_uYV12WriteLen;
    int32_t         m_iYV12WriteWidth;
    int32_t         m_iYV12WriteHeight;

    unsigned char   *m_pRGB24;
    int32_t         m_iRGB24Size;

    unsigned char   *m_pConvertVRGB24;
    int32_t         m_iConvertVRGB24Size;

    typedef boost::unique_lock< boost::mutex > lock_type;
    boost::mutex m_mutex;
};


#endif

