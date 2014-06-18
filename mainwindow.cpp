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
    this->initialize();

}



MainWindow::MainWindow(Form *form, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initialize();

    this->addTab(form);
}




/**
 * @brief 소멸자
 * @fn MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::initialize()
{
    m_parentMainWindow = 0;

    m_tabwidget = 0;

    m_tabwidget = new CQTabWidget(NULL);
    m_tabwidget->setParent(this);
    this->setCentralWidget(m_tabwidget);
    this->setFocus();
    m_btnAddTab = new QPushButton("+", this);
    connect(m_btnAddTab, SIGNAL(clicked()), this, SLOT(slotAddTabButton_clicked()));
    m_tabwidget->setCornerWidget(m_btnAddTab, Qt::TopLeftCorner);

    m_timerProcessAfterTabDetached = new QTimer(this);
    connect(m_timerProcessAfterTabDetached, SIGNAL(timeout()), this, SLOT(slotProcessAfterTabDetached_timeout()));

    m_timerDeleteLaterSafe = new QTimer(this);
    connect(m_timerDeleteLaterSafe, SIGNAL(timeout()), this, SLOT(slotDeleteLaterSafe_timeout()));

    m_isMouseTrackingState = false;

    this->setAttribute(Qt::WA_DeleteOnClose, true);
}



void MainWindow::addTab(Form *widget)
{
    int index = m_tabwidget->addTab(widget, widget->getTabName());
    m_tabwidget->setCurrentIndex(index);
}



/**
 * @brief 메인 윈도우가 마우스를 따라다니게 만든다. 단 왼쪽 마우스가 클릭된 상태여야 한다.
 */
void MainWindow::startMouseTracking()
{
    m_isMouseTrackingState = true;
    m_timerProcessAfterTabDetached->start(10);
}



/**
 * @brief 마우스 추적을 중지한다.
 * @see MainWindow::startMouseTracking()
 */
void MainWindow::stopMouseTracking()
{
    setParentMainWindow(0);
    m_isMouseTrackingState = false;
    m_timerProcessAfterTabDetached->stop();
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



/**
 * @brief MainWindow의 부모를 설정한다.
 * parent의 용도는 새로운 MainWindow가 만들어지자마자 부모 MainWindow로 합쳐지는 것을
 * 방지하기 위해 사용된다.
 *
 * @param parent MainWindow(this)를 생성한 부모객체
 * @see MainWindow::getParentMainWindow(), CQTabWidget::attachTabToNewMainwindow()
 *
 */
void MainWindow::setParentMainWindow(MainWindow *parent)
{
    m_parentMainWindow = parent;
}



/**
 * @brief MainWindow를 만든 부모를 리턴한다.
 * @return MainWindow
 * @see MainWindow::setParentMainWindow(), CQTabWidget::attachTabToNewMainwindow()
 *
 */
MainWindow *MainWindow::getParentMainWindow()
{
    return m_parentMainWindow;
}



/**
 * @brief QTabWidget이 포함하는 QTabBar을 리턴한다.
 * @return QTabBar
 */
QTabBar* MainWindow::getTabBar()
{
    return m_tabwidget->tabBar();
}



/**
 * @brief QTabWidget을 리턴한다.
 * @return QTabWidget
 */
QTabWidget* MainWindow::getTabWidget()
{
    return m_tabwidget;
}



/**
 * @brief 새 탭을 만드는 버튼의 click이벤트 핸들러
 */
void MainWindow::slotAddTabButton_clicked()
{
    static int n = 0;
    QString name = QString("NewTab %1").arg(n++);
    Form *form = new Form(name);
    this->addTab(form);
}



/**
 * @brief 탭을 떼어낸 후에 일어나야 될 일들을 처리한다.
 */
void MainWindow::slotProcessAfterTabDetached_timeout()
{
    MainWindow *windowToGo = 0;

    // 윈도우를 이동시킨다. 이 때 좌표를 약간 이동하여 마치 탭을 드래그하는 효과를 준다.
    if(m_isMouseTrackingState) {
        this->move(QCursor::pos().x()-190, QCursor::pos().y()-50);
    }

    // 마우스로 드래그 중인 MainWindow의 Tab을 다른 MainWindow에 붙인다.
    if(!CWindowManager::isCursorOnTabWithEmptyArea(this->getParentMainWindow())) {
        windowToGo = CWindowManager::findMainWindowByCursorOnTabWithout(this);
        if(windowToGo != 0) {
            stopMouseTracking();
            windowToGo->addTab(reinterpret_cast<Form*>(m_tabwidget->widget(0)));
            windowToGo->raise();
            windowToGo->activateWindow();
            CWindowManager::removeEmptyWindow();
            return;
        }
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
