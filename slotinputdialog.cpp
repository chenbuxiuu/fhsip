#include "slotinputdialog.h"
#include "ui_slotinputdialog.h"

SlotInputDialog::SlotInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SlotInputDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("输入槽位");
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    ui->lineEdit_slot->setPlaceholderText("输入多个用空格隔开");
    QRegExp regx("([0-9]+[ ]?)+");//限制输入 数字+空格
    QValidator *validatorSlot=new QRegExpValidator(regx,ui->lineEdit_slot);
    ui->lineEdit_slot->setValidator(validatorSlot);
}

SlotInputDialog::~SlotInputDialog()
{
    delete ui;
}

void SlotInputDialog::on_pushButton_Cancle_clicked()
{
    emit sendSlot("");
    this->close();
}

void SlotInputDialog::on_pushButton_OK_clicked()
{
    string slot="";
    slot=on_lineEdit_slot_editingFinished();
    if(slot!="")
    {
        emit sendSlot(slot);
        this->close();
    }
    else
    {
         QMessageBox::information(this,tr("提示"),tr("请输入槽位"),QMessageBox::Ok);
    }

}

string SlotInputDialog::on_lineEdit_slot_editingFinished()
{
    string slot=ui->lineEdit_slot->text().toStdString();
    return slot;
}

void SlotInputDialog::on_lineEdit_slot_returnPressed()
{
    on_pushButton_OK_clicked();
}
