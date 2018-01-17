#ifndef FHWIDGET_H
#define FHWIDGET_H


#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <set>
#include <string>
#include <QGroupBox>
#include <QLayout>
#include <QProcess>
#include <QTime>
#include <QTextCodec>
#include <QPushButton>
#include <QString>
#include <QFile>
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <fstream>
#include "device.h"
#include "tool.h"



namespace Ui {
class FHWidget;
}

class FHWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FHWidget(QWidget *parent = 0);
    ~FHWidget();
    QStandardItem* update_TreeView(string ip,string operation);
    void init_LineEdit();
    void init_TreeView();
    void creat_TreeViewPopMenu();
    set<Device>::iterator find_device_by_ip(string ip);
    Device update_device(Device d);

    virtual void creatShell(string shell);
    virtual void showHistory();

    set<Device> treeView_traverse(string level2="none");

    bool paramCheck();//检查参数，无误开启进程，有错提示错误信息

private:

protected:
    Ui::FHWidget *ui;
    set<Device> device;
    string type;
    QStandardItemModel *model;
    QGroupBox *groupBox1;
    QGroupBox *groupBox2;
    QGroupBox *groupBox3;
    QGroupBox *groupBox4;

    QVBoxLayout *layout1;
    QHBoxLayout *layout2;
    QHBoxLayout *layout3;
    QHBoxLayout *layout4;

//    QPushButton *beginButton;
//    QPushButton *saveButton;

    QTreeView *treeView;
    QMenu *treeViewItemPopMenu;
    QAction *delDeviceAction;

    QProcess *proc;

    QString shell;



private slots:
    void treeItemChanged ( QStandardItem * item );//树操作
    string on_lineEdit_IP_editingFinished();//device IP
    void on_pushButton_addDevice_clicked();//device IP加入树
    void on_lineEdit_IP_returnPressed();//device IP加入树
    void on_pushButton_begin_clicked();//开始按钮

    void on_treeView_customContextMenuRequested(const QPoint &pos);

    void on_pushButton_save_clicked();



protected slots:
    void show_process_log();//过程打印
    void stop_process();//中止进程
    void finish_process(int exitCode);//执行完成

    void delete_device();

    void on_comboBox_type_currentTextChanged(const QString &arg1);
};

#endif // FHWIDGET_H
