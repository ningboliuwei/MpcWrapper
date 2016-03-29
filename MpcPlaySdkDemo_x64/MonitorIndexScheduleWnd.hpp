#ifndef MONITORINDEXSCHEDULEWND_HPP_
#define MONITORINDEXSCHEDULEWND_HPP_

#include <QtGui/QDialog>
#include <QtCore/QEvent>
#include <QtGui/QCloseEvent>

class QLineEdit;
class QGridLayout;
class QVBoxLayout;
class QPushButton;
class QLabel;

#include "../include/Common.hpp"
#include "ResourceTreeWidget.hpp"

class CConfigScene;

class CMonitorIndexScheduleWnd : public QDialog
{
	Q_OBJECT

public:
	CMonitorIndexScheduleWnd(CResourceTreeWidget *tree, mpc::nsdk::IManageSession *ms, const SResourceInfo &info, CConfigScene *scene );
	~CMonitorIndexScheduleWnd();

private slots:
	void onOkButtonClicked();
	void onCancelButtonClicked();

private:
	void closeEvent(QCloseEvent *event);

private:
	QLineEdit *m_indexEditor;
	QPushButton *m_okButton;
	QPushButton *m_cancelButon;
	QLabel *m_indexLabel;
	QVBoxLayout *m_mainLayout;
	CResourceTreeWidget *m_resourceTreeWidget;

	 mpc::nsdk::IManageSession *m_managerSession;
	 SResourceInfo m_resInfo;
	 CConfigScene *m_scene;
};

#endif
