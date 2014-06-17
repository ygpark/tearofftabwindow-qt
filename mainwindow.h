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
    void deleteLaterSafe();

    QTabBar* getTabBar();
    QTabWidget* getTabWidget();

private:
    void Initialize();

protected:

public slots:
    void slotAddTabButton_Clicked();
    void slotProcessAfterTabDetached_timeout();
    void slotDeleteLaterSafe_timeout();

private:
    Ui::MainWindow *ui;
public:
    CQTabWidget *m_tabwidget;
    QPushButton *m_btnAddTab;
    QTimer      *m_timerDetachedTab;
    QTimer      *m_timerDeleteLaterSafe;
    bool         m_isMouseTrackingState;
    bool         m_isMousePressed;
};

#endif // MAINWINDOW_H
