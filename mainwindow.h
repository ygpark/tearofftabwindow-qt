#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    virtual void closeEvent (QCloseEvent * event);

    void addTab(Form *widget);

private:
    void Initialize();
    //void redrawAddTabButton();

public slots:
    void slotAddTabButton_Clicked();

private:
    Ui::MainWindow *ui;
public:
    CQTabWidget *m_tabwidget;
    QPushButton *m_btnAddTab;
};

#endif // MAINWINDOW_H
