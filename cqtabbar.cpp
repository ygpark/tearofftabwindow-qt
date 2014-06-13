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
}



void CQTabBar::mousePressEvent(QMouseEvent* event)
{
    qDebug() << "CQTabBar::mousePressEvent";
    if (event->button() == Qt::LeftButton)
    {
        m_dragStartPos = event->pos();
    }
    m_dragDropedPos.setX (0);
    m_dragDropedPos.setY (0);
    m_dragMovedPos.setX (0);
    m_dragMovedPos.setY (0);

    m_dragInitiated = false;

    QTabBar::mousePressEvent(event);
}



void CQTabBar::mouseMoveEvent(QMouseEvent* event)
{
    // Distinguish a drag
    if ( !m_dragStartPos.isNull () &&
         ((event->pos() - m_dragStartPos).manhattanLength() < QApplication::startDragDistance()) )
    {
        m_dragInitiated = true;
    }

    // The left button is pressed
    // And the move could also be a drag
    // And the mouse moved outside the tab bar
    if ( ((event->buttons() & Qt::LeftButton)) &&
         m_dragInitiated &&
         (!geometry ().contains (event->pos ())))
    {
        // Stop the move to be able to convert to a drag
        {
            QMouseEvent* finishMoveEvent = new QMouseEvent (QEvent::MouseMove, event->pos (), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
            QTabBar::mouseMoveEvent (finishMoveEvent);
            delete finishMoveEvent;
            finishMoveEvent = NULL;
        }

        // Initiate Drag
        //   [Warning] Do not delete drag. or you can see error about Mutex::lock()
        QDrag drag(this);
        QMimeData* mimeData = new QMimeData;
        // a crude way to distinguish tab-reordering drops from other ones
        mimeData->setData("action", "application/tab-detach") ;
        drag.setMimeData(mimeData);


        // Create transparent screen dump

        //-----------------------------------------------------------
        // TODO: 이부분에서 crash되는 버그있음.
        //-----------------------------------------------------------
//#if QT_VERSION >= 0x050000
//        QScreen *screen = QGuiApplication::primaryScreen();
//        CQTabWidget *tabWidget = dynamic_cast <CQTabWidget*> (parentWidget ());
//        QPixmap pixmap = screen->grabWindow (tabWidget->currentWidget ()->winId ()
//                                             , tabWidget->x()
//                                             , tabWidget->y());
//#else
//        QPixmap pixmap = QPixmap::grabWindow (dynamic_cast <CQTabWidget*> (parentWidget ())->currentWidget ()->winId ()).scaled (640, 480, Qt::KeepAspectRatio);
//#endif
//        // scale down
//        if(pixmap.width() > 640 || pixmap.height() > 480) {
//            pixmap = pixmap.scaled(640, 480, Qt::KeepAspectRatio);
//        }
//        QPixmap targetPixmap (pixmap.size ());
//        QPainter painter (&targetPixmap);
//        painter.setOpacity (0.1);
//        painter.drawPixmap (0,0, pixmap);
//        painter.end ();
//        drag.setPixmap (targetPixmap);
        //-----------------------------------------------------------

        //drag->setHotSpot (QPoint (20, 10));

        // Handle Detach and Move
        Qt::DropAction dragged = drag.exec (Qt::CopyAction);
        if (Qt::IgnoreAction == dragged) {
            event->accept ();
            emit tabDetachRequested (tabAt(m_dragStartPos), m_dragDropedPos);
        }
    } else {
        QTabBar::mouseMoveEvent(event);
    }
}



void CQTabBar::dragEnterEvent(QDragEnterEvent* event)
{
    qDebug() << "CQTabBar::dragEnterEvent";
    // Only accept if it's an tab-reordering request
    const QMimeData* m = event->mimeData();
    QStringList formats = m->formats();
    if (formats.contains("action") && (m->data("action") == "application/tab-detach"))
    {
        event->acceptProposedAction();
    }
    QTabBar::dragEnterEvent(event);
}



void CQTabBar::dragMoveEvent(QDragMoveEvent* event)
{
    qDebug() << "CQTabBar::dragMoveEvent";
    // Only accept if it's an tab-reordering request
    const QMimeData* m = event->mimeData();
    QStringList formats = m->formats();
    if (formats.contains("action") && (m->data("action") == "application/tab-detach"))
    {
        m_dragMovedPos = event->pos ();
        event->acceptProposedAction();
    }
    QTabBar::dragMoveEvent (event);
}



void CQTabBar::dropEvent(QDropEvent* event)
{
    qDebug() << "CQTabBar::dropEvent";
    // If a dragged Event is dropped within this widget it is not a drag but
    // a move.
    //m_dragDropedPos = event->pos ();
    QTabBar::dropEvent(event);
}



bool CQTabBar::eventFilter(QObject *object, QEvent *event){
    Q_UNUSED(object);
    Q_UNUSED(event);
    return false;
}
