#include "./include/mainwindow.h"
#include "./ui_mainwindow.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QStandardItemModel>

const QString fileName = "./data.log";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setTimeStyle();
    setCurrentTime();
    testSet();

    QFile file(fileName);
    if(file.exists()){
        COMMON::readFile(fileName);
        showTreeView();
    }

    connect(ui->commitBtn,&QPushButton::clicked,[this,&f=fileName](){
        commitAndStorageTx();
        COMMON::saveFile(f);
        showTreeView();
    });
    connect(ui->clearBtn,&QPushButton::clicked,[this,&f=fileName,&p=parent](){
        if(isDelete(p)){
            COMMON::clearFile(f);
            QM.clear();
        }
        showTreeView();
    });

}
QDate MainWindow::getQTime(){
    return ui->TxDateEdit->date();
}
TX MainWindow::getTx(){
    TX tx;
    tx.payName = ui->payName->text();
    tx.payCnt = ui->payCnt->text().toDouble();
    tx.incomeName = ui->incomeName->text();
    tx.incomeCnt = ui->incomeCnt->text().toDouble();
    return tx;
}
void MainWindow::setCurrentTime(){
    QDate currentDate = QDate::currentDate();
    ui->TxDateEdit->setDate(currentDate);
}
void MainWindow::setTxContent(TX tx){
    ui->payName->setText(tx.payName);
    ui->payCnt->setText(QString::number(tx.payCnt,'.',2));
    ui->incomeName->setText(tx.incomeName);
    ui->incomeCnt->setText(QString::number(tx.incomeCnt,'.',2));
}
void MainWindow::setTimeStyle(QString df){
    ui->TxDateEdit->setDisplayFormat(df);
}
void MainWindow::testSet(){
    TX tx = TX{"haha",1,"hehe",2};
    setTxContent(tx);
    setCurrentTime();
}
void MainWindow::commitAndStorageTx(){
    TX tx = getTx();
    QDate t = getQTime();
    if (QM.contains(t)){
        QM[t].push_back(tx);
    }else{
        QM.insert(t,QList<TX>{tx});
    }
}
bool MainWindow::isDelete(QWidget *parent){
    QMessageBox::StandardButton result = QMessageBox::question(parent,"Title","text");
    return result==QMessageBox::Yes;
}

void MainWindow::showTreeView(){
    QStandardItemModel* model = new QStandardItemModel(ui->treeView);
    model->setHorizontalHeaderLabels(QStringList() << "收支情况详情");
    QStringList FTitle;
    QStringList STitle;
    QList<int> STitleLen;
    for(auto each : QM.toStdMap()){
        FTitle<<each.first.toString();
        for(TX tx : each.second){
            STitle<<tx.toQString();
        }
        STitleLen<<each.second.size();
    }
    int start=0;
    for (int i1 = 0; i1 < FTitle.length(); i1++){
        QString subprimaryTitle = FTitle[i1];
        QList<QStandardItem*> items1;
        QStandardItem* item1 = new QStandardItem(subprimaryTitle);
        items1.append(item1);
        model->appendRow(items1);
        for (int i2 = 0; i2 < STitleLen[i1]; i2++){
            QList<QStandardItem*> items2;
            QStandardItem* item3 = new QStandardItem(STitle[start + i2]);
            items2.append(item3);
            item1->appendRow(items2);
        }
        start += STitleLen[i1];
    }
    ui->treeView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

