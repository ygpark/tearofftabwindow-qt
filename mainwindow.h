#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CQTabWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(QWidget *form, QWidget *parent);
    virtual ~MainWindow();
    virtual void closeEvent (QCloseEvent * event);

private:
    void Initialize();
    
public slots:
    void pushbutton_OnClick();

private:
    Ui::MainWindow *ui;
public:
    CQTabWidget *m_tabwidget;
};

#endif // MAINWINDOW_H
