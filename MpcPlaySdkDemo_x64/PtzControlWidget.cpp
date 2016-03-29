#include "PtzControlWidget.hpp"
#include "MediaPlayerWidget.hpp"
#include "PtzController.hpp"
#include "MulLivingPlayerTab.hpp"
 
// 移动
const QString SPEED_SLOWER = "SPEED_SLOWER";
const QString SPEED_SLOW = "SPEED_SLOW";
const QString SPEED_NORMAL = "SPEED_NORMAL";
const QString SPEED_FAST = "SPEED_FAST";
const QString SPEED_FASTER = "SPEED_FASTER";

// focus
const QString FOCUS_STOP = "FOCUS_STOP";
const QString FOCUS_NEAR = "FOCUS_NEAR";
const QString FOCUS_FAR = "FOCUS_FAR";

// zoom
const QString ZOOM_STOP = "ZOOM_STOP";
const QString ZOOM_IN = "ZOOM_IN";
const QString ZOOM_OUT = "ZOOM_OUT";


// iris
const QString IRIS_OPEN = "IRIS_OPEN";
const QString IRIS_CLOSE = "IRIS_CLOSE"; 

// 
const QString SWITCH_ON = "ON";
const QString SWITCH_OFF = "OFF";

CPtzControlWidget::CPtzControlWidget(QWidget *parent, Qt::WindowFlags flag)
: QWidget(parent, flag)
, m_mulLivingWidget( (CMulLivingPlayerTab*) parent )
{
    verticalLayout_2 = new QVBoxLayout(this);
    
    m_ptzGroup = new QGroupBox(this);
    verticalLayout = new QVBoxLayout(m_ptzGroup);
    m_labelSpeed = new QLabel(m_ptzGroup);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(m_labelSpeed->sizePolicy().hasHeightForWidth());
    m_labelSpeed->setSizePolicy(sizePolicy);
    m_labelSpeed->setMinimumSize(QSize(0, 0));
    m_labelSpeed->setMaximumSize(QSize(16777215, 24));
    verticalLayout->addWidget(m_labelSpeed);
    comboBoxSpeed = new QComboBox(m_ptzGroup);
    verticalLayout->addWidget(comboBoxSpeed);
    m_pushButtonUp = new QPushButton(m_ptzGroup);
    verticalLayout->addWidget(m_pushButtonUp);
    m_pushButtonRight = new QPushButton(m_ptzGroup);
    verticalLayout->addWidget(m_pushButtonRight);
    m_pushButtonStop = new QPushButton(m_ptzGroup);
    verticalLayout->addWidget(m_pushButtonStop);
    m_pushButtonLeft = new QPushButton(m_ptzGroup);
    verticalLayout->addWidget(m_pushButtonLeft);
    m_pushButtonDown = new QPushButton(m_ptzGroup);
    verticalLayout->addWidget(m_pushButtonDown);

    
    horizontalLayout = new QHBoxLayout();
    m_labelFocus = new QLabel(m_ptzGroup);
    sizePolicy = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(m_labelFocus->sizePolicy().hasHeightForWidth());
    m_labelFocus->setSizePolicy(sizePolicy);
    m_labelFocus->setMinimumSize(QSize(54, 0));
    m_labelFocus->setMaximumSize(QSize(54, 16777215));
    horizontalLayout->addWidget(m_labelFocus);
    comboBoxFocus = new QComboBox(m_ptzGroup);
    horizontalLayout->addWidget(comboBoxFocus);
    verticalLayout->addLayout(horizontalLayout);

    horizontalLayout_2 = new QHBoxLayout();
    m_labelZoom = new QLabel(m_ptzGroup);
    sizePolicy.setHeightForWidth(m_labelZoom->sizePolicy().hasHeightForWidth());
    m_labelZoom->setSizePolicy(sizePolicy);
    m_labelZoom->setMinimumSize(QSize(54, 0));
    m_labelZoom->setMaximumSize(QSize(54, 16777215));
    horizontalLayout_2->addWidget(m_labelZoom);
    comboBoxZoom = new QComboBox(m_ptzGroup);
    horizontalLayout_2->addWidget(comboBoxZoom);
    verticalLayout->addLayout(horizontalLayout_2);

    horizontalLayout_3 = new QHBoxLayout();
    m_labelIris = new QLabel(m_ptzGroup);
    sizePolicy.setHeightForWidth(m_labelIris->sizePolicy().hasHeightForWidth());
    m_labelIris->setSizePolicy(sizePolicy);
    m_labelIris->setMinimumSize(QSize(54, 0));
    m_labelIris->setMaximumSize(QSize(54, 16777215));
    horizontalLayout_3->addWidget(m_labelIris);
    comboBoxIris = new QComboBox(m_ptzGroup);
    horizontalLayout_3->addWidget(comboBoxIris);
    verticalLayout->addLayout(horizontalLayout_3);

    horizontalLayout_4 = new QHBoxLayout();
    m_labelWiper = new QLabel(m_ptzGroup);
    sizePolicy.setHeightForWidth(m_labelWiper->sizePolicy().hasHeightForWidth());
    m_labelWiper->setSizePolicy(sizePolicy);
    m_labelWiper->setMinimumSize(QSize(54, 0));
    m_labelWiper->setMaximumSize(QSize(54, 16777215));
    horizontalLayout_4->addWidget(m_labelWiper);
    comboBoxWiper = new QComboBox(m_ptzGroup);
    horizontalLayout_4->addWidget(comboBoxWiper);
    verticalLayout->addLayout(horizontalLayout_4);

    horizontalLayout_heat = new QHBoxLayout();
    m_labelHeat = new QLabel(m_ptzGroup);
    sizePolicy.setHeightForWidth(m_labelHeat->sizePolicy().hasHeightForWidth());
    m_labelHeat->setSizePolicy(sizePolicy);
    m_labelHeat->setMinimumSize(QSize(54, 0));
    m_labelHeat->setMaximumSize(QSize(54, 16777215));
    horizontalLayout_heat->addWidget(m_labelHeat);
    comboBoxHeat = new QComboBox(m_ptzGroup);
    horizontalLayout_heat->addWidget(comboBoxHeat);
    verticalLayout->addLayout(horizontalLayout_heat);

    horizontalLayout_fan = new QHBoxLayout();
    m_labelFan = new QLabel(m_ptzGroup);
    sizePolicy.setHeightForWidth(m_labelFan->sizePolicy().hasHeightForWidth());
    m_labelFan->setSizePolicy(sizePolicy);
    m_labelFan->setMinimumSize(QSize(54, 0));
    m_labelFan->setMaximumSize(QSize(54, 16777215));
    horizontalLayout_fan->addWidget(m_labelFan);
    comboBoxFan = new QComboBox(m_ptzGroup);
    horizontalLayout_fan->addWidget(comboBoxFan);
    verticalLayout->addLayout(horizontalLayout_fan);

    horizontalLayout_5 = new QHBoxLayout();
    m_labelLight = new QLabel(m_ptzGroup);
    sizePolicy.setHeightForWidth(m_labelLight->sizePolicy().hasHeightForWidth());
    m_labelLight->setSizePolicy(sizePolicy);
    m_labelLight->setMinimumSize(QSize(54, 0));
    m_labelLight->setMaximumSize(QSize(54, 16777215));
    horizontalLayout_5->addWidget(m_labelLight);
    comboBoxLight = new QComboBox(m_ptzGroup);
    horizontalLayout_5->addWidget(comboBoxLight);
    verticalLayout->addLayout(horizontalLayout_5);

    horizontalLayout_6 = new QHBoxLayout();
    m_labelPreset = new QLabel(m_ptzGroup);
    sizePolicy.setHeightForWidth(m_labelPreset->sizePolicy().hasHeightForWidth());
    m_labelPreset->setSizePolicy(sizePolicy);
    m_labelPreset->setMinimumSize(QSize(54, 0));
    m_labelPreset->setMaximumSize(QSize(54, 16777215));
    horizontalLayout_6->addWidget(m_labelPreset);
    comboBoxPreset = new QComboBox(m_ptzGroup);
    horizontalLayout_6->addWidget(comboBoxPreset);
	m_delPreset = new QPushButton(m_ptzGroup);
	horizontalLayout_6->addWidget(m_delPreset);
    verticalLayout->addLayout(horizontalLayout_6);

	horizontalLayout_7 = new QHBoxLayout();
	m_labelName = new QLabel(m_ptzGroup);
	sizePolicy.setHeightForWidth(m_labelPreset->sizePolicy().hasHeightForWidth());
	m_labelName->setSizePolicy(sizePolicy);
	m_labelName->setMinimumSize(QSize(54, 0));
	m_labelName->setMaximumSize(QSize(54, 16777215));
	horizontalLayout_7->addWidget(m_labelName);
	m_editName = new QLineEdit(m_ptzGroup);
	horizontalLayout_7->addWidget(m_editName);
	m_addPreset = new QPushButton(m_ptzGroup);
	horizontalLayout_7->addWidget(m_addPreset);
	verticalLayout->addLayout(horizontalLayout_7);

    verticalLayout_2->addWidget(m_ptzGroup);
	verticalLayout_2->setSpacing(0);
	verticalLayout_2->setMargin(0);
	verticalLayout_2->setContentsMargins(0,0,0,0);

	// 窗口尺寸
	sizePolicy =  QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
	setSizePolicy(sizePolicy);
	setMinimumSize(QSize(185, 380));
	setMaximumSize(QSize(185, 1900));

    this->setWindowTitle( tr("CPtzControlWidget") );
    m_labelSpeed->setText( tr("Speed:") );
    m_pushButtonUp->setText( tr("up") );
    m_pushButtonRight->setText( tr("right") );
    m_pushButtonStop->setText( tr("stop") );
    m_pushButtonLeft->setText( tr("left") );
    m_pushButtonDown->setText( tr("down") );
    m_labelFocus->setText( tr( "Focus:") );
    m_labelZoom->setText( tr( "Zoom:") );
    m_labelIris->setText( tr("Iris:") );
    m_labelHeat->setText( tr("Heat:") );
    m_labelFan->setText( tr("Fan:") );
    m_labelWiper->setText( tr("Wiper:") );
    m_labelLight->setText( tr("Light:") );
    m_labelPreset->setText( tr("Preset:") );
	m_ptzGroup->setTitle( tr("ptz") );
	m_delPreset->setText( tr("del") );
	m_labelName->setText( tr("preName:"));
	m_addPreset->setText( tr("add"));

	QStringList listString;
	listString.push_back( SPEED_SLOWER );
	listString.push_back( SPEED_SLOW );
	listString.push_back( SPEED_NORMAL );
	listString.push_back( SPEED_FAST );
	listString.push_back( SPEED_FASTER );
	comboBoxSpeed->addItems( listString );

	listString.clear();
	listString.push_back( FOCUS_STOP );
	listString.push_back( FOCUS_NEAR );
	listString.push_back( FOCUS_FAR );
	comboBoxFocus->addItems( listString );

	listString.clear();
	listString.push_back( ZOOM_STOP );
	listString.push_back( ZOOM_IN );
	listString.push_back( ZOOM_OUT );
	comboBoxZoom->addItems( listString );

	listString.clear();
    listString.push_back( IRIS_OPEN  );
    listString.push_back( IRIS_CLOSE );
	comboBoxIris->addItems( listString );

	listString.clear();
	listString.push_back( SWITCH_ON );
	listString.push_back( SWITCH_OFF );
    comboBoxHeat->addItems( listString );
    comboBoxFan->addItems( listString );
    comboBoxWiper->addItems( listString );
	comboBoxLight->addItems( listString );

	connect( m_pushButtonUp, SIGNAL( clicked() ), SLOT(onButtonClick() ) );
	connect( m_pushButtonRight, SIGNAL( clicked() ), SLOT(onButtonClick() ) );
	connect( m_pushButtonStop, SIGNAL( clicked() ), SLOT(onButtonClick() ) );
	connect( m_pushButtonLeft, SIGNAL( clicked() ), SLOT(onButtonClick() ) );
	connect( m_pushButtonDown, SIGNAL( clicked() ), SLOT(onButtonClick() ) );
	connect( m_delPreset, SIGNAL( clicked() ), SLOT(onButtonClick() ) );
	connect( m_addPreset, SIGNAL( clicked() ), SLOT(onButtonClick() ) );

	connect( comboBoxFocus, SIGNAL( activated( int ) ), 
		SLOT(onComboBoxActived() ) );
	connect( comboBoxZoom, SIGNAL( activated( int ) ), 
		SLOT(onComboBoxActived() ) );
	connect( comboBoxIris, SIGNAL( activated( int ) ), 
		SLOT(onComboBoxActived() ) );
    connect( comboBoxHeat, SIGNAL( activated( int ) ), 
        SLOT(onComboBoxActived() ) );
    connect( comboBoxFan, SIGNAL( activated( int ) ), 
        SLOT(onComboBoxActived() ) );
	connect( comboBoxWiper, SIGNAL( activated( int ) ), 
		SLOT(onComboBoxActived() ) );
	connect( comboBoxLight, SIGNAL( activated( int ) ), 
		SLOT(onComboBoxActived() ) );
	connect( comboBoxPreset, SIGNAL( activated( int ) ), 
		SLOT(onComboBoxActived() ) );
}
 
