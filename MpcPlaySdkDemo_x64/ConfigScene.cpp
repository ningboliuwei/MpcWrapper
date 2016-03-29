#include <QtGui/QGraphicsScene>
#include <QtGui/QtGui>
#include "ConfigScene.hpp"
#include "../include/Common.hpp"

CConfigScene::CConfigScene(const std::string &path,const int &sceneWidth,QWidget *parent /*= 0*/)
    : QGraphicsScene(parent)
    , m_parent(parent)
    , m_normalpix(0)
{
    m_allConfigItems.clear();
    std::string normalpixPath   = path;
    m_normalpix = new QPixmap(QString(normalpixPath.c_str()));
    m_sceneWidth = sceneWidth;
    m_lastitem = NULL;

    m_stopMenu=new QAction(QObject::tr("Stop Monitor"), this);
    m_scenemenu.addAction(m_stopMenu);
    connect(m_stopMenu, SIGNAL(triggered()), this, SLOT(stopMonitor()));
    //setDragEnabled(true);
}

CConfigScene::~CConfigScene()
{
}

//添加配置屏幕
void CConfigScene::addItems(mpc::nsdk::IManageSession* session, const std::vector<SMonitorRelation> &monitorRelation)
{
    std::vector<CConfigItem*> configItems;
    for(unsigned int i=0;i<monitorRelation.size();++i)
    {
        CConfigItem *item=new CConfigItem(monitorRelation[i].index,m_normalpix,m_sceneWidth, monitorRelation[i]);
        this->addItem(item);
        configItems.push_back(item);
    }
    m_allConfigItems.insert(std::make_pair(session, configItems));

    showItemByServer(session);
}

void CConfigScene::removeAllItems(mpc::nsdk::IManageSession* session)
{
    std::map<mpc::nsdk::IManageSession*, std::vector<CConfigItem*>>::iterator allconfigItemIt = m_allConfigItems.find(session);
    if (allconfigItemIt == m_allConfigItems.end())
    {
        return;
    }
    std::vector<CConfigItem*>::iterator it = allconfigItemIt->second.begin();
    for (; it != allconfigItemIt->second.end() ; ++it)
    {
        this->removeItem(*it);
        if (*it != 0)
        {
            delete *it;
            *it = 0;
        }
    }
    m_lastitem = NULL;
    m_allConfigItems.erase(allconfigItemIt);
}

void CConfigScene::showItemByServer(mpc::nsdk::IManageSession* session)
{
    std::map<mpc::nsdk::IManageSession*, std::vector<CConfigItem*>>::iterator allconfigItemIt = m_allConfigItems.begin();
    for (; allconfigItemIt != m_allConfigItems.end(); ++allconfigItemIt)
    {
        if (allconfigItemIt->first == session)
        {
            std::vector<CConfigItem*>::iterator it = allconfigItemIt->second.begin();
            for (; it != allconfigItemIt->second.end() ; ++it)
            {
                (*it)->show();
            }
        }
        else
        {
            std::vector<CConfigItem*>::iterator it = allconfigItemIt->second.begin();
            for (; it != allconfigItemIt->second.end() ; ++it)
            {
                (*it)->hide();
            }
        }
    }
}

void CConfigScene::hideItemByServer(mpc::nsdk::IManageSession* session)
{
    std::map<mpc::nsdk::IManageSession*, std::vector<CConfigItem*>>::iterator allconfigItemIt = m_allConfigItems.begin();
    for (; allconfigItemIt != m_allConfigItems.end(); ++allconfigItemIt)
    {
        if (allconfigItemIt->first == session)
        {
            std::vector<CConfigItem*>::iterator it = allconfigItemIt->second.begin();
            for (; it != allconfigItemIt->second.end() ; ++it)
            {
                (*it)->hide();
            }
        }
        else
        {
            std::vector<CConfigItem*>::iterator it = allconfigItemIt->second.begin();
            for (; it != allconfigItemIt->second.end() ; ++it)
            {
                (*it)->show();
            }
        }
    }
}

