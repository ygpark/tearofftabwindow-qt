#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

class CQTabWidget;
class Form;
class QPushButton;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(Form *form, QWidget *parent);
    virtual ~MainWindow();

    void addTab(Form *widget);

    void startMouseTracking();
    void stopMouseTracking();
    void setParentMainWindow(MainWindow* parent);
    MainWindow* getParentMainWindow();

    QTabBar* getTabBar();
    QTabWidget* getTabWidget();

private:
    void initialize();

protected:

public slots:
    void slotAddTabButton_clicked();
    void slotProcessAfterTabDetached_timeout();

private:
    Ui::MainWindow *ui;
public:
    CQTabWidget *m_tabwidget;
    QPushButton *m_btnAddTab;
    // 탭이 드래그 중일 때 이벤트를 처리하기 위한 타이머
    QTimer      *m_timerProcessAfterTabDetached;
    // 탭이 떨어져나온 MainWindow를 기억하기 위한 객체. 마우스가 Release상태가 되면 NULL이 된다.
    MainWindow  *m_parentMainWindow;
    bool         m_isMouseTrackingState;
    bool         m_isMousePressed;
};

#endif // MAINWINDOW_H
