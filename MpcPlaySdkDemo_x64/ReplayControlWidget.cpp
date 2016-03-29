#include <QtGui/QFileDialog>
#include "ReplayControlWidget.hpp"
#include "ReplayTab.hpp"
#include "MediaPlayerWidget.hpp"
#include "ReplayerController.hpp"
#include "DownLoadController.hpp"
#include "DownLoadWidget.hpp"
#include <iostream>
#include <QtGui/QMessageBox>
 
CReplayControlWidget::CReplayControlWidget( QWidget *parent, Qt::WindowFlags flag )
	: QWidget( parent, flag )
	, m_replayWidget( (CReplayTab*) parent )
	, m_snapPath( "" )
{
	// 资源标签 第一列
    m_labelID = new QLabel( this );

	// 第二列
    m_pushButtonPlay = new QPushButton(this);
    m_pushButtonPause = new QPushButton(this);
	QVBoxLayout* verticalLayoutLine2 = new QVBoxLayout();
	verticalLayoutLine2->addWidget( m_pushButtonPlay );
	verticalLayoutLine2->addWidget( m_pushButtonPause );

    // 第三列
    m_pushButtonNext = new QPushButton(this);
    m_pushButtonSnap = new QPushButton(this);
	QVBoxLayout* verticalLayoutLine3 = new QVBoxLayout();
	verticalLayoutLine3->addWidget( m_pushButtonNext );
	verticalLayoutLine3->addWidget( m_pushButtonSnap );

	// 第四列
	m_pushButtonClose = new QPushButton(this);           
	m_pushButtonCloseAll = new QPushButton(this);
	QVBoxLayout* verticalLayoutLine4 = new QVBoxLayout();
	verticalLayoutLine4->addWidget( m_pushButtonClose );
	verticalLayoutLine4->addWidget( m_pushButtonCloseAll );

	// 第五列
	m_pushButtonSnapPath = new QPushButton(this);
	m_pushButtonLoad = new QPushButton(this);
	QVBoxLayout* verticalLayoutLine5 = new QVBoxLayout();
	verticalLayoutLine5->addWidget( m_pushButtonSnapPath );
	verticalLayoutLine5->addWidget( m_pushButtonLoad );

	// 第六列
	m_checkBoxMute = new QCheckBox(this);
	m_sliderVolume = new QSlider(this);
	m_sliderVolume->setOrientation(Qt::Horizontal);
	m_sliderVolume->setRange( 0, 100 );
	m_sliderVolume->setValue( 50 );
	m_sliderVolume->setSingleStep( 1 );
	m_sliderVolume->setPageStep( 1 );
	m_sliderVolume->setTickInterval( 1 );
	QHBoxLayout* horizontalLayoutVolume = new QHBoxLayout();
	horizontalLayoutVolume->addWidget(m_checkBoxMute);
	horizontalLayoutVolume->addWidget(m_sliderVolume);
	
	m_labelSpeed = new QLabel( this );
	m_sliderSpeed = new QSlider( this );
	m_sliderSpeed->setOrientation(Qt::Horizontal);
	m_sliderSpeed->setRange( 0, SLIDER_MAX_VAL );
	m_sliderSpeed->setValue( SLIDER_MID_VAL + 1 );
	m_sliderSpeed->setSingleStep( 1 );
	m_sliderSpeed->setPageStep( 1 );
	m_sliderSpeed->setTickInterval( SLIDER_MAX_VAL / 2 );	// 设置标尺的间距
	m_sliderSpeed->setTickPosition( QSlider::TicksAbove );  // 设置间距放在那边

	QHBoxLayout* horizontalLayoutSpeed = new QHBoxLayout();
	horizontalLayoutSpeed->addWidget(m_labelSpeed);
	horizontalLayoutSpeed->addWidget(m_sliderSpeed);

	QVBoxLayout* verticalLayoutLine6 = new QVBoxLayout();
	verticalLayoutLine6->addLayout( horizontalLayoutVolume );
	verticalLayoutLine6->addLayout( horizontalLayoutSpeed );

	// 外围布局器
    QHBoxLayout* horizontalLayoutOut = new QHBoxLayout( this ); 
	horizontalLayoutOut->setSpacing( 0 );
	horizontalLayoutOut->setMargin( 0 );
	horizontalLayoutOut->setContentsMargins( 0 ,0, 0, 0 );
	
	horizontalLayoutOut->addWidget( m_labelID );
    horizontalLayoutOut->addLayout( verticalLayoutLine2 );
	horizontalLayoutOut->addLayout( verticalLayoutLine3 );
	horizontalLayoutOut->addLayout( verticalLayoutLine4 );
	horizontalLayoutOut->addLayout( verticalLayoutLine5 );
	horizontalLayoutOut->addLayout( verticalLayoutLine6 );

    this->setWindowTitle( tr("CReplayControlWidget") );
    m_labelID->setText( tr("resource ID") );
    m_pushButtonPlay->setText( tr("play") );
    m_pushButtonPause->setText( tr("pause") );
    m_pushButtonNext->setText( tr("next frame") );
    m_pushButtonSnap->setText( tr("snap") );
	m_pushButtonSnapPath->setText( tr("snap path") );
	m_pushButtonLoad->setText( tr("down load") );
	m_checkBoxMute->setText( tr("mute") );
	m_labelSpeed->setText( tr("speed:") );
	m_pushButtonClose->setText( tr("close") );
	m_pushButtonCloseAll->setText( tr("close all") );
	m_pushButtonNext->setEnabled( false );
	m_checkBoxMute->setChecked( true );

	QSizePolicy sizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
	sizePolicy.setHorizontalStretch( 0 );
	sizePolicy.setVerticalStretch( 0 );
	sizePolicy.setHeightForWidth( this->sizePolicy().hasHeightForWidth() );
	setSizePolicy( sizePolicy );
	setMinimumSize( QSize(200, 60) );
	setMaximumSize( QSize(1900, 60) );

	connect( m_pushButtonPlay, SIGNAL( clicked() ), SLOT(onButtonClick() ) );
	connect( m_pushButtonPause, SIGNAL( clicked() ), SLOT(onButtonClick() ) );
	connect( m_pushButtonNext, SIGNAL( clicked() ), SLOT(onButtonClick() ) );
	connect( m_pushButtonSnap, SIGNAL( clicked() ), SLOT(onButtonClick() ) );
	connect( m_pushButtonLoad, SIGNAL( clicked() ), SLOT(onButtonClick() ) );
	connect( m_pushButtonSnapPath, SIGNAL( clicked() ), SLOT(onButtonClick() ) );
	connect( m_pushButtonClose, SIGNAL( clicked() ), SLOT(onButtonClick() ) );
	connect( m_pushButtonCloseAll, SIGNAL( clicked() ), SLOT(onButtonClick() ) );
	connect( m_checkBoxMute, SIGNAL( clicked( bool ) ), SLOT(onCheckBoxMute( bool ) ) );

	// 支持拖动、鼠标滚动、点击等操作
	m_sliderSpeed->setTracking( false );
	m_sliderVolume->setTracking( false );

	connect( m_sliderSpeed, SIGNAL( valueChanged(int) ), this, SLOT( onSpeedChanged(int) ) );  
	connect( m_sliderVolume, SIGNAL( valueChanged(int) ), this, SLOT( onVolumeChanged(int) ) );

	///////////////////////////////////////////////////////////////////
	////////////////////控件大小和位置/////////////////////////////////
	//
	//  宽度最小值：140 + 80*4 + 60 + 140 = 140 320 200
	//
	//
	//
	///////////////////////////////////////////////////////////////////
	const int LABLE_RES_WIDTH = 140;
	const int BUTTON_WIDTH = 80;
	const int BUTTON_HEIGHT = 30;
	const int BUTTON_MAX_WIDTH = 800;
	const int CHECKBOX_WIDTH = 50;
	const int SLIDER_WIDTH = 100;

	const int ITEM_MAX_WIDTH = 500;
	const int FIXED_HEIGHT = 60;

	// 资源标识大小
	sizePolicy.setHorizontalPolicy( QSizePolicy::Expanding );
	sizePolicy.setVerticalPolicy( QSizePolicy::Fixed );
	sizePolicy.setHorizontalStretch( 0 );
	sizePolicy.setVerticalStretch( 0 );
	sizePolicy.setHeightForWidth(m_labelID->sizePolicy().hasHeightForWidth());
	m_labelID->setSizePolicy(sizePolicy);
	m_labelID->setMinimumSize(QSize(LABLE_RES_WIDTH, FIXED_HEIGHT));
	m_labelID->setMaximumSize(QSize(ITEM_MAX_WIDTH, FIXED_HEIGHT));

	//m_labelID->setAutoFillBackground (true); 
	//m_labelID->setPalette( QPalette( QColor( 0, 100, 100 ) ) );

	// play button
	sizePolicy.setHorizontalPolicy( QSizePolicy::Expanding );
	sizePolicy.setVerticalPolicy( QSizePolicy::Fixed );
	sizePolicy.setHeightForWidth(m_pushButtonPlay->sizePolicy().hasHeightForWidth());
	m_pushButtonPlay->setSizePolicy(sizePolicy);
	m_pushButtonPlay->setMinimumSize(QSize(BUTTON_WIDTH, FIXED_HEIGHT/2));
	m_pushButtonPlay->setMaximumSize(QSize(ITEM_MAX_WIDTH, FIXED_HEIGHT/2));

	// pause button
	sizePolicy.setHeightForWidth( m_pushButtonPause->sizePolicy().hasHeightForWidth() );
	m_pushButtonPause->setSizePolicy(sizePolicy);
	m_pushButtonPause->setMinimumSize(QSize(BUTTON_WIDTH, FIXED_HEIGHT/2));
	m_pushButtonPause->setMaximumSize(QSize(ITEM_MAX_WIDTH, FIXED_HEIGHT/2));

	// next frame button
	sizePolicy.setHeightForWidth( m_pushButtonNext->sizePolicy().hasHeightForWidth() );
	m_pushButtonNext->setSizePolicy(sizePolicy);
	m_pushButtonNext->setMinimumSize(QSize(BUTTON_WIDTH, FIXED_HEIGHT/2));
	m_pushButtonNext->setMaximumSize(QSize(ITEM_MAX_WIDTH, FIXED_HEIGHT/2));

	// snap
	sizePolicy.setHeightForWidth( m_pushButtonSnap->sizePolicy().hasHeightForWidth() );
	m_pushButtonSnap->setSizePolicy(sizePolicy);
	m_pushButtonSnap->setMinimumSize(QSize(BUTTON_WIDTH, FIXED_HEIGHT/2));
	m_pushButtonSnap->setMaximumSize(QSize(ITEM_MAX_WIDTH, FIXED_HEIGHT/2));

	// snap path
	sizePolicy.setHeightForWidth( m_pushButtonSnapPath->sizePolicy().hasHeightForWidth() );
	m_pushButtonSnapPath->setSizePolicy(sizePolicy);
	m_pushButtonSnapPath->setMinimumSize(QSize(BUTTON_WIDTH, FIXED_HEIGHT/2));
	m_pushButtonSnapPath->setMaximumSize(QSize(ITEM_MAX_WIDTH, FIXED_HEIGHT/2));

	// 
	sizePolicy.setHeightForWidth( m_pushButtonClose->sizePolicy().hasHeightForWidth() );
	m_pushButtonClose->setSizePolicy(sizePolicy);
	m_pushButtonClose->setMinimumSize(QSize(BUTTON_WIDTH, FIXED_HEIGHT/2));
	m_pushButtonClose->setMaximumSize(QSize(ITEM_MAX_WIDTH, FIXED_HEIGHT/2));

	sizePolicy.setHeightForWidth( m_pushButtonCloseAll->sizePolicy().hasHeightForWidth() );
	m_pushButtonCloseAll->setSizePolicy(sizePolicy);
	m_pushButtonCloseAll->setMinimumSize(QSize(BUTTON_WIDTH, FIXED_HEIGHT/2));
	m_pushButtonCloseAll->setMaximumSize(QSize(ITEM_MAX_WIDTH, FIXED_HEIGHT/2));

	sizePolicy.setHeightForWidth( m_pushButtonLoad->sizePolicy().hasHeightForWidth() );
	m_pushButtonLoad->setSizePolicy(sizePolicy);
	m_pushButtonLoad->setMinimumSize(QSize(BUTTON_WIDTH, FIXED_HEIGHT/2));
	m_pushButtonLoad->setMaximumSize(QSize(ITEM_MAX_WIDTH, FIXED_HEIGHT/2));

	// checkbox mute
	sizePolicy.setHorizontalPolicy( QSizePolicy::Fixed );
	sizePolicy.setVerticalPolicy( QSizePolicy::Fixed );
	sizePolicy.setHeightForWidth(m_checkBoxMute->sizePolicy().hasHeightForWidth());
	m_checkBoxMute->setSizePolicy(sizePolicy);
	m_checkBoxMute->setMinimumSize(QSize(CHECKBOX_WIDTH, FIXED_HEIGHT/2));
	m_checkBoxMute->setMaximumSize(QSize(CHECKBOX_WIDTH, FIXED_HEIGHT/2));

	// slider 
	sizePolicy.setHorizontalPolicy( QSizePolicy::Expanding );
	sizePolicy.setVerticalPolicy( QSizePolicy::Fixed );
	sizePolicy.setHeightForWidth(m_sliderVolume->sizePolicy().hasHeightForWidth());
	m_sliderVolume->setSizePolicy(sizePolicy);
	m_sliderVolume->setMinimumSize(QSize(SLIDER_WIDTH, FIXED_HEIGHT/2));
	m_sliderVolume->setMaximumSize(QSize(ITEM_MAX_WIDTH, FIXED_HEIGHT/2));

	// speed label
	sizePolicy.setHorizontalPolicy( QSizePolicy::Fixed );
	sizePolicy.setVerticalPolicy( QSizePolicy::Fixed );
	sizePolicy.setHeightForWidth(m_labelSpeed->sizePolicy().hasHeightForWidth());
	m_labelSpeed->setSizePolicy(sizePolicy);
	m_labelSpeed->setMinimumSize(QSize(CHECKBOX_WIDTH, FIXED_HEIGHT/2));
	m_labelSpeed->setMaximumSize(QSize(CHECKBOX_WIDTH, FIXED_HEIGHT/2));

	// speed slider
	sizePolicy.setHorizontalPolicy( QSizePolicy::Expanding );
	sizePolicy.setVerticalPolicy( QSizePolicy::Fixed );
	sizePolicy.setHeightForWidth(m_sliderSpeed->sizePolicy().hasHeightForWidth());
	m_sliderSpeed->setSizePolicy(sizePolicy);
	m_sliderSpeed->setMinimumSize(QSize(SLIDER_WIDTH, FIXED_HEIGHT/2));
	m_sliderSpeed->setMaximumSize(QSize(ITEM_MAX_WIDTH, FIXED_HEIGHT/2));
}
 
