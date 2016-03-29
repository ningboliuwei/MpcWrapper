#include <Winsock2.h>
#include <time.h> 
#include <iostream>
#include <QtCore/QTextStream>
#include <QtGui/QApplication>
#include <QtGui/QHeaderView>
#include <QtGui/QScrollBar>
#include <QtGui/QMessageBox>
#include <QtCore/QFile>
#include "ResourceTreeWidget.hpp"
#include "LoginWnd.hpp"
#include "MonitorIndexScheduleWnd.hpp"
#include "PwdModifyWnd.hpp"
#include "MulLivingPlayerTab.hpp"
#include "ReplayTab.hpp"
#include "AlarmTab.hpp"
#include "MainWindow.hpp"
#include "../include/Common.hpp"

#include "AlarmRegRecord.hpp"
#include "md5.h"
#include <algorithm>


const int   RESTREE_COLUMN_INDEX_RESNAME    = 0;
const int   RESTREE_COLUMN_INDEX_CODE       = 0;
const int   RESTREE_COLUMN_INDEX_SECTION    = 0;
const int   RESTREE_COLUMN_INDEX_LANDMARK   = 0;
const int   RESTREE_COLUMN_INDEX_DIRECTION  = 0;

const char  RESTREE_COLUMN_NAME_RESNAME[]   = "Resource Tree";
const char  RESTREE_COLUMN_NAME_CODE[]      = "code";
const char  RESTREE_COLUMN_NAME_SECTION[]   = "section";
const char  RESTREE_COLUMN_NAME_LANDMARK[]  = "landmark";
const char  RESTREE_COLUMN_NAME_DIRECTION[] = "direction";

extern bool isSupportTestSdk;

QTreeWidgetItem *g_lastCmsItem = NULL;         //上一次单击时CMS节点

bool operator == (const SCmsLoginInfo &firstInfo, const SCmsLoginInfo &secondInfo)
{
    if (firstInfo.server != secondInfo.server)  return false;
    if (firstInfo.port != secondInfo.port ) return false;
    if (firstInfo.userName != secondInfo.userName ) return false;

    return true;
}

bool operator == (const SMonitorRelation &info, int index)
{
    return info.index == index;
}

/* Convert byte array to hex string. */
std::string bytesToHexString(const uint8_t* input, size_t length) 
{
    std::string str;
    str.reserve(length << 1);
    for (size_t i = 0; i < length; ++i) {
        int t = input[i];
        int a = t / 16;
        int b = t % 16;
        str.append(1, HEX[a]);
        str.append(1, HEX[b]);
    }
    return str;
}

CResourceTreeWidget::CResourceTreeWidget( QWidget *parent)
                        : QTreeWidget(parent)
                        , m_ItemMenu(NULL)
                        , m_addcmsMenu(NULL)
                        , m_addCmsAction(NULL)
                        , m_loginAction(NULL)
                        , m_logoutAction(NULL)
                        , m_delAction(NULL)
                        , m_propertyAction(NULL)
						, m_scheduleMonitIndex( 0 )
						, m_mainWindow( (CMainWindow*)parent )
{
    m_allMonitorRelation.clear();
    m_ItemRescourceInfo.clear();
    verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    horizontalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);

    this->header()->setMinimumSectionSize(80);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);  

    //设置table的水平滚动条
    QHeaderView *pHeader=header();  
    pHeader->setResizeMode(QHeaderView::ResizeToContents);  
    pHeader->setStretchLastSection(false);

#ifdef _DEBUG
    m_lastinfo.userName = "0";
    m_lastinfo.password = "000000";
    m_lastinfo.server = "192.168.3.144";
    m_lastinfo.port = "10000";
#endif
}

CResourceTreeWidget::~CResourceTreeWidget()
{
    //fini();
}

bool CResourceTreeWidget::init()
{
    setColumnCount(1);
    setHeaderLabel(tr("Resource Tree"));

    setDragEnabled( true );
    setDragDropMode( QAbstractItemView::DragOnly );
    setSelectionMode( QAbstractItemView::ExtendedSelection );

    //创建空白去右键菜单用于添加CMS
    m_addcmsMenu = new QMenu();
    m_addCmsAction = new QAction(this);

    //添加CMS节点右键菜单，用于登陆、登出、删除操作
    m_ItemMenu          = new QMenu();
    m_loginAction       = new QAction(this);
    m_logoutAction      = new QAction(this);
    m_modifyPwdAction   = new QAction(this);
    m_delAction         = new QAction(this);
    m_propertyAction    = new QAction(this);
	m_scheduleMonitIndex = new QAction( this );

    //设置菜单项显示文字
    m_addCmsAction->setText(tr("Add CMS"));
    m_loginAction->setText(tr("Login"));
    m_logoutAction->setText(tr("Logout"));
    m_modifyPwdAction->setText(tr("ModifyPwd"));
    m_delAction->setText(tr("del CMS"));
    m_propertyAction->setText(tr("Property"));
	m_scheduleMonitIndex->setText( tr( "SingleScheduleFromMoniterIndex" ) );

    //设置登出菜单不可用
    m_logoutAction->setEnabled(false);
    m_modifyPwdAction->setEnabled(false);

    //连接信号与槽
    QObject::connect(m_addCmsAction, SIGNAL( triggered() ), this, SLOT(AddCmsItem()));
    QObject::connect(m_loginAction, SIGNAL(triggered()), this, SLOT(login()));
    QObject::connect(m_logoutAction, SIGNAL(triggered()), this, SLOT(logout()));
    QObject::connect(m_modifyPwdAction, SIGNAL(triggered()), this, SLOT(modifyPwd()));
    QObject::connect(m_delAction, SIGNAL(triggered()), this, SLOT(delCmsItem()));
    QObject::connect(m_propertyAction, SIGNAL(triggered()), this, SLOT(propertyCmsItem()));
	QObject::connect(m_scheduleMonitIndex, SIGNAL(triggered()), this, SLOT(singleScheduleMoniterIndex()));

    QObject::connect( this, SIGNAL( itemClicked( QTreeWidgetItem *, int ) ), this, SLOT( slotItemClicked( QTreeWidgetItem *, int )));
    QObject::connect( this, SIGNAL( itemDoubleClicked( QTreeWidgetItem *, int ) ), this, SLOT( slotItemDoubleCliecked( QTreeWidgetItem *, int )));

    //读取硬盘文件缓存
    readHistoryLoginInfo();

    return true;
}


