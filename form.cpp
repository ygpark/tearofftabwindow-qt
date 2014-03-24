#include "form.h"
#include "ui_form.h"
#include "mainwindow.h"
#include "cqtabwidget.h"
#include "cwindowmanager.h"
#include <QToolBar>

//Debug
#include <qdebug.h>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
//    QToolBar *toolbar = new QToolBar(this);
//    toolbar->show();
}

Form::~Form()
{
    delete ui;
}

void Form::pushbutton_OnClicked()
{
    //qDebug() << this->parent()->parent()->objectName();
    CWindowManager *windowlist = CWindowManager::getInstance();
    MainWindow* window = qobject_cast<MainWindow*>(*windowlist->items()->begin());
    window->m_tabwidget->addTab(new Form(this), QString("1"));
    //qDebug() << windowlist->items()->at(0)->objectName();
    //MainWindow *mainwindow = dynamic_cast<MainWindow*> (this->parentWidget()->parentWidget());
    //mainwindow->m_tabwidget;//->addTab(new QWidget(this), QString("1"));
    //qDebug() << mainwindow->m_tabwidget->m_debug;//->addTab(new QWidget(this), QString("1"));
}
