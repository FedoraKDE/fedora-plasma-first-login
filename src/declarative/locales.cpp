/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "locales.h"

#include <KLocale>
#include <KConfig>
#include <KConfigGroup>

LocaleAttached::LocaleAttached(QObject* parent)
    : QObject(parent)
{
}

QStringList LocaleAttached::allLanguagesList() const
{
    return KLocale::global()->allLanguagesList();
}

QStringList LocaleAttached::allCountriesList() const
{
    return KLocale::global()->allCountriesList();
}

QStringList LocaleAttached::installedLanguages() const
{
    return KLocale::global()->installedLanguages();
}

QString LocaleAttached::languageCodeToName(const QString& code) const
{
    return KLocale::global()->languageCodeToName(code);
}

QString LocaleAttached::countryCodeToName(const QString& code) const
{
    return KLocale::global()->countryCodeToName(code);
}

void LocaleAttached::setLanguage(const QString& languageName)
{
    KLocale::global()->setLanguage(languageName, 0);
}

QStringList LocaleAttached::languagesForCountry(const QString& countryCode)
{
    const QString localeConfig = QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                                        QString::fromLatin1("locale/l10n/%1/country.desktop").arg(countryCode.toLower()));
    if (localeConfig.isEmpty()) {
        return QStringList();
    }

    KConfig cfg(localeConfig);
    const KConfigGroup cfgGroup(&cfg, "KCM Locale");
    return cfgGroup.readEntry("Languages", QStringList());
}

QString LocaleAttached::flagForCountry(const QString& countryCode)
{
    return QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                  QString::fromLatin1("kf5/locale/countries/%1/flag.png").arg(countryCode.toLower()));
}


Locale::Locale(QObject* parent)
    : QObject(parent)
{
}

LocaleAttached* Locale::qmlAttachedProperties(QObject* parent)
{
    return new LocaleAttached(parent);
}
