#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include <QProcessEnvironment>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
using namespace std;

// Block size (area around offset to search for hex string)
#define BLOCKSIZE 63

// Macro to manage mkdir call (Windows vs Unix)
#ifdef _WIN32
#include <direct.h>
#define MKDIR(PATH) mkdir(PATH)
#else
#include <sys/stat.h>
#define MKDIR(PATH) mkdir(PATH, 0775)
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readSettings();
    detectPaths();
    selectGame(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readSettings()
{
  QSettings settings("Games.ini", QSettings::IniFormat);
  QStringList groups = settings.childGroups();
  int nbgroups = groups.size();

  for(int i=0; i<nbgroups; ++i)
  {
    settings.beginGroup(groups[i]);

    // Name of the game
    GameFWL newgame;
    newgame.setName(groups[i]);

    // AppID, savegames extension and offset to strip header
    newgame.setAppid(settings.value("app","0").toString());
    newgame.setExtension(settings.value("extension").toString());
    newgame.setOffset(settings.value("offset").toString());
    newgame.setHexstring(settings.value("hexstring").toString().toUpper());

    // Get possible paths to the savegames
    QStringList steamcloud = settings.value("steamcloud").toStringList();
    QStringList steamapps = settings.value("steamapps").toStringList();
    QStringList documents = settings.value("documents").toStringList();
    QStringList local = settings.value("local").toStringList();
    QStringList appdata = settings.value("appdata").toStringList();
    QStringList publicfolder = settings.value("public").toStringList();
    QStringList publicdata = settings.value("publicdata").toStringList();

    // Check if paths are empty before storing them
    if(steamcloud.size() > 1)
      if(!steamcloud[0].isEmpty())
        newgame.setPathInSteamCloud(steamcloud);

    if(steamapps.size() > 1)
      if(!steamapps[0].isEmpty())
        newgame.setPathInSteamApps(steamapps);

    if(documents.size() > 1)
      if(!documents[0].isEmpty())
        newgame.setPathInDocs(documents);

    if(local.size() > 1)
      if(!local[0].isEmpty())
        newgame.setPathInLocal(local);

    if(appdata.size() > 1)
      if(!appdata[0].isEmpty())
        newgame.setPathInAppData(appdata);

    if(publicfolder.size() > 1)
      if(!publicfolder[0].isEmpty())
        newgame.setPathInPublic(publicfolder);

    if(publicdata.size() > 1)
      if(!publicdata[0].isEmpty())
        newgame.setPathInPublicData(publicdata);

    // Done reading game info
    settings.endGroup();

    // New game added to the list of games
    games.push_back(newgame);
  }
  sort(games.begin(),games.end());

  foreach(GameFWL game, games)
    ui->game_selection->addItem(game.getName());
}

void MainWindow::detectPaths()
{
  QSettings settings("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);
  steampath = QDir::toNativeSeparators(settings.value("SteamPath", "").toString());

  QStringList doclist = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
  if(!doclist.empty())
    documentspath = QDir::toNativeSeparators(doclist[0]);

  QStringList genericdata = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
  if(!genericdata.empty())
    localpath = genericdata[0] + QDir::separator();

  if(genericdata.size() > 1)
    publicdatapath = genericdata[1] + QDir::separator();

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  appdatapath = env.value("APPDATA") + QDir::separator();
  publicpath = env.value("PUBLIC") + QDir::separator();

  // Add trailing separators if paths not empty
  if(!steampath.isEmpty())
    steampath += QDir::separator();
  if(!documentspath.isEmpty())
    documentspath += QDir::separator();
  if(!localpath.isEmpty())
    localpath += QDir::separator();
  if(!appdatapath.isEmpty())
    appdatapath += QDir::separator();
  if(!publicdatapath.isEmpty())
    publicdatapath += QDir::separator();
  if(!publicpath.isEmpty())
    publicpath += QDir::separator();
}

void MainWindow::detectSavePath()
{
  int found = 0;
  GameFWL game = games[gameidx];

  QStringList possiblepaths;
  QString detectedpath;

  // We add the possible paths in documents
  if(!documentspath.isEmpty())
    foreach(QString path, game.getPathInDocs())
      possiblepaths.push_back(QDir::toNativeSeparators(documentspath + path));

  // We add the possible paths in Steam Cloud
  QString steamcloudpath;
  if(!steampath.isEmpty() && !game.getPathInSteamCloud().isEmpty())
  {
    steamcloudpath = steampath + QString("userdata") + QDir::separator();

    // We look in each Steam user folder
    QDir userdata(steamcloudpath);
    QFileInfoList steamusers = userdata.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time);
    for(int i=0; i<steamusers.size() && !found; ++i)
    {
      QString user = QDir::toNativeSeparators(steamusers[i].canonicalFilePath() + QDir::separator());
      foreach(QString appid, game.getPathInSteamCloud())
        possiblepaths.push_back(QDir::toNativeSeparators(user + appid + QString("/remote")));
    }
  }

  // We add the possible paths in SteamApps
  QString steamappspath;
  if(!steampath.isEmpty())
  {
    steamappspath = steampath + QString("steamapps") + QDir::separator();
    foreach(QString path, game.getPathInSteamApps())
      possiblepaths.push_back(QDir::toNativeSeparators(steamappspath + path));
  }

  // We add the possible paths in "appdata"
  if(!appdatapath.isEmpty())
    foreach(QString path, game.getPathInAppData())
      possiblepaths.push_back(QDir::toNativeSeparators(appdatapath + path));

  // We add the possible paths in "local"
  if(!localpath.isEmpty())
    foreach(QString path, game.getPathInLocal())
      possiblepaths.push_back(QDir::toNativeSeparators(localpath + path));

  // We add the possible paths in "public"
  if(!publicpath.isEmpty())
    foreach(QString path, game.getPathInPublic())
      possiblepaths.push_back(QDir::toNativeSeparators(publicpath + path));

  // We add the possible paths in "publicdata"
  if(!publicdatapath.isEmpty())
    foreach(QString path, game.getPathInPublicData())
      possiblepaths.push_back(QDir::toNativeSeparators(publicdatapath + path));

  for(int i=0; i<possiblepaths.size() && found<1; ++i)
  {
    QDir folder(possiblepaths[i]);
    QFileInfoList files = folder.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
    foreach(QFileInfo file, files)
    {
      if(file.completeSuffix() == game.getExtension())
        ++found;
      if(found > 0)
        detectedpath = folder.canonicalPath();
    }

    if(found == 0)
    {
      QFileInfoList subfolders = folder.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time);
      foreach(QFileInfo subfolder, subfolders)
      {
        QDir subdir(subfolder.canonicalFilePath());
        QFileInfoList subfiles = subdir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
        foreach(QFileInfo subfile, subfiles)
        {
          if(subfile.completeSuffix() == game.getExtension())
            ++found;
          if(found > 0)
            detectedpath = subdir.canonicalPath();
        }
      }
    }
  }

  ui->filename_lineEdit->setText(QDir::toNativeSeparators(detectedpath));
  ui->statusBar->showMessage(QString("Detected files: ") + QString::number(found));
  selectFolder(detectedpath);
}

