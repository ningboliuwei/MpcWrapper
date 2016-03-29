#include <boost/lexical_cast.hpp>

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPixmap>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtCore/QRegExp>
#include <QtGui/QRegExpValidator>

#include "ConfigScene.hpp"
#include "MonitorIndexScheduleWnd.hpp"


extern bool isSupportTestSdk;

CMonitorIndexScheduleWnd::CMonitorIndexScheduleWnd(CResourceTreeWidget *tree, mpc::nsdk::IManageSession *ms, const SResourceInfo &info, CConfigScene *scene)
: QDialog((QWidget*)(tree->parent()))
, m_resourceTreeWidget(tree)
, m_managerSession( ms )
, m_resInfo( info )
, m_scene( scene )
{
	setWindowFlags(Qt::Popup | Qt::Dialog);
	setWindowTitle(tr("MonitorScheduleInfo"));
	setFixedSize(295,180);

	m_indexLabel         = new QLabel(tr("Index:"));
	QLabel *portLabel   = new QLabel(tr(":"));
	portLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

	m_indexEditor = new QLineEdit();
 	if(!isSupportTestSdk)
 	{
        QRegExp indexregExp("(^6[0-4]\\d{3}$)|(^65[0-4]\\d{2}$)|(^655[0-2]\\d$)|(^6553[0-5]$)|(^[1-9]\\d{0,3}$)|(^[1-5]\\d{4}$)");
 		m_indexEditor->setValidator(new QRegExpValidator(indexregExp, m_indexEditor));
 	}

	m_indexEditor->setContextMenuPolicy(Qt::NoContextMenu);

	m_okButton = new QPushButton(tr("OK"));
	m_cancelButon = new QPushButton(tr("Cancel"));

	m_mainLayout= new QVBoxLayout(this);

	QGridLayout *topLayout = new QGridLayout();
	topLayout->addWidget(m_indexLabel,0,0);
	topLayout->addWidget(m_indexEditor,0,1,1,3);

	topLayout->setColumnStretch(0,2);
	topLayout->setColumnStretch(1,6);
	topLayout->setColumnStretch(2,1);
	topLayout->setColumnStretch(3,4);
	topLayout->setColumnStretch(4,1);

	topLayout->setRowStretch(0,1);
	topLayout->setRowStretch(1,1);
	topLayout->setRowStretch(2,1);
	topLayout->setRowStretch(3,1);
	topLayout->setRowStretch(4,1);

	QHBoxLayout *buttonlayout = new QHBoxLayout();
	buttonlayout->addStretch();
	buttonlayout->addWidget(m_okButton);
	buttonlayout->addWidget(m_cancelButon);

	m_mainLayout->addLayout(topLayout);
	m_mainLayout->addLayout(buttonlayout);

	if (!m_indexEditor->text().isEmpty())
	{
		m_indexEditor->setFocus();
	}

	connect(m_okButton,SIGNAL(clicked()),SLOT(onOkButtonClicked()));
	connect(m_cancelButon,SIGNAL(clicked()),SLOT(onCancelButtonClicked()));
}

CMonitorIndexScheduleWnd::~CMonitorIndexScheduleWnd()
{
	delete m_mainLayout;
	m_mainLayout = NULL;
}

void CMonitorIndexScheduleWnd::closeEvent(QCloseEvent *event)
{
	reject();
}

void CMonitorIndexScheduleWnd::onOkButtonClicked()
{
	std::string monitorIndex = m_indexEditor->text().toLocal8Bit();

	if( monitorIndex.empty() ) 
	{
		std::cerr << "schedule for monitor index ---- the index empty" << std::endl;
		return;
	}

	if( m_managerSession == 0 )
	{
		std::cerr << "schedule for monitor index --- mpc::nsdk::IManageSession is null" << std::endl;
		return;
	}

    try
    {
        int index = boost::lexical_cast< int >( monitorIndex );
        m_scene->scheduleFromMonitorIndex( m_managerSession, index, m_resInfo );
    }
    catch ( ... )
    {
        std::cerr << "schedule for monitor index --- index is not valid! " << monitorIndex << std::endl;
    }

// 	mpc::nsdk::IWallController *wc = m_managerSession->createWallController();
// 
// 	if( wc == 0 ) 
// 	{
// 		std::cerr << "schedule for monitor index --- mpc::nsdk::IWallController is null" << std::endl;
// 		return;
// 	}
// 
// 	std::string mediaId;
// 	resIdStr( m_resInfo.resId, mediaId );
// 	if( !wc->schedule( index, m_resInfo.resId ) )
// 	{
// 		std::cerr << "schedule for monitor index false" << " monitor index : " << index << " media id : " << mediaId << std::endl;
// 	}

//	reject();
}

void CMonitorIndexScheduleWnd::onCancelButtonClicked()
{
	reject();
}
