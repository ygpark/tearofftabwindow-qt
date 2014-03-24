#ifndef CQTABWIDGET_H
#define CQTABWIDGET_H

#include <QTabWidget>

class CQTabBar;
class MainWindow;

class CQTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit CQTabWidget(QWidget *parent = 0);
    virtual ~CQTabWidget();

    void Initialize();

private:
    void attachTab(int srcTabIndex, MainWindow* mainwindow);
    void attachTabToNewMainwindow(int srcTabIndex);



public slots:
    // Move Tab
    void MoveTab(int fromIndex, int toIndex);

    // Detach Tab
    void slotTabDetachRequested (int index, QPoint&);
    void slotTabCloseRequested(int);

signals:
    void newWindowCreated(QWidget*);

private:
    CQTabBar *m_tabbar;
};

#endif // CQTABWIDGET_H