void MainWindow::selectFolder(const QString & path)
{
  if(path.isEmpty())
  {
    QString currentpath = ui->filename_lineEdit->text();
    savepath = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Choose savegame directory"),currentpath));
    if(savepath != 0)
    {
      ui->filename_lineEdit->setText(savepath);
      ui->start_button->setEnabled(true);
      ui->statusBar->clearMessage();
    }
    else
    {
      ui->start_button->setEnabled(false);
    }
  }
  else
  {
    savepath = path;
    ui->start_button->setEnabled(true);
  }
}

void MainWindow::processFolder()
{
  QString message;
  QDir savefolder(savepath);
  GameFWL game = games[gameidx];
  int processed = 0;
  int copied = 0;

  QString appid = game.getAppid();
  QString outputfolder = appid + QDir::separator() + "remote" + QDir::separator();

  if(savefolder.exists())
  {
    QFileInfoList files = savefolder.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    for(int i=0; i<files.size(); ++i)
    {
      if(files[i].completeSuffix() == game.getExtension())
      {
        int mkd1 = MKDIR(appid.toStdString().c_str());
        if(mkd1 == 0 || errno == EEXIST)
        {
          int mkd2 = MKDIR(outputfolder.toStdString().c_str());
          if(mkd2 == 0 || errno == EEXIST)
          {
            QString savefile = files[i].canonicalFilePath();
            QString newsavefile = outputfolder + files[i].fileName();

            ifstream input_file(savefile.toStdString().c_str(),ios::binary);
            ofstream output_file(newsavefile.toStdString().c_str(),ios::binary);

            if (input_file.is_open() && output_file.is_open())
            {
              // We try to find the hex string around "offset"
              int firsthalf = (BLOCKSIZE+1)/2;
              char memblock[BLOCKSIZE];
              input_file.seekg(game.getOffset()-firsthalf);
              input_file.read(memblock,BLOCKSIZE);

              ostringstream oss;
              for (int block = 0; block < BLOCKSIZE; ++block)
              {
                int z = memblock[block]&0xff;
                oss << hex << setfill('0') << setw(2) << uppercase << z;
              }
              QString filehexstring = QString::fromStdString(oss.str());
              int hexpos = filehexstring.indexOf(game.getHexstring());

              // If we found the string, we cut where it starts
              if(hexpos != -1)
                input_file.seekg(game.getOffset()-firsthalf+hexpos/2);
              // Otherwise, we use the default offset
              else
              {
                // We try to look for the hex string around the start
                input_file.seekg(0);
                input_file.read(memblock,BLOCKSIZE);

                ostringstream oss2;
                for (int block = 0; block < BLOCKSIZE; ++block)
                {
                  int z = memblock[block]&0xff;
                  oss2 << hex << setfill('0') << setw(2) << uppercase << z;
                }
                filehexstring = QString::fromStdString(oss2.str());
                hexpos = filehexstring.indexOf(game.getHexstring());

                if(hexpos != -1)
                  input_file.seekg(hexpos/2);
                else
                  input_file.seekg(game.getOffset());
              }

              output_file << input_file.rdbuf();
              output_file.close();
              input_file.close();
            }
            ++processed;
          }
        }
      }
      else
      {
        int mkd1 = MKDIR(appid.toStdString().c_str());
        if(mkd1 == 0 || errno == EEXIST)
        {
          int mkd2 = MKDIR(outputfolder.toStdString().c_str());
          if(mkd2 == 0 || errno == EEXIST)
          {
            QString destfile = outputfolder + files[i].fileName();
            QFile::copy(files[i].canonicalFilePath(),destfile);
            ++copied;
          }
        }
      }
    }
  }
  message = "Processed " + QString::number(processed) + " file(s) & copied " + QString::number(copied) + " file(s) into " + appid + ".";
  ui->statusBar->showMessage(message);
}
