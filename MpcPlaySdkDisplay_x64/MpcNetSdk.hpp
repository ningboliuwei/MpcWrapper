#ifndef MPCNETSDK_HPP_
#define MPCNETSDK_HPP_

#include <vector>
#include <ace/Task.h>
#include <ace/Thread_Mutex.h>

#include "psdk/iil/MpcNetSdkItf.hpp"

namespace mpc
{
namespace nsdk{

class CManageSession: public IManageSession
{
public:
    CManageSession();
private:
    virtual ~CManageSession();

public:
    virtual bool init();
    virtual bool fini();
    virtual bool okey();
    virtual bool login();
    virtual bool logout();
    virtual void subscribe( IManageSessionNotify *notify );
    virtual IMediaIterator *createMediaIterator();
    virtual ILivingStream *createLivingStream( const SResId &msid, const bool sub );
    virtual IReplayStream *createReplayStream( const SResId &msid );
    virtual IPtzController *createPtzController( const SResId &msid );
    virtual IWallController *createWallHandler();
    virtual bool retrieveReplayPeriod( const SResId &msid, const uint32_t beginTime, const uint32_t endTime, IInfoTimePeriod **tp );
    virtual void release();
};

struct SUnit
{
    char *buff;
    uint32_t len;
};

const int BUFF_LEN = 8 * 1024; //KB

class CFileStream: public ACE_Task_Base
{
public:
    CFileStream( IStreamHandler *handler );
    virtual ~CFileStream();

public:
    bool init_();
    bool fini_();
    bool okey_();

    bool start();
    bool stop();
    bool isStarted();

    int svc();

private:
    IStreamHandler      *m_handler; 
    bool                m_start;
    std::vector<SUnit>  m_buff;
    ACE_Thread_Mutex    m_mutex;
    bool                m_okey;
};

class CLivingStream: public ILivingStream
{
public:
    CLivingStream();
private:
    virtual ~CLivingStream();

public:
    virtual bool init();
    virtual bool fini();
    virtual bool okey();
    virtual bool setHandler( IStreamHandler *handler );
    virtual void release();

private:
    CFileStream             *m_stream;
    IStreamHandler          *m_handler;
    bool                    m_okey;
};

class CReplayStream: public IReplayStream
{
public:
    CReplayStream();
private:
    virtual ~CReplayStream();

public:
    virtual bool init();
    virtual bool fini();
    virtual bool okey();
    virtual bool setHandler( IStreamHandler *handler );
    virtual bool modifyTimes( const uint32_t timeBegin, const uint32_t timeEnd );
    virtual bool modifySpeed( const uint8_t numerator, const uint8_t denominator );
    virtual bool play();
    virtual bool pause();
    virtual bool step();
    virtual bool download();
    virtual void release();

private:
    CFileStream             *m_stream;
    IStreamHandler          *m_handler;
    bool                    m_okey;
};

}
}

#endif
