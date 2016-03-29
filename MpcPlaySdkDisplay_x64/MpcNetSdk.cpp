#include <iostream>
#include <sstream>
#include <fstream>
#include <ace/OS_NS_unistd.h>
#include <wtoe/TestSupport/TestSupportExp.hpp>
#include <wtoe/BasicHelper/BasicHelperExp.hpp>

#include "MpcNetSdk.hpp"

namespace mpc{
    namespace nsdk{

bool Runtime::init()
{
    return true;
}

bool Runtime::fini()
{
    return true;
}

bool Runtime::okey()
{
    return true;
}

IManageSession *Factory::createManageSession( const uint32_t addr, const uint16_t port, 
                                              const char *username, const size_t usernameSize,
                                              const char *password, const size_t passwordSize )
{
    return new CManageSession();
}

//////////////////////////////////////////////////////////////////////////
CManageSession::CManageSession()
{

}

CManageSession::~CManageSession()
{

}

bool CManageSession::init()
{
    return true;
}

bool CManageSession::fini()
{
    return true;
}

bool CManageSession::okey()
{
    return true;
}

bool CManageSession::login()
{
    return true;
}

bool CManageSession::logout()
{
    return true;
}

void CManageSession::subscribe( IManageSessionNotify *notify )
{

}

IMediaIterator *CManageSession::createMediaIterator()
{
    return 0;
}

ILivingStream *CManageSession::createLivingStream( const SResId &msid, const bool sub )
{
    return new CLivingStream();
}

IReplayStream *CManageSession::createReplayStream( const SResId &msid )
{
    return new CReplayStream();
}
IPtzController *CManageSession::createPtzController( const SResId &msid )
{
    return 0;
}
IWallController *CManageSession::createWallHandler()
{
    return 0;
}
bool CManageSession::retrieveReplayPeriod( const SResId &msid, const uint32_t beginTime, const uint32_t endTime, IInfoTimePeriod **tp )
{
    return true;
}
void CManageSession::release()
{
    delete this;
}

//////////////////////////////////////////////////////////////////////////
CFileStream::CFileStream( IStreamHandler *handler )
: m_handler( handler ), m_okey( false ), m_start( false )
{

}

CFileStream::~CFileStream()
{
}

bool CFileStream::init_()
{
    if( okey_() ) return true;

    /*
     *	读文件至内存
     */
    std::string tmp = "test.mp4";
    std::ifstream tfile( tmp.c_str(), std::ios::in|std::ios::out|std::ios::binary );

    while(!tfile.eof())  
    {  
        SUnit unit;

        unit.buff = new char[BUFF_LEN];
        unit.len = tfile.read( unit.buff, sizeof(char) * BUFF_LEN ).gcount();

        m_buff.push_back( unit );
    }  

	return ( m_okey = true );
}

bool CFileStream::fini_()
{
    if( !okey_() ) return true;

    m_okey = false;
    
    /*
     *	删除内存
     */
    std::vector<SUnit>::const_iterator it( m_buff.begin() );
    std::vector<SUnit>::const_iterator endIt( m_buff.end() );
    for( ; it != endIt; ++it )
    {
        delete (*it).buff;
    }
    
    m_buff.clear();
    return true;
}

bool CFileStream::okey_()
{
    return m_okey;
}

bool CFileStream::start()
{
    if( !okey_() )
        return false;

    if( isStarted() )
        return false;

    {
        ACE_GUARD_RETURN( ACE_Thread_Mutex, guard, m_mutex, false );
		m_start = true;
    }

    /*
     *	开启播放线程
     */
    activate();
    return true;
}

bool CFileStream::stop()
{
    if( !isStarted() )
        return false;

    {
        ACE_GUARD_RETURN( ACE_Thread_Mutex, guard, m_mutex, false );
        m_start = false;
    }

    /*
     *	等待线程退出
     */
    wait();
    return true;
}

bool CFileStream::isStarted()
{
    return m_start;
}

int CFileStream::svc()
{
    int pos = 0;
    while( true )
    {
        if( !isStarted() )
            return 0;

        if( !m_handler )
            return 0;

        if( pos == m_buff.size() -1 )
            pos = 0;

        SUnit t = m_buff[pos];
        m_handler->stream( (uint8_t*)t.buff, t.len );

        ++pos;

        wtoe::CSharedTimer().sleep( 30 );
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
CLivingStream::CLivingStream()
:m_handler ( 0 ), m_stream( 0 )
, m_okey( false )
{

}

CLivingStream::~CLivingStream()
{

}

bool CLivingStream::init()
{
    if( m_okey )
        return true;

    if( !m_handler || !m_stream )
        return false;

    m_handler->establish( IStreamHandler::PS );

    if( !m_stream->start() )
    {
        m_stream->fini_();
        delete_o ( m_stream );
        m_stream = 0;
        return false;
    }

    return ( m_okey = true );
}

bool CLivingStream::fini()
{
    if( !m_okey )
        return true;

    m_okey = false;

    if( m_handler )
        m_handler->terminate();

    if( m_stream )
    {
        m_stream->stop();
        m_stream->fini_() ;
        delete_o( m_stream );
        m_stream = 0;
    }

    return true;
}

bool CLivingStream::okey()
{
    return m_okey;
}

bool CLivingStream::setHandler( IStreamHandler *handler )
{
    m_handler = handler;

    m_stream = new_o( CFileStream, m_handler );
    if( !m_stream ) 
        return false;

    if( !m_stream->init_() )
    {
        delete_o ( m_stream );
        m_stream = 0;
        return false;
    }

    return true;
}

void CLivingStream::release()
{
    delete this;
}

//////////////////////////////////////////////////////////////////////////
CReplayStream::CReplayStream()
:m_handler ( 0 ), m_stream( 0 )
,m_okey( false )
{

}

CReplayStream::~CReplayStream()
{

}

bool CReplayStream::init()
{
    if( m_okey )
        return true;

    if( !m_handler || !m_stream )
        return false;

    m_handler->establish( IStreamHandler::PS );

    if( !m_stream->start() )
    {
        m_stream->fini_();
        delete_o ( m_stream );
        m_stream = 0;
        return false;
    }

    return ( m_okey = true );
}

bool CReplayStream::fini()
{
    if( !m_okey )
        return true;

    m_okey = false;

    if( m_handler )
        m_handler->terminate();

    if( m_stream )
    {
        m_stream->stop();
        m_stream->fini_() ;
        delete_o( m_stream );
        m_stream = 0;
    }
    
    return true;
}

bool CReplayStream::okey()
{
    return m_okey;
}
bool CReplayStream::setHandler( IStreamHandler *handler )
{
    m_handler = handler;

    m_stream = new_o( CFileStream, m_handler );
    if( !m_stream ) 
        return false;

    if( !m_stream->init_() )
    {
        delete_o ( m_stream );
        m_stream = 0;
        return false;
    }

    return true;
}

bool CReplayStream::modifyTimes( const uint32_t timeBegin, const uint32_t timeEnd )
{
    return true;
}
bool CReplayStream::modifySpeed( const uint8_t numerator, const uint8_t denominator )
{
    return true;
}
bool CReplayStream::play()
{
    return true;
}
bool CReplayStream::pause()
{
    return true;
}
bool CReplayStream::step()
{
    return true;
}
bool CReplayStream::download()
{
    return true;
}
void CReplayStream::release()
{
    delete this;
}

}
}