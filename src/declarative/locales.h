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

#ifndef LOCALES_H
#define LOCALES_H

// NOTE: This file cannot be called locale.h, because it conflicts with glibc's locale.h
// and causes compilation errors.

#include <QObject>
#include <QStringList>
#include <QtQml>

class LocaleAttached: public QObject
{
    Q_OBJECT
  protected:
    explicit LocaleAttached(QObject *parent = 0);

  public:
    Q_INVOKABLE QStringList allLanguagesList() const;
    Q_INVOKABLE QString languageCodeToName(const QString &code) const;
    Q_INVOKABLE QStringList installedLanguages() const;

    Q_INVOKABLE QStringList languagesForCountry(const QString &countryCode);

    Q_INVOKABLE void setLanguage(const QString &languageName);

  friend class Locale;
};

/**
 * A simplified wrapper around KLocale.
 *
 * The wrapper is implemented as an attached property, which makes it possible
 * to use it as if it's method were static:
 *
 * @code
 * var langs = GlobalLocale.allLanguagesList();
 * @endcode
 */
class Locale : public QObject
{
    Q_OBJECT
  public:
    explicit Locale(QObject *parent = 0);

    static LocaleAttached* qmlAttachedProperties(QObject *parent);
};

QML_DECLARE_TYPEINFO(Locale, QML_HAS_ATTACHED_PROPERTIES)

#endif // LOCALES_H
