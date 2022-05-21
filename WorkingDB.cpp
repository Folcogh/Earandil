#include "WorkingDB.hpp"
#include "Dialog/AddDocumentation.hpp"
#include "Dialog/EditDocPath.hpp"
#include "Dialog/SetDBFilename.hpp"
#include "Global.hpp"
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QList>
#include <QMessageBox>
#include <algorithm>

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

WorkingDB::~WorkingDB()
{
    qDeleteAll(this->DocProp.begin(), this->DocProp.end());
}

bool WorkingDB::newDB(QWidget* parent)
{
    // Use temporary vars to keep the current database alive if the user cancels creation process
    QString path;
    QString filename;
    QString name;

    // Early return if a database is already opened and user doen't want to save it
    if (this->Active && !closeDB(parent)) {
        return false;
    }

    // Define a documentation path
    path = EditDocPath::newPath(parent);
    if (path.isNull()) {
        return false;
    }

    // Define a db filename
    if (!SetDBFilename::newFilename(parent, filename, name)) {
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
    bool ret = false;

    if (closeDB(parent)) {
        QString filename = QFileDialog::getOpenFileName(parent, WINDOW_TITLE, QDir::homePath(), APPLICATION_NAME " database (*." DATABASE_EXTENSION ")");
        if (!filename.isNull()) {
            QFile file(filename);
            if (file.open(QIODeviceBase::ReadOnly)) {
                QDataStream stream(&file);

                // Read file
                qsizetype DocpropCount;
                stream >> this->Name >> this->Path >> DocpropCount;

                // Read DocProperty list
                for (int i = 0; i < DocpropCount; i++) {
                    DocProperty* docprop;
                    stream >> &docprop;
                    this->DocProp.append(docprop);
                    addMachine(docprop->getMachine());
                }

                // End: Check stream status
                if (stream.status() == QDataStream::Ok) {
                    refreshDocuments();
                    this->Active   = true;
                    this->Modified = false;
                    this->Filename = filename;
                    ret            = true;
                }
                else { // stream >>() failed
                    QMessageBox::critical(parent, WINDOW_TITLE, QString("Error while reading file %1.").arg(filename));
                }
            }
            else { // QFile::open() failed
                QMessageBox::critical(parent, WINDOW_TITLE, QString("Error while opening file %1.").arg(filename));
            }
        }
        else { // User cancelled file picking, nothing to do
        }
    }
    else { // User didn't want to close, or something wrong happened. Nothing to do
    }

    return ret;
}

bool WorkingDB::saveDB(QWidget* parent)
{
    bool ret = false;

    // Open file
    QFile file(this->Filename);
    if (file.open(QIODeviceBase::WriteOnly)) {
        // Create stream
        QDataStream stream(&file);

        // Write data
        stream << this->Name << this->Path << this->DocProp.size();
        for (int i = 0; i < this->DocProp.size(); i++) {
            stream << this->DocProp.at(i);
        }

        // Check stream status
        if (stream.status() == QDataStream::Ok) {
            this->Modified = false;
            ret            = true;
        }
        else { // Stream status != Ok
            QMessageBox::critical(parent, WINDOW_TITLE, QString("Unable to write into the file %1. Save operation cannot be completed.").arg(this->Filename));
        }
    }
    else { // file.open() == false
        QMessageBox::critical(parent, WINDOW_TITLE, QString("Unable to open the file %1. Save operation cannot be completed.").arg(this->Filename));
    }

    return ret;
}

//  close
//
// Close the current DB if one is opened
// Return true if DB was successfully closed
// Return true if no DB is opened
//
bool WorkingDB::closeDB(QWidget* parent)
{
    // Database not opened: nothing to do
    if (!this->Active) {
        return true;
    }

    // Offer to save the DB if it was modified
    if (this->Modified) {
        QMessageBox::StandardButtons answer = QMessageBox::question(
            parent, WINDOW_TITLE, "Do you want to save the current database?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        // User wants to cancel closing process
        if (answer == QMessageBox::Cancel) {
            return false;
        }

        // User wants to save data before closing
        if (answer == QMessageBox::Yes) {
            saveDB(parent);
        }
    }

    // In any case, we want to drop this DB
    qDeleteAll(this->DocProp.begin(), this->DocProp.end());
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

void WorkingDB::refreshDocuments()
{
    // Parse the documentation directory
    parseDocDirectory();

    // Parse the Document list and remove the linked documentations.
    // Set the Orphean flag of the Documents.
    // Set the Orphean count accordingly.
    this->Orpheans = 0;

    for (int i = 0; i < this->DocProp.size(); i++) {                    // Parse Document list
        QString DocFilename = this->DocProp.at(i)->getFilename();       // Filename in DocProperty
        bool Match          = false;                                    // Default: no linked documentation
        for (int u = 0; u < this->UnlinkedDocumentations.size(); u++) { // Parse unlinked doc files
            QFileInfo Info(this->UnlinkedDocumentations.at(u));         // Read doc file name
            if (Info.fileName() == DocFilename) {                       // Compare with Document name
                this->UnlinkedDocumentations.removeAt(u);               // If match, remove from unlinked file list
                Match = true;                                           // Linked now
                break;                                                  // No need to continue
            }
        }

        // Update orphean status (WorkingDB + Document)
        if (Match) {                                // Match found
            this->DocProp.at(i)->setOrphean(false); // So flag Document as linked
        }
        else {
            this->Orpheans++;                      // Else update orphean count
            this->DocProp.at(i)->setOrphean(true); // And flag Document as orphean
        }
    }

    this->Modified = true;
}

void WorkingDB::addDocument(QWidget* parent)
{
    refreshDocuments();
    if (this->UnlinkedDocumentations.isEmpty()) {
        QMessageBox::information(parent, WINDOW_TITLE, "No new document to add to the database");
    }
    else {
        QList<DocProperty*> list = AddDocumentation::addDocumentation(parent);
        if (!list.isEmpty()) {
            this->DocProp.append(list);
        }
    }
}

void WorkingDB::addMachine(QString machine)
{
    if (!this->MachineList.contains(machine)) {
        this->MachineList.append(machine);
        std::sort(this->MachineList.begin(), this->MachineList.end());
    }
}
