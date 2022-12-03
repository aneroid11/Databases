#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>

const int STARTING_PAGE_ID = 0;
const int SIGNIN_ID = 1;
const int SIGNUP_ID = 2;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_signInButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(SIGNIN_ID);
}
