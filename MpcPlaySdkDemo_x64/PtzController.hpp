#ifndef PTZCONTROLLER_HPP_
#define PTZCONTROLLER_HPP_

#include "MpcPlaySdkItf.hpp"
#include "MpcNetSdkItf.hpp"
#include <string>

class CPtzController
{
public:
	CPtzController();
	~CPtzController();

	bool init( mpc::nsdk::IPtzController  *ptzController );
    bool canControl();

public:
	mpc::nsdk::IPtzController  *m_ptzController;

	void move( const mpc::nsdk::IPtzController::EPtzMove oper, const mpc::nsdk::IPtzController::EPtzSpeed speed );
	void focus( const mpc::nsdk::IPtzController::EPtzFocus oper );
	void zoom( const mpc::nsdk::IPtzController::EPtzZoom oper );
    void iris( const mpc::nsdk::IPtzController::EPtzIris oper ); 
    void heat( const bool on );
    void fan( const bool on );
	void light( const bool on );
	void wiper( const bool on );
	void preset( const char* name );
	void delPre( const char* name );
	int getIndex();	//获取一个可用的预置位索引，失败返回-1
	void addPre( const char* name );
	void getPresetName( std::string** name, int& size );

private:
	void release();

private:
	mpc::nsdk::IInfoPreset* m_infoPreset;
};

#endif // PTZCONTROLLER_HPP_

