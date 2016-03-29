#include <QtGui/QFileDialog>
#include "LivingControlWidget.hpp"
#include "MulLivingPlayerTab.hpp"
#include "MediaPlayerWidget.hpp"
#include "LivingPlayerController.hpp"

QString SUB_STREAM = QString( QObject::tr("sub") );
QString MAIN_STREAM = QString( QObject::tr("main") );  

QString ONE_WINDOWS          = QString( QObject::tr("1 Windows") );
QString FOUR_WINDOWS         = QString( QObject::tr("4 Windows") ); 
QString NINE_WINDOWS         = QString( QObject::tr("9 Windows") ); 
QString SIXTEEN_WINDOWS      = QString( QObject::tr("16 Windows") ); 

CLivingControlWidget::CLivingControlWidget(QWidget *parent, Qt::WindowFlags flag)
: QWidget( parent, flag )
, m_mulLivingPlayWidget( (CMulLivingPlayerTab*)parent )
, m_snap_path( "" )
{
	setContentsMargins( 0, 0, 0, 0 );  // 窗体设置边框距
	
	const int32_t FIXED_HEIGHT = 50;  // 第二行高度
	const int32_t LABLE_STREAM_TYPE_WIDTH = 70;
	const int32_t COMBO_STREAM_TYPE_WIDTH = 50;
	const int32_t BUTTON_WIDTH = 40;
	const int32_t BUTTON_HEIGHT = 50;
	const int32_t BUTTON_MAX_WIDTH = 800;
	const int32_t CHECKBOX_WIDTH = 50;
	const int32_t SLIDER_WIDTH = 90;
	const int32_t SLIDER_HEIGHT = 26;
	const int32_t ITEM_MAX_WIDTH = 500;
	const int32_t LINE1HEIGHT = FIXED_HEIGHT/3;
	const int32_t LINE2HEIGHT = FIXED_HEIGHT/3*2;
	const int32_t COMBOX_HEIGHT = LINE2HEIGHT - 2;
	const int32_t COMBO_WINDOWS_WIDTH = 68;

	// 窗口大小策略
	QSizePolicy sizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
	sizePolicy.setHeightForWidth( this->sizePolicy().hasHeightForWidth() );
	setSizePolicy( sizePolicy );
	setMinimumSize( QSize( 350, FIXED_HEIGHT ) );
	setMaximumSize( QSize( 3000, FIXED_HEIGHT ) );

	// 窗口布局
    m_verticalLayout = new QVBoxLayout( this );  // 垂直
	m_verticalLayout->setSpacing( 0 );
	m_verticalLayout->setMargin(0);
	m_verticalLayout->setContentsMargins(0,0,0,0);

    // 资源标识 第一行
	QHBoxLayout* horizontalLayoutLine1 = new QHBoxLayout();
	horizontalLayoutLine1->setSpacing( 0 );
	horizontalLayoutLine1->setMargin( 0 );
	horizontalLayoutLine1->setContentsMargins( 0, 0, 0, 0 );
	horizontalLayoutLine1->setAlignment( Qt::AlignCenter );

	m_labelID = new QLabel( this );
	sizePolicy.setHorizontalPolicy( QSizePolicy::Expanding );
	sizePolicy.setVerticalPolicy( QSizePolicy::Fixed );
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(m_labelID->sizePolicy().hasHeightForWidth());
	m_labelID->setSizePolicy(sizePolicy);
	m_labelID->setMinimumSize(QSize(180, LINE1HEIGHT));
	m_labelID->setMaximumSize(QSize(180, LINE1HEIGHT));
	//m_labelID->setAutoFillBackground (true); 
	//m_labelID->setPalette( QPalette( QColor( 0, 100, 100 ) ) );
	horizontalLayoutLine1->addWidget( m_labelID );
	m_verticalLayout->addLayout( horizontalLayoutLine1 );
	
	// 第二行
	m_horizontalLayout = new QHBoxLayout();  // 外围水平
	m_horizontalLayout->setSpacing( 0 );
	m_horizontalLayout->setMargin(0);
	m_horizontalLayout->setContentsMargins(0,0,0,0); 
	m_horizontalLayout->setAlignment( Qt::AlignLeft );
	m_horizontalLayout->setSpacing( 0 );
	m_verticalLayout->addLayout( m_horizontalLayout );

	m_labelStremType = new QLabel( this );
	m_horizontalLayout->addWidget( m_labelStremType );
	comboBoxStreamType = new QComboBox( this );
	m_horizontalLayout->addWidget( comboBoxStreamType );
	m_pushButtonClose = new QPushButton(this);
	m_horizontalLayout->addWidget(m_pushButtonClose);
	m_pushButtonCloseAll = new QPushButton(this);
	m_horizontalLayout->addWidget(m_pushButtonCloseAll);
    m_pushButtonSnap = new QPushButton(this);
    m_horizontalLayout->addWidget(m_pushButtonSnap);
	m_pushButtonSnapPath = new QPushButton(this);
	m_horizontalLayout->addWidget( m_pushButtonSnapPath );
    m_comboBoxWindowNum = new QComboBox( this );
    m_horizontalLayout->addWidget( m_comboBoxWindowNum );
    m_checkBoxMute = new QCheckBox(this);
	m_horizontalLayout->addWidget(m_checkBoxMute);
	m_sliderVolume = new QSlider(this);
    m_horizontalLayout->addWidget(m_sliderVolume);

	m_sliderVolume->setOrientation(Qt::Horizontal);
	m_sliderVolume->setRange( 0, 100 );
	m_sliderVolume->setValue( 50 );
	m_sliderVolume->setSingleStep( 1 );
	m_sliderVolume->setPageStep( 1 );
	m_sliderVolume->setTickInterval( 1 );

    setWindowTitle( tr("CLivingControlWidget") );
    m_labelID->setText( tr("resource ID") );
    m_pushButtonSnap->setText( tr("snap") );
	m_pushButtonSnapPath->setText( tr("snap path") );
	m_checkBoxMute->setText( tr("mute") );
	m_labelStremType->setText( tr("stream type:") );
	m_checkBoxMute->setChecked( true );

	QStringList strList;
	strList.push_back( SUB_STREAM );     
	strList.push_back( MAIN_STREAM );
	comboBoxStreamType->addItems( strList );
	m_pushButtonClose->setText( tr("close") );
	m_pushButtonCloseAll->setText( tr("close all") );

    QStringList strListWinNum;
    strListWinNum.push_back( ONE_WINDOWS );
    strListWinNum.push_back( FOUR_WINDOWS );
    strListWinNum.push_back( NINE_WINDOWS );
    strListWinNum.push_back( SIXTEEN_WINDOWS );
    m_comboBoxWindowNum->addItems( strListWinNum );
    m_comboBoxWindowNum->setCurrentIndex(3);

	connect( m_pushButtonSnap, SIGNAL( clicked() ), SLOT(onButtonSnap() ) );
	connect( m_pushButtonSnapPath, SIGNAL( clicked() ), SLOT( onButtonSnapPath() ) );
	
	connect( m_checkBoxMute, SIGNAL( clicked( bool ) ), SLOT(onCheckBoxMute( bool ) ) );

	m_sliderVolume->setTracking( false );
	connect( m_sliderVolume, SIGNAL( valueChanged(int) ), SLOT(onVolumeChanged( int ) ) );

	connect( m_pushButtonClose, SIGNAL( clicked() ), SLOT(onButtonClose() ) );
	connect( m_pushButtonCloseAll, SIGNAL( clicked() ), SLOT(onButtonCloseAll() ) );

    connect( m_comboBoxWindowNum, SIGNAL( currentIndexChanged ( int ) ), SLOT(onComBoxChanged( int ) ) );

	// stream type lable 
	sizePolicy.setHorizontalPolicy( QSizePolicy::Fixed );
	sizePolicy.setVerticalPolicy( QSizePolicy::Fixed );
	sizePolicy.setHeightForWidth(m_labelStremType->sizePolicy().hasHeightForWidth());
	m_labelStremType->setSizePolicy(sizePolicy);
	m_labelStremType->setMinimumSize(QSize(LABLE_STREAM_TYPE_WIDTH, LINE2HEIGHT));
	m_labelStremType->setMaximumSize(QSize(LABLE_STREAM_TYPE_WIDTH, LINE2HEIGHT));
	//m_labelStremType->setAutoFillBackground( true ); 
	//m_labelStremType->setPalette( QPalette( QColor( 150, 250, 0 ) ) );

	sizePolicy.setHorizontalPolicy( QSizePolicy::Expanding );
	sizePolicy.setVerticalPolicy( QSizePolicy::Fixed );
	sizePolicy.setHeightForWidth(comboBoxStreamType->sizePolicy().hasHeightForWidth());
	comboBoxStreamType->setSizePolicy(sizePolicy);
	comboBoxStreamType->setMinimumSize(QSize(COMBO_STREAM_TYPE_WIDTH, COMBOX_HEIGHT));
	comboBoxStreamType->setMaximumSize(QSize(ITEM_MAX_WIDTH, COMBOX_HEIGHT));

	// close button
	sizePolicy.setHeightForWidth(m_pushButtonClose->sizePolicy().hasHeightForWidth());
	m_pushButtonClose->setSizePolicy(sizePolicy);
	m_pushButtonClose->setMinimumSize(QSize(BUTTON_WIDTH, LINE2HEIGHT));
	m_pushButtonClose->setMaximumSize(QSize(BUTTON_MAX_WIDTH, LINE2HEIGHT));

	// close all button
	sizePolicy.setHeightForWidth( m_pushButtonCloseAll->sizePolicy().hasHeightForWidth() );
	m_pushButtonCloseAll->setSizePolicy(sizePolicy);
	m_pushButtonCloseAll->setMinimumSize(QSize(BUTTON_WIDTH, LINE2HEIGHT));
	m_pushButtonCloseAll->setMaximumSize(QSize(BUTTON_MAX_WIDTH, LINE2HEIGHT));

    // snap button
	sizePolicy.setHeightForWidth(m_pushButtonSnap->sizePolicy().hasHeightForWidth());
	m_pushButtonSnap->setSizePolicy(sizePolicy);
	m_pushButtonSnap->setMinimumSize(QSize(BUTTON_WIDTH, LINE2HEIGHT));
	m_pushButtonSnap->setMaximumSize(QSize(BUTTON_MAX_WIDTH, LINE2HEIGHT));

    // snap path button
	sizePolicy.setHeightForWidth(m_pushButtonSnapPath->sizePolicy().hasHeightForWidth());
	m_pushButtonSnapPath->setSizePolicy(sizePolicy);
	m_pushButtonSnapPath->setMinimumSize(QSize(BUTTON_WIDTH, LINE2HEIGHT));
	m_pushButtonSnapPath->setMaximumSize(QSize(BUTTON_MAX_WIDTH, LINE2HEIGHT));

	// windows number
	sizePolicy.setHeightForWidth(m_comboBoxWindowNum->sizePolicy().hasHeightForWidth());
	m_comboBoxWindowNum->setSizePolicy(sizePolicy);
	m_comboBoxWindowNum->setMinimumSize(QSize(COMBO_WINDOWS_WIDTH, COMBOX_HEIGHT));
	m_comboBoxWindowNum->setMaximumSize(QSize(ITEM_MAX_WIDTH, COMBOX_HEIGHT));

	// checkbox mute
	sizePolicy.setHorizontalPolicy( QSizePolicy::Fixed );
	sizePolicy.setVerticalPolicy( QSizePolicy::Fixed );
	sizePolicy.setHeightForWidth(m_checkBoxMute->sizePolicy().hasHeightForWidth());
	m_checkBoxMute->setSizePolicy(sizePolicy);
	m_checkBoxMute->setMinimumSize(QSize(CHECKBOX_WIDTH, LINE2HEIGHT));
	m_checkBoxMute->setMaximumSize(QSize(CHECKBOX_WIDTH, LINE2HEIGHT));

	// slider 
	sizePolicy.setHorizontalPolicy( QSizePolicy::Expanding );
	sizePolicy.setVerticalPolicy( QSizePolicy::Fixed );
	sizePolicy.setHeightForWidth(m_sliderVolume->sizePolicy().hasHeightForWidth());
	m_sliderVolume->setSizePolicy(sizePolicy);
	m_sliderVolume->setMinimumSize(QSize(SLIDER_WIDTH, SLIDER_HEIGHT));
	m_sliderVolume->setMaximumSize(QSize(ITEM_MAX_WIDTH, SLIDER_HEIGHT));
}
 
