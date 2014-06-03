#include <QProcess>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString max_timestamp = ui->max_timestamp->text();
    QString num_filters = ui->num_filters->text();
    QString pool_volume = ui->pool_volume->text();
    QString people_limit = ui->people_limit->text();
    QString ticket_price = ui->ticket_price->text();
    QString repairman_salary = ui->repairman_salary->text();
    QString filter_cost = ui->filter_cost->text();
    QString open_at = ui->open_at->text();
    QString close_at = ui->close_at->text();

    QProcess * p = new QProcess(this);
    p->start("./basen", QStringList() << max_timestamp << num_filters << pool_volume << people_limit << ticket_price << repairman_salary << filter_cost
        << open_at << close_at);
}
