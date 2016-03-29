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

    // ��¼cms�ɹ������һ��session
    void addRegisterBySession( mpc::nsdk::IManageSession* pSession );

	// cms���ߺ󣬵ǳ������ߺ�ɾ��
	void delRegisterBySession( mpc::nsdk::IManageSession* pSession );

	// 
	static void release();

private:

	CAlarmRegRecord(){}

	MulSessionAlarmResRegistered  m_mulSessionAlarmResRegistered;

	static CAlarmRegRecord* m_alarmRecord;

};



#endif   // ALARMREGRECORD_HPP_
