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
interface IVideoEffectControler; // 视频效果接口

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

interface IAlarmController; // 告警订阅,取消订阅，注册通知
interface IAlarmNotify;     // 告警产生通知,告警清除通知
// @}

struct SResId
{
    uint8_t id[16];
};
typedef SResId SAlarmRecordId; // 告警记录的ID

const size_t MAX_DIGEST_SIZE = 128; // 最大资源树摘要长度.单位(byte)

/** 网络SDK运行时环境
 */
struct MPCNETSDK_PUBLIC Runtime
{
    /** 初始化
     *
     * 该组函数用于SDK运行时环境的初始化/反向初始化工作，在调用任何接口函数之前都必须要调用init函数，
     * 在使用完该SDK后必须调用fini来释放所有的资源，okey用于判断是否初始化成功.
     */
    //@{
    static bool init();
    static bool fini();
    static bool okey();
    //@}
};

/** 业务对象创建工厂
 */
struct MPCNETSDK_PUBLIC Factory
{
    /** 创建管理会话
     * 用户指定服务器的地址,用户名,密码,来创建一个管理会话. 用户通过管理会话来进行登录/取流/云台/切换.
     * 注意:创建的管理会话需要手动的调用IManageSession的release,以释放.
     *      相同的addr+port,只能创建一个IManageSession的对象.
     *
     * @param[in] addr 服务器IP地址
     * @param[in] port 服务器端口号
     * @param[in] username 登录服务器的用户名
     * @param[in] usernameSize 登录服务器的用户字符串长度(byte).
     * @param[in] password 登录服务器的明文密码/md5password 登录服务器的MD5密码
     * @param[in] passwordSize 登录服务器的密码长度(byte).
     *
     * @return 管理会话对象接口指针.
     */
    static IManageSession *createManageSession( const uint32_t addr, const uint16_t port, 
                                                const char *username, const size_t usernameSize,
                                                const char *password, const size_t passwordSize);

    static IManageSession *createManageSession( const uint32_t addr, const uint16_t port, 
                                                const char *username, const size_t usernameSize,
                                                const uint8_t *md5password, const size_t passwordSize);


};

/** 管理会话接口
 *
 *  所有成员接口中使用到的 msid ，可以通过 IMediaIterator 遍历资源目录树获取到。
 *  获取 IMediaIterator 需使用 createMediaIterator 接口。
 *
 */
interface MPCNETSDK_PUBLIC IManageSession
{
    /** 初始化
     *
     * 使用该接口之前必须要调用初始化函数，同时在使用完成后必须要调用反向初始化函数.
     */
    //@{
    virtual bool init() = 0;
    virtual bool fini() = 0;
    virtual bool okey() = 0;
    //@}

    /** 用户主动登录与主动登出函数.
     * 
     * 主动登录:完成与服务器之间建链并登录.
     * 主动登出:完成与服务器之间登出并断链.
     * 在掉线后,用户会被动登出,被动登出后应该由sdk的使用者主动再次调用主动登录.
     * 
     * @param[in] 无
     * 
     * @return bool
     */
    //@{
    virtual bool login() = 0;
    virtual bool logout() = 0;
    //@}

    /** 用户修改自己的密码.
     * 
     * 用户登录成功之后,才可以修改自己的密码.
     * 
     * @param[in] curPwd: 用户当前密码/md5curPwd: 经过MD5加密后的用户当前密码.
     * @param[in] curPwdSize: 用户当前密码的长度.
     * @param[in] newPwd: 用户设置的新密码/md5newPwd: 经过MD5加密后的用户设置的新密码.
     * @param[in] newPwdSize: 用户新密码的长度.
     * 
     * @return bool
     */
    virtual bool updatePwd( const char *curPwd, const size_t curPwdSize,
                            const char *newPwd, const size_t newPwdSize ) = 0;

