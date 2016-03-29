#include "MainWindow.hpp"
#include <QtGui/QGridLayout>
#include <QtGui/QTreeWidgetItem>
#include "../include/SysTypeDefine.hpp"

#include "MulLivingPlayerTab.hpp"
#include "ReplayTab.hpp"
#include "../include/sysTypeDefine.hpp"
#include "ResourceTreeWidget.hpp"
#include "ConfigScene.hpp"
#include "MediaPlayerWidget.hpp"
#include "AlarmTab.hpp"

CMainWindow::CMainWindow( QWidget *parent /*= 0 */ )
:QMainWindow( parent )
,m_treeWidget( NULL )
,m_tabWidget( NULL )
,m_videoWallTab( NULL )
,m_livingViewTab( NULL )
,m_replayViewTab( NULL )
,m_alarmTab( NULL )

{
    QWidget     *centralwidget;
    QWidget     *gridLayoutWidget;
    QHBoxLayout *horizontalLayout;

#ifdef _WIN64
    this->setWindowTitle(tr("MpcPlaySdkDemo64"));
#else
	this->setWindowTitle(tr("MpcPlaySdkDemo"));
#endif
    this->setMinimumSize(QSize(900, 700));

    centralwidget       = new QWidget(this);
    gridLayoutWidget    = new QWidget(centralwidget);
    horizontalLayout = new QHBoxLayout(centralwidget);

    m_tabWidget         = new QTabWidget(gridLayoutWidget);
    m_livingViewTab     = new CMulLivingPlayerTab();
    m_replayViewTab     = new CReplayTab();
    m_videoWallTab      = new QGraphicsView;
	m_alarmTab          = new CAlarmTab( this );
    m_scene             = new CConfigScene(SDK_RESOURCEMONITORICON,768, this);           //数字为对话框宽度，用来计算Item的坐标
    m_videoWallTab->setScene(m_scene);
    m_videoWallTab->setMinimumSize(512,300);                //设置view的最小尺寸
    //m_videoWallTab->setAcceptDrops(true);


    //m_treeWidget = new_o(CResourceTreeWidget, this);
    m_treeWidget = new CResourceTreeWidget(this);

    m_treeWidget->setMaximumSize(QSize(200, 16777215));
    horizontalLayout->addWidget(m_treeWidget);

    m_tabWidget->addTab(m_livingViewTab, tr("LivingView"));    
    m_tabWidget->addTab(m_replayViewTab, tr("ReplayView"));
    m_tabWidget->addTab((QWidget*)m_videoWallTab, tr("VideoWall"));
	m_tabWidget->addTab( m_alarmTab, tr("AlarmView") );

    m_tabWidget->setCurrentIndex(0);

    horizontalLayout->addWidget(m_tabWidget);
    this->setCentralWidget(centralwidget);   

	connect ( m_tabWidget, SIGNAL( currentChanged ( int ) ), this, SLOT( tabIndexChangedSlot( int ) ) );

}

CMainWindow::~CMainWindow()
{

}

bool CMainWindow::init()
{
    if (!m_treeWidget->init())
    {
        return false;
    }
    return true;
}

bool CMainWindow::fini()
{
	( ( CMulLivingPlayerTab* ) m_livingViewTab )->fini();
    ( ( CReplayTab* ) m_replayViewTab )->fini();
    ( ( CAlarmTab*) m_alarmTab)->fini();

    if ( !m_treeWidget->fini() )
    {
        return false;
    }

    return true;
}

void CMainWindow::closeEvent( QCloseEvent * event )
{
	( ( CReplayTab* )m_replayViewTab )->closeDownLoadWidget();
	QMainWindow::closeEvent( event );
}

void CMainWindow::tabIndexChangedSlot( int index )
{
	const int LIVING_TAB = 0;
	const int REPLAY_TAB = 1;

	CMulLivingPlayerTab* livingTab = (CMulLivingPlayerTab*)m_livingViewTab;
	CReplayTab* replayTab = (CReplayTab*)m_replayViewTab;

	if ( index == LIVING_TAB )
	{
		replayTab->setAllPlayerMute();
		livingTab->updateActiveWidget( ( CMediaPlayerWidget* )livingTab->getActivePlayWidget() );
	}
	else if ( index == REPLAY_TAB )
	{
		livingTab->setAllPlayerMute();
		replayTab->updateActiveWidget( ( CMediaPlayerWidget* )replayTab->getActivePlayWidget() );
	}
}



