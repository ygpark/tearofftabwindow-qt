#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Local */
#include "cqtabbar.h"
#include "cqtabwidget.h"
#include "cwindowmanager.h"
#include "form.h"

/* Qt */
#include <QApplication>
#include <QToolButton>
#include <QToolBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->Initialize();
}



MainWindow::MainWindow(Form *form, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->Initialize();

    this->addTab(form);
}



MainWindow::~MainWindow()
{
    qDebug() << "MainWindow::~MainWindow";
    delete ui;
}



void MainWindow::Initialize()
{
    m_tabwidget = NULL;
    m_isMouseTrackingState = false;

    m_tabwidget = new CQTabWidget(NULL);
    m_tabwidget->setParent(this);
    this->setCentralWidget(m_tabwidget);
    this->setFocus();

    // AddTabButton
    m_btnAddTab = new QPushButton("+", this);
    connect(m_btnAddTab, SIGNAL(clicked()), this, SLOT(slotAddTabButton_Clicked()));
    m_tabwidget->setCornerWidget(m_btnAddTab, Qt::TopLeftCorner);

    // 탭을 떼어난 후에 끌고다니기 위한 타이머
    m_timerDetachedTab = new QTimer(this);
    connect(m_timerDetachedTab, SIGNAL(timeout()), this, SLOT(slotProcessAfterTabDetached_timeout()));

    m_timerDeleteLaterSafe = new QTimer(this);
    connect(m_timerDeleteLaterSafe, SIGNAL(timeout()), this, SLOT(slotDeleteLaterSafe_timeout()));

    this->setAttribute(Qt::WA_DeleteOnClose, true);
}



void MainWindow::addTab(Form *widget)
{
    int index = m_tabwidget->addTab(widget, widget->getTabName());
    m_tabwidget->setCurrentIndex(index);
}



/**
 * @brief 왼쪽 마우스 버튼이 클릭된 상태라면 메인 윈도우가 마우스를 따라다니게 만든다.
 *        이것은 eventFilter를 통하여 구현되었다.
 */
void MainWindow::startMouseTracking()
{
    m_isMouseTrackingState = true;
    m_timerDetachedTab->start(10);
}

void MainWindow::stopMouseTracking()
{
    m_isMouseTrackingState = false;
    m_timerDetachedTab->stop();
}



/**
 * @brief 안전하게 윈도우를 닫는다.
 * 자기자신을 delete하면 충돌이 발생할 수 있기 때문에 QTimer를 사용해서 일정 시간
 * 뒤에 close()를 호출한다. 윈도우에 Qt::WA_DeleteOnClose 속성을 설정했기 때문에
 * 윈도우의 close() 메소드가 호출되면 자동으로 delete된다.
 *
 */

void MainWindow::deleteLaterSafe()
{
    m_timerDeleteLaterSafe->start(50);
}

QTabBar* MainWindow::getTabBar()
{
    return m_tabwidget->tabBar();
}

QTabWidget* MainWindow::getTabWidget()
{
    return m_tabwidget;
}



void MainWindow::slotAddTabButton_Clicked()
{
    static int n = 0;
    QString name = QString("NewTab %1").arg(n++);
    Form *form = new Form(name);
    this->addTab(form);
}


/**
 * @brief 탭을 떼어낸 후에 일어나야 될 일들을 처리한다.
 *
 *  [탭바(TabBar)의 영역(geometry)을 구하는 방법]
 *
 *   QTabBar의 geometry는 Tab1과 Tab2를 합친 영역이다. 하지만 탭을 드랍할 영역은 Empty Area까지 포함해야한다.
 *   왜냐하면 좌우에 빈 공간이 생기기 때문이다.
 *   이 빈 공간을 없애기 위해 탭위젯의 영역(geometry)를 사용한다.
 *   탭위젯의 영역은 탭과 탭 내부의 폼까지 포함하므로 사실상 메인윈도우 전체라고 해도 무방하다.
 *   결론적으로 탭위젯의 영역에서 탭바의 y축을 비교하여 교집합만 얻는다.
 *
 *
 *   +-------------------------------------+
 *   | TitleBar                            |
 *   +-------------------------------------+  --+-------------------\
 *   | Tab1 | Tab2 |      Empty Area       |     \                   \
 *   +-------------------------------------+  ----> TabBar.Y axis     \
 *   |                                     |                           \
 *   |                                     |                            > TabWidget.Y axis
 *   |                                     |                           /
 *   +-------------------------------------+  ------------------------+
 *
 *
 */
