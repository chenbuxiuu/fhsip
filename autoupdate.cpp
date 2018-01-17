#include "autoupdate.h"
#include "ui_autoupdate.h"

//set<string> stringSplit(string s, char split);

AutoUpdate::AutoUpdate(QWidget *parent) :
    FHWidget(parent),
    ui(new Ui::AutoUpdate)
{
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("设备信息")
                                     <<QStringLiteral("槽位号"));
    ui->setupUi(this);
    init_LineEdit();

    ui->label_MaIP->setText("Ma IP:");
    QHBoxLayout *part1=new QHBoxLayout;
    part1->addWidget(ui->label_MaIP);
    part1->addWidget(ui->lineEdit_MaIP);
    layout1->addLayout(part1);

    ui->label_FTP->setText("FTP IP:");
    QHBoxLayout *part2=new QHBoxLayout;
    part2->addWidget(ui->label_FTP);
    part2->addWidget(ui->lineEdit_FTP);
    layout1->addLayout(part2);

    QHBoxLayout *part3=new QHBoxLayout;
    part3->addWidget(ui->pushButton_fileChoose);
    layout1->addLayout(part3);

    shell="AutoUpdate";
    creat_TreeViewPopMenu();
    showHistory();
    //    connect(treeView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_treeView_customContextMenuRequested(const QPoint)));
}


AutoUpdate::~AutoUpdate()
{
    delete ui;
}


void AutoUpdate::init_LineEdit()
{

    QRegExp ipRegExp("^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\."
                     "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."
                     "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."
                     "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$");
    QValidator *validatorMaIP=new QRegExpValidator(ipRegExp,ui->lineEdit_MaIP);
    ui->lineEdit_MaIP->setValidator(validatorMaIP);

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
    set<string> temp=Tool::stringSplit(slot,' ');
    //更新device set
    string ip=treeView->model()->data(index).toString().toStdString();
    Device d;
    //查找
    set<Device>::iterator dit;
    dit=device.begin();
    for(dit;dit!=device.end();dit++)
    {
        d=*dit;
        if(d.getIp()==ip)
        {
            d.addSlot(temp);
            break;
        }
    }
    d=update_device(d);
    //更新界面:删除所有槽位根据更新后的device重绘
    dev->removeRows(0,dev->rowCount());
    temp=d.getSlot();
    draw_slot(dev,temp);
    //    set<string>::iterator it;
    //    it=temp.begin();
    //    for(it;it!=temp.end();it++)
    //    {
    //        QStandardItem* info=new QStandardItem(QStringLiteral(" "));
    //        info->setCheckable(true);
    //        dev->appendRow(info);
    //        dev->setChild(info->index().row(),1,new QStandardItem(QString::fromStdString(*it)));
    //        info->setCheckState(Qt::Checked);
    //    }
}

//接受添加槽位对话框的信号
void AutoUpdate::getNewAddSlot(string slot)
{
    this->slot=slot;
}

//set<string> stringSplit(string s, char split)
//{
//    set<string> sList;
//    stringstream ss(s);
//    string token;
//    while(1)
//    {
//        getline(ss, token, split);
//        if ( ss.fail() )
//            break;
//        sList.insert(token);
//    }
//    return sList;
//}


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
    shell="script/"+shell;
    out.open(shell,ios::out);
    //    out<<"source initial.tcl"<<endl;
//    out<<"set a 100"<<endl;
//    out<<"puts $a"<<endl;
    //遍历treeview，记录选中ip和槽位信息
    set<Device> choosed_device=treeView_traverse("slot");
    set<Device>::iterator it_device;
    it_device=choosed_device.begin();
    set<string>::iterator it_slot;
    for(it_device;it_device!=choosed_device.end();it_device++)
    {
        Device d=*it_device;
        set<string> temp=d.getSlot();
        //有槽位才写
        if(temp.size()>0)
        {
            out<<"puts "<<d.getIp()<<endl;

            it_slot=temp.begin();
            out<<"puts "<<"\"";
            for(it_slot;it_slot!=temp.end();it_slot++)
            {
                out<<(*it_slot)<<"  ";
            }
            out<<"\"";
            out<<endl;
        }

    }
    //
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

