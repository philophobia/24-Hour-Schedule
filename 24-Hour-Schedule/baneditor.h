#ifndef BANEDITOR_H
#define BANEDITOR_H

#include <QWidget>
#include <QEventLoop>
#include <QCloseEvent>

namespace Ui {
class BanEditor;
}

class BanEditor : public QWidget
{
    Q_OBJECT

public:
    explicit BanEditor(QWidget *parent = nullptr);
    ~BanEditor();
    void exec();
    QString GetData();
    int result = 1;

protected:
	void closeEvent(QCloseEvent *e);

private slots:
    void on_confirm_clicked();

private:
    Ui::BanEditor *ui;
    QEventLoop *loop;
};

#endif // BANEDITOR_H