    virtual bool updatePwd( const uint8_t *md5curPwd, const size_t curPwdSize,
                            const uint8_t *md5newPwd, const size_t newPwdSize ) = 0;
    // 用户修改自己的密码之后, 不需要重新登录.

    /** 订阅管理会话异常消息
     * 用户需要保证在本IManageSession对象存在期间,notify对象都是存在的.
     * 可以不订阅.
     */
    virtual void subscribe( IManageSessionNotify *notify ) = 0;

    /** 查询媒体源摘要.
     * 如果两次摘要信息不同,则代码媒体源有变化.
     * @param[out] digestBuf：      返回摘要信息的二进制缓冲区头指针
     * @param[in]  digestBufSize：  传入摘要信息的二进制缓冲区长度（最大缓冲区长度为 MAX_DIGEST_SIZE）
     * @param[out] digestRetSize:   返回的摘要信息长度(byte).
     *
     * @return 是否成功
     */
    virtual bool getMediaDigest( uint8_t *digestBuf, const size_t digestBufSize, size_t &digestRetSize ) = 0;

    /** 获取媒体源迭代器.
     * 当用户不使用时,需要调用IMediaIterator的release函数.
     */
    virtual IMediaIterator *createMediaIterator() = 0;

    /** 查询告警源摘要.
     * 如果两次摘要信息不同,则代表媒体源有变化.
     * @param[out] digestBuf：      返回摘要信息的二进制缓冲区头指针
     * @param[in]  digestBufSize：  传入摘要信息的二进制缓冲区长度（最大缓冲区长度为 MAX_DIGEST_SIZE）
     * @param[out] digestRetSize:   返回的摘要信息长度(byte).
     *
     * @return 是否成功
     */
    virtual bool getAlarmDigest( uint8_t *digestBuf, const size_t digestBufSize, size_t &digestRetSize ) = 0;

    /** 获取告警源迭代器.
     * 当用户不使用时,需要调用IMediaIterator的release函数.
     */
    virtual IAlarmIterator *createAlarmIterator() = 0;

    /** 创建实时流对象.
     *
     * 用户指定媒体源的ID,并指定主/子码流,来创建一个实时流对象.
     *
     * @param[in] msid 媒体源ID.
     * @param[in] sub  主子码流(true=子码流;false=主码流).
     *
     * @return 实时流对象接口指针.
     */
    virtual ILivingStream *createLivingStream( const SResId &msid, const bool sub ) = 0;
    /** 创建历史流对象.
     * 
     * 用户指定媒体源的ID来创建一个历史流对象.
     *
     * @param[in] msid 媒体源ID.
     *
     * @return 历史流对象接口指针.
     */
    virtual IReplayStream *createReplayStream( const SResId &msid ) = 0;

    /** 创建云台控制对象.
     * 
     * 用户指定媒体源的ID来创建一个云台控制对象.
     * 从属于一个管理会话的云台控制对象都使用此管理会话的上下文,例如:如果此管理会话被动登出,则这些云台控制对象的远程访问都失败.
     * 
     * @param[in] msid 媒体源ID.
     *
     * @return 云台控制对象接口指针.
     */
    virtual IPtzController *createPtzController( const SResId &msid ) = 0;

    /** 创建电视墙控制对象.
     * 
     * 从属于一个管理会话的电视墙控制对象都使用此管理会话的上下文,例如:如果此管理会话被动登出,则这些电视墙控制对象的远程访问都失败.
     * 
     * @return 电视墙控制对象接口指针.
     */
    virtual IWallController *createWallController() = 0;

    /** 创建视频效果调节对象.
     * 
     * 用户指定媒体源的ID来创建一个视频效果调节对象.
     * 从属于一个管理会话的视频效果调节对象都使用此管理会话的上下文,例如:如果此管理会话被动登出,则这些视频效果调节对象的远程访问都失败.
     * 
     * @return 电视墙控制对象接口指针.
     */
    virtual IVideoEffectControler *createVideoEffectControler( const SResId &msid ) = 0;

