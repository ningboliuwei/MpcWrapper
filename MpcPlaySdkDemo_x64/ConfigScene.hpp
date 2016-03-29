#ifndef CONFIGSCENE_HPP_
#define CONFIGSCENE_HPP_

#include <vector>
#include <string>
#include <map>
#include <QtGui/QMenu>
#include <QtCore/QObject>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneMouseEvent>

#include "ConfigItem.hpp"
#include "../include/SysTypeDefine.hpp"


   class  CConfigScene:public QGraphicsScene
   {
       Q_OBJECT
   public:
       CConfigScene(const std::string &path,const int &sceneWidth,QWidget *parent = 0);
       ~CConfigScene();

        //添加配置屏幕
       void addItems(mpc::nsdk::IManageSession* session, const std::vector<SMonitorRelation> &monitorRelation);
       void removeAllItems(mpc::nsdk::IManageSession* session);
       
       void hideItemByServer(mpc::nsdk::IManageSession* session);
       void showItemByServer(mpc::nsdk::IManageSession* session);

       CConfigItem * getCurrentItem();
       void createRelation(CConfigItem *configItem, SMonitorRelation &monitorRelation);
       void delRelation(CConfigItem *configItem, SMonitorRelation &monitorRelation);

       void dropEventHandle(   const QVector<intptr_t> &manageSessionVec
           , const QVector<QString> &resNameVec
           , const QVector<QString> &resIdVec
           , const QVector<bool> &ptzStatusVec
           , const uint32_t &index );

	   void scheduleFromMonitorIndex( mpc::nsdk::IManageSession* session, int index, SResourceInfo &info );

   public slots:

       // 停止调度
       void stopMonitor();

   protected:
       virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
       virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent);
       //拖拽鼠标移动
       virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
       virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
       virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

   private:
       //取消监视器高亮显示,如果监视器操作状
       void unSetMonitorsHighLight();

   private:
       QAction                      *m_stopMenu;                                                 // 菜单项
       QMenu                         m_scenemenu;                                                // 全局菜单

       QWidget                      *m_parent;
       int                           m_sceneWidth;
       QPixmap                      *m_normalpix;
       CConfigItem                  *m_lastitem;
       CConfigItem                  *m_selectItem;   
       std::vector<CConfigItem *>    m_highLightMonitor;                                        // 当前正在高亮显示的监视器ID集合
       std::map<mpc::nsdk::IManageSession*, std::vector<CConfigItem*>> m_allConfigItems;

   };

#endif //CONFIGSCENE_HPP_