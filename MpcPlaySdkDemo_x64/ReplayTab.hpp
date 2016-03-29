#ifndef REPLAYWINDOW_HPP_
#define REPLAYWINDOW_HPP_
 
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

#include "TimeSegmentWidget.hpp"
#include "ReplayControlWidget.hpp"
#include "../include/SysTypeDefine.hpp"

#define REPLAY_WIDGET_NUM 16
#define REPLAY_WIDGET_PER_LINE 4

class CMediaPlayerWidget;
class CDownLoadWidget;

class CReplayTab : public QWidget
{
	    Q_OBJECT
public:
    CReplayTab(QWidget *parent = 0, Qt::WindowFlags flag = 0);
    ~CReplayTab();

	QWidget* getActivePlayWidget();

	// ��ʼ��ѡ�д���
	bool initActiveWidget( mpc::nsdk::IManageSession* session, const SResourceInfo& resInfo );
	
	// ����
	void mute( bool isMute );

	// ��������
	void volume( int nVol );

	// �������д��ھ���
	void setAllPlayerMute();

	// �˳���ǰ�Ự
	bool fini();

	// �ر�ָ���Ự
	bool closeSession( mpc::nsdk::IManageSession* session );

	// ָֹͣ���Ĵ���
	void stopActiveWidget();

	// ֹͣ���д���
	void stopAll();

	// ˢ��ָ������
	void updateActiveWidget( CMediaPlayerWidget* activeWidget );

	// �˳����ش���
	void closeDownLoadWidget();

	CTimeSegmentWidget* getSegTimeWidget() { return m_timeSegWidget; }
	CDownLoadWidget* getDownLoadWidget() {  return m_downLoadWidget; }
	CReplayControlWidget* getControlWidget() { return m_controlWidget; }

    void dragEventHandle( const QVector<intptr_t> &manageSessionVec
                        , const QVector<QString> &resNameVec
                        , const QVector<QString> &resIdVec
                        , const QVector<bool> &ptzStatusVec
                        , QWidget *ptr );

protected:
	void paintEvent( QPaintEvent * event );
	void mousePressEvent ( QMouseEvent * event );

private:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;

	CMediaPlayerWidget* m_playWidget[REPLAY_WIDGET_NUM];

	CMediaPlayerWidget* m_activeWidget;
	CTimeSegmentWidget* m_timeSegWidget;
	CReplayControlWidget* m_controlWidget;
	CDownLoadWidget* m_downLoadWidget;
};
 
#endif    //CREPLAYWINDOW_HPP_
 
