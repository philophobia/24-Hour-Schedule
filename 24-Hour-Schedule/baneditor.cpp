#include "baneditor.h"
#include "ui_baneditor.h"

BanEditor::BanEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BanEditor)
{
    ui->setupUi(this);
    setWindowModality(Qt:: WindowModal);
    setWindowFlags(Qt::Window|Qt::WindowCloseButtonHint);
    setWindowTitle("编辑禁忌");
}

BanEditor::~BanEditor()
{
    delete ui;
}

void BanEditor::exec()
{
    result=1;
    this->show();
	loop = new QEventLoop(this);
	loop->exec();
}

void BanEditor::closeEvent(QCloseEvent *e)
{
	if(loop!=nullptr)
	{
		loop->exit();
		result=1;
	}
	e->accept();
}

QString BanEditor::GetData()
{
    return ui->lineEdit->text();
}

void BanEditor::on_confirm_clicked()
{
    if(loop!=nullptr)
	{
		loop->exit();
	}
    result=0;
}
