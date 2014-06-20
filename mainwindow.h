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

    void setParentMainWindow(MainWindow* parent);

    QTabBar*    getTabBar();
    CQTabWidget *getTabWidget();
    MainWindow* getParentMainWindow();

private:
    void initialize();

public slots:
    void slotAddTabButton_clicked();

private:
    Ui::MainWindow *ui;

public:
    CQTabWidget *m_tabwidget;
    QPushButton *m_btnAddTab;
    // 탭이 떨어져나온 MainWindow를 기억하기 위한 객체. 마우스가 Release상태가 되면 NULL이 된다.
    MainWindow  *m_parentMainWindow;
};

#endif // MAINWINDOW_H
