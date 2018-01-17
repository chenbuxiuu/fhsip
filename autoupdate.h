#ifndef AUTOUPDATE_H
#define AUTOUPDATE_H

#include <QWidget>
#include <sstream>
#include <QFocusEvent>
#include "fhwidget.h"
#include "slotinputdialog.h"
#include "filechoosedialog.h"

namespace Ui {
class AutoUpdate;
}

class AutoUpdate : public FHWidget
{
    Q_OBJECT

public:
    explicit AutoUpdate(QWidget *parent = 0);
    ~AutoUpdate();
    void init_LineEdit();
    void creat_TreeViewPopMenu();

    /**
      *date:2017/12/26
      *author:xhchen
      *brief:获取需要升级的设备ip和对应的槽位
      *parm:
    */
    set<Device> get_update_device_and_slot();

    /**
      *date:2017/12/26
      *author:xhchen
      *brief:生成进程执行的脚本
      *parm:shell 脚本名
    */
    void creatShell(string shell);

    /**
      *date:2017/12/29
      *author:xhchen
      *brief:启动后显示保存信息
      *parm:
    */
    void showHistory();

    string slot;//添加的槽位

    //升级文件路径
    string filepath_xst1;
    string filepath_s1t1;
    string filepath_ccut1;
    string filepath_ucxt1;
    //--------------------

    /**
      *date:2018/01/12
      *author:xhchen
      *brief:在treeview上绘制slot
      *parm:QStandardItem 要绘制slot的item
      *     set<string> slot
    */
    void draw_slot(QStandardItem *dev, set<string> slot);

    /**
      *date:2018/01/12
      *author:xhchen
      *brief:参数检查,子类具体实现
      *parm:
    */
    bool paramCheck();

private:
    Ui::AutoUpdate *ui;
    QGroupBox *groupBox1;
    QAction *addSlotAction;
private slots:
    void add_slot();
    void getNewAddSlot(string slot);


    /**
      *date:2017/12/29
      *author:xhchen
      *brief:保存设备信息
      *parm:
    */
    void on_pushButton_save_clicked();
    /**
      *date:2018/01/02
      *author:xhchen
      *brief:选取设备Ip类型的槽函数
      *parm:
    */
    void on_comboBox_type_currentTextChanged(const QString &arg1);

    /**
      *date:2018/01/09
      *author:xhchen
      *brief:选择升级文件
      *parm:
    */
    void on_pushButton_fileChoose_clicked();

    /**
      *date:2018/01/09
      *author:xhchen
      *brief:选择升级文件xst1槽函数
      *parm:
    */
    void get_xst1_filepath(string filepath);

    /**
      *date:2018/01/09
      *author:xhchen
      *brief:选择升级文件s1t1槽函数
      *parm:
    */
    void get_s1t1_filepath(string filepath);

    /**
      *date:2018/01/09
      *author:xhchen
      *brief:选择升级文件ccut1槽函数
      *parm:
    */
    void get_ccut1_filepath(string filepath);

    /**
      *date:2018/01/09
      *author:xhchen
      *brief:选择升级文件ucxt1槽函数
      *parm:
    */
    void get_ucxt1_filepath(string filepath);

    //treeview右键
    void on_treeView_customContextMenuRequested(const QPoint &pos);
};

#endif // AUTOUPDATE_H
