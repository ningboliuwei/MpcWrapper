#include "DownloadFile.hpp"
#include <time.h>
#include <QtCore/QDateTime>
#include <iostream>

#define BYTES_1G  ( 1024*1024*1024 )

QString stringFromUint32Time( unsigned int nTime )
{
	QString str;
	struct tm tmTime;

	time_t ttime = (time_t)nTime;
	localtime_s( &tmTime, &ttime );
	str.sprintf ( "%d-%02d-%02d,%02d-%02d-%02d", 
		tmTime.tm_year + 1900, 
		tmTime.tm_mon + 1, 
		tmTime.tm_mday, 
		tmTime.tm_hour, 
		tmTime.tm_min, 
		tmTime.tm_sec );

	return str;
}

CDownLoadFile::CDownLoadFile()
	: m_writeBytes( 0 )
	, m_status_opened( false )
    , m_streamMediaType( mpc::nsdk::IStreamHandler::PS )
#ifdef TEST_MP4_PACKAGE
    , m_mediaPackage( 0 )
#endif
{}

bool CDownLoadFile::saveData( const std::string& resName, 
							  const unsigned int beginTime,
							  const unsigned int endTime,
							  unsigned char* buf, 
							  unsigned int len, 
							  std::string& newFile )
{
	newFile = "";
	if ( !m_status_opened )
	{
		QString qResName = QString().fromLocal8Bit( resName.c_str() );
		QString fileName = getDownLoadName( qResName, beginTime, endTime );
		m_downLoadFile.setFileName( fileName );
		bool flag = m_downLoadFile.open( QIODevice::WriteOnly | QIODevice::Truncate );
		if ( !flag )
		{
			m_status_opened = false;
			return false;
		}

#ifdef TEST_MP4_PACKAGE
        if ( mpc::nsdk::IStreamHandler::PS == m_streamMediaType
            || mpc::nsdk::IStreamHandler::TS == m_streamMediaType )
        {
            // 当前只存储 TS/PS 流
            m_mediaPackage = mpc::IFactory::createMediaStreamPackage();
            if ( m_mediaPackage )
            {
                QString     mp4FileName = fileName + ".mp4";
                mp4FileName.replace('\\', '/');

                QByteArray  mp4FileNameLocal = mp4FileName.toUtf8();
                if ( m_mediaPackage->init(mp4FileNameLocal.constData(), mp4FileNameLocal.length(), mpc::EFILETYPE_MP4FILE) )
                {
                    if ( mpc::nsdk::IStreamHandler::TS == m_streamMediaType )
                    {
                        m_mediaPackage->setStreamType(mpc::EPACKERTYPE_TS);
                    }
                    else
                    {
                        m_mediaPackage->setStreamType(mpc::EPACKERTYPE_PS);
                    }
                }
                else
                {
                    m_mediaPackage->fini();
                    m_mediaPackage = 0;
                    std::cout << "init m_mediaPackage failed" << std::endl;
                }
            }
        }
#endif
		m_status_opened = true;
		newFile = fileName.toLocal8Bit().constData();
	}

#ifdef TEST_MP4_PACKAGE
    if ( m_mediaPackage )
    {
        m_mediaPackage->pushStream(buf, (uint32_t)len);
    }
#endif

	QDataStream out( &m_downLoadFile );
	if ( len != out.writeRawData( (char*)buf, len ) )
	{
		m_downLoadFile.close();
		m_status_opened = false;
		return false;
	}

	m_writeBytes += len;
	if ( m_writeBytes >= BYTES_1G )
	{
		m_writeBytes = 0;

		// 关闭文件
		m_downLoadFile.close();
		m_status_opened = false;
	}

	return true;
}

QString CDownLoadFile::getDownLoadName( const QString& resName, 
									    const unsigned int beginTime, 
									    const unsigned int endTime )
{
	QString downLoadPath;

	// 获取系统默认的图片存储路径
	const QString pictureLocationPath = QDesktopServices::storageLocation( QDesktopServices::PicturesLocation );
	if( ( !pictureLocationPath.isEmpty() ) && ( QDir( pictureLocationPath ).exists() ) )
	{
		downLoadPath = pictureLocationPath + "\\";
	}
	else
	{
		// 获取所有盘符路径
		QFileInfoList infoDirs = QDir::drives();
		const QString currentPath( "./" );

		if( infoDirs.empty() )   // 当前路径
		{
			// 都不成功的时候才取这个路径
			
			downLoadPath = currentPath;
		}
		else
		{
			// 取第一个盘符
			QDir parentDir( infoDirs.at( 0 ).absolutePath() );

			// 盘符目录下的相对路径
			const QString wtoePath( "wtoe/image/" );
			if( !parentDir.mkpath( wtoePath ) )
			{
				downLoadPath = currentPath;
			}
			else
			{
				downLoadPath = parentDir.absolutePath() + wtoePath;
			}
		}
	}

	// 设置文件绝对路径的名字
	QString strStartTime = stringFromUint32Time( beginTime );
	QString strEndTime = stringFromUint32Time( endTime );
	const QString tmp = resName + "(" + strStartTime + "---" + strEndTime + ")" +
		QDateTime::currentDateTime().toString( "yyyy-MM-dd-hh-mm-ss-zzz" ) + QString( ".H264" );

	downLoadPath += tmp;

	return downLoadPath;
}

CDownLoadFile::~CDownLoadFile()
{
	endWrite();

#ifdef TEST_MP4_PACKAGE
    if ( m_mediaPackage )
    {
        m_mediaPackage->fini();
        m_mediaPackage = NULL;
    }
#endif

}

void CDownLoadFile::endWrite()
{
	if( m_status_opened )
	{
		m_writeBytes = 0;
		m_downLoadFile.close();
		if ( !m_downLoadFile.size() )  // 有数据
		{
			m_downLoadFile.remove();
		}

		m_status_opened = false;
	}
}