CReplayControlWidget::~CReplayControlWidget()
{}

void CReplayControlWidget::onButtonClick()
{
	QPushButton *button = qobject_cast<QPushButton *>( sender() ); 
	if ( !button )
	{
		return ;
	}

	CMediaPlayerWidget* mediaPlayerWidget = (CMediaPlayerWidget*)( m_replayWidget->getActivePlayWidget() );
	CRePlayerController& playController = mediaPlayerWidget->getReplayerController();

	this->setEnabled( false );

	do 
	{
		if ( button == m_pushButtonPlay )
		{
			QPair<unsigned int, unsigned int> timeSeg;
			if ( !m_replayWidget->getSegTimeWidget()->getTimeSegment( timeSeg ) )
			{
				QMessageBox::information( this, 
					QObject::tr( "replay" ), 
					QObject::tr( "select time segment first!" ),
					QObject::tr("ok") );
				break;
			}

			// 更新检索时间
			if ( !playController.modifyTimes( timeSeg.first, timeSeg.second ) )
			{
#ifdef _DEBUG
				std::cout << "replay modify time fail." << std::endl;
#endif			
				break;
			}

			// 设置速度
			int value = m_sliderSpeed->value();
			int above = SPEED_ABLOVE( value );
			int below = SPEED_BELOW( value ); 
			if ( !playController.modifySpeed( above, below ) )
			{
#ifdef _DEBUG
				std::cout << "replay modify speed fail." << std::endl;
#endif		
				break;
			}

			// 播放
			if ( !playController.play() )
			{
#ifdef _DEBUG
				std::cout << "replay play fail." << std::endl;
#endif	
				break;
			}

			Sleep( 250 );
		}
		else if( button == m_pushButtonPause )
		{
			if ( playController.getPause() )  
			{
				playController.play();
				playController.setPause( false );
			}
			else
			{
				playController.pause();
				playController.setPause( true );
			}
		}
		else if( button == m_pushButtonNext )
		{
			playController.step();
		}
		else if( button == m_pushButtonSnap )
		{
			mediaPlayerWidget->snap( m_snapPath );
		}
		else if( button == m_pushButtonLoad )
		{
			CDownLoadWidget* w = m_replayWidget->getDownLoadWidget();

			QPair<unsigned int, unsigned int> timeSeg;
			if ( m_replayWidget->getSegTimeWidget()->getTimeSegment( timeSeg ) )
			{
				SDownLoadInfo info;
    			info.session = playController.getSession();
    			info.resId = playController.getResInfo().resId;
    			info.resName = playController.getResInfo().resName;
    			info.timeBegin = timeSeg.first; 
    			info.timeEnd = timeSeg.second;

    			w->downLoad( info );
			}

			w->setWindowFlags(  Qt::Window );
			w->showNormal();
		}
		else if( button == m_pushButtonSnapPath )
		{
			//选择一个文件夹;
			QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;

			m_snapPath = QFileDialog::getExistingDirectory(this,
				tr("snap path"),
				QString(),
				options);

			if ( m_snapPath.isEmpty() )
			{
				break ;
			}

			//判断文件路径合法性
			QDir dir( m_snapPath );
			if ( dir.exists() )
			{
				if ( m_snapPath[m_snapPath.length() - 1] != '\\' )
				{
					m_snapPath += "\\";
				}
			}
			else
			{
				m_snapPath = "";
			}
		}
		else if( button == m_pushButtonClose )       
		{
			m_replayWidget->stopActiveWidget();
		}
		else if ( button == m_pushButtonCloseAll )
		{
			m_replayWidget->stopAll();
		}

	} while( 0 );

	this->updateData();
	this->setEnabled( true );
}

