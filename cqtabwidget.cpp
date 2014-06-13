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

    m_tabbar = new CQTabBar(this);
    this->setTabBar(m_tabbar);
    this->setMovable(true);
    this->setTabsClosable(true);

    connect(m_tabbar, SIGNAL(tabDetachRequested(int,QPoint&)), this, SLOT(slotTabDetachRequested(int, QPoint&)));
    connect(m_tabbar, SIGNAL(tabCloseRequested(int))  , this, SLOT(slotTabCloseRequested(int)));

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

    m_tabWidth = 200;
    this->setStyleSheet(QString("QTabBar::tab { height: 40px; width: %1px; }").arg(m_tabWidth));
}



CQTabWidget::~CQTabWidget()
{
    if(m_tabbar!=NULL) {
        delete m_tabbar;
    }
}



void CQTabWidget::initialize()
{
    m_tabbar = NULL;
}



void CQTabWidget::MoveTab(int fromIndex, int toIndex)
{
    qDebug() << "CQTabWidget::MoveTab";
    QWidget* w = widget(fromIndex);
    QIcon icon = tabIcon(fromIndex);
    QString text = tabText(fromIndex);

    removeTab (fromIndex);
    insertTab (toIndex, w, icon, text);
    setCurrentIndex (toIndex);
}



void CQTabWidget::slotTabDetachRequested (int index, QPoint& /*dropPoint*/)
{
    QWidget *widgetAtCursor = QApplication::widgetAt(QCursor::pos ());

    MainWindow *mainWindowOnCursor = CWindowManager::findMainWindow(widgetAtCursor);
    MainWindow *myMainwindow = CWindowManager::findMainWindow(this);

    // cursor on the sky
    if(mainWindowOnCursor == NULL) {
        this->attachTabToNewMainwindow(index);
    }else if(mainWindowOnCursor != NULL && mainWindowOnCursor == myMainwindow) {
        // cursor on the my area
        this->attachTabToNewMainwindow(index);
    } else {
        // cursor on the other area
        attachTab(index, mainWindowOnCursor);
    }

    // Remove mainwindows that has no tab.
    CWindowManager::garbageCollection();
}



void CQTabWidget::attachTabToNewMainwindow(int srcTabIndex)
{
    // Create Window
    MainWindow* detachedWidget = new MainWindow (NULL);
    CWindowManager::getInstance()->items()->insert(detachedWidget);

    // Find Widget and connect
    Form* tearOffWidget = dynamic_cast <Form*> (widget (srcTabIndex));
    this->removeTab(srcTabIndex);
    int newIndex = detachedWidget->m_tabwidget->addTab(tearOffWidget, tearOffWidget->getTabName());
    tearOffWidget->show();

    // Make first active
    if (0 < count ())
    {
        setCurrentIndex (0);
    }

    detachedWidget->move(QCursor::pos());
    detachedWidget->show ();

    emit tabAttached(newIndex);
}


/**
 * @brief [slot] 탭의 간격을 계산하여 적절한 넓이로 다시 그린다.
 *        단, 마우스 위치와 관계없이 강제적으로 그린다.
 * @fn CQTabWidget::slotForceUpdateTabWidth
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
 * @fn CQTabWidget::slotUpdateTabWidth
 * @param force
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

    // 강제 옵션이 켜져있지 않다면, 마우스가 탭 위에 올라가 있지 않은 경우에만 사이즈를 조절한다.
    // 그 이유는 탭을 연속적으로 닫을 때 마우스를 옮기지 않기 위해서다.
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
    // tabAttached(int) 시그널을 발생시킨다.
    emit tabAttached(newIndex);

    return newIndex;
}



CQTabBar *CQTabWidget::customTabBar()
{
    return this->m_tabbar;
}



void CQTabWidget::paintEvent(QPaintEvent *event)
{
    this->slotUpdateTabWidth(false);
    QTabWidget::paintEvent(event);
}



void CQTabWidget::resizeEvent(QResizeEvent *event)
{
    this->slotUpdateTabWidth(false);
    QTabWidget::resizeEvent(event);
}



void CQTabWidget::attachTab(int srcTabIndex, MainWindow* mainwindow)
{
    Form* tearOffWidget = dynamic_cast <Form*> (widget (srcTabIndex));
    this->removeTab(srcTabIndex);
    int newIndex = mainwindow->m_tabwidget->addTab(tearOffWidget, tearOffWidget->getTabName());
    tearOffWidget->show();
    emit tabAttached(newIndex);
}



void CQTabWidget::slotTabCloseRequested(int index)
{
    this->removeTab(index);

    CWindowManager::garbageCollection();
    emit tabClosed(index);
}
