#ifndef ALARMCONTROLLER_HPP_
#define ALARMCONTROLLER_HPP_
 
#include <QtCore/QObject>
#include <QtCore/QVector>
#include <QtGui/QApplication>
#include "../include/Common.hpp"
#include "../include/SysTypeDefine.hpp"
#include "../include/CustomEvent.hpp"
#include "AlarmRegRecord.hpp"

class CAlarmNotify;
struct SAlarmNotifyData;

////////////////////////////////////////////////////////////
//////////////////�澯����������////////////////////////////
////////////////////////////////////////////////////////////

typedef std::pair< mpc::nsdk::IAlarmController*, CAlarmNotify*> AlarmControlItem;

class CAlarmController
{
public:

    CAlarmController();
    ~CAlarmController();

	bool init( mpc::nsdk::IManageSession* pSession, QObject* object, QString& sessionLabel );
	bool fini( mpc::nsdk::IManageSession* session = NULL );

	bool getAlarmNameFromID( mpc::nsdk::SResId& id, std::string& name );

	// �澯��Դ�Ƿ񱻶���
	bool isRegestered(  mpc::nsdk::SResId id, mpc::nsdk::IAlarmController::EAlarmNotifyType type );

	mpc::nsdk::IManageSession* getSession() { return m_pSession; }
	mpc::nsdk::IAlarmController* getController() { return m_pAlarmController; }
	QVector<SAlarmResInfo>& getAlarmRes() { return m_vecAlarmRes; }
	// QVector<SAlarmNotifyData>& getNotifyData() { return m_vecNotifyData; }  
	std::vector<AlarmRegisterItem>& getAlarmRegistered() { return m_vecAlarmRegistered; }

    // ɾ��ͨ�����������ĵĸ澯
    void getSessionByController( mpc::nsdk::IAlarmController* controller, 
                                 std::vector<mpc::nsdk::IManageSession*>& vecSession );

private:

	void getAlarmRes( mpc::nsdk::IMediaIterator *it, 
		   		      int deep, 
					  std::string deviceName, 
					  QVector<SAlarmResInfo>& vecAlarmRes );

	void release();

private:
	
	mpc::nsdk::IManageSession*     m_pSession;            // ��ǰsession
	QVector<SAlarmResInfo>         m_vecAlarmRes;         // ��ǰsession�еĸ澯��Դ
	mpc::nsdk::IAlarmController*   m_pAlarmController;    // ��ǰsession�澯������
	std::vector<AlarmRegisterItem> m_vecAlarmRegistered;  // ��ǰsession������Ϣ

	std::map<mpc::nsdk::IManageSession*, AlarmControlItem>  m_mulSessionNotify;   // ��������session��֪ͨ���ն��� 
	
	//QVector<SAlarmNotifyData>     m_vecNotifyData;  // �������н��յ���֪ͨ
};

//////////////////////////////////////////////////////////////
/////////////////////�澯֪ͨ���Ͷ���/////////////////////////
//////////////////////////////////////////////////////////////
struct SAlarmNotifyData
{
   
	QString                    sessionLabel;                     // cms ����
	mpc::nsdk::SAlarmRecordId  recordId;                         // �澯��¼id
	mpc::nsdk::SResId		   alarmResId;                       // �澯��Դid
	mpc::nsdk::IAlarmController::EAlarmType       alarmType;     // �澯���ͣ�����������Ƶ��ʧ
    mpc::nsdk::IAlarmController::EAlarmClearType  clearType;     // ������ͣ�notifyTypeΪ���ʱ���˲�����Ч 
	mpc::nsdk::IAlarmController::EAlarmNotifyType notifyType;    // �澯֪ͨ���ͣ�������ȷ�Ϻ����

    std::string                  userName;              // �澯������û���
    uint32_t                     time;                  // �����ȷ��ʱ�䣺��notifyType�й�
    mpc::nsdk::IAlarmController *alarmController;       // �澯����ָ��
};

