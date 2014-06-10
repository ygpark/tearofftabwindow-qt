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
    this->initialize();
}



Form::Form(QString tabName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    this->initialize();

    this->tabName = tabName;
}



Form::~Form()
{
    delete ui;
}



void Form::initialize()
{
    this->setMouseTracking(true);
}



QString Form::getTabName()
{
    return this->tabName;
}



void Form::setTabName(QString tabName)
{
    this->tabName = tabName;
}



void Form::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit mouseMoved();
}
