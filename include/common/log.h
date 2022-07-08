#ifndef LOG_H
#define LOG_H

#include "common.h"

#include<QDebug>
#include<QFileDialog>

QDebug operator<<(QDebug debug,TX & tx);
QDebug operator<<(QDebug debug,QFileInfo & file);

#endif // LOG_H
