#ifndef GAMEFWL_H
#define GAMEFWL_H

#include <QString>
#include <QStringList>

class GameFWL
{
  public:
    GameFWL();

    bool operator<(const GameFWL & g) const { return this->name < g.name; }
    bool operator==(const GameFWL & g) const { return this->name == g.name; }

    QString getName() const;
    void setName(const QString & value);

    QString getAppid() const;
    void setAppid(const QString & value);

    QString getExtension() const;
    void setExtension(const QString & value);

    int getOffset() const;
    void setOffset(const QString & value);

    QStringList getPathInSteamCloud() const;
    void setPathInSteamCloud(const QStringList & value);

    QStringList getPathInSteamApps() const;
    void setPathInSteamApps(const QStringList & value);

    QStringList getPathInDocs() const;
    void setPathInDocs(const QStringList & value);

    QStringList getPathInLocal() const;
    void setPathInLocal(const QStringList & value);

    QStringList getPathInAppData() const;
    void setPathInAppData(const QStringList & value);

    QStringList getPathInPublic() const;
    void setPathInPublic(const QStringList & value);

    QStringList getPathInPublicData() const;
    void setPathInPublicData(const QStringList & value);

    QString getHexstring() const;
    void setHexstring(const QString &value);

  private:
    QString     name;
    QString     appid;
    QString     extension;
    QString     hexstring;
    int         offset;
    QStringList pathInSteamCloud;
    QStringList pathInSteamApps;
    QStringList pathInDocs;
    QStringList pathInLocal;
    QStringList pathInAppData;
    QStringList pathInPublic;
    QStringList pathInPublicData;

};

#endif // GAMEFWL_H
