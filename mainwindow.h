#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QIcon>

#include <map>
#include "matrix.h"
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionLoad_triggered();

    void on_executeButton_clicked();

    void on_actionAboutQt_triggered();

    void on_trialTimesSlider_valueChanged(int value);

private:
    bool loadFile(QString);
    Ui::MainWindow *ui;
    map<QString, Matrix*> matrices;
    Matrix* currentMatrix;
};

#endif // MAINWINDOW_H
