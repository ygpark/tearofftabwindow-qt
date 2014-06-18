#ifndef CQTABBAR_H
#define CQTABBAR_H

#include <QTabBar>

class QTimer;

class CQTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit CQTabBar(QWidget *parent = 0);
    void initialize();
    virtual ~CQTabBar();
    
signals:
    
public slots:
    
protected:
    void mousePressEvent(QMouseEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

signals:
    // Detach Tab
    void tabDetachRequested (int index);
    void moveMainWindowRequested();
    // Move Tab
    void OnMoveTab (int fromIndex, int toIndex);

protected slots:
    void slotTimer_timeout();

private:
    QPoint       m_dragDropedPos;

    /* mousePressEvent() 발생하면 true가 되고 dragEnterEvent()가 발생하면 false가 된다.
     * 초기값은 false.
     */
    int          m_dragCurrentIndex;
    int          m_selectedTabIndex;

    QTimer      *m_timer;

};

#endif // CQTABBAR_H