bool CResourceTreeWidget::fini()
{
    //将当前CMS登陆信息保存到硬盘文件
    saveLoginInfo();

    //连接信号与槽
    QObject::disconnect(m_addCmsAction, SIGNAL( triggered() ), this, SLOT(AddCmsItem()));
    QObject::disconnect(m_loginAction, SIGNAL(triggered()), this, SLOT(login()));
    QObject::disconnect(m_logoutAction, SIGNAL(triggered()), this, SLOT(logout()));
    QObject::disconnect(m_modifyPwdAction, SIGNAL(triggered()), this, SLOT(modifyPwd()));
    QObject::disconnect(m_delAction, SIGNAL(triggered()), this, SLOT(delCmsItem()));
    QObject::disconnect(m_propertyAction, SIGNAL(triggered()), this, SLOT(propertyCmsItem()));

    QObject::disconnect( this, SIGNAL( itemClicked( QTreeWidgetItem *, int ) ), this, SLOT( slotItemClicked( QTreeWidgetItem *, int )));
    QObject::disconnect( this, SIGNAL( itemDoubleClicked( QTreeWidgetItem *, int ) ), this, SLOT( slotItemDoubleCliecked( QTreeWidgetItem *, int )));

    std::map<QTreeWidgetItem*, mpc::nsdk::IManageSession*>::iterator manageSessionIt = m_manageSessionInfo.begin();
    for(; manageSessionIt != m_manageSessionInfo.end(); ++manageSessionIt)
    {
        manageSessionIt->second->release();
        manageSessionIt->second = 0;
    }

    m_manageSessionInfo.clear();

	std::vector<mpc::nsdk::IManageSession*>::iterator delSessionIt = m_delManageSession.begin();
	for(; delSessionIt != m_delManageSession.end(); ++delSessionIt)
	{
		(*delSessionIt)->release();
		(*delSessionIt) = 0;
	}

	m_delManageSession.clear();

    return true;
}

void CResourceTreeWidget::contextMenuEvent( QContextMenuEvent *event )
{
    QTreeWidgetItem* curItem=itemAt(event->pos());  //获取当前被点击的节点  
    if(curItem!=NULL)
    {
        if (curItem->parent() == NULL)
        {
            m_ItemMenu->clear();
            m_ItemMenu->addAction(m_loginAction);
            m_ItemMenu->addAction(m_logoutAction);
            m_ItemMenu->addAction(m_modifyPwdAction);
            m_ItemMenu->addAction(m_delAction);
            m_ItemMenu->addAction(m_propertyAction);

            std::map<QTreeWidgetItem*, mpc::nsdk::IManageSession*>::iterator it = m_manageSessionInfo.find(curItem);
            if (it != m_manageSessionInfo.end())
            {
                if(it->second != 0)
                {
                    m_loginAction->setEnabled(false);
                    m_logoutAction->setEnabled(true);
                    m_modifyPwdAction->setEnabled(true);
                }
                else
                {
                    m_loginAction->setEnabled(true);
                    m_logoutAction->setEnabled(false);
                    m_modifyPwdAction->setEnabled(false);
                }
            }
            else
            {
                m_loginAction->setEnabled(true);
                m_logoutAction->setEnabled(false);
                m_modifyPwdAction->setEnabled(false);
            }

            m_ItemMenu->exec(QCursor::pos());
            event->accept();
        }
		else
		{
			if( m_mainWindow->getTabWidget()->currentIndex() == 2 )
			{			
				SResourceInfo info;
				if( getResourceInfoByChildItem( curItem, info ) )
				{
					m_ItemMenu->clear();
					m_ItemMenu->addAction(m_scheduleMonitIndex);
					m_ItemMenu->exec(QCursor::pos());
					event->accept();
				}
			}
		}
    }
    else
    {
        //清除原有菜单
		m_addcmsMenu->clear();
		m_addcmsMenu->addAction(m_addCmsAction);

		//菜单出现的位置为当前鼠标的位置
		m_addcmsMenu->exec(QCursor::pos());
		event->accept();
    }
}

