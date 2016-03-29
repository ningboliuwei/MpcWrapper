
#include <QtGui/QApplication>
#include <iostream>
#include "../include/SysTypeDefine.hpp"
#include "DownLoadController.hpp"

CDownLoadHandle::CDownLoadHandle( CDownLoadController* controller )
	: m_refCounter( 1 )
	, m_controller( controller )
	, m_downLoadStart( false )
	, m_successFlag( false )
	, m_downLoadEnd( false )
{}

CDownLoadHandle::~CDownLoadHandle()
{}

void CDownLoadHandle::startup()
{
	if ( NULL == m_controller )
	{
		return ;
	}

	if ( m_controller->startDownLoad() )
	{
		m_downLoadStart = true;
	}
	else
	{
		std::cout << "m_controller->startDownLoad fail." << std::endl;
		CDowloadEvent* event = new CDowloadEvent( DWONDLOADE_FAIL_EVENT );
		if ( event )
		{
			SDownLoadEventData data;
			data.controller = m_controller;
			event->setData( data );
			QApplication::postEvent( m_controller->getObject(), event );
		}
	}
}

void CDownLoadHandle::establish( const IStreamHandler::EStreamType md )
{
	if ( !m_downLoadStart )
	{
		return ;
	}

    m_downLoadFile.setSteamType(md);

	if ( m_controller->getCanceled() )
	{
		return ;
	}
	CDowloadEvent* event = new CDowloadEvent( DWONDLOADE_ESTABLISH_EVENT );
	if ( event )
	{
		SDownLoadEventData data;
		data.controller = m_controller;
		event->setData( data );
		QApplication::postEvent( m_controller->getObject(), event );
	}
}

void CDownLoadHandle::stream( const uint8_t *buf, const size_t len, const uint32_t realTime )
{
	if ( !len )
	{
		return ;
	}

	m_successFlag = true;  // ��ʶ���سɹ�
	
	std::string file;
	m_downLoadFile.saveData( m_controller->getDownLoadInfo().resName,
		m_controller->getDownLoadInfo().timeBegin,
		m_controller->getDownLoadInfo().timeEnd,
		(unsigned char*)buf, (uint32_t)len, file );
	
	if ( file.empty() )  // û���������ļ�
	{
		return ;
	}
	
	if ( m_controller->getCanceled() )  // �ֶ��ر��򷵻�
	{
		return ;
	}

	CDowloadEvent* event = new CDowloadEvent( DWONDLOADE_NEWFILE_EVENT );
	if ( event )
	{
		SDownLoadEventData data;
		data.controller = m_controller;
		data.fileName = file;
		event->setData( data );
		QApplication::postEvent( m_controller->getObject(), event );
	}	
}

void CDownLoadHandle::release()
{
	m_mutex.lock();
	--m_refCounter;
	if( m_refCounter == 0 )
	{
		m_mutex.unlock();
		delete( this );
	}
	else
	{
		m_mutex.unlock();
	}
}

void CDownLoadHandle::terminate()
{
	endDownLoad();
}

void CDownLoadHandle::shutdown()
{
	endDownLoad();
}

void CDownLoadHandle::addRef()
{
	m_mutex.lock();
	++m_refCounter;
	m_mutex.unlock();
}

void CDownLoadHandle::endDownLoad()
{
	if ( m_downLoadEnd )  // ���ؽ���
	{
		return ;
	}
	m_downLoadEnd = true; 
	
	if ( m_downLoadStart )  // ��ʼ���سɹ�
	{
		m_downLoadFile.endWrite();   // ����д
	}

	if ( m_controller->getCanceled() )  // �û��ֶ��ر�
	{
		return ;
	}
	else
	{
		m_controller->fini();		 // �ͷ���Դ

		CDowloadEvent* event;
		if ( m_successFlag ) // �����ɹ�
		{
			event = new CDowloadEvent( DWONDLOADE_SUCCESS_EVENT );  // ���سɹ�
		}
		else
		{
			event = new CDowloadEvent( DWONDLOADE_FAIL_EVENT );  // ����ʧ��
		}

		if ( event )
		{
			SDownLoadEventData data;
			data.controller = m_controller;
			event->setData( data );
			QApplication::postEvent( m_controller->getObject(), event );
		}
	}
}

/**
*   CDownLoadController
*/
CDownLoadController::CDownLoadController() 
	: m_downLoadHandle( 0 )
	, m_replayStream( 0 )
	, m_object( 0 )
	, m_canceled( false )
{}

CDownLoadController::~CDownLoadController()
{
	release();
}

void CDownLoadController::release()
{
	if ( m_replayStream )
	{
		m_replayStream->fini();
		m_replayStream->release();
		m_replayStream = 0;
	}

	if ( m_downLoadHandle )
	{
		m_downLoadHandle->release();
		m_downLoadHandle = 0;
	}
}

bool CDownLoadController::init( mpc::nsdk::IManageSession* session, const mpc::nsdk::SResId& resId )
{
	// �ͷ���Դ
	release();

	m_replayStream = session->createReplayStream( resId );
	if( !m_replayStream )
	{
		return false;
	}

	m_downLoadHandle  = new CDownLoadHandle( this );
	if ( !m_downLoadHandle )
	{
		release();
		return false;
	}

	m_replayStream->setHandler( m_downLoadHandle );
	if ( !m_replayStream->init() )
	{
		release();
		return false;
	}

	return true;
}

bool CDownLoadController::modifyTimes( const uint32_t timeBegin, const uint32_t timeEnd )
{
	if ( m_replayStream )
	{
		return m_replayStream->modifyTimes( timeBegin, timeEnd );
	}

	return false;
}


bool CDownLoadController::fini()
{
	release();
	return true;
}

bool CDownLoadController::retrieveReplayPeriod( const uint32_t beginTime, 
    const uint32_t endTime,
    TimeSegMents& timeSegment )
{
	if ( m_downLoadInfo.session )
	{
		mpc::nsdk::IInfoTimePeriod* tp;
		m_downLoadInfo.session->retrieveReplayPeriod( m_downLoadInfo.resId, beginTime, endTime, &tp );
		if ( !tp )
		{
			return false;
		}

		timeSegment.clear();
		mpc::nsdk::IInfoTimePeriod* tmp = tp;
		while( tmp )
		{
			timeSegment.push_back( QPair<unsigned int, unsigned int>( tmp->beginTime(), tmp->endTime() ) );
			tmp = tmp->next();
		}

		tp->release();
		tp = 0;

		return true;
	}
	return false;
}

bool  CDownLoadController::downLoad(  const SDownLoadInfo& info, QObject* object )
{
	// �������ز���
	m_downLoadInfo = info;
	m_object = object;

	// ��ʼ����������
	if ( !init( info.session, info.resId ) )
	{

#ifdef  _DEBUG
		std::cout << "CDownLoadController init fail." << std::endl;
#endif 
		return false;
	}

	return true;
}

bool CDownLoadController::startDownLoad()
{
	QVector < QPair<unsigned int, unsigned int> > timeSegment;
	if ( !retrieveReplayPeriod( m_downLoadInfo.timeBegin, m_downLoadInfo.timeEnd, timeSegment ) )
	{
		fini();
		return false;
	}

	if ( !modifyTimes( m_downLoadInfo.timeBegin, m_downLoadInfo.timeEnd ) )
	{
		fini();
		return false;
	}

	if ( !downLoad() )
	{
		fini();
		return false;
	}

	return true;
}

bool CDownLoadController::downLoad()
{
	if ( m_replayStream && m_downLoadHandle )
	{
		return m_replayStream->download();
	}
	return false;
}

