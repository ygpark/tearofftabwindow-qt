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
}

Form::Form(QString tabName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    this->tabName = tabName;
}

Form::~Form()
{
    delete ui;
}

void Form::pushbutton_OnClicked()
{
    MainWindow* window = CWindowManager::findMainWindow(this);
    Form *form = new Form("NewTab");
    window->m_tabwidget->addTab(form, form->getTabName());
}

QString Form::getTabName()
{
    return this->tabName;
}

void Form::setTabName(QString tabName)
{
    this->tabName = tabName;
}