void CConfigScene::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    if (m_lastitem)
    {
        m_lastitem->setZValue(0);
        m_lastitem->update();
        m_lastitem = NULL;
    }
    CConfigItem *item=(CConfigItem *)itemAt(event->scenePos());
    if(item)
    {
        item->setZValue(1);
        item->update();
        m_lastitem = item;
    }
}


void CConfigScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
    CConfigItem *monitorItem=(CConfigItem *)itemAt(contextMenuEvent->scenePos());
    if(monitorItem)
    {
        SMonitorRelation monitorRelation = monitorItem->getMoniorRelation();
        if(monitorRelation.displayText == "")
        {
            m_stopMenu->setEnabled(false);
        }
        else
        {
            m_stopMenu->setEnabled(true);
            m_selectItem = monitorItem;
        }
        
        m_scenemenu.exec(QCursor::pos());
    }
}


CConfigItem * CConfigScene::getCurrentItem()
{
    return m_lastitem == NULL? NULL:m_lastitem;
}


//取消监视器高亮显示,如果监视器操作状
void CConfigScene::unSetMonitorsHighLight()
{
    if (m_lastitem)
    {
        m_lastitem->unSetHighLight();
    }
}

//拖拽鼠标移动
void CConfigScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    CConfigItem *monitorItem=(CConfigItem *)itemAt(event->scenePos());


    unSetMonitorsHighLight();
    if(monitorItem)
    {
        monitorItem->setHighLight();
        m_lastitem = monitorItem;
    }        

    QGraphicsScene::dragMoveEvent(event);
}

//拖拽进入
void CConfigScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setDropAction( Qt::CopyAction );
    event->accept();
    QGraphicsScene::dragEnterEvent(event);
}

void CConfigScene::dropEvent( QGraphicsSceneDragDropEvent *event )
{
    CConfigItem *monitorItem=(CConfigItem *)itemAt(event->scenePos());
    if(monitorItem)
    {
        if (event->mimeData()->hasFormat(DRAGANDDROP_DATATYPE))
        {
            QByteArray itemData = event->mimeData()->data( DRAGANDDROP_DATATYPE );
            QDataStream dataStream( &itemData, QIODevice::ReadOnly );

            QVector<intptr_t>   manageSessionVec;   // 回话链接
            QVector<QString>    resNameVec;         // 资源名字
            QVector<QString>    resIdVec;           // 资源标识
            QVector<bool>       ptzStatusVec;       // 结点云台状态


            dataStream >> manageSessionVec >> resNameVec >> resIdVec >> ptzStatusVec; //保存云台是否可控

            dropEventHandle(manageSessionVec, resNameVec, resIdVec, ptzStatusVec, monitorItem->getIndex());
        }
    }
}

void CConfigScene::dropEventHandle(   const QVector<intptr_t> &manageSessionVec
                                    , const QVector<QString> &resNameVec
                                    , const QVector<QString> &resIdVec
                                    , const QVector<bool> &ptzStatusVec
                                    , const uint32_t &index )
{
    //判断管理回话是否为空
    if (manageSessionVec.empty())
    {
        return;
    }

    std::map<intptr_t, uint32_t> sessionIndex;
    for (int i=0; i<manageSessionVec.size(); ++i)
    {
        std::map<intptr_t, uint32_t>::iterator it = sessionIndex.find(manageSessionVec[i]);
        if (it == sessionIndex.end())
        {
            sessionIndex.insert(std::make_pair(manageSessionVec[i], index));
        }
    }

    for (int i=0; i<manageSessionVec.size(); ++i)
    {
        mpc::nsdk::IManageSession* manageSession = (mpc::nsdk::IManageSession*)manageSessionVec[i];
        if (manageSession)
        {

            std::map<intptr_t, uint32_t>::iterator sessionIt = sessionIndex.find(manageSessionVec[i]);
            if (sessionIt == sessionIndex.end())
            {
                continue;
            }

            //获取对应的CConfigItem
            std::map<mpc::nsdk::IManageSession*, std::vector<CConfigItem*>>::iterator it = m_allConfigItems.find(manageSession);
            if (it == m_allConfigItems.end())
            {
                continue;
            }

            //判断是否已经超出CConfigItem范围
            if (sessionIt->second >= (it->second).size())
            {
                continue;
            }
            SResourceInfo resInfo;

            //将UUID转换为SResId类型
            std::string str(resIdVec[i].toLocal8Bit());
            str2ResId(str, resInfo.resId);

            //获取资源名称
            resInfo.resName = resNameVec[i].toLocal8Bit();

            CConfigItem * item = (it->second).at(sessionIt->second);
            SMonitorRelation monitorRelation = item->getMoniorRelation();
            monitorRelation.displayText = resInfo.resName;
            monitorRelation.resId = resInfo.resId;
            monitorRelation.manageSession = (mpc::nsdk::IManageSession *)manageSession;

            createRelation(item, monitorRelation);
            ++sessionIt->second;
        }
        else
        {
            QMessageBox::warning( m_parent, tr("warning"), tr("manageSession is null!"), tr("ok"));
        }
    }
}