CPtzControlWidget::~CPtzControlWidget()
{

}

void CPtzControlWidget::updateData()
{
	CMediaPlayerWidget* mediaPlayerWidget = (CMediaPlayerWidget*)( m_mulLivingWidget->getActivePlayWidget() );
	CPtzController &ptzController = mediaPlayerWidget->getPtzController();

	if ( !ptzController.canControl() )
	{
		this->setDisabled( true );
		return ;
	}
	this->setDisabled( false );

	// 初始化预置位
	int size = 0;
	std::string* strPreset;
	ptzController.getPresetName( &strPreset, size );
	QStringList listString;
	for ( int i = 0; i < size; i++ )
	{
        listString.push_back( QString().fromLocal8Bit(strPreset[i].c_str()) );
	}

	comboBoxPreset->clear();
	if ( size )
	{
		comboBoxPreset->addItems( listString );
		delete[] strPreset;
		strPreset = 0;
	}
}


void CPtzControlWidget::onButtonClick()
{
	QObject* lSender = sender();
	QPushButton *button = qobject_cast<QPushButton *>(sender()); 
	if ( !button )
	{
		return ;
	}
	
    CMediaPlayerWidget* mediaPlayerWidget = (CMediaPlayerWidget*)( ( (CMulLivingPlayerTab*)parent() )->getActivePlayWidget() );
    CPtzController& ptzController = mediaPlayerWidget->getPtzController();
	
	if ( button == m_pushButtonUp )
	{
		ptzController.move(mpc::nsdk::IPtzController::MOVE_UP, getSpeed());
	}
	else if( button == m_pushButtonDown )
	{
		ptzController.move(mpc::nsdk::IPtzController::MOVE_DOWN, getSpeed());
	}
	else if( button == m_pushButtonLeft )
	{
		ptzController.move(mpc::nsdk::IPtzController::MOVE_LEFT, getSpeed());
	}
	else if( button == m_pushButtonRight )
	{
		ptzController.move(mpc::nsdk::IPtzController::MOVE_RIGHT, getSpeed());
	}
	else if( button == m_pushButtonStop )
	{
		ptzController.move(mpc::nsdk::IPtzController::MOVE_STOP, getSpeed());
	}
	else if( button == m_delPreset )
	{
		ptzController.delPre( comboBoxPreset->currentText().toLocal8Bit().constData() );
		updateData();
	}
	else if( button == m_addPreset )
	{
		ptzController.addPre( m_editName->text().toLocal8Bit().constData() );
		updateData();	
	}
}

