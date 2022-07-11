#include "./include/mainwindow.h"
#include "./ui_mainwindow.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QStandardItemModel>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setTimeStyle();
    setCurrentTime();
//    testSet();

    QFile file(fileName);
    if(file.exists()){
        COMMON::readFile(fileName);
        showTreeView();
    }

    connect(ui->commitBtn,SIGNAL(clicked()),this,SLOT(on_commitBtn_clicked()),Qt::UniqueConnection);
    connect(ui->clearBtn,SIGNAL(clicked()),this,SLOT(on_clearBtn_clicked()),Qt::UniqueConnection);
    connect(ui->computeBtn,SIGNAL(clicked()),this,SLOT(on_computeBtn_clicked()));


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
    ui->startTimeEdit->setDate(currentDate.addDays(-7));
    ui->endTimeEdit->setDate(currentDate);
}
void MainWindow::setTxContent(TX tx){
    ui->payName->setText(tx.payName);
    ui->payCnt->setText(QString::number(tx.payCnt,'.',2));
    ui->incomeName->setText(tx.incomeName);
    ui->incomeCnt->setText(QString::number(tx.incomeCnt,'.',2));
}
void MainWindow::setTimeStyle(QString df){
    ui->TxDateEdit->setDisplayFormat(df);
    ui->startTimeEdit->setDisplayFormat(df);
    ui->endTimeEdit->setDisplayFormat(df);
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

void MainWindow::showTreeView(){
    QStandardItemModel* model = new QStandardItemModel(ui->treeView);
    model->setHorizontalHeaderLabels(QStringList() << "收支情况详情");
    QStringList FTitle;
    QStringList STitle;
    QStringList TTitle;
    QList<int> TTitleLen;
    STitle<<"支出"<<"收入";
    for(auto each : QM.toStdMap()){
        FTitle<<each.first.toString(dateFormat);
        QList<QPair<QString,double>> pay;
        QList<QPair<QString,double>> income;
        COMMON::separatePayAndIncome(each.second,pay,income);
        for(QPair<QString,double> p :pay){
            TTitle<<p.first+":"+QString::number(p.second,'.',2);
        }
        TTitleLen<<pay.size();
        for(QPair<QString,double> in :income){
            TTitle<<in.first+":"+QString::number(in.second,'.',2);
        }
        TTitleLen<<income.size();


    }
    int start=0;
    for (int i1 = 0; i1 < FTitle.length(); i1++){
        QString subprimaryTitle = FTitle[i1];
        QList<QStandardItem*> items1;
        QStandardItem* item1 = new QStandardItem(subprimaryTitle);
        items1.append(item1);
        model->appendRow(items1);
        for (int i2 = 0; i2 < 2; i2++){
            QList<QStandardItem*> items2;
            QStandardItem* item2 = new QStandardItem(STitle[i2]);
            items2.append(item2);
            item1->appendRow(items2);
            for (int i3 = 0; i3 < TTitleLen[i1*2+i2]; i3++){
                QList<QStandardItem*> items3;
                QStandardItem* item3 = new QStandardItem(TTitle[start]);
                items3.append(item3);
                item2->appendRow(items3);
                start++;
            }
        }
    }
    ui->treeView->setModel(model);
    ui->treeView->setItemsExpandable(true);
    ui->treeView->expandAll();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_commitBtn_clicked()
{
    commitAndStorageTx();
    COMMON::saveFile(fileName);
    showTreeView();
}


void MainWindow::on_clearBtn_clicked()
{
    qDebug()<<"hello";
    QMessageBox::StandardButton result = QMessageBox::information(this,  "警告",  "数据将永远消失", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(result==QMessageBox::Yes){
        COMMON::clearFile(fileName);
        if(QM.isEmpty()){
            return;
        }
        QM.clear();
        showTreeView();
    }
}


void MainWindow::on_computeBtn_clicked()
{
    QDate start = ui->startTimeEdit->date();
    QDate end = ui->endTimeEdit->date();
    int days;
    double pay=0,income=0;
    if (ui->toNowCheckBox->isChecked()){
        end = QDate::currentDate();
    }
    days = start.daysTo(end);
    COMMON::count(start,end,pay,income);
    ui->payText->setText(QString::number(pay,'.',2));
    ui->incomeText->setText(QString::number(income,'.',2));
    ui->dayText->setText(QString::number(days));

}

