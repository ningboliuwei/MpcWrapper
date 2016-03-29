#include "AlarmController.hpp"

CAlarmController::CAlarmController() 
: m_pSession( NULL ) 
, m_pAlarmController( NULL )
{
	m_mulSessionNotify.clear();
}

CAlarmController::~CAlarmController()
{
	release();
}

// 释放session切换时要释放的对象
void CAlarmController::release()
{
	m_pSession = NULL;

	// 清除显示资源
	m_vecAlarmRes.clear();
}

bool CAlarmController::init( mpc::nsdk::IManageSession* pSession, QObject* object, QString& sessionLabel )
{
	this->release();
	m_pSession = pSession;
	m_vecAlarmRes.clear();
	if ( NULL != pSession )
	{
		// 初始化资源
		mpc::nsdk::IAlarmIterator *first = NULL;
		first = pSession->createAlarmIterator();
		if ( first )
		{    
			getAlarmRes( first, 0, "", m_vecAlarmRes );
            std::cout << std::endl;
			first->release();
			first = NULL;
		}

		// 初始化控制器
		CAlarmNotify* notify = NULL;
		std::map<mpc::nsdk::IManageSession*, AlarmControlItem>::iterator it;
		it = m_mulSessionNotify.find( pSession );
		if ( it == m_mulSessionNotify.end() )  // 不存在
		{
			m_pAlarmController = pSession->createAlarmController();
			if ( NULL != m_pAlarmController )
			{
				notify = new CAlarmNotify( object, sessionLabel );
				m_pAlarmController->setAlarmNotifyHandler( notify );
			}
			AlarmControlItem controlItem( m_pAlarmController, notify );
			m_mulSessionNotify.insert( std::map<mpc::nsdk::IManageSession*, AlarmControlItem>::value_type(pSession, controlItem) );
		}
		else
		{
			if( NULL != it->second.first )
			{
				m_pAlarmController = it->second.first;
				if ( NULL != it->second.second )
				{
					return true;
				}
				else
				{
					notify = new CAlarmNotify( object, sessionLabel );
					it->second.first->setAlarmNotifyHandler( notify );
					it->second.second = notify;
				}
			}
			else
			{
				m_pAlarmController = pSession->createAlarmController();
				if ( NULL != m_pAlarmController )
				{
					notify = new CAlarmNotify( object, sessionLabel );
					m_pAlarmController->setAlarmNotifyHandler( notify );
				}
				AlarmControlItem controlItem( m_pAlarmController, notify );
				m_mulSessionNotify.insert( std::map<mpc::nsdk::IManageSession*, AlarmControlItem>::value_type(pSession, controlItem) );
			}
		}
	}

	return true;
}

bool CAlarmController::fini( mpc::nsdk::IManageSession* session )
{
    if ( (session == m_pSession && session != NULL) || (session == NULL)  )
	    release();

	mpc::nsdk::IAlarmController* alarmCtrl = NULL;
	std::map<mpc::nsdk::IManageSession*, AlarmControlItem>::iterator it;
	if ( NULL == session )  // 释放所有订阅操作对象
	{
		for ( it = m_mulSessionNotify.begin(); it != m_mulSessionNotify.end();)
		{
			alarmCtrl = it->second.first;
			if ( NULL == alarmCtrl )
			{
				if ( NULL != it->second.second )
				{
					delete it->second.second;
					it->second.second = NULL;
				}
			}
			else
			{
				alarmCtrl->setAlarmNotifyHandler( NULL );
				alarmCtrl->release();
				alarmCtrl = NULL;
                it->second.first = NULL;

				if ( NULL != it->second.second )
				{
					delete it->second.second;
					it->second.second = NULL;
				}
			}

            it = m_mulSessionNotify.erase(it);
		}

        if ( NULL != m_pAlarmController )  // 置空
            m_pAlarmController = NULL;
	}
	else  // 当前传入的session
	{
		it = m_mulSessionNotify.find( session );
		if ( it != m_mulSessionNotify.end() )
		{
			alarmCtrl = it->second.first;
			if ( NULL == alarmCtrl )
			{
				if ( NULL != it->second.second )
				{
					delete it->second.second;
					it->second.second = NULL;
				}
			}
			else
			{
                if ( alarmCtrl == m_pAlarmController )  // 置空
                    m_pAlarmController = NULL;

				alarmCtrl->setAlarmNotifyHandler( NULL );
				alarmCtrl->release();
				alarmCtrl = NULL;
                it->second.first = NULL;

				if ( NULL != it->second.second )
				{
					delete it->second.second;
					it->second.second = NULL;
				}
			}

            m_mulSessionNotify.erase(it);
		}
	}

	return true;
}