bool CResourceTreeWidget::login( )
{
    if (!loginImpl())
    {
        QMessageBox::warning( this, tr("warning"), tr("Login failed!"), tr("ok"));
    }

    return true;
}


bool CResourceTreeWidget::loginImpl()
{
    QTreeWidgetItem *item = currentItem();
    if (item == NULL)
    {
        std::cout << "\nItem is null" << std::endl;
        return false;
    }

    //获取登陆信息
    std::map<QTreeWidgetItem*, SCmsLoginInfo>::iterator it = m_CmsLoginInfo.find(item);
    if (it == m_CmsLoginInfo.end())
    {
        std::cout << "\nError: can not find login info" << std::endl;
        return false;
    }

    uint32_t nServer = inet_addr( it->second.server.c_str() );
    uint16_t nPort = atoi( it->second.port.c_str() );
    nServer = ntohl( nServer );

    mpc::nsdk::MD5 pwd(it->second.password.c_str(), strlen(it->second.password.c_str()));

    //登陆CMS
    mpc::nsdk::IManageSession* manageSession = mpc::nsdk::Factory::createManageSession(nServer, nPort, 
        it->second.userName.c_str(), strlen(it->second.userName.c_str()), 
        pwd.digest(), 16);
    if (!manageSession)
    {
        std::cout << "\nManageSession is null" << std::endl;
        return false;
    }

    if (!manageSession->init())
    {
        if (manageSession)
        {    
            manageSession->release();
            manageSession = 0;
        }
        std::cout << "\nManageSession init failed" << std::endl;
        return false;
    }

	manageSession->subscribe(this);

    if (!manageSession->login())
    {
        if (manageSession)
        {    
            manageSession->release();
            manageSession = 0;
        }
        std::cout << "\nManageSession login failed" << std::endl;
        return false;
    }

    //获取媒体源
    clock_t startTime = clock(); 
    mpc::nsdk::IMediaIterator *first = 0;
    first = manageSession->createMediaIterator();
    clock_t endTime = clock();
    
    if (first )
    {    
        m_ItemRescourceInfo.clear();
        createResTree( first, 0, "", item);
        std::cout << std::endl;

        m_allRescourceInfo.insert(std::make_pair(item, m_ItemRescourceInfo));
        m_ItemRescourceInfo.clear();
        first->release();
        first = 0;
    }
    else
    {
        std::cout << "createMediaIterator failed!" << std::endl;
    }
	//从服务器中获取资源所需要的时间，未统计Demo创建节点时间	
	std::cout << "Getting All Resources have Spent time(msec): " << endTime - startTime << std::endl;

    m_manageSessionInfo.insert(std::make_pair(item, manageSession));


    //创建电视墙
    if (getAllMonitorId(item))
    {
        CConfigScene* scene = m_mainWindow->getConfigScene();
        if (scene)
        {
            std::map<QTreeWidgetItem*, std::vector<SMonitorRelation>>::iterator allMonitorIt = m_allMonitorRelation.find(item);
            if (allMonitorIt != m_allMonitorRelation.end())
            {
                scene->addItems(manageSession, allMonitorIt->second);
            }
        }
    }

    item->setExpanded(true);

	// 添加告警订阅

    CAlarmTab* alarmTab = m_mainWindow->getAlarmTab();
    if (alarmTab)
    {
        alarmTab->addRegisterBySession(manageSession);
        alarmTab->init(manageSession, item->text( 0 ) );
    }
    
	return true;
}
bool CResourceTreeWidget::logout( )
{
    QTreeWidgetItem* item=currentItem();//获得当前节点
    if(NULL==item)//没有选择节点
        return false;
    QTreeWidgetItem* parent=item->parent();//获得当前节点的父节点
    if(NULL==parent)//item就是top节点
    {
        //删除管理会话连接
        SCmsLoginInfo cmsInfo;
        delManageSession(item, cmsInfo);
    }
#if 1 //def _DEBUG
    std::cout << "m_allMonitorRelation.size = " << m_allMonitorRelation.size() << std::endl;
    std::cout << "m_manageSessionInfo.size = " << m_manageSessionInfo.size() << std::endl;
    std::cout << "m_CmsLoginInfo.size = " << m_CmsLoginInfo.size() << std::endl;
    std::cout << "m_allRescourceInfo.size = " << m_allRescourceInfo.size() << std::endl;
#endif

    return true;
}


bool CResourceTreeWidget::modifyPwd()
{
    QTreeWidgetItem* item=currentItem();//获得当前节点
    if(NULL==item)//没有选择节点
        return false;
    QTreeWidgetItem* parent=item->parent();//获得当前节点的父节点
    if(NULL==parent)//item就是top节点
    {
        std::map<QTreeWidgetItem*, mpc::nsdk::IManageSession*>::iterator manageSessionIt = m_manageSessionInfo.find(item);
        if (manageSessionIt != m_manageSessionInfo.end())
        {
            if (manageSessionIt->second)
            {
                CPwdModifyWnd dlg(this, manageSessionIt->second);
                dlg.exec(); 
            }
        }
    }

    return true;
}


