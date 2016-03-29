#ifndef MEDIAPLAYERCONTROLLER_HPP_
#define MEDIAPLAYERCONTROLLER_HPP_

#include "MpcNetSdkItf.hpp"
#include "MpcPlaySdkItf.hpp"
#include <QtGui/QWidget>
#include <QtCore/QVector>
#include "../include/SysTypeDefine.hpp"

class CLivingPlayerController
{
public:

	CLivingPlayerController();
	~CLivingPlayerController();

	bool init( mpc::nsdk::IManageSession* session,
		const SResourceInfo& resInfo,
		const bool sub,
		const HWND hwnd );

	bool fini();

	bool mute();
	void mute( const bool isMute );

    int  volume();
    void volume( int v );
    bool snap( const char *filePath, const size_t filePathSize );

	mpc::nsdk::IPtzController  *getPtzController();
	SResourceInfo&              getResInfo(); 
	mpc::nsdk::IManageSession*  getSession() { return m_session; }

private:
	void release();

private:
	mpc::nsdk::IManageSession  *m_session;
	SResourceInfo               m_resInfo;
	mpc::nsdk::ILivingStream   *m_livingStream;
	mpc::nsdk::IPtzController  *m_ptzController;
	mpc::psdk::IMediaPlayer    *m_mediaPlayer;
};

#endif // MEDIAPLAYERCONTROLLER_HPP_
