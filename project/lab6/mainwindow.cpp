#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "startingpage.h"
#include "signuppage.h"

#include <QPushButton>

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
