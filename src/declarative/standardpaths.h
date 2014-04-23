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

#ifndef STANDARDPATHS_H
#define STANDARDPATHS_H

#include <QObject>
#include <QtQml>

class StandardPathsAttached: public QObject
{
    Q_OBJECT
  protected:
    explicit StandardPathsAttached(QObject *parent = 0);

  public:
    Q_INVOKABLE QString appData(const QString &resource, bool isFile = true);
    Q_INVOKABLE QString avatarsDir();

  friend class StandardPaths;
};


class StandardPaths : public QObject
{
    Q_OBJECT
  public:
    explicit StandardPaths(QObject* parent = 0);

    static StandardPathsAttached* qmlAttachedProperties(QObject *parent);
};

QML_DECLARE_TYPEINFO(StandardPaths, QML_HAS_ATTACHED_PROPERTIES)

#endif // STANDARDPATHS_H
