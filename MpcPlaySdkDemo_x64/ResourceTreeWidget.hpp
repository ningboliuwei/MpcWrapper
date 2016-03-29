
#ifndef RESOURCETREEWIDGET_HPP_
#define RESOURCETREEWIDGET_HPP_

#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QTreeWidget>
#include <QtGui/QGraphicsView>
#include <set>

#include "../include/SysTypeDefine.hpp"
#include "../include/CustomEvent.hpp"
#include "ConfigScene.hpp"


struct SCmsLinkDownEventData
{
    mpc::nsdk::IManageSession *session;    
};

typedef CCustomEvent<SCmsLinkDownEventData> CCmsLinkDownEvent;

class CMainWindow;

class  CResourceTreeWidget : public QTreeWidget, public mpc::nsdk::IManageSessionNotify
{
    Q_OBJECT

public:
    CResourceTreeWidget( QWidget *parent);
    ~CResourceTreeWidget();

public:
    bool init();
    bool fini();
    void setCmsItemInfo(const SCmsLoginInfo &info, const ELoginType &loginType);
    void contextMenuEvent(QContextMenuEvent *event);
    void updateCmsInfo(const std::string &password);

public:
    virtual void terminate( mpc::nsdk::IManageSession *session );

protected:
    virtual void customEvent( QEvent * event );
    // 支持拖放
    QStringList mimeTypes() const;
    QMimeData *mimeData( const QList<QTreeWidgetItem *> items ) const;
    Qt::DropActions supportedDropActions() const;
    void dropEvent(QDropEvent *event);

private:
    void saveLoginInfo();
    void readHistoryLoginInfo();
    void createResTree( mpc::nsdk::IMediaIterator *it, int deep, std::string deviceName, QTreeWidgetItem *item);
    bool delManageSessionImpl(QTreeWidgetItem* item);
    void delManageSession(QTreeWidgetItem* item, SCmsLoginInfo &info);
    bool getAllMonitorId(QTreeWidgetItem* item);
    void createRelation(CConfigScene* scene, QTreeWidgetItem *item);
    bool loginImpl();

private slots:
    bool login();
    bool logout();
    bool modifyPwd();
    void delCmsItem();
    void AddCmsItem();
    void propertyCmsItem();
	void singleScheduleMoniterIndex();
    void slotItemClicked(QTreeWidgetItem *item, int coloum);
    void slotItemDoubleCliecked(QTreeWidgetItem *item, int coloum);
    void getManageSessionByChildItem( QTreeWidgetItem *item, mpc::nsdk::IManageSession** manageSession) const;
    bool getResourceInfoByChildItem( QTreeWidgetItem *item, SResourceInfo &info) const;

private:
    QMenu	*m_ItemMenu;					//登入，登出，删除菜单
    QMenu	*m_addcmsMenu;				    //添加CMS菜单

    QAction *m_addCmsAction;			    //添加CMS节点动作
    QAction *m_loginAction;				    //登入动作
    QAction *m_logoutAction;				//登出动作
    QAction *m_modifyPwdAction;				//修改密码动作
    QAction *m_delAction;					//删除CMS节点动作
    QAction *m_propertyAction;              //获取CMS节点属性动作
	QAction *m_scheduleMonitIndex;          //点击在媒体源节点上才有，用于监视器索引调度
    SCmsLoginInfo m_lastinfo;              //上一次成功输入CMS信息

	CMainWindow     *m_mainWindow;    

    std::map<QTreeWidgetItem*, std::vector<SMonitorRelation>> m_allMonitorRelation;                 //存储每个CMS的监视器信息
    std::map<QTreeWidgetItem*, mpc::nsdk::IManageSession*> m_manageSessionInfo;                     //存储每个CMS的会话连接信息
    std::map<QTreeWidgetItem*, SResourceInfo> m_ItemRescourceInfo;                                  //临时存储源节点对应的通道信息
    std::map<QTreeWidgetItem*, SCmsLoginInfo> m_CmsLoginInfo;                                       //存储每个CMS的登录信息
    std::map<QTreeWidgetItem*, std::map<QTreeWidgetItem*, SResourceInfo> > m_allRescourceInfo;      //存储每个CMS的通道信息
	std::vector<mpc::nsdk::IManageSession*>		m_delManageSession;
	
};

#endif // RESOURCETREEWIDGET_HPP_
 