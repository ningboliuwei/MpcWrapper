#include "VideoEffectControler.hpp"
#include <string.h>

CVideoEffectControler::CVideoEffectControler(): m_pVidEffCtrl( NULL )
{}

CVideoEffectControler::~CVideoEffectControler()
{
	release();
}

void CVideoEffectControler::release()
{
	if ( NULL != m_pVidEffCtrl )
	{
		m_pVidEffCtrl->release();
		m_pVidEffCtrl = NULL;
	}
}

bool CVideoEffectControler::init( mpc::nsdk::IVideoEffectControler *pImgCtrl )
{
	release();

	m_pVidEffCtrl = pImgCtrl;
	return true;
}

bool CVideoEffectControler::getMediaHue( uint8_t &hue )
{
	if ( NULL != m_pVidEffCtrl )
	{
		return m_pVidEffCtrl->getHue( hue );
	}

	return false;
}

void CVideoEffectControler::setMediaHue( uint8_t hue )
{
	if ( NULL != m_pVidEffCtrl )
	{
		m_pVidEffCtrl->setHue( hue );
	}
}

bool CVideoEffectControler::getMediaBrightness( uint8_t &brightness )
{
	if ( NULL != m_pVidEffCtrl )
	{
		return m_pVidEffCtrl->getBrightness( brightness );
	}

	return false;
}

void CVideoEffectControler::setMediaBrightness( uint8_t brightness )
{
	if ( NULL != m_pVidEffCtrl )
	{
		m_pVidEffCtrl->setBrightness( brightness );
	}
}

bool CVideoEffectControler::getMediaSaturation( uint8_t &saturation )
{
	if ( NULL != m_pVidEffCtrl )
	{
		return m_pVidEffCtrl->getSaturation( saturation );
	}

	return false;
}

void CVideoEffectControler::setMediaSaturation( uint8_t saturation )
{
	if ( NULL != m_pVidEffCtrl )
	{
		m_pVidEffCtrl->setSaturation( saturation );
	}
}

bool CVideoEffectControler::getMediaContrast( uint8_t &contrast )
{
	if ( NULL != m_pVidEffCtrl )
	{
		return m_pVidEffCtrl->getContrast( contrast );
	}

	return false;
}

void CVideoEffectControler::setMediaContrast( uint8_t contrast )
{
	if ( NULL != m_pVidEffCtrl )
	{
		m_pVidEffCtrl->setContrast( contrast );
	}
}

bool CVideoEffectControler::canControl()
{
	return ( NULL != m_pVidEffCtrl );
}


