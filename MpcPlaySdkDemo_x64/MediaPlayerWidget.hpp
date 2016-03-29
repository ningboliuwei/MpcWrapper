#ifndef LIVINGPLAYERWIDGET_HPP_
#define LIVINGPLAYERWIDGET_HPP_
 
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>
#include <QtGui/QDropEvent>
#include "../include/SysTypeDefine.hpp"

#include "LivingPlayerController.hpp"
#include "PtzController.hpp"
#include "RePlayerController.hpp"
#include "DownLoadController.hpp"
#include "VideoEffectControler.hpp"
 
class CMediaPlayerWidget : public QWidget
{
    Q_OBJECT

public:
    CMediaPlayerWidget( QWidget *parent = 0, 
						Qt::WindowFlags flag = 0, 
						PLAY_TYPE playType = LIVING_PLAY );

    ~CMediaPlayerWidget();

	// 停止实时播放
	void play( mpc::nsdk::IManageSession* session, const SResourceInfo& resInfo );

	// 拍照
	bool snap( QString snapPath );

	// 反初始化
	bool fini();

	// 链接断开
	bool closeSession( mpc::nsdk::IManageSession* session );

	CPtzController& getPtzController() { return m_ptzController; }
	CVideoEffectControler& getVidEffController(){ return m_vidEffController; }

	CRePlayerController& getReplayerController() {  return m_rePlayerController; } 
	CLivingPlayerController& getMediaPlayerController() { return m_mediaPlayerController; }

protected:
	void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

private:
	QString getImageName( const QString &resName, QString& dirPath );

private:

	PLAY_TYPE			    m_playType;
	CPtzController          m_ptzController;
	CVideoEffectControler   m_vidEffController;
	CRePlayerController     m_rePlayerController;
	CLivingPlayerController m_mediaPlayerController;
	QWidget*                m_parentWidget;

};
 
#endif    //CLIVINGPLAYERWIDGET_HPP_
 
