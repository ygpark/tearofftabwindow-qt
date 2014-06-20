#ifndef CQTABWIDGET_H
#define CQTABWIDGET_H

#include <QTabWidget>
#include <QResizeEvent>


class CQTabBar;
class MainWindow;
class Form;
class QTimer;


class CQTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit CQTabWidget(QWidget *parent = 0);
    virtual ~CQTabWidget();

    virtual int addTab(Form *widget, const QString &);

    CQTabBar* customTabBar();

    void startEventLoop();
    void stopEventLoop();

protected:

    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent (QResizeEvent *event);

private:

    void initialize();

    void attachTab(int srcTabIndex, MainWindow* mainwindow);

public slots:

    void slotTabDetachRequested (int index);
    void slotForceUpdateTabWidth();
    void slotUpdateTabWidth(bool force);
    void slotTabCloseRequested(int index);
    void slotEventLoop_timeout();

public:
    CQTabBar *m_tabbar;
    QTimer   *m_eventLoop;
    int       m_tabWidth;
};

#endif // CQTABWIDGET_H
