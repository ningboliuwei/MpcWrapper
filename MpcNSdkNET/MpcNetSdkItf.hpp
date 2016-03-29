#ifndef MPCNETSDKITF_HPP_
#define MPCNETSDKITF_HPP_

#if defined( WIN32 ) && defined( _MSC_VER )
#    ifdef MPCNETSDK_IMPL
#        define MPCNETSDK_PUBLIC __declspec( dllexport )
#    else
#        define MPCNETSDK_PUBLIC __declspec( dllimport )
#    endif
#elif defined( __GNUC__ )
#    define MPCNETSDK_PUBLIC //__attribute__((visibility("default")))
#endif

#ifdef interface
#undef interface
#endif
#define interface struct

#if defined( WIN32 ) && defined( _MSC_VER )
#include "mpcsdk/stdint.h"
#else
#include <stdint.h>
#endif

#include <sys/types.h>

namespace mpc { namespace nsdk {

// @{
interface IManageSession;
interface IManageSessionNotify;
interface IPtzController;
interface IWallController;
interface IVideoEffectControler; // ��ƵЧ���ӿ�

interface ILivingStream;
interface IReplayStream;
interface IStreamHandler;
// @}
// @{
interface ITreeNode;
interface ITreeDir;
typedef   ITreeNode       IMediaNode;
typedef   ITreeDir        IMediaDir;
interface IMediaIterator;
interface IInfoPreset;
interface IInfoTimePeriod;
interface IInfoMonitor;
// @}

// @{
typedef   ITreeNode       IAlarmNode;
typedef   ITreeDir        IAlarmDir;
typedef   IMediaIterator  IAlarmIterator;

interface IAlarmController; // �澯����,ȡ�����ģ�ע��֪ͨ
interface IAlarmNotify;     // �澯����֪ͨ,�澯���֪ͨ
// @}

struct SResId
{
    uint8_t id[16];
};
typedef SResId SAlarmRecordId; // �澯��¼��ID

const size_t MAX_DIGEST_SIZE = 128; // �����Դ��ժҪ����.��λ(byte)

/** ����SDK����ʱ����
 */
struct MPCNETSDK_PUBLIC Runtime
{
    /** ��ʼ��
     *
     * ���麯������SDK����ʱ�����ĳ�ʼ��/�����ʼ���������ڵ����κνӿں���֮ǰ������Ҫ����init������
     * ��ʹ�����SDK��������fini���ͷ����е���Դ��okey�����ж��Ƿ��ʼ���ɹ�.
     */
    //@{
    static bool init();
    static bool fini();
    static bool okey();
    //@}
};

/** ҵ����󴴽�����
 */
struct MPCNETSDK_PUBLIC Factory
{
    /** ��������Ự
     * �û�ָ���������ĵ�ַ,�û���,����,������һ������Ự. �û�ͨ������Ự�����е�¼/ȡ��/��̨/�л�.
     * ע��:�����Ĺ���Ự��Ҫ�ֶ��ĵ���IManageSession��release,���ͷ�.
     *      ��ͬ��addr+port,ֻ�ܴ���һ��IManageSession�Ķ���.
     *
     * @param[in] addr ������IP��ַ
     * @param[in] port �������˿ں�
     * @param[in] username ��¼���������û���
     * @param[in] usernameSize ��¼���������û��ַ�������(byte).
     * @param[in] password ��¼����������������/md5password ��¼��������MD5����
     * @param[in] passwordSize ��¼�����������볤��(byte).
     *
     * @return ����Ự����ӿ�ָ��.
     */
    static IManageSession *createManageSession( const uint32_t addr, const uint16_t port, 
                                                const char *username, const size_t usernameSize,
                                                const char *password, const size_t passwordSize);

    static IManageSession *createManageSession( const uint32_t addr, const uint16_t port, 
                                                const char *username, const size_t usernameSize,
                                                const uint8_t *md5password, const size_t passwordSize);


};

/** ����Ự�ӿ�
 *
 *  ���г�Ա�ӿ���ʹ�õ��� msid ������ͨ�� IMediaIterator ������ԴĿ¼����ȡ����
 *  ��ȡ IMediaIterator ��ʹ�� createMediaIterator �ӿڡ�
 *
 */
interface MPCNETSDK_PUBLIC IManageSession
{
    /** ��ʼ��
     *
     * ʹ�øýӿ�֮ǰ����Ҫ���ó�ʼ��������ͬʱ��ʹ����ɺ����Ҫ���÷����ʼ������.
     */
    //@{
    virtual bool init() = 0;
    virtual bool fini() = 0;
    virtual bool okey() = 0;
    //@}