void CPtzControlWidget::onComboBoxActived()
{
	QComboBox *comboBox = qobject_cast<QComboBox *>(sender());  

	CMediaPlayerWidget* mediaPlayerWidget = (CMediaPlayerWidget*)( ( (CMulLivingPlayerTab*)parent() )->getActivePlayWidget() );
	CPtzController& ptzController = mediaPlayerWidget->getPtzController();

	if ( comboBox == comboBoxFocus )
	{
		ptzController.focus( getFocus() );
	}
	else if( comboBox == comboBoxZoom )
	{
		ptzController.zoom( getZoom() );
	}
	else if( comboBox == comboBoxIris )
	{
		ptzController.iris( getIris() );
	}
	else if( comboBox == comboBoxWiper )
	{
		ptzController.wiper( getSwitchStatus( comboBoxWiper ) );
	}
	else if( comboBox == comboBoxLight )
	{
		ptzController.light( getSwitchStatus( comboBoxLight ) );
	}
	else if( comboBox == comboBoxPreset )
	{
		ptzController.preset( comboBoxPreset->currentText().toLocal8Bit().constData() );
	}
    else if ( comboBox == comboBoxHeat )
    {
        ptzController.heat( getSwitchStatus(comboBoxHeat) );
    }
    else if( comboBox == comboBoxFan )
    {
        ptzController.fan( getSwitchStatus(comboBoxFan) );
    }
}

