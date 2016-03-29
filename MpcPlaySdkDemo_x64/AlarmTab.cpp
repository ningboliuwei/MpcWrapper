#include "AlarmTab.hpp"
#include <time.h>
#include <QtGui/QMessageBox>
#include "MainWindow.hpp"
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>

// 告警源
const int ALARM_RES_NAME        = 0;
const int APPEAR_NOTIFY         = 1;
const int CONFIRM_NOTIFY        = 2;
const int CLEAN_NOTIFY          = 3;
const int CLEAN_MODE            = 4;
const int ALARM_RES_CODE        = 5;
const int ALARM_RES_SECTION     = 6;
const int ALARM_RES_LANDMARK    = 7;
const int ALARM_RES_DIRECTION   = 8;

const QString COLUMN_ALARM_NAME    = QString( QObject::tr( "alarm name" ) );
const QString COLUMN_APPEAR_NOTIFY = QString( QObject::tr( "active" ) );
const QString COLUMN_CONFIRM_NOTIFY = QString( QObject::tr( "confirm" ) );
const QString COLUMN_CLEAN_NOTIFY  = QString( QObject::tr( "clean" ) );
const QString COLUMN_ALARM_CLEAR_MODE  = QString( QObject::tr( "clean mode" ) );
const QString COLUMN_ALARM_RES_CODE  = QString( QObject::tr( "code" ) );
const QString COLUMN_ALARM_RES_SECTION  = QString( QObject::tr( "section" ) );
const QString COLUMN_ALARM_RES_LANDMARK  = QString( QObject::tr( "landmark" ) );
const QString COLUMN_ALARM_RES_DIRECTION  = QString( QObject::tr( "direction" ) );

// 日志
const int CMS_LABLE   = 0;
const int ALARM_NAME  = 1;
const int NOTIFY_TYPE = 2;
const int ALARM_TYPE  = 3;
const int ALARM_TIME  = 4;
const int ALARM_OPERATOR_USER = 5;
const int ALARM_CLEAN_TYPE = 6;
const int ALARM_BUTTON_CONFIRM = 7;
const int ALARM_BUTTON_CLEAN = 8;

const QString COLUMN_CMS_LABEL	= QString( QObject::tr("cms") );
//const QString COLUMN_ALARM_NAME   = QString( QObject::tr( "alarm name" ) );
const QString COLUMN_NOTIFY_TYPE = QString( QObject::tr( "notify type" ) );
const QString COLUMN_ALARM_TYPE = QString( QObject::tr( "alarm type" ) );
const QString COLUMN_ALARM_TIME = QString( QObject::tr( "time" ) );
const QString COLUMN_ALARM_OPERATOR_USER = QString( QObject::tr( "user" ) );
const QString COLUMN_ALARM_CLEAN_TYPE = QString( QObject::tr( "clear type" ) );
const QString COLUMN_ALARM_BUTTON_CONFIRM = QString( QObject::tr( "confirm" ) );
const QString COLUMN_ALARM_BUTTON_CLEAN = QString( QObject::tr( "clear" ) );

const int LIST_ROW_HEIGHT = 23;

