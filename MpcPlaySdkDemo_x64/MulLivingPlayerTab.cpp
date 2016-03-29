#include "MulLivingPlayerTab.hpp"
#include "LivingControlWidget.hpp"
#include "PtzControlWidget.hpp"
#include "MediaPlayerWidget.hpp"
#include "VidEffCtrWidget.hpp"
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include "../include/Common.hpp"

CMulLivingPlayerTab::CMulLivingPlayerTab(QWidget *parent, Qt::WindowFlags flag)
: QWidget(parent, flag)
{
    this->resize(1000, 800);

	verticalLayout = new QVBoxLayout( this ); // 外围

	horizontalLayout = new QHBoxLayout();  // 播放窗口和云台控制

	//  播放窗口
	int len = sizeof( m_playWidget ) / sizeof( m_playWidget[0] );
	for ( int i = 0; i < len; i++  )
	{
		m_playWidget[i] = new CMediaPlayerWidget(this, 0, LIVING_PLAY);
	}
	gridLayout = new QGridLayout();
	for ( int i = 0; i < len; i++ )
	{
		gridLayout->addWidget(m_playWidget[i], i / LIVING_WIDGET_PER_LINE, i % LIVING_WIDGET_PER_LINE, 1, 1);
	}
	gridLayout->setSpacing( 2 );
	horizontalLayout->addLayout( gridLayout );

    m_currentWindowNum = LIVING_WIDGET_NUM;

    // 
	QVBoxLayout* vPtzAndImgCtrlLayout = new QVBoxLayout();
	m_ptzWidget = new CPtzControlWidget( this, Qt::FramelessWindowHint );
    m_vidEffCtrWidget = new CVidEffCtrWidget( this, Qt::FramelessWindowHint );

	vPtzAndImgCtrlLayout->addWidget( m_ptzWidget );
	vPtzAndImgCtrlLayout->addWidget( m_vidEffCtrWidget );
	vPtzAndImgCtrlLayout->setAlignment( m_vidEffCtrWidget, Qt::AlignBottom );

	horizontalLayout->addLayout( vPtzAndImgCtrlLayout );
	//horizontalLayout->setAlignment( vPtzAndImgCtrlLayout, Qt::AlignRight );
	//vPtzAndImgCtrlLayout->setAlignment( Qt::AlignRight );

	verticalLayout->addLayout( horizontalLayout );

    // 播放控制窗口
	m_livingControlWidget = new CLivingControlWidget( this, Qt::FramelessWindowHint );

	// 空闲窗口
	QWidget* pFreeWidget = new QWidget( this, Qt::FramelessWindowHint );
	pFreeWidget->setFixedSize( 185, 50 );
	//pFreeWidget->setAutoFillBackground(true);
	//pFreeWidget->setPalette( QPalette( QColor( 200, 200, 200 ) ) );

	QHBoxLayout* ctrlhorizontalLayout = new QHBoxLayout();
	ctrlhorizontalLayout->addWidget( m_livingControlWidget );
	ctrlhorizontalLayout->addWidget( pFreeWidget );
	
	verticalLayout->addLayout( ctrlhorizontalLayout );
	verticalLayout->setAlignment( ctrlhorizontalLayout, Qt::AlignBottom | Qt::AlignLeft );
	//verticalLayout->addWidget(m_livingControlWidget);
	//verticalLayout->setAlignment( m_livingControlWidget, Qt::AlignBottom | Qt::AlignLeft ); 

	m_activeWidget = m_playWidget[0];

	// 初始化显示
	m_livingControlWidget->updateData();
	m_ptzWidget->updateData();
	m_vidEffCtrWidget->updateData();
}
 
CMulLivingPlayerTab::~CMulLivingPlayerTab()
{}

QWidget* CMulLivingPlayerTab::getActivePlayWidget()
{
	return m_activeWidget;
}

void CMulLivingPlayerTab::paintEvent( QPaintEvent * event )
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

void CMulLivingPlayerTab::mousePressEvent( QMouseEvent * event )
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
			if ( m_playWidget[i]->isVisible())
			{
				updateActiveWidget( m_playWidget[i] );
				break ;
			}
		}
	}
}

void CMulLivingPlayerTab::mouseDoubleClickEvent( QMouseEvent * event )
{
    if (m_currentWindowNum == 1)
    {
        return;
    }
    int x = event->x();
    int y = event->y();

    QRect rect;
    int len = sizeof( m_playWidget ) / sizeof( m_playWidget[0] );
    for ( int i = 0; i < len; i++  )
    {
        rect = m_playWidget[i]->geometry();
        if ( x > rect.left() && x < rect.right() && y > rect.top() && y < rect.bottom()   )
        {
            doubleClickPlayerWidget(i);
            break ;
        }
    }
}

