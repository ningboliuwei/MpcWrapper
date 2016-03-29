#ifndef PWDMODIFYWND_HPP_
#define PWDMODITYWND_HPP_

#include <QtGui/QDialog>
#include "ResourceTreeWidget.hpp"

class QLineEdit;
class QGridLayout;
class QPushButton;
class QLabel;
class QVBoxLayout;

class CLoginHandle;

 
// ÐÞ¸ÄÃÜÂë´°¿Ú
class CPwdModifyWnd : public QDialog
{
	Q_OBJECT

public:
	CPwdModifyWnd( CResourceTreeWidget *tree, mpc::nsdk::IManageSession* manageSession );
    ~CPwdModifyWnd();

public:
    void init();
    bool checkPassword( const std::string &password );

private slots:
	void onOkbuttonClicked();
    void onCancelbuttonClicked();

private:
    std::string m_oldPassword;
    std::string m_newPassword;
    std::string m_confirmPassword;


private:
    mpc::nsdk::IManageSession* m_manageSession;
    CResourceTreeWidget *m_resourceTreeWidget;
	QVBoxLayout *m_mainLayout;
	QLineEdit *m_oldPwdEditor;
	QLineEdit *m_newPwdEditor;
	QLineEdit *m_confirmPwdEditor;
	QPushButton *m_okButton;
	QPushButton *m_cancelButon;
	QLabel *m_oldPwdLable;
	QLabel *m_newPwdLable;
	QLabel *m_confirmPwdLable;
    QLabel *m_tipLabel;

};



#endif

