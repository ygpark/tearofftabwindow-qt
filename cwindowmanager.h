#ifndef CMAINWINDOWLIST_H
#define CMAINWINDOWLIST_H

class QWidget;
class MainWindow;

class CWindowManager
{
private:
    CWindowManager();

public:
    static CWindowManager* getInstance();
    static void insert(MainWindow* mainWindow);
    static MainWindow* findMainWindowOf(QWidget *widget);
    static MainWindow* findMainWindowByCursorOnTabWithout(MainWindow *except);
    static MainWindow* findMainWindowByCursorOnTab();
    static bool isCursorOnTabWithEmptyArea(MainWindow *mainWindow);
    static void removeEmptyWindow();

private:
    static CWindowManager *m_instance;
};

#endif // CMAINWINDOWLIST_H
