#include "VidEffCtrWidget.hpp"
#include "MulLivingPlayerTab.hpp"
#include "MediaPlayerWidget.hpp"
 
CVidEffCtrWidget::CVidEffCtrWidget( QWidget *parent, Qt::WindowFlags flag )
: QWidget( parent, flag )
, m_pMulLivingTab( (CMulLivingPlayerTab*)parent )
{

#if 0
	this->setAutoFillBackground(true);
	QPalette myPalette( QColor( 200, 200, 200 ) );
	myPalette.setBrush(this->backgroundRole(), QBrush(QPixmap("./pic.jpg")));
	this->setPalette( myPalette );
#endif

	// 窗口尺寸
	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
	setSizePolicy(sizePolicy);
	setMinimumSize(QSize(185, 150));
	setMaximumSize(QSize(185, 200));
	
	verticalLayout_2 = new QVBoxLayout(this);
    groupBox = new QGroupBox(this);
	
    verticalLayout = new QVBoxLayout(groupBox);
    m_labelHue = new QLabel(groupBox);
    
    sizePolicy = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(m_labelHue->sizePolicy().hasHeightForWidth());
    m_labelHue->setSizePolicy(sizePolicy);
    verticalLayout->addWidget(m_labelHue);

    m_horizontalSliderHue = new QSlider(groupBox);
    m_horizontalSliderHue->setOrientation(Qt::Horizontal);
    verticalLayout->addWidget(m_horizontalSliderHue);

    m_labelBright = new QLabel(groupBox);
    sizePolicy.setHeightForWidth(m_labelBright->sizePolicy().hasHeightForWidth());
    m_labelBright->setSizePolicy(sizePolicy);
    verticalLayout->addWidget(m_labelBright);

    m_horizontalSliderBright = new QSlider(groupBox);
    m_horizontalSliderBright->setOrientation(Qt::Horizontal);
    verticalLayout->addWidget(m_horizontalSliderBright);

    m_labelSaturation = new QLabel(groupBox);
    sizePolicy.setHeightForWidth(m_labelSaturation->sizePolicy().hasHeightForWidth());
    m_labelSaturation->setSizePolicy(sizePolicy);
    verticalLayout->addWidget(m_labelSaturation);

    m_horizontalSliderSat = new QSlider(groupBox);
    m_horizontalSliderSat->setOrientation(Qt::Horizontal);
    verticalLayout->addWidget(m_horizontalSliderSat);

    m_labelContrast = new QLabel(groupBox);
    sizePolicy.setHeightForWidth(m_labelContrast->sizePolicy().hasHeightForWidth());
    m_labelContrast->setSizePolicy(sizePolicy);
    verticalLayout->addWidget(m_labelContrast);

    m_horizontalSliderContrast = new QSlider(groupBox);
    m_horizontalSliderContrast->setOrientation(Qt::Horizontal);
    verticalLayout->addWidget(m_horizontalSliderContrast);
	
	verticalLayout_2->addWidget(groupBox);

    this->setWindowTitle("CVidEffCtrWidget");
    m_labelHue->setText(tr("hue[0,255]:"));
    m_labelBright->setText(tr("bright[0,255]:"));
    m_labelSaturation->setText(tr("saturation[0,255]:"));
    m_labelContrast->setText(tr("contrast[0,255]:"));
	groupBox->setTitle(tr("image quality"));

	verticalLayout_2->setSpacing(0);
	verticalLayout_2->setMargin(0);
	verticalLayout_2->setContentsMargins(0,0,0,0);

	m_horizontalSliderHue->setRange( 0, 255 );
	m_horizontalSliderHue->setValue( 0 );
	m_horizontalSliderHue->setSingleStep( 25 );
	m_horizontalSliderHue->setPageStep( 25 );
	m_horizontalSliderHue->setTickInterval( 25 );					
	m_horizontalSliderHue->setTickPosition( QSlider::TicksAbove );

	m_horizontalSliderBright->setRange( 0, 255 );
	m_horizontalSliderBright->setValue( 0 );
	m_horizontalSliderBright->setSingleStep( 25 );
	m_horizontalSliderBright->setPageStep( 25 );
	m_horizontalSliderBright->setTickInterval( 25 );					
	m_horizontalSliderBright->setTickPosition( QSlider::TicksAbove );

	m_horizontalSliderSat->setRange( 0, 255 );
	m_horizontalSliderSat->setValue( 0 );
	m_horizontalSliderSat->setSingleStep( 25 );
	m_horizontalSliderSat->setPageStep( 25 );
	m_horizontalSliderSat->setTickInterval( 25 );					
	m_horizontalSliderSat->setTickPosition( QSlider::TicksAbove );

	m_horizontalSliderContrast->setRange( 0, 255 );
	m_horizontalSliderContrast->setValue( 0 );
	m_horizontalSliderContrast->setSingleStep( 25 );
	m_horizontalSliderContrast->setPageStep( 25 );
	m_horizontalSliderContrast->setTickInterval( 25 );					
	m_horizontalSliderContrast->setTickPosition( QSlider::TicksAbove );

	// 支持拖动、鼠标滚动、点击等操作
	m_horizontalSliderHue->setTracking( false );
	m_horizontalSliderBright->setTracking( false );
	m_horizontalSliderSat->setTracking( false );
	m_horizontalSliderContrast->setTracking( false );

	connect( m_horizontalSliderHue, SIGNAL( valueChanged(int) ), this, SLOT( valueChangedSlot(int) ) );  
	connect( m_horizontalSliderBright, SIGNAL( valueChanged(int) ), this, SLOT( valueChangedSlot(int) ) ); 
	connect( m_horizontalSliderSat, SIGNAL( valueChanged(int) ), this, SLOT( valueChangedSlot(int) ) ); 
	connect( m_horizontalSliderContrast, SIGNAL( valueChanged(int) ), this, SLOT( valueChangedSlot(int) ) ); 
}
 