void CAlarmController::getAlarmRes( mpc::nsdk::IAlarmIterator *it, 
								    int deep, 
								    std::string deviceName, 
								    QVector<SAlarmResInfo>& vecAlarmRes )
{
	mpc::nsdk::IAlarmNode     *self = NULL;
	mpc::nsdk::IAlarmIterator *curr = NULL;
	mpc::nsdk::IAlarmIterator *next = NULL;
	mpc::nsdk::IAlarmIterator *down = NULL;
	mpc::nsdk::IAlarmRes      *resPtr = NULL;

	size_t	strSize = 0;

	curr = it;

	do 
	{
#if 1 //def _DEBUG
        std::cout << std::endl;
        for( int i = 0; i < deep; ++i )
        {
            std::cout << "--|";
        }
#endif
		self = curr->self();
		next = curr->next();
		down = curr->down();

		SAlarmResInfo info;
		if( !self->isRes() )
		{
			deviceName = std::string( self->label( strSize ) );

#ifdef _DEBUG
			std::cout << "dir: " << deviceName;
#endif

		}
		else
		{
			resPtr = dynamic_cast< mpc::nsdk::IAlarmRes* > ( self );

			info.pos        = deep - 1;
			info.deviceName = deviceName;
			info.alarmId    = resPtr->msid();
			info.alarmName  = std::string(self->label( strSize ));
            info.alarmClearMode = resPtr->getAlarmClearMode();
            fillAllExtInfo(resPtr,info);
			vecAlarmRes.push_back( info );

#ifdef _DEBUG
			std::cout << "res: " << info.alarmName << " " << info.alarmName.length();
			printResId( info.alarmId );
#endif

		}

		if( down )
		{
			deep ++;
			if ( !self->isRes() )
			{
				getAlarmRes( down, deep, deviceName, vecAlarmRes );
			}
			else
			{
				getAlarmRes( down, deep, deviceName, vecAlarmRes );
			}
		}

		if ( next )
		{
			curr = next;
		}

	} while ( next );
}

bool CAlarmController::getAlarmNameFromID( mpc::nsdk::SResId& id, std::string& name )
{
	QVector<SAlarmResInfo>::iterator it = m_vecAlarmRes.begin();
	for ( ; it != m_vecAlarmRes.end(); it++ )
	{
		if ( !memcmp( &it->alarmId, &id, sizeof(id) ) )
		{
			name = it->alarmName;
			return true;
		}
	}

	return false;
}

bool CAlarmController::isRegestered( mpc::nsdk::SResId id, mpc::nsdk::IAlarmController::EAlarmNotifyType type )
{
	std::vector<AlarmRegisterItem>::iterator it;
	for ( it = m_vecAlarmRegistered.begin(); it != m_vecAlarmRegistered.end(); it++ )
	{
		if ( !memcmp( it->alarmID.id, id.id, sizeof(id) ) )
		{
			if ( it->type.find( type ) != it->type.end() )
			{
				return true;
			}
		}
	}

	return false;
}

void CAlarmController::getSessionByController( mpc::nsdk::IAlarmController* controller, 
                                               std::vector<mpc::nsdk::IManageSession*>& vecSession )
{
    vecSession.clear();
    std::map<mpc::nsdk::IManageSession*, AlarmControlItem>::iterator it = m_mulSessionNotify.begin();
    for ( ; it != m_mulSessionNotify.end(); ++it )
    {
        if ( it->second.first == controller )
        {
            vecSession.push_back( it->first );      
        }
    }
}

