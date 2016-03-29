#ifndef VIDEOPLAYER_HPP_
#define VIDEOPLAYER_HPP_

#include <QtGui/QWidget>

#include "MpcNetSdkItf.hpp"
#include "MpcPlaySdkItf.hpp"
#include "DecoderDisplayWidget.hpp"

class VideoPlayer : public QWidget
                    , public mpc::psdk::IMediaData
{
    Q_OBJECT
public:
    VideoPlayer( int i
        , std::string addr
        , std::string user
        , std::string pwd
        , std::string uid
        , uint32_t tim
        , QWidget *parent = 0 );
    ~VideoPlayer();

public slots:
    void start();
    void stop();
    void replayPlay();
    void replayPause();
    void replayStep();
    void volume();
    void mute1();
    void mute2();
    void snap();

public:
    bool init();
    bool fini();

public:
    virtual void picData(const mpc::psdk::SPicInfo & picInfo);

private:
    mpc::nsdk::IManageSession *m_managerSession;
    mpc::nsdk::ILivingStream  *m_livingStream;
    mpc::psdk::ILivingPlayer  *m_livingPlayer;

    mpc::nsdk::IReplayStream  *m_replaytream;
    mpc::psdk::IReplayPlayer  *m_replayPlayer;

    mpc::psdk::IMediaDecoder  *m_mediaDecoder;

    std::string m_addr;
    std::string m_user;
    std::string m_pwd;
    std::string m_uid;
    uint32_t    m_tim;

private:
    CDeocderDisplayWidget       *m_videoWidget;
    int                         m_type;
};

#endif