/**
  *date:2017/12/29
  *author:xhchen
  *brief:保存设备信息
  *parm:
*/
void AutoUpdate::on_pushButton_save_clicked()
{
    bool f=Tool::saveXML(device,"AutoUpdate.xml",2);
    if(f)
    {
        QMessageBox::information(this,tr("提示"),tr("保存成功"),QMessageBox::Ok);
    }
    else
    {
        QMessageBox::information(this,tr("提示"),tr("保存失败"),QMessageBox::Ok);
    }
}
/**
  *date:2017/12/29
  *author:xhchen
  *brief:启动后显示保存信息
  *parm:
*/
void AutoUpdate::showHistory()
{
    device=Tool::readXML("AutoUpdate.xml",2);
    set<Device>::iterator it;
    it=device.begin();
    for(it;it!=device.end();it++)
    {
        Device temp=*it;
        QStandardItem *dev=update_TreeView(temp.getIp(),"ADD");
        draw_slot(dev,temp.getSlot());
    }
}
/**
  *date:2018/01/02
  *author:xhchen
  *brief:选取设备Ip类型的槽函数
  *parm:
*/
void AutoUpdate::on_comboBox_type_currentTextChanged(const QString &arg1)
{
    type=arg1.toStdString();
    if(type=="SIG")
    {
        ui->lineEdit_FTP->clear();
        ui->lineEdit_MaIP->clear();
        ui->lineEdit_FTP->setEnabled(false);
        ui->lineEdit_MaIP->setEnabled(false);
    }
    else
    {
        ui->lineEdit_FTP->setEnabled(true);
        ui->lineEdit_MaIP->setEnabled(true);
    }
}


/**
  *date:2018/01/09
  *author:xhchen
  *brief:选择升级文件
  *parm:
*/
void AutoUpdate::on_pushButton_fileChoose_clicked()
{
    //清除上次内容
    filepath_xst1="";
    filepath_s1t1="";
    filepath_ccut1="";
    filepath_ucxt1="";
    //弹出对话框
    FileChooseDialog *dialog=new FileChooseDialog();
    connect(dialog,SIGNAL(sendXST1(string)),this,SLOT(get_xst1_filepath(string)));
    connect(dialog,SIGNAL(sendS1T1(string)),this,SLOT(get_s1t1_filepath(string)));
    connect(dialog,SIGNAL(sendUCXT1(string)),this,SLOT(get_ucxt1_filepath(string)));
    connect(dialog,SIGNAL(sendCCUT1(string)),this,SLOT(get_ccut1_filepath(string)));
    dialog->exec();
}
/**
  *date:2018/01/09
  *author:xhchen
  *brief:选择升级文件xst1槽函数
  *parm:
*/
void AutoUpdate::get_xst1_filepath(string filepath)
{
    this->filepath_xst1=filepath;
    cout<<"xst1:"<<filepath<<endl;
}
/**
  *date:2018/01/09
  *author:xhchen
  *brief:选择升级文件s1t1槽函数
  *parm:
*/
void AutoUpdate::get_s1t1_filepath(string filepath)
{
    this->filepath_s1t1=filepath;
    cout<<"s1t1:"<<filepath<<endl;
}
/**
  *date:2018/01/09
  *author:xhchen
  *brief:选择升级文件ccut1槽函数
  *parm:
*/
void AutoUpdate::get_ccut1_filepath(string filepath)
{
    this->filepath_ccut1=filepath;
    cout<<"ccut1:"<<filepath<<endl;
}
/**
  *date:2018/01/09
  *author:xhchen
  *brief:选择升级文件ucxt1槽函数
  *parm:
*/
void AutoUpdate::get_ucxt1_filepath(string filepath)
{
    this->filepath_ucxt1=filepath;
    cout<<"ucxt1:"<<filepath<<endl;
}
/**
  *date:2018/01/12
  *author:xhchen
  *brief:在treeview上绘制slot
  *parm:QStandardItem 要绘制slot的item
  *     set<string> slot
*/
void AutoUpdate::draw_slot(QStandardItem *dev, set<string> slot)
{
    set<string>::iterator it;
    it=slot.begin();
    for(it;it!=slot.end();it++)
    {
        QStandardItem* info=new QStandardItem(QStringLiteral(" "));
        info->setCheckable(true);
        dev->appendRow(info);
        dev->setChild(info->index().row(),1,new QStandardItem(QString::fromStdString(*it)));
        info->setCheckState(Qt::Checked);
    }
}
/**
  *date:2018/01/12
  *author:xhchen
  *brief:参数检查,子类具体实现
  * 暂未实现，直接返回true
  *parm:
*/
bool AutoUpdate::paramCheck()
{
    return true;
}

/**
  *date:2018/01/12
  *author:xhchen
  *brief:treeview右键
  *parm:
*/
void AutoUpdate::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index=treeView->indexAt(pos);
    string device=treeView->model()->data(index).toString().toStdString();
    if(device!=""&&model->itemFromIndex(index)->parent()==NULL)
    {
        treeViewItemPopMenu->exec(QCursor::pos());
    }
}
