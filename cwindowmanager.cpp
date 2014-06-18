#include "cwindowmanager.h"

/* Local */
#include "mainwindow.h"
#include "cqtabwidget.h"

/* Qt */
#include <QTimer>
#include <QTabBar>
#include <QTabWidget>
#include <QApplication>
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



MainWindow* CWindowManager::findMainWindowOf(QWidget *widget)
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



/**
 * @brief 스크린 상의 모든 MainWindow 중에서 마우스가 탭영역(1,2,3)에 올라가있는 메인윈도우를 찾는다.
 *
 * @param except 제외할 메인 윈도우.
 * @return MainWindow
 *
 */
MainWindow* CWindowManager::findMainWindowByCursorOnTabWithout(MainWindow* except)
{
    MainWindow* windowToGo = 0;
    QWidgetList list = qApp->topLevelWidgets();
    QWidgetList::iterator it;

    // 모든 MainWindow를 순회하면서 마우스가 탭 영역에 올라가있는 MainWindow를 찾는다.
    for(it = list.begin(); it != list.end(); ++it) {
        MainWindow *item = reinterpret_cast<MainWindow*>(*it);
        if(except && except == item) {
            continue;
        }

        if(CWindowManager::isCursorOnTabWithEmptyArea(item)) {
            windowToGo = item;
            break;
        }
    }
    return windowToGo;
}

MainWindow *CWindowManager::findMainWindowByCursorOnTab()
{
    return CWindowManager::findMainWindowByCursorOnTabWithout(0);
}



/**
 * @brief 마우스 커서가 MainWindow의 탭영역(1,2,3)에 올라가있는지 확인한다.
 *
 *   +-------------------------------+-+-+-+
 *   |   TITLE BAR                   |_|O|X|
 *   +----------+----------+---------+-+-+-+
 *   |##1(Tab)##|##2(Tab)##|###3(Empty)####|
 *   +----------+----------+---------------+
 *   |                                     |
 *   |                   4                 |
 *   |                                     |
 *   +-------------------------------------+
 *
 *   TabBar::geometry()는 1,2영역을 나타내고, TabWidget::geometry()는 1,2,3,4영역을 나타낸다.
 *   이 두 geometry()를 연산해서 1,2,3 영역을 찾는다.
 *
 * @param mainWindow 대상 메인 윈도
 * @return bool
 *
 */
bool CWindowManager::isCursorOnTabWithEmptyArea(MainWindow *mainWindow)
{
    QRect barRect;
    QRect barRectG;
    QRect widgetRect;
    QRect widgetRectG;

    QTabBar* tabBar = 0;
    QTabWidget* tabWidget = 0;

    if(!mainWindow)
        return false;

    tabBar = mainWindow->getTabBar();
    tabWidget = mainWindow->getTabWidget();

    barRect = tabBar->geometry();
    widgetRect = tabWidget->geometry();

    barRectG.setTopLeft(tabBar->mapToGlobal(barRect.topLeft()));
    barRectG.setBottomRight(tabBar->mapToGlobal(barRect.bottomRight()));

    widgetRectG.setTopLeft(tabWidget->mapToGlobal(widgetRect.topLeft()));
    widgetRectG.setBottomRight(tabWidget->mapToGlobal(widgetRect.bottomRight()));

    widgetRectG.setTopLeft(QPoint(widgetRectG.topLeft().x(), barRectG.topLeft().y()));
    widgetRectG.setBottomRight(QPoint(widgetRectG.bottomRight().x(), barRectG.bottomRight().y()));

    return widgetRectG.contains(QCursor::pos());
}
