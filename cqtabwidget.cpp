﻿#include "cqtabwidget.h"
#include "cqtabbar.h"
#include "mainwindow.h"
#include "form.h"
#include "cwindowmanager.h"
#include <QApplication>
#include <QFile>
#include <QTimer>
#include <QDebug>

CQTabWidget::CQTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    this->Initialize();

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

}

CQTabWidget::~CQTabWidget()
{
    if(m_tabbar!=NULL) {
        delete m_tabbar;
    }
}

void CQTabWidget::Initialize()
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
    detachedWidget->m_tabwidget->addTab(tearOffWidget, tearOffWidget->getTabName());
    tearOffWidget->show();

    // Make first active
    if (0 < count ())
    {
        setCurrentIndex (0);
    }

    detachedWidget->move(QCursor::pos());
    detachedWidget->show ();
}

void CQTabWidget::attachTab(int srcTabIndex, MainWindow* mainwindow)
{
    Form* tearOffWidget = dynamic_cast <Form*> (widget (srcTabIndex));
    this->removeTab(srcTabIndex);
    mainwindow->m_tabwidget->addTab(tearOffWidget, tearOffWidget->getTabName());
    tearOffWidget->show();
}

void CQTabWidget::slotTabCloseRequested(int index)
{
    this->removeTab(index);
    if(this->count()==0)
        CWindowManager::findMainWindow(this)->close();
}