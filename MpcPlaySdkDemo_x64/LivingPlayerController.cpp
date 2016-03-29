#include "LivingPlayerController.hpp"
#include <time.h>

CLivingPlayerController::CLivingPlayerController() 
: m_mediaPlayer( 0 )
, m_ptzController( 0 )
, m_livingStream( 0 )
, m_session( 0 )
{}

CLivingPlayerController::~CLivingPlayerController()
{
	release();
}

void CLivingPlayerController::release()
{
	if ( m_livingStream )
	{
		m_livingStream->fini();
		m_livingStream->release();
		m_livingStream = 0;
	}

	if ( m_mediaPlayer )
	{
		m_mediaPlayer->fini();
		m_mediaPlayer->release();
		m_mediaPlayer = 0;
	}

	if ( m_ptzController )
	{	
		m_ptzController->release();
		m_ptzController = 0;
	}

     m_session = NULL;
}

bool CLivingPlayerController::init( mpc::nsdk::IManageSession* session,
								   const SResourceInfo& resInfo,
								   const bool sub,
								   const HWND hwnd )
{
    // ÊÍ·Å×ÊÔ´
	release();

	m_session = session;
	m_resInfo = resInfo;

	m_livingStream = session->createLivingStream( m_resInfo.resId, sub );
	if( !m_livingStream )
	{
		return false;
	}

	m_ptzController = session->createPtzController( m_resInfo.resId );
	m_mediaPlayer = mpc::psdk::Factory::createLivingPlayer( hwnd );
	if ( !m_mediaPlayer )
	{
		release();
		return false;
	}

	if ( !m_mediaPlayer->init() )
	{
		release();
		return false;
	}

	m_livingStream->setHandler( m_mediaPlayer );
	if ( !m_livingStream->init() )
	{
		release();
		return false;
	}

	return true;
}

bool CLivingPlayerController::mute()
{
	if ( m_mediaPlayer )
	{
		return m_mediaPlayer->mute();
	}

	return false;
}

void CLivingPlayerController::mute( const bool isMute )
{
	if ( m_mediaPlayer )
	{
		return m_mediaPlayer->mute( isMute );
	}
}

int  CLivingPlayerController::volume()
{
	if ( m_mediaPlayer )
	{
		return m_mediaPlayer->volume();
	}

	return -1;
}

void CLivingPlayerController::volume( int v )
{
	if ( m_mediaPlayer )
	{
		return m_mediaPlayer->volume( v );
	}
}

bool CLivingPlayerController::snap( const char *filePath, const size_t filePathSize )
{
	if ( m_mediaPlayer )
	{
		return m_mediaPlayer->snap( filePath, filePathSize );
	}

	return false;
}

mpc::nsdk::IPtzController  *CLivingPlayerController::getPtzController()
{
	return m_ptzController;
}

SResourceInfo& CLivingPlayerController::getResInfo()
{
	return m_resInfo;
}

bool CLivingPlayerController::fini()
{
	release();
	//memset( &m_resInfo, 0, sizeof( m_resInfo ) );
	m_resInfo.init();
	return true;
}







