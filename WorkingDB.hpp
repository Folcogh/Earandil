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
    // Singleton
    static WorkingDB* instance();
    static void release();

    // Menus
    bool newDB(QWidget* parent);
    bool openDB(QWidget* parent);
    bool save();
    bool close(QWidget* parent);

    bool docAdd(QWidget* parent);

    // Predicates
    bool isActive() const { return Active; }
    bool isModified() const { return Modified; }
    bool hasOrpheans() const { return Orpheans != 0; }

    // Getters
    QString getFilename() const { return this->Filename; }
    QStringList getUnlinkedDocs() const { return this->UnlinkedDocumentations; }
    QString getDocPath() const { return this->Path; }
    QStringList getMachineList() const { return this->MachineList; }

    // Misc
    void addMachine(QString machine);

  private:
    static WorkingDB* workingdb;        // Instance ptr
    bool Active;                        // True if a working db exists. So it false at application startup
    bool Modified;                      // True is the DB was modified since last save, or never saved
    int Orpheans;                       // Number of orphean Documents
    QString Name;                       // User-defined name of the DB. Un-related to filename or path
    QString Filename;                   // Full path + filename of the db
    QString Path;                       // Path to the documentation files
    QStringList UnlinkedDocumentations; // List of documentation files which are not added yet to the database
    QList<DocProperty*> DocProp;        // List of registered documents. May contain orpheans
    QStringList MachineList;            // List of known machine name

    WorkingDB();
    ~WorkingDB();
    void refreshDocuments();
    void parseDocDirectory();
};

#endif // WORKINGDB_HPP