    /** �û�������¼�������ǳ�����.
     * 
     * ������¼:����������֮�佨������¼.
     * �����ǳ�:����������֮��ǳ�������.
     * �ڵ��ߺ�,�û��ᱻ���ǳ�,�����ǳ���Ӧ����sdk��ʹ���������ٴε���������¼.
     * 
     * @param[in] ��
     * 
     * @return bool
     */
    //@{
    virtual bool login() = 0;
    virtual bool logout() = 0;
    //@}

    /** �û��޸��Լ�������.
     * 
     * �û���¼�ɹ�֮��,�ſ����޸��Լ�������.
     * 
     * @param[in] curPwd: �û���ǰ����/md5curPwd: ����MD5���ܺ���û���ǰ����.
     * @param[in] curPwdSize: �û���ǰ����ĳ���.
     * @param[in] newPwd: �û����õ�������/md5newPwd: ����MD5���ܺ���û����õ�������.
     * @param[in] newPwdSize: �û�������ĳ���.
     * 
     * @return bool
     */
    virtual bool updatePwd( const char *curPwd, const size_t curPwdSize,
                            const char *newPwd, const size_t newPwdSize ) = 0;

    virtual bool updatePwd( const uint8_t *md5curPwd, const size_t curPwdSize,
                            const uint8_t *md5newPwd, const size_t newPwdSize ) = 0;
    // �û��޸��Լ�������֮��, ����Ҫ���µ�¼.

    /** ���Ĺ���Ự�쳣��Ϣ
     * �û���Ҫ��֤�ڱ�IManageSession��������ڼ�,notify�����Ǵ��ڵ�.
     * ���Բ�����.
     */
    virtual void subscribe( IManageSessionNotify *notify ) = 0;

    /** ��ѯý��ԴժҪ.
     * �������ժҪ��Ϣ��ͬ,�����ý��Դ�б仯.
     * @param[out] digestBuf��      ����ժҪ��Ϣ�Ķ����ƻ�����ͷָ��
     * @param[in]  digestBufSize��  ����ժҪ��Ϣ�Ķ����ƻ��������ȣ���󻺳�������Ϊ MAX_DIGEST_SIZE��
     * @param[out] digestRetSize:   ���ص�ժҪ��Ϣ����(byte).
     *
     * @return �Ƿ�ɹ�
     */
    virtual bool getMediaDigest( uint8_t *digestBuf, const size_t digestBufSize, size_t &digestRetSize ) = 0;

    /** ��ȡý��Դ������.
     * ���û���ʹ��ʱ,��Ҫ����IMediaIterator��release����.
     */
    virtual IMediaIterator *createMediaIterator() = 0;

    /** ��ѯ�澯ԴժҪ.
     * �������ժҪ��Ϣ��ͬ,�����ý��Դ�б仯.
     * @param[out] digestBuf��      ����ժҪ��Ϣ�Ķ����ƻ�����ͷָ��
     * @param[in]  digestBufSize��  ����ժҪ��Ϣ�Ķ����ƻ��������ȣ���󻺳�������Ϊ MAX_DIGEST_SIZE��
     * @param[out] digestRetSize:   ���ص�ժҪ��Ϣ����(byte).
     *
     * @return �Ƿ�ɹ�
     */
    virtual bool getAlarmDigest( uint8_t *digestBuf, const size_t digestBufSize, size_t &digestRetSize ) = 0;

    /** ��ȡ�澯Դ������.
     * ���û���ʹ��ʱ,��Ҫ����IMediaIterator��release����.
     */
    virtual IAlarmIterator *createAlarmIterator() = 0;

    /** ����ʵʱ������.
     *
     * �û�ָ��ý��Դ��ID,��ָ����/������,������һ��ʵʱ������.
     *
     * @param[in] msid ý��ԴID.
     * @param[in] sub  ��������(true=������;false=������).
     *
     * @return ʵʱ������ӿ�ָ��.
     */
    virtual ILivingStream *createLivingStream( const SResId &msid, const bool sub ) = 0;
    /** ������ʷ������.
     * 
     * �û�ָ��ý��Դ��ID������һ����ʷ������.
     *
     * @param[in] msid ý��ԴID.
     *
     * @return ��ʷ������ӿ�ָ��.
     */
    virtual IReplayStream *createReplayStream( const SResId &msid ) = 0;

    /** ������̨���ƶ���.
     * 
     * �û�ָ��ý��Դ��ID������һ����̨���ƶ���.
     * ������һ������Ự����̨���ƶ���ʹ�ô˹���Ự��������,����:����˹���Ự�����ǳ�,����Щ��̨���ƶ����Զ�̷��ʶ�ʧ��.
     * 
     * @param[in] msid ý��ԴID.
     *
     * @return ��̨���ƶ���ӿ�ָ��.
     */
    virtual IPtzController *createPtzController( const SResId &msid ) = 0;

