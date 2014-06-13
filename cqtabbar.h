#ifndef CQTABBAR_H
#define CQTABBAR_H

#include <QTabBar>

class CQTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit CQTabBar(QWidget *parent = 0);
    void initialize();
    
signals:
    
public slots:
    
protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dragMoveEvent(QDragMoveEvent* event);
    virtual void dropEvent(QDropEvent* event);

    bool eventFilter(QObject *object, QEvent *event);
signals:
    // Detach Tab
    void tabDetachRequested (int index, QPoint& dropPoint);
    // Move Tab
    void OnMoveTab (int fromIndex, int toIndex);

private:
    QPoint       m_dragStartPos;
    QPoint       m_dragMovedPos;
    QPoint       m_dragDropedPos;
    //mousePressEvent() 발생하면 true가 되고 dragEnterEvent()가 발생하면 false가 된다.
    //초기값은 false.
    bool         m_dragAllowed;
    int          m_dragCurrentIndex;

};

#endif // CQTABBAR_H
