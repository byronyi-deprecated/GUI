#include <fstream>
#include <string>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileutils.h"

#include <QDebug>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  currentMatrix(NULL)
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
    if(!loadFile(*iter)) {
        ui->log->append("Error in loading " + *iter + '\n');
        return;
      }

  ui->log->append("Initializing successfully\n");

  ui->inputGroupBox->setEnabled(true);

  string firstLine = currentMatrix->outputFirstLine();
  ui->console->append(QString(firstLine.c_str()));
}

bool MainWindow::loadFile(QString fileName)
{
  map<QString, Matrix*>::iterator found = matrices.find(fileName);
  if (found != matrices.end()) {
      currentMatrix = found->second;
      return true;
    }

  ifstream ifs;
  if (!openFile(ifs, fileName.toStdString()))
    return false;

  Matrix* new_matrix = new Matrix;

  if (!(new_matrix->load(ifs, true))) {
      delete new_matrix;
      return false;
    }

  matrices[fileName] = new_matrix;
  currentMatrix = new_matrix;
  return true;
}

void MainWindow::on_executeButton_clicked()
{
  ui->console->append("=====");
  ui->console->append(QString("The user has input: "));
  ui->console->append(ui->input1LineEdit->text());
}

void MainWindow::on_actionAboutQt_triggered()
{
  qApp->aboutQt();
}
