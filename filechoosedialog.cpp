#include "filechoosedialog.h"
#include "ui_filechoosedialog.h"

FileChooseDialog::FileChooseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileChooseDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("选择文件");
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    CCUT1="";
    UCXT1="";
    S1T1="";
    XST1="";
}

FileChooseDialog::~FileChooseDialog()
{
    delete ui;
}

/**
  *date:2018/01/09
  *author:xhchen
  *brief:文件选择
  *parm:单盘类型
*/
void FileChooseDialog::FileChoose(string slotType)
{
    QList<QString> fileNameList;
    QFileDialog* fd = new QFileDialog(this);//创建对话框
    fd->setNameFilter("tar(*.tar);;Allfile(*.*)"); //设置文件过滤器
    fd->setViewMode(QFileDialog::List);  //设置浏览模式，有 列表（list） 模式和 详细信息（detail）两种方式
    fd->setModal(QFileDialog::ExistingFile);
    if ( fd->exec() == QDialog::Accepted )   //如果成功的执行
    {
        fileNameList = fd->selectedFiles();      //返回文件
        string filename=fileNameList.at(0).toStdString();//获取文件名，包括完整路径
        //获取文件名
        stringstream ss(filename);
        string token;
        while(1)
        {
            getline(ss, token, '/');
            if ( ss.fail() )
                break;
        }

        if(slotType=="XST1")
        {
            ui->lineEdit_XST1->setText(QString::fromStdString(token));
            ui->lineEdit_XST1->setToolTip(QString::fromStdString(filename));
            XST1=filename;
        }
        else if(slotType=="S1T1")
        {
            ui->lineEdit_S1T1->setText(QString::fromStdString(token));
            ui->lineEdit_S1T1->setToolTip(QString::fromStdString(filename));
            S1T1=filename;
        }
        else if(slotType=="UCXT1")
        {
            ui->lineEdit_UCXT1->setText(QString::fromStdString(token));
            ui->lineEdit_UCXT1->setToolTip(QString::fromStdString(filename));
            UCXT1=filename;
        }
        else if(slotType=="CCUT1")
        {
            ui->lineEdit_CCUT1->setText(QString::fromStdString(token));
            ui->lineEdit_CCUT1->setToolTip(QString::fromStdString(filename));
            CCUT1=filename;
        }
        else ;
    }
    else
        fd->close();
}
/**
  *date:2018/01/09
  *author:xhchen
  *brief:升级XST1的文件
  *parm:
*/
void FileChooseDialog::on_pushButton_XST1_clicked()
{
    FileChoose("XST1");
}
/**
  *date:2018/01/09
  *author:xhchen
  *brief:升级S1T1的文件
  *parm:
*/
void FileChooseDialog::on_pushButton_S1T1_clicked()
{
     FileChoose("S1T1");
}
/**
  *date:2018/01/09
  *author:xhchen
  *brief:升级UCXT1的文件
  *parm:
*/
void FileChooseDialog::on_pushButton_UCXT1_clicked()
{
    FileChoose("UCXT1");
}
/**
  *date:2018/01/09
  *author:xhchen
  *brief:升级UCXT1的文件
  *parm:
*/
void FileChooseDialog::on_pushButton_CCUT1_clicked()
{
    FileChoose("CCUT1");
}
/**
  *date:2018/01/09
  *author:xhchen
  *brief:文件选择完成，发送信号
  *parm:
*/
void FileChooseDialog::on_pushButton_clicked()
{
    emit sendXST1(XST1);
    emit sendS1T1(S1T1);
    emit sendUCXT1(UCXT1);
    emit sendCCUT1(CCUT1);
    this->close();
}

void FileChooseDialog::on_pushButton_2_clicked()
{
    emit sendXST1("");
    emit sendS1T1("");
    emit sendUCXT1("");
    emit sendCCUT1("");
    this->close();
}