CLivingControlWidget::~CLivingControlWidget()
{}

void CLivingControlWidget::updateData()
{
	CMediaPlayerWidget* mediaPlayerWidget = (CMediaPlayerWidget*)m_mulLivingPlayWidget->getActivePlayWidget();
	CLivingPlayerController& playController = mediaPlayerWidget->getMediaPlayerController();

	// 资源ID
	if ( playController.getResInfo().resName.empty() )
	{
		m_labelID->setText( tr("no media resource") );
	}
	else
	{
		m_labelID->setText( QString().fromLocal8Bit( playController.getResInfo().resName.c_str() ) );
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
}

void CLivingControlWidget::onButtonSnap()
{
	CMediaPlayerWidget* mediaPlayerWidget = (CMediaPlayerWidget*)( m_mulLivingPlayWidget->getActivePlayWidget() );
	mediaPlayerWidget->snap( m_snap_path );
}

// 静音
void CLivingControlWidget::onCheckBoxMute( bool state )
{	
	CMediaPlayerWidget* mediaPlayerWidget = (CMediaPlayerWidget*)( m_mulLivingPlayWidget->getActivePlayWidget() );
	CLivingPlayerController& playController = mediaPlayerWidget->getMediaPlayerController();
	
	m_mulLivingPlayWidget->mute( state );
	if ( !state )
	{
		m_sliderVolume->setValue( playController.volume() );
	}
}

void CLivingControlWidget::onButtonClose()
{
	m_mulLivingPlayWidget->stopActiveWidget();
}

void CLivingControlWidget::onButtonCloseAll()
{
	m_mulLivingPlayWidget->stopAll();
}

bool CLivingControlWidget::getStreamType()
{
	return comboBoxStreamType->currentText() == SUB_STREAM;
}

void CLivingControlWidget::onButtonSnapPath()
{
    //选择一个文件夹;
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;

    m_snap_path = QFileDialog::getExistingDirectory( this,
        tr("snap path"),
        QString(),
        options );

	if ( m_snap_path.isEmpty() )
	{
		return ;
	}

    // 判断文件路径合法性
	QDir dir( m_snap_path );
	if ( dir.exists() )
	{
		if ( m_snap_path[m_snap_path.length() - 1] != '\\' )
		{
			m_snap_path += "\\";
		}
	}
	else
	{
		m_snap_path = "";
	}
}

void CLivingControlWidget::onVolumeChanged( int value )
{
	CMediaPlayerWidget* mediaPlayerWidget = (CMediaPlayerWidget*)( m_mulLivingPlayWidget->getActivePlayWidget() );
	CLivingPlayerController& playController = mediaPlayerWidget->getMediaPlayerController();

	m_mulLivingPlayWidget->volume( value );
	m_checkBoxMute->setChecked( false );

	QString tick;
	tick.sprintf( "%d", value );
	m_sliderVolume->setToolTip( tick );
}

void CLivingControlWidget::onComBoxChanged( int index )
{
    m_mulLivingPlayWidget->updateLivingPlayers( index + 1 );
}

void CLivingControlWidget::setPlayerWindowNum( const int &num )
{
    m_comboBoxWindowNum->setCurrentIndex( num - 1 );
}


 


