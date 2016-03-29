#ifndef DOWNLOADFILE_HPP_
#define DOWNLOADFILE_HPP_

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QDataStream>
#include <QtGui/QDesktopServices>

#include <string>

#include "../include/SysTypeDefine.hpp"

/* �����mp4�洢���ԣ�������º�
 *  �������ڹ����ļ�������ͷ�ļ�����·����������·������������·����
 */
//#define TEST_MP4_PACKAGE

#ifdef TEST_MP4_PACKAGE
#include "MediaStreamPackage/MediaStreamPackageItf.hpp"
#endif


class CDownLoadFile
{
public:
	CDownLoadFile();
	~CDownLoadFile();

    void setSteamType(const mpc::nsdk::IStreamHandler::EStreamType md)
    {
        m_streamMediaType = md;
    }

	bool saveData( const std::string& resName, 
		const unsigned int beginTime,
		const unsigned int endTime,
		unsigned char* buf, 
		unsigned int len, 
		std::string& newFile );

	void endWrite();

private:
	
	QString getDownLoadName( const QString& resName, 
		const unsigned int beginTime, 
		const unsigned int endTime );

private:
	QFile        m_downLoadFile;
	unsigned int m_writeBytes;
	bool         m_status_opened;
    mpc::nsdk::IStreamHandler::EStreamType     m_streamMediaType;

#ifdef TEST_MP4_PACKAGE
    mpc::IMediaStreamPackage    * m_mediaPackage;
#endif
};


#endif // DOWNLOADFILE_HPP_