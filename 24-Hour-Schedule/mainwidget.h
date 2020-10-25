#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QFile>
#include <QJsonArray>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void Refresh();

private slots:
    void on_plusTable_clicked();
    void on_plusTaboo_clicked();
    void on_deleteBan_clicked(int);
    void on_affairLabel_clicked(int);
    void on_trayIcon_activated(QSystemTrayIcon::ActivationReason);

private:
    Ui::MainWidget *ui;
    QFile jsonFile;
    QJsonArray tableItems;
    QJsonArray tabooItems;
    QSystemTrayIcon *trayIcon;
    void SetTable(QWidget &w);
    void SetTaboo(QWidget &w);
    void UpdateJson();
    void Sort();
};
#endif // MAINWIDGET_H
