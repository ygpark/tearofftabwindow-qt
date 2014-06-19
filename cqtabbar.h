#ifndef CQTABBAR_H
#define CQTABBAR_H

#include <QTabBar>

class QTimer;
class QTime;

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
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void tabDetachRequested (int index);

protected slots:
    void slotEventLoop_timeout();

private:
    int          m_selectedTabIndex;
    QPoint       m_selectedPos;
    QTimer      *m_eventLoop;
};

#endif // CQTABBAR_H
