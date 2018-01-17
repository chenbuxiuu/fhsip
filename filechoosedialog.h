#ifndef FILECHOOSEDIALOG_H
#define FILECHOOSEDIALOG_H

#include <QDialog>
#include <QList>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <string>
#include <sstream>

using namespace std;

namespace Ui {
class FileChooseDialog;
}

class FileChooseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileChooseDialog(QWidget *parent = 0);
    ~FileChooseDialog();

    void FileChoose(string slotType);

private slots:
    void on_pushButton_XST1_clicked();

    void on_pushButton_S1T1_clicked();

    void on_pushButton_UCXT1_clicked();

    void on_pushButton_CCUT1_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::FileChooseDialog *ui;
    string XST1;
    string S1T1;
    string UCXT1;
    string CCUT1;
signals:
    void sendXST1(string filepath);
    void sendS1T1(string filepath);
    void sendUCXT1(string filepath);
    void sendCCUT1(string filepath);
};
#endif // FILECHOOSEDIALOG_H
