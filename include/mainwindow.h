#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./common/common.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    const QString fileName = "./data.log";
private slots:

    void on_commitBtn_clicked();

    void on_clearBtn_clicked();

    void on_computeBtn_clicked();

protected:
    void commitAndStorageTx();
    void setCurrentTime();
    void setTxContent(TX tx);
    void setTimeStyle(QString df="yyyy-MM-dd");
    void testSet();
    QDate getQTime();
    TX getTx();
    void showTreeView();
};
#endif // MAINWINDOW_H
