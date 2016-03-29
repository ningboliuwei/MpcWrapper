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

    //������������ʾ 
    void setHighLight();

    //ȡ��������������ʾ 
    void unSetHighLight();

    int getIndex();

    //�����Ƿ�ѡ��
    void setItemSelected(bool isSelected/*=true*/);

    SMonitorRelation &getMoniorRelation();

protected:
    // ��ȡ��������
    QRectF boundingRect() const;
    // �ػ溯��
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
    int m_sceneWidth;
    QPixmap *m_normalPixmap;
    QRectF m_rect;
    QString m_resName;
    SMonitorRelation m_monitorRelation;

};
#endif //CONFIGITEM_HPP_