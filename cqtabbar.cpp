﻿#include "cqtabbar.h"

/* Local */
#include "cqtabwidget.h"
#include "cwindowmanager.h"
#include "mainwindow.h"

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
    //제목을 표시할 공간이 좁으면 우측을 '...'으로 표시한다.
    setElideMode(Qt::ElideRight);
    //탭이 제거되었을 때 좌측 탭을 활성화 한다.
    setSelectionBehaviorOnRemove (QTabBar::SelectLeftTab);
    //탭 우측에 종료버튼(x)을 표시한다.
    setMovable (true);

    m_eventLoop = new QTimer(this);
    connect(m_eventLoop, SIGNAL(timeout()), this, SLOT(slotEventLoop_timeout()));

    connect(this, SIGNAL(tabMoved(int,int)), this, SLOT(slotTabMoved(int,int)));
}



/**
 * @brief 소멸자
 */
CQTabBar::~CQTabBar()
{
}

QPoint CQTabBar::getDistanceFromMainWindowLeftTopToCursor()
{
    return m_distanceFromMainWindowLeftTopToCursor;
}



/**
 * @brief 소멸자
 */
QPoint CQTabBar::getSelectedGlobalPos()
{
    return m_selectedGlobalPos;
}

void CQTabBar::paintEvent(QPaintEvent* event)
{
    QTabBar::paintEvent(event);
    QPainterPath path;
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);
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
        m_selectedGlobalPos = QCursor::pos();

        m_distanceFromMainWindowLeftTopToCursor = QCursor::pos() - CWindowManager::findMainWindowOf(this)->pos();
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
    // 탭이 하나인 경우 이벤트 루프를 중지하고 메인윈도우 이동을 요청한다.
    if(count() == 1 &&
       qApp->mouseButtons() == Qt::LeftButton)
    {
        m_eventLoop->stop();
        MainWindow *mainWindow = CWindowManager::findMainWindowOf(this);
        if(mainWindow) {
            mainWindow->startMouseTracking();
        }
    }


    // 탭이 여러개 있을 경우 탭을 떼어내달라고 요청한다.
    MainWindow *mainWindow = CWindowManager::findMainWindowOf(this);
    if(count() > 1 &&
       qApp->mouseButtons() == Qt::LeftButton &&
       !CWindowManager::isCursorOnTabWithEmptyArea(mainWindow))
    {

        m_eventLoop->stop();

        // mouseMoveEvent를 중지한다. (중요)
        QMouseEvent* finishMoveEvent = new QMouseEvent (QEvent::MouseMove, QPoint(), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
        QTabBar::mouseMoveEvent(finishMoveEvent);
        delete finishMoveEvent;
        finishMoveEvent = NULL;

        emit tabDetachRequested (m_selectedTabIndex);
    }
}



/**
 * @brief 탭의 위치가 바뀐경우 이벤트 루프를 중지한다.
 * 만약 이벤트 루프를 중지하지 않으면 탭의 순서가 바뀌면서 mouseMoveEvent()와 충돌이 발생한다.
 * @param from
 * @param to
 */
void CQTabBar::slotTabMoved(int from, int to)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    m_eventLoop->stop();
}

