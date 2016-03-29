#include <QtGui/QSizePolicy>
#include <QtGui/QScrollBar>
#include <QtGui/QResizeEvent>
#include <time.h>
#include <vector>
#include <set>

#include "DownLoadController.hpp"
#include "DownLoadWidget.hpp"

const std::string DOWNLOAD_STATUS_FAIL =  std::string( "download fail." );			    // 下载失败
const std::string DOWNLOAD_STATUS_READY =  std::string( "init connect success." );		// 准备下载
const std::string DOWNLOAD_STATUS_ESTABLISH =  std::string( "download established." );	// 下载链接已建立
const std::string DOWNLOAD_STATUS_DOWNLOADING =  std::string( "downloading." );			// 正在下载
const std::string DOWNLOAD_STATUS_CANCELED =  std::string( "download canceled." );      // 关闭
const std::string DOWNLOAD_STATUS_COMPLETE =  std::string( "download complete." );      // 完成

const int COLUMN_FILENAME = 2;
const int COLUMN_STATUS   = 3;


extern QString stringFromUint32Time( unsigned int nTime );

CDownLoadWidget::CDownLoadWidget( QWidget *parent, Qt::WindowFlags flag )
	: QWidget( parent )
{
	this->resize( 800, 600 ); 
	m_table = new QTableWidget( this );

	// 选中行不可编辑
	m_table->setSelectionBehavior ( QAbstractItemView::SelectRows );
	m_table->setEditTriggers( QAbstractItemView::NoEditTriggers );

	// 屏蔽滚动条菜单
	m_table->verticalScrollBar()->setContextMenuPolicy( Qt::NoContextMenu );
	m_table->horizontalScrollBar()->setContextMenuPolicy( Qt::NoContextMenu );
	
	m_headers << tr( "resource name" )         // 视频资源名
			  << tr( "time segment" )		   // 时间段
			  << tr( "save file path name" )   // 保存文件路径名 
			  << tr( "download status" );      // 下载状态

	m_table->setColumnCount( m_headers.size() );
	m_table->setHorizontalHeaderLabels( m_headers );

	m_table->setColumnWidth( 0, 130 );
	m_table->setColumnWidth( 1, 270 );
	m_table->setColumnWidth( COLUMN_FILENAME, 200 );
	m_table->setColumnWidth( COLUMN_STATUS, 150 );

	QVBoxLayout* verticalLayout = new QVBoxLayout( this );
	
	QHBoxLayout* horizontalLayout = new QHBoxLayout();
	verticalLayout->addLayout( horizontalLayout );
	horizontalLayout->setAlignment( Qt::AlignLeft );
	verticalLayout->setAlignment( horizontalLayout, Qt::AlignLeft ); 
	horizontalLayout->addWidget( m_table );

	QHBoxLayout* horizontalLayout1 = new QHBoxLayout();
	horizontalLayout1->setAlignment( Qt::AlignRight );
	verticalLayout->addLayout( horizontalLayout1 );
	verticalLayout->setAlignment( horizontalLayout1, Qt::AlignRight ); 
	m_pushButtonCancel = new QPushButton( this );
	horizontalLayout1->addWidget( m_pushButtonCancel );
	m_pushButtonRemove = new QPushButton( this );
	horizontalLayout1->addWidget( m_pushButtonRemove );

	m_pushButtonCancel->setText( "cancel" );
	m_pushButtonRemove->setText( "remove" );

	connect( m_pushButtonCancel, SIGNAL( clicked() ), SLOT( onButtonCancel() ) );
	connect( m_pushButtonRemove, SIGNAL( clicked() ), SLOT( onButtonRemove() ) );

	m_vListItem.clear();
}
	 
CDownLoadWidget::~CDownLoadWidget()
{}

void CDownLoadWidget::customEvent( QEvent * event )
{
	CDowloadEvent* e = ( CDowloadEvent* )event;
	if( DWONDLOADE_ESTABLISH_EVENT == event->type() ) 
	{
		updateTableView( e->getData().controller, COLUMN_STATUS, DOWNLOAD_STATUS_ESTABLISH );
	}
	else if ( DWONDLOADE_NEWFILE_EVENT == event->type() )
	{
		updateTableView( e->getData().controller, COLUMN_FILENAME, e->getData().fileName );
		updateTableView( e->getData().controller, COLUMN_STATUS, DOWNLOAD_STATUS_DOWNLOADING );
	}
	else if ( DWONDLOADE_SUCCESS_EVENT == event->type() )
	{
		updateTableView( e->getData().controller, COLUMN_STATUS, DOWNLOAD_STATUS_COMPLETE );
	}
	else if ( DWONDLOADE_FAIL_EVENT == event->type() )
	{
		updateTableView( e->getData().controller, COLUMN_STATUS, DOWNLOAD_STATUS_FAIL );
	}

	event->accept();
}

