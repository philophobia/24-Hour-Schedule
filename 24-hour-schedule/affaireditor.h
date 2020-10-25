#ifndef AFFAIREDITOR_H
#define AFFAIREDITOR_H

#include <QWidget>
#include <QJsonObject>
#include <QEventLoop>
#include <QCloseEvent>
#include <QTime>

namespace Ui {
class AffairEditor;
}

class AffairEditor : public QWidget
{
    Q_OBJECT

public:
    explicit AffairEditor(QWidget *parent = nullptr);
    ~AffairEditor();
    void exec();
    QJsonObject GetData();
    void SetData(QString start,QString end,QString affair);
    int result=1;
    bool newlyCreated=false;

protected:
	void closeEvent(QCloseEvent *e);

private slots:
    void on_confirmButton_clicked();
    void on_deleteButton_clicked();

private:
    Ui::AffairEditor *ui;
    QEventLoop *loop;
};

#endif // AFFAIREDITOR_H
