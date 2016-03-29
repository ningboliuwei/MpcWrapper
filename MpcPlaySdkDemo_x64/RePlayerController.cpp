#include "RePlayerController.hpp"

CRePlayerController::CRePlayerController() 
	: m_rePlayPlayer( 0 )
	, m_replayStream( 0 )
	, m_session( 0 )
	, m_isPause( false )
	, m_speed( SLIDER_MID_VAL + 1 )
{
	m_timeSegment.clear();
}

CRePlayerController::~CRePlayerController()
{
	release();
}

void CRePlayerController::release()
{
	if ( m_replayStream )
	{
		m_replayStream->fini();
		m_replayStream->release();
		m_replayStream = 0;
	}

	if ( m_rePlayPlayer )
	{
		m_rePlayPlayer->fini();
		m_rePlayPlayer->release();
		m_rePlayPlayer = 0;
	}

    m_session = NULL;
	m_isPause = false;
	m_timeSegment.clear();

	m_speed = SLIDER_MID_VAL + 1;
}

bool CRePlayerController::init( mpc::nsdk::IManageSession* session,
							   const SResourceInfo& resInfo,
							   HWND hwnd )
{
    // 释放资源
	release();

	m_session = session;
	m_resInfo = resInfo;

	m_replayStream = session->createReplayStream( m_resInfo.resId );
	if( !m_replayStream )
	{
		return false;
	}

	m_rePlayPlayer = mpc::psdk::Factory::createReplayPlayer( hwnd );
	if ( !m_rePlayPlayer )
	{
		release();
		return false;
	}

	if ( !m_rePlayPlayer->init() )
	{
		release();
		return false;
	}

	m_replayStream->setHandler( m_rePlayPlayer );
	if ( !m_replayStream->init() )
	{
		release();
		return false;
	}

	return true;
}

bool CRePlayerController::mute()
{
	if ( m_rePlayPlayer )
	{
		return m_rePlayPlayer->mute();
	}
	return false;
}

void CRePlayerController::mute( const bool isMute )
{
	if ( m_rePlayPlayer )
	{
		return m_rePlayPlayer->mute( isMute );
	}
}

int  CRePlayerController::volume()
{
	if ( m_rePlayPlayer )
	{
		return m_rePlayPlayer->volume();
	}

	return -1;
}

void CRePlayerController::volume( int v )
{
	if ( m_rePlayPlayer )
	{
		return m_rePlayPlayer->volume( v );
	}
}

bool CRePlayerController::snap( const char *filePath, const size_t filePathSize )
{
	if ( m_rePlayPlayer )
	{
		return m_rePlayPlayer->snap( filePath, filePathSize );
	}

	return false;
}

bool CRePlayerController::modifySpeed( const uint8_t numerator, const uint8_t denominator )
{
	if ( m_rePlayPlayer && m_replayStream )
	{
		return m_replayStream->modifySpeed( numerator, denominator );
	}

	return false;
}

bool CRePlayerController::play()
{
	m_isPause = false;
	if ( m_rePlayPlayer && m_replayStream )
	{
		if ( !m_replayStream->play() )  // 请求流
		{
			return false;
		}

		m_rePlayPlayer->play(); // 设置解码
		return true;
	}

	return false;
}

bool CRePlayerController::step()
{
	if ( m_rePlayPlayer )
	{
		if( m_replayStream->step() )
		{
			return m_rePlayPlayer->step();
		}
	}

	return false;
}

bool CRePlayerController::pause()
{
	if ( m_rePlayPlayer && m_replayStream  )
	{
		m_replayStream->pause();  // 流暂停
		m_rePlayPlayer->pause();  // 解码暂停
		m_isPause = true;
		return true;
	}

	return false;
}

bool CRePlayerController::retrieveReplayPeriod( const uint32_t beginTime, 
												const uint32_t endTime,
												QVector < QPair<unsigned int, unsigned int> >& timeSegment )
{
	if ( m_session )
	{
		mpc::nsdk::IInfoTimePeriod* tp;
		m_session->retrieveReplayPeriod( m_resInfo.resId, beginTime, endTime, &tp );
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

		m_timeSegment.clear();
		m_timeSegment << timeSegment;
		return true;
	}
	return false;
}

bool CRePlayerController::modifyTimes( const uint32_t timeBegin, const uint32_t timeEnd )
{
	if ( m_replayStream )
	{
		return m_replayStream->modifyTimes( timeBegin, timeEnd );
	}

	return false;
}

bool CRePlayerController::fini()
{
	release();
	//memset( &m_resInfo, 0, sizeof( m_resInfo ) );
	m_resInfo.init();
	return true;
}

