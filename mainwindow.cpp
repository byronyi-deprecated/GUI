#include <fstream>
#include <string>
#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->inputGroupBox->setDisabled(true);
    ui->executeButton->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Load Training Set"),
                                                          ".", tr("Training Set files (*.txt)"));


    if(fileNames.empty())
        return;

    ui->log->append("Loading files...\n");

    for(QStringList::Iterator iter = fileNames.begin(); iter != fileNames.end(); ++iter)
        if(!loadFile(*iter))
        {
            ui->log->append("Error in loading " + *iter + '\n');
            return;
        }

    ui->log->append("Initializing successfully\n");

    ui->inputGroupBox->setEnabled(true);
}

bool MainWindow::loadFile(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    ui->console->clear();

    string inputFileName=fileName.toLocal8Bit().constData();
    /*const char* chtempInput=inputFileName.c_str();
    ifstream fin(chtempInput);*/
    Load_TainVali_Matrix(inputFileName,matrix,classType,row,column,p0,p1);

    /*int temp;
    fin>>temp;
    ui->console->append("temp");
    fin.close();*/


    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        ui->console->append(line);
    }

    return true;
}

void MainWindow::on_executeButton_clicked()
{
    ui->console->append("=====");
    ui->console->append(QString("The user has input: "));
    ui->console->append(ui->input1LineEdit->text());
}

void MainWindow::on_input1LineEdit_textEdited(const QString &arg1)
{
    ui->executeButton->setDisabled(arg1.isEmpty());
}

void MainWindow::on_actionAboutQt_triggered()
{
    qApp->aboutQt();
}
