#include "cwindowmanager.h"

#include "mainwindow.h"
#include "cqtabwidget.h"
#include <QTimer>

#include <QDebug>

//
//static variables
//
QSet<MainWindow*> CWindowManager::m_list;
CWindowManager* CWindowManager::m_instance = NULL;


//
// functions
//
CWindowManager::CWindowManager()
{

}



CWindowManager* CWindowManager::getInstance()
{
    if(CWindowManager::m_instance == NULL)
        CWindowManager::m_instance = new CWindowManager();

    garbageCollection();
    return CWindowManager::m_instance;
}



QSet<MainWindow*>* CWindowManager::items()
{
    return &m_list;
}



void CWindowManager::garbageCollection()
{
    QSetIterator<MainWindow*> iter(m_list);
    while(iter.hasNext()) {
        MainWindow* i = iter.next();
        if(i->m_tabwidget->count() == 0 || i->isHidden()) {
            QTimer::singleShot(10, i, SLOT(close()));
            m_list.remove(i);
        }
    }
}



MainWindow* CWindowManager::findMainWindow(QWidget *widget)
{
    QWidget *parentWidget = widget;
    MainWindow *mainwindow = NULL;
    while(parentWidget != NULL) {
        mainwindow = qobject_cast<MainWindow*>(parentWidget);
        if(mainwindow != NULL){
            break;
        }
        parentWidget = parentWidget->parentWidget();
    }
    return mainwindow;
}