void CReplayControlWidget::onCheckBoxMute( bool state )
{	
	CMediaPlayerWidget* mediaPlayerWidget = (CMediaPlayerWidget*)( m_replayWidget->getActivePlayWidget() );
	CRePlayerController& playController = mediaPlayerWidget->getReplayerController();
	
	m_replayWidget->mute( state );
	if ( !state )
	{
		m_sliderVolume->setValue( playController.volume() );
	}
}

void CReplayControlWidget::updateData()
{
	CMediaPlayerWidget* mediaPlayerWidget = (CMediaPlayerWidget*)( m_replayWidget->getActivePlayWidget() );
	CRePlayerController& playController = mediaPlayerWidget->getReplayerController();

	// 资源ID
	if ( playController.getResInfo().resName.empty() )
	{
		m_labelID->setText( tr("no media resource") );
	}
	else
	{
		m_labelID->setText( QString().fromLocal8Bit( playController.getResInfo().resName.c_str() ) );
	}
	
	m_pushButtonNext->setEnabled( playController.getPause() );

	if ( playController.getPause() )  
	{
		m_pushButtonPause->setText( tr("continue") );
		m_pushButtonNext->setEnabled( true );
	}
	else
	{
		m_pushButtonPause->setText( tr("pause") );
		m_pushButtonNext->setEnabled( false );
	}

	// mute
	if ( playController.mute() )
	{
		m_checkBoxMute->setChecked( true );
	}
	else
	{
		m_checkBoxMute->setChecked( false );
		m_sliderVolume->setValue( playController.volume() );
	}

	// speed
	m_sliderSpeed->setValue( playController.getSpeed() );
}

void CReplayControlWidget::onVolumeChanged( int value )
{
	CMediaPlayerWidget* mediaPlayerWidget = (CMediaPlayerWidget*)( m_replayWidget->getActivePlayWidget() );
	CRePlayerController& playController = mediaPlayerWidget->getReplayerController();

	QString tick;
	tick.sprintf( "%d", value );
	m_sliderVolume->setToolTip( tick );

	m_replayWidget->volume( value );
	m_checkBoxMute->setChecked( false );
}

void CReplayControlWidget::onSpeedChanged( int value )
{
	CMediaPlayerWidget* mediaPlayerWidget = (CMediaPlayerWidget*)( m_replayWidget->getActivePlayWidget() );
	CRePlayerController& playController = mediaPlayerWidget->getReplayerController();

	QString tick;
	int above = SPEED_ABLOVE( value );
	int below = SPEED_BELOW( value );
	tick.sprintf( "%d/%d", above, below );
	m_sliderSpeed->setToolTip( tick );

	playController.modifySpeed( above, below );
	playController.setSpeed( value );
}