typedef CCustomEvent<SAlarmNotifyData> CAlarmNotifyEvent;

class CAlarmNotify : public mpc::nsdk::IAlarmNotify
{

public:
	CAlarmNotify( QObject* desObj, QString& label )
		:  m_desObj( desObj ), m_sessionLabel( label ){};

	~CAlarmNotify(){};

	virtual bool alarmActiveNotify(
		const mpc::nsdk::SAlarmRecordId &alarmRecordId, 
		const mpc::nsdk::SResId         &alarmResId,
		const mpc::nsdk::IAlarmController::EAlarmType alarmType,
		const uint32_t activeTime )
	{ 
		if ( !m_desObj )
			return false;

		SAlarmNotifyData data;
		data.sessionLabel = m_sessionLabel;
		data.recordId     = alarmRecordId;
		data.alarmResId	  = alarmResId;
		data.alarmType	  = alarmType;
		data.time		  = activeTime;
		data.notifyType	  = mpc::nsdk::IAlarmController::ACTIVE;
		printResId( alarmRecordId );
		return postEvent( AMS_ALARM_CLEAN_EVENT, data );
	}

	virtual bool alarmCleanNotify(
		const mpc::nsdk::SAlarmRecordId &alarmRecordId, 
		const mpc::nsdk::SResId &alarmResId,
		const mpc::nsdk::IAlarmController::EAlarmType alarmType,
		const uint32_t clearTime,
        const mpc::nsdk::IAlarmController::EAlarmClearType clearType,
		const char *userName,
		const size_t userNameLen )
	{
		if ( !m_desObj )
			return true;

		SAlarmNotifyData data;
		data.sessionLabel = m_sessionLabel;
		data.recordId     = alarmRecordId;
		data.alarmResId	  = alarmResId;
		data.alarmType	  = alarmType;
		data.time		  = clearTime;
		data.notifyType	  = mpc::nsdk::IAlarmController::CLEAR;
		data.clearType    = clearType;
		data.userName.assign( userName, userNameLen );
		return postEvent( AMS_ALARM_CLEAN_EVENT, data );
	}

    virtual void alarmControllerTerminate( mpc::nsdk::IAlarmController *alarmController )
    {
        std::cout << "CAlarmNotify::alarmControllerTerminate(" << alarmController << ")" << std::endl;
//         if (alarmController)
//         {
//             alarmController->release();
//         }

        SAlarmNotifyData data;
        data.alarmController = alarmController;
        postEvent( AMS_OFF_LINE, data );
    }

	virtual bool alarmConfirmNotify(
        const mpc::nsdk::SAlarmRecordId &alarmRecordId,
        const mpc::nsdk::SResId &alarmResId,
        const mpc::nsdk::IAlarmController::EAlarmType alarmType,
        const uint32_t confirmTime,
		const char *userName,
		const size_t userNameLen )
		{
			if ( !m_desObj )
				return true;

			SAlarmNotifyData data;
			data.sessionLabel = m_sessionLabel;
			data.recordId     = alarmRecordId;
			data.alarmResId	  = alarmResId;
			data.alarmType	  = alarmType;
			data.time		  = confirmTime;
			data.notifyType   = mpc::nsdk::IAlarmController::CONFIRM;
            data.userName.assign( userName, userNameLen );
			return postEvent( AMS_ALARM_CONFIRM_EVENT, data );
		}

private:

	QObject* m_desObj;
	QString  m_sessionLabel;

    bool postEvent( const QEvent::Type t, const SAlarmNotifyData& data )
	{
		CAlarmNotifyEvent* event = new CAlarmNotifyEvent( t );
		if ( event )
		{
			event->setData( data );
			QApplication::postEvent( m_desObj, event );
			return true;
		}
		return false;
	}
};

#endif    // ALARMCONTROLLER_HPP_


 
