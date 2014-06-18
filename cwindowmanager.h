#ifndef CMAINWINDOWLIST_H
#define CMAINWINDOWLIST_H

#include <QSet>

class QWidget;
class MainWindow;

class CWindowManager
{
private:
    CWindowManager();

public:
    static CWindowManager* getInstance();
    static QSet<MainWindow*>* items();
    static void removeEmptyWindow();
    static MainWindow* findMainWindowOf(QWidget *widget);
    static MainWindow* findMainWindowByCursorOnTabWithout(MainWindow *except);
    static MainWindow* findMainWindowByCursorOnTab();
    static bool isCursorOnTabWithEmptyArea(MainWindow *mainWindow);

private:
    static QSet<MainWindow*> m_list;
    static CWindowManager *m_instance;
};

#endif // CMAINWINDOWLIST_H
