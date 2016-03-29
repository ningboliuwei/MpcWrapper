#include "AlarmRegRecord.hpp"

CAlarmRegRecord* CAlarmRegRecord::m_alarmRecord = NULL;

CAlarmRegRecord& CAlarmRegRecord::getRecord()
{
	if ( NULL == m_alarmRecord )
	{
		m_alarmRecord = new CAlarmRegRecord();
	}

	return *m_alarmRecord;
}

MulSessionAlarmResRegistered& CAlarmRegRecord::getContainer()
{
	return m_mulSessionAlarmResRegistered;
}

bool CAlarmRegRecord::addRegister( mpc::nsdk::IManageSession* pSession, 
								   mpc::nsdk::SResId& id,
								   std::string& name,
								   mpc::nsdk::IAlarmController::EAlarmNotifyType type )
{
	MulSessionAlarmResRegistered& container = getContainer();
	MulSessionAlarmResRegistered::iterator it = container.find( pSession );
	if ( it == container.end() )
	{
		return false;
	}

	// 查找ID
	std::vector<AlarmRegisterItem>::iterator itemIt = it->second.begin();
	for( ; itemIt != it->second.end(); itemIt++  )
	{
		if ( ! memcmp( itemIt->alarmID.id, id.id, sizeof( id.id ) ) )
		{
			break;
		}
	}

	// 没有找到添加
	if ( itemIt == it->second.end() )
	{
		AlarmRegisterItem regItem;
		memcpy( regItem.alarmID.id, id.id, sizeof(id.id) );
		regItem.alarmName = name;
		regItem.type.insert( type );

		it->second.push_back( regItem );
	}
	else
	{
		itemIt->type.insert( type );
	}

	return true;
}

bool CAlarmRegRecord::delRegister( mpc::nsdk::IManageSession* pSession, 
								   mpc::nsdk::SResId& id, 
								   mpc::nsdk::IAlarmController::EAlarmNotifyType type )
{
	MulSessionAlarmResRegistered& container = getContainer();
	MulSessionAlarmResRegistered::iterator it = container.find( pSession );
	if ( it == container.end() )
	{
		return false;
	}

	// 查找ID
	std::vector<AlarmRegisterItem>::iterator itemIt = it->second.begin();
	for( ; itemIt != it->second.end(); itemIt++  )
	{
		if ( ! memcmp( itemIt->alarmID.id, id.id, sizeof( id.id ) ) )
		{
			break;
		}
	}

	// 没有找到添加
	if ( itemIt != it->second.end() )
	{
		std::set< mpc::nsdk::IAlarmController::EAlarmNotifyType >::iterator itType;
		for ( itType = itemIt->type.begin(); itType != itemIt->type.end(); itType ++ )
		{
			if ( (*itType) == type )
			{
				itemIt->type.erase( itType );
				if ( itemIt->type.size() == 0 )
				{
					it->second.erase( itemIt );
				}
				return true;
			}
		}
	}

	return false;
}

void CAlarmRegRecord::getAlarmRegistered( mpc::nsdk::IManageSession* pSession, 
										  std::vector<AlarmRegisterItem>& vecAlarm )
{
	vecAlarm.clear();
	MulSessionAlarmResRegistered& container = getContainer();
	MulSessionAlarmResRegistered::iterator it = container.find( pSession );
	if ( it != container.end() )
	{
		vecAlarm = it->second;
	}	
}

bool CAlarmRegRecord::getAlarmNameFromId( mpc::nsdk::SResId& id, std::string& name )
{
	MulSessionAlarmResRegistered& container = getContainer();
	MulSessionAlarmResRegistered::iterator containerIt;
	for ( containerIt = container.begin(); containerIt != container.end(); containerIt++ )
	{
		std::vector<AlarmRegisterItem>::iterator registerIt;
		for ( registerIt = containerIt->second.begin(); 
			registerIt != containerIt->second.end(); registerIt++ )
		{
			if ( !memcmp( registerIt->alarmID.id, id.id, sizeof( id.id ) ) )
			{
				name = registerIt->alarmName;
				return true;
			}
		}
	}
	return false;
}

void CAlarmRegRecord::delRegisterBySession( mpc::nsdk::IManageSession* pSession )
{
	MulSessionAlarmResRegistered& container = getContainer();
	MulSessionAlarmResRegistered::iterator it = container.find( pSession );
	if ( it != container.end() )
	{
		container.erase( it );
	}
}

void CAlarmRegRecord::release()
{
	if ( NULL != m_alarmRecord )
	{
		delete m_alarmRecord;
		m_alarmRecord = NULL;
	}
}

void CAlarmRegRecord::addRegisterBySession( mpc::nsdk::IManageSession* pSession )
{
    MulSessionAlarmResRegistered& container = getContainer();

    std::vector<AlarmRegisterItem> vectRegItem;
    container.insert( std::map< mpc::nsdk::IManageSession*, 
                      std::vector<AlarmRegisterItem> >::value_type( pSession, vectRegItem ) );
}

