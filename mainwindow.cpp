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
}

MainWindow::~MainWindow()
{
  delete ui;
  for (map<QString, Matrix*>::iterator iter = matrices.begin(); iter != matrices.end(); ++iter)
    delete iter->second;
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

  QString col;
  col.setNum(currentMatrix->col());
  QString p0;
  p0.setNum(currentMatrix->p0());
  QString p1;
  p1.setNum(currentMatrix->p1());

  QString msg("The class Type of the training set consist ");
  msg += col + " samples.\n" + "The fraction of 0 is ";
  msg += p0 + "\n" + "The fraction of 1 is " + p1 + "\n";

  ui->console->append(msg);
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
  int trialTimes = ui->trialTimesSpinBox->value();
  int startingSize = ui->startingSizeCombo->currentText().toInt();

  currentMatrix->findModules(trialTimes, startingSize);

  ui->console->append("Find all modules!");
}

void MainWindow::on_actionAboutQt_triggered()
{
  qApp->aboutQt();
}

void MainWindow::on_trialTimesSlider_valueChanged(int value)
{
  int round_off = value / 100000;
  round_off *= 100000;
  ui->trialTimesSlider->setValue(round_off);
}
