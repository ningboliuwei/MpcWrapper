#ifndef LIVINGCONTROLWIDGET_HPP_
#define LIVINGCONTROLWIDGET_HPP_
 
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
#include <Phonon/VolumeSlider>
#include <QtGui/QComboBox>
 
class CMulLivingPlayerTab;

class CLivingControlWidget : public QWidget
{
		Q_OBJECT
public:
    CLivingControlWidget(QWidget *parent = 0, Qt::WindowFlags flag = 0);
    ~CLivingControlWidget();

	void updateData();
	bool getStreamType();  // true: 主码流；false：子码流
    void setPlayerWindowNum( const int &num );
 
private:
    QVBoxLayout *m_verticalLayout;
    QLabel      *m_labelID;
    QHBoxLayout *m_horizontalLayout;
	QLabel		*m_labelStremType;
	QComboBox	*comboBoxStreamType;
	QPushButton *m_pushButtonClose;
	QPushButton *m_pushButtonCloseAll;
    QPushButton *m_pushButtonSnap;
	QPushButton *m_pushButtonSnapPath;
    QComboBox	*m_comboBoxWindowNum;
	QCheckBox   *m_checkBoxMute;
	QSlider     *m_sliderVolume;
	CMulLivingPlayerTab  *m_mulLivingPlayWidget;
	QString      m_snap_path;

private slots:
	void onButtonSnap();
	void onButtonSnapPath();
	void onCheckBoxMute( bool state );
	void onVolumeChanged( int value );
    void onComBoxChanged( int index );

	void onButtonClose();
	void onButtonCloseAll();
};
 
#endif    //LIVINGCONTROLWIDGET_HPP_
 