void CResourceTreeWidget::delCmsItem(  )
{
    QTreeWidgetItem* item=currentItem();//获得当前节点
    if(NULL==item)//没有选择节点
        return;
    QTreeWidgetItem* parent=item->parent();//获得当前节点的父节点
    if(NULL==parent)//item就是top节点
    {
        //删除管理会话连接
        SCmsLoginInfo info;
        delManageSession(item, info);

        //删除缓存
        std::map<QTreeWidgetItem*, SCmsLoginInfo>::iterator it = m_CmsLoginInfo.find(item);
        if (it != m_CmsLoginInfo.end())
        {
            info = it->second;
            m_CmsLoginInfo.erase(it);
        }

        delete item;
        item=NULL;
    }
}

void CResourceTreeWidget::propertyCmsItem()
{
    QTreeWidgetItem* item=currentItem();//获得当前节点
    if(NULL==item)//没有选择节点
    {
        std::cout << "current item is null" << std::endl;
        return;
    }

    QTreeWidgetItem* parent=item->parent();//获得当前节点的父节点
    if(NULL==parent)//item就是top节点
    {
        std::map<QTreeWidgetItem*, SCmsLoginInfo>::iterator it = m_CmsLoginInfo.find(item);
        if (it != m_CmsLoginInfo.end())
        {
            std::map<QTreeWidgetItem*, mpc::nsdk::IManageSession*>::iterator manageSessionIt = m_manageSessionInfo.find(it->first);
            if (manageSessionIt != m_manageSessionInfo.end())
            {
                if (manageSessionIt->second)
                {
                    size_t size = 0;
                    uint8_t digest[mpc::nsdk::MAX_DIGEST_SIZE];
                    manageSessionIt->second->getMediaDigest(digest, mpc::nsdk::MAX_DIGEST_SIZE, size);
                    CLoginWnd dlg(this, ELOGINREAD, bytesToHexString(digest, size));
                    dlg.setPropertyInfo(it->second);
                    dlg.exec(); 
                }
                else
                {
                    CLoginWnd dlg(this, ELOGINMODIFY);
                    dlg.setPropertyInfo(it->second);
                    dlg.exec();
                }
            }
            else
            {
                CLoginWnd dlg(this, ELOGINMODIFY);
                if(dlg.setPropertyInfo(it->second))
                {
                    dlg.exec();
                }
                else
                {
                    QMessageBox::warning( this, tr("warning"), tr("get CMS info failed!"), tr("ok"));
                }
            }
        }
        else
        {
            std::cout << "not find cmsLoginInfo" << std::endl;
        }
    }
    else
    {
        std::cout << "it is not CmsItem!" << std::endl;
    }
}

void CResourceTreeWidget::AddCmsItem()
{
    //弹出登陆属性对话框
    CLoginWnd dlg(this);
    dlg.setPropertyInfo(m_lastinfo);
    dlg.exec();
}

void CResourceTreeWidget::setCmsItemInfo( const SCmsLoginInfo &info, const ELoginType &loginType )
{
    if (loginType == ELOGINMODIFY)           //修改CMS节点
    {
        QTreeWidgetItem* item=currentItem();//获得当前节点
        if(NULL==item)//没有选择节点
            return;
        std::map<QTreeWidgetItem*, SCmsLoginInfo>::iterator it = m_CmsLoginInfo.find(item);
        if (it != m_CmsLoginInfo.end())
        {
            it->second = info;
            QString strTips = tr("UserName: ") + QString(QString::fromLocal8Bit(info.userName.c_str()));
            it->first->setToolTip(0, strTips);
        }
    }
    else if(loginType == ELOGINADD)          //添加CMS节点
    {
//         std::map<QTreeWidgetItem*, SCmsLoginInfo>::iterator it = m_CmsLoginInfo.begin();
//         for (; it != m_CmsLoginInfo.end(); ++it)
//         {
//             if (it->second == info)
//             {
//                 QMessageBox::warning( this, tr("warning"), tr("The CmsUser is exist!"), tr("ok"));
//                 return;
//             }
//         }
        QTreeWidgetItem *cmsItem = new QTreeWidgetItem(this,QStringList(QString(info.server.c_str())));
        cmsItem->setFlags(cmsItem->flags() & ~Qt::ItemIsDragEnabled); //设置不可拖拽
        QString strTips = tr("UserName: ") + QString(QString::fromLocal8Bit(info.userName.c_str()));
        cmsItem->setToolTip(0, strTips);
        m_CmsLoginInfo.insert(std::make_pair(cmsItem, info));
        m_lastinfo = info;
    }
}

