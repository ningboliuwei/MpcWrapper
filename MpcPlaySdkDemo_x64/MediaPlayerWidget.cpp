#include "MediaPlayerWidget.hpp"
#include <iostream>
#include <QtGui/QPainter>
#include <QtGui/QDesktopServices>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtGui/QFileDialog>
#include "MulLivingPlayerTab.hpp"
#include "ReplayTab.hpp"
#include "LivingControlWidget.hpp"
#include "../include/Common.hpp"
 
CMediaPlayerWidget::CMediaPlayerWidget(QWidget *parent, Qt::WindowFlags flag, PLAY_TYPE playType )
	: QWidget( parent, flag )
	, m_playType( playType )
	, m_parentWidget( parent )
{
    this->resize(400, 400);

	setAutoFillBackground(true);
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(0, 0, 0));
	setPalette(palette);

	setAcceptDrops(true);

    this->setWindowTitle( tr("CMediaPlayerWidget") );
}
 
CMediaPlayerWidget::~CMediaPlayerWidget()
{}

void CMediaPlayerWidget::play( mpc::nsdk::IManageSession* session, const SResourceInfo& resInfo )
{
	if( m_playType == LIVING_PLAY )
	{
		// 在切换到没有资源的窗口时，需要置空，否则会使用上一个窗口的云台控制器
		m_ptzController.init( NULL ); 
		bool sub = ( ( CMulLivingPlayerTab* )m_parentWidget )->getLivingControlWidget()->getStreamType();
		if ( m_mediaPlayerController.init( session, resInfo, sub, winId() ) )
		{
			// 云台
			if ( resInfo.isHasPtz )
			{
				m_ptzController.init( m_mediaPlayerController.getPtzController() );
			}
		}

		// 图像质量
		m_vidEffController.init( session->createVideoEffectControler( resInfo.resId ) );

		// 更新控制窗口
		Sleep( 100 );
		( ( CMulLivingPlayerTab* ) m_parentWidget )->updateActiveWidget( this );
	}
	else  // 历史回放
	{
		m_rePlayerController.init( session, resInfo, winId() );

		// 更新控制窗口
		( ( CReplayTab* ) m_parentWidget )->updateActiveWidget( this );
	}
}

bool CMediaPlayerWidget::fini()
{
	if( m_playType == LIVING_PLAY )
	{
		if ( m_mediaPlayerController.getSession() != NULL )
		{
			m_ptzController.init( NULL );
			m_vidEffController.init( NULL );
			m_mediaPlayerController.fini();
			Sleep( 100 );
		}
	}
	else
	{
		if ( m_rePlayerController.getSession() != NULL )
		{
			m_rePlayerController.fini();
			Sleep( 200 );
		}
	}

	this->update();
	return true;
}

bool CMediaPlayerWidget::snap( QString snapPath )
{
	QString fileName;
	if( m_playType == LIVING_PLAY  )
	{
		fileName = getImageName( QString().fromLocal8Bit( m_mediaPlayerController.getResInfo().resName.c_str() ), snapPath );
		return m_mediaPlayerController.snap( fileName.toUtf8().constData(), fileName.toUtf8().length() );
	}
	else
	{
		fileName = getImageName( QString().fromLocal8Bit( m_rePlayerController.getResInfo().resName.c_str() ), snapPath );
		return m_rePlayerController.snap( fileName.toUtf8().constData(), fileName.toUtf8().length() );
	}
}

QString CMediaPlayerWidget::getImageName( const QString &resName, QString &dirPath )
{	
	QString snapPicturePath;
	if ( dirPath.isEmpty() )
	{
		// 获取系统默认的图片存储路径
		const QString pictureLocationPath = QDesktopServices::storageLocation( QDesktopServices::PicturesLocation );
		if( ( !pictureLocationPath.isEmpty() ) && ( QDir( pictureLocationPath ).exists() ) )
		{
			snapPicturePath = pictureLocationPath + "\\";
		}
		else
		{
			const QString currentPath("./");
			QFileInfoList infoDirs = QDir::drives();
			if(infoDirs.empty())
			{
				snapPicturePath = currentPath;
			}
			else
			{
				const QString wtoePath("wtoe/image/");
				QDir parentDir( infoDirs.at(0).absolutePath() );
				if( !parentDir.mkpath( wtoePath ) )
				{
					snapPicturePath = currentPath;
				}
				else
				{
					snapPicturePath = parentDir.absolutePath() + wtoePath;
				}
			}
		}
	}
	else
	{
		snapPicturePath = dirPath;
	}

	const QString fileName = resName + "-" + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz") + QString(".png");	
	return ( snapPicturePath + fileName );
}

bool CMediaPlayerWidget::closeSession( mpc::nsdk::IManageSession* session )
{
	if( m_playType == LIVING_PLAY )
	{
		if( !m_mediaPlayerController.getSession() )
		{
			return false;
		}

		if ( session == m_mediaPlayerController.getSession() )
		{
			fini();
            this->update();
			return true;
		}
	}
	else
	{
		if( !this->m_rePlayerController.getSession() )
		{
			return false;
		}

		if ( session == m_rePlayerController.getSession() )
		{
			fini();
			return true;
		}
	}

	return false;
}

void CMediaPlayerWidget::dropEvent( QDropEvent *event )
{
    if (event->mimeData()->hasFormat(DRAGANDDROP_DATATYPE))
    {
        QByteArray itemData = event->mimeData()->data( DRAGANDDROP_DATATYPE );
        QDataStream dataStream( &itemData, QIODevice::ReadOnly );

        QVector<intptr_t>   manageSessionVec;   // 回话链接
        QVector<QString>    resNameVec;         // 资源名字
        QVector<QString>    resIdVec;           // 资源标识
        QVector<bool>       ptzStatusVec;       // 结点云台状态


        dataStream >> manageSessionVec >> resNameVec >> resIdVec >> ptzStatusVec; //保存云台是否可控

        if (m_parentWidget)
        {
            if (m_playType == LIVING_PLAY)
            {
                ((CMulLivingPlayerTab*)m_parentWidget)->dragEventHandle(manageSessionVec, resNameVec, resIdVec, ptzStatusVec, this);
            }
            else if (m_playType == RE_PLAY)
            {
                ((CReplayTab*)m_parentWidget)->dragEventHandle(manageSessionVec, resNameVec, resIdVec, ptzStatusVec, this);
            }
            
        }
    }
}

void CMediaPlayerWidget::dragEnterEvent( QDragEnterEvent *event )
{
    event->setDropAction( Qt::CopyAction );
    event->accept();
}
