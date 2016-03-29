#ifndef CONFIGITEM_HPP_
#define CONFIGITEM_HPP_

#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtGui/QGraphicsPixmapItem>
#include "../include/SysTypeDefine.hpp"


class  CConfigItem:public QGraphicsPixmapItem
{
public:
    CConfigItem(const int &index, QPixmap *normalPixmap,const int &sceneWidth, const SMonitorRelation &monitorRelation, QGraphicsItem *parent = 0);
    ~CConfigItem();

    bool isExistRelation(SMonitorRelation &monitorRelation);

    void setMonitorRelation (const SMonitorRelation &monitorRelation);

    //监视器高亮显示 
    void setHighLight();

    //取消监视器高亮显示 
    void unSetHighLight();

    int getIndex();

    //更改是否选中
    void setItemSelected(bool isSelected/*=true*/);

    SMonitorRelation &getMoniorRelation();

protected:
    // 获取绘制区域
    QRectF boundingRect() const;
    // 重绘函数
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
    int m_sceneWidth;
    QPixmap *m_normalPixmap;
    QRectF m_rect;
    QString m_resName;
    SMonitorRelation m_monitorRelation;

};
#endif //CONFIGITEM_HPP_