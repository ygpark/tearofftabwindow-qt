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

    m_tabwidget = new CQTabWidget();
    this->setCentralWidget(m_tabwidget);
    this->setFocus();

    m_btnAddTab = new QPushButton("+", this);
    connect(m_btnAddTab, SIGNAL(clicked()), this, SLOT(slotAddTabButton_clicked()));
    m_tabwidget->setCornerWidget(m_btnAddTab, Qt::TopLeftCorner);

    this->setAttribute(Qt::WA_DeleteOnClose, true);
}



void MainWindow::addTab(Form *widget)
{
    int index = m_tabwidget->addTab(widget, widget->getTabName());
    m_tabwidget->setCurrentIndex(index);
}



/**
 * @brief MainWindow의 부모를 설정한다.
 * parent의 용도는 새로운 MainWindow가 만들어지자마자 부모 MainWindow로 합쳐지는 것을
 * 방지하기 위해 사용된다.
 *
 * @param parent MainWindow(this)를 생성한 부모객체
 * @see MainWindow::getParentMainWindow(), CQTabWidget::slotTabDetachRequested()
 *
 */
void MainWindow::setParentMainWindow(MainWindow *parent)
{
    m_parentMainWindow = parent;
}



/**
 * @brief MainWindow를 만든 부모를 리턴한다.
 * @return MainWindow
 * @see MainWindow::setParentMainWindow(), CQTabWidget::slotTabDetachRequested()
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
CQTabWidget* MainWindow::getTabWidget()
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
