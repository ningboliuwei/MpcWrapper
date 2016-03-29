#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPixmap>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtCore/QRegExp>
#include <QtGui/QRegExpValidator>

#include "LoginWnd.hpp"


extern bool isSupportTestSdk;

CLoginWnd::CLoginWnd(CResourceTreeWidget *tree, ELoginType loginType, const std::string &strMd5)
        : QDialog((QWidget*)(tree->parent()))
        , m_resourceTreeWidget(tree)
        , m_loginType(loginType)
	{
        setWindowFlags(Qt::Popup | Qt::Dialog);
		setWindowTitle(tr("CmsInfo"));
        setFixedSize(295,180);

		m_userLabel         = new QLabel(tr("Name:"));
		m_pwdLabel          = new QLabel(tr("Password:"));
		m_serverLabel       = new QLabel(tr("Server:"));
        m_md5Label          = new QLabel(tr("Md5:"));
        m_tipLabel          = new QLabel();
        QLabel *portLabel   = new QLabel(tr(":"));
        portLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

        QRegExp usernameregExp("[a-z0-9A-Z_]{32}");
		m_userEditor = new QLineEdit();
        if(!isSupportTestSdk)
        {
			m_userEditor->setMaxLength(MAX_USERNAME_LENGTH);
            m_userEditor->setValidator(new QRegExpValidator(usernameregExp, m_userEditor));
        }

        m_userEditor->setContextMenuPolicy(Qt::NoContextMenu);

        QRegExp passwordregExp("[a-z0-9A-Z]{16}");
		m_pwdEditor = new QLineEdit();
        //m_pwdEditor->setEchoMode(QLineEdit::Password);
        if(!isSupportTestSdk)
        {
			m_pwdEditor->setMaxLength(MAX_PASSWORD_LENGTH);
            m_pwdEditor->setValidator(new QRegExpValidator(passwordregExp, m_pwdEditor));
        }
        m_pwdEditor->setContextMenuPolicy(Qt::NoContextMenu);

        QRegExp serverregExp("[0-9.]{15}");
		m_serverEditor = new QLineEdit();
        if(!isSupportTestSdk)
        {
			m_serverEditor->setMaxLength(MAX_SERVERADD_LENGTH);
            m_serverEditor->setValidator(new QRegExpValidator(serverregExp, m_serverEditor));
        }
        m_serverEditor->setContextMenuPolicy(Qt::NoContextMenu);

        QRegExp portregExp("(^6[0-4]\\d{3}$)|(^65[0-4]\\d{2}$)|(^655[0-2]\\d$)|(^6553[0-5]$)|(^[1-9]\\d{0,3}$)|(^[1-5]\\d{4}$)");
        m_portEditor = new QLineEdit();
        if(!isSupportTestSdk)
        {
			m_portEditor->setMaxLength(MAX_USERNAME_LENGTH);
            m_portEditor->setValidator(new QRegExpValidator(portregExp, m_portEditor));
        }

        m_portEditor->setContextMenuPolicy(Qt::NoContextMenu);

        m_md5Editor = new QLineEdit();

		m_loginButton = new QPushButton(tr("OK"));
		m_cancelButon = new QPushButton(tr("Cancel"));

		m_mainLayout= new QVBoxLayout(this);

        QGridLayout *topLayout = new QGridLayout();
		topLayout->addWidget(m_userLabel,0,0);
		topLayout->addWidget(m_userEditor,0,1,1,3);

		topLayout->addWidget(m_pwdLabel,1,0);
		topLayout->addWidget(m_pwdEditor,1,1,1,3);

		topLayout->addWidget(m_serverLabel,2,0);
		topLayout->addWidget(m_serverEditor,2,1);

        topLayout->addWidget(portLabel,2,2);
        topLayout->addWidget(m_portEditor,2,3);

        topLayout->addWidget(m_md5Label,3,0);
        topLayout->addWidget(m_md5Editor,3,1,1,3);

        topLayout->addWidget(m_tipLabel,4,0,1,4);

        topLayout->setColumnStretch(0,2);
        topLayout->setColumnStretch(1,6);
        topLayout->setColumnStretch(2,1);
        topLayout->setColumnStretch(3,4);
        topLayout->setColumnStretch(4,1);

        topLayout->setRowStretch(0,1);
        topLayout->setRowStretch(1,1);
        topLayout->setRowStretch(2,1);
        topLayout->setRowStretch(3,1);
        topLayout->setRowStretch(4,1);

        QHBoxLayout *buttonlayout = new QHBoxLayout();
        buttonlayout->addStretch();
        buttonlayout->addWidget(m_loginButton);
        buttonlayout->addWidget(m_cancelButon);

        m_mainLayout->addLayout(topLayout);
        m_mainLayout->addLayout(buttonlayout);

        if (!m_userEditor->text().isEmpty())
        {
            m_pwdEditor->setFocus();
        }

        if (m_loginType == ELOGINREAD)
        {
            m_userEditor->setDisabled(true);
            m_pwdEditor->setDisabled(true);
            m_serverEditor->setDisabled(true);
            m_portEditor->setDisabled(true);
        }
        else if (m_loginType == ELOGINMODIFY)
        {
            m_serverEditor->setDisabled(true);
        }

        m_md5Editor->setText(QString(strMd5.c_str()));
        m_md5Editor->setDisabled(true);

		connect(m_loginButton,SIGNAL(clicked()),SLOT(onLoginButtonClicked()));
		connect(m_cancelButon,SIGNAL(clicked()),SLOT(onCancelButtonClicked()));
	}

    CLoginWnd::~CLoginWnd()
    {
        delete m_mainLayout;
        m_mainLayout = NULL;
    }

    void CLoginWnd::closeEvent(QCloseEvent *event)
    {
        reject();
    }

	void CLoginWnd::onLoginButtonClicked()
	{
        if (m_loginType == ELOGINREAD)
        {
            reject();
            return;
        }
        std::string username;
        std::string password;
        std::string server;
        if(isSupportTestSdk)
        {
            username = m_userEditor->text().toLocal8Bit();
            password = m_pwdEditor->text().toLocal8Bit();
            server = m_serverEditor->text().toLocal8Bit();
        }
        else
        {
            username = m_userEditor->text().toLocal8Bit();
            if (!checkUserName(username))
            {
                setTipInfo(tr("username invalid"));
                return;
            }

            password = m_pwdEditor->text().toLocal8Bit();
            if (!checkPassword(password))
            {
                setTipInfo(tr("password invalid"));
                return;
            }

            server = m_serverEditor->text().toLocal8Bit();
            if (!checkServer(server))
            {
                setTipInfo(tr("server invalid"));
                return;
            }
        }

        std::string port = m_portEditor->text().toLocal8Bit();
        if (m_resourceTreeWidget)
        {
            SCmsLoginInfo loginInfo;
            loginInfo.userName = username;
            loginInfo.password = password;
            loginInfo.server   = server;
            loginInfo.port     = port;
            m_resourceTreeWidget->setCmsItemInfo(loginInfo, m_loginType);
            reject();
        }

	}


    bool CLoginWnd::checkUserName(const std::string &userName)
    {
        if (userName=="admin" || userName.empty())
        {
            return false;
        }
        return true;
    }

    bool CLoginWnd::checkPassword(const std::string &password)
    {
        if ( password.size() >= MIN_PASSWORD_LENGTH && password.size() <= MAX_PASSWORD_LENGTH)
        {
            return true;
        }
        return false;
    }

    bool CLoginWnd::checkServer( const std::string &server )
    {
        if ( server.size() < MIN_SERVERADD_LENGTH || server.size() > MAX_SERVERADD_LENGTH )
        {
            return false;
        }
        return true;
    }

    bool CLoginWnd::checkPort( const std::string &port)
    {
        return true;
    }

	void CLoginWnd::onCancelButtonClicked()
	{
        reject();
	}

    void CLoginWnd::setTipInfo( const QString &tipInfo)
    {
        m_tipLabel->clear();
        m_tipLabel->setStyleSheet("color:red");
        m_tipLabel->setText( tipInfo );
    }

    bool CLoginWnd::setPropertyInfo(const SCmsLoginInfo &info)
    {
        QString username(info.userName.c_str());
        QString password(info.password.c_str());
        QString server(info.server.c_str());
        QString port(info.port.c_str());

        QRegExp usernameExp("[a-z0-9A-Z_]{0,32}");
        if( !usernameExp.exactMatch(username) )
        {
            return false;
        }
        m_userEditor->setText(username);

        QRegExp passwordExp("[a-z0-9A-Z]{0,16}");
        if( !passwordExp.exactMatch(password) )
        {
            return false;
        }
        m_pwdEditor->setText(password);

        QRegExp serverExp("[0-9.]{0,15}");
        if( !serverExp.exactMatch(server) )
        {
            return false;
        }
        m_serverEditor->setText(server);

        QRegExp portExp("[0-9]{0,5}");
        if( !portExp.exactMatch(port) )
        {
            return false;
        }
        m_portEditor->setText(port);

        return true;

    }