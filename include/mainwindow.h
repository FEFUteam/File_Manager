#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <findwidget.h>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_SearchButton_clicked();
    void on_ListGridButton_toggled(bool checked);


private:
    FindWidget* findwid;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
