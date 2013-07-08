//parameters used globally
const int maxMatrixRow=10000;
const int maxMatrixColumn=200;
short classType[maxMatrixColumn],matrix[maxMatrixRow][maxMatrixColumn];
int row, column;
double p0,p1;

#include <iostream>
#include <fstream>
#include <string>
#include "loadmatrix.h"
#include <QApplication>
using namespace std;
#include "mainwindow.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
