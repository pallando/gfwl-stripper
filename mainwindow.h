#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "gamefwl.h"
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

private:
    void readSettings();
    void detectPaths();
    void detectSavePath();

private slots:
    void selectGame(int selected_game) { gameidx = selected_game; detectSavePath(); }
    void selectFolder(const QString & path=QString());
    void processFolder();

private:
    Ui::MainWindow * ui;
    vector<GameFWL> games;
    int gameidx;
    QString steampath;
    QString documentspath;  // DocumentsLocation: 0
    QString appdatapath;    // QProcessEnvironment::systemEnvironment(); %APPDATA%
    QString localpath;      // GenericDataLocation: 0
    QString publicpath;     // QProcessEnvironment::systemEnvironment(); %PUBLIC%
    QString publicdatapath; // GenericDataLocation: 1
    QString savepath;
};

#endif // MAINWINDOW_H
