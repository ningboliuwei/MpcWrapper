#ifndef PTZCONTROLWIDGET_HPP_
#define PTZCONTROLWIDGET_HPP_
 
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/QLineEdit>

#include "MpcPlaySdkItf.hpp"
#include "MpcNetSdkItf.hpp"
 
class CMulLivingPlayerTab;
class CPtzControlWidget : public QWidget
{
		Q_OBJECT
public:
    CPtzControlWidget( QWidget *parent = 0, Qt::WindowFlags flag = 0 );
    ~CPtzControlWidget();

	void updateData();

private:
	mpc::nsdk::IPtzController::EPtzSpeed getSpeed(); 
	mpc::nsdk::IPtzController::EPtzFocus getFocus(); 
	mpc::nsdk::IPtzController::EPtzZoom  getZoom();
    mpc::nsdk::IPtzController::EPtzIris getIris();

	bool getSwitchStatus( QComboBox *comboBox );

private:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *m_ptzGroup;
    QVBoxLayout *verticalLayout;

    QLabel *m_labelSpeed;
    QComboBox *comboBoxSpeed;

    QPushButton *m_pushButtonUp;
    QPushButton *m_pushButtonRight;
    QPushButton *m_pushButtonStop;
    QPushButton *m_pushButtonLeft;
    QPushButton *m_pushButtonDown;
    QHBoxLayout *horizontalLayout;
    QLabel *m_labelFocus;
    QComboBox *comboBoxFocus;
    QHBoxLayout *horizontalLayout_2;
    QLabel *m_labelZoom;
    QComboBox *comboBoxZoom;
    QHBoxLayout *horizontalLayout_3;
    QLabel *m_labelIris;
    QComboBox *comboBoxIris;
    QHBoxLayout *horizontalLayout_4;
    QLabel *m_labelHeat;
    QComboBox *comboBoxHeat;
    QHBoxLayout *horizontalLayout_heat;
    QLabel *m_labelFan;
    QComboBox *comboBoxFan;
    QHBoxLayout *horizontalLayout_fan;
    QLabel *m_labelWiper;
    QComboBox *comboBoxWiper;
    QHBoxLayout *horizontalLayout_5;
    QLabel *m_labelLight;
    QComboBox *comboBoxLight;
    QHBoxLayout *horizontalLayout_6;
    QLabel *m_labelPreset;
    QComboBox *comboBoxPreset;
	QPushButton *m_delPreset;
	QHBoxLayout *horizontalLayout_7;
	QLabel *m_labelName;
	QPushButton *m_addPreset;
	QLineEdit *m_editName;

	CMulLivingPlayerTab* m_mulLivingWidget;

private slots:

	void onButtonClick();
	void onComboBoxActived();
};
 
#endif    //PTZCONTROLWIDGET_HPP_
 