    /** ��������ǽ���ƶ���.
     * 
     * ������һ������Ự�ĵ���ǽ���ƶ���ʹ�ô˹���Ự��������,����:����˹���Ự�����ǳ�,����Щ����ǽ���ƶ����Զ�̷��ʶ�ʧ��.
     * 
     * @return ����ǽ���ƶ���ӿ�ָ��.
     */
    virtual IWallController *createWallController() = 0;

    /** ������ƵЧ�����ڶ���.
     * 
     * �û�ָ��ý��Դ��ID������һ����ƵЧ�����ڶ���.
     * ������һ������Ự����ƵЧ�����ڶ���ʹ�ô˹���Ự��������,����:����˹���Ự�����ǳ�,����Щ��ƵЧ�����ڶ����Զ�̷��ʶ�ʧ��.
     * 
     * @return ����ǽ���ƶ���ӿ�ָ��.
     */
    virtual IVideoEffectControler *createVideoEffectControler( const SResId &msid ) = 0;

    /** ������ʷ��Ƶʱ���.
     * 
     * �û�ָ��һ��ý��Դ��ID�ͼ����Ŀ�ʼʱ�������ʱ��,���������ʱ���ڴ�ý��Դ����ʷ��Ƶʱ���.
     * 
     * @param[in] msid ý��ԴID.
     * @param[in] beginTime ��ʼʱ��
     * @param[in] endTime ����ʱ��
     * @param[out] tp ý��Դ��ʷ��Ƶʱ����б�.
     *
     */
    virtual void retrieveReplayPeriod( const SResId &msid, const uint32_t beginTime, const uint32_t endTime, IInfoTimePeriod **tp ) = 0;

    /** �����澯���ƶ���.
     * 
     * ������һ������Ự�ĸ澯���ƶ���ʹ�ô˹���Ự��������,����:����˹���Ự�����ǳ�,����Щ�澯���ƶ����Զ�̷��ʶ�ʧ��.
     * 
     * @return �澯���ƶ���ӿ�ָ��.
     */
    virtual IAlarmController *createAlarmController() = 0;

    /** �ͷŹ���Ự����
     * 
     * �û���Ҫ���ô˺������ͷŴ�������.
     */
    virtual void release() = 0;

protected:
    virtual ~IManageSession() {};
};

/** ����Ự�쳣��Ϣ�ӿ�.
 */
interface MPCNETSDK_PUBLIC IManageSessionNotify
{
    /** �Ự��ֹ.
     * 
     * �������Ự����Ӧ���û������ǳ�,��SDK����ô˻Ự��ֹ����,��֪ͨSDK��ʹ�����û������ǳ��¼�����.
     * ʵ��Ҫ��:����Ѹ�ٷ��أ����Һ����ڲ��ܵ��� IManageSession::release() �ͷŸ�session���󣬷�����ܵ���δ֪���쳣��
     *
     * @param[in] session �����û������ǳ��Ĺ���Ự����.
     */
    virtual void terminate( IManageSession *session ) = 0;

    virtual ~IManageSessionNotify() {};
};

/** ��̨���ƽӿ�.
 */
interface MPCNETSDK_PUBLIC IPtzController
{
    enum EPtzMove  { MOVE_STOP  = 0, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };
    enum EPtzFocus { FOCUS_STOP = 0, FOCUS_NEAR, FOCUS_FAR };
    enum EPtzZoom  { ZOOM_STOP  = 0, ZOOM_IN, ZOOM_OUT };
    enum EPtzIris  { IRIS_OPEN = 0, IRIS_CLOSE };
    enum EPtzSpeed { SPEED_SLOWER = 0, SPEED_SLOW, SPEED_NORMAL, SPEED_FAST, SPEED_FASTER };

    /** ��̨�ƶ�.
     *      speed Ϊö��ֵ������������ǿ��ת���������ͣ�������Χ��ο� EPtzSpeed �Ķ���
     */
    virtual void move( const EPtzMove oper, const EPtzSpeed speed ) = 0;
    /** ��ͷ����������(nearǰ��,far���) (���淭����ʾΪ"�Խ�")
     */
    virtual void focus( const EPtzFocus oper ) = 0;
    /** ��ͷ����������(in���ʱ��.out���ʱ�С) (���淭����ʾΪ"����")
     */
    virtual void zoom( const EPtzZoom oper ) = 0;
    /** ��ͷ��������Ȧ(wide=open����,close=tele��С)(���淭����ʾΪ"��Ȧ")
     */
    virtual void iris( const EPtzIris oper ) = 0; 
    /** ���ȶ��������ȿ��������ȹر�
     */
    virtual void heat( const bool on ) = 0;
    /** ���ȶ��������ȿ��������ȹر�
     */
    virtual void fan( const bool on ) = 0;
    /** ��ˢ����.
     */
    virtual void wiper( const bool on ) = 0;
    /** �ƹ⿪��
     */
    virtual void light( const bool on ) = 0;

