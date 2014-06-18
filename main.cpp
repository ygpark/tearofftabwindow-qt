#include "mainwindow.h"
#include "form.h"
#include "cwindowmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Form *form = new Form("TabName");
    MainWindow *w = new MainWindow(form, NULL);
    w->show();

    CWindowManager::getInstance()->items()->insert(w);
    
    return a.exec();
}
