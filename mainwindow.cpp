#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cqtabbar.h"
#include "cqtabwidget.h"
#include "cwindowmanager.h"
#include "form.h"
#include <QToolButton>
#include <QToolBar>
#include <QVBoxLayout>
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

    this->addTab(qobject_cast<Form*>(form));
}


MainWindow::~MainWindow()
{
    qDebug() << "MainWindow::~MainWindow";
    delete m_tabwidget;
    delete ui;
}

void MainWindow::closeEvent (QCloseEvent* /*event*/)
{
    CWindowManager::garbageCollection();
}

void MainWindow::Initialize()
{
    m_tabwidget = NULL;

    m_tabwidget = new CQTabWidget(NULL);
    m_tabwidget->setParent(this);
    this->setCentralWidget(m_tabwidget);
    this->setFocus();

    CWindowManager::garbageCollection();
}

void MainWindow::addTab(Form *widget)
{
    m_tabwidget->addTab(widget, widget->getTabName());
}

void MainWindow::pushbutton_OnClick()
{

}
