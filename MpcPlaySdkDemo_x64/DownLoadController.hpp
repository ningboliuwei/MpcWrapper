#ifndef DWONLOADCONTROLLER_HPP_
#define DWONLOADCONTROLLER_HPP_

#include "../include/SysTypeDefine.hpp"
#include <string>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QDataStream>
#include <QtGui/QDesktopServices>
#include <WTypes.h>
#include <QtCore/QPair>
#include <QtCore/QVector>
#include <QtCore/QMutex>
#include "../include/CustomEvent.hpp"
#include "DownloadFile.hpp"

class CDownLoadController;

struct SDownLoadEventData
{
	CDownLoadController* controller;
	std::string          fileName;
	std::string          status;
};

struct SDownLoadInfo
{	
	mpc::nsdk::IManageSession* session; 
	std::string		   resName;
	mpc::nsdk::SResId  resId;
	uint32_t		   timeBegin; 
	uint32_t		   timeEnd;
};

typedef CCustomEvent<SDownLoadEventData> CDowloadEvent;

class CDownLoadHandle: public mpc::nsdk::IStreamHandler
{
public:
	CDownLoadHandle( CDownLoadController* controller );

private:
	virtual ~CDownLoadHandle();
	void endDownLoad();

public:

	virtual void startup();
	virtual void shutdown();
    virtual void establish( const EStreamType md );
    virtual void stream( const uint8_t *buf, const size_t len, const uint32_t realTime );
    virtual void terminate();
    virtual void release();
	virtual void addRef();

private:

	CDownLoadController* m_controller;
	CDownLoadFile		 m_downLoadFile;
	QMutex				 m_mutex;
	uint32_t			 m_refCounter;
	bool				 m_downLoadStart;  // ��ʾ��ʼ����
	bool                 m_successFlag;    // ���سɹ���ʶ
	bool                 m_downLoadEnd;    // ���ؽ���
};

class CDownLoadController
{
public:

	CDownLoadController();
	~CDownLoadController();

	// ���س�ʼ��
	bool downLoad( const SDownLoadInfo& info, QObject* object );  

	//  ��ʼ����
	bool startDownLoad();

	// ��������
	bool fini();  

	// �ֶ��ر�����
	void setCanceled( bool flag ) { m_canceled = flag; }
	bool getCanceled() { return m_canceled; }

	// ������Ϣ���ڵĴ���
	QObject* getObject() { return m_object; }

	// ������Ϣ
	SDownLoadInfo& getDownLoadInfo() { return m_downLoadInfo; }

private:

	// ��ʼ������
	bool init( mpc::nsdk::IManageSession* session, const mpc::nsdk::SResId& resId );

	// ��������ʱ���
	bool retrieveReplayPeriod( const uint32_t beginTime, 
							   const uint32_t endTime,
							   TimeSegMents& timeSegment );

	bool modifyTimes( const uint32_t timeBegin, const uint32_t timeEnd );
	bool downLoad();
	void release();

private:

	SDownLoadInfo				m_downLoadInfo;
	QObject*					m_object;

	bool                        m_canceled;
	mpc::nsdk::IReplayStream   *m_replayStream;
	CDownLoadHandle            *m_downLoadHandle;
};

#endif // DWONLOADCONTROLLER_HPP_



