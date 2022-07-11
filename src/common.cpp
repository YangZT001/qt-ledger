#include"../include/common/common.h"
#include<QFileDialog>

QMap<QDate,QList<TX>> QM={};



void COMMON::saveFile(QString fileName){
    QMapIterator<QDate,QList<TX>> iter(QM);
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    while(iter.hasNext()){
        iter.next();
        out<<iter.key().toString(dateFormat)<<",";
        for(TX tx : iter.value()){
            out<<tx.payName<<"_";
            out<<tx.payCnt<<"_";
            out<<tx.incomeName<<"_";
            out<<tx.incomeCnt<<";";
        }
        out<<'\n';
    }
    file.close();
}


TX stringToTx(QString s){
    TX tx;
    QStringList qlist = s.split("_");
    tx.payName = qlist[0];
    tx.payCnt = qlist[1].toDouble();
    tx.incomeName = qlist[2];
    tx.incomeCnt = qlist[3].toDouble();
    return tx;
}
QList<TX> stringToListTx(QString s){

    QStringList qlist = s.split(";");
    QList<TX> txs={};

    for(int i=0;i<qlist.size()-1;i++){
        txs.push_back(stringToTx(qlist[i]));
    }
    return txs;
}
void COMMON::readFile(QString fileName){
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);


    QTextStream in(&file);
    QString line = in.readLine();
    if(line.isEmpty()){
        return;
    }
    while(!line.isNull()){

        QStringList qlist = line.split(",");
        QDate date = QDate::fromString(qlist[0],dateFormat);
        QList txs = stringToListTx(qlist[1]);
        QM.insert(date,txs);
        line = in.readLine();

    }

    file.close();

}
void COMMON::clearFile(QString fileName){
    QFile file(fileName);
    if (file.exists()){
        file.remove();
    }else{
        return;
    }
}
void COMMON::separatePayAndIncome(QList<TX> qlist,
                          QList<QPair<QString,double>>&pay,
                          QList<QPair<QString,double>>&income){
    for (TX tx : qlist){
        if (!tx.payName.isEmpty()){
            pay.push_back({tx.payName,tx.payCnt});
        }
        if (!tx.incomeName.isEmpty()){
            income.push_back({tx.incomeName,tx.incomeCnt});
        }
    }
}
void COMMON::count(QDate start,QDate end,double &pay,double &income){
    for (auto iter : QM.toStdMap()){
        if(iter.first<start){
            continue;
        }
        if (iter.first>end){
            break;
        }
        QList<QPair<QString,double>>Ps;
        QList<QPair<QString,double>>Is;
        separatePayAndIncome(iter.second,Ps,Is);
        for(QPair<QString,double> P:Ps){
            pay+=P.second;
        }
        for(QPair<QString,double> I:Is){
            income+=I.second;
        }
    }
}