void CResourceTreeWidget::createResTree( mpc::nsdk::IMediaIterator *it, int deep, std::string deviceName, QTreeWidgetItem *item)
{
    mpc::nsdk::IMediaNode *self = 0;
    mpc::nsdk::IMediaIterator *curr = 0;
    mpc::nsdk::IMediaIterator *next = 0;
    mpc::nsdk::IMediaIterator *down = 0;
    size_t strSize = 0;
    mpc::nsdk::IMediaRes *resPtr = 0;
    mpc::nsdk::SResId resId;
    curr = it;

    do 
    {
        SResourceInfo info;
        QTreeWidgetItem *dirItem = NULL;
        //
#if 1 //def _DEBUG
        std::cout << std::endl;
        for( int i = 0; i < deep; ++i )
        {
            std::cout << "--|";
        }
#endif

        self = curr->self();
        next = curr->next();
        down = curr->down();

        // 先将自己的信息打出来.
        if( self->isRes() == false )
        {

#if 1 //def _DEBUG
            std::cout << "dir: ";
            std::cout << self->label( strSize );
#endif
            deviceName = std::string(self->label( strSize ));
            if (/*deep > */1)
            {
                QStringList contentList;
                contentList << QString(self->label( strSize ));
                dirItem = new QTreeWidgetItem(item,contentList);  //添加子节点
                dirItem->setFlags(dirItem->flags() & ~Qt::ItemIsDragEnabled); //设置不可拖拽
                item->addChild(dirItem);
            }
        }
        else
        {
            resPtr = dynamic_cast< mpc::nsdk::IMediaRes* > ( self );
            resId = resPtr->msid();
#if 1 //def _DEBUG
            std::cout << "res: ";
            std::cout << self->label( strSize );
            std::cout << " " << strSize;
            std::cout << " ptz=" << resPtr->hasPtz();
            printResId(resId);
#endif
            info.pos        = deep - 1;
            info.deviceName = deviceName;
            info.isHasPtz   = resPtr->hasPtz();
            info.resId      = resId;
            info.resName    = std::string(self->label( strSize ));
            fillAllExtInfo(resPtr, info);

            QStringList contentList;
            contentList << QString(self->label( strSize ));
            QTreeWidgetItem *resItem = new QTreeWidgetItem(item,contentList);  //添加子节点
            item->addChild(resItem);

			QString strTips = tr( "code: ") + QString(QString::fromLocal8Bit(info.code.c_str()))
							+ tr("\r\nsection: ") + QString(QString::fromLocal8Bit(info.section.c_str()))
							+ tr("\r\ndirection: ") + QString(QString::fromLocal8Bit(info.direction.c_str()))
							+ tr("\r\nlandmark: ") + QString(QString::fromLocal8Bit(info.landmark.c_str()));
			resItem->setToolTip(0, strTips);

            if (resPtr->hasPtz())
            {
                resItem->setIcon(0, QIcon(SDK_RESOURCEHASPTZICON));
            }
            else
            {
                resItem->setIcon(0, QIcon(SDK_RESOURCENOTHASPTZICON));
            }
            m_ItemRescourceInfo.insert(std::make_pair(resItem, info));
        }
        if( down )
        {
            deep++;
            if (dirItem)
            {
                createResTree( down, deep, deviceName,dirItem );
            }
            else
            {
                createResTree( down, deep, deviceName,item );
            }
        }

        if (next)
        {
            curr = next;
        }
    } while (next);
}

void CResourceTreeWidget::saveLoginInfo()
{
    QFile file("cmsInfo.ini");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
    {
        return;
    }

    QTextStream out(&file);

    std::map<QTreeWidgetItem*, SCmsLoginInfo>::iterator it = m_CmsLoginInfo.begin();
    for (; it != m_CmsLoginInfo.end(); ++it)
    {
        out << it->second.userName.c_str() << "\n";
        out << it->second.password.c_str() << "\n";
        out << it->second.server.c_str() << "\n";
        out << it->second.port.c_str() << "\n";
    }

    file.close();   
}

void CResourceTreeWidget::readHistoryLoginInfo()
{
    QFile file("cmsInfo.ini");
    if (!file.open(QIODevice::ReadWrite| QIODevice::Text))
    {
        return;
    }

    SCmsLoginInfo info;
    std::string oneLine;
    
    QTextStream in(&file);

    try
    {
        while (!in.atEnd())
        {
            info.userName   = in.readLine().toLocal8Bit();
            info.password   = in.readLine().toLocal8Bit();
            info.server     = in.readLine().toLocal8Bit();
            info.port       = in.readLine().toLocal8Bit();

            if (info.userName.empty() || info.password.empty() || info.server.empty() || info.port.empty())
            {
                break;
            }

            setCmsItemInfo(info, ELOGINADD);
        }
    }
    catch(...)
    {

    }

    file.close();

    return;
}

void CResourceTreeWidget::delManageSession(QTreeWidgetItem* item, SCmsLoginInfo &info)
{
    if (item)
    {
        //登出操作
        delManageSessionImpl(item);        

        //释放掉存放节点的内存空间
        int childCount=item->childCount();//子节点数
        for (int i=0;i<childCount;i++)
        {
            QTreeWidgetItem* childItem=item->child(0);
            delete childItem;
            childItem=NULL;
        }
    }
}

