#include <Winsock2.h>
#include <Rpc.h>
#include <time.h>
#include <iostream>
#include <QtGui/QBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QStyle>
#include <QtGui/QPainter>
#include <QtGui/QImage>

#include "VideoPlayer.hpp"

VideoPlayer::VideoPlayer( int i
                         , std::string addr
                         , std::string user
                         , std::string pwd
                         , std::string uid
                         , uint32_t tim
                         , QWidget *parent )
: m_type( i ), m_addr( addr ), m_user( user ), m_pwd( pwd ), m_uid( uid ), m_tim( tim ), QWidget(parent)
, m_managerSession( 0 ), m_livingStream( 0 ), m_livingPlayer( 0 ), m_replaytream( 0 ), m_replayPlayer( 0 )
, m_mediaDecoder( 0 )
{
    resize( 600, 500 );

    m_videoWidget = new CDeocderDisplayWidget;
    m_videoWidget->setAutoFillBackground( true );
    m_videoWidget->setBackgroundRole( QPalette::Shadow );

    QSpacerItem *sapcer1 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    QSpacerItem *sapcer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    QSpacerItem *sapcer3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

    //RealPlay
    QAbstractButton *startButton = new QPushButton;
    startButton->setText("Real-Start");
    connect(startButton, SIGNAL(clicked()),this, SLOT(start()));

    QAbstractButton *stopButton = new QPushButton;
    stopButton->setText("Real-Stop");
    connect(stopButton, SIGNAL(clicked()),this, SLOT(stop()));

    //Replay
    QAbstractButton *replayPlayButton = new QPushButton;
    replayPlayButton->setText("Replay-Play");
    connect(replayPlayButton, SIGNAL(clicked()),this, SLOT(replayPlay()));

    QAbstractButton *replayPauseButton = new QPushButton;
    replayPauseButton->setText("Replay-Pause");
    connect(replayPauseButton, SIGNAL(clicked()),this, SLOT(replayPause()));

    QAbstractButton *replayStepButton = new QPushButton;
    replayStepButton->setText("Replay-Step");
    connect(replayStepButton, SIGNAL(clicked()),this, SLOT(replayStep()));

    //volome,mute,snap
    QAbstractButton *volumeButton = new QPushButton;
    volumeButton->setEnabled(true);
    volumeButton->setText("Volume");
    connect(volumeButton, SIGNAL(clicked()),this, SLOT(volume()));

    QAbstractButton *muteButton1 = new QPushButton;
    muteButton1->setEnabled(true);
    muteButton1->setText("Mute-false");
    connect(muteButton1, SIGNAL(clicked()),this, SLOT(mute1()));

    QAbstractButton *muteButton2 = new QPushButton;
    muteButton2->setEnabled(true);
    muteButton2->setText("Mute-true");
    connect(muteButton2, SIGNAL(clicked()),this, SLOT(mute2()));

    QAbstractButton *snapButton = new QPushButton;
    snapButton->setEnabled(true);
    snapButton->setText("Snap");
    connect(snapButton, SIGNAL(clicked()),this, SLOT(snap()));

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(startButton);
    controlLayout->addWidget(stopButton);
    controlLayout->addItem(sapcer1);

    QBoxLayout *replayControlLayout = new QHBoxLayout;
    replayControlLayout->setMargin(0);
    replayControlLayout->addWidget(replayPlayButton);
    replayControlLayout->addWidget(replayPauseButton);
    replayControlLayout->addWidget(replayStepButton);
    replayControlLayout->addItem(sapcer2);

    QBoxLayout *volumeControlLayout = new QHBoxLayout;
    volumeControlLayout->setMargin(0);
    volumeControlLayout->addWidget(volumeButton);
    volumeControlLayout->addWidget(muteButton1);
    volumeControlLayout->addWidget(muteButton2);
    volumeControlLayout->addWidget(snapButton);
    volumeControlLayout->addItem(sapcer3);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_videoWidget);
    layout->addLayout(controlLayout);
    layout->addLayout(replayControlLayout);
    layout->addLayout(volumeControlLayout);

    setLayout(layout);

    if( i == 1 )
    {
        startButton->setEnabled( false );
        stopButton->setEnabled( false );

        replayPlayButton->setEnabled( false );
        replayPauseButton->setEnabled( false );
        replayStepButton->setEnabled( false );

        setWindowTitle( "Living Test");
    }
    else if ( 2 == i )
    {
        setWindowTitle( "Replay Test");

        startButton->setEnabled( false );
        stopButton->setEnabled( false );

        replayPlayButton->setEnabled( true );
        replayPauseButton->setEnabled( true );
        replayStepButton->setEnabled( true );
    }
    else if ( 3 == i )
    {
        startButton->setEnabled( false );
        stopButton->setEnabled( false );

        replayPlayButton->setEnabled( false );
        replayPauseButton->setEnabled( false );
        replayStepButton->setEnabled( false );

        volumeButton->setEnabled( false );
        muteButton1->setEnabled( false );
        muteButton2->setEnabled( false );
        snapButton->setEnabled( false );

        setWindowTitle( "Decoder Test");
    }
}

