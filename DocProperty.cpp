#include "DocProperty.hpp"
#include "Global.hpp"
#include "Languages.hpp"

DocProperty::DocProperty(QString StrNumber, QString Machine, int DevStep, int SN_prefix, int FromSN, int ToSN, bool Orphean)
    : StrNumber(StrNumber)
    , Machine(Machine)
    , DevStep(DevStep)
    , SN_prefix(SN_prefix)
    , FromSN(FromSN)
    , ToSN(ToSN)
    , Orphean(Orphean)
{
    // Documentation filename format:
    // <doctype>-<serial>-<language><version>.<extension>
    // Doctype is 2 or 3 alpha characters
    // Serial is an integer (random length)
    // Language and Version are integers (length = 2)
    QString Base = this->StrNumber;
    Base.chop(QString(DOCUMENTATION_DOT_EXTENSION).size());
    QString Suffix = Base.last(LANGUAGE_LENGTH + VERSION_LENGTH);
    this->Version  = Suffix.last(VERSION_LENGTH).toInt();

    // Translate the language code into a string
    int language   = Suffix.first(LANGUAGE_LENGTH).toInt();
    this->Language = Languages::instance()->getLanguage(language);
}

DocProperty::DocProperty(QString StrNumber, QString Machine, QString DevStep, QString FromSN, QString ToSN, bool Orphean)
    : DocProperty(StrNumber,
                  Machine,
                  DevStep.toInt(),
                  FromSN.first(SN_PREFIX_LENGTH).toInt(),
                  FromSN.last(SN_SUFFIX_LENGTH).toInt(),
                  ToSN.isEmpty() ? TO_SN_UNDEFINED : ToSN.toInt(),
                  Orphean)
{
}

void DocProperty::getData(QString& StrNumber, QString& Machine, int& DevStep, int& SN_prefix, int& FromSN, int& ToSN) const
{
    StrNumber = this->StrNumber;
    Machine   = this->Machine;
    DevStep   = this->DevStep;
    SN_prefix = this->SN_prefix;
    FromSN    = this->FromSN;
    ToSN      = this->ToSN;
}

// Read a document from a stream
QDataStream& operator>>(QDataStream& stream, DocProperty** docprop)
{
    QString StrNumber;
    QString Machine;
    int DevStep;
    int SN_prefix;
    int FromSN;
    int ToSN;

    stream >> StrNumber >> Machine >> DevStep >> SN_prefix >> FromSN >> ToSN;
    *docprop = new DocProperty(StrNumber, Machine, DevStep, SN_prefix, FromSN, ToSN);

    return stream;
}

// Write a document in a stream
QDataStream& operator<<(QDataStream& stream, const DocProperty* docprop)
{
    QString StrNumber;
    QString Machine;
    int DevStep;
    int SN_prefix;
    int FromSN;
    int ToSN;

    docprop->getData(StrNumber, Machine, DevStep, SN_prefix, FromSN, ToSN);
    stream << StrNumber << Machine << DevStep << SN_prefix << FromSN << ToSN;

    return stream;
}
