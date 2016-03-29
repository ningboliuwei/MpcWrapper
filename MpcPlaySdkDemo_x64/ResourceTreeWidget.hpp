
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
    // ֧���Ϸ�
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
    QMenu	*m_ItemMenu;					//���룬�ǳ���ɾ���˵�
    QMenu	*m_addcmsMenu;				    //���CMS�˵�

    QAction *m_addCmsAction;			    //���CMS�ڵ㶯��
    QAction *m_loginAction;				    //���붯��
    QAction *m_logoutAction;				//�ǳ�����
    QAction *m_modifyPwdAction;				//�޸����붯��
    QAction *m_delAction;					//ɾ��CMS�ڵ㶯��
    QAction *m_propertyAction;              //��ȡCMS�ڵ����Զ���
	QAction *m_scheduleMonitIndex;          //�����ý��Դ�ڵ��ϲ��У����ڼ�������������
    SCmsLoginInfo m_lastinfo;              //��һ�γɹ�����CMS��Ϣ

	CMainWindow     *m_mainWindow;    

    std::map<QTreeWidgetItem*, std::vector<SMonitorRelation>> m_allMonitorRelation;                 //�洢ÿ��CMS�ļ�������Ϣ
    std::map<QTreeWidgetItem*, mpc::nsdk::IManageSession*> m_manageSessionInfo;                     //�洢ÿ��CMS�ĻỰ������Ϣ
    std::map<QTreeWidgetItem*, SResourceInfo> m_ItemRescourceInfo;                                  //��ʱ�洢Դ�ڵ��Ӧ��ͨ����Ϣ
    std::map<QTreeWidgetItem*, SCmsLoginInfo> m_CmsLoginInfo;                                       //�洢ÿ��CMS�ĵ�¼��Ϣ
    std::map<QTreeWidgetItem*, std::map<QTreeWidgetItem*, SResourceInfo> > m_allRescourceInfo;      //�洢ÿ��CMS��ͨ����Ϣ
	std::vector<mpc::nsdk::IManageSession*>		m_delManageSession;
	
};

#endif // RESOURCETREEWIDGET_HPP_
 