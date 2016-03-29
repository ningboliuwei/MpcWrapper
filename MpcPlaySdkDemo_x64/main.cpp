#ifdef WIN32
#include <Windows.h>
#include <DbgHelp.h>
#endif

#include <QtGui/QApplication>
#include <QtGui/QtGui>

#include "MainWindow.hpp"
#include "../include/SysTypeDefine.hpp"

bool isSupportTestSdk = false;

#ifdef WIN32

#include <sstream>

LONG WINAPI ApplicationCrashHandler( struct _EXCEPTION_POINTERS *ExceptionInfo )
{
    /*
     * ����dump�ļ�.
     */
    std::wostringstream ostr;
    ostr << "C:\\core." << GetCurrentProcessId();
    HANDLE hDumpFile = CreateFile( ostr.str().c_str(), GENERIC_WRITE, 0, NULL, 
                                   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );  
    /*
     * ����쳣��Ϣ.
     */
    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;  
    dumpInfo.ExceptionPointers = ExceptionInfo;  
    dumpInfo.ThreadId          = GetCurrentThreadId();  
    dumpInfo.ClientPointers    = TRUE;  
    /*
     * д���쳣��Ϣ.
     */  
    MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), 
                       hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL );  
  
    CloseHandle(hDumpFile);
    /*
     * �˳�Ӧ�ó���.
     */
    ostr << "\n*** Unhandled Exception! ***";
    FatalAppExit( 0, ostr.str().c_str() );  
  
    return EXCEPTION_EXECUTE_HANDLER; 
}

#endif


int main( int argc, char **argv )
{
#ifdef WIN32
	SetUnhandledExceptionFilter( &ApplicationCrashHandler );
#endif

    QTextCodec::setCodecForCStrings( QTextCodec::codecForLocale() );   
    QTextCodec::setCodecForTr( QTextCodec::codecForName( "utf8" ) );

    QApplication app(argc, argv);
    QTranslator translator;

    //��ȡ�����ļ�
    if (argc == 3)
    {
        translator.load(QString(argv[2]));
        app.installTranslator(&translator);
    }

    //�Ƿ�֧��SDK���� 1֧��SDK���ԣ�����ֵΪ��֧��SDK����
    if (argc > 1)
    {
        std::string str = argv[1];
        if (str == "1")
        {
            isSupportTestSdk = true;
        }
    }

    if(!mpc::nsdk::Runtime::init() || !mpc::psdk::Runtime::init())
    {
        return -1;
    }

    do 
    {
        CMainWindow *pMainWindow = new (CMainWindow);
        if (!pMainWindow->init())
        {
            if (pMainWindow)
            {
                delete (pMainWindow);
                pMainWindow = NULL;

            }

            break;
        }
        pMainWindow->show();

        app.exec();

        if (!pMainWindow->fini())
        {
            if (pMainWindow)
            {
                delete (pMainWindow);
                pMainWindow = NULL;

            }
            break;
        }
        if (pMainWindow)
        {
            delete (pMainWindow);
            pMainWindow = NULL;
            
        }
    } while(0);

    if(!mpc::nsdk::Runtime::fini() || !mpc::psdk::Runtime::fini())
    {
        return -1;
    }
    return 0;
}