CAlarmTab::CAlarmTab( QWidget *parent, Qt::WindowFlags flag )
: QWidget( parent, flag )
, m_mainWindow( ( CMainWindow* )parent )
, m_isInited( false )
, m_amsOffing( false )
{
    //this->resize( 780, 526 );
    horizontalLayout = new QHBoxLayout( this );

    m_tableViewAlarmRes = new QTableWidget( this );
	
    /*horizontalLayout->addWidget( m_tableViewAlarmRes );
	QSizePolicy sizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding );
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(m_tableViewAlarmRes->sizePolicy().hasHeightForWidth());
	m_tableViewAlarmRes->setSizePolicy(sizePolicy);
	m_tableViewAlarmRes->setMinimumSize(QSize(300, 200));
	m_tableViewAlarmRes->setMaximumSize(QSize(400, 5000));*/

    m_tableViewAlarmLog = new QTableWidget();
    //horizontalLayout->addWidget( m_tableViewAlarmLog );

	QSplitter *hSplitter = new QSplitter( this );
	hSplitter->setOrientation( Qt::Horizontal );
	hSplitter->addWidget( m_tableViewAlarmRes );
	hSplitter->addWidget( m_tableViewAlarmLog );
	horizontalLayout->addWidget( hSplitter );

	// 告警资源列表
	QStringList resHeader;
	resHeader << COLUMN_ALARM_NAME   
		      << COLUMN_APPEAR_NOTIFY
		      << COLUMN_CONFIRM_NOTIFY
		      << COLUMN_CLEAN_NOTIFY
		      << COLUMN_ALARM_CLEAR_MODE
              << COLUMN_ALARM_RES_CODE
              << COLUMN_ALARM_RES_SECTION
              << COLUMN_ALARM_RES_LANDMARK
              << COLUMN_ALARM_RES_DIRECTION;

	initTableWiget( m_tableViewAlarmRes, resHeader, 0 );
	m_tableViewAlarmRes->setColumnWidth( ALARM_RES_NAME, 136 );
	m_tableViewAlarmRes->setColumnWidth( APPEAR_NOTIFY, 42 );
	m_tableViewAlarmRes->setColumnWidth( CONFIRM_NOTIFY, 55 );
	m_tableViewAlarmRes->setColumnWidth( CLEAN_NOTIFY , 42 );
    m_tableViewAlarmRes->setColumnWidth( CLEAN_MODE , 80 );
    m_tableViewAlarmRes->setColumnWidth( ALARM_RES_CODE , 80 );
    m_tableViewAlarmRes->setColumnWidth( ALARM_RES_SECTION , 80 );
    m_tableViewAlarmRes->setColumnWidth( ALARM_RES_LANDMARK , 80 );
    m_tableViewAlarmRes->setColumnWidth( ALARM_RES_DIRECTION , 80 );

	// 日志列表
	QStringList logHeader;
	logHeader << COLUMN_CMS_LABEL
		      << COLUMN_ALARM_NAME  
		      << COLUMN_NOTIFY_TYPE
		      << COLUMN_ALARM_TYPE	
		      << COLUMN_ALARM_TIME
		      << COLUMN_ALARM_OPERATOR_USER
		      << COLUMN_ALARM_CLEAN_TYPE
              << COLUMN_ALARM_BUTTON_CONFIRM
              << COLUMN_ALARM_BUTTON_CLEAN;
	initTableWiget( m_tableViewAlarmLog, logHeader, 0 );
	m_tableViewAlarmLog->setColumnWidth( CMS_LABLE,    115 ); 
	m_tableViewAlarmLog->setColumnWidth( ALARM_NAME,   135 );
	m_tableViewAlarmLog->setColumnWidth( NOTIFY_TYPE,  80 );  
	m_tableViewAlarmLog->setColumnWidth( ALARM_TYPE,   74 );  
	m_tableViewAlarmLog->setColumnWidth( ALARM_TIME,   135 ); 
	m_tableViewAlarmLog->setColumnWidth( ALARM_OPERATOR_USER, 80); 
    m_tableViewAlarmLog->setColumnWidth( ALARM_CLEAN_TYPE, 80 );
	
	connect( m_tableViewAlarmRes, SIGNAL( cellChanged( int , int  ) ),
		this, SLOT( alarmResItemChangedSlot( int , int ) ) );

}
 
CAlarmTab::~CAlarmTab()
{
	CAlarmRegRecord::release();
}

