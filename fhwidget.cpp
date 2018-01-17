#include "fhwidget.h"
#include "ui_fhwidget.h"

void treeItem_checkAllChild(QStandardItem *item, bool check);
void treeItem_checkAllChild_recursion(QStandardItem *item, bool check);
void treeItem_CheckChildChanged(QStandardItem *item);
Qt::CheckState checkSibling(QStandardItem *item);

FHWidget::FHWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FHWidget)
{
    ui->setupUi(this);

    type="MGMT";

    init_LineEdit();
    init_TreeView();

    ui->label_Dev->setText("Device:");
    ui->label_type->setText("设备IP类型");
    ui->comboBox_type->addItem("MGMT");
    ui->comboBox_type->addItem("SIG");

    groupBox1=ui->groupBox;
    layout1=ui->layout1;
    QHBoxLayout *part0=new QHBoxLayout;
    part0->addWidget(ui->label_type);
    part0->addWidget(ui->comboBox_type);
    QHBoxLayout *part1=new QHBoxLayout;
    part1->addWidget(ui->label_Dev);
    part1->addWidget(ui->lineEdit_IP);
    part1->addWidget(ui->pushButton_addDevice);
    layout1->addLayout(part0);
    layout1->addLayout(part1);
    //groupBox1->setLayout(layout1);

    groupBox2=ui->groupBox_2;
    layout2=ui->layout2;
    layout2->addWidget(ui->treeView);
    //groupBox2->setLayout(layout2);

    groupBox3=ui->groupBox_3;
    layout3=ui->layout3;
    layout3->addStretch();
    //layout3->addWidget(ui->horizontalSpacer_2);
    layout3->addWidget(ui->pushButton_begin);
    //layout3->addWidget(ui->horizontalSpacer);
    layout3->addStretch(3);
    layout3->addWidget(ui->pushButton_save);
    layout3->addStretch(2);
    // groupBox3->setLayout(layout3);

    groupBox4=ui->groupBox_4;
    layout4=ui->layout4;
    layout4->addWidget(ui->textBrowser_log);
    //groupBox4->setLayout(layout4);

    treeView=ui->treeView;
    treeView->setEditTriggers(0);
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);//右键菜单
    creat_TreeViewPopMenu();

    //    beginButton=ui->pushButton_begin;
    //    saveButton=ui->pushButton_save;

    proc=new QProcess;
    connect(proc,SIGNAL(readyRead()),this,SLOT(show_process_log()));
    connect(proc,SIGNAL(finished(int)),this,SLOT(finish_process(int)));
    shell="null";
}


FHWidget::~FHWidget()
{
    delete ui;
}

void FHWidget::init_LineEdit()
{
    QRegExp ipRegExp("^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\."
                     "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."
                     "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."
                     "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$");
    QValidator *validatorDev=new QRegExpValidator(ipRegExp,ui->lineEdit_IP);
    ui->lineEdit_IP->setValidator(validatorDev);
}

void FHWidget::init_TreeView()
{
    model=new QStandardItemModel(ui->treeView);
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("设备IP"));
    ui->treeView->setModel(model);
    connect(model,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(treeItemChanged(QStandardItem*)));
    QString widthStandard("255.255.255.255");
    ui->treeView->setColumnWidth(0,widthStandard.length()*12);
}


void FHWidget::treeItemChanged(QStandardItem *item)
{
    if ( item == nullptr )
        return ;
    if (item->isCheckable ())
    {
        //如果条目是存在复选框的，那么就进行下面的操作
        Qt::CheckState state = item -> checkState (); //获取当前的选择状态
        if ( item -> isTristate ())
        {
            //如果条目是三态的，说明可以对子目录进行全选和全不选的设置
            //都选中Qt::Checked，都不选中Qt::Unchecked,不完全选中Qt::PartiallyChecked
            if ( state != Qt :: PartiallyChecked )
            {
                //当前是选中状态，需要对其子项目进行全选
                treeItem_checkAllChild ( item , state == Qt::Checked ? true : false );
            }
        }
        else
        {
            //说明是两态的，两态会对父级的三态有影响

            //判断兄弟节点的情况
            treeItem_CheckChildChanged ( item );
        }
    }
}

