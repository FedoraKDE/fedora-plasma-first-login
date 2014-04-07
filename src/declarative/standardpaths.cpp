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

#include "standardpaths.h"

#include <QStandardPaths>

StandardPathsAttached::StandardPathsAttached(QObject* parent)
    : QObject(parent)
{
}

QString StandardPathsAttached::appData(const QString& resource, bool isFile)
{
    return QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                  QLatin1String("fedora-plasma-first-login/") + resource,
                                  isFile ? QStandardPaths::LocateFile
                                         : QStandardPaths::LocateDirectory);
}

StandardPaths::StandardPaths(QObject* parent)
    : QObject(parent)
{
}

StandardPathsAttached* StandardPaths::qmlAttachedProperties(QObject* parent)
{
    return new StandardPathsAttached(parent);
}