    /** 检索历史视频时间段.
     * 
     * 用户指定一个媒体源的ID和检索的开始时间与结束时间,检索在这段时间内此媒体源的历史视频时间段.
     * 
     * @param[in] msid 媒体源ID.
     * @param[in] beginTime 开始时间
     * @param[in] endTime 结束时间
     * @param[out] tp 媒体源历史视频时间段列表.
     *
     */
    virtual void retrieveReplayPeriod( const SResId &msid, const uint32_t beginTime, const uint32_t endTime, IInfoTimePeriod **tp ) = 0;

    /** 创建告警控制对象.
     * 
     * 从属于一个管理会话的告警控制对象都使用此管理会话的上下文,例如:如果此管理会话被动登出,则这些告警控制对象的远程访问都失败.
     * 
     * @return 告警控制对象接口指针.
     */
    virtual IAlarmController *createAlarmController() = 0;

    /** 释放管理会话对象
     * 
     * 用户需要调用此函数以释放创建对象.
     */
    virtual void release() = 0;

protected:
    virtual ~IManageSession() {};
};

/** 管理会话异常消息接口.
 */
interface MPCNETSDK_PUBLIC IManageSessionNotify
{
    /** 会话终止.
     * 
     * 如果管理会话所对应的用户被动登出,则SDK会调用此会话终止函数,以通知SDK的使用者用户被动登出事件发生.
     * 实现要求:函数迅速返回，并且函数内不能调用 IManageSession::release() 释放该session对象，否则可能导致未知的异常。
     *
     * @param[in] session 发生用户被动登出的管理会话对象.
     */
    virtual void terminate( IManageSession *session ) = 0;

    virtual ~IManageSessionNotify() {};
};

/** 云台控制接口.
 */
interface MPCNETSDK_PUBLIC IPtzController
{
    enum EPtzMove  { MOVE_STOP  = 0, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };
    enum EPtzFocus { FOCUS_STOP = 0, FOCUS_NEAR, FOCUS_FAR };
    enum EPtzZoom  { ZOOM_STOP  = 0, ZOOM_IN, ZOOM_OUT };
    enum EPtzIris  { IRIS_OPEN = 0, IRIS_CLOSE };
    enum EPtzSpeed { SPEED_SLOWER = 0, SPEED_SLOW, SPEED_NORMAL, SPEED_FAST, SPEED_FASTER };

    /** 云台移动.
     *      speed 为枚举值，如需由整数强制转换到该类型，整数范围请参看 EPtzSpeed 的定义
     */
    virtual void move( const EPtzMove oper, const EPtzSpeed speed ) = 0;
    /** 镜头动作：焦点(near前调,far后调) (界面翻译显示为"对焦")
     */
    virtual void focus( const EPtzFocus oper ) = 0;
    /** 镜头动作：焦距(in倍率变大.out倍率变小) (界面翻译显示为"焦距")
     */
    virtual void zoom( const EPtzZoom oper ) = 0;
    /** 镜头动作：光圈(wide=open扩大,close=tele缩小)(界面翻译显示为"光圈")
     */
    virtual void iris( const EPtzIris oper ) = 0; 
    /** 加热动作：加热开启、加热关闭
     */
    virtual void heat( const bool on ) = 0;
    /** 风扇动作：风扇开启、风扇关闭
     */
    virtual void fan( const bool on ) = 0;
    /** 雨刷开关.
     */
    virtual void wiper( const bool on ) = 0;
    /** 灯光开关
     */
    virtual void light( const bool on ) = 0;

	/** 添加预置位.
	 * @param[in] buf 	预置位名称字符串
	 * @param[in] len 	预置位名称长度
	 * @param[in] index 	预置位索引.
     */
    virtual bool addPreset( const char *presetName, const size_t presetNameLen, const uint8_t index ) = 0; 

