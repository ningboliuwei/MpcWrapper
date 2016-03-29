#include "PtzController.hpp"
#include <string.h>
#include <set>

CPtzController::CPtzController(): m_ptzController( 0 ), m_infoPreset( 0 )
{}

CPtzController::~CPtzController()
{
	release();
}

void CPtzController::release()
{
	if ( m_infoPreset )
	{
		m_infoPreset->release();
		m_infoPreset = 0;
	}
}

bool CPtzController::init( mpc::nsdk::IPtzController  *ptzController )
{
	release();

	m_ptzController = ptzController;

	//if ( m_ptzController )
	//{
	//	m_ptzController->preset( &m_infoPreset );
	//}
	
	return true;
}

void CPtzController::move( const mpc::nsdk::IPtzController::EPtzMove oper, 
						  const mpc::nsdk::IPtzController::EPtzSpeed speed )
{
	if ( m_ptzController )
	{
		m_ptzController->move( oper, speed );
	}
}

void CPtzController::focus( const mpc::nsdk::IPtzController::EPtzFocus oper )
{
	if ( m_ptzController )
	{
		m_ptzController->focus( oper );
	}
}

void CPtzController::zoom( const mpc::nsdk::IPtzController::EPtzZoom oper )
{
	if ( m_ptzController )
	{
		m_ptzController->zoom( oper );
	}
}

void CPtzController::iris( const mpc::nsdk::IPtzController::EPtzIris oper )
{
	if ( m_ptzController )
	{
		m_ptzController->iris( oper );
	}
}

void CPtzController::light( const bool on )
{
	if ( m_ptzController )
	{
		m_ptzController->light( on );
	}
}

void CPtzController::wiper( const bool on )
{
	if ( m_ptzController )
	{
		m_ptzController->wiper( on );
	}
}

void CPtzController::preset( const char* name )
{
	if ( m_ptzController )
	{
		release();
		m_ptzController->preset( &m_infoPreset );
		if ( m_infoPreset )
		{
			mpc::nsdk::IInfoPreset* tmp = m_infoPreset;
			while( tmp )
			{
				size_t len = 0 ;
				char* getName = 0;
				getName = (char*) tmp->name( len );
				if ( getName )
				{
					if ( !strcmp(getName, name) )
					{
						m_ptzController->preset( tmp->idx() );
					}
				}

				tmp = tmp->next();
			}
		}
	}
}

void CPtzController::delPre( const char* name )
{
	if ( m_ptzController )
	{
		release();
		m_ptzController->preset( &m_infoPreset );
		if ( m_infoPreset )
		{
			mpc::nsdk::IInfoPreset* tmp = m_infoPreset;
			while( tmp )
			{
				size_t len = 0 ;
				char* getName = 0;
				getName = (char*) tmp->name( len );
				if ( getName )
				{
					if ( !strcmp(getName, name) )
					{
						m_ptzController->delPreset( tmp->idx() );
						break;
					}
				}

				tmp = tmp->next();
			}
		}
	}
}

int CPtzController::getIndex()
{
	if ( !m_ptzController )
		return -1;

	release();
	m_ptzController->preset( &m_infoPreset );
	
	if ( m_infoPreset )
	{
		mpc::nsdk::IInfoPreset* tmp = m_infoPreset;
		std::set<uint8_t> idxIfno;
		while( tmp )
		{
			idxIfno.insert(tmp->idx());
			tmp = tmp->next();
		}
		for (uint8_t i = 0; i < 10; ++i)
		{
			if (idxIfno.end() == idxIfno.find(i))
				return i;
		}
		return -1;
		
	}

	return 0;
}
void CPtzController::addPre( const char* name )
{
	if ( !m_ptzController )
		return;

	int index = getIndex();
	if (-1 == index )
		return;
	m_ptzController->addPreset( name, std::strlen(name), index);
}

void CPtzController::getPresetName( std::string** name, int& size )
{
	size = 0;
	if ( m_ptzController )
	{
		release();
		m_ptzController->preset( &m_infoPreset );
		mpc::nsdk::IInfoPreset* tmp = m_infoPreset;

		int count = 0;
		while( tmp )
		{
			count ++;
			tmp = tmp->next();
		}

		if ( !count )
		{
			return;
		}

		std::string* tmpName;
		tmpName = new std::string[count];
		if ( !tmpName )
		{
			return ;
		}

		tmp = m_infoPreset;
		while( tmp )
		{
			size_t len = 0;
			tmpName[size] = std::string( tmp->name( len ) );
			size ++;
			tmp = tmp->next();
		}

		*name = tmpName;
	}
}

bool CPtzController::canControl()
{
	return ( NULL != m_ptzController );
}

void CPtzController::heat( const bool on )
{
    if ( m_ptzController )
        m_ptzController->heat( on );
}

void CPtzController::fan( const bool on )
{
    if ( m_ptzController )
        m_ptzController->fan( on );
}