bool CResourceTreeWidget::delManageSessionImpl(QTreeWidgetItem* item)
{
    //delete ManageSession
    std::map<QTreeWidgetItem*, mpc::nsdk::IManageSession*>::iterator manageSessionIt = m_manageSessionInfo.find(item);
    if (manageSessionIt != m_manageSessionInfo.end())
    {    
        if (manageSessionIt->second)
        {
            //关闭实时视频与历史视频播放
            CMulLivingPlayerTab*	livingPlayTab = (CMulLivingPlayerTab*)( m_mainWindow->getLivingViewTab() ); 
            if (livingPlayTab != NULL)
            {
                livingPlayTab->closeSession(manageSessionIt->second);
            }

            CReplayTab*	replayTab = (CReplayTab*)( m_mainWindow->getReplayViewTab() );
            if (replayTab != NULL)
            {
                replayTab->closeSession(manageSessionIt->second);
            }

            CConfigScene* scene = (CConfigScene*)( m_mainWindow->getConfigScene() );
            if (scene)
            {
                scene->removeAllItems(manageSessionIt->second);
            }

            CAlarmTab* alarmTab = m_mainWindow->getAlarmTab();
            if (alarmTab)
            {
                alarmTab->delRegisterBySession( manageSessionIt->second );
                alarmTab->fini(manageSessionIt->second );
            }

			manageSessionIt->second->logout();

			// 释放用fini处理
			m_delManageSession.push_back( manageSessionIt->second );

            //if (!manageSessionIt->second->logout())
            //{  
            //    manageSessionIt->second->release();
            //    manageSessionIt->second = 0;
            //    return false;
            //}

            //manageSessionIt->second->release();
            //manageSessionIt->second = 0;
        }

        m_manageSessionInfo.erase(manageSessionIt);
    }

    //delete ItemRescource
    std::map<QTreeWidgetItem*, std::map<QTreeWidgetItem*, SResourceInfo> >::iterator allResIt = m_allRescourceInfo.find(item);
    if (allResIt != m_allRescourceInfo.end())
    {
        m_allRescourceInfo.erase(allResIt);
    }

    //delete MonitorRelation
    std::map<QTreeWidgetItem*, std::vector<SMonitorRelation>>::iterator allMonitorIt = m_allMonitorRelation.find(item);
    if (allMonitorIt != m_allMonitorRelation.end())
    {
        m_allMonitorRelation.erase(allMonitorIt);
    }

    return true;
}


void CResourceTreeWidget::slotItemClicked( QTreeWidgetItem *item, int coloum )
{
	if ( item == NULL )
	{
		return;
	}

    //获取当前节点的根节点（CMS节点）
    QTreeWidgetItem *cmsItem = item;
    while(cmsItem->parent() != NULL)
    {
        std::string str = cmsItem->text(0).toLocal8Bit();
        std::cout << str << std::endl;
        cmsItem = cmsItem->parent();
    }

    //CMS节点未做变更，无需更新电视墙与告警界面
    if (cmsItem == g_lastCmsItem )
    {
        return;
    }

    CConfigScene* scene = (CConfigScene*)( m_mainWindow->getConfigScene() );
    if (!scene)
    {
        return;
    }

    CAlarmTab* alarmTab = m_mainWindow->getAlarmTab();
    if (!alarmTab)
    {
        return;
    }
    std::map<QTreeWidgetItem*, mpc::nsdk::IManageSession*>::iterator manageSessionIt = m_manageSessionInfo.find(cmsItem);
    if (manageSessionIt != m_manageSessionInfo.end())
    {
        scene->showItemByServer(manageSessionIt->second);

		alarmTab->init( manageSessionIt->second, cmsItem->text( 0 ) );
    }
    else
    {
        scene->showItemByServer(NULL);

		alarmTab->clearCurrentTable();
    }

    g_lastCmsItem = cmsItem;
}


void CResourceTreeWidget::slotItemDoubleCliecked( QTreeWidgetItem *item, int coloum )
{
	if ( item == NULL )
	{
		return;
	}

    QTreeWidgetItem *cmsItem = item;
    while(cmsItem->parent() != NULL)
    {
        cmsItem = cmsItem->parent();
    }

    std::map<QTreeWidgetItem*, std::map<QTreeWidgetItem*, SResourceInfo> >::iterator allResIt = m_allRescourceInfo.find(cmsItem);
    if (allResIt == m_allRescourceInfo.end())
    {
        return;
    }
    std::map<QTreeWidgetItem*, SResourceInfo>::iterator it = allResIt->second.find(item);
    if (it != allResIt->second.end())
    {
		QTabWidget* tab = m_mainWindow->getTabWidget();
        if (tab == NULL)
        {
            return;
        }

        if (tab->currentIndex() == 0)
        {
            CMulLivingPlayerTab*	livingPlayTab = (CMulLivingPlayerTab*)( m_mainWindow->getLivingViewTab() ); 
            if (livingPlayTab != NULL)
            {
                mpc::nsdk::IManageSession* manageSession = NULL;
                getManageSessionByChildItem(item, &manageSession);
                if (manageSession != NULL)
                {
                    livingPlayTab->play(manageSession, it->second );
                }
                return;
            }
        }
        else if (tab->currentIndex() == 1)
        {
            CReplayTab*	replayTab = (CReplayTab*)( m_mainWindow->getReplayViewTab() );
            if (replayTab != NULL)
            {
                mpc::nsdk::IManageSession* manageSession = NULL;
                getManageSessionByChildItem(item, &manageSession);
                if (manageSession != NULL)
                {
                    replayTab->initActiveWidget(manageSession, it->second);
                }
                return;
            }
        }
        else if (tab->currentIndex() == 2)
        {
            CConfigScene*	scene = (CConfigScene*)( m_mainWindow->getConfigScene() );

            createRelation(scene, item);
        }		
    }
}