void CAlarmTab::initTableWiget( QTableWidget* tableView, const QStringList& header, const int row )
{
	if ( header.size() > 0 )  // 初始化头
	{
		tableView->clear();	
		tableView->setColumnCount( header.size() );
		tableView->setHorizontalHeaderLabels( header );
	}

	// 当前行,列数
	int currentColumn = tableView->columnCount();
	int currentRow = tableView->rowCount();

	if ( currentRow < row )  // 增加
	{
		tableView->setRowCount( row );
		for ( int i = currentRow; i < row; i++ )
		{
			for ( int j = 0; j < currentColumn; j++ )
			{
				if ( i >= currentRow )
				{
					tableView->setItem( i, j, new QTableWidgetItem( QString( "" ) ) );
				}
				else
				{
					tableView->item( i, j )->setText( QString( "" ) );  
				}	
			}

			tableView->setRowHeight( i , LIST_ROW_HEIGHT );
		}
	}
	else if ( currentRow > row )  // 删除
	{
		tableView->setRowCount( row );
		for ( int i = currentRow - 1; i >= 0; i-- )
		{
			if ( i > row - 1 )
			{
				tableView->removeRow( i );
			}
			else
			{
				for ( int j = 0; j < currentColumn; j++ )
				{
					tableView->item( i, j )->setText( QString( "" ) );  
				}
			}
		}
	}
	else // 初始化
	{
		for ( int i = 0; i < currentRow; i++ )
		{
			for ( int j = 0; j < currentColumn; j++ )
			{
				tableView->item( i, j )->setText( QString( "" ) );  	
			}
		}
	}
}

void CAlarmTab::setItemCheckable( QTableWidget* tableView, int column )
{
	int rowCount = tableView->rowCount();
	for ( int i = 0; i < rowCount; i++ )
	{
		tableView->item( i, column )->setCheckState( Qt::Unchecked );
	}
}

bool CAlarmTab::init( mpc::nsdk::IManageSession* pSession, QString& sessionLabel )
{
	m_isInited = false; // 标识窗口不能处理告警资源表格项显示状态
   
	// 清除日志显示
	// initTableWiget( m_tableViewAlarmLog, QStringList(), 0 );

	// 获得告警资源信息
	bool flag;
	flag = m_alarmController.init( pSession, this, sessionLabel );
	QVector<SAlarmResInfo>& vecAlarmInfo = m_alarmController.getAlarmRes();

	CAlarmRegRecord::getRecord().getAlarmRegistered( pSession, m_alarmController.getAlarmRegistered() );

	// 初始化告警显示窗口
	initTableWiget( m_tableViewAlarmRes, QStringList(), vecAlarmInfo.size() );
	setItemCheckable( m_tableViewAlarmRes, APPEAR_NOTIFY );
	setItemCheckable( m_tableViewAlarmRes, CONFIRM_NOTIFY );
	setItemCheckable( m_tableViewAlarmRes, CLEAN_NOTIFY  );
	
    // 设置告警资源数据
	setAlarmResWidget( vecAlarmInfo );
	m_isInited = true;
	return flag;
}

