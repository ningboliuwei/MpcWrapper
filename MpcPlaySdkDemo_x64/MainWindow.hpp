#ifndef MAINWINDOW_HPP_
#define MAINWINDOW_HPP_

#include <QtGui/QMainWindow>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QGraphicsView>

class CResourceTreeWidget;
class CConfigScene;
class CAlarmTab;

class CMainWindow : public QMainWindow
{
	Q_OBJECT
public:
    CMainWindow(  QWidget *parent = 0 );
    virtual ~CMainWindow();

public:
    bool init();
    bool fini();

	QTabWidget*    getTabWidget() { return m_tabWidget; }
	QWidget*	   getLivingViewTab() { return	m_livingViewTab; }
	QWidget*       getReplayViewTab(){ return  m_replayViewTab; }
    CConfigScene*  getConfigScene(){ return m_scene; }
	CAlarmTab*     getAlarmTab() { return m_alarmTab; }
	CResourceTreeWidget* getTreeWidget() { return m_treeWidget; }

protected:

	virtual void closeEvent ( QCloseEvent * event );

protected slots:

	void tabIndexChangedSlot( int index );

private:
    CResourceTreeWidget         *m_treeWidget;
    QTabWidget                  *m_tabWidget;

    QGraphicsView				*m_videoWallTab;
	QWidget						*m_livingViewTab;
	QWidget						*m_replayViewTab;
	CAlarmTab					*m_alarmTab;

    CConfigScene				*m_scene;
};
#endif