#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include <QDataStream>
#include <QMetaType>
#include <QString>

#define DOCUMENTATION_EXTENSION "PDF"

// Length of the infos in the document filename
#define LANGUAGE_LENGTH 2
#define VERSION_LENGTH 2

//  Document
//
// This class represent a pdf and its properties
//
class Document
{
  public:
    Document(QString StrNumber, QString Machine, int DevStep, int FromSN, int ToSN);
    QString getFilename() const { return this->StrNumber; }
    void setOrphean(bool orphean) { this->Orphean = orphean; }

  private:
    // Description saved in the data file
    QString StrNumber; // Basically, it's the file name without the "pdf" extension
    QString Machine;   // Machine type (A3F, A3CF, etc...)
    int DevStep;       // 100, 150, etc
    int FromSN;        // First serial number of the machine for this document
    int ToSN;          // Last serial number. -1 if it doesn't apply

    // Handy access to data described in the StrNumber
    int Version;      // Documentation version
    QString Language; // Documentation language

    // Misc
    bool Orphean; // True if not linked to a documentation file
};

// Serialization
QDataStream& operator>>(QDataStream& stream, Document** document);

// Document* is used as data type in widgets
Q_DECLARE_METATYPE(Document*)

#endif // DOCUMENT_HPP