void CDownLoadWidget::updateTableView( CDownLoadController* controller, int column, const std::string& str )
{
	int i = -1;
	std::vector<SListItem>::iterator it;
	for ( it = m_vListItem.begin(); it != m_vListItem.end(); it++ )
	{
		i++;
		if ( (*it).controller == controller )
		{
			if ( COLUMN_FILENAME == column )  
			{
				it->fileName = str;
			}
			else if ( COLUMN_STATUS == column )
			{
				it->status = str;
			}
			break;
		}
	}

	if ( m_table->item( i, column ) != NULL )
	{
		m_table->item( i, column )->setText( QString().fromLocal8Bit( str.c_str() ) ); 
	}
}

bool CDownLoadWidget::downLoad( const SDownLoadInfo& downLoadInfo )
{
	CDownLoadController* controller = new CDownLoadController();
	if ( !controller )
		return false;

	bool flag = controller->downLoad( downLoadInfo, this );
	
	SListItem item;
	item.controller = controller;
	item.resName = downLoadInfo.resName;
	item.timeBegin = downLoadInfo.timeBegin;
	item.timeEnd = downLoadInfo.timeEnd;
	item.fileName = std::string("");
	if ( flag )
	{
		item.status = DOWNLOAD_STATUS_READY;  // 准备下载
	}
	else
	{	
		item.status = DOWNLOAD_STATUS_FAIL;  // 下载失败
	}

	m_vListItem.push_back( item );
	addListItem( item );

	return true;
}

void CDownLoadWidget::addListItem( const SListItem& item )
{
	int rowCount = m_table->rowCount();
	m_table->setRowCount( rowCount + 1 );

	int j = -1;
	m_table->setItem( rowCount, ++j, new QTableWidgetItem(  QString().fromLocal8Bit( item.resName.c_str() ) ) );
	m_table->setItem( rowCount, ++j, new QTableWidgetItem(  stringFromUint32Time(item.timeBegin) + 
		"---" + stringFromUint32Time(item.timeEnd) ) );
	m_table->setItem( rowCount, ++j, new QTableWidgetItem(  QString().fromLocal8Bit( item.fileName.c_str() ) ) );
	m_table->setItem( rowCount, ++j, new QTableWidgetItem(  QString().fromLocal8Bit( item.status.c_str() ) ) );
}

void CDownLoadWidget::onButtonCancel()
{
	QList<QTableWidgetItem*> items = m_table->selectedItems();

	// 获得选中的行
	int count = items.count();
	std::set<int> rows;
	for( int i = 0; i < count; i++ )
	{
		rows.insert( m_table->row( items.at(i) ) );
	}
	
	std::set<int>::iterator it;
	for ( it = rows.begin(); it != rows.end(); it ++ )
	{
		std::vector<SListItem>::iterator itemIt = m_vListItem.begin() + (*it);

		if ( itemIt->status == DOWNLOAD_STATUS_READY ||
			itemIt->status == DOWNLOAD_STATUS_ESTABLISH ||
			itemIt->status == DOWNLOAD_STATUS_DOWNLOADING )
		{
			itemIt->controller->setCanceled( true );
			itemIt->controller->fini();

			itemIt->status = DOWNLOAD_STATUS_CANCELED;
			m_table->item( (*it), 3 )->setText( QString().fromLocal8Bit( DOWNLOAD_STATUS_CANCELED.c_str() ) );
		}
	}
}

// 删除所有下载
void CDownLoadWidget::onButtonRemove()
{
	QList<QTableWidgetItem*> items = m_table->selectedItems();

	// 获得选中的行
	int count = items.count();
	std::set<int> rows;
	for( int i = 0; i < count; i++ )
	{
		rows.insert( m_table->row( items.at(i) ) );
	}

	// 删除所有下载
	std::set<int>::reverse_iterator it;
	for ( it = rows.rbegin(); it != rows.rend(); it ++ )
	{
		// 停止下载
		std::vector<SListItem>::iterator itemIt = m_vListItem.begin() + (*it);
		if ( itemIt->status == DOWNLOAD_STATUS_READY ||
			itemIt->status == DOWNLOAD_STATUS_ESTABLISH ||
			itemIt->status == DOWNLOAD_STATUS_DOWNLOADING )
		{
			itemIt->controller->setCanceled( true );
			itemIt->controller->fini();
		}

		m_vListItem.erase( itemIt );
		m_table->removeRow( (*it) );
	}

	// 更新显示
	m_table->viewport()->update();
}

void CDownLoadWidget::release()
{
	std::vector<SListItem>::iterator itemIt;
	for ( itemIt = m_vListItem.begin(); itemIt != m_vListItem.end(); itemIt = m_vListItem.begin() )
	{
		if ( itemIt->status == DOWNLOAD_STATUS_READY ||
			 itemIt->status == DOWNLOAD_STATUS_ESTABLISH ||
			 itemIt->status == DOWNLOAD_STATUS_DOWNLOADING )
		{
			itemIt->controller->setCanceled( true );
			itemIt->controller->fini();
		}

		m_vListItem.erase( itemIt );
	}
}






