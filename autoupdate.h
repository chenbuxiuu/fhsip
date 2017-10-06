#ifndef AUTOUPDATE_H
#define AUTOUPDATE_H

#include <QWidget>
#include "fhwidget.h"

namespace Ui {
class AutoUpdate;
}

class AutoUpdate : public FHWidget
{
    Q_OBJECT

public:
    explicit AutoUpdate(QWidget *parent = 0);
    ~AutoUpdate();
    void initLineEdit();
private:
    Ui::AutoUpdate *ui;
    QGroupBox *groupBox1;
};

#endif // AUTOUPDATE_H
