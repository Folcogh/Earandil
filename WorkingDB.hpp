#ifndef WORKINGDB_HPP
#define WORKINGDB_HPP

#include "Document.hpp"
#include <QString>
#include <QStringList>
#include <QWidget>

//  WorkingDB
//
// Holds the currently opened database
//
class WorkingDB
{
  public:
    static WorkingDB* instance();
    static void release();

    bool newDB(QWidget* parent);
    bool openDB(QWidget* parent);
    bool save();
    bool close(QWidget* parent);

    bool docAdd(QWidget* parent);

    bool isActive() const { return Active; }
    bool isModified() const { return Modified; }
    bool hasOrpheans() const { return Orpheans != 0; }

    QString getFilename() const { return this->Filename; }
    QStringList getUnlinkedDocs() const { return this->UnlinkedDocumentations; }
    QString getDocPath() const { return this->Path; }

  private:
    static WorkingDB* workingdb;        // Instance ptr
    bool Active;                        // True if a working db exists. So it false at application startup
    bool Modified;                      // True is the DB was modified since last save, or never saved
    int Orpheans;                       // Number of orphean Documents
    QString Name;                       // User-defined name of the DB. Un-related to filename or path
    QString Filename;                   // Full path + filename of the db
    QString Path;                       // Path to the documentation files
    QStringList UnlinkedDocumentations; // List of documentation files which are not added yet to the database
    QList<Document*> Documents;         // List of registered documents. May contain orpheans

    WorkingDB();
    void refreshDocuments();
    void parseDocDirectory();
};

#endif // WORKINGDB_HPP