	/** 移除预置位.
	 * @param[in] name 预置位索引.
     */
    virtual bool delPreset( const uint8_t index ) = 0;
	
    /** 获取预置位列表.
     * @param[out] idxList 预置位列表.
     */
    virtual void preset( IInfoPreset **idxList ) = 0;
    /** 切换预置位.
     */
    virtual void preset( const uint8_t idx ) = 0;

    /** 释放云台控制对象.
     * 
     * 用户需要调用此函数以释放创建的对象.
     */
    virtual void release() = 0;

protected:
    virtual ~IPtzController() {};
};

/** 数据流处理接口.
 */
interface IStreamHandler
{
    enum EStreamType { ES = 0, PS, TS };

    /** StreamHandler建立消息.
     * 实现要求:函数迅速返回.
     * 对于实时流,代表建立.
     * 对于历史流,不仅代表建立;还表示:在此函数调用之后,再进行修改时间,回放/下载,等操作才是安全的.
     */
    virtual void startup() = 0;

    /** 数据流开始消息.
     * 实现要求:函数迅速返回.
     * @param[in] md 码流类型.
     */
    virtual void establish( const EStreamType md ) = 0;

    /** 数据流.
     * 实现要求:函数迅速返回.
     * @param[in] buf 数据缓冲区首地址.
     * @param[in] len 数据缓冲区大小(byte).
     * @param[in] realTime 数据包的时间戳（从1970-1-1开始的秒数）.
     */
    virtual void stream( const uint8_t *buf, const size_t len, const uint32_t realTime ) = 0;

    /** 数据流终止消息.
     * 实现要求:函数迅速返回.
     * establish与terminate并非总是成对出现,有可能没有establish但有terminate.
     */
    virtual void terminate() = 0;

    /** StreamHandler关闭消息.
     * 实现要求:函数迅速返回.
     * startup与shutdown并非总是成对出现,有可能没有startup但有shutdown.
     */
    virtual void shutdown() = 0;


    /** 释放数据流处理对象.
     * 实现要求:函数迅速返回.
     * 用户实现此函数,以释放本对象,然后sdk会在不需要StreamHandler的时候调用一次release.
     */
    virtual void release() = 0;

    /** 数据流处理对象引用计数加1.
     * 实现要求:函数迅速返回.
     * 用户实现此函数,以将引用计数加1.然后sdk会在setHandler的时候调用一次addRef.
     */
    virtual void addRef() = 0;

protected:
    virtual ~IStreamHandler() {};
};

/** 实时流接口.
 * 
 * 数据流一但建立成功,就需要用户显示关闭,比如用户先登录并获取到流了,然后不论是主动登出还是被动登出,都不影响流.
 */
interface MPCNETSDK_PUBLIC ILivingStream
{
    /** 设置数据流处理对象.
     * 为本实时流对象指定一个数据流处理对象.
     * 
     * 由本函数设置进来的streamHandler,sdk会在不需要它的时候调用一次release.
     * init成功之后,再调用此函数会直接失败.
     * 
     * @param[in] handler 数据流处理对象指针.
     */
    virtual bool setHandler( IStreamHandler *streamHandler ) = 0;

    /** 初始化
     *
     * init:完成建链并请求播放.
     * 在调用init之前,需要调用setHandler;
     * 如果init成功,则不可以重复调用,只能再创建另外一个ILivingStream来重新做.
     * 如果init失败,可以重试.
     * fini:完成关链并释放链相关资源.
     * okey:是否已经init成功.
     */
    //@{
    virtual bool init() = 0;
    virtual bool fini() = 0;
    virtual bool okey() = 0;
    //@}

    /** 释放本实时流对象
     * 
     * 用户需要调用此函数以释放创建对象.
     */
    virtual void release() = 0;

protected:
    virtual ~ILivingStream() {};
};

/** 历史流接口
 *
 * 使用说明：
 *     如果用于播放，不能使用 download 接口
 *     如果用于下载，不能使用 play,pause,step,modifySpeed 接口
 */