VideoPlayer::~VideoPlayer()
{
}

bool str2ResId( const std::string &strId, mpc::nsdk::SResId &resId )
{
    if( strId.size() < 32 )
    {
        return false;
    }
    std::string tmp;
    for( size_t i = 0; i < strId.size(); ++i )
    {
        if( strId[i] >= '0' && strId[i] <= '9' )
        {
            tmp += strId[i];
        }
        else if( strId[i] >= 'a' && strId[i] <= 'f' )
        {
            tmp += strId[i];
        }
        else if( strId[i] >= 'A' && strId[i] <= 'F' )
        {
            tmp += strId[i];
        }
    }
    if( tmp.size() != 32 )
    {
        return false;
    }
    for( size_t i = 0; i < 16; ++i )
    {
        std::string item = tmp.substr( i*2, 2 );
        uint32_t itemValue;// = atoi( item.c_str() );
        sscanf( item.c_str(), "%2X", &itemValue );
        resId.id[i] = itemValue;
    }
    return true;
}

bool VideoPlayer::init()
{
//     {
//         mpc::nsdk::SResId resId;
//         std::string test = "01234567890123456789abcdefABCDEF";
//         bool b = str2ResId( test, resId );
// 
//         ::memset( resId.id, 0, 16 );
//         test = "01234567-8901-2345-6789-abcdefABCDEF";
//         b = str2ResId( test, resId );
// 
//         ::memset( resId.id, 0, 16 );
//         test = "01234567890123456789abcdefABCDE";
//         b = str2ResId( test, resId );
// 
//         ::memset( resId.id, 0, 16 );
//         test = "01234567890123456789abcdefABCDEFF";
//         b = str2ResId( test, resId );
// 
//         ::memset( resId.id, 0, 16 );
//         test = "01234567890123456789abcdefABCDEG";
//         b = str2ResId( test, resId );
//         return false;
//     }

	mpc::nsdk::SResId resId;
	if ( !str2ResId( m_uid, resId ) )
		return false;

    if( !mpc::nsdk::Runtime::init() )
        return false;

    if( !mpc::psdk::Runtime::init() )
    {
        mpc::nsdk::Runtime::fini();
        return false;
    }
    
    size_t pos = m_addr.find( ":" );
    if( pos == std::string::npos )
        return false;

    std::string ipStr = m_addr.substr( 0, pos );
    std::string portStr = m_addr.substr( pos+1, std::string::npos );

    uint32_t nServer = inet_addr( ipStr.c_str() );
    nServer = ntohl( nServer );
    
    uint16_t nPort = atoi( portStr.c_str() );

    m_managerSession = mpc::nsdk::Factory::createManageSession( nServer,
                                                                nPort,
                                                                m_user.c_str(),
                                                                m_user.length(),
                                                                m_pwd.c_str(),
                                                                m_pwd.length() );
    if( !m_managerSession )
    {
        mpc::nsdk::Runtime::fini();
        mpc::psdk::Runtime::fini();
        return false;
    }

    if( !m_managerSession->init() )
    {
        m_managerSession->release();

        mpc::nsdk::Runtime::fini();
        mpc::psdk::Runtime::fini();
        return false;
    }

    if( !m_managerSession->login() )
    {
        m_managerSession->fini();
        m_managerSession->release();

        mpc::nsdk::Runtime::fini();
        mpc::psdk::Runtime::fini();
        return false;
    }

	/*   
    UuidFromStringA( (unsigned char *)m_uid.c_str(), (UUID*)(&resId) );
    UUID *ttt = (UUID*)(&resId);
    ttt->Data1 = ntohl( ttt->Data1 );
    ttt->Data2 = ntohs( ttt->Data2 );
    ttt->Data3 = ntohs( ttt->Data3 );
	*/
	
    if( m_type == 1 )
    {
        m_livingStream = m_managerSession->createLivingStream( resId, false );
        if( !m_livingStream )
        {
            m_managerSession->logout();
            m_managerSession->fini();
            m_managerSession->release();
            
            mpc::nsdk::Runtime::fini();
            mpc::psdk::Runtime::fini();
            
            return false;
        }

        m_livingPlayer = mpc::psdk::Factory::createLivingPlayer( (HWND)m_videoWidget->winId() );
        if( !m_livingPlayer  )
        {
            m_livingStream->release();

            m_managerSession->logout();
            m_managerSession->fini();
            m_managerSession->release();
            
            mpc::nsdk::Runtime::fini();
            mpc::psdk::Runtime::fini();

            return false;
        }

        if( !m_livingPlayer->init() )
        {
            m_livingStream->release();
            m_livingPlayer->release();

            m_managerSession->logout();
            m_managerSession->fini();
            m_managerSession->release();

            mpc::nsdk::Runtime::fini();
            mpc::psdk::Runtime::fini();

            return false;
        }

        m_livingStream->setHandler( m_livingPlayer );

        if( !m_livingStream->init() )
        {
            m_livingPlayer->fini();
            m_livingStream->release();
            m_livingPlayer->release();

            m_managerSession->logout();
            m_managerSession->fini();
            m_managerSession->release();

            mpc::nsdk::Runtime::fini();
            mpc::psdk::Runtime::fini();

            return false;
        }
    }
    else if ( 2 == m_type )
    {
        // for test，省略了获取媒体源时间周期的步骤

        m_replaytream = m_managerSession->createReplayStream( resId );
        if( !m_replaytream )
        {
            m_managerSession->logout();
            m_managerSession->fini();
            m_managerSession->release();

            mpc::nsdk::Runtime::fini();
            mpc::psdk::Runtime::fini();

            return false;
        }

        m_replayPlayer = mpc::psdk::Factory::createReplayPlayer( (HWND)m_videoWidget->winId() );
        if( !m_replayPlayer  )
        {
            m_replaytream->release();

            m_managerSession->logout();
            m_managerSession->fini();
            m_managerSession->release();

            mpc::nsdk::Runtime::fini();
            mpc::psdk::Runtime::fini();

            return false;
        }

        if( !m_replayPlayer->init() )
        {
            m_replaytream->release();
            m_replayPlayer->release();

            m_managerSession->logout();
            m_managerSession->fini();
            m_managerSession->release();

            mpc::nsdk::Runtime::fini();
            mpc::psdk::Runtime::fini();

            return false;
        }

        m_replaytream->setHandler( m_replayPlayer );

        if( !m_replaytream->init() )
        {
            m_replayPlayer->fini();
            m_replaytream->release();
            m_replayPlayer->release();

            m_managerSession->logout();
            m_managerSession->fini();
            m_managerSession->release();

            mpc::nsdk::Runtime::fini();
            mpc::psdk::Runtime::fini();

            return false;
        }
    }
    else if ( 3 == m_type )
    {
        m_livingStream = m_managerSession->createLivingStream( resId, false );
        if( !m_livingStream )
        {
            m_managerSession->logout();
            m_managerSession->fini();
            m_managerSession->release();

            mpc::nsdk::Runtime::fini();
            mpc::psdk::Runtime::fini();

            return false;
        }

        m_mediaDecoder = mpc::psdk::Factory::createMediaDecoder( this );
        if( !m_mediaDecoder )
        {
            m_livingStream->release();

            m_managerSession->logout();
            m_managerSession->fini();
            m_managerSession->release();

            mpc::nsdk::Runtime::fini();
            mpc::psdk::Runtime::fini();

            return false;
        }

        if( !m_mediaDecoder->init() )
        {
            m_livingStream->release();
            m_mediaDecoder->release();

            m_managerSession->logout();
            m_managerSession->fini();
            m_managerSession->release();

            mpc::nsdk::Runtime::fini();
            mpc::psdk::Runtime::fini();

            return false;
        }

        m_livingStream->setHandler( m_mediaDecoder );

        if( !m_livingStream->init() )
        {
            m_mediaDecoder->fini();
            m_livingStream->release();
            m_mediaDecoder->release();

            m_managerSession->logout();
            m_managerSession->fini();
            m_managerSession->release();

            mpc::nsdk::Runtime::fini();
            mpc::psdk::Runtime::fini();

            return false;
        }
    }

    return true;
}

