#include<QString>
#include<QTime>

namespace COMMON{

    void saveFile(QString fileName);
    void readFile(QString fileName);
    void clearFile(QString fileName);

}

struct TX{
    QString payName;
    double payCnt;
    QString incomeName;
    double incomeCnt;
    QString toQString(){
        QString s = "";
        if(!payName.isEmpty()){
            s+=payName+":"+QString::number(-payCnt,'.',2)+'\n';
        }
        if(!incomeName.isEmpty()){
            s+=incomeName+":"+QString::number(incomeCnt,'.',2);
        }
        return s;
    }
};



extern QMap<QDate,QList<TX>> QM;

const QString dateFormat = "yyyy-MM-dd";
