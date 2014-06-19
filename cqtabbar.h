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
    
signals:
    
public slots:
    
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    // Detach Tab
    void tabDetachRequested (int index);
    void moveMainWindowRequested();
    // Move Tab
    void OnMoveTab (int fromIndex, int toIndex);

protected slots:
    void slotEventLoop_timeout();

private:
    QPoint       m_dragDropedPos;

    /* mousePressEvent() 발생하면 true가 되고 dragEnterEvent()가 발생하면 false가 된다.
     * 초기값은 false.
     */
    int          m_dragCurrentIndex;
    int          m_selectedTabIndex;

    QTimer      *m_eventLoop;
    QPoint       m_selectedPos;
    /*
     * mouseMoveEvent가 끝나는 시간을 측정하기 위한 시간 측정기
     * 탭순서를 좌우로 반복적으로 이동중일 때 탭이 떨어져 나오면 mouseMoveEvent와 충돌이 나므로
     * mouseMoveEvent가 끝난 후 일정 시간이 흐르면 창을 떼어낸다.
     */
    QTime       *m_time;
};

#endif // CQTABBAR_H