void CAlarmTab::setAlarmResWidget( QVector<SAlarmResInfo>& vecAlarmInfo )
{
	int rowCount = m_tableViewAlarmRes->rowCount();
	QVector<SAlarmResInfo>::iterator it = vecAlarmInfo.begin();
	
	m_checkBoxState.clear();
	for ( int i = 0; i < rowCount; i++, it++ )
	{
		m_checkBoxState.push_back( SOptcheckBoxState() );

		m_tableViewAlarmRes->item( i, ALARM_RES_NAME )->setText( QString( it->alarmName.c_str() ) );
		
		if ( m_alarmController.isRegestered( it->alarmId, mpc::nsdk::IAlarmController::ACTIVE ) )
		{
			m_checkBoxState[i].active = true;
			m_tableViewAlarmRes->item( i, APPEAR_NOTIFY )->setCheckState( Qt::Checked );
		}
		else
		{
			m_tableViewAlarmRes->item( i, APPEAR_NOTIFY )->setCheckState( Qt::Unchecked );
		}

		if ( m_alarmController.isRegestered( it->alarmId, mpc::nsdk::IAlarmController::CONFIRM ) )
		{
			m_checkBoxState[i].confirm = true;
			m_tableViewAlarmRes->item( i, CONFIRM_NOTIFY )->setCheckState( Qt::Checked );
		}
		else
		{
			m_tableViewAlarmRes->item( i, CONFIRM_NOTIFY )->setCheckState( Qt::Unchecked );
		}

		if ( m_alarmController.isRegestered( it->alarmId, mpc::nsdk::IAlarmController::CLEAR ) )
		{
			m_checkBoxState[i].clear = true;
			m_tableViewAlarmRes->item( i, CLEAN_NOTIFY )->setCheckState( Qt::Checked );
		}
		else
		{
			m_tableViewAlarmRes->item( i, CLEAN_NOTIFY )->setCheckState( Qt::Unchecked );
		}

        // 告警清除模式
        if ( mpc::nsdk::IAlarmController::CLEAR_MODE_AUTO == it->alarmClearMode )
        {
            m_tableViewAlarmRes->item( i, CLEAN_MODE )->setText( QString( QObject::tr("auto") ) );
        }
        else
        {
            m_tableViewAlarmRes->item( i, CLEAN_MODE )->setText( QString( QObject::tr("manual") ) );
        }

        // 显示编码，路段，桩号，方向
        m_tableViewAlarmRes->item( i, ALARM_RES_CODE )->setText( QString::fromLocal8Bit( it->code.c_str() ) );
        m_tableViewAlarmRes->item( i, ALARM_RES_SECTION )->setText( QString::fromLocal8Bit( it->section.c_str() ) );
        m_tableViewAlarmRes->item( i, ALARM_RES_LANDMARK )->setText( QString::fromLocal8Bit( it->landmark.c_str() ) );
        m_tableViewAlarmRes->item( i, ALARM_RES_DIRECTION )->setText( QString::fromLocal8Bit( it->direction.c_str() ) );
	}
}

extern QString stringFromUint32Time( unsigned int nTime );

