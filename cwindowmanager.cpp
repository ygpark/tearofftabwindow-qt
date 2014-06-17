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

    removeEmptyWindow();
    return CWindowManager::m_instance;
}



QSet<MainWindow*>* CWindowManager::items()
{
    return &m_list;
}



/**
 * @brief 빈 메인윈도우를 지운다.
 */
void CWindowManager::removeEmptyWindow()
{
    QSetIterator<MainWindow*> iter(m_list);
    while(iter.hasNext()) {
        MainWindow* i = iter.next();
        if(i->m_tabwidget->count() == 0 || i->isHidden()) {
            m_list.remove(i);
            i->setAttribute(Qt::WA_DeleteOnClose, true);
            i->hide();
            i->deleteLaterSafe();
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
