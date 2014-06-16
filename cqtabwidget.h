#ifndef CQTABWIDGET_H
#define CQTABWIDGET_H

#include <QTabWidget>
#include <QResizeEvent>



class CQTabBar;
class MainWindow;
class Form;


class CQTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit CQTabWidget(QWidget *parent = 0);
    virtual ~CQTabWidget();

    void initialize();
    virtual int addTab(Form *widget, const QString &);
    CQTabBar* customTabBar();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent (QResizeEvent *event);

private:
    void attachTab(int srcTabIndex, MainWindow* mainwindow);
    void attachTabToNewMainwindow(int srcTabIndex);
    //void setTabWidth(int width = -1);


public slots:
    // Move Tab
    void MoveTab(int fromIndex, int toIndex);
    // Detach Tab
    void slotTabDetachRequested (int index, QPoint&);
    void slotTabCloseRequested(int);
    void slotForceUpdateTabWidth();
    void slotUpdateTabWidth(bool force);
    void slotMoveMainWindowRequested();

signals:
    void tabClosed(int index);
    void tabAttached(int index);


public:
    CQTabBar *m_tabbar;
    int m_tabWidth;
};

#endif // CQTABWIDGET_H
