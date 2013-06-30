#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QIcon>

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

    void on_input1LineEdit_textEdited(const QString &arg1);

    void on_actionAboutQt_triggered();

private:
    bool loadFile(QString);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
