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
    static void garbageCollection();
    static MainWindow* findMainWindow(QWidget *widget);

private:
    static QSet<MainWindow*> m_list;
    static CWindowManager *m_instance;
};

#endif // CMAINWINDOWLIST_H
