#include "../include/common/log.h"

QDebug operator<<(QDebug debug,TX & tx){

    debug<<"===============\n";
    debug<<tx.payName<<":"<<tx.payCnt<<"\n";
    debug<<tx.incomeName<<":"<<tx.incomeCnt<<"\n";
    debug<<"===============\n";
    return debug;

}
QDebug operator<<(QDebug debug,QFileInfo & file){
    debug<<"FILE PATH:"<<file.absoluteFilePath()<<"\n";
    return debug;
}
