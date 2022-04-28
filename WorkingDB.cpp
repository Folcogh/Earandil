#include "WorkingDB.hpp"
#include "DlgDocAdd.hpp"
#include "DlgDocSetPath.hpp"
#include "DlgSetDBFile.hpp"
#include "Document.hpp"
#include "Global.hpp"
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QMessageBox>

WorkingDB* WorkingDB::workingdb = nullptr;

WorkingDB* WorkingDB::instance()
{
    if (workingdb == nullptr) {
        workingdb = new WorkingDB;
    }
    return workingdb;
}

void WorkingDB::release()
{
    if (workingdb != nullptr) {
        delete workingdb;
        workingdb = nullptr;
    }
}

WorkingDB::WorkingDB()
    : Active(false)
    , Modified(false)
    , Orpheans(0)
{
}

bool WorkingDB::newDB(QWidget* parent)
{
    // Use temporary vars to keep the current database alive if the user cancels creation process
    QString path;
    QString filename;
    QString name;

    // Early return if a database is already opened and user doen't want to save it
    if (this->Active && !close(parent)) {
        return false;
    }

    // Define a documentation path
    path = DlgDocSetPath::newPath(parent);
    if (path.isNull()) {
        return false;
    }

    // Define a db file
    if (!DlgSetDBFile::newDBFile(parent, filename, name)) {
        return false;
    }

    // Complete data
    this->Active   = true;
    this->Modified = true;
    this->Orpheans = 0;
    this->Path     = path;
    this->Filename = filename;
    this->Name     = name;

    return true;
}

bool WorkingDB::openDB(QWidget* parent)
{
    (void)parent;
    return true;
}

//  close
//
// Close the current DB if one is opened
// Return true if DB was successfully closed
// Return true if no DB is opened
//
bool WorkingDB::close(QWidget* parent)
{
    // Database not opened: nothing to do
    if (!this->Active) {
        return true;
    }

    // Database not modified: just drop it
    if (!this->Modified) {
        this->Active = false;
        return true;
    }

    // Else ask the user if he wants to save it
    QMessageBox::StandardButtons answer
        = QMessageBox::question(parent, WINDOW_TITLE, "Do you want to save the current database?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    // User wants to cancel closing process
    if (answer == QMessageBox::Cancel) {
        return false;
    }

    // User wants to save data before closing
    if (answer == QMessageBox::Yes) {
        save();
    }

    // In any case, we wan't to drop this DB
    this->Active = false;

    return true;
}

//  parseDocDirectory
//
// Populate the list of PDF files in the documentation directory
// Doesn't remove the unlinked documentations
//
void WorkingDB::parseDocDirectory()
{
    QDir Directory(this->Path);
    const QStringList Names("*." DOCUMENTATION_EXTENSION);
    Directory.setNameFilters(Names);
    Directory.setSorting(QDir::Name | QDir::IgnoreCase);
    Directory.setFilter(QDir::Files | QDir::Readable);
    this->UnlinkedDocumentations = Directory.entryList();
}

bool WorkingDB::save()
{
    return true; // TO BE REMOVED
}

void WorkingDB::refreshDocuments()
{
    // Parse the documentation directory
    parseDocDirectory();

    // Parse the Document list and remove the linked documentations.
    // Set the Orphean flag of the Documents.
    // Set the Orphean count accordingly.
    this->Orpheans = 0;

    for (int i = 0; i < this->Documents.size(); i++) {                  // Parse Document list
        QString DocFilename = this->Documents.at(i)->getFilename();     // Filename in Document
        bool Match          = false;                                    // Default: no linked documentation
        for (int u = 0; u < this->UnlinkedDocumentations.size(); u++) { // Parse unlinked doc files
            QFileInfo Info(this->UnlinkedDocumentations.at(u));         // Read doc file name
            if (Info.completeBaseName() == DocFilename) {               // Compare with Document name
                this->UnlinkedDocumentations.removeAt(u);               // If match, remove from unlinked file list
                Match = true;                                           // Linked now
                break;                                                  // No need to continue
            }
        }

        // Update orphean status (WorkingDB + Document)
        if (Match) {                                  // Match found
            this->Documents.at(i)->setOrphean(false); // So flag Document as linked
        }
        else {
            this->Orpheans++;                        // Else update orphean count
            this->Documents.at(i)->setOrphean(true); // And flag Document as orphean
        }
    }
}

bool WorkingDB::docAdd(QWidget* parent)
{
    bool ret;

    refreshDocuments();
    QList<Document*> list = DlgDocAdd::addDocumentations(parent);
    if (!list.isEmpty()) {
        this->Documents.append(list);
        ret = true;
    }
    else {
        QMessageBox::information(parent, WINDOW_TITLE, "No new document to add to the database");
        ret = false;
    }
    return ret;
}
