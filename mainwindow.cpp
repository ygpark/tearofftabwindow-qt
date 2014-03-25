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

    this->addTab(qobject_cast<Form*>(form));
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
    CWindowManager::garbageCollection();
}

void MainWindow::Initialize()
{
    m_tabwidget = NULL;

    m_tabwidget = new CQTabWidget(NULL);
    m_tabwidget->setParent(this);
    this->setCentralWidget(m_tabwidget);
    this->setFocus();

    // AddTabButton
    m_btnAddTab = new QPushButton("+", this);
    connect(m_btnAddTab, SIGNAL(clicked()), this, SLOT(slotAddTabButton_Clicked()));
}

void MainWindow::addTab(Form *widget)
{
    int index = m_tabwidget->addTab(widget, widget->getTabName());
    m_tabwidget->setCurrentIndex(index);
}

void MainWindow::redrawAddTabButton()
{
    m_btnAddTab->setGeometry(m_tabwidget->m_tabbar->width()+8
                             , m_tabwidget->y()+2
                             , 20
                             , 20);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    this->redrawAddTabButton();
    //this->redrawTabBarWidth();
}

void MainWindow::slotAddTabButton_Clicked()
{
    Form *form = new Form("NewTab");
    this->addTab(form);
}
