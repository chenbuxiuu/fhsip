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

    initLineEdit();
    initTreeView();

    ui->label_Dev->setText("Device:");

    groupBox1=ui->groupBox;
    layout1=ui->layout1;
    QHBoxLayout *part1=new QHBoxLayout;
    part1->addWidget(ui->label_Dev);

    part1->addWidget(ui->lineEdit_IP);
    part1->addWidget(ui->pushButton_addDevice);
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
}


FHWidget::~FHWidget()
{
    delete ui;
}

void FHWidget::initLineEdit()
{
    QRegExp ipRegExp("^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\."
                     "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."
                     "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."
                     "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$");
    QValidator *validatorDev=new QRegExpValidator(ipRegExp,ui->lineEdit_IP);
    ui->lineEdit_IP->setValidator(validatorDev);
}

void FHWidget::initTreeView()
{
    model=new QStandardItemModel(ui->treeView);
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("device info"));
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
            updateTreeView(ip,"ADD");
//        if(insertDevice(d))
//            updateTreeView(ip,"ADD");
    }
    ui->lineEdit_IP->setText("");
}

void FHWidget::on_lineEdit_IP_returnPressed()
{
    on_pushButton_addDevice_clicked();
}

void FHWidget::updateTreeView(string ip, string operation)
{
    if(operation=="ADD")
    {
        QStandardItem* dev=new QStandardItem(QString::fromStdString(ip));
        model->appendRow(dev);
        dev->setCheckable(true);
        dev->setTristate(true);
        dev->setEditable(false);
        dev->setToolTip(QString::fromStdString(ip));
    }
    else if(operation=="DEL")
    {}
    else
    {}
}
