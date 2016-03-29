#ifndef IMGQLTCTRWIDGET_HPP_
#define IMGQLTCTRWIDGET_HPP_
 
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "../include/SysTypeDefine.hpp"
 
class CMulLivingPlayerTab;

class CVidEffCtrWidget: public QWidget
{
    Q_OBJECT
public:
    CVidEffCtrWidget( QWidget *parent = 0, Qt::WindowFlags flag = 0 );
    ~CVidEffCtrWidget();

	void updateData();
 
private:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QLabel *m_labelHue;
    QSlider *m_horizontalSliderHue;
    QLabel *m_labelBright;
    QSlider *m_horizontalSliderBright;
    QLabel *m_labelSaturation;
    QSlider *m_horizontalSliderSat;
    QLabel *m_labelContrast;
    QSlider *m_horizontalSliderContrast;

	CMulLivingPlayerTab* m_pMulLivingTab;

protected slots:
	void valueChangedSlot( int val );

};
 
#endif    //CIMGQLTCTRWIDGET_HPP_
 