	/** ���Ԥ��λ.
	 * @param[in] buf 	Ԥ��λ�����ַ���
	 * @param[in] len 	Ԥ��λ���Ƴ���
	 * @param[in] index 	Ԥ��λ����.
     */
    virtual bool addPreset( const char *presetName, const size_t presetNameLen, const uint8_t index ) = 0; 

	/** �Ƴ�Ԥ��λ.
	 * @param[in] name Ԥ��λ����.
     */
    virtual bool delPreset( const uint8_t index ) = 0;
	
    /** ��ȡԤ��λ�б�.
     * @param[out] idxList Ԥ��λ�б�.
     */
    virtual void preset( IInfoPreset **idxList ) = 0;
    /** �л�Ԥ��λ.
     */
    virtual void preset( const uint8_t idx ) = 0;

    /** �ͷ���̨���ƶ���.
     * 
     * �û���Ҫ���ô˺������ͷŴ����Ķ���.
     */
    virtual void release() = 0;

protected:
    virtual ~IPtzController() {};
};

/** ����������ӿ�.
 */
interface IStreamHandler
{
    enum EStreamType { ES = 0, PS, TS };

    /** StreamHandler������Ϣ.
     * ʵ��Ҫ��:����Ѹ�ٷ���.
     * ����ʵʱ��,������.
     * ������ʷ��,����������;����ʾ:�ڴ˺�������֮��,�ٽ����޸�ʱ��,�ط�/����,�Ȳ������ǰ�ȫ��.
     */
    virtual void startup() = 0;

    /** ��������ʼ��Ϣ.
     * ʵ��Ҫ��:����Ѹ�ٷ���.
     * @param[in] md ��������.
     */
    virtual void establish( const EStreamType md ) = 0;

    /** ������.
     * ʵ��Ҫ��:����Ѹ�ٷ���.
     * @param[in] buf ���ݻ������׵�ַ.
     * @param[in] len ���ݻ�������С(byte).
     * @param[in] realTime ���ݰ���ʱ�������1970-1-1��ʼ��������.
     */
    virtual void stream( const uint8_t *buf, const size_t len, const uint32_t realTime ) = 0;

    /** ��������ֹ��Ϣ.
     * ʵ��Ҫ��:����Ѹ�ٷ���.
     * establish��terminate�������ǳɶԳ���,�п���û��establish����terminate.
     */
    virtual void terminate() = 0;

    /** StreamHandler�ر���Ϣ.
     * ʵ��Ҫ��:����Ѹ�ٷ���.
     * startup��shutdown�������ǳɶԳ���,�п���û��startup����shutdown.
     */
    virtual void shutdown() = 0;


    /** �ͷ��������������.
     * ʵ��Ҫ��:����Ѹ�ٷ���.
     * �û�ʵ�ִ˺���,���ͷű�����,Ȼ��sdk���ڲ���ҪStreamHandler��ʱ�����һ��release.
     */
    virtual void release() = 0;

    /** ����������������ü�����1.
     * ʵ��Ҫ��:����Ѹ�ٷ���.
     * �û�ʵ�ִ˺���,�Խ����ü�����1.Ȼ��sdk����setHandler��ʱ�����һ��addRef.
     */
    virtual void addRef() = 0;

protected:
    virtual ~IStreamHandler() {};
};

/** ʵʱ���ӿ�.
 * 
 * ������һ�������ɹ�,����Ҫ�û���ʾ�ر�,�����û��ȵ�¼����ȡ������,Ȼ�����������ǳ����Ǳ����ǳ�,����Ӱ����.
 */
interface MPCNETSDK_PUBLIC ILivingStream
{
    /** �����������������.
     * Ϊ��ʵʱ������ָ��һ���������������.
     * 
     * �ɱ��������ý�����streamHandler,sdk���ڲ���Ҫ����ʱ�����һ��release.
     * init�ɹ�֮��,�ٵ��ô˺�����ֱ��ʧ��.
     * 
     * @param[in] handler �������������ָ��.
     */
    virtual bool setHandler( IStreamHandler *streamHandler ) = 0;

    /** ��ʼ��
     *
     * init:��ɽ��������󲥷�.
     * �ڵ���init֮ǰ,��Ҫ����setHandler;
     * ���init�ɹ�,�򲻿����ظ�����,ֻ���ٴ�������һ��ILivingStream��������.
     * ���initʧ��,��������.
     * fini:��ɹ������ͷ��������Դ.
     * okey:�Ƿ��Ѿ�init�ɹ�.
     */
    //@{
    virtual bool init() = 0;
    virtual bool fini() = 0;
    virtual bool okey() = 0;
    //@}

