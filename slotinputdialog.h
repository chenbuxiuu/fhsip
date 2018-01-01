#ifndef SLOTINPUTDIALOG_H
#define SLOTINPUTDIALOG_H

#include <QDialog>
#include <string>
#include <QMessageBox>

using namespace std;

namespace Ui {
class SlotInputDialog;
}

class SlotInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SlotInputDialog(QWidget *parent = 0);
    ~SlotInputDialog();

private slots:
    void on_pushButton_Cancle_clicked();

    void on_pushButton_OK_clicked();

    string on_lineEdit_slot_editingFinished();

    void on_lineEdit_slot_returnPressed();

private:
    Ui::SlotInputDialog *ui;
signals:
    void sendSlot(string slot);
};

#endif // SLOTINPUTDIALOG_H
