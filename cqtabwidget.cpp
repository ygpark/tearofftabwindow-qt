#include "cqtabwidget.h"
#include "cqtabbar.h"
#include "mainwindow.h"
#include "form.h"
#include "cwindowmanager.h"
#include <QApplication>
#include <QFile>
#include <QTimer>
#include <QPushButton>
#include <QMouseEvent>
#include <QDebug>



CQTabWidget::CQTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    this->initialize();

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

    // dummy button for margin
    QPushButton * paddingRight = new QPushButton();
    paddingRight->setGeometry(0,0,200,paddingRight->height());
    paddingRight->hide();
    this->setCornerWidget(paddingRight, Qt::TopRightCorner);

    this->slotUpdateTabWidth(true);

    m_tabWidth = 200;
    this->setStyleSheet(QString("QTabBar::tab { height: 40px; width: %1px; }").arg(m_tabWidth));
}



CQTabWidget::~CQTabWidget()
{
    if(m_tabbar!=NULL) {
        delete m_tabbar;
    }
}



void CQTabWidget::initialize()
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
    int newIndex = detachedWidget->m_tabwidget->addTab(tearOffWidget, tearOffWidget->getTabName());
    tearOffWidget->show();

    // Make first active
    if (0 < count ())
    {
        setCurrentIndex (0);
    }

    detachedWidget->move(QCursor::pos());
    detachedWidget->show ();

    emit tabAttached(newIndex);
}



void CQTabWidget::slotUpdateTabWidth()
{
    slotUpdateTabWidth(true);
}



void CQTabWidget::slotUpdateTabWidth(bool force)
{
    int margin = 0;
    int count;
    QWidget* topRightCornerWidget = 0;
    QWidget* topLeftCornerWidget = 0;

    count = (this->count() == 0) ? 1 : this->count();
    topRightCornerWidget = this->cornerWidget(Qt::TopRightCorner);
    topLeftCornerWidget = this->cornerWidget(Qt::TopLeftCorner);

    if(topRightCornerWidget)
        margin += topRightCornerWidget->width();
    if(topLeftCornerWidget)
        margin += topLeftCornerWidget->width();

    m_tabWidth = (this->width()- margin) / count;

    if(m_tabWidth > 200 || m_tabWidth < 0) {
        m_tabWidth = 200;
    }

    if(!this->customTabBar()->underMouse() || force) {
        this->setStyleSheet(QString("QTabBar::tab { height: 40px; width: %1px; }").arg(m_tabWidth));
    }
}



int CQTabWidget::addTab(Form *widget, const QString &tabName)
{
    int newIndex = QTabWidget::addTab(widget, tabName);
    connect(widget, SIGNAL(mouseMoved()), this, SLOT(slotUpdateTabWidth()));
    slotUpdateTabWidth();

    emit tabAttached(newIndex);
    return newIndex;
}



CQTabBar *CQTabWidget::customTabBar()
{
    return this->m_tabbar;
}



void CQTabWidget::paintEvent(QPaintEvent *event)
{
    this->slotUpdateTabWidth(false);
    QTabWidget::paintEvent(event);
}



void CQTabWidget::resizeEvent(QResizeEvent *event)
{
    this->slotUpdateTabWidth(false);
    QTabWidget::resizeEvent(event);
}



void CQTabWidget::attachTab(int srcTabIndex, MainWindow* mainwindow)
{
    Form* tearOffWidget = dynamic_cast <Form*> (widget (srcTabIndex));
    this->removeTab(srcTabIndex);
    int newIndex = mainwindow->m_tabwidget->addTab(tearOffWidget, tearOffWidget->getTabName());
    tearOffWidget->show();
    emit tabAttached(newIndex);
}



void CQTabWidget::slotTabCloseRequested(int index)
{
    this->removeTab(index);

    CWindowManager::garbageCollection();
    emit tabClosed(index);
}
