#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "customlabel.h"
#include "custombutton.h"
#include "affaireditor.h"
#include "baneditor.h"
#include <QMessageBox>
#include <QJsonParseError>
#include <QJsonObject>
#include <QMenu>
#include <windows.h>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    setWindowFlags(Qt::Tool);
    setAttribute(Qt::WA_QuitOnClose);
    //setWindowIcon(QIcon(":/image/notebook.png"));
    trayIcon=new QSystemTrayIcon(QIcon(":/image/notebook.png"),this);
    QMenu *menu=new QMenu(this);
    QAction *act=new QAction(menu);
    act->setText("退出");
    connect(act,SIGNAL(triggered()),qApp,SLOT(quit()));
    menu->addAction(act);
    trayIcon->setContextMenu(menu);
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_trayIcon_activated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();
    Refresh();
}

MainWidget::~MainWidget()
{
    jsonFile.close();
    delete trayIcon;
    trayIcon=nullptr;
    delete ui;
}

void MainWidget::Refresh()
{
    jsonFile.setFileName(":/data/storage.json");
    if(!jsonFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::about(this, "错误","无法打开json文件读取数据");
        exit(1);
    }
    QJsonParseError parseJsonErr;
    QString jsonStr = jsonFile.readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonStr.toUtf8(), &parseJsonErr);
    if (!(parseJsonErr.error == QJsonParseError::NoError)) {
        QMessageBox::about(this,"错误","json文件解析错误");
        return;
    }
    QJsonObject jsonObject=jsonDocument.object();
    tableItems=jsonObject.value("table").toArray();
    tabooItems=jsonObject.value("taboo").toArray();
    Sort();

    //初始化显示-table
    QPixmap tableImg(":/image/timeline.png");
    QLabel *tableWidget=new QLabel;
    tableWidget->setPixmap(tableImg);
    SetTable(*tableWidget);
    ui->tableScroll->setWidget(tableWidget);

    //初始化显示-taboo
    QImage *tabooImg=new QImage(":/image/forbid.png");
    ui->tabooImage->setPixmap(QPixmap::fromImage(tabooImg->scaled(200,200,Qt::KeepAspectRatio)));
    ui->tabooImage->setAlignment(Qt::AlignCenter);
    QLabel *tabooWidget=new QLabel;
    tabooWidget->resize(530,40*tabooItems.size());
    SetTaboo(*tabooWidget);
    ui->tabooScroll->setWidget(tabooWidget);
}

void MainWidget::SetTable(QWidget &w)
{
    int i;
    for(i=0;i<tableItems.count();i++)
    {
        QJsonObject tmp=tableItems.at(i).toObject();
        QString startTime=tmp.value("start-time").toString();
        QString endTime=tmp.value("end-time").toString();
        QString affair=tmp.value("affair").toString();
        QStringList strList=startTime.split(":");
        int start=60*(strList[0].toInt()+1)+strList[1].toInt();
        strList=endTime.split(":");
        int end=60*(strList[0].toInt()+1)+strList[1].toInt();
        CustomLabel *label=new CustomLabel(&w);
        label->index=i;
        label->setText(affair);
        label->setFrameShape(QFrame::Box);
        label->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 255, 255);background-color: rgb(150,150,150);");
        label->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        if(i%2==0)
        {
            label->setGeometry(43,start,180,end-start);
        }
        else
        {
            label->setGeometry(309,start,180,end-start);
        }
        connect(label,SIGNAL(clicked(int)),this,SLOT(on_affairLabel_clicked(int)));
    }
}

void MainWidget::SetTaboo(QWidget &w)
{
    int i;
    for(i=0;i<tabooItems.count();i++)
    {
        QJsonObject tmp=tabooItems.at(i).toObject();
        QString text=tmp.value("text").toString();
        QWidget *line=new QWidget(&w);
        CustomButton *del=new CustomButton(line);
        del->index=i;
        del->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        del->setText("—");
        del->setFixedHeight(30);
        QLabel *det=new QLabel(line);
        det->setText(text);
        det->setFixedHeight(30);
        if(i==0)
        {
            del->setGeometry(5,5,30,30);
            det->setGeometry(55,5,475,30);
        }
        else
        {
            del->setGeometry(5,i*40,30,30);
            det->setGeometry(55,i*40,475,30);
        }
        connect(del,SIGNAL(clicked(int)),this,SLOT(on_deleteBan_clicked(int)));
        line->show();
    }
}