mpc::nsdk::IPtzController::EPtzSpeed CPtzControlWidget::getSpeed()
{
	mpc::nsdk::IPtzController::EPtzSpeed speed;
	QString strSpeed = comboBoxSpeed->currentText();
	if ( strSpeed == SPEED_SLOWER )
	{
		speed = mpc::nsdk::IPtzController::SPEED_SLOWER;
	}
	else if( strSpeed == SPEED_SLOW )
	{
		speed = mpc::nsdk::IPtzController::SPEED_SLOW;
	}
	else if( strSpeed == SPEED_FAST )
	{
		speed = mpc::nsdk::IPtzController::SPEED_FAST;
	}
	else if( strSpeed == SPEED_FASTER )
	{
		speed = mpc::nsdk::IPtzController::SPEED_FASTER;
	}
	else
	{
		speed = mpc::nsdk::IPtzController::SPEED_NORMAL;
	}

	return speed;
}
 
mpc::nsdk::IPtzController::EPtzFocus CPtzControlWidget::getFocus()
{
	mpc::nsdk::IPtzController::EPtzFocus focus;
	QString strFocus = comboBoxFocus->currentText();
	if ( strFocus == FOCUS_STOP )
	{
		focus = mpc::nsdk::IPtzController::FOCUS_STOP;
	}
	else if( strFocus == FOCUS_NEAR )
	{
		focus = mpc::nsdk::IPtzController::FOCUS_NEAR;
	}
	else // strFocus == FOCUS_FAR 
	{
		focus = mpc::nsdk::IPtzController::FOCUS_FAR;
	}

	return focus;
}