void treeItem_checkAllChild(QStandardItem *item, bool check)
{
    if(item == nullptr)
        return ;
    int rowCount = item->rowCount();
    for(int i=0;i<rowCount;++i)
    {
        QStandardItem* childItems = item->child(i);
        treeItem_checkAllChild_recursion(childItems,check);
    }
    if(item->isCheckable())
    {
        item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
    }
}

void treeItem_checkAllChild_recursion(QStandardItem *item, bool check)
{
    if(item == nullptr)
        return;
    int rowCount = item->rowCount();
    for(int i=0;i<rowCount;++i)
    {
        QStandardItem* childItems = item->child(i);

        treeItem_checkAllChild_recursion(childItems,check);
    }
    if(item->isCheckable())
    {
        item->setCheckState(check ? Qt::Checked : Qt::Unchecked);

    }
}

void treeItem_CheckChildChanged(QStandardItem *item)
{
    if(nullptr == item)
        return;
    Qt::CheckState siblingState = checkSibling(item);
    QStandardItem * parentItem = item->parent();
    if(nullptr == parentItem)
        return;
    if(Qt::PartiallyChecked == siblingState)
    {
        if(parentItem->isCheckable() && parentItem->isTristate())
            parentItem->setCheckState(Qt::PartiallyChecked);
    }
    else if(Qt::Checked == siblingState)
    {
        if(parentItem->isCheckable())
            parentItem->setCheckState(Qt::Checked);
    }
    else
    {
        if(parentItem->isCheckable())
            parentItem->setCheckState(Qt::Unchecked);
    }
    treeItem_CheckChildChanged(parentItem);
}

Qt::CheckState checkSibling(QStandardItem *item)
{
    //先通过父节点获取兄弟节点
    QStandardItem * parent = item->parent();
    if(nullptr == parent)
        return item->checkState();
    int brotherCount = parent->rowCount();
    int checkedCount(0),unCheckedCount(0);
    Qt::CheckState state;
    for(int i=0;i<brotherCount;++i)
    {
        QStandardItem* siblingItem = parent->child(i);
        state = siblingItem->checkState();
        if(Qt::PartiallyChecked == state)
            return Qt::PartiallyChecked;
        else if(Qt::Unchecked == state)
            ++unCheckedCount;
        else
            ++checkedCount;
        if(checkedCount>0 && unCheckedCount>0)
            return Qt::PartiallyChecked;
    }
    if(unCheckedCount>0)
        return Qt::Unchecked;
    return Qt::Checked;
}

/**
  *date:2018/01/11
  *author:xhchen
  *brief:treeview遍历，返回复选框选中的item
  * 依次为treeview 逐级目录 目前一级为ip，默认
  *     有2级的为slot,默认参数为none
  * 如有其它需要，可修改此函数
  *parm:
*/
set<Device> FHWidget::treeView_traverse(string level2)
{
    set<Device> result;

    for(int i=0;i<model->rowCount();i++)
    {
        //遍历第一级
        QModelIndex index=model->index(i,0);
        string ip=model->data(index).toString().toStdString();
        //        cout<<i<<":"<<ip<<endl;
        Device d(ip);
        QStandardItem* item=model->itemFromIndex(index);

        //记录第一级，遍历第二级
        if(item->checkState()!=Qt::Unchecked)
        {
            //记录第一级
            //            d.setIp(ip);
            //遍历第二级,slot
            if(item->hasChildren()&&level2=="slot")
            {
                set<string> slot;
                if(item->rowCount()>0)
                {
                    for(int j=0;j<item->rowCount();j++)
                    {
                        if(item->child(j)->checkState()==Qt::Checked)
                        {
                            string t=item->child(j,1)->text().toStdString();
                            slot.insert(t);
                        }
                    }
                }
                if(slot.size()>0)
                {
                    d.setSlot(slot);
                }

            }
            result.insert(d);
        }
    }
    /*调试信息 begin*/
    cout<<"choosed ip:"<<endl;
    set<Device>::iterator it;
    it=result.begin();
    for(it;it!=result.end();it++)
    {
        Device d=*it;
        cout<<d.getIp()<<endl;
        d.getSlot(true);
    }
    /*调试信息 end*/
    return result;
}


