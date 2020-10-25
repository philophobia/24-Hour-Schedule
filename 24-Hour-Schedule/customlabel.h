#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QMouseEvent>

class CustomLabel : public QLabel
{
    Q_OBJECT

public:
    explicit CustomLabel(QWidget *parent);
    int index;
protected:
    virtual void mouseReleaseEvent(QMouseEvent * e);
signals:
    void clicked(int);
};

#endif // CUSTOMLABEL_H
