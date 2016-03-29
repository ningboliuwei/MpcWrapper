#ifndef ALARMTAB_HPP_
#define ALARMTAB_HPP_
 
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

#include "../include/Common.hpp"
#include "AlarmController.hpp"

class CMainWindow;
 
class CAlarmTab : public QWidget
{
    Q_OBJECT
public:

    CAlarmTab( QWidget *parent = 0, Qt::WindowFlags flag = 0);
    ~CAlarmTab();

	bool init( mpc::nsdk::IManageSession* pSession, QString& sessionLabel );
	bool fini( mpc::nsdk::IManageSession* pSession = NULL );

	void addRegisterBySession( mpc::nsdk::IManageSession* pSession );
	void delRegisterBySession( mpc::nsdk::IManageSession* pSession );

	void clearCurrentTable();

private: 

	void initTableWiget( QTableWidget* tableView, const QStringList& header, const int row );
	void setItemCheckable( QTableWidget* tableView, int column );
	void setAlarmResWidget( QVector<SAlarmResInfo>& vecAlarmInfo );
	void setAlarmLogWidget( SAlarmNotifyData& notifyData );

    // ams掉线去掉check的勾选
    void clearSelectOnAmsOff( mpc::nsdk::IAlarmController *alarmController );

protected slots:
   
	void alarmResItemChangedSlot( int row, int column );
    void onConfirmButtonClicked();
    void onCleanButtonClicked();

protected:

	virtual void customEvent( QEvent * event );
 
private:

	struct SOptcheckBoxState
	{
		bool active;
		bool confirm;
		bool clear;
		SOptcheckBoxState():active(false),confirm(false),clear(false){}
	};

    QHBoxLayout   *horizontalLayout;
    QTableWidget  *m_tableViewAlarmRes;
    QTableWidget  *m_tableViewAlarmLog;
	CMainWindow   *m_mainWindow;

	CAlarmController m_alarmController;
	bool             m_isInited;          // 标识窗口空间是否初始化成功，没成功不能处理窗口消息
    bool             m_amsOffing;            // 接收到ams掉线通知为真，其它为假
	std::vector< SOptcheckBoxState > m_checkBoxState;
	
};
 
#endif    //CALARMTAB_HPP_

 
