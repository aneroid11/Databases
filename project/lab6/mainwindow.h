#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_signInButton_clicked();

    void on_signUpPageButton_clicked();

    void on_signUpButton_clicked();

    void on_submitButton_clicked();

    void on_stackedWidget_currentChanged(int index);

private:
    void artistPageInit();

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