bool CResourceTreeWidget::getAllMonitorId(QTreeWidgetItem* item)
{
    std::map<QTreeWidgetItem*, mpc::nsdk::IManageSession*>::iterator manageSessionIt = m_manageSessionInfo.find(item);
    if (manageSessionIt != m_manageSessionInfo.end())
    {
        if (!manageSessionIt->second)
        {
            return false;
        }

        mpc::nsdk::IWallController *wall;
        wall = manageSessionIt->second->createWallController();
        if (!wall)
        {
            return false;
        }

        clock_t startTime = clock(); 
        mpc::nsdk::IInfoMonitor *idList = 0;
        wall->getMonitorId( &idList );
        clock_t endTime = clock();

        if (!idList)
        {
            wall->release();
            wall = 0;
            return false;
        }

        std::vector<SMonitorRelation> monitorRelationVec;
        mpc::nsdk::IInfoMonitor *next = idList;
        next = idList;
        int index = 0;
        do 
        {
#if 1 // def _DEBUG
            std::cout << "-------=-=-=monitor id-----===-=-" << std::endl;
            printResId(next->id());
            std::cout << std::endl;
#endif
            SMonitorRelation monitorRelation;
            monitorRelation.index = index; 
            monitorRelation.manageSession = manageSessionIt->second;
            monitorRelation.monitorId = next->id();
            monitorRelationVec.push_back(monitorRelation);

            next = next->next();
            ++index;
        } while ( next );

		//统计从服务器获取所有监视器所用时获取间
        std::cout << "Getting All MonitorIds have Spent time(msec): " << endTime - startTime << std::endl;

        idList->release();
        idList = 0;

        wall->release();
        wall = 0;

        m_allMonitorRelation.insert(std::make_pair(item, monitorRelationVec));
    }

    return true;
}

void CResourceTreeWidget::createRelation(CConfigScene* scene, QTreeWidgetItem *item)
{
    if (scene == NULL || item == NULL )
    {
        return ;
    }

    CConfigItem *configItem= scene->getCurrentItem();
    if (!configItem)
    {
        return;
    }
    if (!configItem->isVisible())
    {
        return;
    }  

    QTreeWidgetItem *cmsItem = item;
    while(cmsItem->parent() != NULL)
    {
        cmsItem = cmsItem->parent();
    }

    std::map<QTreeWidgetItem*, std::vector<SMonitorRelation>>::iterator allMonitorIt = m_allMonitorRelation.find(cmsItem);
    if (allMonitorIt == m_allMonitorRelation.end())
    {
        return;
    }
    std::vector<SMonitorRelation>::iterator itmonitor = std::find(allMonitorIt->second.begin(), allMonitorIt->second.end(), configItem->getIndex()) ;
    if (itmonitor != allMonitorIt->second.end())
    {
        std::map<QTreeWidgetItem*, std::map<QTreeWidgetItem*, SResourceInfo> >::iterator allResIt = m_allRescourceInfo.find(cmsItem);
        if (allResIt == m_allRescourceInfo.end())
        {
            return;
        }

        std::map<QTreeWidgetItem*, SResourceInfo>::iterator itMap = allResIt->second.find(item);
        if (itMap == allResIt->second.end())
        {
            return;
        }

        itmonitor->displayText = item->text(0).toLocal8Bit();
        itmonitor->resId = itMap->second.resId;
        itmonitor->monitorId = configItem->getMoniorRelation().monitorId;

        scene->createRelation(configItem, *itmonitor);
    }
}

void CResourceTreeWidget::getManageSessionByChildItem( QTreeWidgetItem *item, mpc::nsdk::IManageSession**manageSession) const
{   
    if (item == NULL)
    {
        return;
    }

    QTreeWidgetItem *cmsitem = NULL;

    while(item->parent() != NULL)
    {
        item = item->parent();
    }

    cmsitem = item;

    std::map<QTreeWidgetItem*, mpc::nsdk::IManageSession*>::const_iterator manageSessionIt = m_manageSessionInfo.find(cmsitem);
    if (manageSessionIt != m_manageSessionInfo.end())
    {
        *manageSession = manageSessionIt->second;
    }

}


bool CResourceTreeWidget::getResourceInfoByChildItem( QTreeWidgetItem *item, SResourceInfo &info ) const
{
    if (item == NULL)
    {
        return false;
    }

    QTreeWidgetItem *cmsitem = item;

    while(cmsitem->parent() != NULL)
    {
        cmsitem = cmsitem->parent();
    }

    std::map<QTreeWidgetItem*, std::map<QTreeWidgetItem*, SResourceInfo> >::const_iterator allresInfoIt = m_allRescourceInfo.find(cmsitem);
    if (allresInfoIt != m_allRescourceInfo.end())
    {
        std::map<QTreeWidgetItem*, SResourceInfo>::const_iterator it = allresInfoIt->second.find(item);
        if (it != allresInfoIt->second.end())
        {
            info = it->second;
            return true;
        }
    }

    return false;
}


