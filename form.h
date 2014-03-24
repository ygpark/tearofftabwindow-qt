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
    QString getTabName();
    void setTabName(QString tabName);
    
private slots:
    void pushbutton_OnClicked();
private:
    Ui::Form *ui;
    QString tabName;
};

#endif // FORM_H