    /** �ͷű�ʵʱ������
     * 
     * �û���Ҫ���ô˺������ͷŴ�������.
     */
    virtual void release() = 0;

protected:
    virtual ~ILivingStream() {};
};

/** ��ʷ���ӿ�
 *
 * ʹ��˵����
 *     ������ڲ��ţ�����ʹ�� download �ӿ�
 *     ����������أ�����ʹ�� play,pause,step,modifySpeed �ӿ�
 */
interface MPCNETSDK_PUBLIC IReplayStream
{
    /** �����������������.
     * Ϊ����ʷ������ָ��һ���������������.
     * 
     * �ɱ��������ý�����streamHandler,sdk���ڲ���Ҫ����ʱ�����һ��release.
     * init�ɹ�֮��,�ٵ��ô˺�����ֱ��ʧ��.
     *
     * @param[in] handler �������������ָ��.
     */
    virtual bool setHandler( IStreamHandler *streamHandler ) = 0;
    /** ��ʼ��
     *
     * init:��ɽ��������󲥷�.
     * �ڵ���init֮ǰ,��Ҫ����setHandler;
     * ���init�ɹ�,�򲻿����ظ�����,ֻ���ٴ�������һ��IReplayStream��������.
     * ���initʧ��,��������.
     * fini:��ɹ������ͷ��������Դ.
     * okey:�Ƿ��Ѿ�init�ɹ�.
     */
    //@{
    virtual bool init() = 0;
    virtual bool fini() = 0;
    virtual bool okey() = 0;
    //@}


    /** ���ſ���:�޸Ĳ���ʱ��.
     * 
     * ע��:����ǻط�,�򵽴����ʱ��ʱ,������滹����ʷ����,�򲻻�ֹͣ.
     *      ���������,�򵽴����ʱ��ͻ�ֹͣ.
     * 
     * @param[in] timeBegin ���ſ�ʼʱ��.(utc)
     * @param[in] timeEnd ���Ž���ʱ��.(utc)
     *
     * ʹ��˵����
     *     init ����ȴ� IStreamHandler::startup ֪ͨ����ܵ��ã�����ᵼ��ʧ��
     */
    virtual bool modifyTimes( const uint32_t timeBegin, const uint32_t timeEnd ) = 0;
    /** ���ſ���:�޸Ĳ����ٶ�.
     * 
     * ע��:�ٶȵ�ȡֵ��Χ��[8��֮һ,8]
     * @param[in] numerator �ٶȷ���,��Χ[1��8].
     * @param[in] denominator �ٶȷ�ĸ,��Χ[1��8].
     *
     * ʹ��˵����
     *     init ����ȴ� IStreamHandler::startup ֪ͨ����ܵ��ã�����ᵼ��ʧ��
     */
    virtual bool modifySpeed( const uint8_t numerator, const uint8_t denominator ) = 0;

    /** ���ſ���:����.
     *
     * ʹ��˵����
     *     ����ǰ�������һ�� modifyTimes ָ����ȷ����ʱ��Σ�����ᵼ��ʧ��
     *     IReplayStream ���� play ֮��Ͳ������� download����֮��Ȼ��
     */
    virtual bool play() = 0;
    /** ���ſ���:��ͣ.
     *
     * ʹ��˵����
     *     play ״̬�²��ܵ��� pause
     */
    virtual bool pause() = 0;
    /** ���ſ���:��֡.
     *
     * ʹ��˵����
     *     pause ״̬�²��ܵ��� step
     */
    virtual bool step() = 0;

    /** ���ſ���:����.
     *
     * ʹ��˵����
     *     IReplayStream ���� download ֮��Ͳ������� play����֮��Ȼ��
     *     ����ǰ�������һ�� modifyTimes ָ����ȷ����ʱ��Σ�����ᵼ��ʧ��
     *     ���øýӿں󣬲����ٵ��� modifyTimes ��
     */
    virtual bool download() = 0;

    /** �ͷű���ʷ������
     * 
     * �û���Ҫ���ô˺������ͷŴ�������.
     */
    virtual void release() = 0;

protected:
    virtual ~IReplayStream() {};
};

/** ����ǽ���ƽӿ�.
 */
interface MPCNETSDK_PUBLIC IWallController
{
    /** ��ȡ�������б�.
     * @param[out] idList �������б�.
     */
    //<< ��Ҫ�޸ģ�ֻ����ǰ����ǽ
    virtual void getMonitorId( IInfoMonitor **idList ) = 0;

    /** ִ�е���
     * 
     * ��ý��Դ�л�����������.
     * 
     * @param[in] monitorId ������ID.
     * @param[in] mediaId ý��ԴID.
     */
    ///<< �޸ģ��������йأ�ֻ�����ڵ�ǰ����
    virtual bool schedule( const SResId &monitorId, const SResId &mediaId ) = 0;