interface MPCNETSDK_PUBLIC IReplayStream
{
    /** 设置数据流处理对象.
     * 为本历史流对象指定一个数据流处理对象.
     * 
     * 由本函数设置进来的streamHandler,sdk会在不需要它的时候调用一次release.
     * init成功之后,再调用此函数会直接失败.
     *
     * @param[in] handler 数据流处理对象指针.
     */
    virtual bool setHandler( IStreamHandler *streamHandler ) = 0;
    /** 初始化
     *
     * init:完成建链并请求播放.
     * 在调用init之前,需要调用setHandler;
     * 如果init成功,则不可以重复调用,只能再创建另外一个IReplayStream来重新做.
     * 如果init失败,可以重试.
     * fini:完成关链并释放链相关资源.
     * okey:是否已经init成功.
     */
    //@{
    virtual bool init() = 0;
    virtual bool fini() = 0;
    virtual bool okey() = 0;
    //@}


    /** 播放控制:修改播放时间.
     * 
     * 注意:如果是回放,则到达结束时间时,如果后面还有历史数据,则不会停止.
     *      如果是下载,则到达结束时间就会停止.
     * 
     * @param[in] timeBegin 播放开始时间.(utc)
     * @param[in] timeEnd 播放结束时间.(utc)
     *
     * 使用说明：
     *     init 后，需等待 IStreamHandler::startup 通知后才能调用，否则会导致失败
     */
    virtual bool modifyTimes( const uint32_t timeBegin, const uint32_t timeEnd ) = 0;
    /** 播放控制:修改播放速度.
     * 
     * 注意:速度的取值范围是[8分之一,8]
     * @param[in] numerator 速度分子,范围[1，8].
     * @param[in] denominator 速度分母,范围[1，8].
     *
     * 使用说明：
     *     init 后，需等待 IStreamHandler::startup 通知后才能调用，否则会导致失败
     */
    virtual bool modifySpeed( const uint8_t numerator, const uint8_t denominator ) = 0;

    /** 播放控制:播放.
     *
     * 使用说明：
     *     调用前必须调用一次 modifyTimes 指定正确播放时间段，否则会导致失败
     *     IReplayStream 用于 play 之后就不能用于 download，反之亦然。
     */
    virtual bool play() = 0;
    /** 播放控制:暂停.
     *
     * 使用说明：
     *     play 状态下才能调用 pause
     */
    virtual bool pause() = 0;
    /** 播放控制:单帧.
     *
     * 使用说明：
     *     pause 状态下才能调用 step
     */
    virtual bool step() = 0;

    /** 播放控制:下载.
     *
     * 使用说明：
     *     IReplayStream 用于 download 之后就不能用于 play，反之亦然。
     *     调用前必须调用一次 modifyTimes 指定正确下载时间段，否则会导致失败
     *     调用该接口后，不能再调用 modifyTimes 。
     */
    virtual bool download() = 0;

    /** 释放本历史流对象
     * 
     * 用户需要调用此函数以释放创建对象.
     */
    virtual void release() = 0;

protected:
    virtual ~IReplayStream() {};
};

/** 电视墙控制接口.
 */
interface MPCNETSDK_PUBLIC IWallController
{
    /** 获取监视器列表.
     * @param[out] idList 监视器列表.
     */
    //<< 需要修改，只给当前电视墙
    virtual void getMonitorId( IInfoMonitor **idList ) = 0;

    /** 执行单切
     * 
     * 将媒体源切换到监视器上.
     * 
     * @param[in] monitorId 监视器ID.
     * @param[in] mediaId 媒体源ID.
     */
    ///<< 修改，更布局有关，只工作在当前布局
    virtual bool schedule( const SResId &monitorId, const SResId &mediaId ) = 0;

