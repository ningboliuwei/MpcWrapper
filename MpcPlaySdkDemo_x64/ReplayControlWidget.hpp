#ifndef REPLAYCONTROLWIDGET_HPP_
#define REPLAYCONTROLWIDGET_HPP_
 
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/QCheckBox>
#include <QtGui/QSlider>

class CReplayTab;
class CReplayControlWidget : public QWidget
{
		    Q_OBJECT
public:
    CReplayControlWidget(QWidget *parent = 0, Qt::WindowFlags flag = 0);
    ~CReplayControlWidget();

	void updateData();
 
private:

    QLabel      *m_labelID;
    QPushButton *m_pushButtonPlay;
    QPushButton *m_pushButtonPause;
    QPushButton *m_pushButtonNext;
    QPushButton *m_pushButtonSnap;
	QPushButton *m_pushButtonSnapPath;
	QPushButton *m_pushButtonClose;
	QPushButton *m_pushButtonCloseAll;

	QPushButton *m_pushButtonLoad;
	QCheckBox   *m_checkBoxMute;
	QSlider     *m_sliderVolume;
	QLabel      *m_labelSpeed;
	QSlider     *m_sliderSpeed;
	CReplayTab *m_replayWidget;
	QString     m_snapPath;
	bool        m_isPause;

private slots:

	void onButtonClick();
	void onCheckBoxMute( bool state );
	void onVolumeChanged( int value );
	void onSpeedChanged( int value );

};
 
#endif    //REPLAYCONTROLWIDGET_HPP_
 
