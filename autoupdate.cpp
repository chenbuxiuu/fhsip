#include "autoupdate.h"
#include "ui_autoupdate.h"

set<string> stringSplit(string s, char split);

AutoUpdate::AutoUpdate(QWidget *parent) :
    FHWidget(parent),
    ui(new Ui::AutoUpdate)
{
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("设备信息")
                                     <<QStringLiteral("槽位号"));
    ui->setupUi(this);
    init_LineEdit();
    shell="AutoUpdate";
    creat_TreeViewPopMenu();
//    connect(treeView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_treeView_customContextMenuRequested(const QPoint)));
}


AutoUpdate::~AutoUpdate()
{
    delete ui;
}


void AutoUpdate::init_LineEdit()
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

void AutoUpdate::creat_TreeViewPopMenu()
{
    addSlotAction=new QAction("添加槽位",this);
    connect(addSlotAction,SIGNAL(triggered()),this,SLOT(add_slot()));
    treeViewItemPopMenu->addAction(addSlotAction);
}

void AutoUpdate::add_slot()
{
    //清除上次slot内容
    slot="";
    //弹出对话框
    SlotInputDialog *dialog=new SlotInputDialog();
    connect(dialog,SIGNAL(sendSlot(string)),this,SLOT(getNewAddSlot(string)));
    dialog->exec();
    //更新device set和界面
    QModelIndex index=treeView->currentIndex();
    QStandardItem *dev=model->itemFromIndex(index);
    set<string> temp=stringSplit(slot,' ');
    //更新device set
    string ip=treeView->model()->data(index).toString().toStdString();
    Device d(ip);
    d.setSlot(temp);
    d=update_device(d);
    //更新界面:删除所有槽位根据更新后的device重绘
    dev->removeRows(0,dev->rowCount());
    temp=d.getSlot();
    set<string>::iterator it;
    it=temp.begin();
    for(it;it!=temp.end();it++)
    {
        QStandardItem* info=new QStandardItem(QStringLiteral(" "));
        info->setCheckable(true);
        dev->appendRow(info);
        dev->setChild(info->index().row(),1,new QStandardItem(QString::fromStdString(*it)));
        info->setCheckState(Qt::Checked);
    }
}

//接受添加槽位对话框的信号
void AutoUpdate::getNewAddSlot(string slot)
{
    this->slot=slot;
}

set<string> stringSplit(string s, char split)
{
    set<string> sList;
    stringstream ss(s);
    string token;
    while(1)
    {
        getline(ss, token, split);
        if ( ss.fail() )
            break;
        sList.insert(token);
    }
    return sList;
}

void AutoUpdate::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index=treeView->indexAt(pos);
    string device=treeView->model()->data(index).toString().toStdString();
    if(device!=""&&model->itemFromIndex(index)->parent()==NULL)
    {
        treeViewItemPopMenu->exec(QCursor::pos());
    }
}

/**
  *date:2017/12/26
  *author:xhchen
  *brief:生成进程执行的脚本
  *parm:shell 脚本名
*/
void AutoUpdate::creatShell(string shell)
{
    get_update_device_and_slot();

    ofstream out;
    shell+=".tcl";
    out.open(shell,ios::out);
//    out<<"source initial.tcl"<<endl;
    out<<"set a 100"<<endl;
    out<<"puts $a"<<endl;
    out.close();
}


/**
  *date:2017/12/26
  *author:xhchen
  *brief:获取需要升级的设备ip和对应的槽位
  *parm:
*/


set<Device> AutoUpdate::get_update_device_and_slot()
{
    set<Device> updateDevice;

    return updateDevice;
}



