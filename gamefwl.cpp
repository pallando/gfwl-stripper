#include "gamefwl.h"

GameFWL::GameFWL()
{
}

QString GameFWL::getName() const
{
  return name;
}
void GameFWL::setName(const QString & value)
{
  name = value;
}

QString GameFWL::getAppid() const
{
  return appid;
}
void GameFWL::setAppid(const QString & value)
{
  appid = value;
}

QString GameFWL::getSavename() const
{
  return savename;
}
void GameFWL::setSavename(const QString &value)
{
  savename = value;
}

QString GameFWL::getExtension() const
{
  return extension;
}
void GameFWL::setExtension(const QString & value)
{
  extension = value;
}

QStringList GameFWL::getPathInSteamCloud() const
{
  return pathInSteamCloud;
}
void GameFWL::setPathInSteamCloud(const QStringList & value)
{
  pathInSteamCloud = value;
}

QStringList GameFWL::getPathInSteamApps() const
{
  return pathInSteamApps;
}
void GameFWL::setPathInSteamApps(const QStringList & value)
{
  pathInSteamApps = value;
}

QStringList GameFWL::getPathInDocs() const
{
  return pathInDocs;
}
void GameFWL::setPathInDocs(const QStringList & value)
{
  pathInDocs = value;
}

QStringList GameFWL::getPathInLocal() const
{
  return pathInLocal;
}
void GameFWL::setPathInLocal(const QStringList & value)
{
  pathInLocal = value;
}

QStringList GameFWL::getPathInAppData() const
{
  return pathInAppData;
}
void GameFWL::setPathInAppData(const QStringList & value)
{
  pathInAppData = value;
}

QStringList GameFWL::getPathInPublic() const
{
  return pathInPublic;
}
void GameFWL::setPathInPublic(const QStringList & value)
{
  pathInPublic = value;
}

QStringList GameFWL::getPathInPublicData() const
{
  return pathInPublicData;
}
void GameFWL::setPathInPublicData(const QStringList & value)
{
  pathInPublicData = value;
}


int GameFWL::getOffset() const
{
  return offset;
}
void GameFWL::setOffset(const QString & value)
{
  bool ok;
  offset = value.toInt(&ok,16);
  if(!ok)
    offset = 0;
}