    /** 停止切换
     * 
     * 停止一个监视器上所有的切换.
     * 
     * @param[in] monitorId 监视器ID.
     */
    ///<< 修改，更布局有关，只工作在当前布局
    virtual bool stop( const SResId &monitorId ) = 0;

    /** 释放电视墙控制对象.
     * 
     * 用户需要调用此函数以释放创建的对象.
     */
    virtual void release() = 0;

    ///<< 添加接口
    virtual bool schedule( uint32_t monitorIndex, const SResId &mediaId ) = 0;

protected:
    virtual ~IWallController() {};
};

/** 视频效果调节接口.
 */
interface MPCNETSDK_PUBLIC IVideoEffectControler
{
    /**
     * 色度配置
     * 色度,范围[0~255]
     */
    virtual bool getHue( uint8_t &hue ) = 0;
    virtual bool setHue( const uint8_t hue ) = 0;
    /**
     * 亮度配置
     * 亮度,范围[0~255]
     */
    virtual bool getBrightness( uint8_t &bri ) = 0;
    virtual bool setBrightness( const uint8_t bri ) = 0;
    /**
     * 饱和度配置
     * 饱和度,范围[0~255]
     */
    virtual bool getSaturation( uint8_t &sat ) = 0;
    virtual bool setSaturation( const uint8_t sat ) = 0;
    /**
     * 对比度配置
     * 对比度,范围[0~255]
     */
    virtual bool getContrast( uint8_t &con ) = 0;
    virtual bool setContrast( const uint8_t con ) = 0;

    /** 释放视频效果调节对象.
     * 
     * 用户需要调用此函数以释放创建的对象.
     */
    virtual void release() = 0;

protected:
    virtual ~IVideoEffectControler() {};
};

/** 告警控制接口.
 */
interface MPCNETSDK_PUBLIC IAlarmController
{
    /** 告警通知类型
     */
    enum EAlarmNotifyType
    {
        ACTIVE = 0,     // 告警产生
        CONFIRM,        // 告警确认
        CLEAR           // 告警清除
    };

    /** 告警通知类型
     */
    enum EAlarmType
    {
        SWITCH = 0,     // 开关量
        VIDEOLOSS       // 视频丢失
    };

    /** 告警清除类型
     */
    enum EAlarmClearType
    {
        CLEAR_TYPE_AUTO = 0,    // 自动清除
        CLEAR_TYPE_MANUAL       // 人工清除
    };

    /** 告警清除模式
     */
    enum EAlarmClearMode
    {
        CLEAR_MODE_AUTO = 0,    // 自动清除模式
        CLEAR_MODE_MANUAL       // 手动清除模式，设备自动上报的清除被忽略，必须人工清除
    };

    /** 注册需要关注的告警ID及类型
     * 
     *  如需关注同一个告警ID的不同类型，需多次调用该接口
     */
    virtual bool registerAlarm( const SResId &alarmId, const EAlarmNotifyType type ) = 0;

    /** 注销关注的告警ID及类型
     * 
     *  调用一次该接口只注销某个告警ID的某一个类型
     *  如需注销该告警ID的所有类型，需多次调用
     */
    virtual bool unregisterAlarm( const SResId &alarmId, const EAlarmNotifyType type ) = 0;

    /** 注册/注销告警通知处理对象
     * 
     *  handler: 为NULL时，注销告警通知对象；不为空时，注册告警通知对象
     *
     *  在告警产生或清除时，会调用该对象的相应处理函数
     */
    virtual bool setAlarmNotifyHandler( IAlarmNotify * handler ) = 0;

    /** 确认告警
     */
    virtual bool confirmAlarm(const SAlarmRecordId &alarmRecordId) = 0;

    /** 清除告警
     */
    virtual bool cleanAlarm(const SAlarmRecordId &alarmRecordId) = 0;

    /** 释放告警控制对象.
     * 
     * 用户需要调用此函数以释放创建的对象.
     */
    virtual void release() = 0;

protected:
    virtual ~IAlarmController() {};
};

