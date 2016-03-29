#ifdef WIN32
#include <Windows.h>
#include <DbgHelp.h>
#endif

#include <iostream>
#include <QtGui/QApplication>

#include "VideoPlayer.hpp"

#ifdef WIN32

LONG WINAPI ApplicationCrashHandler( struct _EXCEPTION_POINTERS *ExceptionInfo )
{
    /*
     * 创建dump文件.
     */
    std::wostringstream ostr;
    ostr << "C:\\core." << GetCurrentProcessId();
    HANDLE hDumpFile = CreateFile( ostr.str().c_str(), GENERIC_WRITE, 0, NULL, 
                                   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );  
    /*
     * 填充异常信息.
     */
    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;  
    dumpInfo.ExceptionPointers = ExceptionInfo;  
    dumpInfo.ThreadId          = GetCurrentThreadId();  
    dumpInfo.ClientPointers    = TRUE;  
    /*
     * 写入异常信息.
     */  
    MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), 
                       hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL );  
  
    CloseHandle(hDumpFile);
    /*
     * 退出应用程序.
     */
    ostr << "\n*** Unhandled Exception! ***";
    FatalAppExit( 0, ostr.str().c_str() );  
  
    return EXCEPTION_EXECUTE_HANDLER; 
}

#endif


int main(int argc, char *argv[])
{
    if( argc < 5 )
    {
        std::cout << "living mode: 1 cmsIp:port user pwd uuid " << std::endl;
        std::cout << "eg: 1 192.168.1.1:10000 userName pwd e963afbb14dad4429f30f938a9cc0826 " << std::endl;
        std::cout << "replay mode: 2 cmsIp:port user pwd uuid time(utc) " << std::endl;
        std::cout << "eg: 2 192.168.1.1:10000 userName pwd e963afbb14dad4429f30f938a9cc0826 1383736341 " << std::endl;
        std::cout << "decoder mode: 3 cmsIp:port user pwd uuid " << std::endl;
        std::cout << "eg: 3 192.168.1.1:10000 userName pwd e963afbb14dad4429f30f938a9cc0826 " << std::endl;
        return -1;
    }

#ifdef WIN32
	SetUnhandledExceptionFilter( &ApplicationCrashHandler );
#endif


    QApplication app(argc, argv);

    std::string addr = argv[2];
    std::string user = argv[3];
    std::string pwd = argv[4];
    std::string uid = argv[5];
    uint32_t tim = 0;
    if( argc == 7 )
        tim = atoi( argv[6] );

    VideoPlayer player( atoi( argv[1] ), addr, user, pwd, uid, tim );
    if( !player.init() )
    {
        std::cout<< "Player init error\n";
        return -1;
    }

    player.show();

    app.exec();

    if( !player.fini() )
    {
        std::cout<< "Player fini error\n";
        return -1;
    }

    return 0;
}