void MainWidget::on_plusTaboo_clicked()
{
    BanEditor *banEditor=new BanEditor(this);
    banEditor->show();
    banEditor->exec();
    if(banEditor->result==0)
    {
        QString str = banEditor->GetData();
        delete banEditor;
        QJsonObject obj;
        obj.insert("text",str);
        tabooItems.append(obj);
        UpdateJson();
    }
}

void MainWidget::on_deleteBan_clicked(int index)
{
    tabooItems.removeAt(index);
    UpdateJson();
}

void MainWidget::on_plusTable_clicked()
{
    AffairEditor *affairEditor=new AffairEditor(this);
    affairEditor->newlyCreated=true;
    affairEditor->show();

    affairEditor->exec();
    if(affairEditor->result==0)
    {
        QJsonObject obj = affairEditor->GetData();
        delete affairEditor;
        QString s1=obj.value("start-time").toString();
        QString s2=obj.value("end-time").toString();
        if(s1!=s2)
        {
            tableItems.append(obj);
            UpdateJson();
        }
    }
    else
    {
        delete affairEditor;
    }
}

void MainWidget::on_affairLabel_clicked(int index)
{
    AffairEditor *affairEditor=new AffairEditor(this);
    QJsonObject tmp=tableItems.at(index).toObject();
    affairEditor->SetData(tmp.value("start-time").toString(),tmp.value("end-time").toString(),tmp.value("affair").toString());
    affairEditor->show();
    affairEditor->exec();
    if(affairEditor->result==0)
    {
        QJsonObject obj = affairEditor->GetData();
        delete affairEditor;
        tableItems.replace(index,obj);
        UpdateJson();
    }
    else if(affairEditor->result==-1)
    {
        delete affairEditor;
        if(affairEditor->newlyCreated==false)
        {
            tableItems.removeAt(index);
            UpdateJson();
        }
    }
}

void MainWidget::on_trayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason==QSystemTrayIcon::DoubleClick)
    {
        this->showNormal();
        SetWindowPos(HWND(this->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        SetWindowPos(HWND(this->winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        this->show();
    }
}

void MainWidget::UpdateJson()
{
    Sort();
    QJsonObject jo;
    jo.insert("table",tableItems);
    jo.insert("taboo",tabooItems);
    QJsonDocument doc;
    doc.setObject(jo);
    jsonFile.open(QIODevice::Truncate);
    jsonFile.close();
    if(!jsonFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::about(this, "错误","无法打开json文件写入数据");
        exit(1);
    }
    jsonFile.write(doc.toJson());
    jsonFile.flush();
    jsonFile.close();
    Refresh();
}

void MainWidget::Sort()
{
    //start-time、升序
    int i,j;
    for(j=0;j<tableItems.count()-1;j++)
    {
        for(i=j;i<tableItems.count()-1;i++)
        {
            QJsonObject t1=tableItems.at(i).toObject();
            QString time=t1.value("start-time").toString();
            int base1=time.split(":")[0].toInt();
            int base2=time.split(":")[1].toInt();
            QJsonObject t2=tableItems.at(i+1).toObject();
            time=t2.value("start-time").toString();
            int cur1=time.split(":")[0].toInt();
            int cur2=time.split(":")[1].toInt();
            if(cur1<base1)
            {
                tableItems.replace(i+1,t1);
                tableItems.replace(i,t2);
            }
            else if(cur1==base1)
            {
                if(cur2<base2)
                {
                    tableItems.replace(i+1,t1);
                    tableItems.replace(i,t2);
                }
            }
        }
    }
}
