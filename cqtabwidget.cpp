#include "cqtabwidget.h"
#include "cqtabbar.h"
#include "mainwindow.h"
#include "form.h"
#include "cwindowmanager.h"
#include <QApplication>
#include <QFile>
#include <QTimer>
#include <QPushButton>
#include <QMouseEvent>
#include <QDebug>



CQTabWidget::CQTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    this->initialize();
}



/**
 * @brief 소멸자
 *      - m_tabbar를 delete하면 충돌이 발생한다.
 */
CQTabWidget::~CQTabWidget()
{
    m_tabbar->deleteLater();
}


/**
 * @brief 초기화 함수
 *      - CQTabBar(this)로 초기화하면 소멸자에서 충돌이 발생하므로 주의
 */
void CQTabWidget::initialize()
{
    m_tabWidth = 200;
    m_tabbar = new CQTabBar();

    this->setTabBar(m_tabbar);
    this->setMovable(true);
    this->setTabsClosable(true);

    connect(m_tabbar, SIGNAL(tabDetachRequested(int)),  this, SLOT(slotTabDetachRequested(int)));
    connect(m_tabbar, SIGNAL(tabCloseRequested(int)),   this, SLOT(slotTabCloseRequested(int)));

    // Apply styleSheet
    QFile file(":/css/cqtabwidget.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    this->setStyleSheet(styleSheet);

    // dummy button for margin
    QPushButton * paddingRight = new QPushButton();
    paddingRight->setGeometry(0,0,200,paddingRight->height());
    paddingRight->hide();
    this->setCornerWidget(paddingRight, Qt::TopRightCorner);

    this->slotForceUpdateTabWidth();

    this->setStyleSheet(QString("QTabBar::tab { height: 40px; width: %1px; }").arg(m_tabWidth));
}



void CQTabWidget::slotTabDetachRequested (int index)
{
    // Create Window
    MainWindow* newMainWindow = new MainWindow (NULL);
    newMainWindow->setParentMainWindow(CWindowManager::findMainWindowOf(this));

    // Find Widget and connect
    Form* tearOffWidget = dynamic_cast <Form*> (widget (index));

    /****************************************
     * 주의
     ****************************************
     * 탭순서를 바꾸는 중일 경우(마우스로 좌우이동)
     * 탭이 삭제되면 죽는다.
     ****************************************/
    this->removeTab(index);

    newMainWindow->m_tabwidget->addTab(tearOffWidget, tearOffWidget->getTabName());
    tearOffWidget->show();

    // Make first active
    if (0 < count ())
    {
        setCurrentIndex (0);
    }

    //탭을 떼어낸 후 계속 마우스 버튼이 클릭된 상태라면 메인 윈도우가 마우스를 따라다니게 만든다.
    newMainWindow->startMouseTracking();
    newMainWindow->show ();
}


/**
 * @brief [slot] 탭의 간격을 계산하여 적절한 넓이로 다시 그린다.
 *        단, 마우스 위치와 관계없이 강제적으로 그린다.
 * @see CQTabWidget::slotUpdateTabWidth(bool)
 */
void CQTabWidget::slotForceUpdateTabWidth()
{
    slotUpdateTabWidth(true);
}



/**
 * @brief [slot] 탭의 간격을 계산하여 적절한 넓이로 다시 그린다.
 *        단, 마우스가 탭 위에 올라가 있는 상태라면 다시 그리지 않는다.
 *        왜냐하면 마우스를 옮기지 않고 탭을 연속적으로 닫을 수 있기 때문이다.
 * @param force true면 무조건 탭을 다시 그리고 false면 마우스가 탭 바깥에 있을때문 다시 그린다.
 */
void CQTabWidget::slotUpdateTabWidth(bool force)
{
    int margin = 0;
    int tabNumber;
    QWidget* topRightCornerWidget = 0;
    QWidget* topLeftCornerWidget = 0;

    tabNumber = (this->count() == 0) ? 1 : this->count();
    topRightCornerWidget = this->cornerWidget(Qt::TopRightCorner);
    topLeftCornerWidget = this->cornerWidget(Qt::TopLeftCorner);

    if(topRightCornerWidget)
        margin += topRightCornerWidget->width();
    if(topLeftCornerWidget)
        margin += topLeftCornerWidget->width();

    m_tabWidth = (this->width()- margin) / tabNumber;

    if(m_tabWidth > 200 || m_tabWidth < 0) {
        m_tabWidth = 200;
    }

    if(!this->customTabBar()->underMouse() || force) {
        this->setStyleSheet(QString("QTabBar::tab { height: 40px; width: %1px; }").arg(m_tabWidth));
    }
}



int CQTabWidget::addTab(Form *widget, const QString &tabName)
{
    int newIndex;

    newIndex = QTabWidget::addTab(widget, tabName);

    // 탭을 닫은 후 마우스가 탭위젯 아래 방향으로 내려가면 탭을 다시 그린다.
    // 이유는 마우스가 탭을 벗어났는데도 불구하고 QTabWidget::underMouse()함수가 false를 반환하지
    // 않는 경우가 있기 때문이다. 이 경우는 마우스 위치와 상관없이 탭을 다시 그린다.
    connect(widget, SIGNAL(mouseMoved()), this, SLOT(slotForceUpdateTabWidth()));
    // 폼이 리사이즈되면 탭을 다시 그린다.
    connect(widget, SIGNAL(resized()), this, SLOT(slotForceUpdateTabWidth()));
    // 탭이 새로 추가되면 탭 간격을 다시 조절한다.
    slotForceUpdateTabWidth();

    return newIndex;
}



CQTabBar *CQTabWidget::customTabBar()
{
    return this->m_tabbar;
}



/**
 * @brief paintEvent 이벤트 핸들러
 * 탭 간격을 조절하기 위해 사용한다.
 * @param event
 */
void CQTabWidget::paintEvent(QPaintEvent *event)
{
    this->slotUpdateTabWidth(false);
    QTabWidget::paintEvent(event);
}



/**
 * @brief resizeEvent 이벤트 핸들러
 * 탭 간격을 조절하기 위해 사용한다.
 * @param event
 */
void CQTabWidget::resizeEvent(QResizeEvent *event)
{
    this->slotUpdateTabWidth(false);
    QTabWidget::resizeEvent(event);
}



void CQTabWidget::attachTab(int srcTabIndex, MainWindow* mainwindow)
{
    Form* tearOffWidget = static_cast <Form*>(this->widget(srcTabIndex));
    this->removeTab(srcTabIndex);
    mainwindow->m_tabwidget->addTab(tearOffWidget, tearOffWidget->getTabName());
    tearOffWidget->show();
}



void CQTabWidget::slotTabCloseRequested(int index)
{
    this->removeTab(index);
    CWindowManager::removeEmptyWindow();
}
