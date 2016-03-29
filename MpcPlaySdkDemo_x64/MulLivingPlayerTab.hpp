#ifndef MULLIVINGPLAYERWIDGET_HPP_
#define MULLIVINGPLAYERWIDGET_HPP_
 
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>
#include "MpcPlaySdkItf.hpp"
#include "MpcNetSdkItf.hpp"
#include "../include/SysTypeDefine.hpp"
 
class CLivingControlWidget;
class CMediaPlayerWidget;
class CPtzControlWidget;
class CVidEffCtrWidget;

#define LIVING_WIDGET_NUM 16
#define LIVING_WIDGET_PER_LINE 4

class CMulLivingPlayerTab : public QWidget
{
	Q_OBJECT

public:
    CMulLivingPlayerTab( QWidget *parent = 0, Qt::WindowFlags flag = 0 );
    ~CMulLivingPlayerTab();

	QWidget* getActivePlayWidget();

	// ��ʼ��ʵʱ����
	bool play( mpc::nsdk::IManageSession* session, const SResourceInfo& resInfo );

	// ����
	void mute( bool isMute );

	// ��������
	void volume( int nVol );

	// ���д��ھ���
	void setAllPlayerMute();

	// �����˳�
	bool fini();

	// ���Ӷ���ʱʹ��
	bool closeSession( mpc::nsdk::IManageSession* session );

	// ֹͣ���в���
	void stopAll();

	// ֹͣ��ǰѡ�в��Ŵ���
	void stopActiveWidget();

    void updateLivingPlayers( const uint16_t &num );

	// 
	void updateActiveWidget( CMediaPlayerWidget* activeWidget );

	//
	CLivingControlWidget* getLivingControlWidget() { return m_livingControlWidget; }

    void dragEventHandle( const QVector<intptr_t> &manageSessionVec
                                             , const QVector<QString> &resNameVec
                                             , const QVector<QString> &resIdVec
                                             , const QVector<bool> &ptzStatusVec
                                             , QWidget *ptr );

    void doubleClickPlayerWidget( const int &index );

protected:
	void paintEvent( QPaintEvent * event );
	void mousePressEvent ( QMouseEvent * event );
    void mouseDoubleClickEvent ( QMouseEvent * event );
 
private:

	QVBoxLayout* verticalLayout;
    QHBoxLayout *horizontalLayout;

	CMediaPlayerWidget* m_playWidget[LIVING_WIDGET_NUM];
	CMediaPlayerWidget* m_activeWidget;

	CLivingControlWidget* m_livingControlWidget;
	CPtzControlWidget* m_ptzWidget;
	CVidEffCtrWidget* m_vidEffCtrWidget;

    QGridLayout* gridLayout;
    uint16_t m_currentWindowNum;
};
 
#endif    //CMULLIVINGPLAYERWIDGET_HPP_
 
