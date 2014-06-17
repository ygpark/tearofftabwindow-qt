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
    //이벤트 필터를 설치한다.
    installEventFilter(this);
    setMouseTracking(true);

    m_dragAllowed = false;
}



/**
 * @brief 소멸자
 */
CQTabBar::~CQTabBar()
{

}



/**
 * @brief mousePressEvent 이벤트 핸들러
 *      왼쪽 버튼으로 탭을 클릭하면 드래그가 시작되었음을 알린다.
 *
 */
void CQTabBar::mousePressEvent(QMouseEvent* event)
{
    QTabBar::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        m_dragStartPos = event->pos();
        //드래그가 시작
        m_dragAllowed = true;
    }
}



void CQTabBar::mouseMoveEvent(QMouseEvent* event)
{
    // 마우스가 드래그를 시작한 상태가 아니라면 리턴
    if (!(event->buttons() & Qt::LeftButton) || !m_dragAllowed)
        return;

    // 드래그를 위한 최소 거리보다 작으면 리턴
    if ((event->pos() - m_dragStartPos).manhattanLength()
            < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setData("application/text", "dummy data");
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
}


void CQTabBar::dragEnterEvent(QDragEnterEvent* event)
{
    QTabBar::dragEnterEvent(event);

    if(m_dragAllowed && this->count() > 1) {
        //event->acceptProposedAction();

        //TODO: 드랍을 자기자신(탭) 위에 하는 경우를 막아야 한다.

        emit tabDetachRequested (tabAt(m_dragStartPos), m_dragDropedPos);
        m_dragAllowed = false;

    } if(m_dragAllowed && this->count() == 1) {
        emit moveMainWindowRequested();
    }
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



bool CQTabBar::eventFilter(QObject *object, QEvent *event){
    Q_UNUSED(object);
    Q_UNUSED(event);
    return false;
}
