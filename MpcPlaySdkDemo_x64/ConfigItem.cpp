#include "ConfigItem.hpp"
#include <cmath>

    CConfigItem::CConfigItem(const int &index, QPixmap *normalPixmap,const int &sceneWidth, const SMonitorRelation &monitorRelation, QGraphicsItem *parent/* = 0*/)
        : QGraphicsPixmapItem(parent)
        , m_sceneWidth(sceneWidth)
        , m_monitorRelation(monitorRelation)
        , m_normalPixmap(normalPixmap)
    {     
        setPixmap(*m_normalPixmap);
        setAcceptDrops(true);

        QSize size=m_normalPixmap->size();
        m_rect=QRectF(0,0,size.width(),size.height());  //由图片大小确定区域大小
        int width=m_rect.width();                       //计算Item的坐标
        int height=m_rect.height();
        int xMaxNum=floor(double(sceneWidth/width));
        int xNum=index%xMaxNum;
        int yNum=ceil(double(index/xMaxNum));
        setPos(QPointF(xNum*width,yNum*height));
        m_monitorRelation.index = index;
    }

    CConfigItem::~CConfigItem()
    {
        if(m_normalPixmap)
            m_normalPixmap=NULL;
    }

    // 获取绘制区域
    QRectF CConfigItem::boundingRect() const
    {
        return m_rect;
    }
    // 重绘函数
    void CConfigItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
    {
        int width  = boundingRect().width();
        int height = boundingRect().height();

        QRectF rect = boundingRect();
        QPixmap pixmap = m_normalPixmap->scaled(width,height,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);;
        painter->fillRect(0,0,rect.width(),rect.height(),QBrush(pixmap));

        QPen pen(Qt::white);

        if (!m_monitorRelation.displayText.empty())
        {
            pen.setColor(Qt::green);
            QString str(m_monitorRelation.displayText.c_str());
            painter->setPen(pen);
            QFont font("Verdana",10, QFont::Bold);
            painter->setFont(font);
            painter->drawText(QRectF(0,0,boundingRect().width(),boundingRect().height()),Qt::AlignCenter,str);
        }
        
        painter->setPen(pen);
        QFont font("Verdana",10, QFont::Bold);
        painter->setFont(font);
        painter->drawText(QRectF(5,3,rect.width(),30),Qt::AlignLeft| Qt::AlignTop,QString::number(m_monitorRelation.index));

        if (zValue())
        {
            pen.setColor(Qt::black);
            painter->setPen(pen);

            pen.setColor(Qt::red);
            painter->setPen(pen);
            painter->drawRect(0,0,boundingRect().width(),boundingRect().height());
        }
    }

    bool CConfigItem::isExistRelation(SMonitorRelation &monitorRelation)
    {
        monitorRelation.monitorId = m_monitorRelation.monitorId;

        if (m_monitorRelation.displayText != monitorRelation.displayText)
        {
            return false;
        }
        return true;
    }

    void CConfigItem::setMonitorRelation (const SMonitorRelation &monitorRelation)
    {
        m_monitorRelation.displayText = monitorRelation.displayText;
        m_monitorRelation.resId = monitorRelation.resId;
        update();
    }

    int CConfigItem::getIndex()
    {
        return m_monitorRelation.index;
    }

    //监视器高亮显示 
    void CConfigItem::setHighLight()
    {
        setItemSelected(true);
        update();
    }

    //取消监视器高亮显示 
    void CConfigItem::unSetHighLight()
    {
        setItemSelected(false);
        update();
    }

    //更改是否选中
    void CConfigItem::setItemSelected(bool isSelected/*=true*/)
    {
        if(isSelected)
            setZValue(1);
        else
            setZValue(0);
    }

    SMonitorRelation & CConfigItem::getMoniorRelation()
    {
        return m_monitorRelation;
    }
