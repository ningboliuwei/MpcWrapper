#ifndef TIMESEGMENTWIDGET_HPP_
#define TIMESEGMENTWIDGET_HPP_
 
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
 
class CTimeSegmentWidget : public QWidget
{
	Q_OBJECT
public:
    CTimeSegmentWidget(QWidget *parent = 0, Qt::WindowFlags flag = 0);
    ~CTimeSegmentWidget();

	bool getTimeSegment( QPair<unsigned int, unsigned int>& timeSeg );
	bool updateList( const QVector< QPair<unsigned int, unsigned int> >& timeSegment );

private:
    QVBoxLayout   *verticalLayout;
    QLabel        *label;
    QHBoxLayout   *horizontalLayout;
    QDateTimeEdit *m_dateTimeEditEnd;
    QLabel        *labelTo;
    QDateTimeEdit *m_dateTimeEditStart;
    QPushButton   *m_pushButtonRetrieval;
    QListWidget   *m_listWidgetTimeSeg;

	QVector< QPair<unsigned int, unsigned int> > m_timeSegment;

	unsigned int getIntTime( QDateTimeEdit *timeEditEnd );
	void initListWidget();

private slots:

	void OnButtonRetrieval();
	void OnitemDoubleClicked(QListWidgetItem* item);

};
 
#endif    //TIMESEGMENTWIDGET_HPP_
 
