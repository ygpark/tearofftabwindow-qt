#include "cqtabbar.h"

/* Local */
#include "cqtabwidget.h"
#include "cwindowmanager.h"

/* Qt */
#include <QMouseEvent>
#include <QMimeData>
#include <QPainter>
#include <QApplication>
#include <QDrag>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QThread>
#if QT_VERSION >= 0x050000
#include <QScreen>
#endif


/**
 * @brief 생성자
 */
CQTabBar::CQTabBar(QWidget *parent) :
    QTabBar(parent)
{
    initialize();
}



/**
 * @brief 생성자에서 사용하는 초기화 함
 */
void CQTabBar::initialize()
{
    setAcceptDrops(true);
    //제목을 표시할 공간이 좁으면 우측을 ...으로 표시한다.
    setElideMode(Qt::ElideRight);
    //탭이 제거되었을 때 좌측 탭을 활성화 한다.
    setSelectionBehaviorOnRemove (QTabBar::SelectLeftTab);
    //탭 우측에 종료버튼(x)을 표시한다.
    setMovable (true);

    m_time = new QTime();

    m_eventLoop = new QTimer(this);
    connect(m_eventLoop, SIGNAL(timeout()), this, SLOT(slotEventLoop_timeout()));
}



/**
 * @brief 소멸자
 */
CQTabBar::~CQTabBar()
{
}



/**
 * @brief mousePressEvent 이벤트 핸들러
 *  왼쪽 버튼으로 탭을 클릭하면 드래그가 시작되었음을 알린다.
 *
 */
void CQTabBar::mousePressEvent(QMouseEvent* event)
{
    QTabBar::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        m_selectedTabIndex = tabAt(event->pos());
        m_selectedPos = QCursor::pos();
        m_eventLoop->start(10);
    }
}

void CQTabBar::mouseMoveEvent(QMouseEvent *event)
{
    if(count() > 1) {
        QTabBar::mouseMoveEvent(event);
    }
}



/**
 * @brief 타이머 이벤트 핸들러
 * 탭을 클릭하면 시작한다.
 */
void CQTabBar::slotEventLoop_timeout()
{
    /*****************************************************************
     * 주의!
     *****************************************************************
     * x축으로 100픽셀 이상 이동하면 탭을 떼어내지 못하게 한다.
     * 왜냐하면 mouseMoveEvent를 완전히 중지할 수 없기 때문이다.
     * 아래 조건문에서 QTabBar::mouseMoveEvent (finishMoveEvent) 호출을 통해
     * 이벤트를 중단하려고 하였으나 탭을 잡고 좌우로 흔드는 경우에 이벤트가 중단되지
     * 않아서 충돌이 발생하였다.
     ******************************************************************/
    if(count() > 1 && abs(m_selectedPos.x()- QCursor::pos().x()) > 100) {
        m_eventLoop->stop();
        return;
    }


    // 탭이 하나인 경우 창을 이동한다.
    if(count() == 1 && qApp->mouseButtons() == Qt::LeftButton) {
        m_eventLoop->stop();
        emit moveMainWindowRequested();
    }


    MainWindow *mainWindow = CWindowManager::findMainWindowOf(this);

    if(count() > 1 && qApp->mouseButtons() == Qt::LeftButton &&
            !CWindowManager::isCursorOnTabWithEmptyArea(mainWindow)) {

        m_eventLoop->stop();

        // mouseMoveEvent를 중지한다.
        {
            QMouseEvent* finishMoveEvent = new QMouseEvent (QEvent::MouseMove, QPoint(), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
            QTabBar::mouseMoveEvent (finishMoveEvent);
            delete finishMoveEvent;
            finishMoveEvent = NULL;
        }

        emit tabDetachRequested (m_selectedTabIndex);
    }
}

