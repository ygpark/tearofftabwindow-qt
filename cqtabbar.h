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
    QPoint getSelectedGlobalPos();
    
protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void tabDetachRequested (int index);

protected slots:
    void slotEventLoop_timeout();
    void slotTabMoved(int from, int to);

private:
    int          m_selectedTabIndex;
    // 탭을 드래그 할 때 클릭한 지점으로부터 x축으로 일정간격 이상 멀어지면 탭이 떼어지지 않게 하기 위해 사용한다.
    QPoint       m_selectedGlobalPos;
    // 메인윈도우 좌상단부터 마우스커서까지의 거리.
    QPoint       m_distanceFromMainWindowLeftTopToCursor;
    QTimer      *m_eventLoop;
};

#endif // CQTABBAR_H
