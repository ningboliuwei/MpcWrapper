#ifndef VIDEOEFFECTCONTROLLER_HPP_
#define VIDEOEFFECTCONTROLLER_HPP_

#include "MpcPlaySdkItf.hpp"
#include "MpcNetSdkItf.hpp"
#include <string>

class CVideoEffectControler
{
public:
	CVideoEffectControler();
	~CVideoEffectControler();

	bool init( mpc::nsdk::IVideoEffectControler  *pImgQltCtrl );

public:
	
	/**
     * 色度配置
     * 色度范围[0~255]
     */
	bool getMediaHue( uint8_t &hue );
	void setMediaHue( uint8_t hue );
    
    /**
     * 亮度配置
     * 亮度范围[0~255]
     */
    bool getMediaBrightness( uint8_t &brightness );
    void setMediaBrightness( uint8_t brightness );
    
	/**
     * 饱和度配置
     * 饱和度范围[0~255]
     */
    bool getMediaSaturation( uint8_t &saturation );
    void setMediaSaturation( uint8_t saturation );
    
    /**
     * 对比度配置
     * 对比度范围[0~255]
     */
    bool getMediaContrast( uint8_t &contrast );
    void setMediaContrast( uint8_t contrast );

    bool canControl();

private:
	void release();

private:

	mpc::nsdk::IVideoEffectControler  *m_pVidEffCtrl;
};

#endif // IMGQLTCONTROLLER_HPP_

