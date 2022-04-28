#ifndef LANGUAGES_HPP
#define LANGUAGES_HPP

#include <QList>
#include <QPair>
#include <QString>

//  Languages
//
// This class allows to encode/decode the TP language code and the language name
//
class Languages
{
  public:
    static Languages* instance();
    static void release();
    QString getLanguage(int value);

  private:
    static Languages* languages;
    Languages();
    QList<QPair<int, QString>> Map;
};

#endif // LANGUAGES_HPP
