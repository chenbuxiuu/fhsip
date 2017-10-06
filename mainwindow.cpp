#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //FHWidget *w=new FHWidget(ui->tab);
    AutoUpdate *w=new AutoUpdate(ui->tab);
    ui->tab1Layout->addWidget(w);

    FHWidget *a=new FHWidget(ui->tab_2);
    ui->tab2Layout->addWidget(a);



}

MainWindow::~MainWindow()
{
    delete ui;
}
