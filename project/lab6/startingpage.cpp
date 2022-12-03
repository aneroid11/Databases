#include "startingpage.h"
#include "ui_startingpage.h"

StartingPage::StartingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartingPage)
{
    ui->setupUi(this);
}

StartingPage::~StartingPage()
{
    delete ui;
}