    /** ֹͣ�л�
     * 
     * ֹͣһ�������������е��л�.
     * 
     * @param[in] monitorId ������ID.
     */
    ///<< �޸ģ��������йأ�ֻ�����ڵ�ǰ����
    virtual bool stop( const SResId &monitorId ) = 0;

    /** �ͷŵ���ǽ���ƶ���.
     * 
     * �û���Ҫ���ô˺������ͷŴ����Ķ���.
     */
    virtual void release() = 0;

    ///<< ��ӽӿ�
    virtual bool schedule( uint32_t monitorIndex, const SResId &mediaId ) = 0;

protected:
    virtual ~IWallController() {};
};

/** ��ƵЧ�����ڽӿ�.
 */
interface MPCNETSDK_PUBLIC IVideoEffectControler
{
    /**
     * ɫ������
     * ɫ��,��Χ[0~255]
     */
    virtual bool getHue( uint8_t &hue ) = 0;
    virtual bool setHue( const uint8_t hue ) = 0;
    /**
     * ��������
     * ����,��Χ[0~255]
     */
    virtual bool getBrightness( uint8_t &bri ) = 0;
    virtual bool setBrightness( const uint8_t bri ) = 0;
    /**
     * ���Ͷ�����
     * ���Ͷ�,��Χ[0~255]
     */
    virtual bool getSaturation( uint8_t &sat ) = 0;
    virtual bool setSaturation( const uint8_t sat ) = 0;
    /**
     * �Աȶ�����
     * �Աȶ�,��Χ[0~255]
     */
    virtual bool getContrast( uint8_t &con ) = 0;
    virtual bool setContrast( const uint8_t con ) = 0;

    /** �ͷ���ƵЧ�����ڶ���.
     * 
     * �û���Ҫ���ô˺������ͷŴ����Ķ���.
     */
    virtual void release() = 0;

protected:
    virtual ~IVideoEffectControler() {};
};

/** �澯���ƽӿ�.
 */
interface MPCNETSDK_PUBLIC IAlarmController
{
    /** �澯֪ͨ����
     */
    enum EAlarmNotifyType
    {
        ACTIVE = 0,     // �澯����
        CONFIRM,        // �澯ȷ��
        CLEAR           // �澯���
    };

    /** �澯֪ͨ����
     */
    enum EAlarmType
    {
        SWITCH = 0,     // ������
        VIDEOLOSS       // ��Ƶ��ʧ
    };

    /** �澯�������
     */
    enum EAlarmClearType
    {
        CLEAR_TYPE_AUTO = 0,    // �Զ����
        CLEAR_TYPE_MANUAL       // �˹����
    };

    /** �澯���ģʽ
     */
    enum EAlarmClearMode
    {
        CLEAR_MODE_AUTO = 0,    // �Զ����ģʽ
        CLEAR_MODE_MANUAL       // �ֶ����ģʽ���豸�Զ��ϱ�����������ԣ������˹����
    };

    /** ע����Ҫ��ע�ĸ澯ID������
     * 
     *  �����עͬһ���澯ID�Ĳ�ͬ���ͣ����ε��øýӿ�
     */
    virtual bool registerAlarm( const SResId &alarmId, const EAlarmNotifyType type ) = 0;

    /** ע����ע�ĸ澯ID������
     * 
     *  ����һ�θýӿ�ֻע��ĳ���澯ID��ĳһ������
     *  ����ע���ø澯ID���������ͣ����ε���
     */
    virtual bool unregisterAlarm( const SResId &alarmId, const EAlarmNotifyType type ) = 0;

    /** ע��/ע���澯֪ͨ�������
     * 
     *  handler: ΪNULLʱ��ע���澯֪ͨ���󣻲�Ϊ��ʱ��ע��澯֪ͨ����
     *
     *  �ڸ澯���������ʱ������øö������Ӧ������
     */
    virtual bool setAlarmNotifyHandler( IAlarmNotify * handler ) = 0;

    /** ȷ�ϸ澯
     */
    virtual bool confirmAlarm(const SAlarmRecordId &alarmRecordId) = 0;

    /** ����澯
     */
    virtual bool cleanAlarm(const SAlarmRecordId &alarmRecordId) = 0;

    /** �ͷŸ澯���ƶ���.
     * 
     * �û���Ҫ���ô˺������ͷŴ����Ķ���.
     */
    virtual void release() = 0;

protected:
    virtual ~IAlarmController() {};
};

/** �澯֪ͨ�ӿ�.
 */
interface IAlarmNotify
{
    /** �澯����֪ͨ
     * 
     */
    virtual bool alarmActiveNotify(
        const SAlarmRecordId &alarmRecordId, 
        const SResId &alarmResId,
        const IAlarmController::EAlarmType alarmType,
        const uint32_t activeTime
        ) = 0; 