string FHWidget::on_lineEdit_IP_editingFinished()
{
    string ip=ui->lineEdit_IP->text().toStdString();
    return ip;
}

void FHWidget::on_pushButton_addDevice_clicked()
{
    string ip=on_lineEdit_IP_editingFinished();
    if(ip!="")
    {
        Device d(ip);
        pair<set<Device>::iterator,bool> success=device.insert(d);
        if(success.second==true)
            update_TreeView(ip,"ADD");
        //        if(insertDevice(d))
        //            updateTreeView(ip,"ADD");
    }
    ui->lineEdit_IP->setText("");
}

void FHWidget::on_lineEdit_IP_returnPressed()
{
    on_pushButton_addDevice_clicked();
}
/**
  *date:2017/12/29
  *author:xhchen
  *brief:treeview更新操作
  *parm:ip
  *     operation 操作动作
*/
QStandardItem *FHWidget::update_TreeView(string ip, string operation)
{
    QStandardItem *result=NULL;
    if(operation=="ADD")
    {
        //更新界面
        QStandardItem* dev=new QStandardItem(QString::fromStdString(ip));
        model->appendRow(dev);
        dev->setCheckable(true);
        dev->setTristate(true);
        dev->setEditable(false);
        dev->setToolTip(QString::fromStdString(ip));
        result=dev;
        //更新device set
        Device temp(ip);
        device.insert(temp);
    }
    else if(operation=="DEL")
    {
        //更新界面,set里其实ip升级唯一的，由于函数返回值的规定为QList，所以有for循环
        QList<QStandardItem *> delItem=model->findItems(QString::fromStdString(ip));
        for(int i=0;i<delItem.size();i++)
        {
            int row=delItem.at(i)->row();
            model->takeRow(row);
        }
        result=delItem.at(0);
        //更新device set
        set<Device>::iterator it;
        it=device.begin();
        for(it;it!=device.end();it++)
        {
            Device temp=*it;
            if(temp.getIp()==ip)
            {
                it=device.erase(it);
                break;
            }

        }
        //更新选中device set
    }
    else
    {
        //do nothing
    }
    return result;
}

