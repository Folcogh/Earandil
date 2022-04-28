#include "Languages.hpp"

Languages* Languages::languages = nullptr;

Languages::Languages()
{
    this->Map.append(QPair<int, QString>(1, "English"));
    this->Map.append(QPair<int, QString>(2, "Swedish"));
    this->Map.append(QPair<int, QString>(3, "German"));
    this->Map.append(QPair<int, QString>(4, "French"));
    this->Map.append(QPair<int, QString>(5, "Spanish"));
    this->Map.append(QPair<int, QString>(6, "Italian"));
    this->Map.append(QPair<int, QString>(7, "Portuguese"));
    this->Map.append(QPair<int, QString>(8, "Danish"));
    this->Map.append(QPair<int, QString>(9, "Greek"));
    this->Map.append(QPair<int, QString>(11, "Finnish"));
    this->Map.append(QPair<int, QString>(12, "Russian"));
    this->Map.append(QPair<int, QString>(13, "Latvian"));
    this->Map.append(QPair<int, QString>(14, "Estonian"));
    this->Map.append(QPair<int, QString>(15, "Lithuanian"));
    this->Map.append(QPair<int, QString>(16, "Serbo-Croatian"));
    this->Map.append(QPair<int, QString>(17, "Czech"));
    this->Map.append(QPair<int, QString>(18, "Polish"));
    this->Map.append(QPair<int, QString>(19, "Serbian"));
    this->Map.append(QPair<int, QString>(20, "Turkish"));
    this->Map.append(QPair<int, QString>(21, "Arabic"));
    this->Map.append(QPair<int, QString>(22, "Persian"));
    this->Map.append(QPair<int, QString>(23, "Malaysian"));
    this->Map.append(QPair<int, QString>(24, "Japanese"));
    this->Map.append(QPair<int, QString>(25, "Chinese Simplified"));
    this->Map.append(QPair<int, QString>(26, "Dutch"));
    this->Map.append(QPair<int, QString>(27, "Norwegian Bokmål"));
    this->Map.append(QPair<int, QString>(28, "Thai"));
    this->Map.append(QPair<int, QString>(29, "Icelandic"));
    this->Map.append(QPair<int, QString>(30, "Korean"));
    this->Map.append(QPair<int, QString>(31, "Croatian"));
    this->Map.append(QPair<int, QString>(32, "Slovenian"));
    this->Map.append(QPair<int, QString>(33, "Slovak"));
    this->Map.append(QPair<int, QString>(34, "Maltese"));
    this->Map.append(QPair<int, QString>(35, "Romanian"));
    this->Map.append(QPair<int, QString>(36, "Bulgarian"));
    this->Map.append(QPair<int, QString>(39, "Indonesian"));
    this->Map.append(QPair<int, QString>(40, "Ukrainian"));
    this->Map.append(QPair<int, QString>(44, "Norwegian Nynorsk"));
    this->Map.append(QPair<int, QString>(50, "Portuguese Brazilian"));
    this->Map.append(QPair<int, QString>(51, "Spanish Mexican"));
    this->Map.append(QPair<int, QString>(52, "Hungarian"));
    this->Map.append(QPair<int, QString>(53, "Chinese Traditional"));
    this->Map.append(QPair<int, QString>(54, "Vietnamese"));
    this->Map.append(QPair<int, QString>(56, "Albanian"));
}

Languages* Languages::instance()
{
    if (languages == nullptr) {
        languages = new Languages;
    }
    return languages;
}

void Languages::release()
{
    delete languages;
    languages = nullptr;
}

QString Languages::getLanguage(int value)
{
    // Default
    QString language = "< unknown >";

    // Parse the map
    for (int i = 0; i < this->Map.count(); i++) {
        if (this->Map[i].first == value) {
            language = this->Map[i].second;
            break;
        }
    }

    return language;
}
