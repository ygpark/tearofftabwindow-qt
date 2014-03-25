#include "mainwindow.h"
#include "form.h"
#include "cwindowmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Form *form = new Form("TabName");
    MainWindow w(form, NULL);
    w.show();

    CWindowManager *list = CWindowManager::getInstance();
    list->items()->insert(&w);

    
    return a.exec();
}