mpc::nsdk::IPtzController::EPtzZoom  CPtzControlWidget::getZoom()
{
	mpc::nsdk::IPtzController::EPtzZoom zoom;
	QString strZoom = comboBoxZoom->currentText();
	if ( strZoom == ZOOM_STOP )
	{
		zoom = mpc::nsdk::IPtzController::ZOOM_STOP;
	}
	else if( strZoom == ZOOM_IN )
	{
		zoom = mpc::nsdk::IPtzController::ZOOM_IN;
	}
	else // strZoom == ZOOM_OUT 
	{
		zoom = mpc::nsdk::IPtzController::ZOOM_OUT;
	}

	return zoom;
}


mpc::nsdk::IPtzController::EPtzIris CPtzControlWidget::getIris()
{
	mpc::nsdk::IPtzController::EPtzIris iris;
	QString strIris = comboBoxIris->currentText();
	if ( strIris == IRIS_OPEN )
	{
        iris = mpc::nsdk::IPtzController::IRIS_OPEN;
	}
	else // ( strIris == IRIS_CLOSE )
	{
		iris = mpc::nsdk::IPtzController::IRIS_CLOSE;
	}

	return iris;
}

bool CPtzControlWidget::getSwitchStatus( QComboBox *comboBox )
{
	return ( comboBox->currentText() == SWITCH_ON );
}