void CConfigScene::createRelation(CConfigItem *configItem, SMonitorRelation &monitorRelation)
{
    mpc::nsdk::IWallController *wall;
    wall = monitorRelation.manageSession->createWallController();
    if (!wall)
    {
        QMessageBox::warning( m_parent, tr("warning"), tr("Get WallController failed!"), tr("ok"));
        return;
    }

    if (wall->schedule( monitorRelation.monitorId, monitorRelation.resId ))
    {
        configItem->setMonitorRelation(monitorRelation);
        std::cout << "\nSchedule-->\nMonitorId: \n";
        printResId(monitorRelation.monitorId);
        std::cout << "\nResId: \n";
        printResId(monitorRelation.resId);
        std::cout << std::endl;
    }
    else
    {
        std::cout << "\nerror:MonitorId: \n";
        printResId(monitorRelation.monitorId);
        QMessageBox::warning( m_parent, tr("warning"), QString::fromLocal8Bit(monitorRelation.displayText.c_str()) + tr("--> SingleSwitch failed!"), tr("ok"));
    }

    wall->release();
    wall = 0;
}

void CConfigScene::delRelation(CConfigItem *configItem, SMonitorRelation &monitorRelation)
{   
    mpc::nsdk::IWallController *wall;
    wall = monitorRelation.manageSession->createWallController();
    if (!wall)
    {
        QMessageBox::warning( m_parent, tr("warning"), tr("Get WallController failed!"), tr("ok"));
        return;
    }

    if(wall->stop(monitorRelation.monitorId))
    {
        monitorRelation.displayText = "";
        configItem->setMonitorRelation(monitorRelation);
        std::cout << "\nStop-->\nMonitorId: \n";
        printResId(monitorRelation.monitorId);
        std::cout << std::endl;
    }
    else
    {
        std::cout << "\nerror:MonitorId: \n";
        printResId(monitorRelation.monitorId);
        QMessageBox::warning( m_parent, tr("warning"), QString::fromLocal8Bit(monitorRelation.displayText.c_str()) + tr("stop SingleSwitch failed!"), tr("ok"));
    }

    wall->release();
    wall = 0;
}


void CConfigScene::stopMonitor()
{
    if (m_selectItem)
    {
        SMonitorRelation monitorRelation = m_selectItem->getMoniorRelation();
        delRelation(m_selectItem, monitorRelation);
    }
}

void CConfigScene::scheduleFromMonitorIndex( mpc::nsdk::IManageSession* session, int index, SResourceInfo &info )
{
	mpc::nsdk::IWallController *wall;
	wall = session->createWallController();
	if (!wall)
	{
		QMessageBox::warning( m_parent, tr("warning"), tr("Get WallController failed!"), tr("ok"));
		return;
	}

	if (!wall->schedule( index, info.resId ))
	{
		QMessageBox::warning( m_parent, tr("warning"), QString::fromLocal8Bit(info.resName.c_str())
                                                        + tr(" --> monitor ")
                                                        + QString("%1").arg(index)
                                                        + tr(" SingleSwitch failed!"), tr("ok"));
	}

	wall->release();
	wall = 0;
}
