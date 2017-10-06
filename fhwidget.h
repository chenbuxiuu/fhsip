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
#include "device.h"

namespace Ui {
class FHWidget;
}

class FHWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FHWidget(QWidget *parent = 0);
    ~FHWidget();
    void updateTreeView(string ip,string operation);
    void initLineEdit();
    void initTreeView();

private:

protected:
    Ui::FHWidget *ui;
    set<Device> device;
    QStandardItemModel *model;
    QGroupBox *groupBox1;
    QGroupBox *groupBox2;
    QGroupBox *groupBox3;
    QGroupBox *groupBox4;

    QVBoxLayout *layout1;
    QGridLayout *layout2;
    QGridLayout *layout3;
    QGridLayout *layout4;

    QTreeView *treeView;
private slots:
    void treeItemChanged ( QStandardItem * item );
    string on_lineEdit_IP_editingFinished();
    void on_pushButton_addDevice_clicked();
    void on_lineEdit_IP_returnPressed();
};

#endif // FHWIDGET_H