void MainWindow::slotProcessAfterTabDetached_timeout()
{
    // 윈도우를 이동시킨다. 이 때 좌표를 약간 이동하여 마치 탭을 드래그하는 효과를 준다.
    if(m_isMouseTrackingState) {
        this->move(QCursor::pos().x()-190, QCursor::pos().y()-50);
    }

    // 탭이 이동할 타겟 윈도우 상단의 TabBar 영역에 마우스 커서가 올라가면 드래그중인 탭을 타겟 윈도우에 붙인다.
    MainWindow *windowToGo = 0;
    QRect tabRegionToGo;
    QRect globalTabRegionToGo;
    QRect tabWidgetRegionToGo;
    QRect globalTabWidgetRegionToGo;
    QWidgetList list = qApp->topLevelWidgets();
    QWidgetList::iterator it;
    QTabBar* tabBar = 0;
    QTabWidget* tabWidget = 0;

    // 모든 MainWindow를 순회하면서
    for(it = list.begin(); it != list.end(); ++it) {
        MainWindow *item = reinterpret_cast<MainWindow*>(*it);
        if(this == item) {
            continue;
        }
        tabBar = item->getTabBar();
        tabWidget = item->getTabWidget();

        tabRegionToGo = tabBar->geometry();
        tabWidgetRegionToGo = tabWidget->geometry();

        globalTabRegionToGo.setTopLeft(tabBar->mapToGlobal(tabRegionToGo.topLeft()));
        globalTabRegionToGo.setBottomRight(tabBar->mapToGlobal(tabRegionToGo.bottomRight()));

        globalTabWidgetRegionToGo.setTopLeft(tabWidget->mapToGlobal(tabWidgetRegionToGo.topLeft()));
        globalTabWidgetRegionToGo.setBottomRight(tabWidget->mapToGlobal(tabWidgetRegionToGo.bottomRight()));

        globalTabWidgetRegionToGo.setTopLeft(QPoint(globalTabWidgetRegionToGo.topLeft().x(), globalTabRegionToGo.topLeft().y()));
        globalTabWidgetRegionToGo.setBottomRight(QPoint(globalTabWidgetRegionToGo.bottomRight().x(), globalTabRegionToGo.bottomRight().y()));

        if(globalTabWidgetRegionToGo.contains(QCursor::pos())) {
            windowToGo = item;
        }
    }

    if(windowToGo != 0) {
        stopMouseTracking();
        windowToGo->addTab(reinterpret_cast<Form*>(m_tabwidget->widget(0)));
        windowToGo->raise();
        windowToGo->activateWindow();
        CWindowManager::removeEmptyWindow();
        return;
    }

    // 마우스 좌측 버튼이 릴리즈되면 이동을 중지한다.
    if(QApplication::mouseButtons() != Qt::LeftButton) {
        stopMouseTracking();
    }
}



/**
 * @brief m_timerDeleteLaterSafe메소드의 timeout() 이벤트 핸들러
 *
 * 안전하게 창을 닫기위해 사용한다.
 *
 * 탭이 attach된 후 마우스가 클릭된 상태로 메인윈도우를 delete하면
 * 충돌이 발생하는데 이걸 막기위해 추가되었다.
 *
 * @see MainWindow::deleteLaterSafe(), QWidget::setAttribute()
 */
void MainWindow::slotDeleteLaterSafe_timeout()
{
    if(qApp->mouseButtons() == Qt::NoButton) {
        this->close();
    }
}
