#include <QtGui/QMessageBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPixmap>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtCore/QRegExp>
#include <QtGui/QRegExpValidator>

#include "PwdModifyWnd.hpp"

CPwdModifyWnd::CPwdModifyWnd( CResourceTreeWidget *tree, mpc::nsdk::IManageSession* manageSession )
    : QDialog((QWidget*)(tree->parent()))
    , m_resourceTreeWidget(tree)
    , m_manageSession(manageSession)
{
	setWindowTitle(tr("PwdModifyWindow"));
    setFixedSize(280,150);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint );

	m_oldPwdLable = new QLabel(tr("Original Password:"));
	m_newPwdLable = new QLabel(tr("New Password:"));
	m_confirmPwdLable = new QLabel(tr("Confirm Password:"));
    m_tipLabel = new QLabel();

    QRegExp passwordregExp("[a-z0-9A-Z]{16}");
	m_oldPwdEditor = new QLineEdit();
    m_oldPwdEditor->setEchoMode(QLineEdit::Password);
    m_oldPwdEditor->setValidator(new QRegExpValidator(passwordregExp, m_oldPwdEditor));
    m_oldPwdEditor->setContextMenuPolicy(Qt::NoContextMenu);

	m_newPwdEditor = new QLineEdit();
    m_newPwdEditor->setEchoMode(QLineEdit::Password);
    m_newPwdEditor->setValidator(new QRegExpValidator(passwordregExp, m_newPwdEditor));
    m_newPwdEditor->setContextMenuPolicy(Qt::NoContextMenu);

	m_confirmPwdEditor = new QLineEdit();
    m_confirmPwdEditor->setEchoMode(QLineEdit::Password);
    m_confirmPwdEditor->setValidator(new QRegExpValidator(passwordregExp, m_confirmPwdEditor));
    m_confirmPwdEditor->setContextMenuPolicy(Qt::NoContextMenu);

	m_okButton = new QPushButton(tr("OK"));
	m_cancelButon = new QPushButton(tr("Cancel"));

	QGridLayout *layout = new QGridLayout();
	layout->addWidget(m_oldPwdLable,0,0);
	layout->addWidget(m_oldPwdEditor,0,1);

	layout->addWidget(m_newPwdLable,1,0);
	layout->addWidget(m_newPwdEditor,1,1);

	layout->addWidget(m_confirmPwdLable,2,0);
	layout->addWidget(m_confirmPwdEditor,2,1);

    layout->addWidget(m_tipLabel,3,0,1,2);

    QHBoxLayout *buttonlayout = new QHBoxLayout();
    buttonlayout->addStretch();
    buttonlayout->addWidget(m_okButton);
    buttonlayout->addWidget(m_cancelButon);

    m_mainLayout= new QVBoxLayout(this);
    m_mainLayout->addLayout(layout);
    m_mainLayout->addLayout(buttonlayout);

	connect(m_okButton,SIGNAL(clicked()),SLOT(onOkbuttonClicked()));
	connect(m_cancelButon,SIGNAL(clicked()),SLOT(onCancelbuttonClicked()));

}

CPwdModifyWnd::~CPwdModifyWnd()
{
    delete m_mainLayout;
    m_mainLayout = NULL;
}

void CPwdModifyWnd::init()
{
    m_oldPwdEditor->clear();
    m_newPwdEditor->clear();
    m_confirmPwdEditor->clear();
    m_tipLabel->clear();
}

void CPwdModifyWnd::onOkbuttonClicked()
{
    m_oldPassword = m_oldPwdEditor->text().toLocal8Bit();
    m_newPassword = m_newPwdEditor->text().toLocal8Bit();
    m_confirmPassword = m_confirmPwdEditor->text().toLocal8Bit();

    do 
    {
        if (m_oldPwdEditor->text().length()==0||m_newPwdEditor->text().length()==0
            ||m_confirmPwdEditor->text().length()==0)
        {
            m_tipLabel->clear();
            m_tipLabel->setStyleSheet("color:red");
            m_tipLabel->setText(tr("password is empty!"));
            break;
        }

        if (!checkPassword(m_oldPassword)||!checkPassword(m_newPassword)
            ||!checkPassword(m_confirmPassword))
        {
            m_tipLabel->clear();
            m_tipLabel->setStyleSheet("color:red");
            m_tipLabel->setText(tr("password is error!"));
            break;
        }
        if (m_newPassword != m_confirmPassword)
        {
            m_tipLabel->clear();
            m_tipLabel->setStyleSheet("color:red");
            m_tipLabel->setText(tr("two password is not equal!"));
            break;
        }

        if (m_manageSession->updatePwd(m_oldPassword.c_str(), m_oldPassword.length(),m_newPassword.c_str(), m_newPassword.length()) )
        {
            m_tipLabel->clear();
            m_tipLabel->setText(tr("Successfull!"));
            m_tipLabel->setStyleSheet("color:black");
            m_resourceTreeWidget->updateCmsInfo(m_newPassword);

            QMessageBox::information(this, QObject::tr("info"), QObject::tr("Password modify successfull!"),QObject::tr("ok")  );
            accept();
            break;
        }
        else
        {
            m_tipLabel->clear();
            m_tipLabel->setStyleSheet("color:red");
            m_tipLabel->setText(tr("modify password fail!"));
            break;
        }
    } while (0);
}

void CPwdModifyWnd::onCancelbuttonClicked()
{
    reject();
}

bool CPwdModifyWnd::checkPassword( const std::string &password )
{
    if ( password.size() >= MIN_PASSWORD_LENGTH && password.size() <= MAX_PASSWORD_LENGTH)
    {
        return true;
    }
    return false;
}
