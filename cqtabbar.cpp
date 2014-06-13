#include "cqtabbar.h"
#include "cqtabwidget.h"
#include <QMouseEvent>
#include <QMimeData>
#include <QPainter>
#include <QApplication>
#include <QDrag>
#include <QDebug>
#if QT_VERSION >= 0x050000
#include <QScreen>
#endif



CQTabBar::CQTabBar(QWidget *parent) :
    QTabBar(parent)
{
    initialize();
}

void CQTabBar::initialize()
{
    setAcceptDrops(true);
    //제목을 표시할 공간이 좁으면 우측을 ...으로 표시한다.
    setElideMode(Qt::ElideRight);
    //탭이 제거되었을 때 좌측 탭을 활성화 한다.
    setSelectionBehaviorOnRemove (QTabBar::SelectLeftTab);
    //탭 우측에 종료버튼(x)을 표시한다.
    setMovable (true);
    //이벤트 필터를 설치한다.
    installEventFilter(this);
    setMouseTracking(true);

    m_dragAllowed = false;
}



void CQTabBar::mousePressEvent(QMouseEvent* event)
{
    qDebug() << "CQTabBar::mousePressEvent";

    QTabBar::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        m_dragStartPos = event->pos();
        //드래그가 시작
        m_dragAllowed = true;
    }

}



void CQTabBar::mouseMoveEvent(QMouseEvent* event)
{

    // TODO: 탭이 하나인 경우 예외처리.
    //      현재 윈도우가 마우스를 따라다니게 만든 후 return한다.


    // 마우스가 드래그를 시작한 상태가 아니라면 리턴
    if (!(event->buttons() & Qt::LeftButton) || !m_dragAllowed)
        return;

    // 드래그를 위한 최소 거리 측
    if ((event->pos() - m_dragStartPos).manhattanLength()
            < QApplication::startDragDistance())
        return;

    // 드래그 시
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setData("application/text", "data");
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}


void CQTabBar::dragEnterEvent(QDragEnterEvent* event)
{
    qDebug() << "CQTabBar::dragEnterEvent";
    if(m_dragAllowed) {
        emit tabDetachRequested (tabAt(m_dragStartPos), m_dragDropedPos);
        event->acceptProposedAction();
        m_dragAllowed = false;
    }

    QTabBar::dragEnterEvent(event);
}



void CQTabBar::dragMoveEvent(QDragMoveEvent* event)
{
    qDebug() << "CQTabBar::dragMoveEvent";
    // Only accept if it's an tab-reordering request
//    const QMimeData* m = event->mimeData();
//    QStringList formats = m->formats();
//    if (formats.contains("action") && (m->data("action") == "application/tab-detach"))
//    {
//        m_dragMovedPos = event->pos ();
        event->acceptProposedAction();
//    }
    QTabBar::dragMoveEvent (event);
}



void CQTabBar::dropEvent(QDropEvent* event)
{
    qDebug() << "CQTabBar::dropEvent";
    QTabBar::dropEvent(event);
}



bool CQTabBar::eventFilter(QObject *object, QEvent *event){
    Q_UNUSED(object);
    Q_UNUSED(event);
    return false;
}
