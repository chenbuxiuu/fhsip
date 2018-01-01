#ifndef AUTOUPDATE_H
#define AUTOUPDATE_H

#include <QWidget>
#include <sstream>
#include <QFocusEvent>
#include "fhwidget.h"
#include "slotinputdialog.h"

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
    set<Device> get_update_device_and_slot();
    void creatShell(string shell);

    string slot;//添加的槽位
private:
    Ui::AutoUpdate *ui;
    QGroupBox *groupBox1;
    QAction *addSlotAction;
private slots:
    void add_slot();
    void getNewAddSlot(string slot);
    void on_treeView_customContextMenuRequested(const QPoint &pos);
};

#endif // AUTOUPDATE_H