CVidEffCtrWidget::~CVidEffCtrWidget()
{
 
}

void CVidEffCtrWidget::updateData()
{
	CMediaPlayerWidget* playWidget = ( CMediaPlayerWidget* )m_pMulLivingTab->getActivePlayWidget();
	CVideoEffectControler& imgQltCtrl = playWidget->getVidEffController();

	if ( !imgQltCtrl.canControl() )
	{
		m_horizontalSliderHue->setValue( 0 );
		m_horizontalSliderBright->setValue( 0 );
		m_horizontalSliderSat->setValue( 0 );
		m_horizontalSliderContrast->setValue( 0 );

		this->setDisabled( true );
		return;
	}

	this->setDisabled( false );
	
	uint8_t val = 0;
	if ( imgQltCtrl.getMediaHue( val ) )
	{
		m_horizontalSliderHue->setValue( val );
	}
	if ( imgQltCtrl.getMediaBrightness( val ) )
	{
		m_horizontalSliderBright->setValue( val );
	}
	if ( imgQltCtrl.getMediaSaturation( val ) )
	{
		m_horizontalSliderSat->setValue( val );
	}
	if ( imgQltCtrl.getMediaContrast( val ) )
	{
		m_horizontalSliderContrast->setValue( val );
	}
}

void CVidEffCtrWidget::valueChangedSlot( int val )
{
	QSlider *slider = qobject_cast<QSlider *>( sender() ); 
	if ( NULL == slider )
	{
		return ;
	}

	QString tick;
	tick.sprintf( "%d", val );
	slider->setToolTip( tick );

	CMediaPlayerWidget* playWidget = ( CMediaPlayerWidget* )m_pMulLivingTab->getActivePlayWidget();
	CVideoEffectControler& imgQltCtrl = playWidget->getVidEffController();

	if ( slider == m_horizontalSliderHue )
	{
		imgQltCtrl.setMediaHue( val );
	}
	else if ( slider == m_horizontalSliderBright )
	{
		imgQltCtrl.setMediaBrightness( val );   
	} 
	else if ( slider == m_horizontalSliderContrast )
	{	
		imgQltCtrl.setMediaContrast( val );
	} 
	else if ( slider == m_horizontalSliderSat )
	{
		imgQltCtrl.setMediaSaturation( val );
	}

}