bool VideoPlayer::fini()
{
    if( m_livingStream )
    {
        m_livingStream->fini();
    }

    if( m_replaytream )
    {
        m_replaytream->fini();
    }



    if( m_livingPlayer )
    {
        m_livingPlayer->fini();
    }

    if( m_replayPlayer )
    {
        m_replayPlayer->fini();
    }

    if ( m_mediaDecoder )
    {
        m_mediaDecoder->fini();
    }



    if( m_livingStream )
    {
        m_livingStream->release(); 
        m_livingStream = 0;
    }

    if( m_livingPlayer )
    {
        m_livingPlayer->release();
        m_livingPlayer = 0;
    }

    if ( m_mediaDecoder )
    {
        m_mediaDecoder->release();
        m_mediaDecoder = 0;
    }

    if( m_replaytream )
    {
        m_replaytream->release();
        m_replaytream = 0;
    }
    
    if( m_replayPlayer ) 
    {
        m_replayPlayer->release();
        m_replayPlayer = 0;
    }

    if( m_managerSession )
    {
        m_managerSession->logout();
        m_managerSession->fini();
        m_managerSession->release();
        m_managerSession = 0;
    }

    mpc::nsdk::Runtime::fini();
    mpc::psdk::Runtime::fini();
    
    return true;
}

