#include "TimeSegmentWidget.hpp"
#include "MediaPlayerWidget.hpp"
#include "LivingPlayerController.hpp"
#include "ReplayTab.hpp"
#include <time.h>
#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtGui/QMessageBox>
 
CTimeSegmentWidget::CTimeSegmentWidget(QWidget *parent, Qt::WindowFlags flag)
:QWidget(parent, flag)
{
	//  标识标签
    label = new QLabel(this);
    
	// 开始结束标签
	QLabel* labelStart = new QLabel( this );
	labelTo = new QLabel( this );
	QVBoxLayout* labelVerticalLayout = new QVBoxLayout();
	labelVerticalLayout->addWidget( labelStart );
	labelVerticalLayout->addWidget( labelTo );

	// 开始结束时间
	m_dateTimeEditStart = new QDateTimeEdit( QDateTime::currentDateTime().addDays( -30 ), this );
	m_dateTimeEditStart->setDisplayFormat( QString( "yyyy/MM/dd hh:mm:ss" ) );
	m_dateTimeEditStart->setCalendarPopup( true ); 

	m_dateTimeEditEnd = new QDateTimeEdit( QDateTime::currentDateTime(), this );
	m_dateTimeEditEnd->setDisplayFormat( QString( "yyyy/MM/dd hh:mm:ss" ) );
	m_dateTimeEditEnd->setCalendarPopup( true ); 

	QVBoxLayout* timeVerticalLayout = new QVBoxLayout();
	timeVerticalLayout->addWidget( m_dateTimeEditStart );
	timeVerticalLayout->addWidget( m_dateTimeEditEnd );

	// 把垂直的水平放置
    horizontalLayout = new QHBoxLayout();
	horizontalLayout->addLayout( labelVerticalLayout );
	horizontalLayout->addLayout( timeVerticalLayout );

	// 检索按钮
	m_pushButtonRetrieval = new QPushButton(this);

	// 时间列表
	 m_listWidgetTimeSeg = new QListWidget(this);

	// 外围布局
	verticalLayout = new QVBoxLayout(this);  // 外围
	verticalLayout->setSpacing( 4 );
	verticalLayout->setMargin( 2 );
	//verticalLayout->setContentsMargins( 0 ,0, 0, 0 );

	verticalLayout->addWidget( label );      // 第一行
    verticalLayout->addLayout( horizontalLayout );  
    verticalLayout->addWidget( m_pushButtonRetrieval );
    verticalLayout->addWidget( m_listWidgetTimeSeg );

    setWindowTitle( tr("CTimeSegmentWidget"));
    label->setText( tr("time segment") );
	labelStart->setText( tr("start:") );
	labelTo->setText( tr("end:") );
    m_pushButtonRetrieval->setText( tr("retrieval") );

	QSizePolicy sizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding );
	sizePolicy.setHorizontalStretch( 0 );
	sizePolicy.setVerticalStretch( 0 );
	sizePolicy.setHeightForWidth( this->sizePolicy().hasHeightForWidth() );
	setSizePolicy( sizePolicy );
	setMinimumSize( QSize(185, 231) );
	setMaximumSize( QSize(185, 1900) );

	connect( m_pushButtonRetrieval, SIGNAL( clicked() ), SLOT(OnButtonRetrieval() ) );
	connect( m_listWidgetTimeSeg, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ), SLOT( OnitemDoubleClicked( QListWidgetItem* ) )  );
}
 
CTimeSegmentWidget::~CTimeSegmentWidget()
{}

void CTimeSegmentWidget::OnButtonRetrieval()
{
	unsigned int start  = getIntTime( m_dateTimeEditStart );
	unsigned int end  = getIntTime( m_dateTimeEditEnd );

	CMediaPlayerWidget* mediaPlayerWidget = (CMediaPlayerWidget*)( ( (CReplayTab*)parent() )->getActivePlayWidget() );
	CRePlayerController& playController = mediaPlayerWidget->getReplayerController();

	m_timeSegment.clear();
	if ( !playController.retrieveReplayPeriod( start, end, m_timeSegment ) )
	{
		QMessageBox::information( this, 
			QObject::tr( "replay" ), 
			QObject::tr( "retrieve time segment fail!" ),
			QObject::tr( "ok" ) );	
	}

	initListWidget();
}

unsigned int CTimeSegmentWidget::getIntTime( QDateTimeEdit *timeEditEnd )
{
	time_t time;
	struct tm t;
	t.tm_year = timeEditEnd->date().year() - 1900;
	t.tm_mon  = timeEditEnd->date().month() - 1 ;
	t.tm_mday = timeEditEnd->date().day();
	t.tm_hour = timeEditEnd->time().hour();
	t.tm_min  = timeEditEnd->time().minute();
	t.tm_sec  = timeEditEnd->time().second();
	t.tm_isdst = 0;

	time = mktime( &t );
	return time;
}

void CTimeSegmentWidget::initListWidget()
{
	QStringList m_timeSegmentString;
	QVector< QPair<unsigned int, unsigned int> >::iterator it;
	for ( it = m_timeSegment.begin(); it != m_timeSegment.end(); it++ )
	{
		time_t tStart = (time_t)(*it).first;
		time_t tEnd = (time_t)(*it).second;
		struct tm start;
        struct tm end; 
	    localtime_s( &start, &tStart );
		localtime_s( &end, &tEnd );  

		QString str;
		str.sprintf ( "%d-%d-%d,%d:%d:%d\n    ~ %d-%d-%d,%d:%d:%d",
			     start.tm_year + 1900, start.tm_mon + 1, start.tm_mday, start.tm_hour, start.tm_min, start.tm_sec,
				 end.tm_year + 1900, end.tm_mon + 1, end.tm_mday, end.tm_hour, end.tm_min, end.tm_sec );

		m_timeSegmentString.push_back( str );
	}

	m_listWidgetTimeSeg->clear();
	m_listWidgetTimeSeg->addItems( m_timeSegmentString );
}

void CTimeSegmentWidget::OnitemDoubleClicked(QListWidgetItem* item)
{
	return ;
}

bool CTimeSegmentWidget::getTimeSegment( QPair<unsigned int, unsigned int>& timeSeg )
{
	if ( m_listWidgetTimeSeg->count() <= 0 )
	{
		return false;
	}

	int index = m_listWidgetTimeSeg->currentRow();
	if ( m_timeSegment.size() < (index + 1) || index < 0 )
	{
		return false;
	}

	timeSeg = m_timeSegment[index];
	return true;
}

bool CTimeSegmentWidget::updateList( const QVector< QPair<unsigned int, unsigned int> >& timeSegment )
{
	m_timeSegment.clear();
	m_timeSegment << timeSegment;
	initListWidget();
	return true;
}




 