/** 告警通知接口.
 */
interface IAlarmNotify
{
    /** 告警产生通知
     * 
     */
    virtual bool alarmActiveNotify(
        const SAlarmRecordId &alarmRecordId, 
        const SResId &alarmResId,
        const IAlarmController::EAlarmType alarmType,
        const uint32_t activeTime
        ) = 0; 

    /** 告警确认通知
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

    /** 告警清除通知
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

    /** 告警控制器失效通知
     *
     *  当该通知被调用时，告警控制对象不可用。
     *  需要调用 IAlarmController::release 释放告警控制器，并重新创建
     * 
     */
    virtual void alarmControllerTerminate(IAlarmController *alarmController) = 0;

    virtual ~IAlarmNotify() {};
};
/************************************************************************/
/*                                                                      */
/************************************************************************/

/** 媒体节点.
 */
interface MPCNETSDK_PUBLIC ITreeNode
{
    /** 获取节点标识串
     * @param[out] labelSize 标识串大小(byte).
     * 如果串为空,则返回0,且labelSize=0;
     * @return 标识串.
     */
    virtual const char *label( size_t &labelSize ) = 0;

    /** 此节点是否是资源节点.
     */
    virtual bool  isRes() = 0;

protected:
    virtual ~ITreeNode() {};
};

/** 目录节点
 */
interface MPCNETSDK_PUBLIC ITreeDir : public ITreeNode
{
    virtual bool isRes() { return false; }

protected:
    virtual ~ITreeDir() {};
};


/** 资源基础节点
 */
interface MPCNETSDK_PUBLIC ITreeResBase : public ITreeNode
{
    virtual bool isRes() { return true; }
    virtual SResId msid() = 0;

    /** 使用属性名称获取资源相关属性信息
     *  name：          属性名称。
     *                  当前可用查询的属性名称有 "编码"、"路段"、"桩号"、"方向"。
     *                  输入字符串为 GBK 编码格式。
     *  nameLen：       属性名称的长度。
     *  valueBuf：      用于返回属性值的缓冲区
     *  valueBufLen：   用于返回属性值的缓冲区长度
     *                  如果缓冲区长度不足，返回值会自动截断。
     *  retValueLen：   实际返回的属性值长度
     *  返回值：    true：  获取成功
     *              false： 获取失败
	 * descript:        当输入参数valueBuf为空且valueBufLen等于0时，函数会把属性值的长度
	 *					传给retValueLen
     */
    virtual bool getPropertyByName(const char *name, const size_t nameLen,
                                    char * valueBuf, size_t valueBufLen, size_t &retValueLen) = 0;

    /** 获取属性总数
     */
    virtual size_t getPropertyCount() = 0;

    /** 根据下标获取属性名及属性值
     *  index：         属性索引，范围： 0 ~ (getPropertyCount()-1)
     *  nameBuf：       用于返回属性名的缓冲区
     *  nameBufLen：    用于返回属性名的缓冲区长度
     *                  如果缓冲区长度不足，返回值会自动截断。
     *  retNameLen：    实际返回的属性名长度
     *  valueBuf：      用于返回属性值的缓冲区
     *  valueBufLen：   用于返回属性值的缓冲区长度
     *                  如果缓冲区长度不足，返回值会自动截断。
     *  retValueLen：   实际返回的属性值长度
	 * descript:        当输入参数nameBuf和valueBuf为空且nameBufLen和valueBufLen等于0时，
	 *					函数会把名字的长度传给retNameLen，属性值长度传给retValueLen。
     */
    virtual bool getPropertyByIndex(const size_t index,
                                    char * nameBuf, size_t nameBufLen, size_t &retNameLen,
                                    char * valueBuf, size_t valueBufLen, size_t &retValueLen) = 0;

protected:
    virtual ~ITreeResBase() {};
};

/** 媒体源节点
 */
