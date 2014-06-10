#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT
    
public:
    explicit Form(QWidget *parent = 0);
    explicit Form(QString tabName, QWidget *parent = 0);
    virtual ~Form();
    void initialize();
    QString getTabName();
    void setTabName(QString tabName);
    
protected:
    virtual void mouseMoveEvent(QMouseEvent* event);

private slots:

signals:
    void mouseMoved();

private:
    Ui::Form *ui;
    QString tabName;
};

#endif // FORM_H
