#include "Settings.hpp"
#include "Global.hpp"
#include <QFileInfo>

Settings* Settings::settings = nullptr;

Settings* Settings::instance()
{
    if (settings == nullptr) {
        settings = new Settings;
    }
    return settings;
}

void Settings::release()
{
    if (settings != nullptr) {
        delete settings;
        settings = nullptr;
    }
}

Settings::Settings()
    : QSettings(ORGANIZATION_NAME, APPLICATION_NAME)
    , RecentDatabases(value(KEY_RECENT_DATABASES).toStringList())
{
    cleanRecentDatabases();
}

Settings::~Settings()
{
    // Save the recent database list
    setValue(KEY_RECENT_DATABASES, this->RecentDatabases);
}

QStringList Settings::getRecentDatabases()
{
    return this->RecentDatabases;
}

//  hasRecentAvailable
//
// Return true if a recent database, which is not the current one, is available
// It means that we must have at least 2 entries in the list
//
bool Settings::hasRecentAvailable()
{
    return this->RecentDatabases.size() >= 2;
}

//  cleanRecentDatabases
//
// Remove non-existing recent files from the list
//
void Settings::cleanRecentDatabases()
{
    QStringList Tmp;
    for (int i = 0; i < RecentDatabases.size(); i++) {
        QString Database = RecentDatabases.at(i);
        if (QFileInfo::exists(Database)) {
            Tmp << Database;
        }
    }
    this->RecentDatabases = Tmp;
}

//  addRecentDatabase
//
// Add a file to the Recent list.
// Put it in last position if it was already present.
// Trash the last DB if the list grows over the limit. The older one is ranked #0
//
void Settings::addRecentDatabase(QString filename)
{
    if (this->RecentDatabases.contains(filename, Qt::CaseInsensitive)) {
        // Item already present, just move it to the end
        qsizetype index = this->RecentDatabases.indexOf(filename);
        this->RecentDatabases.move(index, RECENT_MAX_COUNT - 1);
    }
    else {
        // Add the item, and shorten the list if it's too big
        this->RecentDatabases.append(filename);
        if (this->RecentDatabases.size() > RECENT_MAX_COUNT) {
            this->RecentDatabases.removeFirst();
        }
    }
}