interface MPCNETSDK_PUBLIC IMediaRes : public ITreeResBase
{
    /** 是否有云台
     */
    virtual bool hasPtz() = 0;

protected:
    virtual ~IMediaRes() {};
};

/** 告警源结点
 */
interface MPCNETSDK_PUBLIC IAlarmRes : public ITreeResBase
{
    /** 获取告警清除模式
     */
    virtual IAlarmController::EAlarmClearMode getAlarmClearMode() = 0;

protected:
    virtual ~IAlarmRes() {};
};


/** 媒体节点迭代器
 *      使用该迭代器可以遍历整个媒体资源树
 *      媒体资源树是一个用于展示资源的树状结构，类似如下：
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
 *  self() 成员函数返回 IMediaNode 指针，IMediaNode中包含了节点的详细信息
 *      如果 IMediaNode::isRes() 返回 true ，表示这是一个资源节点，
 *          此时可以将 IMediaNode 指针强制转换成 IMediaRes 指针来获取相关更详细的信息。
 *      如果 IMediaNode::isRes() 返回 false ，表示这是一个目录节点。
 *  next() 成员函数返回平级的下一个节点，如上例中 "dir1" 的 next() 就是 "dir2"
 *  down() 成员函数返回下级的第一个节点，如上例中 "dir1" 的 down() 就是 "subdir1"
 *        
 */
interface MPCNETSDK_PUBLIC IMediaIterator
{
    /** 获取媒体节点，其中包含媒体节点的详细信息
     */
    virtual IMediaNode *self() = 0;

    /** 获取平级的下一个节点
     */
    virtual IMediaIterator *next() = 0;

    /** 获取下级的第一个节点
     */
    virtual IMediaIterator *down() = 0;

    /** 释放对象
     * 
     * 用户需要调用此函数以释放创建对象.
     * 会将其所有下级一并释放.
     */
    virtual void release() = 0;

protected:
    virtual ~IMediaIterator() {};
};

/** 云台预置位信息.
 */
interface MPCNETSDK_PUBLIC IInfoPreset
{
    /** 获取预置位名称
     * @param[out] nameSize 名称串大小(byte).
     * 如果串为空,则返回0,且nameSize=0;
     * @return 名称串.
     */
    virtual const char *name( size_t &nameSize ) = 0;
    /** 获取预置位编号.
     */
    virtual uint8_t idx() = 0;
    /** 获取下一个预置位信息.
     */
    virtual IInfoPreset *next() = 0;

    /** 释放云台预置位信息对象.
     * 
     * 用户需要调用此函数以释放创建对象.
     * 注意:释放首指针时,自动释放所有next.
     */
    virtual void release() = 0;

protected:
    virtual ~IInfoPreset() {};
};

/** 时间段信息.
 */
interface MPCNETSDK_PUBLIC IInfoTimePeriod
{
    /** 获取开始时间.(utc)
     */
    virtual uint32_t beginTime() = 0;
    /** 获取结束时间.(utc)
     */
    virtual uint32_t endTime() = 0;
    /** 获取下一个时间段信息.
     */
    virtual IInfoTimePeriod *next() = 0;

    /** 释放时间段信息对象.
     * 
     * 用户需要调用此函数以释放创建对象.
     * 注意:释放首指针时,自动释放所有next.
     */
    virtual void release() = 0;

protected:
    virtual ~IInfoTimePeriod() {};
};

/** 监视器ID信息.
 */
interface MPCNETSDK_PUBLIC IInfoMonitor
{
    /** 获取监视器ID,和监视器顺序号.
     */
    virtual SResId id() = 0;
    /** 获取下一个监视器ID信息.
     */
    virtual IInfoMonitor *next() = 0;

    /** 释放监视器ID信息对象.
     * 
     * 用户需要调用此函数以释放创建对象.
     * 注意:释放首指针时,自动释放所有next.
     */
    virtual void release() = 0;

protected:
    virtual ~IInfoMonitor() {};
};

} }

#endif
