#include "cqtabwidget.h"
#include "cqtabbar.h"
#include "mainwindow.h"
#include "cwindowmanager.h"
#include <QApplication>
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

    m_debug = "CQTabWidget";
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
    //지가 지꺼위에서는 안들어가게 처리
    // Check if it is on the other window,

    QWidget *widgetAtCursor = QApplication::widgetAt(QCursor::pos ());

    MainWindow *mainwindow = CWindowManager::findMainWindow(widgetAtCursor);

    // Then move the tab to the window.
    if(mainwindow == NULL) {
        this->attachTabToNewMainwindow(index);
    }else if(mainwindow != NULL) {
        // move it.
        attachTab(index, mainwindow);
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
    QWidget* tearOffWidget = dynamic_cast <QWidget*> (widget (srcTabIndex));
    this->removeTab(srcTabIndex);
    detachedWidget->m_tabwidget->addTab(tearOffWidget, QString("new"));
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
    QWidget* tearOffWidget = dynamic_cast <QWidget*> (widget (srcTabIndex));
    this->removeTab(srcTabIndex);
    mainwindow->m_tabwidget->addTab(tearOffWidget, QString("new"));
    tearOffWidget->show();

    // if the source form is empty, then close.
    QWidget *srcWidget = this->parentWidget();
    MainWindow *srcForm = NULL;
    while(srcWidget != NULL) {
        if((srcForm = qobject_cast<MainWindow*>(srcWidget)) != NULL &&
                srcForm->m_tabwidget->count() == 0){
            QTimer::singleShot(10, srcForm, SLOT(close()));
        }
        srcWidget=srcWidget->parentWidget();
    }
}

void CQTabWidget::slotTabCloseRequested(int index)
{
    this->removeTab(index);
    if(this->count()==0)
        CWindowManager::findMainWindow(this)->close();
}
