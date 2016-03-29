#ifndef REPLAYERCONTROLLER_HPP_
#define REPLAYERCONTROLLER_HPP_

#include <QtGui/QWidget>
#include <QtCore/QVector>
#include "../include/SysTypeDefine.hpp"

//////////////////////////////////////////////////////////////////////////
/////////////////////////设置回放速度范围/////////////////////////////////
//////////////////////////////////////////////////////////////////////////
const int MAX_SPEED = 8;// [1/8, 8]
#define SLIDER_MID_VAL ( MAX_SPEED-2 )    
#define SLIDER_MAX_VAL ( SLIDER_MID_VAL + MAX_SPEED )  // 滑块最大值
#define SPEED_ABLOVE( val )  ( val <= SLIDER_MID_VAL ? 1 : ( val - SLIDER_MID_VAL )  )
#define SPEED_BELOW( val )  ( val <= SLIDER_MID_VAL ? ( MAX_SPEED - val ) : 1 )

class CRePlayerController
{
public:

	CRePlayerController();
	~CRePlayerController();

	bool init( mpc::nsdk::IManageSession* session, 
			   const SResourceInfo& resInfo,
			   HWND hwnd );

	bool fini();
	bool mute();
	void mute( const bool isMute );

    int  volume();
    void volume( int v );
    bool snap( const char *filePath, const size_t filePathSize );

	bool play();
	bool step();
	bool pause();

	bool modifySpeed( const uint8_t numerator, const uint8_t denominator );
	bool modifyTimes( const uint32_t timeBegin, const uint32_t timeEnd );
	bool retrieveReplayPeriod( const uint32_t beginTime, 
							   const uint32_t endTime,
							   TimeSegMents& timeSegment );

	SResourceInfo&  getResInfo() { return m_resInfo; } 
	mpc::nsdk::IManageSession* getSession() { return m_session; }
	TimeSegMents& getTimeSegMent() { return m_timeSegment; }

	bool getPause() { return m_isPause; }
	void setPause( bool pause ) { m_isPause = pause; }
	
	void setSpeed( int speed ) { m_speed = speed; }
	int  getSpeed() { return m_speed; }

private:
	void release();

private:
	mpc::nsdk::IManageSession  *m_session;
	SResourceInfo               m_resInfo;
	mpc::nsdk::IReplayStream   *m_replayStream;
	mpc::psdk::IReplayPlayer   *m_rePlayPlayer;

	TimeSegMents m_timeSegment;
	bool m_isPause;
	int  m_speed;  // 默认值7，范围 [0，15]
};

#endif // REPLAYERCONTROLLER_HPP_
