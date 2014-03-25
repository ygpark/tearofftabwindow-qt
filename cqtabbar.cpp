#include "cqtabbar.h"
#include "cqtabwidget.h"
#include <QMouseEvent>
#include <QMimeData>
#include <QPainter>
#include <QApplication>
#include <QDrag>
#include <QScreen>

#include <QDebug>

CQTabBar::CQTabBar(QWidget *parent) :
    QTabBar(parent)
{
    setAcceptDrops(true);
    setElideMode(Qt::ElideRight);
    setSelectionBehaviorOnRemove (QTabBar::SelectLeftTab);
    setMovable (true);
    setStyleSheet("QTabBar::tab { width: 100px; }");
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
    qDebug() << "CQTabBar::mouseMoveEvent";
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
        QDrag* drag = new QDrag(this);
        QMimeData* mimeData = new QMimeData;
        // a crude way to distinguish tab-reordering drops from other ones
        mimeData->setData("action", "application/tab-detach") ;
        drag->setMimeData(mimeData);

        // Create transparent screen dump
        QScreen *screen = QGuiApplication::primaryScreen();
        CQTabWidget *tabWidget = dynamic_cast <CQTabWidget*> (parentWidget ());
        QPixmap pixmap = screen->grabWindow (tabWidget->currentWidget ()->winId ()
                                             , tabWidget->x()
                                             , tabWidget->y());
        // scale down
        if(pixmap.width() > 640 || pixmap.height() > 480) {
            pixmap = pixmap.scaled(640, 480, Qt::KeepAspectRatio);
        }
        QPixmap targetPixmap (pixmap.size ());
        QPainter painter (&targetPixmap);
        painter.setOpacity (0.1);
        painter.drawPixmap (0,0, pixmap);
        painter.end ();
        drag->setPixmap (targetPixmap);
            drag->setHotSpot (QPoint (20, 10));

        // Handle Detach and Move
        Qt::DropAction dragged = drag->exec (Qt::CopyAction);
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
