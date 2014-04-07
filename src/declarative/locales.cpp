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

#include <KGlobal>
#include <KLocale>
#include <KConfig>
#include <KConfigGroup>

LocaleAttached::LocaleAttached(QObject* parent)
    : QObject(parent)
{
}

QStringList LocaleAttached::allLanguagesList() const
{
    return KGlobal::locale()->allLanguagesList();
}

QStringList LocaleAttached::installedLanguages() const
{
    return KGlobal::locale()->installedLanguages();
}

QString LocaleAttached::languageCodeToName(const QString& code) const
{
    return KGlobal::locale()->languageCodeToName(code);
}

void LocaleAttached::setLanguage(const QString& languageName)
{
    KGlobal::locale()->setLanguage(languageName, 0);
}

QStringList LocaleAttached::languagesForCountry(const QString& countryCode)
{
    const QString localeConfig = QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                                        QString::fromLatin1("locale/l10n/%1/entry.desktop").arg(countryCode.toLower()));
    if (localeConfig.isEmpty()) {
        return QStringList();
    }

    KConfig cfg(localeConfig);
    const KConfigGroup cfgGroup(&cfg, "KCM Locale");
    return cfgGroup.readEntry("Languages", QStringList());
}


Locale::Locale(QObject* parent)
    : QObject(parent)
{
}

LocaleAttached* Locale::qmlAttachedProperties(QObject* parent)
{
    return new LocaleAttached(parent);
}