    /** �澯ȷ��֪ͨ
     * 
     */
    virtual bool alarmConfirmNotify(
        const SAlarmRecordId &alarmRecordId,
        const SResId &alarmResId,
        const IAlarmController::EAlarmType alarmType,
        const uint32_t confirmTime,
		const char *userName,
		const size_t userNameLen
        ) = 0; 

    /** �澯���֪ͨ
     * 
     */
    virtual bool alarmCleanNotify(
        const SAlarmRecordId &alarmRecordId, 
        const SResId &alarmResId,
        const IAlarmController::EAlarmType alarmType,
        const uint32_t clearTime,
        const IAlarmController::EAlarmClearType clearType,
		const char *userName,
		const size_t userNameLen
        ) = 0; 

    /** �澯������ʧЧ֪ͨ
     *
     *  ����֪ͨ������ʱ���澯���ƶ��󲻿��á�
     *  ��Ҫ���� IAlarmController::release �ͷŸ澯�������������´���
     * 
     */
    virtual void alarmControllerTerminate(IAlarmController *alarmController) = 0;

    virtual ~IAlarmNotify() {};
};
/************************************************************************/
/*                                                                      */
/************************************************************************/

/** ý��ڵ�.
 */
interface MPCNETSDK_PUBLIC ITreeNode
{
    /** ��ȡ�ڵ��ʶ��
     * @param[out] labelSize ��ʶ����С(byte).
     * �����Ϊ��,�򷵻�0,��labelSize=0;
     * @return ��ʶ��.
     */
    virtual const char *label( size_t &labelSize ) = 0;

    /** �˽ڵ��Ƿ�����Դ�ڵ�.
     */
    virtual bool  isRes() = 0;

protected:
    virtual ~ITreeNode() {};
};

/** Ŀ¼�ڵ�
 */
interface MPCNETSDK_PUBLIC ITreeDir : public ITreeNode
{
    virtual bool isRes() { return false; }

protected:
    virtual ~ITreeDir() {};
};


/** ��Դ�����ڵ�
 */
interface MPCNETSDK_PUBLIC ITreeResBase : public ITreeNode
{
    virtual bool isRes() { return true; }
    virtual SResId msid() = 0;

    /** ʹ���������ƻ�ȡ��Դ���������Ϣ
     *  name��          �������ơ�
     *                  ��ǰ���ò�ѯ������������ "����"��"·��"��"׮��"��"����"��
     *                  �����ַ���Ϊ GBK �����ʽ��
     *  nameLen��       �������Ƶĳ��ȡ�
     *  valueBuf��      ���ڷ�������ֵ�Ļ�����
     *  valueBufLen��   ���ڷ�������ֵ�Ļ���������
     *                  ������������Ȳ��㣬����ֵ���Զ��ضϡ�
     *  retValueLen��   ʵ�ʷ��ص�����ֵ����
     *  ����ֵ��    true��  ��ȡ�ɹ�
     *              false�� ��ȡʧ��
	 * descript:        ���������valueBufΪ����valueBufLen����0ʱ�������������ֵ�ĳ���
	 *					����retValueLen
     */
    virtual bool getPropertyByName(const char *name, const size_t nameLen,
                                    char * valueBuf, size_t valueBufLen, size_t &retValueLen) = 0;

    /** ��ȡ��������
     */
    virtual size_t getPropertyCount() = 0;

