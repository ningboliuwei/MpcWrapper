#include "ReplayTab.hpp"
#include "MediaPlayerWidget.hpp"
#include "DownLoadWidget.hpp"
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtCore/QRect>
#include "../include/Common.hpp"
 
CReplayTab::CReplayTab(QWidget *parent, Qt::WindowFlags flag)
:QWidget(parent, flag)
{
    this->resize(1000, 800);

    verticalLayout = new QVBoxLayout(this);
    horizontalLayout = new QHBoxLayout();
   
	//  播放窗口
	int len = sizeof( m_playWidget ) / sizeof( m_playWidget[0] );
	for ( int i = 0; i < len; i++  )
	{
		 m_playWidget[i] = new CMediaPlayerWidget(this, 0, RE_PLAY);
	}

	QGridLayout* gridLayout = new QGridLayout();
	for ( int i = 0; i < len; i++ )
	{
		gridLayout->addWidget(m_playWidget[i], i / REPLAY_WIDGET_PER_LINE, i % REPLAY_WIDGET_PER_LINE, 1, 1);
	}
	gridLayout->setSpacing( 2 );
	horizontalLayout->addLayout( gridLayout );
	m_timeSegWidget = new CTimeSegmentWidget(this);

    horizontalLayout->addWidget(m_timeSegWidget);

    verticalLayout->addLayout(horizontalLayout);
	m_controlWidget = new CReplayControlWidget( this, Qt::FramelessWindowHint );

	// 空闲窗口
	QWidget* pFreeWidget = new QWidget( this, Qt::FramelessWindowHint );
	pFreeWidget->setFixedSize( 185, 50 );
	//pFreeWidget->setAutoFillBackground(true);
	//pFreeWidget->setPalette( QPalette( QColor( 200, 200, 200 ) ) );

	QHBoxLayout* ctrlhorizontalLayout = new QHBoxLayout();
	ctrlhorizontalLayout->addWidget( m_controlWidget );
	ctrlhorizontalLayout->addWidget( pFreeWidget );

	verticalLayout->addLayout( ctrlhorizontalLayout );
	verticalLayout->setAlignment( ctrlhorizontalLayout, Qt::AlignBottom | Qt::AlignLeft );
    // verticalLayout->addWidget(m_controlWidget);
	// verticalLayout->setAlignment( m_controlWidget, Qt::AlignBottom | Qt::AlignLeft ); 

	m_activeWidget = m_playWidget[0];
	m_downLoadWidget = new CDownLoadWidget( 0 );

	m_controlWidget->updateData();
}
 
CReplayTab::~CReplayTab()
{}

QWidget* CReplayTab::getActivePlayWidget()
{
	return m_activeWidget;
}

bool CReplayTab::initActiveWidget( mpc::nsdk::IManageSession* session, const SResourceInfo& resInfo )
{
	m_activeWidget->play( session, resInfo );
	m_controlWidget->updateData();
	this->m_timeSegWidget->updateList( m_activeWidget->getReplayerController().getTimeSegMent() );
	m_activeWidget->update();
	return true;
}

bool CReplayTab::fini()
{
	int len = sizeof( m_playWidget ) / sizeof( m_playWidget[0] );
	for ( int i = 0; i < len; i++  )
	{
		m_playWidget[i]->fini();
	}

	m_controlWidget->updateData();
	m_timeSegWidget->updateList( m_activeWidget->getReplayerController().getTimeSegMent() );

	return true;
}

void CReplayTab::paintEvent( QPaintEvent * event )
{
	if ( m_activeWidget )
	{
		QPainter* paint = new QPainter; 
		paint->begin( this ); 
		paint->setPen( QPen( Qt::red, 4, Qt::DashLine ) );      // 设置画笔形式 
		paint->setBrush( QBrush( Qt::red, Qt::SolidPattern ) ); // 设置画刷形式 
		QRect rect = m_activeWidget->geometry();
		paint->drawRect( rect ); 
		paint->end(); 
	}
}

void CReplayTab::mousePressEvent( QMouseEvent * event )
{
	int x = event->x();
	int y = event->y();

	QRect rect;
	int len = sizeof( m_playWidget ) / sizeof( m_playWidget[0] );
	for ( int i = 0; i < len; i++  )
	{
		rect = m_playWidget[i]->geometry();
		if ( x > rect.left() && x < rect.right() && y > rect.top() && y < rect.bottom()   )
		{
			updateActiveWidget( m_playWidget[i] );
			break ;
		}
	}
}