void CAlarmTab::setAlarmLogWidget( SAlarmNotifyData& notifyData )
{
	
	int i = 0;
	m_tableViewAlarmLog->insertRow( i );
	m_tableViewAlarmLog->setRowHeight( i , LIST_ROW_HEIGHT );

	// 创建部件
	int columnCount = m_tableViewAlarmLog->columnCount();
	for ( int j = 0; j < columnCount; j++ )
	{
		m_tableViewAlarmLog->setItem( i, j, new QTableWidgetItem( tr("") ) );
	}
	
	// 设置显示数据
	SAlarmNotifyData& it = notifyData;

	// 告警源名字
	std::string alarmName("");
	CAlarmRegRecord::getRecord().getAlarmNameFromId( it.alarmResId, alarmName );
	m_tableViewAlarmLog->item( i, ALARM_NAME )->setText( QString( alarmName.c_str() ) );

	// cms
	m_tableViewAlarmLog->item( i, CMS_LABLE )->setText( it.sessionLabel );

	// 通知类型 和 操作用户
	if ( it.notifyType == mpc::nsdk::IAlarmController::ACTIVE )
	{
		m_tableViewAlarmLog->item( i, NOTIFY_TYPE )->setText( tr("alarm active") );  

        // 增加确认按钮
        QPushButton *pConfirmBtn = new QPushButton( tr("confirm") );
        pConfirmBtn->setProperty("alarmid", QByteArray( (const char*)it.recordId.id, 16 ) );
        connect(pConfirmBtn, SIGNAL(clicked()), this, SLOT(onConfirmButtonClicked()));
        m_tableViewAlarmLog->setCellWidget(i, ALARM_BUTTON_CONFIRM, pConfirmBtn);

        // 增加清除按钮
        QPushButton *pCleanBtn = new QPushButton( tr("clear") );
        pCleanBtn->setProperty("alarmid", QByteArray( (const char*)it.recordId.id, 16 ) );
        connect(pCleanBtn, SIGNAL(clicked()), this, SLOT(onCleanButtonClicked()));
        m_tableViewAlarmLog->setCellWidget(i, ALARM_BUTTON_CLEAN, pCleanBtn);
	}
	else if ( it.notifyType == mpc::nsdk::IAlarmController::CLEAR )  // 清除
	{
		m_tableViewAlarmLog->item( i, NOTIFY_TYPE )->setText( tr("alarm clean") );
		m_tableViewAlarmLog->item( i, ALARM_OPERATOR_USER )->setText( QString::fromUtf8( it.userName.c_str() ) ); 

        // 告警清除类型
        if ( mpc::nsdk::IAlarmController::CLEAR_TYPE_AUTO == it.clearType )
        {
            m_tableViewAlarmLog->item( i, ALARM_CLEAN_TYPE )->setText( tr("auto") );
        }
        else
        {
            m_tableViewAlarmLog->item( i, ALARM_CLEAN_TYPE )->setText( tr("manual") );
        }
	}
	else  // 确认
	{
		m_tableViewAlarmLog->item( i, NOTIFY_TYPE )->setText( tr("alarm confirm") );
		m_tableViewAlarmLog->item( i, ALARM_OPERATOR_USER )->setText( QString::fromUtf8( it.userName.c_str() ) );
	}

	// 告警类型
	if ( mpc::nsdk::IAlarmController::SWITCH == it.alarmType )
	{
		m_tableViewAlarmLog->item( i, ALARM_TYPE )->setText( tr("switch") );
	}
	else if( mpc::nsdk::IAlarmController::VIDEOLOSS == it.alarmType )
	{
		m_tableViewAlarmLog->item( i, ALARM_TYPE )->setText( tr("video loss") );
	}

	// 告警时间
	m_tableViewAlarmLog->item( i, ALARM_TIME )->setText( stringFromUint32Time( it.time ) );


    // 最多只容纳300条记录，超出的需要清除掉
    int     maxRowCount = 300;
    int     rowCount = m_tableViewAlarmLog->rowCount();
    if ( rowCount > maxRowCount )
    {
        int rowIndex = rowCount-1;
        for ( ; rowIndex >= maxRowCount; --rowIndex )
        {
            m_tableViewAlarmLog->removeRow( rowIndex );
        }
    }
}


void CAlarmTab::customEvent( QEvent * event )
{
	if ( AMS_ALARM_CLEAN_EVENT == event->type()
		|| AMS_ALARM_ACTIVE_EVENT == event->type()
        || AMS_ALARM_CONFIRM_EVENT == event->type() )
	{
		CAlarmNotifyEvent* e = ( CAlarmNotifyEvent* )event;
		setAlarmLogWidget( e->getData() );
	}
    else if ( AMS_OFF_LINE == event->type() )
    {
        CAlarmNotifyEvent* e = ( CAlarmNotifyEvent* )event;

        // 清除勾选
        clearSelectOnAmsOff( e->getData().alarmController );
    }

	event->accept();
}

