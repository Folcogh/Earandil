#ifndef DOCPROPERTY_HPP
#define DOCPROPERTY_HPP

#include <QDataStream>
#include <QMetaType>
#include <QString>

#define DOCUMENTATION_EXTENSION "PDF"
#define DOCUMENTATION_DOT_EXTENSION ".PDF"

// Length of the infos in the document filename
#define LANGUAGE_LENGTH 2
#define VERSION_LENGTH 2

// Orphean status
#define IS_ORPHEAN true
#define IS_NOT_ORPHEAN false

// SN constant
#define TO_SN_UNDEFINED -1

//  DocProperty
//
// This class represent a pdf and its properties
//
class DocProperty
{
  public:
    DocProperty(QString StrNumber, QString Machine, int DevStep, int SN_prefix, int FromSN, int ToSN, bool Orphean = true);
    DocProperty(QString StrNumber, QString Machine, QString DevStep, QString FromSN, QString ToSN, bool Orphean = true);
    QString getFilename() const { return this->StrNumber; }
    void setOrphean(bool orphean) { this->Orphean = orphean; }
    void getData(QString& StrNumber, QString& Machine, int& DevStep, int& SN_prefix, int& FromSN, int& ToSN) const;

  private:
    // Description saved in the data file
    QString StrNumber; // Basically, it's the file name without the "pdf" extension
    QString Machine;   // Machine type (A3F, A3CF, etc...)
    int DevStep;       // 100, 150, etc
    int SN_prefix;     // First group of the SN
    int FromSN;        // First serial number of the machine for this document
    int ToSN;          // Last serial number, TO_SN_UNDEFINED if not applicable

    // Handy access to data described in the StrNumber
    int Version;      // Documentation version
    QString Language; // Documentation language

    // Misc
    bool Orphean; // True if not linked to a documentation file
};

// Serialization
QDataStream& operator>>(QDataStream& stream, DocProperty** docprop);
QDataStream& operator<<(QDataStream& stream, const DocProperty* docprop);

// Document* is used as data type in widgets
Q_DECLARE_METATYPE(DocProperty*)

#endif // DOCPROPERTY_HPP
