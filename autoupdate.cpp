#include "autoupdate.h"
#include "ui_autoupdate.h"

AutoUpdate::AutoUpdate(QWidget *parent) :
    FHWidget(parent),
    ui(new Ui::AutoUpdate)
{
    ui->setupUi(this);
    initLineEdit();
}


AutoUpdate::~AutoUpdate()
{
    delete ui;
}

void AutoUpdate::initLineEdit()
{
    ui->label_MaIP->setText("Ma IP:");
    QRegExp ipRegExp("^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\."
                     "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."
                     "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."
                     "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$");
    QValidator *validatorMaIP=new QRegExpValidator(ipRegExp,ui->lineEdit_MaIP);
    ui->lineEdit_MaIP->setValidator(validatorMaIP);

    QHBoxLayout *part1=new QHBoxLayout;
    part1->addWidget(ui->label_MaIP);
    part1->addWidget(ui->lineEdit_MaIP);
    part1->addWidget(ui->pushButton_addMa);
    layout1->addLayout(part1);
}
