#include "Document.hpp"
#include "Languages.hpp"

Document::Document(QString StrNumber, QString Machine, int DevStep, int FromSN, int ToSN)
    : StrNumber(StrNumber)
    , Machine(Machine)
    , DevStep(DevStep)
    , FromSN(FromSN)
    , ToSN(ToSN)
    , Orphean(true)
{
    // Documentation filename format:
    // <doctype>-<serial>-<language><version>
    // Doctype is 2 or 3 alpha characters
    // Serial is an integer (random length)
    // Language and Version are integers (length = 2)
    QString Suffix = this->StrNumber.last(LANGUAGE_LENGTH + VERSION_LENGTH);
    this->Version  = Suffix.last(VERSION_LENGTH).toInt();

    // Translate the language code into a string
    int language   = Suffix.first(LANGUAGE_LENGTH).toInt();
    this->Language = Languages::instance()->getLanguage(language);
}

// Read a document from a stream
QDataStream& operator>>(QDataStream& stream, Document** document)
{
    QString StrNumber;
    QString Machine;
    int DevStep;
    int FromSN;
    int ToSN;

    stream >> StrNumber >> Machine >> DevStep >> FromSN >> ToSN;
    *document = new Document(StrNumber, Machine, DevStep, FromSN, ToSN);

    return stream;
}
