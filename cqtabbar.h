#ifndef CQTABBAR_H
#define CQTABBAR_H

#include <QTabBar>

class QTimer;
class QTime;

class CQTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit CQTabBar(QWidget *parent = 0);
    void initialize();
    virtual ~CQTabBar();

    QPoint getDistanceFromMainWindowLeftTopToCursor();
    
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void tabDetachRequested (int index);

protected slots:
    void slotEventLoop_timeout();
    void slotTabBar_tabMoved(int from, int to);
    void slotTabBar_currentChanged(int index);

private:
    int          m_selectedTabIndex;
    // 메인윈도우 좌상단부터 마우스커서까지의 거리.
    QPoint       m_distanceFromMainWindowLeftTopToCursor;
    QTimer      *m_eventLoop;
};

#endif // CQTABBAR_H
