#ifndef ALARMREGRECORD_HPP_
#define ALARMREGRECORD_HPP_

#include "ResourceTreeWidget.hpp"

struct AlarmRegisterItem
{
	mpc::nsdk::SResId alarmID;
	std::string       alarmName; 
	std::set< mpc::nsdk::IAlarmController::EAlarmNotifyType > type;
};

typedef std::map< mpc::nsdk::IManageSession*, std::vector<AlarmRegisterItem> >  MulSessionAlarmResRegistered;

class CAlarmRegRecord
{

public:

	static CAlarmRegRecord& getRecord();

	MulSessionAlarmResRegistered& getContainer(); 

	bool addRegister(  mpc::nsdk::IManageSession* pSession, 
					   mpc::nsdk::SResId& id, 
					   std::string& name,
					   mpc::nsdk::IAlarmController::EAlarmNotifyType type );

	bool delRegister( mpc::nsdk::IManageSession* pSession, 
					  mpc::nsdk::SResId& id, 
					  mpc::nsdk::IAlarmController::EAlarmNotifyType type );

	void getAlarmRegistered( mpc::nsdk::IManageSession* pSession,
							 std::vector<AlarmRegisterItem>& vecAlarm );

	bool getAlarmNameFromId( mpc::nsdk::SResId& id, std::string& name );

    // 登录cms成功后添加一个session
    void addRegisterBySession( mpc::nsdk::IManageSession* pSession );

	// cms掉线后，登出，掉线后删除
	void delRegisterBySession( mpc::nsdk::IManageSession* pSession );

	// 
	static void release();

private:

	CAlarmRegRecord(){}

	MulSessionAlarmResRegistered  m_mulSessionAlarmResRegistered;

	static CAlarmRegRecord* m_alarmRecord;

};



#endif   // ALARMREGRECORD_HPP_