    /** �����±��ȡ������������ֵ
     *  index��         ������������Χ�� 0 ~ (getPropertyCount()-1)
     *  nameBuf��       ���ڷ����������Ļ�����
     *  nameBufLen��    ���ڷ����������Ļ���������
     *                  ������������Ȳ��㣬����ֵ���Զ��ضϡ�
     *  retNameLen��    ʵ�ʷ��ص�����������
     *  valueBuf��      ���ڷ�������ֵ�Ļ�����
     *  valueBufLen��   ���ڷ�������ֵ�Ļ���������
     *                  ������������Ȳ��㣬����ֵ���Զ��ضϡ�
     *  retValueLen��   ʵ�ʷ��ص�����ֵ����
	 * descript:        ���������nameBuf��valueBufΪ����nameBufLen��valueBufLen����0ʱ��
	 *					����������ֵĳ��ȴ���retNameLen������ֵ���ȴ���retValueLen��
     */
    virtual bool getPropertyByIndex(const size_t index,
                                    char * nameBuf, size_t nameBufLen, size_t &retNameLen,
                                    char * valueBuf, size_t valueBufLen, size_t &retValueLen) = 0;

protected:
    virtual ~ITreeResBase() {};
};

/** ý��Դ�ڵ�
 */
interface MPCNETSDK_PUBLIC IMediaRes : public ITreeResBase
{
    /** �Ƿ�����̨
     */
    virtual bool hasPtz() = 0;

protected:
    virtual ~IMediaRes() {};
};

/** �澯Դ���
 */
interface MPCNETSDK_PUBLIC IAlarmRes : public ITreeResBase
{
    /** ��ȡ�澯���ģʽ
     */
    virtual IAlarmController::EAlarmClearMode getAlarmClearMode() = 0;

protected:
    virtual ~IAlarmRes() {};
};


/** ý��ڵ������
 *      ʹ�øõ��������Ա�������ý����Դ��
 *      ý����Դ����һ������չʾ��Դ����״�ṹ���������£�
 *
 *    dirroot
 *      |   
 *      |--dir1
 *      |   |--subdir1
 *      |   |   |-------media1
 *      |   |   |-------media2 
 *      |   |
 *      |   |----media3
 *      |
 *      |--dir2
 *      |   |----media4
 *      .   .
 *      .   .
 *      .   .
 *
 *  self() ��Ա�������� IMediaNode ָ�룬IMediaNode�а����˽ڵ����ϸ��Ϣ
 *      ��� IMediaNode::isRes() ���� true ����ʾ����һ����Դ�ڵ㣬
 *          ��ʱ���Խ� IMediaNode ָ��ǿ��ת���� IMediaRes ָ������ȡ��ظ���ϸ����Ϣ��
 *      ��� IMediaNode::isRes() ���� false ����ʾ����һ��Ŀ¼�ڵ㡣
 *  next() ��Ա��������ƽ������һ���ڵ㣬�������� "dir1" �� next() ���� "dir2"
 *  down() ��Ա���������¼��ĵ�һ���ڵ㣬�������� "dir1" �� down() ���� "subdir1"
 *        
 */
interface MPCNETSDK_PUBLIC IMediaIterator
{
    /** ��ȡý��ڵ㣬���а���ý��ڵ����ϸ��Ϣ
     */
    virtual IMediaNode *self() = 0;

    /** ��ȡƽ������һ���ڵ�
     */
    virtual IMediaIterator *next() = 0;

    /** ��ȡ�¼��ĵ�һ���ڵ�
     */
    virtual IMediaIterator *down() = 0;

    /** �ͷŶ���
     * 
     * �û���Ҫ���ô˺������ͷŴ�������.
     * �Ὣ�������¼�һ���ͷ�.
     */
    virtual void release() = 0;

protected:
    virtual ~IMediaIterator() {};
};

/** ��̨Ԥ��λ��Ϣ.
 */
interface MPCNETSDK_PUBLIC IInfoPreset
{
    /** ��ȡԤ��λ����
     * @param[out] nameSize ���ƴ���С(byte).
     * �����Ϊ��,�򷵻�0,��nameSize=0;
     * @return ���ƴ�.
     */
    virtual const char *name( size_t &nameSize ) = 0;
    /** ��ȡԤ��λ���.
     */
    virtual uint8_t idx() = 0;
    /** ��ȡ��һ��Ԥ��λ��Ϣ.
     */
    virtual IInfoPreset *next() = 0;

    /** �ͷ���̨Ԥ��λ��Ϣ����.
     * 
     * �û���Ҫ���ô˺������ͷŴ�������.
     * ע��:�ͷ���ָ��ʱ,�Զ��ͷ�����next.
     */
    virtual void release() = 0;

protected:
    virtual ~IInfoPreset() {};
};

/** ʱ�����Ϣ.
 */
interface MPCNETSDK_PUBLIC IInfoTimePeriod
{
    /** ��ȡ��ʼʱ��.(utc)
     */
    virtual uint32_t beginTime() = 0;
    /** ��ȡ����ʱ��.(utc)
     */
    virtual uint32_t endTime() = 0;
    /** ��ȡ��һ��ʱ�����Ϣ.
     */
    virtual IInfoTimePeriod *next() = 0;

    /** �ͷ�ʱ�����Ϣ����.
     * 
     * �û���Ҫ���ô˺������ͷŴ�������.
     * ע��:�ͷ���ָ��ʱ,�Զ��ͷ�����next.
     */
    virtual void release() = 0;

protected:
    virtual ~IInfoTimePeriod() {};
};

/** ������ID��Ϣ.
 */
interface MPCNETSDK_PUBLIC IInfoMonitor
{
    /** ��ȡ������ID,�ͼ�����˳���.
     */
    virtual SResId id() = 0;
    /** ��ȡ��һ��������ID��Ϣ.
     */
    virtual IInfoMonitor *next() = 0;

    /** �ͷż�����ID��Ϣ����.
     * 
     * �û���Ҫ���ô˺������ͷŴ�������.
     * ע��:�ͷ���ָ��ʱ,�Զ��ͷ�����next.
     */
    virtual void release() = 0;

protected:
    virtual ~IInfoMonitor() {};
};

} }

#endif