void VideoPlayer::start()
{
    std::cout << "start" << std::endl;
}

void VideoPlayer::stop()
{
    std::cout << "stop" << std::endl;
}

void VideoPlayer::replayPlay()
{
    std::cout << "replayPlay" << std::endl;

    if( !m_replayPlayer )
        return;

    m_replaytream->modifyTimes( m_tim, m_tim+1 );
    //m_replaytream->modifySpeed( m_nSpeed, m_dSpeed );

    m_replaytream->play();
    m_replayPlayer->play();
}

void VideoPlayer::replayPause()
{
    std::cout << "replayPause" << std::endl;

    if( !m_replayPlayer )
        return;

    m_replaytream->pause();
    m_replayPlayer->pause();
}

void VideoPlayer::replayStep()
{
    std::cout << "replayStep" << std::endl;

    if( !m_replayPlayer )
        return;

    m_replaytream->step();
    m_replayPlayer->step();
}

bool g_sign = false;
void VideoPlayer::volume()
{
    if( m_type == 1 )
    {
        if( !m_livingPlayer )
            return;

        if( g_sign )
        {
            std::cout << "cur volume= " << m_livingPlayer->volume() << ", set volume = 20" << std::endl;
            m_livingPlayer->volume( 20 );
            g_sign = false;
        }
        else
        {
            std::cout << "cur volume= " << m_livingPlayer->volume() << ", set volume = 50" << std::endl;
            m_livingPlayer->volume( 50 );
            g_sign = true;
        }
    }
    else
    {
        if( !m_replayPlayer )
            return;

        if( g_sign )
        {
            std::cout << "cur volume= " << m_replayPlayer->volume() << ", set volume = 20" << std::endl;
            m_replayPlayer->volume( 20 );
            g_sign = false;
        }
        else
        {
            std::cout << "volume = 50, cur volume= " << m_replayPlayer->volume() << ", set volume = 50" << std::endl;
            m_replayPlayer->volume( 50 );
            g_sign = true;
        }
    }
}

void VideoPlayer::mute1()
{   
    if( m_type == 1 )
    {
        if( !m_livingStream )
            return;

        std::cout << "cur mute = " << m_livingPlayer->mute() << ", set mute = false"<<std::endl;
        m_livingPlayer->mute( false );
    }
    else
    {
        if( !m_replayPlayer )
            return;

        std::cout << "cur mute = " << m_replayPlayer->mute() << ", set mute = false"<< std::endl;
        m_replayPlayer->mute( false );
    }
}

void VideoPlayer::mute2()
{
    if( m_type == 1 )
    {
        if( !m_livingStream )
            return;

        std::cout << "cur mute = " << m_livingPlayer->mute() << ", set mute = true" << std::endl;
        m_livingPlayer->mute( true );
    }
    else
    {
        if( !m_replayPlayer )
            return;

        std::cout << "cur mute = " << m_replayPlayer->mute() << ", set mute = true" << std::endl;
        m_replayPlayer->mute( true );
    }
}

void VideoPlayer::snap()
{
    std::cout << "snap" << std::endl;

    std::string path;
    uint32_t ts = time( 0 );
    char filename[16] = { 0 };
    sprintf( filename, "%08x.png", ts );
    path += filename;

    if( m_type == 1 )
    {
        if( !m_livingStream )
            return;

        m_livingPlayer->snap( path.c_str(), path.length() );
    }
    else
    {
        if( !m_replayPlayer )
            return;

        m_replayPlayer->snap( path.c_str(), path.length() );
    }
}

void VideoPlayer::picData(const mpc::psdk::SPicInfo & picInfo)
{
    if ( !m_videoWidget )
    {
        return;
    }

    m_videoWidget->paintYV12Data(picInfo.ptrY
                                , picInfo.ptrYSize
								, picInfo.ptrV
								, picInfo.ptrVSize
								, picInfo.ptrU
								, picInfo.ptrUSize
                                , picInfo.width
                                , picInfo.height );
}