bool CMulLivingPlayerTab::play( mpc::nsdk::IManageSession* session, const SResourceInfo& resInfo )
{
	m_activeWidget->play( session, resInfo );
	m_activeWidget->repaint();

	m_livingControlWidget->updateData();
	m_ptzWidget->updateData();
	m_vidEffCtrWidget->updateData();
	return true;
}

bool CMulLivingPlayerTab::fini()
{
	int len = sizeof( m_playWidget ) / sizeof( m_playWidget[0] );
	for ( int i = 0; i < len; i++  )
	{
		m_playWidget[i]->fini();
	}

	m_livingControlWidget->updateData();
	m_ptzWidget->updateData();
	m_vidEffCtrWidget->updateData();
	return true;
}

bool CMulLivingPlayerTab::closeSession( mpc::nsdk::IManageSession* session )
{
	int len = sizeof( m_playWidget ) / sizeof( m_playWidget[0] );
	for ( int i = 0; i < len; i++  )
	{
		m_playWidget[i]->closeSession( session );
	}

	m_livingControlWidget->updateData();
	m_ptzWidget->updateData();
	m_vidEffCtrWidget->updateData();
	return true;
}

void CMulLivingPlayerTab::stopActiveWidget() 
{ 
	m_activeWidget->fini();
	m_livingControlWidget->updateData();
	m_ptzWidget->updateData();
	m_vidEffCtrWidget->updateData();
}

void CMulLivingPlayerTab::stopAll() 
{  
	fini(); 
}

void CMulLivingPlayerTab::updateActiveWidget( CMediaPlayerWidget* activeWidget )
{
	m_activeWidget = activeWidget;

	this->update();
	m_livingControlWidget->updateData();
	m_ptzWidget->updateData();
	m_vidEffCtrWidget->updateData();
}

void CMulLivingPlayerTab::mute( bool isMute )
{
	int len = sizeof( m_playWidget ) / sizeof( m_playWidget[0] );
	for ( int i = 0; i < len; i++  )
	{
		CLivingPlayerController& controller = m_playWidget[i]->getMediaPlayerController();
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
void CMulLivingPlayerTab::volume( int nVol )
{
	int len = sizeof( m_playWidget ) / sizeof( m_playWidget[0] );
	for ( int i = 0; i < len; i++  )
	{
		CLivingPlayerController& controller = m_playWidget[i]->getMediaPlayerController();
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

void CMulLivingPlayerTab::setAllPlayerMute()
{
	int len = sizeof( m_playWidget ) / sizeof( m_playWidget[0] );
	for ( int i = 0; i < len; i++  )
	{
		CLivingPlayerController& controller = m_playWidget[i]->getMediaPlayerController();
		if ( !controller.mute() )
		{
			controller.mute( true );
		}
	}
}

void CMulLivingPlayerTab::dragEventHandle( const QVector<intptr_t> &manageSessionVec
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

    //查找当前实时视频播放窗口
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

    uint32_t nIndexBak = nIndex;
    for (int i=0; i<(int)manageSessionVec.size() && nIndex < LIVING_WIDGET_NUM; ++i)
    {
        m_activeWidget = m_playWidget[nIndex];
        SResourceInfo resInfo;

        //将UUID转换为SResId类型
        std::string str(resIdVec[i].toLocal8Bit());
        str2ResId(str, resInfo.resId);

        //获取资源名称
        resInfo.resName = resNameVec[i].toLocal8Bit();

        resInfo.isHasPtz = ptzStatusVec[i];

        play((mpc::nsdk::IManageSession*)manageSessionVec[i], resInfo);
        ++nIndex;
    }
    updateActiveWidget(m_playWidget[nIndexBak]);
}

void CMulLivingPlayerTab::updateLivingPlayers( const uint16_t &num )
{
    for ( int i = 0; i < LIVING_WIDGET_NUM; i++ )
    {
        if ( i >= num*num)
        {
            m_playWidget[i]->hide();
        }
        else
        {
            m_playWidget[i]->show();
        }
        gridLayout->addWidget(m_playWidget[i], i / num, i % num, 1, 1);
    }

    updateActiveWidget(m_playWidget[0]);

    m_currentWindowNum = num * num;
}

void CMulLivingPlayerTab::doubleClickPlayerWidget( const int &index )
{
    m_livingControlWidget->setPlayerWindowNum(1);
    for ( int i = 0; i < LIVING_WIDGET_NUM; i++ )
    {
        if ( i != index)
        {
            m_playWidget[i]->hide();
        }
        else
        {
            m_playWidget[i]->show();
            gridLayout->addWidget(m_playWidget[i], 0, 0, 1, 1);
            updateActiveWidget(m_playWidget[i]);
            m_currentWindowNum = 1;
        }
    }
}





