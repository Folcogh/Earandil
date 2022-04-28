#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QSettings>
#include <QStringList>

#define KEY_RECENT_DATABASES "RecentDatabases"

class Settings : public QSettings
{
  public:
    static Settings* instance();
    static void release();
    QStringList getRecentDatabases();
    bool hasRecentAvailable();
    void addRecentDatabase(QString filename);

  private:
    static Settings* settings;
    QStringList RecentDatabases;

    Settings();
    ~Settings() override;
    void cleanRecentDatabases();
};

#endif // SETTINGS_HPP
