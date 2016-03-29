#ifndef LOGINWND_HPP_
#define LOGINWND_HPP_

#include <QtGui/QDialog>
#include <QtCore/QEvent>
#include <QtGui/QCloseEvent>

class QLineEdit;
class QGridLayout;
class QVBoxLayout;
class QPushButton;
class QLabel;

#include "ResourceTreeWidget.hpp"

class CLoginWnd : public QDialog
{
	Q_OBJECT

public:
    CLoginWnd(CResourceTreeWidget *tree, ELoginType loginType = ELOGINADD, const std::string &strMd5 = "");
    ~CLoginWnd();

    void setTipInfo( const QString &tipInfo);
    bool setPropertyInfo(const SCmsLoginInfo &info);

private slots:
	void onLoginButtonClicked();
	void onCancelButtonClicked();

private:
    void closeEvent(QCloseEvent *event);

    bool checkUserName(const std::string &userName);
    bool checkPassword(const std::string &password);
    bool checkServer( const std::string &server );
    bool checkPort( const std::string &port);
	
private:
	QLineEdit *m_userEditor;
	QLineEdit *m_pwdEditor;
	QLineEdit *m_serverEditor;
    QLineEdit *m_portEditor;
    QLineEdit *m_md5Editor;
	QPushButton *m_loginButton;
	QPushButton *m_cancelButon;
	QLabel *m_userLabel;
	QLabel *m_pwdLabel;
	QLabel *m_serverLabel;
    QLabel *m_md5Label;
    QLabel *m_tipLabel;
    QVBoxLayout *m_mainLayout;
    CResourceTreeWidget *m_resourceTreeWidget;
    ELoginType m_loginType;
    std::string m_strMd5;
   
};

#endif