/**
  *date:2017/12/29
  *author:xhchen
  *brief:创建treeview右键菜单，父类只有删除,其他的按需在子类实现,
  *parm:
*/
void FHWidget::creat_TreeViewPopMenu()
{
    delDeviceAction=new QAction("删除",this);
    connect(delDeviceAction,SIGNAL(triggered()),this,SLOT(delete_device()));
    treeViewItemPopMenu=new QMenu(this);
    treeViewItemPopMenu->addAction(delDeviceAction);
}
/**
  *date:2017/12/29
  *author:xhchen
  *brief:treeview右键菜单-删除
  *parm:
*/
void FHWidget::delete_device()
{
    QModelIndex index=ui->treeView->currentIndex();
    string device=ui->treeView->model()->data(index).toString().toStdString();
    update_TreeView(device,"DEL");
}
/**
  *date:2017/12/29
  *author:xhchen
  *brief:treeview右键单击槽函数，父类不实现，父类实现会执行2次
  *parm:
*/
void FHWidget::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    //        QModelIndex index=ui->treeView->indexAt(pos);
    //        string device=ui->treeView->model()->data(index).toString().toStdString();
    //        if(device!=""&&model->itemFromIndex(index)->parent()==NULL)
    //        {
    //            treeViewItemPopMenu->exec(QCursor::pos());
    //        }
}
/**
  *date:2017/12/29
  *author:xhchen
  *brief:执行过程
  *parm:
*/
void FHWidget::show_process_log()
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");
    QTime t;
    t.start();
    while(t.elapsed()<500)
        QCoreApplication::processEvents();
    QString log=tc->toUnicode(proc->readAll());
    ui->textBrowser_log->append(log);
}
/**
  *date:2017/12/29
  *author:xhchen
  *brief:开始按钮槽函数，开启进程，解绑槽函数，重绑槽函数
  *parm:
*/
void FHWidget::on_pushButton_begin_clicked()
{

    //参数检查
    bool param_right=paramCheck();
    //开启进程
    if(proc->state()!=QProcess::Running&&shell!="null"&&param_right)
    {
        QString filename="shell/"+shell+".bat";
        QFile file1(filename);
        if(file1.exists())
        {
            creatShell(shell.toStdString());
            proc->start(filename);
            ui->textBrowser_log->setText("begin");
            ui->pushButton_begin->setText("中止");
            disconnect(ui->pushButton_begin,SIGNAL(clicked()),this,SLOT(on_pushButton_begin_clicked()));
            connect(ui->pushButton_begin,SIGNAL(clicked()),this,SLOT(stop_process()));
        }
        else
            ui->textBrowser_log->setText(filename+" is not exist");
    }
}
/**
  *date:2017/12/29
  *author:xhchen
  *brief:中止按钮槽函数，中止进程，解绑槽函数，重绑槽函数
  *parm:
*/
void FHWidget::stop_process()
{
    if(proc->state()==QProcess::Running)
        proc->kill();
    ui->textBrowser_log->append("中止");
    ui->pushButton_begin->setText("开始");
    disconnect(ui->pushButton_begin,SIGNAL(clicked()),this,SLOT(stop_process()));
    connect(ui->pushButton_begin,SIGNAL(clicked()),this,SLOT(on_pushButton_begin_clicked()));
}
/**
  *date:2017/12/29
  *author:xhchen
  *brief:进程执行完成
  *parm:
*/
void FHWidget::finish_process(int exitCode)
{
    ui->pushButton_begin->setText("开始");
    disconnect(ui->pushButton_begin,SIGNAL(clicked()),this,SLOT(stop_process()));;
    connect(ui->pushButton_begin,SIGNAL(clicked()),this,SLOT(on_pushButton_begin_clicked()));
    ui->textBrowser_log->append("exit with code "+QString::number(exitCode,10));
}
/**
  *date:2017/12/29
  *author:xhchen
  *brief:在set中根据ip查找device，返回迭代器位置
  *parm:
*/
set<Device>::iterator FHWidget::find_device_by_ip(string ip)
{
    set<Device>::iterator it;
    it=device.begin();
    for(it;it!=device.end();it++)
    {
        Device temp=*it;
        if(temp.getIp()==ip)
            return it;
    }
}
/**
  *date:2017/12/29
  *author:xhchen
  *brief:根据给定device更新已有device，此函数被迫写的，目前只支持更新slot
  *parm:Device
*/
Device FHWidget::update_device(Device d)
{
    set<Device>::iterator it;
    it=device.begin();
    for(it;it!=device.end();it++)
    {
        Device temp=*it;
        if(temp.getIp()==d.getIp())
        {
            device.erase(it);
            temp.setSlot(d.getSlot());
            device.insert(temp);
            return temp;
        }
    }
}

/**
  *date:2017/12/26
  *author:xhchen
  *brief:生成线程执行的脚本
  *parm:shell 脚本名
*/
void FHWidget::creatShell(string shell)
{
}


/**
  *date:2017/12/29
  *author:xhchen
  *brief:保存设备信息
  *parm:
*/
void FHWidget::on_pushButton_save_clicked()
{
    //子类实现
}

/**
  *date:2017/12/29
  *author:xhchen
  *brief:启动后显示保存信息
  *parm:
*/
void FHWidget::showHistory()
{
    //子类实现
}
/**
  *date:2018/01/02
  *author:xhchen
  *brief:选取设备Ip类型的槽函数
  *parm:
*/
void FHWidget::on_comboBox_type_currentTextChanged(const QString &arg1)
{
    //    子类实现
}
/**
  *date:2018/01/12
  *author:xhchen
  *brief:参数检查,子类具体实现
  *parm:
*/
bool FHWidget::paramCheck()
{
    //    return true;
}