void CAlarmTab::alarmResItemChangedSlot( int row, int column )
{
    if ( m_amsOffing )  
    {
        return;
    }

	if ( !m_isInited || (column != APPEAR_NOTIFY && column != CLEAN_NOTIFY && column != CONFIRM_NOTIFY) ) 
	{
		return ;
	}

	// checkbox 状态判断
	bool checked = ( m_tableViewAlarmRes->item( row, column )->checkState() == Qt::Checked );
	
	bool lastChecked;
    if ( column == APPEAR_NOTIFY )
		lastChecked = m_checkBoxState[row].active;
	else if ( column == CONFIRM_NOTIFY )
		lastChecked = m_checkBoxState[row].confirm;
	else if ( column == CLEAN_NOTIFY )
		lastChecked = m_checkBoxState[row].clear;
	else
		return ;

	if ( checked == lastChecked )  // checkbox 状态没有发生变化
		return ;

	// 通知类型
	mpc::nsdk::IAlarmController::EAlarmNotifyType notifyType;
	if ( column == APPEAR_NOTIFY )
		notifyType = mpc::nsdk::IAlarmController::ACTIVE;
	else if ( column == CLEAN_NOTIFY )
		notifyType = mpc::nsdk::IAlarmController::CLEAR;
	else if ( column == CONFIRM_NOTIFY )
		notifyType = mpc::nsdk::IAlarmController::CONFIRM;
	else
		return ;

	mpc::nsdk::IAlarmController* pAlarmController = m_alarmController.getController();

	// register
	if ( checked && !lastChecked )
	{
		QVector<SAlarmResInfo>& vecRes = m_alarmController.getAlarmRes();
		if ( NULL == pAlarmController )
		{
			QMessageBox::information( this
                                    , QObject::tr( "regester" )
                                    , QObject::tr( "regester fail." )
                                    , QObject::tr( "ok" ) );
			m_tableViewAlarmRes->item( row, column )->setCheckState( Qt::Unchecked );
            std::cout << "register fail, alarm controller is null.\n";
			return ;
		}
		if ( pAlarmController->registerAlarm( vecRes[row].alarmId, notifyType ) )
		{
			// 添加到全局告警数据
			CAlarmRegRecord::getRecord().addRegister( m_alarmController.getSession()
                                                    , vecRes[row].alarmId
                                                    , vecRes[row].alarmName
                                                    , notifyType );
			if ( column == APPEAR_NOTIFY )
				m_checkBoxState[row].active = true;
			else if ( column == CONFIRM_NOTIFY )
				m_checkBoxState[row].confirm = true;
			else if ( column == CLEAN_NOTIFY )
				m_checkBoxState[row].clear = true;

		}
		else  // 订阅失败
		{
			QMessageBox::information( this
                                    , QObject::tr( "regester" )
                                    , QObject::tr( "regester fail!" )
                                    , QObject::tr( "ok" ) );
			m_tableViewAlarmRes->item( row, column )->setCheckState( Qt::Unchecked );
		}
	}

	// unregiseter
	if (  !checked && lastChecked )
	{
		QVector<SAlarmResInfo>& vecRes = m_alarmController.getAlarmRes();
		if ( NULL == pAlarmController )
		{
			QMessageBox::information( this
                                    , QObject::tr( "unregester" )
                                    , QObject::tr( "unregester fail." )
                                    , QObject::tr("ok") );
			m_tableViewAlarmRes->item( row, column )->setCheckState( Qt::Checked );
            std::cout << "unregister fail, alarm controller is null.\n";
			return ;
		}
		if ( pAlarmController->unregisterAlarm( vecRes[row].alarmId, notifyType ) )
		{
			
			// 添加到全局告警数据
			CAlarmRegRecord::getRecord().delRegister( m_alarmController.getSession()
                                                    , vecRes[row].alarmId
                                                    , notifyType );
			if ( column == APPEAR_NOTIFY )
				m_checkBoxState[row].active = false;
			else if ( column == CONFIRM_NOTIFY )
				m_checkBoxState[row].confirm = false;
			else if ( column == CLEAN_NOTIFY )
				m_checkBoxState[row].clear = false;
		}
		else // 取消订阅失败
		{
			QMessageBox::information( this
                                    , QObject::tr( "unregester" )
                                    , QObject::tr( "unregester fail!" )
                                    , QObject::tr( "ok" ) );
            m_tableViewAlarmRes->item( row, column )->setCheckState( Qt::Checked );
		}
	} 
}

