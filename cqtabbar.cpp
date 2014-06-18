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
#include <QDebug>
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

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTimer_timeout()));
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
        m_timer->start(10);
    }
}



/**
 * @brief
 */
void CQTabBar::slotTimer_timeout()
{
    MainWindow *mainWindow = CWindowManager::findMainWindowOf(this);
    if(!CWindowManager::isCursorOnTabWithEmptyArea(mainWindow)) {

        m_timer->stop();

        // 탭순서를 변경중이었다면 새 탭을 생성하기 위하여 mouseMoveEvent를 중지한다.
        {
            QMouseEvent* finishMoveEvent = new QMouseEvent (QEvent::MouseMove, QPoint(), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
            QTabBar::mouseMoveEvent (finishMoveEvent);
            delete finishMoveEvent;
            finishMoveEvent = NULL;
        }

        if(qApp->mouseButtons() == Qt::LeftButton &&
           this->count() > 1)
        {
            emit tabDetachRequested (m_selectedTabIndex);

        } else if(qApp->mouseButtons() == Qt::LeftButton &&
                  this->count() == 1)
        {
            emit moveMainWindowRequested();
        }


    }
}



/**
 * @brief dragEnterEvent 이벤트 핸들러
 * @param event
 * @see CQTabWindow::slotTabDetachRequested()
 */
void CQTabBar::dragEnterEvent(QDragEnterEvent* event)
{
    qDebug() << "dragEnterEvent(QDragEnterEvent* event)";

    QTabBar::dragEnterEvent(event);


}



void CQTabBar::dragMoveEvent(QDragMoveEvent* event)
{
    QTabBar::dragMoveEvent (event);
    //event->acceptProposedAction();
}



void CQTabBar::dropEvent(QDropEvent* event)
{
    QTabBar::dropEvent(event);
    qDebug() << "CQTabBar::dropEvent";
}
