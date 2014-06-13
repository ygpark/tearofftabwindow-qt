#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cqtabbar.h"
#include "cqtabwidget.h"
#include "cwindowmanager.h"
#include "form.h"
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
    delete m_btnAddTab;
    delete m_tabwidget;
    delete ui;
}



void MainWindow::closeEvent (QCloseEvent* /*event*/)
{
    CWindowManager::removeEmptyWindow();
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

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotUpdateMainWindowPos()));
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
    m_timer->start(10);
}

void MainWindow::stopMouseTracking()
{
    m_isMouseTrackingState = false;
    m_timer->stop();
}



void MainWindow::slotAddTabButton_Clicked()
{
    Form *form = new Form("NewTab");
    this->addTab(form);
}

void MainWindow::slotUpdateMainWindowPos()
{
    if(m_isMouseTrackingState) {
        this->move(QCursor::pos().x()-190, QCursor::pos().y()-50);
    }

    if(QApplication::mouseButtons() != Qt::LeftButton) {
        stopMouseTracking();
    }
}
