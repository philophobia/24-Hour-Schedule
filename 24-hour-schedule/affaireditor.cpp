#include "affaireditor.h"
#include "ui_affaireditor.h"

AffairEditor::AffairEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AffairEditor)
{
    ui->setupUi(this);
    setWindowModality(Qt:: WindowModal);
    setWindowFlags(Qt::Window|Qt::WindowCloseButtonHint);
    setWindowTitle("编辑时间表");
}

AffairEditor::~AffairEditor()
{
    delete ui;
}

void AffairEditor::exec()
{
    result=1;
    this->show();
	loop = new QEventLoop(this);
	loop->exec();
}

void AffairEditor::closeEvent(QCloseEvent *e)
{
	if(loop!=nullptr)
	{
		loop->exit();
		result=1;
	}
	e->accept();
}

QJsonObject AffairEditor::GetData()
{
    QTime s=ui->startTime->time();
    QTime e=ui->endTime->time();
    QString a=ui->LineEdit->text();
    QJsonObject j;
    j.insert("start-time",s.toString().mid(0,5));
    j.insert("end-time",e.toString().mid(0,5));
    j.insert("affair",a);
    return j;
}

void AffairEditor::SetData(QString start, QString end, QString affair)
{
    QTime *s=new QTime(start.split(":")[0].toInt(),start.split(":")[1].toInt());
    QTime *e=new QTime(end.split(":")[0].toInt(),end.split(":")[1].toInt());
    ui->startTime->setDisplayFormat("HH:mm");
    ui->endTime->setDisplayFormat("HH:mm");
    ui->startTime->setTime(*s);
    ui->endTime->setTime(*e);
    ui->LineEdit->setText(affair);
}

void AffairEditor::on_confirmButton_clicked()
{
    if(loop!=nullptr)
	{
		loop->exit();
	}
    result=0;
}

void AffairEditor::on_deleteButton_clicked()
{
    if(loop!=nullptr)
	{
		loop->exit();
	}
    result=-1;
}