void CResourceTreeWidget::terminate( mpc::nsdk::IManageSession *session )
{
    CCmsLinkDownEvent* event = new CCmsLinkDownEvent( CMS_TERMINATE_EVENT );
    if ( event )
    {
        SCmsLinkDownEventData data;
        data.session = session;
        event->setData( data );
        QApplication::postEvent( this, event );
    }
}

void CResourceTreeWidget::customEvent( QEvent * event )
{
    if ( event->type() == CMS_TERMINATE_EVENT )
    {
        CCmsLinkDownEvent* linkdownEvent = ( CCmsLinkDownEvent* )event;
        //获取当前CMS地址
        QTreeWidgetItem* cmsItem = NULL;
        std::map<QTreeWidgetItem*, mpc::nsdk::IManageSession*>::iterator manageSessionIt = m_manageSessionInfo.begin();
        for (; manageSessionIt != m_manageSessionInfo.end(); ++manageSessionIt)
        {
            if (manageSessionIt->second == linkdownEvent->getData().session)
            {
                cmsItem = manageSessionIt->first;
                break;
            }
        }

        if (cmsItem == NULL)
        {
            return;
        }

        SCmsLoginInfo info;
        std::map<QTreeWidgetItem*, SCmsLoginInfo>::iterator it = m_CmsLoginInfo.find(cmsItem);
        if (it != m_CmsLoginInfo.end())
        {
            info = it->second;
        }

        if (!isSupportTestSdk)
        {
            SCmsLoginInfo tempInfo;
            delManageSession(cmsItem, tempInfo);
        }

        //释放掉存放节点的内存空间
        int childCount=cmsItem->childCount();//子节点数
        for (int i=0;i<childCount;i++)
        {
            QTreeWidgetItem* childItem=cmsItem->child(0);
            delete childItem;
            childItem=NULL;
        }
        QMessageBox::warning( this, tr("warning"), QString(info.server.c_str()) + tr(" has shutdown!"), tr("ok"));
    }
}

QStringList CResourceTreeWidget::mimeTypes() const
{
    QStringList types;
    types << DRAGANDDROP_DATATYPE ;
    return types;
}

QMimeData * CResourceTreeWidget::mimeData( const QList<QTreeWidgetItem *> items ) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    QVector<intptr_t>   manageSessionVec;   // 回话链接
    QVector<QString>    resNameVec;         // 资源名字
    QVector<QString>    resIdVec;           // 资源标识
    QVector<bool>       ptzStatusVec;       // 结点云台状态

    for (int i=0; i<items.size(); ++i)
    {
        //获取当前节点的根节点（CMS节点）
        QTreeWidgetItem *cmsItem = items[i];
        while(cmsItem->parent() != NULL)
        {
            cmsItem = cmsItem->parent();
        }

        g_lastCmsItem = cmsItem;

        QTreeWidgetItem *item = items[i];
        mpc::nsdk::IManageSession* manageSession = NULL;
        getManageSessionByChildItem(item, &manageSession);
        if (manageSession != NULL)
        {
            CConfigScene* scene = (CConfigScene*)( m_mainWindow->getConfigScene() );
            if (scene)
            {
                scene->showItemByServer(manageSession);
                SResourceInfo info;
                if (getResourceInfoByChildItem(item, info))
                {
                    std::string strId;
                    resIdStr(info.resId, strId);
                    QString resId(strId.c_str());
                    QString resName(info.resName.c_str());

                    manageSessionVec.push_back((intptr_t)manageSession);
                    resNameVec.push_back(resName);
                    resIdVec.push_back(resId);
                    ptzStatusVec.push_back(info.isHasPtz);
                }
            }
        }
    }

    stream << manageSessionVec << resNameVec << resIdVec << ptzStatusVec;
    mimeData->setData( DRAGANDDROP_DATATYPE, encodedData );

    return mimeData;

}

Qt::DropActions CResourceTreeWidget::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

void CResourceTreeWidget::dropEvent( QDropEvent *event )
{
    event->setDropAction(Qt::IgnoreAction);
    QTreeWidget::dropEvent(event);
}

void CResourceTreeWidget::updateCmsInfo( const std::string &password )
{
    QTreeWidgetItem* item=currentItem();//获得当前节点
    if(NULL==item)//没有选择节点
        return;

    std::map<QTreeWidgetItem*, SCmsLoginInfo>::iterator it = m_CmsLoginInfo.find(item);
    if (it != m_CmsLoginInfo.end())
    {
        it->second.password = password;
    }
}

void CResourceTreeWidget::singleScheduleMoniterIndex()
{
	QTreeWidgetItem* item=currentItem();//获得当前节点
	if(NULL==item)//没有选择节点
	{
		std::cout << "current item is null" << std::endl;
		return;
	}

	mpc::nsdk::IManageSession* manageSession = 0;
	getManageSessionByChildItem(item, &manageSession);

	SResourceInfo info;
	getResourceInfoByChildItem( item, info );

	CConfigScene* scene = (CConfigScene*)( m_mainWindow->getConfigScene() );
	if (!scene)
	{
		return;
	}

	CMonitorIndexScheduleWnd dlg(this, manageSession, info, scene );
	dlg.exec();
}