bool CReplayTab::closeSession( mpc::nsdk::IManageSession* session )
{
	int len = sizeof( m_playWidget ) / sizeof( m_playWidget[0] );
	for ( int i = 0; i < len; i++  )
	{
		m_playWidget[i]->closeSession( session );
	}

	m_controlWidget->updateData();
	m_timeSegWidget->updateList( m_activeWidget->getReplayerController().getTimeSegMent() );
	return true;
}

void CReplayTab::stopActiveWidget() 
{ 
	m_activeWidget->fini();
	m_controlWidget->updateData();
	m_timeSegWidget->updateList( m_activeWidget->getReplayerController().getTimeSegMent() );
}

void CReplayTab::stopAll() 
{  
	fini(); 
}

void CReplayTab::updateActiveWidget( CMediaPlayerWidget* activeWidget )
{
	m_activeWidget = activeWidget;

	// 显示更新
	this->update();

	m_controlWidget->updateData();
	this->m_timeSegWidget->updateList( m_activeWidget->getReplayerController().getTimeSegMent() );
}

void CReplayTab::closeDownLoadWidget()
{
	if ( m_downLoadWidget != NULL )
	{
		m_downLoadWidget->release();
		m_downLoadWidget->hide();

		delete m_downLoadWidget;
		m_downLoadWidget = NULL;
	}
}

// 静音
void CReplayTab::mute( bool isMute )
{
	int len = sizeof( m_playWidget ) / sizeof( m_playWidget[0] );
	for ( int i = 0; i < len; i++  )
	{
		CRePlayerController& controller = m_playWidget[i]->getReplayerController();
		if ( m_playWidget[i] == m_activeWidget )
		{
			if ( controller.mute() == isMute ) 
			{ 
				continue;
			}
			else
			{
				controller.mute( isMute );
			}
		}
		else
		{
			if ( !controller.mute() )
			{
				controller.mute( true );
			}
		}
	}
}

// 调节音量
void CReplayTab::volume( int nVol )
{
	int len = sizeof( m_playWidget ) / sizeof( m_playWidget[0] );
	for ( int i = 0; i < len; i++  )
	{
		CRePlayerController& controller = m_playWidget[i]->getReplayerController();
		if ( m_playWidget[i] == m_activeWidget )
		{
			if ( controller.mute() )
			{
				controller.mute( false );
			}

			controller.volume( nVol );
		}
		else
		{
			if ( !controller.mute() )
			{
				controller.mute( true );
			}
		}
	}
}

void CReplayTab::setAllPlayerMute()
{
	int len = sizeof( m_playWidget ) / sizeof( m_playWidget[0] );
	for ( int i = 0; i < len; i++  )
	{
		CRePlayerController& controller = m_playWidget[i]->getReplayerController();
		if ( !controller.mute() )
		{
			controller.mute( true );
		}
	}
}

void CReplayTab::dragEventHandle( const QVector<intptr_t> &manageSessionVec
                                 , const QVector<QString> &resNameVec
                                 , const QVector<QString> &resIdVec
                                 , const QVector<bool> &ptzStatusVec
                                 , QWidget *ptr )
{
    //参数有误
    if (manageSessionVec.empty() || ptr == NULL)
    {
        return;
    }

    //查找当前历史视频播放窗口
    uint32_t nIndex = -1;
    uint32_t len = sizeof( m_playWidget ) / sizeof( m_playWidget[0] );
    for ( uint32_t i = 0; i < len; i++  )
    {
        if (m_playWidget[i] == ptr)
        {
            nIndex = i;
            break;
        }
    }

    //未拖放到视频播放窗口
    if (nIndex == -1)
    {
        return;
    }

    for (int i=0; i<(int)manageSessionVec.size() && nIndex < REPLAY_WIDGET_NUM; ++i)
    {
        m_activeWidget = m_playWidget[nIndex];
        SResourceInfo resInfo;

        //将UUID转换为SResId类型
        std::string str(resIdVec[i].toLocal8Bit());
        str2ResId(str, resInfo.resId);

        //获取资源名称
        resInfo.resName = resNameVec[i].toLocal8Bit();

        resInfo.isHasPtz = ptzStatusVec[i];

        initActiveWidget((mpc::nsdk::IManageSession*)manageSessionVec[i], resInfo);
        ++nIndex;
    }
}






 