bool CAlarmTab::fini( mpc::nsdk::IManageSession* pSession )
{
	if ( NULL != pSession ) // 关闭单个session
	{
		if ( pSession == m_alarmController.getSession() )  // 当前显示与传入的session有关
		{
			m_alarmController.fini( m_alarmController.getSession() );
			initTableWiget( m_tableViewAlarmRes, QStringList(), 0 ); // 清除告警资源列表
			m_checkBoxState.clear();
			m_isInited = false;
		}
        else
        {
            m_alarmController.fini( pSession );
        }
	}
	else // 关闭所有session
	{
		m_alarmController.fini();
		initTableWiget( m_tableViewAlarmRes, QStringList(), 0 );  
		m_checkBoxState.clear();
		m_isInited = false;
	}

	return true;
}

void CAlarmTab::delRegisterBySession( mpc::nsdk::IManageSession* pSession )
{
	CAlarmRegRecord::getRecord().delRegisterBySession( pSession );
}

void CAlarmTab::addRegisterBySession( mpc::nsdk::IManageSession* pSession )
{
    CAlarmRegRecord::getRecord().addRegisterBySession( pSession );
}

void CAlarmTab::onConfirmButtonClicked()
{
    QPushButton *senderObj = qobject_cast<QPushButton*>(sender());
    if ( !senderObj )
    {
        return;
    }

    if ( !m_alarmController.getController() )
        return ;

    QByteArray  qalarmId = senderObj->property("alarmid").toByteArray();
    mpc::nsdk::SAlarmRecordId      alarmId;

    memcpy(alarmId.id, qalarmId.data(), 16);

    m_alarmController.getController()->confirmAlarm(alarmId);
}

void CAlarmTab::onCleanButtonClicked()
{
    QPushButton *senderObj = qobject_cast<QPushButton*>(sender());
    if ( !senderObj )
    {
        return;
    }

    if ( !m_alarmController.getController() )
        return ;

    QByteArray  qalarmId = senderObj->property("alarmid").toByteArray();
    mpc::nsdk::SAlarmRecordId      alarmId;

    memcpy(alarmId.id, qalarmId.data(), 16);

    m_alarmController.getController()->cleanAlarm(alarmId);
}

void CAlarmTab::clearSelectOnAmsOff( mpc::nsdk::IAlarmController *alarmController )
{
    // 当界面显示的是与此告警控制器相关的资源的订阅，清除所有的勾选
    m_amsOffing = true;
    if ( alarmController == m_alarmController.getController() ) 
    {
        int rowCount = m_tableViewAlarmRes->rowCount();
        for ( int j = 0; j < rowCount; j++ )
        {
            m_tableViewAlarmRes->item( j, APPEAR_NOTIFY )->setCheckState( Qt::Unchecked );
            m_tableViewAlarmRes->item( j, CONFIRM_NOTIFY )->setCheckState( Qt::Unchecked );
            m_tableViewAlarmRes->item( j, CLEAN_NOTIFY )->setCheckState( Qt::Unchecked ); 
        }
    }
    m_amsOffing = false;

    std::vector< SOptcheckBoxState >::iterator  itr = m_checkBoxState.begin();
    for ( ; m_checkBoxState.end() != itr; ++itr )
    {
        (*itr).active = false;
        (*itr).confirm = false;
        (*itr).clear = false;
    }

    // 清除记录
    std::vector<mpc::nsdk::IManageSession*> vecSession;
    m_alarmController.getSessionByController( alarmController, vecSession );
    std::vector<mpc::nsdk::IManageSession*>::iterator it = vecSession.begin();
    for ( ; it != vecSession.end(); ++it )
    {
        CAlarmRegRecord::getRecord().delRegisterBySession( *it );
    }
}

void CAlarmTab::clearCurrentTable()
{
	initTableWiget( m_tableViewAlarmRes, QStringList(), 0 ); // 清除告警资源列表	
}





