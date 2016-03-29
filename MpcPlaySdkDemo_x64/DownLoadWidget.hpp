#ifndef DOWNLOADWIDGET_HPP_
#define DOWNLOADWIDGET_HPP_
 
#include <QtGui/QTableWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

#include "MpcPlaySdkItf.hpp"
#include "MpcNetSdkItf.hpp"
#include "DownLoadController.hpp"

#include <vector>

class CDownLoadController;

struct SListItem
{
	CDownLoadController* controller;
	std::string		 resName;
	uint32_t		 timeBegin; 
	uint32_t		 timeEnd;
	std::string      fileName;
	std::string      status;
};

class  CDownLoadWidget: public QWidget
{
	Q_OBJECT

public:
	CDownLoadWidget( QWidget *parent = 0, Qt::WindowFlags flag = 0 );
	~CDownLoadWidget();

	bool downLoad( const SDownLoadInfo& downLoadInfo );
	void release();
	
protected:

	virtual void customEvent( QEvent * event );

protected slots:
	
	void onButtonCancel();
	void onButtonRemove();

private:

	QTableWidget*	 m_table;
	QStringList      m_headers;
	std::vector<SListItem> m_vListItem;

	QPushButton*  m_pushButtonCancel;
	QPushButton*  m_pushButtonRemove;

private:

	void addListItem( const SListItem& item );

	void updateTableView( CDownLoadController* controller, int column, const std::string& str );
	
};
 
#endif    // DOWNLOADWIDGET_HPP_
 
